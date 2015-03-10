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
// real_vertex.hpp is part of LaserBoy.
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
#include <LaserBoy/color.hpp>
#include <LaserBoy/3D_double.hpp>

namespace LaserBoy {

//############################################################################
class RealVertex : public Double3d, public Color
{
public:
    RealVertex(const double&  _x = 0,
                         const double&  _y = 0,
                         const double&  _z = 0,
                         const u_char&  _r = 0,
                         const u_char&  _g = 0,
                         const u_char&  _b = 0,
                         const u_char&  _k = LASERBOY_BLANKING_BIT,
                         const u_char&  _c = 0
                        )
                  : Double3d (_x, _y, _z),
                    Color     (_r, _g, _b),
                    k                  (_k        ),
                    c                  (_c        )             {}
    //------------------------------------------------------------------------
    RealVertex(const RealVertex& p)
                  : Double3d (p.x, p.y, p.z),
                    Color     (p.r, p.g, p.b),
                    k                  (p.k          ),
                    c                  (p.c          )          {}
    //------------------------------------------------------------------------
    RealVertex(const Double3d& f,
                         const Color&     rgb,
                         const u_char&             _k = LASERBOY_BLANKING_BIT,
                         const u_char&             _c = 0
                        )
                  : Double3d (f  ),
                    Color     (rgb),
                    k                  (_k ),
                    c                  (_c )                    {}
    //------------------------------------------------------------------------
    RealVertex(const Double3d& f,
                         const u_char&             _c
                        )
                  : Double3d (f),
                    Color     (0, 0, 0),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (_c)                     {}
    //------------------------------------------------------------------------
    RealVertex(const Double3d& f)
                  : Double3d (f),
                    Color     (0, 0, 0),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (0)                      {}
    //------------------------------------------------------------------------
    RealVertex( const Color& rgb)
                  : Double3d (0, 0, 0),
                    Color     (rgb),
                    k                  (LASERBOY_BLANKING_BIT),
                    c                  (0)                      {}
    //------------------------------------------------------------------------
    RealVertex& operator = (const RealVertex& p)
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
    RealVertex& operator = (const Double3d& f) // only assign the coordinates
                    {
                        x = f.x;
                        y = f.y;
                        z = f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    RealVertex  operator + (const Double3d& f)
                    {
                        RealVertex sum(*this);
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
    RealVertex& operator += (const Double3d& f)
                    {
                        x += f.x;
                        y += f.y;
                        z += f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    RealVertex  operator - (const Double3d& f)
                    {
                        RealVertex diff(*this);
                        diff.x -= f.x;
                        diff.y -= f.y;
                        diff.z -= f.z;
                        return diff;
                    }
    //------------------------------------------------------------------------
    RealVertex& operator -= (const Double3d& f)
                    {
                        x -= f.x;
                        y -= f.y;
                        z -= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    RealVertex& operator *= (const Double3d& f)
                    {
                        x *= f.x;
                        y *= f.y;
                        z *= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    RealVertex operator * (const Double3d& f)
                    {
                        RealVertex product;
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
    bool operator == (const RealVertex& p) const
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
    bool operator != (const RealVertex& p) const
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
    RealVertex operator - ()
                    {
                        RealVertex negative;
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
    RealVertex& blank  () { k |=  LASERBOY_BLANKING_BIT; return *this; }
    RealVertex& unblank() { k &= ~LASERBOY_BLANKING_BIT; return *this; }
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
    Double3d as_3D_double() const {  return (Double3d)(*this); }
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
    Color as_Color() const
            {
                return (Color)(*this);
            }
    //------------------------------------------------------------------------
    u_char  k, c;
};

//############################################################################
class RealSegmentBase : public vector<RealVertex>
{
public:
    //------------------------------------------------------------------------
    RealSegmentBase()
            {}
    //------------------------------------------------------------------------
virtual
   ~RealSegmentBase()
            { clear(); }
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
