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
// ild_header.cpp is part of LaserBoy.
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
#include <LaserBoy/ild_header.hpp>

namespace LaserBoy {

//############################################################################
bool ILDHeader::from_ifstream_ild(ifstream& in, long int& bytes_skipped)
{
    char      a, b, c, d;
    int       i;
    long int  in_position_was,
              in_position_now,
              bytes_total,
              bytes_left,
              bytes_read;
    //------------------------------------------------------------------------
    name.erase ();
    owner.erase();
    quantity = 0;
    identity = 0;
    total    = 0;
    scanner  = 0;
    future   = 0;
    //------------------------------------------------------------------------
    in_position_was = in.tellg();
    in.seekg (0, in.end);
    bytes_total = in.tellg();
//    cout << "bytes_total = " << bytes_total << ENDL;
    in.seekg(in_position_was);
    bytes_left = bytes_total - in_position_was;
    //------------------------------------------------------------------------
    if(bytes_total == 0)
    {
//        cout << "bytes_total == 0 " << ENDL;
        bytes_skipped = -2;
        return false;
    }
    //------------------------------------------------------------------------
    if(bytes_left == 0)
    {
//        cout << "bytes_left == 0 " << ENDL;
        return false;
    }
    //------------------------------------------------------------------------
    if(bytes_left >= 32)
    {
        do
        {
            d = ' ';
            while(d != 'A')
            {
                c = ' ';
                while(c != 'D')
                {
                    b = ' ';
                    while(b != 'L')
                    {
                        a = ' ';
                        while(a != 'I')
                            if(!in.get(a)) goto on_eof_error; // bottom of this function!
                        if(!in.get(b)) goto on_eof_error;
                    }
                    if(!in.get(c)) goto on_eof_error;
                }
                if(!in.get(d)) goto on_eof_error;
            }
            //----------------------------------------------------------------
            if(!in.get(a)) goto on_eof_error;
            if(!in.get(b)) goto on_eof_error;
            if(!in.get(c)) goto on_eof_error;
            if(!in.get(d)) goto on_eof_error;
            i = (   (a & 0x000000ff) << 24
                  | (b & 0x000000ff) << 16
                  | (c & 0x000000ff) << 8
                  | (d & 0x000000ff)
                );
        //--------------------------------------------------------------------
        } while(    i != LASERBOY_3D_FRAME     // 0
                 && i != LASERBOY_2D_FRAME     // 1
                 && i != LASERBOY_PALETTE      // 2
                 && i != LASERBOY_TABLE        // 3
                 && i != LASERBOY_3D_FRAME_RGB // 4
                 && i != LASERBOY_2D_FRAME_RGB // 5
               );
        //--------------------------------------------------------------------
        in_position_now = in.tellg();
        bytes_read = (in_position_now - in_position_was);
        if(bytes_read > 8)
        {
            bytes_skipped += (bytes_read - 8);
//            cout << "bytes_read > 8 " << bytes_skipped << ENDL;
        }
        //--------------------------------------------------------------------
        format = i;
        for(i = 0; i < 8; i++)
        {
            if(!in.get(a)) goto on_eof_error;
            name += a;
        }
        //************************************************
        // Code to recognise and read BUSTED Format 3
        //************************************************
        int numV3bytes  =   (u_char(name[0]) << 24)
                          + (u_char(name[1]) << 16)
                          + (u_char(name[2]) << 8)
                          +  u_char(name[3]) - 4;

        int numV3colors =   (u_char(name[4]) << 24)
                          + (u_char(name[5]) << 16)
                          + (u_char(name[6]) << 8)
                          +  u_char(name[7]);
        if(numV3bytes == numV3colors * 3)
        {
            quantity = numV3colors;
        }
        //************************************************
        // end BUSTED Format 3
        //************************************************
        else // if(numV3bytes != numV3colors * 3)
        {
            for(i = 0; i < 8; i++)
            {
                if(!in.get(a)) goto on_eof_error;
                owner += a;
            }

            if(!in.get(a)) goto on_eof_error;
            if(!in.get(b)) goto on_eof_error;
            quantity = ((a & 0x00ff) << 8 | (b & 0x00ff));

            if(!in.get(a)) goto on_eof_error;
            if(!in.get(b)) goto on_eof_error;
            identity = ((a & 0x00ff) << 8 | (b & 0x00ff));

            if(!in.get(a)) goto on_eof_error;
            if(!in.get(b)) goto on_eof_error;
            total    = ((a & 0x00ff) << 8 | (b & 0x00ff));

            if(!in.get(scanner)) goto on_eof_error;
            if(!in.get(future )) goto on_eof_error;
        /*
            cout << "format   = " << (int)format << ENDL;
            cout << "name     = " << name          << ENDL;
            cout << "owner    = " << owner         << ENDL;
            cout << "quantity = " << quantity      << ENDL;
            cout << "identity = " << identity      << ENDL;
            cout << "total    = " << total         << ENDL;
            cout << "scanner  = " << (int)scanner  << ENDL;
            cout << "future   = " << (int)future   << ENDL;
            cout << ENDL << ENDL;
        */
            //----------------------------------------------------------------
        }
        return true;
    } // end if(bytes_left >= 32)
    else
    {
        bytes_skipped += bytes_left;
//        cout << "bytes_left < 32 " << bytes_left << ENDL;
        return false;
    }

on_eof_error:
    bytes_skipped += (bytes_total - in_position_was);
//    cout << "on_eof_error called " << bytes_skipped << ENDL;
    return false;
}

//############################################################################
void ILDHeader::to_ofstream_ild(ofstream& out, bool dumb_file_format)
{
    u_int i;
    //------------------------------------------------------------------------
    out.put('I');
    out.put('L');
    out.put('D');
    out.put('A');
    out.put( 0 );
    out.put( 0 );
    out.put( 0 );
    //------------------------------------------------------------------------
    if(dumb_file_format)
    {
        if(format == LASERBOY_3D_FRAME)
            format = LASERBOY_3D_FRAME_RGB;

        else if(format == LASERBOY_2D_FRAME)
            format = LASERBOY_2D_FRAME_RGB;
    }
    out.put(format);
    //------------------------------------------------------------------------
    for(i = 0; i < 8; i++)
        out.put(name[i]);

    for(i = 0; i < 8; i++)
        out.put(owner[i]);

    out.put((char)((quantity & 0xff00) >> 8));
    out.put((char) (quantity & 0x00ff)      );

    out.put((char)((identity & 0xff00) >> 8));
    out.put((char) (identity & 0x00ff)      );

    out.put((char)((total    & 0xff00) >> 8));
    out.put((char) (total    & 0x00ff)      );

    out.put(scanner);
    out.put(future) ;
    //------------------------------------------------------------------------
    return;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
