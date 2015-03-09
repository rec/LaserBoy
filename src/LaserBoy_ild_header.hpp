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
// LaserBoy_ild_header.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_ILD_HEADER_DEFINITIONS__
#define __LASERBOY_ILD_HEADER_DEFINITIONS__

//############################################################################
#include "LaserBoy_common.hpp"

//############################################################################
class LaserBoy_ild_header
{
public:
    LaserBoy_ild_header(const char&    fmt = 0,
                        const string&  n   = "ESI.....",
                        const string&  o   = "LaserBoy",
                        const u_short& q   = 0,
                        const u_short& id  = 0,
                        const u_short& t   = 0,
                        const char&    s   = 0,
                        const char&    f   = 0,
                        const u_int&   h   = LASERBOY_OK
                       )
                  : format           (fmt),
                    name             (n  ),
                    owner            (o  ),
                    quantity         (q  ),
                    identity         (id ),
                    total            (t  ),
                    scanner          (s  ),
                    future           (f  ),
                    ild_header_error (h  )
                  {}
    //------------------------------------------------------------------------
    LaserBoy_ild_header(const LaserBoy_ild_header& header)
                  : format           (header.format          ),
                    name             (header.name            ),
                    owner            (header.owner           ),
                    quantity         (header.quantity        ),
                    identity         (header.identity        ),
                    total            (header.total           ),
                    scanner          (header.scanner         ),
                    future           (header.future          ),
                    ild_header_error (header.ild_header_error)
                  {}
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_ild_header() {}
    //------------------------------------------------------------------------
    LaserBoy_ild_header& operator = (const LaserBoy_ild_header& header)
                            {
                                format           = header.format          ;
                                name             = header.name            ;
                                owner            = header.owner           ;
                                quantity         = header.quantity        ;
                                identity         = header.identity        ;
                                total            = header.total           ;
                                scanner          = header.scanner         ;
                                future           = header.future          ;
                                ild_header_error = header.ild_header_error;
                                return *this;
                            }
    //------------------------------------------------------------------------
    bool  from_ifstream_ild (ifstream& in, long int& bytes_skipped);
    //------------------------------------------------------------------------
    void  to_ofstream_ild   (ofstream& out, bool dumb_format = false);
    //------------------------------------------------------------------------
    char                 format          ;
    string               name            ,
                         owner           ;
    u_short              quantity        ,
                         identity        ,
                         total           ;
    char                 scanner         ,
                         future          ;
    LaserBoy_Error_Code  ild_header_error;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
