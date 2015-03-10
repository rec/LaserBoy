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
// palette_set.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_PALETTE_SET_DEFINITIONS__
#define __LASERBOY_PALETTE_SET_DEFINITIONS__

//############################################################################
#include <LaserBoy/palette.hpp>

namespace LaserBoy {

//############################################################################
class PaletteSet : public PaletteSetBase
{
public:
    PaletteSet(Space* ps = NULL)
                  : p_space                (ps),
                    palette_set_error      (LASERBOY_OK),
                    palette_index          (LASERBOY_ILDA_DEFAULT),
                    target_palette_index   (0),
                    selected_color_index   (0)
                    {
                        populate_palette_set();
                    }
    //------------------------------------------------------------------------
virtual
   ~PaletteSet()  {}
    //------------------------------------------------------------------------
    PaletteSet& replace_palette_set(const PaletteSet& palette_set)
                    {
                        clear();
                        insert(begin(), palette_set.begin(), palette_set.end());
                        return *this;
                    }
    //------------------------------------------------------------------------
    PaletteSet& operator = (const PaletteSet& palette_set)
                    {
                        clear();
                        reserve(palette_set.size());
                        insert(begin(), palette_set.begin(), palette_set.end());
                        p_space = palette_set.p_space;
                        return *this;
                    }
    //------------------------------------------------------------------------
    PaletteSet& operator += (const PaletteSet& palette_set)
                    {
                        reserve(size() + palette_set.size());
                        insert(end(), palette_set.begin(), palette_set.end());
                        return *this;
                    }
    //------------------------------------------------------------------------
    PaletteSet& operator += (const Palette& palette)
                    {
                        push_back(palette);
                        return *this;
                    }
    //------------------------------------------------------------------------
    PaletteSet& push_back_palette(const Palette& palette)
                    {
                        push_back(palette);
                        return *this;
                    }
    //------------------------------------------------------------------------
    void  set_palette_index(u_int index) //sets palette_index
                    {
                        if(number_of_palettes())
                        {
                            if(index >= number_of_palettes())
                                index = 0;
                            if(index < 0)
                                index = number_of_palettes() - 1;
                            palette_index = index;
                        }
                        else
                            palette_index = 0;
                    }
    //------------------------------------------------------------------------
    void  set_target_palette_index(u_int index) //sets target_palette_index
                    {
                        if(number_of_palettes())
                        {
                            if(index >= number_of_palettes())
                                index = 0;
                            if(index < 0)
                                index = number_of_palettes() - 1;
                            target_palette_index = index;
                        }
                        else
                            target_palette_index = 0;
                    }
    //------------------------------------------------------------------------
    Palette&  palette(u_int index) //sets palette_index
                    {
                        if(number_of_palettes())
                        {
                            if(index >= number_of_palettes())
                                index = 0;
                            if(index < 0)
                                index = number_of_palettes() - 1;
                            palette_index = index;
                            return at(palette_index);
                        }
                        return front();
                    }
    //------------------------------------------------------------------------
    Palette&  set_target_palette_to(u_int index) //sets target_palette_index
                    {
                        if(number_of_palettes())
                        {
                            if(index >= number_of_palettes())
                                index = 0;
                            if(index < 0)
                                index = number_of_palettes() - 1;
                            target_palette_index = index;
                            return at(target_palette_index);
                        }
                        return front();
                    }
    //------------------------------------------------------------------------
    Palette& current_palette()
                    {
                        if(palette_index != LASERBOY_TRUE_COLOR)
                            return at(palette_index);
                        return at(target_palette_index);
                    }
    //------------------------------------------------------------------------
    Palette& target_palette()
                    {
                        return at(target_palette_index);
                    }
    //------------------------------------------------------------------------
    int  name_to_palette_index(string name)
                    {
                        for(u_int i = 0; i < number_of_palettes(); i++)
                            if(at(i).name == name)
                                return i;
                        return LASERBOY_PALETTE_NOT_FOUND; // -2
                    }
    //------------------------------------------------------------------------
    u_int greater_number_of_colors() const
                    {
                        u_int i = 0;
                        if(i < at(target_palette_index).size())
                            i = at(target_palette_index).size();
                        if(    palette_index != LASERBOY_TRUE_COLOR
                            && i < at(palette_index).size()
                          )
                            i = at(palette_index).size();
                        return i;
                    }
    //------------------------------------------------------------------------
    u_int number_of_palettes() const
                    {
                        return size();
                    }
    //------------------------------------------------------------------------
    string              GUID8char                   ();
    //------------------------------------------------------------------------
    Palette&   palette_picker              (int index);
    //------------------------------------------------------------------------
    ErrorCode palette_from_ifstream_txt   (ifstream&       in,
                                                     const u_int&    group_type,
                                                     u_int&          line_number,
                                                     const string&   name = ""
                                                    );
    //------------------------------------------------------------------------
    ErrorCode palettes_from_txt_file      (const string& file);
    //------------------------------------------------------------------------
    void                populate_palette_set        ();
    void                add_palette                 ();
    void                copy_palette                (int index);
    //------------------------------------------------------------------------
    Space*      p_space;
    ErrorCode  palette_set_error   ;
    int                  palette_index       ;  // may be LASERBOY_TRUE_COLOR (-1)
    u_int                target_palette_index,
                         selected_color_index;
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
