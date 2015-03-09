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
// LaserBoy_real_vertex.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_3D_REAL_DEFINITIONS__
#define __LASERBOY_3D_REAL_DEFINITIONS__

//############################################################################
#include "LaserBoy_color.hpp"
#include "LaserBoy_3D_double.hpp"

//############################################################################
class LaserBoy_real_vertex : public LaserBoy_3D_double, public LaserBoy_color
{
public:
    LaserBoy_real_vertex(const double&  _x = 0,
                         const double&  _y = 0,
                         const double&  _z = 0,
                         const u_char&  _r = 0,
                         const u_char&  _g = 0,
                         const u_char&  _b = 0,
                         const u_char&  _k = LASERBOY_BLANKING_BIT,
                         const u_char&  _c = 0
                        )
                  : LaserBoy_3D_double (_x, _y, _z),
                    LaserBoy_color     (_r, _g, _b),
                    k                  (_k        ),
                    c                  (_c        )             {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex(const LaserBoy_real_vertex& p)
                  : LaserBoy_3D_double (p.x, p.y, p.z),
                    LaserBoy_color     (p.r, p.g, p.b),
                    k                  (p.k          ),
                    c                  (p.c          )          {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex(const LaserBoy_3D_double& f,
                         const LaserBoy_color&     rgb,
                         const u_char&             _k = LASERBOY_BLANKING_BIT,
                         const u_char&             _c = 0
                        )
                  : LaserBoy_3D_double (f  ),
                    LaserBoy_color     (rgb),
                    k                  (_k ),
                    c                  (_c )                    {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex(const LaserBoy_3D_double& f,
                         const u_char&             _c
                        )
                  : LaserBoy_3D_double (f),
                    LaserBoy_color     (0, 0, 0),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (_c)                     {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex(const LaserBoy_3D_double& f)
                  : LaserBoy_3D_double (f),
                    LaserBoy_color     (0, 0, 0),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (0)                      {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex( const LaserBoy_color& rgb)
                  : LaserBoy_3D_double (0, 0, 0),
                    LaserBoy_color     (rgb),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (0)                      {}
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& operator = (const LaserBoy_real_vertex& p)
                    {
                        x = p.x;
                        y = p.y;
                        z = p.z;
                        r = p.r;
                        g = p.g;
                        b = p.b;
                        k = p.k;
                        c = p.c;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& operator = (const LaserBoy_3D_double& f) // only assign the coordinates
                    {
                        x = f.x;
                        y = f.y;
                        z = f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex  operator + (const LaserBoy_3D_double& f)
                    {
                        LaserBoy_real_vertex sum(*this);
                        sum.x += f.x;
                        sum.y += f.y;
                        sum.z += f.z;
                        sum.k =  k;
                        sum.c =  c;
                        sum.r =  r;
                        sum.g =  g;
                        sum.b =  b;
                        return sum;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& operator += (const LaserBoy_3D_double& f)
                    {
                        x += f.x;
                        y += f.y;
                        z += f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex  operator - (const LaserBoy_3D_double& f)
                    {
                        LaserBoy_real_vertex diff(*this);
                        diff.x -= f.x;
                        diff.y -= f.y;
                        diff.z -= f.z;
                        return diff;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& operator -= (const LaserBoy_3D_double& f)
                    {
                        x -= f.x;
                        y -= f.y;
                        z -= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& operator *= (const LaserBoy_3D_double& f)
                    {
                        x *= f.x;
                        y *= f.y;
                        z *= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex operator * (const LaserBoy_3D_double& f)
                    {
                        LaserBoy_real_vertex product;
                        product.r = r;
                        product.g = g;
                        product.b = b;
                        product.x = x * f.x;
                        product.y = y * f.y;
                        product.z = z * f.z;
                        product.k = k;
                        product.c = c;
                        return product;
                    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_real_vertex& p) const
            {
                if(    (x != p.x)
                    || (y != p.y)
                    || (z != p.z)
                    || (r != p.r)
                    || (g != p.g)
                    || (b != p.b)
                    || ((k & LASERBOY_BLANKING_BIT) != (p.k & LASERBOY_BLANKING_BIT))
                    || (c != p.c)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator != (const LaserBoy_real_vertex& p) const
            {
                if(    (x == p.x)
                    && (y == p.y)
                    && (z == p.z)
                    && (r == p.r)
                    && (g == p.g)
                    && (b == p.b)
                    && ((k & LASERBOY_BLANKING_BIT) == (p.k & LASERBOY_BLANKING_BIT))
                    && (c == p.c)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex operator - ()
                    {
                        LaserBoy_real_vertex negative;
                        negative.x = -x;
                        negative.y = -y;
                        negative.z = -z;
                        return negative;
                    }
    //------------------------------------------------------------------------
    bool from_ifstream_txt(ifstream& in,
                           const u_int& group_type,
                           u_int& line_number
                          )
        {
            bool    got_data = false;
            u_int   next_char;
            double  number;
            clear_to_token(in, next_char, line_number);
            if(    isdigit(next_char)
                || next_char == '.'
                || next_char == '-'
                || next_char == '+'
              )
            {
                if(get_next_number(in, x, line_number)) // 1
                {
                    got_data = true;
                    if(get_next_number(in, y, line_number)) // 2
                    {
                        // x and y are always the first 2 values.
                        //.................
                        // if there is a z
                        if(    group_type == LASERBOY_3D_FRAME_RGB
                            || group_type == LASERBOY_3D_FRAME_HEX
                            || group_type == LASERBOY_3D_FRAME_PALETTE
                            || group_type == LASERBOY_3D_FRAME_TABLE
                          )
                        {
                            if(get_next_number(in, z, line_number))
                            {
                                // we just got z!
                            }
                            else
                                return in.good();
                        }
                        else
                            z = 0;
                        //.................
                        // color values
                        // if there is an r g b
                        if(    (    (group_type == LASERBOY_3D_FRAME_RGB)
                                 || (group_type == LASERBOY_2D_FRAME_RGB)
                               )
                            && (get_next_number(in, number, line_number))
                          )
                        {
                            if(number == -1) // it's blank
                            {
                                blank();
                                r = g = b = c = 0;
                            }
                            else
                            {
                                unblank();
                                r = (u_char)number;
                                if(get_next_number(in, number, line_number))
                                {
                                    g = (u_char)number;
                                    if(get_next_number(in, number, line_number))
                                        b = (u_char)number;
                                }
                                c = 0;
                            }
                        }
                        //.................
                        // if there is a hex
                        else if(    (    (group_type == LASERBOY_3D_FRAME_HEX)
                                      || (group_type == LASERBOY_2D_FRAME_HEX)
                                    )
                                 && (get_next_hex(in, number, line_number))
                               )
                        {
                            if(number == -1) // it's blank
                            {
                                blank();
                                r = g = b = c = 0;
                            }
                            else
                            {
                                unblank();
                                r = (((int)number) & 0x00ff0000) >> 16;
                                g = (((int)number) & 0x0000ff00) >>  8;
                                b = (((int)number) & 0x000000ff)      ;
                                c = 0;
                            }
                        }
                        //.................
                        // else if there is c (color index to palette)
                        else if(    (    (group_type == LASERBOY_3D_FRAME_PALETTE)
                                      || (group_type == LASERBOY_2D_FRAME_PALETTE)
                                    )
                                 && (get_next_number(in, number, line_number))
                               )
                        {
                            if(number == -1) // it's blank
                            {
                                blank();
                                r = g = b = c = 0;
                            }
                            else
                            {
                                unblank();
                                c = (u_char)number;
                            }
                        }
                    } // end if(get_next_number(in, number, line_number)) // 2
                } // end if(get_next_number(in, number, line_number)) // 1
            }
            return (in.good() && got_data);
        }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex& blank  () { k |=  LASERBOY_BLANKING_BIT; return *this; }
    LaserBoy_real_vertex& unblank() { k &= ~LASERBOY_BLANKING_BIT; return *this; }
    //------------------------------------------------------------------------
    bool  is_blank    () const { return (bool)(k & LASERBOY_BLANKING_BIT);  }
    bool  is_lit      () const { return !is_blank()                      ;  }
    //------------------------------------------------------------------------
    bool  is_black    (int black_level = 0) const
            {
                return (    is_lit()
                         && ((r + g + b) <= black_level)
                       );
            }
    //------------------------------------------------------------------------
    bool  is_color    (int black_level = 0) const
            {
                return (    is_lit()
                         && ((r + g + b) > black_level)
                       );
            }
    //------------------------------------------------------------------------
    bool  is_dark     (int black_level = 0) const
            {
                return (    is_blank()
                         || ((r + g + b) <= black_level)
                       );
            }
    //------------------------------------------------------------------------
    LaserBoy_3D_double as_3D_double() const {  return (LaserBoy_3D_double)(*this); }
    //------------------------------------------------------------------------
    int color_of(int black_level) const
            {
                if(is_black(black_level))
                    return 0;
                return   is_lit()
                       ? (int)(   (r    << RED_BIT_SHIFT  )
                                | (g    << GREEN_BIT_SHIFT)
                                | (b    << BLUE_BIT_SHIFT )
                                | (0x00 << ALPHA_BIT_SHIFT)
                              )
                       : -1; // is blank
            }
    //------------------------------------------------------------------------
    LaserBoy_color as_LaserBoy_color() const
            {
                return (LaserBoy_color)(*this);
            }
    //------------------------------------------------------------------------
    u_char  k, c;
};

//############################################################################
class LaserBoy_real_segment_base : public vector<LaserBoy_real_vertex>
{
public:
    //------------------------------------------------------------------------
    LaserBoy_real_segment_base()
            {}
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_real_segment_base()
            { clear(); }
    //------------------------------------------------------------------------
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
