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
// LaserBoy_frame.cpp is part of LaserBoy.
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
#include "LaserBoy_GUI_base.hpp"

//############################################################################
LaserBoy_frame::LaserBoy_frame(LaserBoy_space* ps,
                               LaserBoy_vertex(*F)(int, int),
                               int vertices_per_frame,
                               int frame_index
                              )
               : LaserBoy_segment (ps   )
               , is_selected      (false)
               , is_unique        (true )
               , is_wagged        (false)
               , egg              (0    )
               , spider           (0    )
               , intro            (ps   )
               , bridge           (ps   )
               , coda             (ps   )
{
    is_selected            = false;
    is_unique              = true;
    is_wagged              = false;
    palette_index          = LASERBOY_LASERBOY_HUES;
    format                 = LASERBOY_3D_FRAME;
    LaserBoy_vertex vertex = F(0, frame_index);
    vertex.blank();
    reserve(vertices_per_frame + 2);
    push_back(vertex);
    for(int p = 0; p < vertices_per_frame; p++)
        push_back(F(p, frame_index));
    push_back(vertex);
}

//############################################################################
bool LaserBoy_frame::save_as_ild(const string& file)
{
    if(palette_index != LASERBOY_ILDA_DEFAULT)
        return p_space->save_as_ild(file);
    //------------------------------------------------------------------------
    if(!name.size())
        name = "ESI00000";
    //------------------------------------------------------------------------
    ofstream out(file.c_str(), ios::out | ios::binary);
    if(out.is_open())
    {
        if(p_space->auto_minimize)
        {
            LaserBoy_frame copy(*this);
            copy.minimize(0);
            copy.to_ofstream_ild(out);
        }
        else
            to_ofstream_ild(out);
        out.close();
    }
    else
        return false;
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
bool LaserBoy_frame::save_as_txt(const string& file)
{
    ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        //--------------------------------------------------------------------
        LaserBoy_frame copy(*this);
        if(p_space->auto_minimize)
            copy.minimize(0);
        //--------------------------------------------------------------------
        if(!p_space->save_txt_with_color)
        {
            if(copy.palette_index != LASERBOY_TRUE_COLOR)
            {
                if(    p_space->save_txt_named_palettes
                    || copy.palette_index >= LASERBOY_BUILT_IN_PALETTES
                  )
                    p_space->palette_picker(copy.palette_index).to_ofstream_txt(out);
                else
                {
                    out << "palette named "
                        << p_space->palette_picker(copy.palette_index).name
                        << ENDL
                        << ENDL;
                }
            }
            else
                copy.to_ofstream_txt_color_table(out);
        }
        copy.to_ofstream_txt(out, 0);
        out.close();
    }
    else
        return false;
        //--------------------------------------------------------------------
    return true;
}

//############################################################################
bool LaserBoy_frame::save_as_txt_table(const string& file)
{
    ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        to_ofstream_txt_color_table(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::optimize(LaserBoy_3D_short point_of_entry)
{
    int                  i,
                         j,
                         dwell_samples;
    LaserBoy_3D_double   _0,
                         _1,
                         _2;
    LaserBoy_vertex      vertex,
                         black_vertex;
    LaserBoy_Error_Code  stat = LASERBOY_OK;
    //------------------------------------------------------------------------
    vertex = front();
    vertex.blank();
    //------------------------------------------------------------------------
    stat |= add_dwell();
    if(p_space->dots_setting == LASERBOY_DOTS_ENHANCE)
        stat |= enhance_dots();
    stat |= add_lit_span_vertices();
    stat |= add_blank_span_vertices();
    //------------------------------------------------------------------------
    is_wagged = false;
    //------------------------------------------------------------------------
    // intro from the origin to the first vertex
    //------------------------------------------------------------------------
    intro.clear();
    intro += LaserBoy_segment(p_space, point_of_entry, vertex); // line from entry point to first vertex
    //------------------------------------------------------------------------
    _0 = point_of_entry;
    _1 =
    _2 = front();
    //------------------------------------------------------------------------
    j  = 0;
    while(    (_1 == _2)
           && (++j < (int)size())
         )
        _2 = at(j);
    //------------------------------------------------------------------------
    if(_0 == _1)
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = start_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
    else
        dwell_samples = start_dwell_vertices(delta_angle(_0 | _2,
                                                         _0 | _1,
                                                         _1 | _2
                                                        ),
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
    if(dwell_samples) // dwell from the line to the first lit vector
        for(i = 0; i < dwell_samples; i++)
            intro.push_back(vertex); // front() all blank
    //------------------------------------------------------------------------
    // bridge to join the end back to the biginning
    //------------------------------------------------------------------------
    bridge.clear();
    _0 =
    _1 = back();
    _2 = front();
    //------------------------------------------------------------------------
    j  = 0;
    while(    (_1 == _2)
           && (++j < (int)size())
         )
        _2 = at(j);
    //------------------------------------------------------------------------
    j  = size() - 1;
    while(    (_0 == _1)
           && (--j >= 0)
         )
        _0 = at(j);
    //------------------------------------------------------------------------
    if(    (_0 == _1)
        || (_1 == _2)
      )
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = end_dwell_vertices(pi,
                                           p_space->sample_rate,
                                           p_space->max_dwell_microsec
                                          );
    else
        dwell_samples = end_dwell_vertices(delta_angle(_0 | _2,
                                                       _0 | _1,
                                                       _1 | _2
                                                      ),
                                           p_space->sample_rate,
                                           p_space->max_dwell_microsec
                                          );
    if(dwell_samples)
    {
        vertex = black_vertex = back();
        if(p_space->black_dwell_vertices)
        {
            black_vertex.c = p_space->palette_picker(palette_index).black;
            black_vertex.r =
            black_vertex.g =
            black_vertex.b = 0x00;
        }
        for(i = 0; i < dwell_samples; i++)
            if(i < (int)p_space->lit_dwell_overhang)
                bridge.push_back(vertex); // dwell on last lit vector
            else
                bridge.push_back(black_vertex);
    } // end if(dwell_samples)
    //------------------------------------------------------------------------
    if(total_magnitude() > p_space->insignificant_distance)
    {
        vertex = front();
        vertex.blank();
        bridge += LaserBoy_segment(p_space, back(), vertex); // line from last vertex to first for repete
        //--------------------------------------------------------------------
        _0 = back();
        _1 =
        _2 = front();
        //--------------------------------------------------------------------
        j  = 0;
        while(    (_1 == _2)
               && (++j < (int)size())
             )
            _2 = at(j);
        //--------------------------------------------------------------------
        j  = size() - 1;
        while(    (_0 == _1)
               && (--j >= 0)
             )
            _0 = at(j);
        //--------------------------------------------------------------------
        if(    (_0 == _1)
            || (_1 == _2)
          )
            dwell_samples = 0;
        else if(_0 == _2)
            dwell_samples = start_dwell_vertices(pi,
                                                 p_space->sample_rate,
                                                 p_space->max_dwell_microsec
                                                );
        else
            dwell_samples = start_dwell_vertices(delta_angle(_0 | _2,
                                                             _0 | _1,
                                                             _1 | _2
                                                            ),
                                                 p_space->sample_rate,
                                                 p_space->max_dwell_microsec
                                                );
        if(dwell_samples) // dwell from the line to the front()
        {
            vertex = front();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
        }
    }
    //------------------------------------------------------------------------
    if(length_in_time() < (p_space->max_wag_microsec / 1000000.0))
    {
        dwell_samples = start_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
        if((int)bridge.size() > 2 * dwell_samples)
        {
            bridge.clear();
            vertex = front();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
            bridge += *this;
            vertex = back();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
            bridge.reverse();
            is_wagged = true;
        }
    }
    return stat;
}

//############################################################################
void LaserBoy_frame::add_coda(LaserBoy_3D_short next_frame_entry_point)
{
    int                 i,
                        j,
                        dwell_samples;
    LaserBoy_3D_double  _0,
                        _1,
                        _2;
    LaserBoy_vertex     vertex,
                        black_vertex;
    //------------------------------------------------------------------------
    // coda is dwell only at the last point before moving toward netx frame entry
    //------------------------------------------------------------------------
    coda.clear();
    //------------------------------------------------------------------------
    _0 = at(size() - 2);
    _1 = back();
    _2 = next_frame_entry_point;
    j  = size() - 1;
    while(    (_0 == _1)
           && (--j >= 0)
         )
        _0 = at(j);
    //------------------------------------------------------------------------
    if(_1 == _2)
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = end_dwell_vertices(pi,
                                           p_space->sample_rate,
                                           p_space->max_dwell_microsec
                                          );
    else
        dwell_samples = end_dwell_vertices(delta_angle(_0 | _2,
                                                       _0 | _1,
                                                       _1 | _2
                                                      ),
                                           p_space->sample_rate,
                                           p_space->max_dwell_microsec
                                          );
    //------------------------------------------------------------------------
    vertex = black_vertex = back();
    if(p_space->black_dwell_vertices)
    {
        black_vertex.c = p_space->palette_picker(palette_index).black;
        black_vertex.r =
        black_vertex.g =
        black_vertex.b = 0x00;
    }
    if(dwell_samples > 2)
        for(i = 0; i < (int)dwell_samples; i++)
        {
            if(i < (int)p_space->lit_dwell_overhang)
                coda.push_back(vertex);
            else
                coda.push_back(black_vertex);
        }
    else // coda must exist to convey end_of_frame in optimized wave!
    {
       coda.push_back(black_vertex);
       coda.push_back(black_vertex);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::from_ifstream_format_3(ifstream& in,
                                            const LaserBoy_ild_header& header,
                                            long int& bytes_skipped
                                           )
{
    clear();
    *this = header;
    //------------------------------------------------------------------------
    if(quantity)
    {
        u_int                i;
        LaserBoy_color       color;
        LaserBoy_vertex      vertex;
        LaserBoy_ild_header  next_header;
        //--------------------------------------------------------------------
        reserve(quantity);
        for(i = 0; i < quantity; i++)
            if(color.from_ifstream_ild(in))
                push_back((LaserBoy_vertex)color);
            else
                return false;
        //--------------------------------------------------------------------
        if(next_header.from_ifstream_ild(in, bytes_skipped))
        {
            if(quantity != next_header.quantity)
                segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                
            if(quantity < next_header.quantity)
                reserve(next_header.quantity);

            *this = next_header;

            for(i = 0; i < next_header.quantity; i++)
            {
                if(vertex.from_ifstream_ild(in, next_header.format))
                {
                    at(i).x = vertex.x;
                    at(i).y = vertex.y;
                    at(i).z = vertex.z;
                    at(i).k = vertex.k;
                    at(i).c = vertex.c;
                }
                else
                    return false;
            }
            //--------------------------------------------------------------------
            front().blank();
            //--------------------------------------------------------------------
            palette_index = LASERBOY_TRUE_COLOR;
            sync_rgb_and_palette();
        }
        else
            return false;
    }
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
bool LaserBoy_frame::from_ifstream_ild(ifstream& in,
                                       const LaserBoy_ild_header& header
                                      )
{
    u_int            i;
    LaserBoy_vertex  vertex;
    //------------------------------------------------------------------------
    clear();
    *this = header;
    //------------------------------------------------------------------------
    if(    format == LASERBOY_3D_FRAME_RGB
        || format == LASERBOY_2D_FRAME_RGB
      )
        palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(quantity)
    {
        reserve(quantity);
        for(i = 0; i < quantity; i++)
            if(vertex.from_ifstream_ild(in, format))
                push_back(vertex);
            else
                return false;
        //--------------------------------------------------------------------
        front().blank();
    }
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::from_ifstream_dxf(ifstream& in)
{
    u_int            i;
    LaserBoy_vertex  vertex;

    LaserBoy_real_segment real_vertices(p_space);
    //------------------------------------------------------------------------
    format        = LASERBOY_3D_FRAME ;
    palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    segment_error |= real_vertices.from_ifstream_dxf(in);
    //------------------------------------------------------------------------
    if(real_vertices.size() > 2) // more than the default original vector
    {
        real_vertices.normalize();
        reserve(real_vertices.size() - 2);
        for(i = 2; i < real_vertices.size(); i++)
            push_back((LaserBoy_vertex)real_vertices[i]);
    }
    else
    {
        reserve(2);
        push_back(vertex);
        push_back(vertex);
    }
    //------------------------------------------------------------------------
    is_2D(); // if so change format to LASERBOY_2D_FRAME
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::from_ifstream_txt(ifstream& in,
                                                      const u_int& group_type,
                                                      const u_int& element_type,
                                                      u_int& line_number
                                                     )
{   //------------------------------------------------------------------------
    switch(group_type)
    {
        case LASERBOY_TABLE:
                format        = LASERBOY_TABLE;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_RGB:
                format        = LASERBOY_3D_FRAME_RGB;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_RGB:
                format        = LASERBOY_2D_FRAME_RGB;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_HEX:
                format        = LASERBOY_3D_FRAME_RGB;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_HEX:
                format        = LASERBOY_2D_FRAME_RGB;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_PALETTE:
                format        = LASERBOY_3D_FRAME;
                palette_index = p_space->palette_index;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_PALETTE:
                format        = LASERBOY_2D_FRAME;
                palette_index = p_space->palette_index;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_TABLE:
                format        = LASERBOY_3D_FRAME;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_TABLE:
                format        = LASERBOY_2D_FRAME;
                palette_index = LASERBOY_TRUE_COLOR;
                break;
    }
    //------------------------------------------------------------------------
    if(    group_type == LASERBOY_3D_FRAME_TABLE
        || group_type == LASERBOY_2D_FRAME_TABLE // we already have r g b
      )
    {
        u_int i;
        LaserBoy_vertex vertex;
        for(i = 0; i < size(); i++)
            if(!at(i).from_ifstream_txt(in, group_type, element_type, line_number))
                break;
        if(i < size()) // too few vertices
            segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
        i = 0;
        while(vertex.from_ifstream_txt(in, group_type, element_type, line_number))
            i++;
        if(i > 0)
            segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
    }
    //------------------------------------------------------------------------
    else
    {
        LaserBoy_vertex vertex;
        clear();
        while(vertex.from_ifstream_txt(in, group_type, element_type, line_number))
            push_back(vertex);
        if(    group_type == LASERBOY_3D_FRAME_PALETTE
            || group_type == LASERBOY_2D_FRAME_PALETTE
          )
            set_rgb_from_palette();
    }
    //------------------------------------------------------------------------
    return segment_error;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::move_selection(LaserBoy_3D_double d, bool check_bounds)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;
        //--------------------------------------------------------------------
        if(check_bounds)
            for(i = 0; i < size(); i++)
            {
                if(is_index_selected(i))
                {
                    out_of_bounds = LaserBoy_bounds_check(d + at(i).as_3D_short(), LASERBOY_CUBE);
                    if(out_of_bounds)
                        return out_of_bounds;
                }
            }
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                at(i) = LaserBoy_vertex(d + at(i).as_3D_short(),
                                        at(i).as_LaserBoy_color(),
                                        at(i).k,
                                        at(i).c
                                       );
            }
        }
        //--------------------------------------------------------------------
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::scale_selection(LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  f,
                            center = selected_segment().mean_of_coordinates();

        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                out_of_bounds |= LaserBoy_bounds_check(((f - center) * m) + center, LASERBOY_CUBE);
            }
        }
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                at(i) = LaserBoy_vertex( ((f - center) * m) + center,
                                       (LaserBoy_color)at(i),
                                       at(i).k,
                                       at(i).c
                                     );
            }
        }
        //--------------------------------------------------------------------
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::scale_selection_on_fulcrum(LaserBoy_3D_double magnitude)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  f;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                out_of_bounds |= LaserBoy_bounds_check(((f - p_space->fulcrum) * magnitude) + p_space->fulcrum, LASERBOY_CUBE);
            }
        }
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                at(i) = LaserBoy_vertex( ((f - p_space->fulcrum) * magnitude) + p_space->fulcrum,
                                       (LaserBoy_color)at(i),
                                       at(i).k,
                                       at(i).c
                                     );
            }
        }
        //--------------------------------------------------------------------
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::rotate_selection(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  f,
                            center = selected_segment().mean_of_coordinates();

        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(f, center, a), LASERBOY_CUBE);
            }
        }
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates(f, center, a),
                                       (LaserBoy_color)at(i),
                                       at(i).k,
                                       at(i).c
                                     );
            }
        }
        //--------------------------------------------------------------------
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::rotate_selection_on_fulcrum(LaserBoy_3D_double angle)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  f;

        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(f, p_space->fulcrum, angle), LASERBOY_CUBE);
            }
        }
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            if(is_index_selected(i))
            {
                f = at(i).as_3D_short();
                at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates(f, p_space->fulcrum, angle),
                                       (LaserBoy_color)at(i),
                                       at(i).k,
                                       at(i).c
                                     );
            }
        }
        //--------------------------------------------------------------------
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
void LaserBoy_frame::next_segment_select()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    find_segment_at_index(segment_index + 1, egg, spider);
    return;
}

//############################################################################
void LaserBoy_frame::next_segment_egg()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    find_segment_at_index(segment_index + 1, egg, end);
    return;
}

//############################################################################
void LaserBoy_frame::next_segment_spider()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(spider, start, end, segment_index);
    find_segment_at_index(segment_index + 1, start, spider);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_select()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, egg, spider);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_egg()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, egg, end);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_spider()
{
    u_int segment_index, start, end;
    find_segment_of_vertex(spider, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, start, spider);
    return;
}

//############################################################################
void LaserBoy_frame::color_select()
{
    if(size() > 1)
    {
        u_int i;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_index_select()
{
    if(size() > 1)
    {
        u_int i,
              num_colors,
              j = 0;
              
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            num_colors = p_space->palette_picker(palette_index).last;
            num_colors++; // you can't do modulus with ZERO!
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[j % num_colors].r;
                    at(i).g = p_space->palette_picker(palette_index)[j % num_colors].g;
                    at(i).b = p_space->palette_picker(palette_index)[j % num_colors].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            num_colors = p_space->palette_picker(p_space->target_palette_index).last;
            num_colors++; // you can't do modulus with ZERO!
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_span_select()
{
    if(size() > 1)
    {
        u_int i,
              j = 0,
              selected = 0,
              span   = egg <= spider ? egg    : spider,
              offset = egg <= spider ? spider : egg   ;
        //--------------------------------------------------------------------
        for(i = span; i <= offset; i++)
            if(at(i).is_color(p_space->black_level))
                selected++;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            span   =    p_space->palette_picker(p_space->target_palette_index).last
                      - p_space->palette_picker(p_space->target_palette_index).first,
            offset =    p_space->palette_picker(p_space->target_palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[at(i).c].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_index_black()
{
    if(size() > 1)
    {
        u_int i,
              j = 0;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].r;
                    at(i).g = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].g;
                    at(i).b = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_span_black()
{
    if(size() > 1)
    {
        u_int i,
              j = 0,
              selected = 0,
              span   = egg <= spider ? egg    : spider,
              offset = egg <= spider ? spider : egg   ;
        //--------------------------------------------------------------------
        for(i = span; i <= offset; i++)
            if(at(i).is_black(p_space->black_level))
                selected++;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            span   =    p_space->palette_picker(p_space->target_palette_index).last
                      - p_space->palette_picker(p_space->target_palette_index).first,
            offset =    p_space->palette_picker(p_space->target_palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[at(i).c].b;
                    j++;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_black_in_select()
{
    if(size() > 1)
    {
        u_int i;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(is_index_selected(i) && at(i).is_black(p_space->black_level))
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(is_index_selected(i) && at(i).is_black(p_space->black_level))
                {
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_cycle_select(int steps)
{
    if(size() > 1)
    {
        u_int i,
              span,
              offset;
        //--------------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            while(span > 0 && steps < 0)
                steps += span;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (at(i).c - offset + steps) % span + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                }
        }
        //--------------------------------------------------------------------
        else
        {
            LaserBoy_segment selected_vectors(p_space);
            vector<int>      selected_vectors_index;

            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    selected_vectors.push_back(at(i));
                    selected_vectors_index.push_back(i);
                }

            while(selected_vectors.size() > 0 && steps < 0)
                steps += selected_vectors.size();

            for(i = 0; i < selected_vectors.size(); i++)
            {
                at(selected_vectors_index[i]).r = selected_vectors[(i + steps) % selected_vectors.size()].r;
                at(selected_vectors_index[i]).g = selected_vectors[(i + steps) % selected_vectors.size()].g;
                at(selected_vectors_index[i]).b = selected_vectors[(i + steps) % selected_vectors.size()].b;
            }
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::blank_vertices()
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
            if(is_index_selected(i))
                at(i).blank();
    }
    return;
}

//############################################################################
void LaserBoy_frame::unblank_vertices()
{
    if(size() > 1)
    {
        for(u_int i = 1; i < size(); i++)
            if(is_index_selected(i))
                at(i).unblank();
    }
    return;
}

//############################################################################
void LaserBoy_frame::black_vertices()
{
    if(size() > 1)
    {
        for(u_int i = 1; i < size(); i++)
            if(is_index_selected(i) && at(i).is_blank())
            {
                at(i).r = at(i).g = at(i).b = 0x00;
                at(i).c = p_space->palette_picker(palette_index).black;
                at(i).unblank();
            }
    }
    return;
}

//############################################################################
void LaserBoy_frame::unblack_vertices()
{
    if(size() > 1)
    {
        for(u_int i = 1; i < size(); i++)
            if(is_index_selected(i) && at(i).color_of(p_space->black_level) == 0x00) // lit black
                at(i).blank();
    }
    return;
}

//############################################################################
void LaserBoy_frame::minimize(int frame_index)
{
    LaserBoy_segment entry(p_space);
    //------------------------------------------------------------------------
    if(frame_index <= 0)
    {
        entry.push_back(LaserBoy_vertex());
        entry.push_back(LaserBoy_vertex()); // the orginal vector
    }
    else
        entry = p_space->frame_picker(frame_index - 1);
    //------------------------------------------------------------------------
    intro.clear();
    bridge.clear();
    coda.clear();
    //------------------------------------------------------------------------
    if(!p_space->allow_lit_black)
        convert_black_to_blank();
    //------------------------------------------------------------------------
    remove_dwell_vertices();
    reduce_blank_vectors ();
    remove_short_vectors ();
    reduce_lit_vectors   ();
    //------------------------------------------------------------------------
    if(p_space->no_equivalent_vectors)
    {
        omit_equivalent_vectors ();
        reduce_blank_vectors    ();
    }
    //------------------------------------------------------------------------
    if(p_space->dots_setting == LASERBOY_DOTS_REMOVE)
        remove_dots();
    //------------------------------------------------------------------------
    if(p_space->conglomerate_in_minimize == LASERBOY_DOTS_REMOVE)
        conglomerate_lit_segments();
    //------------------------------------------------------------------------
    reorder_segments(entry);
    bond_segments    ();
    normalize_cursors();
    //------------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::add_vertex(u_int vertex_index)
{
    if(    !LaserBoy_segment::add_vertex(vertex_index)
        && size() > 2
        && vertex_index < size()
      )
    {
        if((at(vertex_index) | at(vertex_index + 1)) > p_space->insignificant_distance)
            spider = vertex_index + 1;
        else
            spider = vertex_index;
    }
    else
        egg = spider = vertex_index = 0;
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::break_segment()
{
    LaserBoy_segment::break_segment(spider); // sets spider
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::set_spider_to_egg()
{
    at(spider) = (LaserBoy_3D_short)at(egg);
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::set_egg_to_spider()
{
    at(egg) = (LaserBoy_3D_short)at(spider);
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::connect_the_dots()
{
    LaserBoy_segment::connect_the_dots(egg, spider);
    return segment_error;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::to_dots()
{
    u_int           i;
    LaserBoy_frame  frame(p_space, palette_index, false);
    //------------------------------------------------------------------------
    frame.reserve(size() + size_of_selection());
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
    {
        if(    at(i).is_lit()
            && at_index_has_magnitude(i)
          )
        {
            frame += at(i);
            frame.back().r = 0;
            frame.back().g = 0;
            frame.back().b = 0;
            frame.back().c = p_space->palette_picker(palette_index).black;
        }
        frame += at(i);
    }
    //------------------------------------------------------------------------
    *this = frame;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::selected_to_dots()
{
    u_int           i,
                    new_spider = spider;
    LaserBoy_frame  frame(p_space, palette_index, false);
    //------------------------------------------------------------------------
    frame.reserve(size() + size_of_selection());
    //------------------------------------------------------------------------
    frame.push_back(front());
    for(i = 1; i < size(); i++)
    {
        if(    is_index_selected(i)
            && at(i).is_lit()
            && at_index_has_magnitude(i)
          )
        {
            frame += at(i);
            frame.back().r = 0;
            frame.back().g = 0;
            frame.back().b = 0;
            frame.back().c = p_space->palette_picker(palette_index).black;
            new_spider++;
        }
        frame += at(i);
    }
    //------------------------------------------------------------------------
    spider = new_spider;
    *this = frame;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::remove_at_spider()
{
    int new_spider = spider;
    LaserBoy_segment::remove_vertex(spider);
    spider = new_spider - 1;
    normalize_cursors();
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::remove_vertex(u_int vertex_index)
{
    LaserBoy_segment::remove_vertex(vertex_index);
    normalize_cursors();
    return *this;
}

//############################################################################
void LaserBoy_frame::to_ofstream_ild(ofstream& out)
{
    char number[8];
    quantity = (u_short)size();
    owner    = "LaserBoy";
    //------------------------------------------------------------------------
    if(is_2D())
        format = LASERBOY_2D_FRAME;
    else
        format = LASERBOY_3D_FRAME;
    //------------------------------------------------------------------------
    if(p_space->dumb_file_format)
    {
        if(format == LASERBOY_3D_FRAME)
            format = LASERBOY_3D_FRAME_RGB;
        else if(format == LASERBOY_2D_FRAME)
            format = LASERBOY_2D_FRAME_RGB;
        if(!name.size())
        {
            sprintf(number, "%05d", identity);
            name = "ESI" + string(number);
        }
        LaserBoy_ild_header::to_ofstream_ild(out);
        for(int i = 0; i < quantity; i++)
            at(i).to_ofstream_ild(out, format, (i == quantity - 1));
    }
    //------------------------------------------------------------------------
    else
    {
        if(palette_index == LASERBOY_TRUE_COLOR)
        {
            LaserBoy_ild_header header  = (LaserBoy_ild_header)*this;
            LaserBoy_palette    palette = as_color_table(); // *this
            header.identity--;
            header.format = LASERBOY_TABLE;
            header.name   = p_space->GUID8char();
            palette.to_ofstream_ild(out, header);
        }
        else if(!name.size())
        {
            sprintf(number, "%05d", identity);
            name = "ESI" + string(number);
        }
        LaserBoy_ild_header::to_ofstream_ild(out);
        for(int i = 0; i < quantity; i++)
            at(i).to_ofstream_ild(out, format, (i == quantity - 1));
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_frame::to_ofstream_txt_color_table(ofstream& out)
{
    out << "# ---------------------------------------------------------------"
        << ENDL
        << "table ";
    if(p_space->save_txt_color_hex)
        out << "hex";
    else
        out << "rgb";
    out << ENDL;
    //------------------------------------------------------------------------
    for(u_int i = 0; i < size(); i++)
        if(at(i).is_lit())
            ((LaserBoy_color)at(i)).to_ofstream_txt(out, p_space->save_txt_color_hex);
        else
            out << "    -1"
                << ENDL;
    //------------------------------------------------------------------------
    out << ENDL;
    return;
}

//############################################################################
void LaserBoy_frame::to_ofstream_txt(ofstream& out, int frame_index)
{
    int output_format = (   (is_2D())
                           ?(  (p_space->save_txt_with_color)
                              ?(LASERBOY_2D_FRAME_RGB)
                              :(   (palette_index == LASERBOY_TRUE_COLOR)
                                 ? (LASERBOY_2D_FRAME_TABLE)
                                 : (LASERBOY_2D_FRAME_PALETTE)
                               )
                            ) // 3D
                           :(  (p_space->save_txt_with_color)
                              ?(LASERBOY_3D_FRAME_RGB)
                              :(   (palette_index == LASERBOY_TRUE_COLOR)
                                 ? (LASERBOY_3D_FRAME_TABLE)
                                 : (LASERBOY_3D_FRAME_PALETTE)
                               )
                            )
                        );
    //------------------------------------------------------------------------
    if(frame_index >= 0)
        out << "# " << setw(6) << frame_index << " ---------------------------------------------------------" << ENDL;
    out << "frame ";
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_2D_FRAME_PALETTE:
                out << "xy ";
                break;
        case LASERBOY_3D_FRAME_RGB:
        case LASERBOY_3D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_PALETTE:
                out << "xyz ";
                break;
    }
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_3D_FRAME_RGB:
                if(p_space->save_txt_color_hex)
                    out << "hex";
                else
                    out << "rgb";
                break;
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_TABLE:
                out << "table";
                break;
        case LASERBOY_2D_FRAME_PALETTE:
        case LASERBOY_3D_FRAME_PALETTE:
                out << "palette";
                break;
    }
    //------------------------------------------------------------------------
    if(p_space->save_txt_unit)
    {
        out << " unit" << ENDL;
        switch(output_format)
        {
            case LASERBOY_3D_FRAME_PALETTE:
                     for(u_int i = 0; i < size(); i++)
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                             << noshowpos
                             << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_PALETTE:
                     for(u_int i = 0; i < size(); i++)
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << noshowpos
                             << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_TABLE: // nothing but x y z
                     for(u_int i = 0; i < size(); i++)
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                             << noshowpos
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_TABLE: // nothing but x y
                     for(u_int i = 0; i < size(); i++)
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << noshowpos
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_RGB:
                     for(u_int i = 0; i < size(); i++)
                     {
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                             << noshowpos;
                         if(p_space->save_txt_color_hex)
                         {
                             if(at(i).is_lit())
                                 out << "    0x"
                                     << hex
                                     << setw(2) << setfill('0') << (int)at(i).r
                                     << setw(2) << setfill('0') << (int)at(i).g
                                     << setw(2) << setfill('0') << (int)at(i).b
                                     << setfill(' ');
                             else
                                 out << dec
                                     << "   -1";
                         }
                         else
                         {
                             if(at(i).is_lit())
                                 out << setw(7) << (int)at(i).r
                                     << setw(7) << (int)at(i).g
                                     << setw(7) << (int)at(i).b;
                             else
                                 out << dec
                                     << "   -1";
                         }
                         out << ENDL;
                     }
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_RGB:
                     for(u_int i = 0; i < size(); i++)
                     {
                         out << "    "
                             << showpos
                             << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                             << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                             << noshowpos;
                         if(p_space->save_txt_color_hex)
                         {
                             if(at(i).is_lit())
                                 out << "    0x"
                                     << hex
                                     << setw(2) << setfill('0') << (int)at(i).r
                                     << setw(2) << setfill('0') << (int)at(i).g
                                     << setw(2) << setfill('0') << (int)at(i).b
                                     << setfill(' ');
                             else
                                 out << dec
                                     << "   -1";
                         }
                         else
                         {
                             if(at(i).is_lit())
                                 out << setw(7) << (int)at(i).r
                                     << setw(7) << (int)at(i).g
                                     << setw(7) << (int)at(i).b;
                             else
                                 out << dec
                                     << "   -1";
                         }
                         out << ENDL;
                     }
                     break;
            //----------------------------------------------------------------
        }
    }
    else
    {
        out << " short" << ENDL;
        switch(output_format)
        {
            case LASERBOY_3D_FRAME_PALETTE:
                     for(u_int i = 0; i < size(); i++)
                         out << setw(9) << at(i).x
                             << setw(9) << at(i).y
                             << setw(9) << at(i).z
                             << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_PALETTE:
                     for(u_int i = 0; i < size(); i++)
                         out << setw(9) << at(i).x
                             << setw(9) << at(i).y
                             << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_TABLE:
                     for(u_int i = 0; i < size(); i++)
                         out << setw(9) << at(i).x
                             << setw(9) << at(i).y
                             << setw(9) << at(i).z
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_TABLE:
                     for(u_int i = 0; i < size(); i++)
                         out << setw(9) << at(i).x
                             << setw(9) << at(i).y
                             << ENDL;
                     break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_RGB:
                     for(u_int i = 0; i < size(); i++)
                     {
                         out << dec
                             << setw(9) << at(i).x
                             << setw(9) << at(i).y
                             << setw(9) << at(i).z;
                         if(p_space->save_txt_color_hex)
                         {
                             if(at(i).is_lit())
                                 out << "    0x"
                                     << hex
                                     << setw(2) << setfill('0') << (int)at(i).r
                                     << setw(2) << setfill('0') << (int)at(i).g
                                     << setw(2) << setfill('0') << (int)at(i).b
                                     << setfill(' ');
                             else
                                 out << dec
                                     << "   -1";
                         }
                         else
                         {
                             if(at(i).is_lit())
                                 out << setw(7) << (int)at(i).r
                                     << setw(7) << (int)at(i).g
                                     << setw(7) << (int)at(i).b;
                             else
                                 out << dec
                                     << "   -1";
                         }
                         out << ENDL;
                     }
                     break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_RGB:
                     for(u_int i = 0; i < size(); i++)
                     {
                         out << dec
                             << setw(9) << at(i).x
                             << setw(9) << at(i).y;
                         if(p_space->save_txt_color_hex)
                         {
                             if(at(i).is_lit())
                                 out << "    0x"
                                     << hex
                                     << setw(2) << setfill('0') << (int)at(i).r
                                     << setw(2) << setfill('0') << (int)at(i).g
                                     << setw(2) << setfill('0') << (int)at(i).b
                                     << setfill(' ');
                             else
                                 out << dec
                                     << "   -1";
                         }
                         else
                         {
                             if(at(i).is_lit())
                                 out << setw(7) << (int)at(i).r
                                     << setw(7) << (int)at(i).g
                                     << setw(7) << (int)at(i).b;
                             else
                                 out << dec
                                     << "   -1";
                         }
                         out << ENDL;
                     }
                     break;
            //----------------------------------------------------------------
        }
    }
    out << ENDL;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_frame::render(int skin) const
{
    LaserBoy_3D_double _0,
                       _1;

    LaserBoy_color     lb_color;

    bool     rotate_view = (p_space->view_angle  != 0.0),
             offset_view = (p_space->view_offset != 0  ),
             scale_view  = (p_space->view_scale  != 1.0);

    u_int    i             ,
             bmp_color     ,
             bmp_blank    = p_space->bmp.make_rgb(p_space->rendered_blank.r,
                                                  p_space->rendered_blank.g,
                                                  p_space->rendered_blank.b
                                                 ),
             bmp_black    = p_space->bmp.make_rgb(p_space->rendered_black.r,
                                                  p_space->rendered_black.g,
                                                  p_space->rendered_black.b
                                                 ),
             bmp_white    = p_space->bmp.white,
             bmp_bg_color = p_space->bmp.make_rgb(p_space->bg_color.r,
                                                  p_space->bg_color.g,
                                                  p_space->bg_color.b
                                                 );

    int      xx,
             yy,
             bmp_center = (   (p_space->show_quad_view)
                            ? (p_space->bmp.yres / 4)
                            : (p_space->bmp.yres / 2)
                          ),
             bmp_3X_center = bmp_center * 3,
             bigger =   (   ((p_space->show_bridge) ? (bridge.size()) : (0))
                          > ((p_space->show_coda)   ? (coda.size())   : (0))
                        )
                      ? ((p_space->show_bridge) ? (bridge.size()) : (0))
                      : ((p_space->show_coda)   ? (coda.size())   : (0));

    double   fade,
             deep = LASERBOY_SHORT_SPAN,
             half_deep = deep / 2.0,
             z_depth_scale =   deep
                             / (   size()
                                 + ((p_space->show_intro)  ? (intro.size())  : (0))
                                 + bigger
                               ),

             z_depth_body_offset =   half_deep
                                   - (   z_depth_scale
                                       * ((p_space->show_intro) ? (intro.size()) : (0))
                                     ),

             z_depth_coda_offset =   half_deep
                                   - (   (z_depth_scale * ((p_space->show_intro) ? (intro.size()) : (0)))
                                       + (z_depth_scale * size())
                                     ),

             scale = (   (p_space->show_quad_view)
                       ? (p_space->bmp.yres / (deep * 2.0))
                       : (p_space->bmp.yres / deep)
                     );
    //------------------------------------------------------------------------
    if(skin)
    {
        fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
        if(fade > 1.0)
            fade = 1.0;
        lb_color = p_space->rendered_blank;
        lb_color = lb_color.blend(bmp_bg_color, fade);
        bmp_blank = p_space->bmp.make_rgb(lb_color.r,
                                          lb_color.g,
                                          lb_color.b
                                         );
        lb_color = p_space->rendered_black;
        lb_color = lb_color.blend(bmp_bg_color, fade);
        bmp_black = p_space->bmp.make_rgb(lb_color.r,
                                          lb_color.g,
                                          lb_color.b
                                         );
        lb_color = p_space->bmp.white;
        lb_color = lb_color.blend(bmp_bg_color, fade);
        bmp_white = p_space->bmp.make_rgb(lb_color.r,
                                          lb_color.g,
                                          lb_color.b
                                         );
    }
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // blank vectors
    //------------------------------------------------------------------------
    if(    p_space->show_blanking
        && (    skin == 0
             || p_space->show_onion_visuals
           )
      )
    {
        //--------------------------------------------------------------------
        // blank in intro
        //--------------------------------------------------------------------
        if(p_space->show_intro)
        {
            for(i = 1; i < intro.size(); i++)
            {
                if(intro.at(i).is_blank())
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    // normal view
                    bmp_put_line_fast(&(p_space->bmp),     (int)(_0.x * scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                      bmp_blank
                                      );
                    if(p_space->show_quad_view)
                    {
                        _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - half_deep;
                            _1.z =  i      * z_depth_scale - half_deep;
                        }
                        if(offset_view)
                        {
                            _0 += p_space->view_offset;
                            _1 += p_space->view_offset;
                        }
                        if(scale_view)
                        {
                            _0 *= p_space->view_scale;
                            _1 *= p_space->view_scale;
                        }
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                               (int)(_0.x * scale) + bmp_3X_center,
                                          p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                          xx,
                                          p_space->bmp.yres - yy,
                                          bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                               (int)(_0.x * scale) + bmp_center,
                                          p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                          xx,
                                          p_space->bmp.yres - yy,
                                          bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                               (int)(_0.z * scale) + bmp_3X_center,
                                          p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                          xx,
                                          p_space->bmp.yres - yy,
                                          bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                    } // end if(p_space->show_quad_view)
                } // end if(intro.at(i).is_blank())
            } // end for(i = 1; i < (int)intro.size(); i++)
        } // end if(p_space->show_intro)
        //--------------------------------------------------------------------
        // blank in frame
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_blank())
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                // normal view
                bmp_put_line_fast(&(p_space->bmp),     (int)(_0.x * scale) + bmp_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_blank
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_blank
                                  );
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_blank
                                  );
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_blank,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                if(p_space->show_quad_view)
                {
                    _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                        _1.z =  i      * z_depth_scale - z_depth_body_offset;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                    // top
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x *  scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.z * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                } // end if(p_space->show_quad_view)
            } // end if(at(i).is_blank())
        } // end for(i = 1; i < size(); i++)
        //--------------------------------------------------------------------
        // blank in bridge
        //--------------------------------------------------------------------
        if(p_space->show_bridge)
            for(i = 1; i < bridge.size(); i++)
            {
                if(bridge.at(i).is_blank())
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    // normal view
                    bmp_put_line_fast(&(p_space->bmp),     (int)(_0.x * scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(p_space->show_quad_view)
                    {
                        _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                            _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                        }
                        if(offset_view)
                        {
                            _0 += p_space->view_offset;
                            _1 += p_space->view_offset;
                        }
                        if(scale_view)
                        {
                            _0 *= p_space->view_scale;
                            _1 *= p_space->view_scale;
                        }
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                               (int)(_0.x * scale) + bmp_3X_center,
                                          p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                          xx,
                                          p_space->bmp.yres - yy,
                                          bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                                (int)(_0.x *  scale) + bmp_center,
                                           p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        bmp_put_line_fast(&(p_space->bmp),
                                                                (int)(_0.z * scale) + bmp_3X_center,
                                           p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                    } // end if(p_space->show_quad_view)
                } // if(bridge.at(i).is_blank())
            } // end for(i = 1; i < (int)bridge.size(); i++)
        //--------------------------------------------------------------------
        // blank in coda
        //--------------------------------------------------------------------
        if(p_space->show_coda)
            for(i = 1; i < coda.size(); i++)
            {
                if(coda.at(i).is_blank())
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    // normal view
                    bmp_put_line_fast( &(p_space->bmp),     (int)(_0.x * scale) + bmp_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_blank
                                      );
                    if(p_space->show_quad_view)
                    {
                        _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                            _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                        }
                        if(offset_view)
                        {
                            _0 += p_space->view_offset;
                            _1 += p_space->view_offset;
                        }
                        if(scale_view)
                        {
                            _0 *= p_space->view_scale;
                            _1 *= p_space->view_scale;
                        }
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        bmp_put_line_fast( &(p_space->bmp),
                                                                (int)(_0.x * scale) + bmp_3X_center,
                                           p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        bmp_put_line_fast( &(p_space->bmp),
                                                                (int)(_0.x *  scale) + bmp_center,
                                           p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        bmp_put_line_fast( &(p_space->bmp),
                                                                (int)(_0.z * scale) + bmp_3X_center,
                                           p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                         );
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(&(p_space->bmp),
                                           xx,
                                           p_space->bmp.yres - yy,
                                           bmp_blank
                                          );
                    } // end if(p_space->show_quad_view)
                } // end if(coda.at(i).is_blank())
            } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_blanking)
    //------------------------------------------------------------------------
    // black vectors
    //------------------------------------------------------------------------
    if(p_space->show_intro)
    {
        //--------------------------------------------------------------------
        // black in intro
        //--------------------------------------------------------------------
        for(i = 1; i < intro.size(); i++)
        {
            if(intro.at(i).is_black(p_space->black_level))
            {
                _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - half_deep;
                    _1.z =  i      * z_depth_scale - half_deep;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                // normal view
                //------------------------------------------------------------
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x * scale) + bmp_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    //front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x *  scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.z * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                } // end if(p_space->show_quad_view)
            } // if(intro.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)intro.size(); i++)
    } // end if(p_space->show_intro)
    //------------------------------------------------------------------------
    // black in frame
    //------------------------------------------------------------------------
    for(i = 1; i < size(); i++)
    {
        if(at(i).is_black(p_space->black_level))
        {
            _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                _1.z =  i      * z_depth_scale - z_depth_body_offset;
            }
            if(rotate_view)
            {
                _0 = rotate_vertex(_0, p_space->view_angle);
                _1 = rotate_vertex(_1, p_space->view_angle);
            }
            if(offset_view)
            {
                _0 += p_space->view_offset;
                _1 += p_space->view_offset;
            }
            if(scale_view)
            {
                _0 *= p_space->view_scale;
                _1 *= p_space->view_scale;
            }
            // normal view
            xx = (int)(_1.x * scale) + bmp_center;
            yy = (int)(_1.y * scale) + bmp_center;
            //----------------------------------------------------------------
            bmp_put_line_fast(&(p_space->bmp),
                                                   (int)(_0.x * scale) + bmp_center,
                              p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                              xx,
                              p_space->bmp.yres - yy,
                              bmp_black
                             );
            if(    p_space->show_vertices
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_vertex(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_black
                              );
            //----------------------------------------------------------------
            if(    p_space->show_cursors
                && is_index_selected(i)
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_select(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_white
                              );
            if(    p_space->show_indices
                && (skin == 0)
              )
                bmp_printf(&(p_space->bmp),
                           xx,
                           p_space->bmp.yres - yy,
                           bmp_white,
                           bmp_bg_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           i
                          );
            if(p_space->show_quad_view)
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //front
                xx = (int)(_1.x * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_3X_center;
                bmp_color = p_space->bmp.make_rgb(p_space->rendered_black.r,
                                                  p_space->rendered_black.g,
                                                  p_space->rendered_black.b
                                                 );
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                  bmp_black
                                  );
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                // top
                xx = (int)(_1.x *  scale) + bmp_center;
                yy = (int)(_1.z * -scale) + bmp_3X_center;
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x *  scale) + bmp_center,
                                  p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                // side
                xx = (int)(_1.z * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_center;
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.z * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
            }
        } // if(at(i).is_black(p_space->black_level))
    } // end for(i = 1; i < size(); i++)
    //------------------------------------------------------------------------
    // black in bridge
    //------------------------------------------------------------------------
    if(p_space->show_bridge)
    {
        for(i = 1; i < bridge.size(); i++)
        {
            if(bridge.at(i).is_black(p_space->black_level))
            {
                _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                //------------------------------------------------------------
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x * scale) + bmp_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    //front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x *  scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.z * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                } // end if(p_space->show_quad_view)
            } // if(bridge.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)bridge.size(); i++)
    } // end if(p_space->show_bridge)
    //------------------------------------------------------------------------
    // black in coda
    //------------------------------------------------------------------------
    if(p_space->show_coda)
    {
        for(i = 1; i < coda.size(); i++)
        {
            if(coda.at(i).is_black(p_space->black_level))
            {
                _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                //------------------------------------------------------------
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x * scale) + bmp_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_black
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_black
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    //front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.x *  scale) + bmp_center,
                                      p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast(&(p_space->bmp),
                                                           (int)(_0.z * scale) + bmp_3X_center,
                                      p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                      xx,
                                      p_space->bmp.yres - yy,
                                      bmp_black
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_black
                                      );
                } // end if(p_space->show_quad_view)
            } // if(coda.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_coda)
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // color vectors
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // color in intro
    //------------------------------------------------------------------------
    if(p_space->show_intro)
    {
        for(i = 1; i < intro.size(); i++)
        {
            if(intro.at(i).is_color(p_space->black_level))
            {
                _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - half_deep;
                    _1.z =  i      * z_depth_scale - half_deep;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                
                bmp_color = p_space->bmp.make_rgb(intro.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                                  intro.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                                  intro.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                                  );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = intro.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = p_space->bmp.make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                bmp_put_line_fast( &(p_space->bmp),
                                                        (int)(_0.x * scale) + bmp_center,
                                   p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x *  scale) + bmp_center,
                                       p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.z * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                } // end if(p_space->show_quad_view)
            } // if(intro.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)intro.size(); i++)
    } // end if(p_space->show_intro)
    //------------------------------------------------------------------------
    // color in frame
    //------------------------------------------------------------------------
    for(i = 1; i < size(); i++)
    {
        if(at(i).is_color(p_space->black_level))
        {
            _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                _1.z =  i      * z_depth_scale - z_depth_body_offset;
            }
            if(rotate_view)
            {
                _0 = rotate_vertex(_0, p_space->view_angle);
                _1 = rotate_vertex(_1, p_space->view_angle);
            }
            if(offset_view)
            {
                _0 += p_space->view_offset;
                _1 += p_space->view_offset;
            }
            if(scale_view)
            {
                _0 *= p_space->view_scale;
                _1 *= p_space->view_scale;
            }
            // normal view
            xx = (int)(_1.x * scale) + bmp_center;
            yy = (int)(_1.y * scale) + bmp_center;

            bmp_color = p_space->bmp.make_rgb(at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                              at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                              at(i).bit_masked_color(p_space->signal_bit_mask).b
                                             );
            if(skin)
            {
                fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                if(fade > 1.0)
                    fade = 1.0;
                lb_color = at(i).bit_masked_color(p_space->signal_bit_mask);
                lb_color = lb_color.blend(bmp_bg_color, fade);
                bmp_color = p_space->bmp.make_rgb(lb_color.r,
                                                  lb_color.g,
                                                  lb_color.b
                                                 );
            }
            //----------------------------------------------------------------
            bmp_put_line_fast( &(p_space->bmp),
                                                    (int)(_0.x * scale) + bmp_center,
                               p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_color
                             );
            if(    p_space->show_vertices
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_vertex(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_color
                              );
            //----------------------------------------------------------------
            if(    p_space->show_cursors
                && is_index_selected(i)
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_select(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_color
                              );
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(&(p_space->bmp),
                               xx,
                               p_space->bmp.yres - yy,
                               bmp_color,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
            if(p_space->show_quad_view)
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // front
                xx = (int)(_1.x * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_3X_center;
                bmp_put_line_fast(&(p_space->bmp),
                                                       (int)(_0.x * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                  xx,
                                  p_space->bmp.yres - yy,
                                  bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                // top
                xx = (int)(_1.x *  scale) + bmp_center;
                yy = (int)(_1.z * -scale) + bmp_3X_center;
                bmp_put_line_fast( &(p_space->bmp),
                                                        (int)(_0.x *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                // side
                xx = (int)(_1.z * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_center;
                bmp_put_line_fast( &(p_space->bmp),
                                                        (int)(_0.z * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
            } // end if(p_space->show_quad_view)
        } // if(at(i).is_color(p_space->black_level))
    } // end for(i = 1; i < size(); i++)
    //------------------------------------------------------------------------
    // color in bridge
    //------------------------------------------------------------------------
    if(p_space->show_bridge)
    {
        for(i = 1; i < bridge.size(); i++)
        {
            if(bridge.at(i).is_color(p_space->black_level))
            {
                _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                bmp_color = p_space->bmp.make_rgb(bridge.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                                  bridge.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                                  bridge.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                                 );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = bridge.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = p_space->bmp.make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                bmp_put_line_fast( &(p_space->bmp),
                                                        (int)(_0.x * scale) + bmp_center,
                                   p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x *  scale) + bmp_center,
                                       p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.z * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                }
            } // if(bridge.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)bridge.size(); i++)
    } // end if(p_space->show_bridge)
    //------------------------------------------------------------------------
    // color in coda
    //------------------------------------------------------------------------
    if(p_space->show_coda)
    {
        for(i = 1; i < coda.size(); i++)
        {
            if(coda.at(i).is_color(p_space->black_level))
            {
                _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                
                bmp_color = p_space->bmp.make_rgb(coda.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                                  coda.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                                  coda.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                                 );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = coda.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = p_space->bmp.make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                bmp_put_line_fast( &(p_space->bmp),
                                                        (int)(_0.x * scale) + bmp_center,
                                   p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                 );
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(&(p_space->bmp),
                                   xx,
                                   p_space->bmp.yres - yy,
                                   bmp_color
                                  );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.x *  scale) + bmp_center,
                                       p_space->bmp.yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    bmp_put_line_fast( &(p_space->bmp),
                                                            (int)(_0.z * scale) + bmp_3X_center,
                                       p_space->bmp.yres - ((int)(_0.y * scale) + bmp_center),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                     );
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(&(p_space->bmp),
                                       xx,
                                       p_space->bmp.yres - yy,
                                       bmp_color
                                      );
                    //--------------------------------------------------------
                }
            } // if(coda.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_coda)
    //------------------------------------------------------------------------
    // END ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::save_as_wave(const string& file, bool optimized, bool timed)
{
    fstream out(file.c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(out.is_open() && size() > 1)
    {
        int                   i;
        LaserBoy_frame        copy (*this);
        LaserBoy_wave_header  header(p_space->sample_rate,
                                       LASERBOY_WAVE_POSITIVE
                                     | LASERBOY_WAVE_END_OF_FRAME
                                     | LASERBOY_WAVE_UNIQUE_FRAME
                                     | LASERBOY_WAVE_SIGNAL_MATRIX
                                     | LASERBOY_SIGNAL_BIT_RESOLUTION
                                     | LASERBOY_WAVE_NUM_FRAMES
                                     | p_space->color_rescales_flags(),
                                     p_space->channels_of_wav_out,
                                     LASERBOY_WAVE_VERSION
                                    );
        //--------------------------------------------------------------------
        if(p_space->invert_wave_output)
            header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE;
        //--------------------------------------------------------------------
        header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION; // default values
        header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION;
        header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG;
        header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
        header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG;
        header.signal_id [5] = LASERBOY_SIGNAL_Z_POSITION;
        header.signal_id [6] = LASERBOY_SIGNAL_UNDEFINED;
        header.signal_id [7] = LASERBOY_SIGNAL_UNDEFINED;
        //--------------------------------------------------------------------
        header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED; // default values
        header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [2] = LASERBOY_LSB_END_OF_FRAME;
        header.LSB_tag   [3] = LASERBOY_LSB_UNIQUE_FRAME;
        header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;
        //--------------------------------------------------------------------
        header.resolution[0] = 16 - p_space->signal_bit_mask[0];
        header.resolution[1] = 16 - p_space->signal_bit_mask[1];
        header.resolution[2] = 16 - p_space->signal_bit_mask[2];
        header.resolution[3] = 16 - p_space->signal_bit_mask[3];
        header.resolution[4] = 16 - p_space->signal_bit_mask[4];
        header.resolution[5] = 16 - p_space->signal_bit_mask[5];
        header.resolution[6] = 16 - p_space->signal_bit_mask[6];
        header.resolution[7] = 16 - p_space->signal_bit_mask[7];
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            for(i = 0; i < 256; i++)
                header.color_rescale_r[i] = p_space->color_rescale_r[i];
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
            for(i = 0; i < 256; i++)
                header.color_rescale_g[i] = p_space->color_rescale_g[i];
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
            for(i = 0; i < 256; i++)
                header.color_rescale_b[i] = p_space->color_rescale_b[i];
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
            for(i = 0; i < 256; i++)
                header.color_rescale_i[i] = p_space->color_rescale_i[i];
        //--------------------------------------------------------------------
        if(p_space->view_angle != 0.0)
            copy.rotate_around_origin();
        //--------------------------------------------------------------------
        if(copy.is_2D() || p_space->auto_flatten_z)
        {
            header.signal_id [5] = p_space->signal_if_not_z;
            header.resolution[5] = 9;
            copy.flatten_z();
        }
        //--------------------------------------------------------------------
        if(optimized)
        {
            LaserBoy_vertex  origin;
            header.LaserBoy_wave_mode |= LASERBOY_WAVE_OPTIMIZED;
            header.parms.lit_dwell_overhang     =        p_space->lit_dwell_overhang;
            header.parms.lit_delta_max          = (float)p_space->lit_delta_max;
            header.parms.blank_delta_max        = (float)p_space->blank_delta_max;
            header.parms.max_dwell_microsec     = (float)p_space->max_dwell_microsec;
            header.parms.max_wag_microsec       = (float)p_space->max_wag_microsec;
            header.parms.insignificant_distance = (float)p_space->insignificant_distance;
            header.parms.insignificant_angle    = (float)p_space->insignificant_angle;
            header.parms.frames_per_second      =    (timed)
                                                   ? ((float)p_space->frames_per_second)
                                                   : (0);
            header.to_fstream_wave(out); // take up the space!
            copy.optimize(0); // as a single frame
            copy.add_coda(origin);
            copy.coda += LaserBoy_segment(p_space, copy.coda.back(), origin);
            copy.to_fstream_wave(out, header, true, timed);
        }
        else
        {
            header.to_fstream_wave(out); // take up the space!
            copy.to_fstream_wave(out, header, false, timed);
        }
        //--------------------------------------------------------------------
        header.to_fstream_wave(out);
        //--------------------------------------------------------------------
        out.close();
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_frame::to_fstream_wave(fstream& out,
                                     LaserBoy_wave_header& header,
                                     bool optimized,
                                     bool timed
                                    )
{
    //------------------------------------------------------------------------
    if(optimized)
    {
        int  chill_samples = (int)ceil(   p_space->sample_rate
                                        * (   p_space->max_dwell_microsec
                                            / 500000.0 // make it twice the max_dwell_microsec
                                          )
                                      );
        LaserBoy_vertex   origin;
        LaserBoy_segment  chill(p_space);
        if(chill_samples < 10)
            chill_samples = 10;
        for(int i = 0; i < chill_samples; i++)
            chill.push_back(origin);
        //--------------------------------------------------------------------
        p_space->p_GUI->display_state("saving frame as wave");
        chill.to_fstream_wave(out, header, false, false);
        intro.to_fstream_wave(out, header, false, false);
        //--------------------------------------------------------------------
        if(timed)
        {
            int  frame_scan,
                 total_frame_scans = (int)ceil(   (   p_space->sample_rate
                                                    * p_space->still_frame_duration
                                                  )
                                                / (   size()
                                                    + bridge.size()
                                                  )
                                              );
            LaserBoy_segment::to_fstream_wave(out, header, is_wagged, is_wagged);
            if(total_frame_scans > 1)
            {
                for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
                {
                    bridge.to_fstream_wave(out, header, true, !is_wagged && (frame_scan == 1)); // frame scan 1 is unique
                    LaserBoy_segment::to_fstream_wave(out, header, is_wagged, false);
                    p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
                }
                coda.to_fstream_wave(out, header, false, false);
            }
        } // end if(p_space->still_frame_duration > 0.0)
        else // no repete no bridge
        {
            LaserBoy_segment::to_fstream_wave(out, header, false, false);
            coda.to_fstream_wave(out, header, false, false);
        }
        //--------------------------------------------------------------------
        chill.to_fstream_wave(out, header, true, true);
        //--------------------------------------------------------------------
    } // end if(optimized)
    //------------------------------------------------------------------------
    else if(timed) // no intro, bridge or coda!
    {
        p_space->p_GUI->display_state("saving frame as wave");
        //--------------------------------------------------------------------
        int  frame_scan,
             total_frame_scans = (int)ceil(   (   p_space->sample_rate
                                                * p_space->still_frame_duration
                                              )
                                            / size()
                                          );
        LaserBoy_segment::to_fstream_wave(out, header, true, true);
        for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
        {
            LaserBoy_segment::to_fstream_wave(out, header, true, false);
            p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
        }
    } // end if(optimized)
    //------------------------------------------------------------------------
    else // no intro, bridge or coda!
        LaserBoy_segment::to_fstream_wave(out, header, true, true);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::save_as_dxf(const string& file) const
{
    ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        u_int               segment_vector_index,
                            vertex_index;

        LaserBoy_frame_set  segments(p_space);
        //--------------------------------------------------------------------
        if(p_space->auto_minimize)
        {
            LaserBoy_frame  copy(*this);
            copy.minimize(0);
            segments = copy.explode_segments();
        }
        else
            segments = explode_segments();
        //--------------------------------------------------------------------
        out << " 0\n"
                "SECTION\n"
                " 2\n"
                "ENTITIES\n";
        //--------------------------------------------------------------------
        for(segment_vector_index = 0; segment_vector_index < segments.size(); segment_vector_index++)
        {
            if(segments[segment_vector_index].number_of_color_vectors() == 1)
            {
                if(    (   segments[segment_vector_index].first_lit_anchor().as_3D_short()
                         | segments[segment_vector_index].first_lit_vector().as_3D_short()
                       )
                    <= p_space->insignificant_distance
                  )
                {
                    out << " 0\n"
                           "POINT\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].first_lit_vector().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].first_lit_vector());
                    out << "\n 10\n"
                        << segments[segment_vector_index].first_lit_anchor().x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].first_lit_anchor().y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].first_lit_anchor().z
                        << ".00\n";
                }
                else
                {
                    out << " 0\n"
                           "LINE\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].first_lit_vector().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].first_lit_vector());
                    out << "\n 10\n"
                        << segments[segment_vector_index].first_lit_anchor().x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].first_lit_anchor().y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].first_lit_anchor().z
                        << ".00\n 11\n"
                        << segments[segment_vector_index].first_lit_vector().x
                        << ".00\n 21\n"
                        << segments[segment_vector_index].first_lit_vector().y
                        << ".00\n 31\n"
                        << segments[segment_vector_index].first_lit_vector().z
                        << ".00\n";
                }
            }
            //----------------------------------------------------------------
            else
            {
                out << " 0\n"
                        "POLYLINE\n"
                        " 70\n"
                    << segments[segment_vector_index].is_closed_polygon()
                    << "\n 10\n"
                       "0.00\n"
                       " 20\n"
                       "0.00\n"
                       " 30\n"
                       "0.00\n"
                       //---------
                       " 0\n"
                       "VERTEX\n";
                if(p_space->save_true_color_dxf)
                    out << " 420\n"
                        << segments[segment_vector_index].at(1).color_of(p_space->black_level);
                else
                    out << " 62\n"
                        << p_space->
                           palette_picker(LASERBOY_DXF)
                           .best_match(segments[segment_vector_index].at(1));
                out << "\n 10\n"
                    << segments[segment_vector_index].first_lit_anchor().x
                    << ".00\n 20\n"
                    << segments[segment_vector_index].first_lit_anchor().y
                    << ".00\n 30\n"
                    << segments[segment_vector_index].first_lit_anchor().z
                    << ".00\n";

                for(vertex_index = 1; vertex_index < segments[segment_vector_index].size() - 1; vertex_index++)
                {
                    out << " 0\n"
                        << "VERTEX\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].at(vertex_index + 1).color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].at(vertex_index + 1));
                    out << "\n 10\n"
                        << segments[segment_vector_index].at(vertex_index).x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].at(vertex_index).y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].at(vertex_index).z
                        << ".00\n";
                }
                out << " 0\n"
                       "VERTEX\n";
                if(p_space->save_true_color_dxf)
                    out << " 420\n";
                else
                    out << " 62\n";
                out << "0\n"
                       " 10\n"
                    << segments[segment_vector_index].at(vertex_index).x
                    << ".00\n 20\n"
                    << segments[segment_vector_index].at(vertex_index).y
                    << ".00\n 30\n"
                    << segments[segment_vector_index].at(vertex_index).z
                    << ".00\n";
            }
        }
        //--------------------------------------------------------------------
        out << " 0\n"
               "ENDSEC\n"
               " 0\n"
               "EOF\n";
        //--------------------------------------------------------------------
        out.close();
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_frame::save_as_bmp(const string& file) const
{
    if(size() > 1)
    {
        char   file_name[80];
        struct LaserBoy_bmp bmp_out = {0};

        if(palette_index == LASERBOY_TRUE_COLOR)
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size, 24);
        else
        {
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  8);
            p_space->palette_picker(palette_index).to_bmp_palette(&bmp_out);
        }
        bmp_clear(&bmp_out, bmp_out.black);

        u_short              i;
        LaserBoy_3D_double   _0,
                             _1;
        int                  bmp_center  = bmp_out.yres / 2;
        double               scale       = bmp_out.yres / 65536.0;
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_color(p_space->black_level))
            {
                _0 = at(i - 1);
                _1 = at(i    );
                //------------------------------------------------------------

                if(palette_index == LASERBOY_TRUE_COLOR)
                    bmp_put_line_fast(   &bmp_out
                                       ,  (int)(_0.x * scale) + bmp_center
                                       , ((int)(_0.y * scale) + bmp_center)
                                       ,  (int)(_1.x * scale) + bmp_center
                                       , ((int)(_1.y * scale) + bmp_center)
                                       , bmp_out.make_rgb(at(i).r, at(i).g, at(i).b)
                                     );
                else
                    bmp_put_line_fast(   &bmp_out
                                       ,  (int)(_0.x * scale) + bmp_center
                                       , ((int)(_0.y * scale) + bmp_center)
                                       ,  (int)(_1.x * scale) + bmp_center
                                       , ((int)(_1.y * scale) + bmp_center)
                                       , at(i).c
                                     );
            }
        }
        sprintf(file_name, "%s", file.c_str());
        bmp_save_as(&bmp_out, file_name);
        bmp_free(&bmp_out);
        return true;
    }
    return false;
}

//############################################################################
LaserBoy_frame blank_frame(LaserBoy_space* ps)
{
    LaserBoy_frame frame(ps);
    frame = blank_segment(ps);
    frame.is_2D();
    return frame;
}

//############################################################################
LaserBoy_frame NULL_frame(LaserBoy_space* ps)
{
    LaserBoy_frame frame(ps);
    frame = blank_segment(ps);
    frame.is_2D();
    return frame;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
