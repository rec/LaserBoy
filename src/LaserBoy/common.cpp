//############################################################################
//
// LaserBoy !!!
//
// by James Lehman
// Extra Stimulus Inc.
// james@akrobiz.com
//
// began: October 2003
//
// Copyright 2003, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 2015 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// LaserBoy_common.cpp is part of LaserBoy.
//
// LaserBoy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LaserBoy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LaserBoy. If not, see <http://www.gnu.org/licenses/>.
//
//############################################################################
#include <LaserBoy/common.hpp>
#include <boost/asio.hpp>

//############################################################################
const double pi                = (4 * atan(1.0)) ;
const double two_pi            = (pi * 2.0)      ;
const double quarter_pi        = (pi / 4.0)      ;
const double half_pi           = (pi / 2.0)      ;
const double three_quarters_pi = (3 * quarter_pi);
const double one_degree        = (pi / 180.0)    ;
const double one_radian        = (180.0 / pi)    ;

//############################################################################
const u_short short_bit_mask[16] =
{
    (u_short)0xffff, // 1111 1111 1111 1111
    (u_short)0xfffe, // 1111 1111 1111 1110
    (u_short)0xfffc, // 1111 1111 1111 1100
    (u_short)0xfff8, // 1111 1111 1111 1000

    (u_short)0xfff0, // 1111 1111 1111 0000
    (u_short)0xffe0, // 1111 1111 1110 0000
    (u_short)0xffc0, // 1111 1111 1100 0000
    (u_short)0xff80, // 1111 1111 1000 0000

    (u_short)0xff00, // 1111 1111 0000 0000
    (u_short)0xfe00, // 1111 1110 0000 0000
    (u_short)0xfc00, // 1111 1100 0000 0000
    (u_short)0xf800, // 1111 1000 0000 0000

    (u_short)0xf000, // 1111 0000 0000 0000
    (u_short)0xe000, // 1110 0000 0000 0000
    (u_short)0xc000, // 1100 0000 0000 0000
    (u_short)0x8000  // 1000 0000 0000 0000
};
//############################################################################
//                 delta_02, delta_01, delta_12,
double delta_angle(double a, double b, double c) // sss triangle
{                       // returns the simple angle in the three vertices 0, 1, 2
    double angle;
    //------------------------------------------------------------------------
    if(    b == 0.0
        || c == 0.0
      )
        angle = 0;
    //------------------------------------------------------------------------
    else if(a == 0.0)
        angle = pi;
    //------------------------------------------------------------------------
    else if(a >= b + c)
        angle = 0.0;
    //------------------------------------------------------------------------
    else
        angle = pi - acos((b*b + c*c - a*a)/(2*b*c));
    //------------------------------------------------------------------------
    return angle;
}

//############################################################################
double simple_2D_angle(double x1, double y1, double x2, double y2)
{
    return atan2(y2 - y1, x2 - x1);
}

//############################################################################
u_char rescale_to_index(u_short* rescale, u_short value)
{
    u_int i;
    value &= 0xfffe; // remove possible LSB tag
    for(i = 0; i < 256; i++)
        if(rescale[i] == value)
            return (u_char)i;
    return 0x00;
}


//############################################################################
int greatest_common_devisor(int x, int y)
{
    x = abs(x);
    y = abs(y);
    int z;
    while(y != 0)
    {
        z = x % y;
        x = y;
        y = z;
    }
    return x;
}

//############################################################################
int lowest_common_denominator(int x, int y)
{
  int z,
      min = x > y ? y : x;
  for(z = 2; z < min; z++)
    if(((x % z) == 0) && ((y % z) == 0))
       break;
  if(z == min)
    return y;
  return z;
}

//############################################################################
bool file_exists(const string& file)
{
    ifstream in;
    in.open(file.c_str(), ios::in);
    if(in.is_open())
    {
        in.close();
        return true;
    }
    return false;
}

//############################################################################
bool directory_exists(const string& dir)
{
    if(chdir(dir.c_str()) == 0) // success!
    {
        chdir("../");
        return true;
    }
    return false;
}

//############################################################################
string time_as_string(double seconds)
{
    if(seconds >= 0)
    {
        char   buffer[256];
        string time_string;
        int    hh,
               mm,
               ss;
        double sec;

        hh  = (int)(seconds / 3600);
        mm  = (int)((seconds - hh * 3600) / 60);
        ss  = (int)(seconds - (hh * 3600 + mm * 60));
        sec = seconds - (hh * 3600 + mm * 60 + ss);

        sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
        time_string = buffer;
        sprintf(buffer, "%lf", sec);
        time_string += &(buffer[1]); // ignore the leading zero!
        return time_string;
    }
    return string("??:??:??");
}

//############################################################################
double random_01() // returns a double between 0 & 1.
{
    return ((double)(rand()) / (double)RAND_MAX);
}

//############################################################################
double random_neg_to_pos_1() // returns a double between -1 & 1.
{
    return random_01() - random_01();
}

//############################################################################
void txt_tag(ofstream& out)
{
    out << "# This file was generated by " LASERBOY_VERSION " !!!\n"
           "#\n"
           "# the free, multiplatform laser display application\n"
           "# by James Lehman <james@akrobiz.com>\n"
           "# Extra Stimulus Inc., Akron, Ohio USA\n"
           "# http://laserboy.org/\n\n"

           "# ASCII format version: " LASERBOY_TXT_VERSION "\n"
        << ENDL;
    return;
}

//############################################################################
bool get_dxf_pair(ifstream& in, int& group_code, string& entity_string)
{
    char line[256];
    in >> group_code;
    if(group_code != -1)
    {
        while(isspace(in.peek()))
            in.get(); // eat the '\r''\n'

        in.getline(line, 255);
        entity_string = line;

        int i = entity_string.size() - 1;
        while(!isgraph(entity_string[i]))
            i--;

        entity_string = entity_string.substr(0, i + 1);
    }
    return (group_code != 0 && in.good());
}

//############################################################################
bool get_dxf_pair(ifstream& in, int& group_code, char entity_string[256])
{
    in >> group_code;
    if(group_code != -1)
    {
        while(isspace(in.peek()))
            in.get(); // eat the '\r''\n'
        in.getline(entity_string, 255);
    }
    return (group_code != 0 && in.good());
}

//############################################################################
bool clear_to_alpha(ifstream& in, u_int& line_number)
{
    u_int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(!isalpha(next_char))
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
            }
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is alpha
            break;
    }
    return in.good();
}

//############################################################################
bool clear_to_digit(ifstream& in, u_int& line_number)
{
    u_int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(    !isdigit(next_char)
            && next_char != '.'
            && next_char != '-'
            && next_char != '+'
          )
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
            }
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is a digit or '.' or '-'
            break;
    }
    return in.good();
}

//############################################################################
bool clear_to_token(ifstream& in, u_int& next_char, u_int& line_number)
{
    while(in.good())
    {
        next_char = in.peek();
        if(    !isalnum(next_char)
            && next_char != '.'
            && next_char != '-'
            && next_char != '+'
          )
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
            }
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is alpha or digit or '.' or '-'
            break;
    }
    return in.good();
}

//############################################################################
bool get_next_word(ifstream& in, string& word, u_int& line_number)
{
    u_int next_char = '\0';
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
                return false;
            }
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(    isalpha(next_char)
        || next_char == '-'
        || next_char == '_'
      )
    {
        in >> word;
        return true;
    }
    return false;
}

//############################################################################
bool get_next_number(ifstream& in, double& number, u_int& line_number)
{
    u_int next_char = '\0';
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
                return false;
            }
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(    isdigit(next_char)
        || next_char == '+'
        || next_char == '-'
        || next_char == '.'
      )
    {
        in >> number;
        return true;
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
bool get_next_hex(ifstream& in, double& number, u_int& line_number)
{
    u_int next_char = '\0';
    string token;
    char** junk = NULL;
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
                return false;
            }
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(    isxdigit(next_char)
        || next_char == '+'
        || next_char == '-'
      )
    {
        in >> token;
        if(token == "-1")
            number = -1;
        else
            number = (int)(strtol(token.c_str(), junk, 16));
        return true;
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
int LaserBoy_version_check(string& version_string, string install_GUID, u_int app_runs_count)
{
    using boost::asio::ip::tcp;
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("laserboy.org", "http");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "POST /cgi-bin/laserboy_version?"
                       << LASERBOY_VERSION
                       << '+'
                       << install_GUID
                       << '+'
                       << app_runs_count
                       << " HTTP/1.0\r\n";
        //--------------------------------------------------------------------
        request_stream << "Host: laserboy.org\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
        //--------------------------------------------------------------------
        boost::asio::write(socket, request);
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        u_int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if(!response_stream || http_version.substr(0, 5) != "HTTP/")
            return 1;
        if(status_code != 200)
            return 1;
        boost::asio::read_until(socket, response, "\r\n\r\n");
        std::string header;
        while(std::getline(response_stream, header) && header != "\r")
        {}
        std::ostringstream ostringstream_buffer;
        string version_string_buffer;
        if(response.size() > 0)
        {
            ostringstream_buffer << &response;
            version_string_buffer = ostringstream_buffer.str();
        }
        for(u_int i = 0; i < version_string_buffer.size(); i++)
            if(!strchr(" \t\r\n\0", version_string_buffer.at(i)))
                version_string += version_string_buffer.at(i);
        boost::system::error_code error;
        while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
            std::cout << &response;
        if(error != boost::asio::error::eof)
            throw boost::system::system_error(error);
    }
    catch(std::exception& e)
    {
        return 1;
    }
    return 0;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
