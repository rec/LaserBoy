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
// palettes.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_PALETTE_DEFINITIONS__
#define __LASERBOY_PALETTE_DEFINITIONS__

//############################################################################
#include <LaserBoy/bmp.hpp>
#include <LaserBoy/color.hpp>
#include <LaserBoy/ild_header.hpp>

namespace LaserBoy {

//############################################################################
class Space;

//############################################################################
class Palette : public PaletteBase
{
public:
    Palette(Space* ps = NULL)
              : in_use  (false),
                white   ( 0),
                black   ( 0),
                index   ( 0),
                first   ( 0),
                last    ( 0),
                dud     (  ),
                name    (""),
                p_space (ps)
              {}
    //------------------------------------------------------------------------
    Palette(Space* ps, const int& size)
              : in_use  (false),
                white   ( 0),
                black   ( 0),
                index   ( 0),
                first   ( 0),
                last    ( 0),
                dud     (  ),
                name    (""),
                p_space(ps)
              {
                  reserve(size);
              }
    //------------------------------------------------------------------------
    Palette(const Palette& p)
              : in_use  (true     ),
                white   (p.white  ),
                black   (p.black  ),
                index   (p.index  ),
                first   (p.first  ),
                last    (p.last   ),
                dud     (         ),
                name    (p.name   ),
                p_space (p.p_space)
                {
                    clear();
                    reserve(p.size());
                    insert(begin(), p.begin(), p.end());
                }
    //------------------------------------------------------------------------
virtual
   ~Palette() {}
    //------------------------------------------------------------------------
    Palette& operator = (const Palette& p)
                {
                    clear();
                    reserve(p.size());
                    insert(end(), p.begin(), p.end());
                    white   = p.white  ;
                    black   = p.black  ;
                    index   = p.index  ;
                    first   = p.first  ;
                    last    = p.last   ;
                    name    = p.name   ;
                    p_space = p.p_space;
                    return *this;
                }
    //------------------------------------------------------------------------
    Palette& operator += (const Palette& p)
                {
                    reserve(size() + p.size());
                    insert(end(), p.begin(), p.end());
                    return *this;
                }
    //------------------------------------------------------------------------
    Color& operator [] (const int& index)
                {
                    if(index >= 0 && index < (int)number_of_colors())
                        return at(index);
                    return dud;
                }
    //------------------------------------------------------------------------
    Color& last_color()
                {
                    if(size())
                        return at(size() - 1);
                    return dud;
                }
    //------------------------------------------------------------------------
    u_int  number_of_colors () const {   return size();   }
    //------------------------------------------------------------------------
    void   set_index_color  (const u_int&  color_index,
                             const u_char& r,
                             const u_char& g,
                             const u_char& b
                            )
            {
                if(color_index < number_of_colors())
                {
                    at(color_index).r = r;
                    at(color_index).g = g;
                    at(color_index).b = b;
                }
            }
    //------------------------------------------------------------------------
    void   set_index_color  (const u_int&          color_index,
                             const Color& c
                            )
            {
                if(color_index < number_of_colors())
                    at(color_index) = c;
            }
    //------------------------------------------------------------------------
    void   erase_index(u_int index)
            {
                if(index < number_of_colors())
                    erase(begin() + index);
                if(size())
                {
                    if(!at(size() - 1).is_black())
                        push_back(Color());
                }
                else
                    push_back(Color());
                find_factors();
                return;
            }
    //------------------------------------------------------------------------
    void   clear_from_index(u_int index)
            {
                if(index < number_of_colors())
                    erase(begin() + index, end());
                if(size())
                {
                    if(!at(size() - 1).is_black())
                        push_back(Color());
                }
                else
                    push_back(Color());
                find_factors();
                return;
            }
    //------------------------------------------------------------------------
    void   wipe()
            {
                for(u_int i = 0; i < number_of_colors(); i++)
                    at(i) = (u_char)0x00;
                return;
                find_factors();
            }
    //------------------------------------------------------------------------
    bool   add_color(Color c = (u_char)0x00)
            {
                if(size() < 256)
                {
                    if(size() && at(size() - 1).is_black())
                    {
                        at(size() - 1) = c;
                        push_back(Color(0, 0, 0));
                    }
                    else
                        push_back(c);
                    find_factors();
                    return true;
                }
                find_factors();
                return false;
            }
    //------------------------------------------------------------------------
    bool   insert_color(Color c, u_int index)
            {
                if(size() < 256)
                {
                    while(size() <= index)
                        add_color();
                    insert(begin() + index, c);
                    find_factors();
                    return true;
                }
                find_factors();
                return false;
            }
    //------------------------------------------------------------------------
    Palette&   reorder();
    //------------------------------------------------------------------------
    bool   from_ifstream_ild(ifstream& in,  const ILDHeader& header);
    void   to_ofstream_ild  (ofstream& out,       ILDHeader& header) const;
    //------------------------------------------------------------------------
    bool   from_ifstream_txt(ifstream&  in,
                             const int& group_type,
                             u_int&     line_number
                            );
    //------------------------------------------------------------------------
    bool   save_as_txt           (const string& file       ) const ;
    void   to_ofstream_txt       (ofstream&     out        ) const ;
    //------------------------------------------------------------------------
    ErrorCode  to_bmp_palette        (struct Bitmap* bmp ) const ;
    //------------------------------------------------------------------------
    void   find_factors          (                         )       ;
    int    best_match            (Color rgb       )       ;
    void   best_reduction        (                         )       ;
    void   straight_blend        (                         )       ;
    void   circular_blend        (                         )       ;
    void   shade                 (u_char shade             )       ;
    void   tint                  (u_char tint              )       ;
    bool   unite                 (const Palette& palette) ;
    //------------------------------------------------------------------------
    bool             in_use ;
    u_int            white  ,
                     black  ,
                     index  ,
                     first  ,
                     last   ;
    Color   dud    ;
    string           name   ;
    Space  *p_space;
};

//############################################################################
class PaletteSetBase : public vector<Palette>
{
public:
    //------------------------------------------------------------------------
    PaletteSetBase()
            {}
    //------------------------------------------------------------------------
virtual
   ~PaletteSetBase()
            { clear(); }
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
