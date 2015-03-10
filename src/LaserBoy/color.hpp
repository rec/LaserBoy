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
// color.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_COLOR_DEFINITIONS__
#define __LASERBOY_COLOR_DEFINITIONS__

#include <LaserBoy/common.hpp>

namespace LaserBoy {

//############################################################################
class Color
{
public:
    Color( const u_char& _r = 0,
                    const u_char& _g = 0,
                    const u_char& _b = 0
                  )
            : r(_r), g(_g), b(_b)                      {}
    //------------------------------------------------------------------------
    Color(const Color& c)
            : r(c.r), g(c.g), b(c.b)                   {}
    //------------------------------------------------------------------------
    Color(const u_int& c)
            : r((c & 0x00ff0000) >> 16)
            , g((c & 0x0000ff00) >>  8)
            , b((c & 0x000000ff)      )                {}
    //------------------------------------------------------------------------
   ~Color()                                   {}
    //------------------------------------------------------------------------
    Color& operator = (const Color& c)
                {
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    return *this;
                }
    //------------------------------------------------------------------------
    Color& operator = (const u_char& c)
                {
                    r = c;
                    g = c;
                    b = c;
                    return *this;
                }
    //------------------------------------------------------------------------
    Color& operator = (const u_int& i)
                {
                    r = (i & 0x00ff0000) >> 16;
                    g = (i & 0x0000ff00) >>  8;
                    b = (i & 0x000000ff)      ;
                    return *this;
                }
    //------------------------------------------------------------------------
    bool operator == (const Color& c) const
            {
                return(as_uint() == c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator != (const Color& c) const
            {
                return(as_uint() != c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator >  (const Color& c) const
            {
                return(as_uint() > c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator >= (const Color& c) const
            {
                return(as_uint() >= c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator <  (const Color& c) const
            {
                return(as_uint() < c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator <=  (const Color& c) const
            {
                return(as_uint() <= c.as_uint());
            }
    //------------------------------------------------------------------------
    bool is_black()
            {
                return (r + g + b) == 0;
            }
    //------------------------------------------------------------------------
    bool is_color(int black_level = 0) const
            {
                return ((r + g + b) > black_level);
            }
    //------------------------------------------------------------------------
    bool from_ifstream_ild (ifstream& in)
            {
                r = (u_char)in.get();
                g = (u_char)in.get();
                b = (u_char)in.get();
                return in.good();
            }
    //------------------------------------------------------------------------
    void  to_ofstream_ild (ofstream& out) const
            {
                out.put((char)r);
                out.put((char)g);
                out.put((char)b);
                return;
            }
    //------------------------------------------------------------------------
    bool from_ifstream_txt (ifstream& in, const u_int& group_type, u_int& line_number)
            {
                bool     got_data = false;
                u_int    next_char;
                double   number;
                clear_to_token(in, next_char, line_number);
                if(isdigit(next_char))
                {
                    if(get_next_number(in, number, line_number))
                    {
                        got_data = true;
                        if(group_type == LASERBOY_HEX)
                        {
                            r = (u_char)((((u_int)number) & 0x00ff0000) >> 16);
                            g = (u_char)((((u_int)number) & 0x0000ff00) >>  8);
                            b = (u_char)((((u_int)number) & 0x000000ff)      );
                        }
                        else
                        {
                            r = (u_char)number;
                            if(get_next_number(in, number, line_number))
                            {
                                g = (u_char)number;
                                if(get_next_number(in, number, line_number))
                                    b = (u_char)number;
                            }

                        }
                    }
                }
                return (in.good() && got_data);
            }
    //------------------------------------------------------------------------
    void  to_ofstream_txt (ofstream& out, const bool& save_as_hex) const
            {
                if(save_as_hex)
                {
                    out << "    0x"
                        << hex
                        << setw(2) << setfill('0') << (int)r
                        << setw(2) << setfill('0') << (int)g
                        << setw(2) << setfill('0') << (int)b
                        << dec
                        << setfill(' ')
                        << ENDL;
                }
                else
                    out << setw(7) << (int)r
                        << setw(7) << (int)g
                        << setw(7) << (int)b
                        << ENDL;
                return;
            }
    //------------------------------------------------------------------------
    u_int as_uint  () const {    return(   r    << RED_BIT_SHIFT
                                         | g    << GREEN_BIT_SHIFT
                                         | b    << BLUE_BIT_SHIFT
                                         | 0xff << ALPHA_BIT_SHIFT
                                       );
                            }
    //------------------------------------------------------------------------
    u_int intensity() const {    return(r + g + b) / 3;           }
    //------------------------------------------------------------------------
    Color  blend(const Color& c, double ratio)
                {
                    Color blended;
                    blended.r = (u_char)(((1.0 - ratio) * r) + (ratio * c.r) + 0.5);
                    blended.g = (u_char)(((1.0 - ratio) * g) + (ratio * c.g) + 0.5);
                    blended.b = (u_char)(((1.0 - ratio) * b) + (ratio * c.b) + 0.5);
                    return blended;
                }
    //------------------------------------------------------------------------
    Color& shade(const u_char& s)
                {
                    (r - s > 0) ? (r -= s) : (r = 0);
                    (g - s > 0) ? (g -= s) : (g = 0);
                    (b - s > 0) ? (b -= s) : (b = 0);
                    return *this;
                }
    //------------------------------------------------------------------------
    Color& tint(const u_char& t)
                {
                    (r + t <= 255) ? (r += t) : (r = 255);
                    (g + t <= 255) ? (g += t) : (g = 255);
                    (b + t <= 255) ? (b += t) : (b = 255);
                    return *this;
                }
    //------------------------------------------------------------------------
    u_char r, g, b;
};

//############################################################################
class PaletteBase : public vector<Color>
{
public:
    //------------------------------------------------------------------------
    PaletteBase()
            {}
    //------------------------------------------------------------------------
virtual
   ~PaletteBase()
            { clear(); }
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
