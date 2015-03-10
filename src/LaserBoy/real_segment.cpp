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
// RealSegment.cpp is part of LaserBoy.
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
#include <LaserBoy/GUI_base.hpp>

namespace LaserBoy {

//############################################################################
RealSegment::RealSegment(Space* ps,
                                             RealVertex from,
                                             RealVertex to
                                            ) // 3D line function
                : p_space            (ps),
                  palette_index      (LASERBOY_ILDA_DEFAULT),
                  real_segment_error (LASERBOY_OK)
{   // 3D line constructor
    int steps = linear_steps(to, from, (   (to.is_lit())
                                         ? (p_space->lit_delta_max)
                                         : (p_space->blank_delta_max)
                                       )
                            );
    if(steps)
    {
        Double3d _to        (to),
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //--------------------------------------------------------------------
        for(int i = 1; i < steps; i++)
            push_back(RealVertex( Double3d(_from + (delta * i)),
                                            (Color)to,
                                            to.k,
                                            to.c
                                          )
                     );
        //--------------------------------------------------------------------
    }
}

//############################################################################
u_int RealSegment::number_of_color_vectors() const
{
    u_int count = 0;
    for(u_int i = 1; i < size(); i++)
        if(at(i).is_color(p_space->black_level))
            count++;
    return count;
}

//############################################################################
u_int RealSegment::number_of_dark_vectors() const
{
    u_int count = 0;
    for(u_int i = 1; i < size(); i++)
        if(at(i).is_dark(p_space->black_level))
            count++;
    return count;
}

//############################################################################
Double3d RealSegment::segment_front() const
{
    Double3d front_;
    front_.z = -DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).z > front_.z)
            front_ = at(i);
    return front_;
}

//############################################################################
Double3d RealSegment::segment_back() const
{
    Double3d back_;
    back_.z = DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).z < back_.z)
            back_ = at(i);
    return back_;
}

//############################################################################
Double3d RealSegment::segment_top() const
{
    Double3d top_;
    top_.y = -DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).y > top_.y)
            top_ = at(i);
    return top_;
}

//############################################################################
Double3d RealSegment::segment_bottom() const
{
    Double3d bottom_;
    bottom_.y = DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).y < bottom_.y)
            bottom_ = at(i);
    return bottom_;
}

//############################################################################
Double3d RealSegment::segment_right() const
{
    Double3d right_;
    right_.x = -DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).x > right_.x)
            right_ = at(i);
    return right_;
}

//############################################################################
Double3d RealSegment::segment_left() const
{
    Double3d left_;
    left_.x = DBL_MAX;
    for(u_int i = 0; i < size(); i++)
        if(at(i).x < left_.x)
            left_ = at(i);
    return left_;
}

//############################################################################
double RealSegment::segment_height() const
{
    return (segment_top().y - segment_bottom().y);
}
//############################################################################
double RealSegment::segment_width() const
{
    return (segment_right().x - segment_left().x);
}

//############################################################################
double RealSegment::segment_depth() const
{
    return (segment_front().z - segment_back().z);
}

//############################################################################
double RealSegment::segment_size() const
{
    double greatest = 0;
    if(segment_width () > greatest)    greatest = segment_width ();
    if(segment_height() > greatest)    greatest = segment_height();
    if(segment_depth () > greatest)    greatest = segment_depth ();
    return greatest;
}

//############################################################################
void RealSegment::blank_all_vertices()
{
    for(u_int i = 0; i < size(); i++)
        at(i).blank();
    return;
}

//############################################################################
void RealSegment::unblank_all_vertices()
{
    for(u_int i = 1; i < size(); i++)
        at(i).unblank();
    return;
}

//############################################################################
RealSegment& RealSegment::reverse()
{
    if(size() > 1)
    {
        int                    i;
        RealSegment  reversed;
        RealVertex   vertex;
        //--------------------------------------------------------------------
        reversed.reserve(size());
        //--------------------------------------------------------------------
        i = size() - 1;
        //--------------------------------------------------------------------
        vertex = at(i--);
        vertex.blank();
        reversed.push_back(vertex);
        //--------------------------------------------------------------------
        while(i >= 0)
        {
            vertex   = at(i    );
            vertex.r = at(i + 1).r;
            vertex.g = at(i + 1).g;
            vertex.b = at(i + 1).b;
            vertex.c = at(i + 1).c;
            vertex.k = at(i + 1).k;
            reversed.push_back(vertex);
            i--;
        }
        //--------------------------------------------------------------------
        *this = reversed;
    }
    return *this;
}

//############################################################################
void RealSegment::flip(u_int plane)
{
    if(size() > 1)
    {
        u_int i;
        switch(plane)
        {
           case 0: // X mirror
                   for(i = 0; i < size(); i++)
                       at(i).x = -at(i).x;
                   break;
           //-----------------------------------------------------------------
           case 1: // Y flip
                   for(i = 0; i < size(); i++)
                       at(i).y = -at(i).y;
                   break;
           //-----------------------------------------------------------------
           case 2: // Z
                   for(i = 0; i < size(); i++)
                       at(i).z = -at(i).z;
                   break;
           //-----------------------------------------------------------------
           case 3: // X, Y
                   for(i = 0; i < size(); i++)
                   {
                       at(i).x = -at(i).x;
                       at(i).y = -at(i).y;
                   }
                   break;
           //-----------------------------------------------------------------
           case 4: // X, Y, Z
                   for(i = 0; i < size(); i++)
                       at(i) = -at(i);
                   break;
           //-----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
void RealSegment::quarter_turn(u_int plane, u_int turns)
{
    if(size() > 1)
    {
        u_int   i,
                j;
        double  temp;
        switch(plane)
        {
           case 0: // X Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 1: // Z Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).z;
                           at(i).z = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 2: // X Z
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).z;
                           at(i).z = temp;
                       }
                   break;
           //-----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
void RealSegment::flatten_z()
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
            at(i).z = 0;
    }
    return;
}

//############################################################################
void RealSegment::rotate_around_origin(Double3d a)
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
            at(i) = RealVertex(rotate_vertex(at(i), a),
                                         (Color)at(i),
                                         at(i).k,
                                         at(i).c
                                        );
    }
    return;
}

//############################################################################
void RealSegment::rotate_on_coordinates(Double3d p, Double3d a)
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
            at(i) = RealVertex(rotate_vertex_on_coordinates(at(i), p, a),
                                         (Color)at(i),
                                         at(i).k,
                                         at(i).c
                                        );
    }
    return;
}

//############################################################################
bool RealSegment::find_rgb_in_palette(const Palette& palette)
{
    u_int         i,
                  j;
    vector<bool>  match(size(), false);
    bool          all_colors_found = true;
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
        for(j = 0; j < palette.number_of_colors(); j++)
            if((Color)at(i) == palette.at(j))
            {
                at(i).c = (u_char)j;
                match[i] = true;
            }
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
        all_colors_found &= match[i];
    //------------------------------------------------------------------------
    return all_colors_found;
}

//############################################################################
void RealSegment::set_rgb_from_palette()
{
    front().r =
    front().g =
    front().b = 0x00;
    //------------------------------------------------------------------------
    for(u_int i = 1; i < size(); i++)
    {
        at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
        at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
        at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
    }
    return;
}

//############################################################################
void RealSegment::set_palette_to_332()
{
    front().c = 0x00;
    for(u_int i = 1; i < size(); i++)
        at(i).c =    (at(i).r & 0xe0)
                  | ((at(i).g & 0xe0) >> 3)
                  | ((at(i).b & 0xc0) >> 6);
    // does NOT set palette_index to LASERBOY_REDUCED_332
    return;
}

//############################################################################
void RealSegment::sync_rgb_and_palette()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
            set_rgb_from_palette();
        //--------------------------------------------------------------------
        else
        {
            u_int             i               ,
                              j               ;
            Palette  palette(p_space);
            //----------------------------------------------------------------
            palette.push_back((Color)at(first_lit_vector_index()));
            //----------------------------------------------------------------
            for(i = first_lit_vector_index() + 1; i < size(); i++)
            {
                for(j = 0; j < palette.number_of_colors(); j++)
                    if(at(i).is_lit() && (palette[j] == (Color)at(i)))
                        break;
                if(    j == palette.number_of_colors()
                    && at(i).is_lit()
                  )
                    palette.push_back((Color)at(i));
                if(palette.number_of_colors() > LASERBOY_PALETTE_MAX)
                    break;
            }
            //----------------------------------------------------------------
            if(palette.number_of_colors() <= LASERBOY_PALETTE_MAX)
            {
                palette.reorder();
                palette.find_factors();
                find_rgb_in_palette(palette);
                palette.name = p_space->GUID8char();
                p_space->push_back_palette(palette);
                palette_index = p_space->number_of_palettes() - 1;
            }
            //----------------------------------------------------------------
            else // (palette.number_of_colors() > LASERBOY_PALETTE_MAX)
                set_palette_to_332();
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void RealSegment::best_match_palette(u_int index)
{
    if(    (int)index != palette_index
        && index < p_space->number_of_palettes()
      )
    {
        if(size() > 1)
        {
            if(!p_space->allow_lit_black)
                convert_black_to_blank();
            else
                impose_black_level();

            for(u_int i = 1; i < size(); i++)
                 at(i).c = p_space->
                           palette_picker(index)
                           .best_match(at(i));
        }
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void RealSegment::bit_reduce_to_palette()
{
    if(palette_index == LASERBOY_TRUE_COLOR)
    {
        set_palette_to_332();
        palette_index = LASERBOY_REDUCED_332;
        set_rgb_from_palette();
    }
    return;
}


//############################################################################
void RealSegment::best_reduce_to_palette()
{
    if(number_of_color_vectors() >= 1)
    {
        u_int             i               ,
                          j               ;
        Palette  palette(p_space);
        //--------------------------------------------------------------------
        if(!p_space->allow_lit_black)
            convert_black_to_blank();
        else
            impose_black_level();
        //--------------------------------------------------------------------
        palette.push_back((Color)at(first_lit_vector_index()));
        //--------------------------------------------------------------------
        for(i = first_lit_vector_index() + 1; i < size(); i++)
        {
            for(j = 0; j < palette.number_of_colors(); j++)
                if(at(i).is_lit() && (palette[j] == (Color)at(i)))
                    break;
            if(    j == palette.number_of_colors()
                && at(i).is_lit()
              )
                palette.push_back((Color)at(i));
        }
        //--------------------------------------------------------------------
        palette.best_reduction();
        //--------------------------------------------------------------------
        palette.name = p_space->GUID8char();
        p_space->push_back_palette(palette);
        //--------------------------------------------------------------------
        best_match_palette(p_space->number_of_palettes() - 1);
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void RealSegment::convert_black_to_blank()
{
    for(u_int i = 0; i < size(); i++)
    {
        if(at(i).is_black(p_space->black_level))
        {
            at(i).r = 255;
            at(i).g = 0;
            at(i).b = 0;
            at(i).blank();
        }
    }
    return;
}

//############################################################################
void RealSegment::convert_blank_to_black()
{
    for(u_int i = 0; i < size(); i++)
    {
        if(at(i).is_blank())
        {
            at(i).r = 0;
            at(i).g = 0;
            at(i).b = 0;
            at(i).c = p_space->palette_picker(palette_index).black;
            at(i).unblank();
        }
    }
    return;
}

//############################################################################
void RealSegment::impose_black_level()
{
    for(u_int i = 0; i < size(); i++)
    {
        if(at(i).as_Color().intensity() < p_space->black_level)
        {
            at(i).r = 0;
            at(i).g = 0;
            at(i).b = 0;
        }
    }
    return;
}

//############################################################################
void RealSegment::move(Double3d d)
{
    for(u_int i = 0; i < size(); i++)
        at(i) = RealVertex(at(i).as_3D_double() + d,
                                     (Color)at(i),
                                     at(i).k,
                                     at(i).c
                                    );
    return;
}

//############################################################################
void RealSegment::scale(Double3d s)
{
    Double3d center = mean_of_coordinates();
    scale_on_coordinates(center, s);
    return;
}

//############################################################################
void RealSegment::scale_on_coordinates(Double3d p, Double3d s)
{
    for(u_int i = 0; i < size(); i++)
        at(i) = RealVertex(((at(i).as_3D_double() - p) * s) + p,
                                     (Color)at(i),
                                     at(i).k,
                                     at(i).c
                                    );
    return;
}

//############################################################################
void RealSegment::scale_around_origin(Double3d s)
{
    for(u_int i = 0; i < size(); i++)
        at(i) *= s;
    return;
}

//############################################################################
Double3d RealSegment::rectangular_center_of() const
{
    Double3d center;
    if(size() > 1)
    {
        double  max_x = -DBL_MAX,
                min_x =  DBL_MAX,
                max_y = -DBL_MAX,
                min_y =  DBL_MAX,
                max_z = -DBL_MAX,
                min_z =  DBL_MAX;

        for(u_int i = 0; i < size(); i++)
        {
            if(at(i).x > max_x)    max_x = at(i).x;
            if(at(i).x < min_x)    min_x = at(i).x;
            if(at(i).y > max_y)    max_y = at(i).y;
            if(at(i).y < min_y)    min_y = at(i).y;
            if(at(i).z > max_z)    max_z = at(i).z;
            if(at(i).z < min_z)    min_z = at(i).z;
        }
        center.x = (max_x - ((max_x - min_x) / 2.0));
        center.y = (max_y - ((max_y - min_y) / 2.0));
        center.z = (max_z - ((max_z - min_z) / 2.0));
    }
    return center;
}

//############################################################################
Double3d RealSegment::mean_of_coordinates() const
{
    Double3d mean;
    if(size() > 1)
    {
        mean = front();
        for(u_int i = 1; i < size(); i++)
            mean += at(i);
        mean /= size();
    }
    return mean;
}

//############################################################################
u_int RealSegment::number_of_segments() const // a segment is a series of lit verticies
{
    u_int  i,
           segment_count = 0;
    if(size() > 1)
    {
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                segment_count++;
            }
        }
    }
    return segment_count;
}

//############################################################################
bool RealSegment::find_segment_at_index(u_int segment_index, u_int& start, u_int& end) const
{   // the first segment is number zero!
    if(size() > 1)
    {
        u_int  i;
        int    segment_count = -1;
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                {
                    end = i;
                }
                //------------------------------------------------------------
                segment_count++;
                if(segment_count == (int)segment_index)
                    return true;
            }
        } // segment index either negative or out of range
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                return false; // and set start, end to first segment
            }
        }
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
RealSegment RealSegment::copy_segment(u_int segment_index) const
{
    RealSegment segment(p_space);
    if(size() > 1)
    {
        u_int start, end;
        if(find_segment_at_index(segment_index, start, end))
        {
            segment.clear();
            segment.reserve(end - start + 1);
            for(u_int i = start; i <= end; i++)
                segment.push_back(at(i));
        }
    }
    return segment;
}

//############################################################################
Double3d RealSegment::rectangular_center_of_segment(u_int segment_index) const
{
    return (copy_segment(segment_index)).rectangular_center_of();
}

//############################################################################
Double3d RealSegment::mean_of_coordinates_of_segment(u_int segment_index) const
{
    return (copy_segment(segment_index)).mean_of_coordinates();
}

//############################################################################
void RealSegment::move_segment(u_int segment_index, Double3d d)
{
    if(size() > 1)
    {
        u_int  i    ,
               start,
               end  ;
        find_segment_at_index(segment_index, start, end);
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) += d;
    }
    return;
}

//############################################################################
void RealSegment::rotate_segment(u_int segment_index, Double3d a)
{
    if(size() > 1)
    {
        u_int               i     ,
                            start ,
                            end   ;
        Double3d  center;

        find_segment_at_index(segment_index, start, end);
        center = mean_of_coordinates_of_segment(segment_index);
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = RealVertex( rotate_vertex_on_coordinates(at(i), center, a),
                                   (Color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return;
}

//############################################################################
void RealSegment::rotate_segment_around_origin(u_int segment_index, Double3d a)
{
    if(size() > 1)
    {
        u_int  i    ,
               start,
               end  ;
        find_segment_at_index(segment_index, start, end);
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = RealVertex( rotate_vertex(at(i), a),
                                   (Color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return;
}

//############################################################################
void RealSegment::scale_segment(u_int segment_index, Double3d m)
{
    if(size() > 1)
    {
        u_int              i     ,
                           start ,
                           end   ;
        Double3d center;
        //--------------------------------------------------------------------
        find_segment_at_index(segment_index, start, end);
        center = mean_of_coordinates_of_segment(segment_index);
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = RealVertex( scale_vertex_on_coordinates(at(i), center, m),
                                   (Color)at(i),
                                   at(i).k,
                                   at(i).c);
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void RealSegment::scale_segment_around_origin(u_int segment_index, Double3d m)
{
    if(size() > 1)
    {
        u_int  i    ,
               start,
               end  ;
        //--------------------------------------------------------------------
        find_segment_at_index(segment_index, start, end);
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = RealVertex( m * at(i),
                                   (Color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
ErrorCode RealSegment::from_ifstream_dxf(ifstream& in)
{
    u_char                 dxf_color_index     ,
                           prev_dxf_color_index;
    u_int                  arc_vertices  = 0         ,
                           group_70_flag = 0         ,
                           group_71_flag = 0         ,
                           group_72_flag = 0         ,
                           group_73_flag = 0         ;
    int                    group_code                ,
                           first_segment_vertex_index;
    double                 double_value,
                           radius      ,
                           arc_start   ,
                           arc_end     ,
                           arc_angle   ,
                           arc_step    ;
    string                 entity_string,
                           font         ,
                           text         ;
    Double3d     double_3D_1,
                           double_3D_2,
                           double_3D_3,
                           double_3D_4;
    Color         color,
                           prev_color;
    RealSegment  real_vertices(p_space);
    vector<double>         vertex_x,
                           vertex_y;
    bool                   closed_polyline;
    //------------------------------------------------------------------------
    push_back(RealVertex()); // every RealSegment::from_ifstream_dxf
    push_back(RealVertex()); // has an original vector this.size() not less than 2
    //------------------------------------------------------------------------
    while(in.good() && entity_string != "ENTITIES") // ignore everthing up to ENTITIES
        in >> entity_string;
    //------------------------------------------------------------------------
    if(in.good())
    {
        while(in.good())
        {
            dxf_color_index          = p_space->palette_picker(LASERBOY_DXF).white;
            prev_dxf_color_index     = p_space->palette_picker(LASERBOY_DXF).white;
            color                      = (u_int)0X00ffffff;
            prev_color                 = (u_int)0X00ffffff;
            first_segment_vertex_index = -1,
            closed_polyline            = false;
            group_70_flag              = 0;
            radius                     = 0;
            arc_vertices               = 0;
            arc_start                  = 0;
            arc_end                    = 360;
            group_code                 = -1;
            //----------------------------------------------------------------
            vertex_x.clear();
            vertex_y.clear();
            //----------------------------------------------------------------
            if(entity_string == "POINT")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                                    double_3D_1.x = double_value;
                                    break;
                        case  20: // y1
                                    double_3D_1.y = double_value;
                                    break;
                        case  30: // z1
                                    double_3D_1.z = double_value;
                                    break;
                        case  62: // palette_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                    }
                }
                push_back(RealVertex(double_3D_1, color).blank());
                push_back(RealVertex(double_3D_1, color).unblank());
            }
            else if(entity_string == "LINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                                    double_3D_1.x = double_value;
                                    break;
                        case  20: // y1
                                    double_3D_1.y = double_value;
                                    break;
                        case  30: // z1
                                    double_3D_1.z = double_value;
                                    break;
                        case  11: // x2
                                    double_3D_2.x = double_value;
                                    break;
                        case  21: // y2
                                    double_3D_2.y = double_value;
                                    break;
                        case  31: // z2
                                    double_3D_2.z = double_value;
                                    break;
                        case  62: // dxf_color_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                    }
                }
                push_back(RealVertex(double_3D_1, color).blank());
                push_back(RealVertex(double_3D_2, color).unblank());
            }
            //----------------------------------------------------------------
            else if(    entity_string == "CIRCLE"
                     || entity_string == "ARC"
                   )
            {
                u_int a;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                                    double_3D_1.x = double_value;
                                    break;
                        case  20: // y1
                                    double_3D_1.y = double_value;
                                    break;
                        case  30: // z1
                                    double_3D_1.z = double_value;
                                    break;
                        case  40: // radius
                                    radius = double_value;
                                    break;
                        case  50: // start angle
                                    arc_start = double_value;
                                    break;
                        case  51: // end angle
                                    arc_end = double_value;
                                    break;
                        case  62: // dxf_color_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                    }
                }

                if(arc_start >= arc_end)
                    arc_end += 360.0;
                arc_angle  = arc_end - arc_start;
                arc_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
                arc_step   = arc_angle / arc_vertices;
                arc_start *= one_degree;
                arc_end   *= one_degree;
                arc_step  *= one_degree;

                double_3D_2.x = radius * cos(arc_start);
                double_3D_2.y = radius * sin(arc_start);
                double_3D_2.z = double_3D_1.z;

                push_back(RealVertex(double_3D_1 + double_3D_2, color).blank());

                for(a = 1; a <= arc_vertices; a++)
                {
                    double_3D_2.x = radius * cos(a * arc_step + arc_start);
                    double_3D_2.y = radius * sin(a * arc_step + arc_start);
                    double_3D_2.z = double_3D_1.z;

                    push_back(RealVertex(double_3D_1 + double_3D_2, color).unblank());
                }
            }
            //----------------------------------------------------------------
            else if(entity_string == "ELLIPSE")
            {
                u_int a,
                      vertex_index;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                                    double_3D_1.x = double_value;
                                    break;
                        case  20: // y1
                                    double_3D_1.y = double_value;
                                    break;
                        case  30: // z1
                                    double_3D_1.z = double_value;
                                    break;
                        case  11: // x2
                                    double_3D_2.x = double_value;
                                    break;
                        case  21: // y2
                                    double_3D_2.y = double_value;
                                    break;
                        case  31: // z2
                                    double_3D_2.z = double_value;
                                    break;
                        case  40: // ratio between major and minor axis
                                    radius = double_value;
                                    break;
                        case  41: // start angle
                                    arc_start = double_value;
                                    break;
                        case  42: // end angle
                                    arc_end = double_value;
                                    break;
                        case  62: // dxf_color_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                    }
                }
                if(arc_start >= arc_end) arc_end += two_pi;
                arc_angle  = arc_end - arc_start;
                arc_vertices = (int)ceil(arc_angle / (p_space->rendered_arc_angle * one_degree));
                arc_step   = arc_angle / arc_vertices;
                double major_axis = double_3D_2.magnitude(),
                      minor_axis = major_axis * radius;
                double_3D_3   = 0.0;
                double_3D_3.z = -double_3D_2.direction().z;
                real_vertices.clear();
                double_3D_4.x = -major_axis * cos(arc_start);
                double_3D_4.y = -minor_axis * sin(arc_start);
                double_3D_4.z = double_3D_1.z;
                real_vertices.push_back(RealVertex(double_3D_4, color).blank());
                for(a = 1; a <= arc_vertices; a++)
                {
                    double_3D_4.x = -major_axis * cos(a * arc_step + arc_start);
                    double_3D_4.y = -minor_axis * sin(a * arc_step + arc_start);
                    double_3D_4.z = double_3D_1.z;

                    real_vertices.push_back(RealVertex(double_3D_4, color).unblank());
                }
                if(double_3D_3 != 0.0)
                    for(vertex_index = 0; vertex_index < real_vertices.size(); vertex_index++)
                        real_vertices[vertex_index] = rotate_vertex(real_vertices[vertex_index], double_3D_3);
                for(vertex_index = 0; vertex_index < real_vertices.size(); vertex_index++)
                    push_back(real_vertices[vertex_index] + double_3D_1);
            }
            //----------------------------------------------------------------
            else if(entity_string == "POLYLINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  62: // dxf_color_index
                                    dxf_color_index = (u_char)double_value;
                                    prev_dxf_color_index = dxf_color_index;
                                    prev_color = p_space->palette_picker(LASERBOY_DXF)[dxf_color_index];
                                    if(prev_color.is_black())
                                        prev_color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    color = p_space->palette_picker(LASERBOY_DXF)[dxf_color_index];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    prev_color = color;
                                    break;
                        case  70:
                                    if(((int)double_value) & 0x01)
                                        closed_polyline = true;
                                    break;
                    }
                }
                while(entity_string == "VERTEX")
                {
                    while(get_dxf_pair(in, group_code, entity_string))
                    {
                        sscanf(entity_string.c_str(), "%lf", &double_value);
                        switch(group_code)
                        {
                            case  10: // x1
                                        double_3D_1.x = double_value;
                                        break;
                            case  20: // y1
                                        double_3D_1.y = double_value;
                                        break;
                            case  30: // z1
                                        double_3D_1.z = double_value;
                                        break;
                            case  62: // dxf_color_index
                                        prev_dxf_color_index = dxf_color_index;
                                        dxf_color_index = (u_char)double_value;
                                        prev_color = p_space->palette_picker(LASERBOY_DXF)[prev_dxf_color_index];
                                        if(prev_color.is_black())
                                            prev_color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                        color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                        if(color.is_black())
                                            color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                        break;
                            case 420: // true color
                                        prev_color = color;
                                        color = (u_int)double_value;
                                        if(color.is_black())
                                            color = (u_int)0X00ffffff;
                                        break;
                        }
                    }
                    if(first_segment_vertex_index == -1)
                    {
                        push_back(RealVertex(double_3D_1, color).blank());
                        first_segment_vertex_index = (int)size() - 1;
                    }
                    else
                        push_back(RealVertex(double_3D_1, prev_color).unblank());
                } // end while(entity_string == "VERTEX")
                if(closed_polyline)
                    push_back(RealVertex((Double3d)(at(first_segment_vertex_index)), back().as_Color()).unblank());
            }
            //----------------------------------------------------------------
            else if(entity_string == "LWPOLYLINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x
                                    vertex_x.push_back(double_value);
                                    break;
                        case  20: // y
                                    vertex_y.push_back(double_value);
                                    break;
                        case  62: // dxf_color_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                        case  70: // is closed polyline
                                    group_70_flag = (int)double_value;
                                    break;
                        case  90: // number of verticies
                                    arc_vertices = (int)double_value;
                                    break;
                    }
                }
                if(    (arc_vertices > 0)
                    && (arc_vertices == vertex_x.size())
                    && (arc_vertices == vertex_y.size())
                  )
                {
                    double_3D_1.x = vertex_x.front();
                    double_3D_1.y = vertex_y.front();
                    double_3D_1.z = 0;
                    push_back(RealVertex(double_3D_1, color).blank());
                    for(u_int a = 0; a < arc_vertices; a++)
                    {
                        double_3D_1.x = vertex_x[a];
                        double_3D_1.y = vertex_y[a];
                        push_back(RealVertex(double_3D_1, color).unblank());
                    }
                    if(group_70_flag & 0x01) // closed polyline
                    {
                        double_3D_1.x = vertex_x.front();
                        double_3D_1.y = vertex_y.front();
                        push_back(RealVertex(double_3D_1, color).unblank());
                    }
                }
            }
            //----------------------------------------------------------------
            else if(entity_string == "TEXT")
            {
                int                text_index,
                                   vertex_index;
                long int           bytes_skipped;
                FrameSet font_frames(p_space);
                double_3D_1 = 0.0;
                double_3D_3 = 0.0;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case   1: // the text
                                    text = entity_string;
                                    break;
                        case   7: // the font
                                    font = entity_string;
                                    break;
                        case  10: // x1
                                    double_3D_1.x = double_value;
                                    break;
                        case  20: // y1
                                    double_3D_1.y = double_value;
                                    break;
                        case  30: // z1
                                    double_3D_1.z = double_value;
                                    break;
                        case  40: // text height
                                    radius = double_value;
                                    break;
                        case  50: // flat rotation angle
                                    double_3D_3.z = double_value * one_degree;
                                    break;
                        case  51: // oblique rotation angle
                                    double_3D_3.y = double_value * one_degree;
                                    break;
                        case  62: // dxf_color_index
                                    color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                    if(color.is_black())
                                        color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                    break;
                        case 420: // true color
                                    color = (u_int)double_value;
                                    if(color.is_black())
                                        color = (u_int)0X00ffffff;
                                    break;
                        case  71: // flipped or mirrored
                                    group_71_flag = (int)double_value;
                                    break;
                        case  72: // horizontal justification
                                    group_72_flag = (int)double_value;
                                    break;
                        case  73: // vertical justification
                                    group_73_flag = (int)double_value;
                                    break;
                    }
                }
                //------------------------------------------------------------
                if(font == "ARIAL")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_arial.ild", bytes_skipped);

                else if(font == "COMIC_SANS_MS")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_comic_sans.ild", bytes_skipped);

                else if(font == "COURIER_NEW")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_courier_new.ild", bytes_skipped);

                else if(font == "LUCIDA_CONSOLE")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_lucida.ild", bytes_skipped);

                else if(font == "IMPACT")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_impact.ild", bytes_skipped);

                else if(font == "TIMES_NEW_ROMAN")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_times_new_roman.ild", bytes_skipped);

                else
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + "font_narrow_vector.ild", bytes_skipped);

                if(    font_frames.frame_set_error != LASERBOY_OK
                    || font_frames.number_of_frames() < (('~' - '!') + 1)
                  )
                {
                    real_segment_error = LASERBOY_BAD_FONT_FILE;
                }
                else
                {
                    real_vertices.clear();
                    for(text_index = 0; text_index < (int)text.size(); text_index++)
                    {
                        if(text[text_index] >= '!' && text[text_index] <= '~')
                        {
                            for(vertex_index = 0; vertex_index < (int)font_frames[text[text_index] - '!'].size(); vertex_index++)
                            {
                                double_3D_2 = font_frames[text[text_index] - '!'].at(vertex_index);
                                //------------------------------------------------
                                double_3D_2.x = (   (   double_3D_2.x
                                                      / (   p_space->rendered_mono_font_space
                                                          * LASERBOY_MAX_SHORT
                                                        )
                                                      + (double)(text_index)
                                                    )
                                                ) * radius;
                                //------------------------------------------------
                                double_3D_2.y = (   (   double_3D_2.y
                                                      / (   p_space->rendered_mono_font_space
                                                          * LASERBOY_MAX_SHORT
                                                        )
                                                    )
                                                ) * radius;
                                //------------------------------------------------
                                real_vertices.push_back(   (font_frames[text[text_index] - '!'].at(vertex_index).is_blank())
                                                         ? (RealVertex(double_3D_2, color).blank())
                                                         : (   (font_frames[text[text_index] - '!'].at(vertex_index).is_black(p_space->black_level))
                                                             ? (RealVertex(double_3D_2, Color(0,0,0)).unblank())
                                                             : (RealVertex(double_3D_2, color).unblank())
                                                           )
                                                       );
                            }
                        }
                    }
                    if(group_71_flag & 2) // mirrored
                        real_vertices.flip(0);
                    if(group_71_flag & 4) // flipped
                        real_vertices.flip(1);
                    if(double_3D_3 != 0.0)
                        for(vertex_index = 0; vertex_index < (int)real_vertices.size(); vertex_index++)
                            real_vertices[vertex_index] = rotate_vertex(real_vertices[vertex_index], double_3D_3);
                    for(vertex_index = 0; vertex_index < (int)real_vertices.size(); vertex_index++)
                        push_back(real_vertices[vertex_index] + double_3D_1);
                }
            } // end else if(entity_string == "TEXT")
            //----------------------------------------------------------------
            else if(entity_string == "ENDSEC")
                break;
            //----------------------------------------------------------------
            else
                get_dxf_pair(in, group_code, entity_string);
        } // end while(in.good())
    } // end if(in.good())
    //------------------------------------------------------------------------
    else
    {
        real_segment_error = LASERBOY_EOF;
    }
    //------------------------------------------------------------------------
    return real_segment_error;
}

//############################################################################
ErrorCode RealSegment::from_ifstream_txt(ifstream& in,
                                                             const u_int& group_type,
                                                             u_int& line_number
                                                            )
{
    RealVertex real_vertex;
    //------------------------------------------------------------------------
    if(    group_type == LASERBOY_3D_FRAME_PALETTE
        || group_type == LASERBOY_2D_FRAME_PALETTE
      )
        palette_index = p_space->palette_index;
    else
        palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    clear();
    push_back(RealVertex());
    push_back(RealVertex());
    //------------------------------------------------------------------------
    if(in.good())
    {
        real_segment_error = LASERBOY_OK;
        while(real_vertex.from_ifstream_txt(in, group_type, line_number))
            push_back(real_vertex);
        sync_rgb_and_palette();
    }
    else
        real_segment_error = LASERBOY_EOF;
    //------------------------------------------------------------------------
    return real_segment_error;
}

//############################################################################
void RealSegment::normalize()
{
    u_int               i;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    Double3d  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX,
                        real_offset =  0.0;

    if(p_space->auto_scale_real)
    {
        real_min =  DBL_MAX;
        real_max = -DBL_MAX;
        for(i = 2; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
        }
        if(p_space->maintain_real_origin)
        {
            if(fabs(real_max.x) > real_size)    real_size = fabs(real_max.x);
            if(fabs(real_min.x) > real_size)    real_size = fabs(real_min.x);
            if(fabs(real_max.y) > real_size)    real_size = fabs(real_max.y);
            if(fabs(real_min.y) > real_size)    real_size = fabs(real_min.y);
            if(fabs(real_max.z) > real_size)    real_size = fabs(real_max.z);
            if(fabs(real_min.z) > real_size)    real_size = fabs(real_min.z);
            real_size *= 2;
        }
        else // find the new center of the universe
        {
            real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
            real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
            real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));

            if(fabs(real_max.x - real_min.x) > real_size)
                real_size = fabs(real_max.x - real_min.x);

            if(fabs(real_max.y - real_min.y) > real_size)
                real_size = fabs(real_max.y - real_min.y);

            if(fabs(real_max.z - real_min.z) > real_size)
                real_size = fabs(real_max.z - real_min.z);
        }
        real_scale = 65000.0 / real_size; // a bit smaller than short int space
    } // end if(auto_scale_real)
    //--------------------------------------------------------------------
    for(i = 2; i < size(); i++) // ignore the origin vector
    {
        at(i).x = (short)((at(i).x - real_offset.x) * real_scale);
        at(i).y = (short)((at(i).y - real_offset.y) * real_scale);
        at(i).z = (short)((at(i).z - real_offset.z) * real_scale);
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
void RealSegmentSet::normalize()
{
    u_int               i,
                        j;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    Double3d  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX,
                        real_offset =  0.0;

    if(p_space->auto_scale_real)
    {
        for(j = 0; j < size(); j++)
        {
            for(i = 2; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        if(p_space->maintain_real_origin)
        {
            if(fabs(real_max.x) > real_size)    real_size = fabs(real_max.x);
            if(fabs(real_min.x) > real_size)    real_size = fabs(real_min.x);
            if(fabs(real_max.y) > real_size)    real_size = fabs(real_max.y);
            if(fabs(real_min.y) > real_size)    real_size = fabs(real_min.y);
            if(fabs(real_max.z) > real_size)    real_size = fabs(real_max.z);
            if(fabs(real_min.z) > real_size)    real_size = fabs(real_min.z);
            real_size *= 2;
        }
        else // find the new center of the universe
        {
            real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
            real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
            real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));
            if(fabs(real_max.x - real_min.x) > real_size)    real_size = fabs(real_max.x - real_min.x);
            if(fabs(real_max.y - real_min.y) > real_size)    real_size = fabs(real_max.y - real_min.y);
            if(fabs(real_max.z - real_min.z) > real_size)    real_size = fabs(real_max.z - real_min.z);
        }
        if(real_size)
            real_scale = 65000.0 / real_size; // a bit smaller than short int space
        else
            real_scale = 1.0;
    } // end if(auto_scale_real)
    //------------------------------------------------------------
    for(j = 0; j < size(); j++)
    {
        for(i = 2; i < at(j).size(); i++) // ignore the origin vector
        {
            at(j)[i].x = (short)((at(j)[i].x - real_offset.x) * real_scale);
            at(j)[i].y = (short)((at(j)[i].y - real_offset.y) * real_scale);
            at(j)[i].z = (short)((at(j)[i].z - real_offset.z) * real_scale);
        }
    }
    //--------------------------------------------------------------------
    return;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
