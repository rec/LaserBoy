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
// palette.cpp is part of LaserBoy.
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
#include <LaserBoy/space.hpp>

namespace LaserBoy {

//############################################################################
Palette& Palette::reorder()
{
    if(size())
    {
        u_int           i;
        Color  black,
                        white(255,255,255);

        Palette reordered(p_space);
        //--------------------------------------------------------------------
        reordered.reserve(size());
        sort(begin(), end()); // sort this palette
        for(i = 0; i < number_of_colors(); i++)
            if(at(i) != black)
            {
                reordered.push_back(at(i));
                break;
            }
        for(/* i from above */ ; i < number_of_colors(); i++)
            if(at(i) != reordered.back()) // remove duplicates!
                reordered.push_back(at(i));
        if(reordered.size() == 0)
            reordered.push_back(white); // at least one real color!
        *this = reordered;
    }
    return *this;
}

//############################################################################
bool Palette::from_ifstream_ild(ifstream& in,
                                         const ILDHeader& header
                                        )
{
    u_int           i;
    Color  color;
    //------------------------------------------------------------------------
    clear();
    reserve(header.quantity);
    //------------------------------------------------------------------------
    for(i = 0; i < header.quantity; i++)
        if(color.from_ifstream_ild(in))
            push_back(color);
        else
            return false;
    find_factors();
    name = header.name;
    for(i = 0; i < 8; i++)
        if(!isprint(name[i]))
        {
            name = p_space->GUID8char();
            break;
        }
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void Palette::to_ofstream_ild(ofstream& out, ILDHeader& header) const
{
    header.quantity = (u_short)number_of_colors();
    header.to_ofstream_ild(out);
    for(u_int i = 0; i < number_of_colors(); i++)
        at(i).to_ofstream_ild(out);
    return;
}

//############################################################################
bool Palette::from_ifstream_txt(ifstream&  in,
                                         const int& group_type,
                                         u_int&     line_number
                                        )
{
    u_int           i = 255;
    Color  color;
    clear();
    while(    color.from_ifstream_txt(in, group_type, line_number)
           && i--
         )
        push_back(color);
    if(size())
    {
        find_factors();
        return true;
    }
    return false;
}

//############################################################################
bool Palette::save_as_txt(const string& file) const
{
    ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        to_ofstream_txt(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
void Palette::to_ofstream_txt(ofstream& out) const
{
    u_int i;
    //------------------------------------------------------------------------
    out << "# ----------------------------------------------------------------"
        << ENDL
        << (   (number_of_colors() > 256)
             ? ("table")
             : ("palette")
           )
        << (   (p_space->save_txt_color_hex)
             ? (" hex ")
             : (" rgb ")
           )
        << name
        << ENDL;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_colors(); i++)
        at(i).to_ofstream_txt(out, p_space->save_txt_color_hex);
    //------------------------------------------------------------------------
    out << ENDL;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
ErrorCode Palette::to_bmp_palette(struct Bitmap* bmp) const
{
    u_int i;
    if(bmp->bpp == 8)
    {
        for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
            if(i < number_of_colors())
                bmp_set_palette_index(bmp, i, at(i).r, at(i).g, at(i).b);
            else
                bmp_set_palette_index(bmp, i, 0, 0, 0);
        bmp->black = black;
        bmp->white = white;
        return LASERBOY_OK;
    }
    return LASERBOY_LB_WAVE_FORMAT_ERROR;
}

//############################################################################
void Palette::find_factors()
{
    int i;
    //------------------------------------------------------------------------
    first = black = white = last = 0;
    //------------------------------------------------------------------------
    for(i = 0; i < (int)number_of_colors(); i++)
    {
        if(at(black) > at(i))
          black = i;
        //--------------------------------------------------------------------
        if(at(white) < at(i))
          white = i;
    }
    //------------------------------------------------------------------------
    for(i = 0; i < (int)number_of_colors(); i++)
        if(at(i).is_color(p_space->black_level))
        {
            first = i;
            break;
        }
    //------------------------------------------------------------------------
    for(i = number_of_colors() - 1; i >= 0 ; i--)
        if(at(i).is_color(p_space->black_level))
        {
            last = i;
            break;
        }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
int Palette::best_match(Color rgb)
{
    u_int  i,
           j,
           k = 255,
           match = black;
    //------------------------------------------------------------------------
    if(p_space->no_black_match)
        match = white;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_colors(); i++)
    {
        j =   abs(at(i).r - rgb.r)
            + abs(at(i).g - rgb.g)
            + abs(at(i).b - rgb.b);
        if(j < k)
        {
            match = i;
            k     = j;
        }
    }
    //------------------------------------------------------------------------
    return match;
}

//############################################################################
void Palette::best_reduction()
{
    if(size())
    {
        u_int  i,
               j,
               k = 255;
        //--------------------------------------------------------------------
        reorder();
        //--------------------------------------------------------------------
        for(k = 1; k < 765; k++) // minimum difference
        {
            for(i = 0; i < size() - 1; i++)
            {
                j =   abs(at(i).r - at(i + 1).r)
                    + abs(at(i).g - at(i + 1).g)
                    + abs(at(i).b - at(i + 1).b);

                if(j <= k)
                    erase(begin() + i + 1);
            }
            if(size() <= LASERBOY_PALETTE_MAX) // if we have LASERBOY_PALETTE_MAX or less colors, we're done.
                break;
        }
        //--------------------------------------------------------------------
        find_factors();
    }
    return;
}

//############################################################################
void Palette::straight_blend()
{
    if(size())
    {
        u_int          i,
                       colors;
        Color c1, c2, c3, c4, c5, c6, c7;

        if(at(size() - 1).is_black())
            colors = size() - 1;
        else
            colors = size();

        switch(colors)
        {
            case  0:
                c1 = (u_char)255;
                c2 = at(0);
                clear();
                for(i = 0; i < 256; i++)
                    push_back(c1.blend(c2, i / 255.0));
                break;
            case  1:
            case  2:
                c1 = at(0);
                c2 = at(1);
                clear();
                for(i = 0; i < 255; i++)
                    push_back(c1.blend(c2, i / 254.0));
                break;
            case  3:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 127; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  4:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                clear();
                for(i = 0; i < 85; i++)
                    push_back(c1.blend(c2, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c2.blend(c3, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c3.blend(c4, i / 84.0));
                break;
            case  5:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                clear();
                for(i = 0; i < 64; i++)
                    push_back(c1.blend(c2, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c2.blend(c3, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c3.blend(c4, i / 63.0));
                for(i = 0; i < 63; i++)
                    push_back(c4.blend(c5, i / 62.0));
                break;
            case  6:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                clear();
                for(i = 0; i < 51; i++)
                    push_back(c1.blend(c2, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c2.blend(c3, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c3.blend(c4, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c4.blend(c5, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c5.blend(c6, i / 50.0));
                break;
            default:
            case  7:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(6);
                clear();
                for(i = 0; i < 43; i++)
                    push_back(c1.blend(c2, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c2.blend(c3, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c3.blend(c4, i / 42.0));
                for(i = 0; i < 42; i++)
                    push_back(c4.blend(c5, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c5.blend(c6, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c6.blend(c7, i / 41.0));
                break;
        }
    }
    push_back(Color());
    find_factors();
    return;
}

//############################################################################
void Palette::circular_blend()
{
    if(size())
    {
        u_int          i,
                       colors;
        Color c1, c2, c3, c4, c5, c6, c7, c8;

        if(at(size() - 1).is_black())
            colors = size() - 1;
        else
            colors = size();

        switch(colors)
        {
            case  0:
                c1 = (u_char)255;
                c2 = at(0);
                c3 = (u_char)255;
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 126; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  1:
            case  2:
                c1 = at(0);
                c2 = at(1);
                c3 = at(0);
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 127; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  3:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(0);
                clear();
                for(i = 0; i < 85; i++)
                    push_back(c1.blend(c2, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c2.blend(c3, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c3.blend(c4, i / 84.0));
                break;
            case  4:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(0);
                clear();
                for(i = 0; i < 64; i++)
                    push_back(c1.blend(c2, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c2.blend(c3, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c3.blend(c4, i / 63.0));
                for(i = 0; i < 63; i++)
                    push_back(c4.blend(c5, i / 62.0));
                break;
            case  5:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(0);
                clear();
                for(i = 0; i < 51; i++)
                    push_back(c1.blend(c2, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c2.blend(c3, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c3.blend(c4, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c4.blend(c5, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c5.blend(c6, i / 50.0));
                break;
            case  6:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(0);
                clear();
                for(i = 0; i < 43; i++)
                    push_back(c1.blend(c2, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c2.blend(c3, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c3.blend(c4, i / 42.0));
                for(i = 0; i < 42; i++)
                    push_back(c4.blend(c5, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c5.blend(c6, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c6.blend(c7, i / 41.0));
                break;
            default:
            case  7:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(6);
                c8 = at(0);
                clear();
                for(i = 0; i < 37; i++)
                    push_back(c1.blend(c2, i / 36.0));
                for(i = 0; i < 37; i++)
                    push_back(c2.blend(c3, i / 36.0));
                for(i = 0; i < 37; i++)
                    push_back(c3.blend(c4, i / 36.0));
                for(i = 0; i < 36; i++)
                    push_back(c4.blend(c5, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c5.blend(c6, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c6.blend(c7, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c7.blend(c8, i / 35.0));
                break;
        }
    }
    push_back(Color());
    find_factors();
    return;
}

//############################################################################
void Palette::shade(u_char shade) // 0 shade is no change 255 is black
{
    if(shade)
    {
        for(u_int i = 0; i < number_of_colors(); i++)
        {
            (at(i).r - shade > 0) ? (at(i).r -= shade) : (at(i).r = 0);
            (at(i).g - shade > 0) ? (at(i).g -= shade) : (at(i).g = 0);
            (at(i).b - shade > 0) ? (at(i).b -= shade) : (at(i).b = 0);
        }
    }
    return;
}

//############################################################################
void Palette::tint(u_char tint) // 0 tint is no change 255 is white
{
    if(tint)
    {
        for(u_int i = 0; i < number_of_colors(); i++)
        {
            (at(i).r + tint < 255) ? (at(i).r += tint) : (at(i).r = 255);
            (at(i).g + tint < 255) ? (at(i).g += tint) : (at(i).g = 255);
            (at(i).b + tint < 255) ? (at(i).b += tint) : (at(i).b = 255);
        }
    }
    return;
}

//############################################################################
bool Palette::unite(const Palette& palette)
{
    if(    palette.number_of_colors() >  0
        && palette.number_of_colors() <= LASERBOY_PALETTE_MAX
      )
    {
        u_int             i,
                          j;
        Palette  super_palette(*this);
        //--------------------------------------------------------------------
        super_palette.reserve(LASERBOY_PALETTE_MAX);
        //--------------------------------------------------------------------
        for(i = 0; i < palette.number_of_colors(); i++)
        {
            for(j = 0; j < super_palette.number_of_colors(); j++)
                if(super_palette[j] == palette.at(i))
                    break;
            if(j == super_palette.number_of_colors())
                super_palette.push_back(palette.at(i));
        }
        //--------------------------------------------------------------------
        if(super_palette.number_of_colors() <= LASERBOY_PALETTE_MAX)
        {
            super_palette.reorder();
            super_palette.find_factors();
            *this = super_palette;
            return true;
        }
        //--------------------------------------------------------------------
        else
            return false;
    }
    //------------------------------------------------------------------------
    return false;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
