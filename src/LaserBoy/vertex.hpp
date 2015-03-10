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
// LaserBoy_vertex.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_3D_VERTEX_DEFINITIONS__
#define __LASERBOY_3D_VERTEX_DEFINITIONS__

//############################################################################
#include <LaserBoy/real_vertex.hpp>
#include <LaserBoy/wave.hpp>

namespace LaserBoy {

//############################################################################
class LaserBoy_vertex : public LaserBoy_3D_short, public LaserBoy_color
{
public:
    LaserBoy_vertex(const short&   _x = 0,
                    const short&   _y = 0,
                    const short&   _z = 0,
                    const u_char&  _r = 255,
                    const u_char&  _g = 255,
                    const u_char&  _b = 255,
                    const u_char&  _k = LASERBOY_BLANKING_BIT,
                    const u_char&  _c = 55
                   )
                 : LaserBoy_3D_short (_x, _y, _z),
                   LaserBoy_color    (_r, _g, _b),
                   k                 (_k        ),
                   c                 (_c        )               {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_vertex& p)
                 : LaserBoy_3D_short (p.x, p.y, p.z),
                   LaserBoy_color    (p.r, p.g, p.b),
                   k                 (p.k          ),
                   c                 (p.c          )            {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_real_vertex& rv) // cast to LaserBoy_vertex from LaserBoy_real_vertex
                 : LaserBoy_3D_short ((short)round(rv.x),
                                      (short)round(rv.y),
                                      (short)round(rv.z)
                                     ),
                   LaserBoy_color    (rv.r, rv.g, rv.b),
                   k                 (rv.k),
                   c                 (rv.c)                     {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_3D_short& s,
                    const LaserBoy_color&    rgb,
                    const u_char&            _k = LASERBOY_BLANKING_BIT,
                    const u_char&            _c = 0
                   )
                 : LaserBoy_3D_short (s  ),
                   LaserBoy_color    (rgb),
                   k                 (_k ),
                   c                 (_c )                      {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_3D_short& s,
                    const u_char&            _c
                   )
                 : LaserBoy_3D_short (s            ),
                   LaserBoy_color    (255, 255, 255),
                   k                 (LASERBOY_BLANKING_BIT),
                   c                 (_c           )            {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_3D_short& s)
                 : LaserBoy_3D_short (s            ),
                   LaserBoy_color    (255, 255, 255),
                   k                 (LASERBOY_BLANKING_BIT),
                   c                 (0            )            {}
    //------------------------------------------------------------------------
    LaserBoy_vertex(const LaserBoy_color& rgb)
                 : LaserBoy_3D_short (0, 0, 0),
                   LaserBoy_color    (rgb    ),
                   k                 (LASERBOY_BLANKING_BIT),
                   c                 (0      )                  {}
    //------------------------------------------------------------------------
    LaserBoy_vertex& operator = (const LaserBoy_vertex& p)
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
    LaserBoy_vertex& operator = (const LaserBoy_3D_short& s) // only assign the coordinates
                    {
                        x = s.x;
                        y = s.y;
                        z = s.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex  operator + (const LaserBoy_3D_short& s)
                        {
                            int X, Y, Z;
                            LaserBoy_vertex sum(*this);
                            X = x + s.x;
                            Y = y + s.y;
                            Z = z + s.z;
                            if(X >= LASERBOY_MAX_SHORT)
                                sum.x = LASERBOY_MAX_SHORT;
                            else if(X <= LASERBOY_MIN_SHORT)
                                sum.x = LASERBOY_MIN_SHORT;
                            else
                                sum.x = X;
                            if(Y >= LASERBOY_MAX_SHORT)
                                sum.y = LASERBOY_MAX_SHORT;
                            else if(Y <= LASERBOY_MIN_SHORT)
                                sum.y = LASERBOY_MIN_SHORT;
                            else
                                sum.y = Y;
                            if(Z >= LASERBOY_MAX_SHORT)
                                sum.z = LASERBOY_MAX_SHORT;
                            else if(Z <= LASERBOY_MIN_SHORT)
                                sum.z = LASERBOY_MIN_SHORT;
                            else
                                sum.z = Z;
                            return sum;
                        }
    //------------------------------------------------------------------------
    LaserBoy_vertex& operator += (const LaserBoy_3D_short& s)
                    {
                        x += s.x;
                        y += s.y;
                        z += s.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex  operator - (const LaserBoy_3D_short& s)
                    {
                        LaserBoy_vertex diff(*this);
                        diff.x -= s.x;
                        diff.y -= s.y;
                        diff.z -= s.z;
                        return diff;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex& operator -= (const LaserBoy_3D_short& s)
                    {
                        x -= s.x;
                        y -= s.y;
                        z -= s.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex& operator *= (const LaserBoy_3D_short& s)
                    {
                        x = (short)(x * s.x);
                        y = (short)(y * s.y);
                        z = (short)(z * s.z);
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex& operator *= (const LaserBoy_3D_double& f)
                    {
                        x = (short)(x * f.x);
                        y = (short)(y * f.y);
                        z = (short)(z * f.z);
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_vertex blend(const LaserBoy_vertex& v, double ratio)
                    {
                        LaserBoy_vertex blended;
                        blended.unblank();
                        blended.x = (short) (((1.0 - ratio) * x) + (ratio * v.x) + 0.5);
                        blended.y = (short) (((1.0 - ratio) * y) + (ratio * v.y) + 0.5);
                        blended.z = (short) (((1.0 - ratio) * z) + (ratio * v.z) + 0.5);
                        blended.r = (u_char)(((1.0 - ratio) * r) + (ratio * v.r) + 0.5);
                        blended.g = (u_char)(((1.0 - ratio) * g) + (ratio * v.g) + 0.5);
                        blended.b = (u_char)(((1.0 - ratio) * b) + (ratio * v.b) + 0.5);
                        return blended;
                    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_vertex& p) const
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
    bool operator != (const LaserBoy_vertex& p) const
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
    LaserBoy_vertex  operator - ()
                    {
                        LaserBoy_vertex negative;
                        negative.x = -x;
                        negative.y = -y;
                        negative.z = -z;
                        return negative;
                    }
    //------------------------------------------------------------------------
    bool is_equal_2D(const LaserBoy_vertex& p) const
            {
                if(    (x != p.x)
                    || (y != p.y)
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
    bool from_ifstream_ild(ifstream& in, char format)
            {
                if(in.good())
                {
                    char p, q;
                    //--------------------------------------------------------
                    in.get(p);
                    in.get(q);
                    x = ((p & 0x00ff) << 8 | (q & 0x00ff));
                    if(x == -32768) x = -32767;
                    //--------------------------------------------------------
                    in.get(p);
                    in.get(q);
                    y = ((p & 0x00ff) << 8 | (q & 0x00ff));
                    if(y == -32768) y = -32767;
                    //--------------------------------------------------------
                    if(    format == LASERBOY_3D_FRAME
                        || format == LASERBOY_3D_FRAME_RGB
                      )
                    {
                        in.get(p);
                        in.get(q);
                        z = ((p & 0x00ff) << 8 | (q & 0x00ff));
                        if(z == -32768) z = -32767;
                    }
                    //--------------------------------------------------------
                    k = (u_char)in.get();
                    if(    format == LASERBOY_3D_FRAME
                        || format == LASERBOY_2D_FRAME
                      )
                        c = (u_char)in.get();
                    else if(    format == LASERBOY_3D_FRAME_RGB
                             || format == LASERBOY_2D_FRAME_RGB
                           )
                    {
                        b = (u_char)in.get();
                        g = (u_char)in.get();
                        r = (u_char)in.get();
                    }
                    //--------------------------------------------------------
                }
                return in.good();
            }
    //------------------------------------------------------------------------
    void to_ofstream_ild(ofstream& out, char format, bool last_vertex) const
            {
                switch(format)
                {
                    case LASERBOY_3D_FRAME:
                             out.put((char)((x & 0xff00) >> 8));
                             out.put((char) (x & 0x00ff)      );
                             out.put((char)((y & 0xff00) >> 8));
                             out.put((char) (y & 0x00ff)      );
                             out.put((char)((z & 0xff00) >> 8));
                             out.put((char) (z & 0x00ff)      );
                             out.put((char)k | ((last_vertex)?(LASERBOY_LAST_VERTEX):(0x00)));
                             out.put((char)c);
                             break;
                    //--------------------------------------------------------
                    case LASERBOY_2D_FRAME:
                             out.put((char)((x & 0xff00) >> 8));
                             out.put((char) (x & 0x00ff)      );
                             out.put((char)((y & 0xff00) >> 8));
                             out.put((char) (y & 0x00ff)      );
                             out.put((char)k | ((last_vertex)?(LASERBOY_LAST_VERTEX):(0x00)));
                             out.put((char)c);
                             break;
                    //--------------------------------------------------------
                    case LASERBOY_3D_FRAME_RGB:
                             out.put((char)((x & 0xff00) >> 8));
                             out.put((char) (x & 0x00ff)      );
                             out.put((char)((y & 0xff00) >> 8));
                             out.put((char) (y & 0x00ff)      );
                             out.put((char)((z & 0xff00) >> 8));
                             out.put((char) (z & 0x00ff)      );
                             out.put((char)k | ((last_vertex)?(LASERBOY_LAST_VERTEX):(0x00)));
                             out.put((char)b);
                             out.put((char)g);
                             out.put((char)r);
                             break;
                    //--------------------------------------------------------
                    case LASERBOY_2D_FRAME_RGB:
                             out.put((char)((x & 0xff00) >> 8));
                             out.put((char) (x & 0x00ff)      );
                             out.put((char)((y & 0xff00) >> 8));
                             out.put((char) (y & 0x00ff)      );
                             out.put((char)k | ((last_vertex)?(LASERBOY_LAST_VERTEX):(0x00)));
                             out.put((char)b);
                             out.put((char)g);
                             out.put((char)r);
                             break;
                    //--------------------------------------------------------
                }
                return;
            }
    //------------------------------------------------------------------------
    bool from_ifstream_txt(ifstream& in,
                           const u_int& group_type,
                           const u_int& element_type,
                           u_int& line_number
                          )
        {
            bool    got_data  = false;
            u_int   next_char = '\0';
            double  number;
            clear_to_token(in, next_char, line_number);
            if(    isdigit(next_char)
                || next_char == '.'
                || next_char == '-'
                || next_char == '+'
              )
            {
                if(group_type == LASERBOY_TABLE)
                {
                    clear();  // this vertex
                    if(element_type == LASERBOY_RGB)
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            got_data = true;
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
                    } // end if(element_type == LASERBOY_RGB)
                    else if(element_type == LASERBOY_HEX)
                    {
                        if(get_next_hex(in, number, line_number))
                        {
                            got_data = true;
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
                    } // end else if(element_type == LASERBOY_HEX)
                } // end if(group_type == LASERBOY_TABLE)
                //.......................................
                else // It is not a table. It's a frame!
                {
                    if(get_next_number(in, number, line_number)) // 1
                    {
                        got_data = true;
                        if(element_type == LASERBOY_UNIT)
                            x = (short)(number * LASERBOY_MAX_SHORT);
                        else
                            x = (short)(number);

                        if(get_next_number(in, number, line_number)) // 2
                        {
                            if(element_type == LASERBOY_UNIT)
                                y = (short)(number * LASERBOY_MAX_SHORT);
                            else
                                y = (short)(number);
                            // x and y are always the first 2 values.
                            //.................
                            // if there is a z
                            if(    group_type == LASERBOY_3D_FRAME_RGB
                                || group_type == LASERBOY_3D_FRAME_HEX
                                || group_type == LASERBOY_3D_FRAME_PALETTE
                                || group_type == LASERBOY_3D_FRAME_TABLE
                              )
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(element_type == LASERBOY_UNIT)
                                        z = (short)(number * LASERBOY_MAX_SHORT);
                                    else
                                        z = (short)(number);

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
                } // end else of if(group_type == LASERBOY_TABLE)
            }
            return (in.good() && got_data);
        }
    //------------------------------------------------------------------------
    LaserBoy_vertex& blank  () { k |=  LASERBOY_BLANKING_BIT;  return *this; }
    LaserBoy_vertex& unblank() { k &= ~LASERBOY_BLANKING_BIT;  return *this; }
    //------------------------------------------------------------------------
    bool is_blank     () const { return (bool)(k & LASERBOY_BLANKING_BIT);   }
    bool is_lit       () const { return !is_blank();                         }
    //------------------------------------------------------------------------
    bool  is_black    (int black_level) const
            {
                return (    is_lit()
                         && ((r + g + b) <= black_level)
                       );
            }
    //------------------------------------------------------------------------
    bool  is_color    (int black_level) const
            {
                return (    is_lit()
                         && ((r + g + b) > black_level)
                       );
            }
    //------------------------------------------------------------------------
    bool  is_dark     (int black_level) const
            {
                return (    is_blank()
                         || ((r + g + b) <= black_level)
                       );
            }
    //------------------------------------------------------------------------
    void  clear()
            {
                x = 0;
                y = 0;
                z = 0;
                r = 0;
                g = 0;
                b = 0;
                k = LASERBOY_BLANKING_BIT;
                c = 0;
            }
    //------------------------------------------------------------------------
    LaserBoy_3D_short as_3D_short() const {  return (LaserBoy_3D_short)(*this); }
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
    LaserBoy_vertex bit_masked(const u_int signal_bit_mask[8]) const
                    {
                        LaserBoy_vertex masked = *this;
                        masked.x &=  short_bit_mask[signal_bit_mask[0]];
                        masked.y &=  short_bit_mask[signal_bit_mask[1]];
                        masked.z &=  short_bit_mask[signal_bit_mask[5]];
                        masked.r &= (short_bit_mask[signal_bit_mask[2]] >> 7);
                        masked.g &= (short_bit_mask[signal_bit_mask[3]] >> 7);
                        masked.b &= (short_bit_mask[signal_bit_mask[4]] >> 7);
                        return masked;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_short bit_masked_position(const u_int signal_bit_mask[8]) const
                        {
                            LaserBoy_3D_short masked = *this;
                            masked.x &= short_bit_mask[signal_bit_mask[0]];
                            masked.y &= short_bit_mask[signal_bit_mask[1]];
                            masked.z &= short_bit_mask[signal_bit_mask[5]];
                            return masked;
                        }
    //------------------------------------------------------------------------
    LaserBoy_color bit_masked_color(const u_int signal_bit_mask[8]) const
                    {
                        LaserBoy_color masked = *this;
                        masked.r &= (short_bit_mask[signal_bit_mask[2]] >> 7);
                        masked.g &= (short_bit_mask[signal_bit_mask[3]] >> 7);
                        masked.b &= (short_bit_mask[signal_bit_mask[4]] >> 7);
                        return masked;
                    }
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    void to_fstream_wave(fstream&               out,
                         LaserBoy_wave_header&  header,
                         const u_int            signal_bit_mask[8],
                         const bool&            end_of_frame,
                         const bool&            unique_frame
                        ) const
    {
        short  xx,
               yy,
               zz,
               color = 0x0000;

        u_char rr = 0x00,
               gg = 0x00,
               bb = 0x00;
        //--------------------------------------------------------------------
        xx = x & short_bit_mask[signal_bit_mask[0]];
        yy = y & short_bit_mask[signal_bit_mask[1]];
        zz = z & short_bit_mask[signal_bit_mask[5]];
        //--------------------------------------------------------------------
        out.put(    xx & 0x00ff      ); // x
        out.put((   xx & 0xff00) >> 8);
        //--------------------------------------------------------------------
        out.put(    yy & 0x00ff      ); // y
        out.put((   yy & 0xff00) >> 8);
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        {
            if(is_blank())
                color = header.color_rescale_r[0];
            else
                color = header.color_rescale_r[r & (short_bit_mask[signal_bit_mask[2]] >> 7)];
            color |= end_of_frame;
            out.put( color & 0x00ff      ); // red
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                rr = 0;
            else
                rr = r & (short_bit_mask[signal_bit_mask[2]] >> 7);
            color = (rr << 7) | end_of_frame;
            out.put( color & 0x00ff      ); // red
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        {
            if(is_blank())
                color = header.color_rescale_g[0];
            else
                color = header.color_rescale_g[g & (short_bit_mask[signal_bit_mask[3]] >> 7)];
            color |= unique_frame;
            out.put( color & 0x00ff      ); // green
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                gg = 0;
            else
                gg = g & (short_bit_mask[signal_bit_mask[3]] >> 7);
            color = (gg << 7) | unique_frame;
            out.put( color & 0x00ff      ); // green
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        {
            if(is_blank())
                color = header.color_rescale_b[0];
            else
                color = header.color_rescale_b[b & (short_bit_mask[signal_bit_mask[4]] >> 7)];
            out.put( color & 0x00ff      ); // blue
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                bb = 0;
            else
                bb = b & (short_bit_mask[signal_bit_mask[4]] >> 7);
            color = (bb << 7);
            out.put( color & 0x00ff      ); // blue
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        switch(abs(header.signal_id[5])) // sign indicates polarity of chanel data
        {
            case LASERBOY_NO_SIGNAL:
                out.put(0x00);
                out.put(0x00);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Z_POSITION:
                out.put(    zz & 0x00ff      ); // z
                out.put((   zz & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_TTL:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = (is_lit()) ? (header.color_rescale_i[LASERBOY_MAX_COLOR_SHORT]) : (header.color_rescale_i[0]);
                else
                    color = (is_lit()) ? (LASERBOY_MAX_COLOR_SHORT) : (0);
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[rr | gg | bb];
                else
                    color = (rr | gg | bb) << 7;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[((76 * rr) + (150 * gg) + (28 * bb)) / 254];
                else
                    color = (((76 * rr) + (150 * gg) + (28 * bb)) / 254) << 7;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[(rr + gg + bb) / 3];
                else
                    color = ((rr + gg + bb) / 3) << 7;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[(((76 * rr) + (150 * gg) + (28 * bb)) / 254)] -  - LASERBOY_MAX_COLOR_SHORT;
                else
                    color = ((((76 * rr) + (150 * gg) + (28 * bb)) / 254) << 7) - LASERBOY_MAX_COLOR_SHORT;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
        }
        if(header.num_channels == 8)
        {
            out.put(0x00);
            out.put(0x00);
            out.put(0x00);
            out.put(0x00);
        }
        header.num_samples++;
        return;
    }
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    void to_fstream_wave_inverted(fstream&               out,
                                  LaserBoy_wave_header&  header,
                                  const u_int            signal_bit_mask[8],
                                  const bool&            end_of_frame,
                                  const bool&            unique_frame
                                 ) const
    {
        u_char rr = 0x00,
               gg = 0x00,
               bb = 0x00;

        short  xx, yy, zz, color;
        //--------------------------------------------------------------------
        xx = x & short_bit_mask[signal_bit_mask[0]];
        yy = y & short_bit_mask[signal_bit_mask[1]];
        zz = z & short_bit_mask[signal_bit_mask[5]];
        //--------------------------------------------------------------------
        out.put( -xx & 0x00ff      ); // x
        out.put((-xx & 0xff00) >> 8);
        //--------------------------------------------------------------------
        out.put( -yy & 0x00ff      ); // y
        out.put((-yy & 0xff00) >> 8);
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        {
            if(is_blank())
                color = header.color_rescale_r[0];
            else
                color = header.color_rescale_r[r & (short_bit_mask[signal_bit_mask[2]] >> 7)];
            color |= end_of_frame;
            color = -color;
            out.put( color & 0x00ff      ); // red
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                rr = 0;
            else
                rr = r & (short_bit_mask[signal_bit_mask[2]] >> 7);
            color = (rr << 7) | end_of_frame;
            color = -color;
            out.put( color & 0x00ff      ); // red
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        {
            if(is_blank())
                color = header.color_rescale_g[0];
            else
                color = header.color_rescale_g[g & (short_bit_mask[signal_bit_mask[3]] >> 7)];
            color |= unique_frame;
            color = -color;
            out.put( color & 0x00ff      ); // green
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                gg = 0;
            else
                gg = g & (short_bit_mask[signal_bit_mask[3]] >> 7);
            color = (gg << 7) | unique_frame;
            color = -color;
            out.put( color & 0x00ff      ); // green
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        {
            if(is_blank())
                color = header.color_rescale_b[0];
            else
                color = header.color_rescale_b[b & (short_bit_mask[signal_bit_mask[4]] >> 7)];
            color = -color;
            out.put( color & 0x00ff      ); // blue
            out.put((color & 0xff00) >> 8);
        }
        else
        {
            if(is_blank())
                bb = 0;
            else
                bb = b & (short_bit_mask[signal_bit_mask[4]] >> 7);
            color = (bb << 7);
            color = -color;
            out.put( color & 0x00ff      ); // blue
            out.put((color & 0xff00) >> 8);
        }
        //--------------------------------------------------------------------
        switch(abs(header.signal_id[5])) // sign indicates polarity of channel data
        {
            case LASERBOY_NO_SIGNAL:
                out.put(0x00);
                out.put(0x00);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Z_POSITION:
                out.put( -zz & 0x00ff      ); // z
                out.put((-zz & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_TTL:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = (is_lit()) ? (header.color_rescale_i[LASERBOY_MAX_COLOR_SHORT]) : (header.color_rescale_i[0]);
                else
                    color = (is_lit()) ? (LASERBOY_MAX_COLOR_SHORT) : (0);
                color = -color;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[rr | gg | bb];
                else
                    color = (rr | gg | bb) << 7;
                color = -color;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[((76 * rr) + (150 * gg) + (28 * bb)) / 254];
                else
                    color = (((76 * rr) + (150 * gg) + (28 * bb)) / 254) << 7;
                color = -color;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[(rr + gg + bb) / 3];
                else
                    color = ((rr + gg + bb) / 3) << 7;
                color = -color;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                    color = header.color_rescale_i[(((76 * rr) + (150 * gg) + (28 * bb)) / 254)] -  - LASERBOY_MAX_COLOR_SHORT;
                else
                color = ((((76 * rr) + (150 * gg) + (28 * bb)) / 254) << 7) - LASERBOY_MAX_COLOR_SHORT;
                color = -color;
                out.put( color & 0x00ff      );
                out.put((color & 0xff00) >> 8);
                break;
            //----------------------------------------------------------------
        }
        if(header.num_channels == 8)
        {
            out.put(0x00);
            out.put(0x00);
            out.put(0x00);
            out.put(0x00);
        }
        header.num_samples++;
        return;
    }
    //------------------------------------------------------------------------
    u_char  k, // blanking byte
            c; // color index into palette
};

//############################################################################
class LaserBoy_segment_base : public vector<LaserBoy_vertex>
{
public:
    //------------------------------------------------------------------------
    LaserBoy_segment_base()
            {}
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_segment_base()
            { clear(); }
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
