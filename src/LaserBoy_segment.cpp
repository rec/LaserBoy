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
// LaserBoy_segment.cpp is part of LaserBoy.
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
LaserBoy_segment::LaserBoy_segment(LaserBoy_space   *ps,
                                   LaserBoy_3D_short from,
                                   LaserBoy_vertex   to
                                  )
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{   // 3D line constructor, linear steps
    int steps = linear_steps(to, from, (   (to.is_lit())
                                         ? (p_space->lit_delta_max)
                                         : (p_space->blank_delta_max)
                                       )
                            );
    //------------------------------------------------------------------------
    if(steps)
    {
        LaserBoy_3D_double _to        (to), // convert 3D short to 3D double
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //--------------------------------------------------------------------
        reserve(steps);
        for(int i = 1; i <= steps; i++)
            push_back(LaserBoy_vertex(LaserBoy_3D_short(_from + (delta * i)),
                                      (LaserBoy_color)to,
                                      to.k,
                                      to.c
                                     )
                     );
        //--------------------------------------------------------------------
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space* ps,
                                   LaserBoy_vertex p0,
                                   LaserBoy_vertex p1,
                                   string          font,
                                   string          text
                                  )
                : p_space       (ps)
                , palette_index (p_space->current_frame().palette_index)
                , segment_error (LASERBOY_OK)
{   // mono spaced font constructor
    //------------------------------------------------------------------------
    long int bytes_skipped = 0;
    LaserBoy_frame_set font_frames(p_space);
    font_frames.from_ild_file(font, bytes_skipped);
    //------------------------------------------------------------------------
    if(font_frames.number_of_frames() < ('~' - '!' + 1)) // are there enough frames?
        segment_error = LASERBOY_BAD_FONT_FILE;
    else
    {
        int                   text_index,
                              vertex_index;

        double                length     = p0 | p1;
//                              y_rotation = atan2(p1.z - p0.z,
//                                                 p1.x - p0.x
//                                                );

//        LaserBoy_3D_short     p1_minus_y_rotation = rotate_vertex_on_coordinates_y(p1, p0, -y_rotation);
        LaserBoy_3D_double    float_3D;
        LaserBoy_real_segment real_segment(p_space);
        LaserBoy_vertex       vertex;
        //--------------------------------------------------------------------
        vertex.blank();
        vertex.c = p_space->selected_color_index;
        vertex.r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
        vertex.g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
        vertex.b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
        //--------------------------------------------------------------------
        vertex.unblank();
        for(text_index = 0; text_index < (int)text.size(); text_index++)
        {
            if(text[text_index] >= '!' && text[text_index] <= '~')
            {
                for(vertex_index = 0; vertex_index < (int)font_frames[text[text_index] - '!'].size(); vertex_index++)
                {
                    float_3D   = font_frames[text[text_index] - '!'].at(vertex_index);
                    //--------------------------------------------------------
                    float_3D.x = float_3D.x + (   text_index
                                                * (   p_space->rendered_mono_font_space
                                                    * LASERBOY_MAX_SHORT
                                                  )
                                              );
                    //--------------------------------------------------------
                    if(font_frames[text[text_index] - '!'].at(vertex_index).is_black(p_space->black_level))
                        real_segment.push_back(LaserBoy_real_vertex(float_3D,
                                                                    LaserBoy_color(0,0,0),
                                                                    vertex.k,
                                                                    p_space->palette_picker(palette_index).black
                                                                   )
                                              );
                    else
                        real_segment.push_back(LaserBoy_real_vertex(float_3D,
                                                                    vertex.as_LaserBoy_color(),
                                                                    vertex.k,
                                                                    vertex.c
                                                                   )
                                              );
                    if(font_frames[text[text_index] - '!'].at(vertex_index).is_blank())
                        real_segment.back().blank();
                }
            }
        }
        //--------------------------------------------------------------------
        for(vertex_index = 0; vertex_index < (int)real_segment.size(); vertex_index++)
            push_back(LaserBoy_vertex(   real_segment[vertex_index]
                                       * (length / real_segment.segment_width())
                                       + p0
                                     )
                     );
        //--------------------------------------------------------------------
        sync_rgb_and_palette();
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space* ps,
                                   LaserBoy_vertex p0,
                                   LaserBoy_vertex p1,
                                   string          font,
                                   string          text,
                                   int = 0
                                  )
                : p_space       (ps)
                , palette_index (p_space->current_frame().palette_index)
                , segment_error (LASERBOY_OK)
{   // variable spaced font constructor
    //------------------------------------------------------------------------
    long int bytes_skipped = 0;
    LaserBoy_frame_set font_frames(p_space);
    font_frames.from_ild_file(font, bytes_skipped);
    //------------------------------------------------------------------------
    if(font_frames.number_of_frames() < ('~' - '!' + 1)) // are there enough frames?
        segment_error = LASERBOY_BAD_FONT_FILE;
    else
    {
        int                   text_index,
                              vertex_index;

        double                glyph_offset          ,
                              widest_glyph       = 0,
                              accumulated_offset = 0,
                              length             = p0 | p1;
//                              y_rotation         = atan2(p1.z - p0.z,
//                                                         p1.x - p0.x
//                                                        );

//        LaserBoy_3D_short     p1_minus_y_rotation = rotate_vertex_on_coordinates_y(p1, p0, -y_rotation);
        LaserBoy_3D_double    float_3D;
        LaserBoy_real_segment real_segment(p_space);
        LaserBoy_vertex       vertex;
        
        for(u_int i = 0; i < font_frames.size(); i++)
            if(widest_glyph < font_frames[i].segment_width())
                widest_glyph = font_frames[i].segment_width();
        //--------------------------------------------------------------------
        vertex.blank();
        vertex.c = p_space->selected_color_index;
        vertex.r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
        vertex.g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
        vertex.b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
        //--------------------------------------------------------------------
        vertex.unblank();
        for(text_index = 0; text_index < (int)text.size(); text_index++)
        {
            if(text[text_index] >= '!' && text[text_index] <= '~')
            {
                glyph_offset = font_frames[text[text_index] - '!'].segment_left().x;
                for(vertex_index = 0; vertex_index < (int)font_frames[text[text_index] - '!'].size(); vertex_index++)
                {
                    float_3D   = font_frames[text[text_index] - '!'].at(vertex_index);
                    //--------------------------------------------------------
                    float_3D.x = (float_3D.x - glyph_offset) + accumulated_offset;
                    //--------------------------------------------------------
                    if(font_frames[text[text_index] - '!'].at(vertex_index).is_black(p_space->black_level))
                        real_segment.push_back(LaserBoy_real_vertex(float_3D,
                                                                    LaserBoy_color(0,0,0),
                                                                    vertex.k,
                                                                    p_space->palette_picker(palette_index).black
                                                                   )
                                              );
                    else
                        real_segment.push_back(LaserBoy_real_vertex(float_3D,
                                                                    vertex.as_LaserBoy_color(),
                                                                    vertex.k,
                                                                    vertex.c
                                                                   )
                                              );
                    if(font_frames[text[text_index] - '!'].at(vertex_index).is_blank())
                        real_segment.back().blank();
                }
                accumulated_offset += (   font_frames[text[text_index] - '!'].segment_width()
                                        + (   p_space->rendered_vari_font_gap
                                            * widest_glyph // percent of widest glyph
                                          )
                                      );
            } // end if(text[text_index] >= '!' && text[text_index] <= '~')
            else // space character
                accumulated_offset += (   widest_glyph // space is the widest glyph
                                        + (   p_space->rendered_vari_font_gap
                                            * widest_glyph
                                          )
                                      );
        } // end for(text_index = 0; text_index < (int)text.size(); text_index++)
        //--------------------------------------------------------------------
        for(vertex_index = 0; vertex_index < (int)real_segment.size(); vertex_index++)
            push_back(LaserBoy_vertex(   real_segment[vertex_index]
                                       * (length / real_segment.segment_width())
                                       + p0
                                     )
                     );
        //--------------------------------------------------------------------
        sync_rgb_and_palette();
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   LaserBoy_3D_short arc_start,
                                   double            arc_angle
                                  )
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{   // 3D circular arc constructor
    while(arc_angle <= 0)    arc_angle += 360;
    u_int  total_vertices = (u_int)ceil(arc_angle / p_space->rendered_arc_angle);
    double arc_step       = arc_angle / total_vertices,
           radius         = center | arc_start, // distance between
           y_rotation     = atan2(arc_start.z - center.z, arc_start.x - center.x),
           z_rotation;
    LaserBoy_vertex vertex(arc_start);
    //------------------------------------------------------------------------
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex.blank();
    push_back(LaserBoy_vertex(rotate_vertex_on_coordinates_y(vertex, center, -y_rotation),
                              (LaserBoy_color)vertex
                             )
             );
    vertex.unblank();
    //------------------------------------------------------------------------
    z_rotation = atan2(back().y - center.y, back().x - center.x);
    arc_step *= one_degree; // convert to radians!
    //------------------------------------------------------------------------
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    for(u_int a = 1; a <= total_vertices; a++)
    {
        vertex.x = (short)(radius * cos(a * arc_step + z_rotation));
        vertex.y = (short)(radius * sin(a * arc_step + z_rotation));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    rotate_on_coordinates_y(center, y_rotation);
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   LaserBoy_3D_short arc_start,
                                   double            arc_angle,
                                   double            radii_ratio
                                  )
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{   // 3D elliptical arc constructor
    while(arc_angle <= 0)    arc_angle += 360;
    u_int  total_vertices = (u_int)ceil(arc_angle / p_space->rendered_arc_angle);
    double arc_step       = arc_angle / total_vertices,
           radius_1       = center | arc_start,
           radius_2       = radius_1 * radii_ratio,
           y_rotation     = atan2(arc_start.z - center.z, arc_start.x - center.x),
           z_rotation;
    LaserBoy_vertex vertex(arc_start);
    //------------------------------------------------------------------------
    vertex.c   = p_space->selected_color_index;
    vertex.r   = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g   = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b   = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex     = rotate_vertex_on_coordinates_y(arc_start, center, -y_rotation);
    z_rotation = atan2(vertex.y - center.y, vertex.x - center.x);
    vertex.x   = (short)(radius_1);
    vertex.y   = 0;
    vertex.z   = center.z;
    vertex.blank();
    push_back(LaserBoy_vertex(vertex + center));
    vertex.unblank();
    arc_step *= one_degree; // convert to radians!
    //------------------------------------------------------------------------
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    for(u_int a = 1; a <= total_vertices; a++)
    {
        vertex.x = (short)(radius_1 * cos(a * arc_step));
        vertex.y = (short)(radius_2 * sin(a * arc_step));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    rotate_on_coordinates_z(center, z_rotation);
    rotate_on_coordinates_y(center, y_rotation);
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   LaserBoy_3D_short first_vertex,
                                   u_int             number_of_sides
                                  )
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{   // 3D polygon constructor
    double arc_step   = two_pi / number_of_sides,
           radius     = center | first_vertex,
           y_rotation = atan2(first_vertex.z - center.z, first_vertex.x - center.x);

    LaserBoy_vertex vertex(first_vertex);
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    push_back(LaserBoy_vertex(rotate_vertex_on_coordinates_y(vertex, center, -y_rotation),
                              (LaserBoy_color)vertex
                             )
             );
    vertex.unblank();
    double z_rotation = atan2(back().y - center.y, back().x - center.x);
    if(number_of_sides > 15000) // that's insane!
        number_of_sides = 15000;
    for(u_int a = 1; a <= number_of_sides; a++)
    {
        vertex.x = (short)(radius * cos(a * arc_step + z_rotation));
        vertex.y = (short)(radius * sin(a * arc_step + z_rotation));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    rotate_on_coordinates_y(center, y_rotation);
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   LaserBoy_3D_short first_vertex,
                                   u_int             number_of_points,
                                   double            ratio
                                  )
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{   // 3D polystar constructor
    u_int  a;
    double arc_step   = two_pi / number_of_points,
           radius     = center | first_vertex,
           y_rotation = atan2(first_vertex.z - center.z, first_vertex.x - center.x);
    LaserBoy_vertex vertex(first_vertex);
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    push_back(LaserBoy_vertex(rotate_vertex_on_coordinates_y(vertex, center, -y_rotation),
                              (LaserBoy_color)vertex
                             )
             );
    vertex.unblank();
    double z_rotation = atan2(back().y - center.y, back().x - center.x);
    if(number_of_points > 7500) // two vertices per star point
        number_of_points = 7500;
    for(a = 0; a <= number_of_points - 1; a++)
    {
        vertex.x = (short)(radius * cos(a * arc_step + z_rotation));
        vertex.y = (short)(radius * sin(a * arc_step + z_rotation));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));

        vertex.x = (short)(radius * ratio * cos(a * arc_step + arc_step / 2 + z_rotation));
        vertex.y = (short)(radius * ratio * sin(a * arc_step + arc_step / 2 + z_rotation));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    vertex.x = (short)(radius * cos(a * arc_step + z_rotation));
    vertex.y = (short)(radius * sin(a * arc_step + z_rotation));
    vertex.z = center.z;
    push_back(LaserBoy_vertex(vertex + center));
    rotate_on_coordinates_y(center, y_rotation);
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   int               radius,
                                   int               pedals_numerator,
                                   int               pedals_denominator
                                  ) // rhodonea
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_step,
           arc_angle,
           ratio = pedals_numerator / (double)pedals_denominator;
    LaserBoy_vertex vertex;
    arc_angle = pedals_denominator / greatest_common_devisor(pedals_numerator, pedals_denominator) * 360;
    total_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex.x = (short)(radius);
    vertex.y = 0;
    vertex.z = center.z;
    push_back(LaserBoy_vertex(vertex + center));
    vertex.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        vertex.x = (short)(radius * cos(ratio * a * arc_step) * cos(a * arc_step));
        vertex.y = (short)(radius * cos(ratio * a * arc_step) * sin(a * arc_step));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   int               center_radius,
                                   LaserBoy_3D_short center,
                                   int               roller_radius,
                                   int               roller_offset
                                  ) // epitrochoid and epicycloid when roller_radius == roller_offset
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = center_radius / (double)roller_radius;
    LaserBoy_vertex vertex;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360;
    else
        arc_angle = roller_radius / greatest_common_devisor(center_radius, roller_radius) * 360;
    total_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex.x = (short)(roller_radius * (ratio + 1) - roller_offset);
    vertex.y = 0;
    vertex.z = center.z;
    push_back(LaserBoy_vertex(vertex + center));
    vertex.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        vertex.x = (short)(   roller_radius * (ratio + 1) * cos(a * arc_step)
                            - roller_offset * cos((ratio + 1) * a * arc_step)
                          );
        vertex.y = (short)(   roller_radius * (ratio + 1) * sin(a * arc_step)
                            - roller_offset * sin((ratio + 1) * a * arc_step)
                          );
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   int               center_radius,
                                   int               roller_radius,
                                   int               roller_offset,
                                   LaserBoy_3D_short center
                                  ) // hypotrochoid and hypocycloid when roller_radius == roller_offset
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = center_radius / (double)roller_radius;
    LaserBoy_vertex vertex;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360;
    else
        arc_angle = roller_radius / greatest_common_devisor(center_radius, roller_radius) * 360;
    total_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex.x = (short)((center_radius - roller_radius) + roller_offset);
    vertex.y = 0;
    vertex.z = center.z;
    push_back(LaserBoy_vertex(vertex + center));
    vertex.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        vertex.x = (short)(   (center_radius - roller_radius) * cos(a * arc_step)
                            + roller_offset * cos((ratio - 1) * a * arc_step)
                          );
        vertex.y = (short)(   (center_radius - roller_radius) * sin(a * arc_step)
                            - roller_offset * sin((ratio - 1) * a * arc_step)
                          );
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_space*   ps,
                                   LaserBoy_3D_short center,
                                   int               amplitude_x,
                                   int               amplitude_y,
                                   int               frequency_x,
                                   int               frequency_y,
                                   double            phase_x,
                                   double            phase_y
                                  ) // Lissajous curve
                : p_space       (ps),
                  palette_index (LASERBOY_ILDA_DEFAULT),
                  segment_error (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = frequency_x / frequency_x;
    LaserBoy_vertex vertex;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360;
    else
        arc_angle = frequency_x / greatest_common_devisor(frequency_y, frequency_x) * 360;
    total_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
    if(total_vertices > 15000) // that's a lot!
        total_vertices = 15000;
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    phase_x  *= one_degree;
    phase_y  *= one_degree;
    vertex.blank();
    vertex.c = p_space->selected_color_index;
    vertex.r = p_space->palette_picker(p_space->palette_index)[vertex.c].r;
    vertex.g = p_space->palette_picker(p_space->palette_index)[vertex.c].g;
    vertex.b = p_space->palette_picker(p_space->palette_index)[vertex.c].b;
    vertex.x = (short)(amplitude_x * sin(phase_x));
    vertex.y = (short)(amplitude_y * sin(phase_y));
    vertex.z = center.z;
    push_back(LaserBoy_vertex(vertex + center));
    vertex.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        vertex.x = (short)(amplitude_x * sin(frequency_x * a * arc_step + phase_x));
        vertex.y = (short)(amplitude_y * sin(frequency_y * a * arc_step + phase_y));
        vertex.z = center.z;
        push_back(LaserBoy_vertex(vertex + center));
    }
    if(size() > LASERBOY_MAX_USHORT)
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
}

//############################################################################
u_int LaserBoy_segment::number_of_color_vectors() const
{
    u_int count = 0;
    for(u_int i = 1; i < size(); i++)
        if(at(i).is_color(p_space->black_level))
            count++;
    return count;
}

//############################################################################
u_int LaserBoy_segment::number_of_dark_vectors() const
{
    u_int count = 0;
    for(u_int i = 1; i < size(); i++)
        if(at(i).is_dark(p_space->black_level))
            count++;
    return count;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_front() const
{
    LaserBoy_3D_short front_;
    front_.z = LASERBOY_MIN_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).z > front_.z)
            front_ = at(i);
    return front_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_back() const
{
    LaserBoy_3D_short back_;
    back_.z = LASERBOY_MAX_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).z < back_.z)
            back_ = at(i);
    return back_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_top() const
{
    LaserBoy_3D_short top_;
    top_.y = LASERBOY_MIN_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).y > top_.y)
            top_ = at(i);
    return top_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_bottom() const
{
    LaserBoy_3D_short bottom_;
    bottom_.y = LASERBOY_MAX_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).y < bottom_.y)
            bottom_ = at(i);
    return bottom_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_right() const
{
    LaserBoy_3D_short right_;
    right_.x = LASERBOY_MIN_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).x > right_.x)
            right_ = at(i);
    return right_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_left() const
{
    LaserBoy_3D_short left_;
    left_.x = LASERBOY_MAX_SHORT;
    for(u_int i = 0; i < size(); i++)
        if(at(i).x < left_.x)
            left_ = at(i);
    return left_;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::vertex_farthest_from_origin() const
{
    double            distance = 0;
    LaserBoy_3D_short origin,
                      farthest_;
    for(u_int i = 0; i < size(); i++)
        if((origin | at(i)) > distance)
        {
            distance  = (origin | at(i));
            farthest_ = at(i);
        }
    return farthest_;
}

//############################################################################
short LaserBoy_segment::segment_height() const
{
    return (segment_top().y - segment_bottom().y);
}
//############################################################################
short LaserBoy_segment::segment_width() const
{
    return (segment_right().x - segment_left().x);
}

//############################################################################
short LaserBoy_segment::segment_depth() const
{
    return (segment_front().z - segment_back().z);
}

//############################################################################
short LaserBoy_segment::segment_size() const
{
    short greatest = 0;
    if(segment_width () > greatest)    greatest = segment_width ();
    if(segment_height() > greatest)    greatest = segment_height();
    if(segment_depth () > greatest)    greatest = segment_depth ();
    return greatest;
}

//############################################################################
bool LaserBoy_segment::is_closed_polygon() const
{
    if(   (first_lit_anchor() | last_lit_vector())
        < p_space->insignificant_distance
      )
        return true;
    return false;
}

//############################################################################
void LaserBoy_segment::blank_all_vertices()
{
    for(u_int i = 0; i < size(); i++)
        at(i).blank();
    return;
}

//############################################################################
void LaserBoy_segment::unblank_all_vertices()
{
    for(u_int i = 1; i < size(); i++)
        at(i).unblank();
    return;
}

//############################################################################
void LaserBoy_segment::blacken_vertices()
{
    for(u_int i = 0; i < size(); i++)
    {
        at(i).r = 0;
        at(i).g = 0;
        at(i).b = 0;
        at(i).c = p_space->palette_picker(palette_index).black;
    }
    return;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::reverse()
{
    int               i;
    LaserBoy_segment  reversed(p_space);
    LaserBoy_vertex   vertex;

    if(size() > 2)
    {
        reversed.reserve(size());
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
            i--; // i goes to -1 !
        }
        //--------------------------------------------------------------------
        *this = reversed;
    }
    else if(size() == 2)
    {
        //--------------------------------------------------------------------
        reversed.reserve(2);
        vertex   = at(1);
        vertex.blank();
        reversed.push_back(vertex);
        vertex   = at(0);
        vertex.r = at(1).r;
        vertex.g = at(1).g;
        vertex.b = at(1).b;
        vertex.c = at(1).c;
        vertex.k = at(1).k;
        reversed.push_back(vertex);
        *this = reversed;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::reverse_sub_segment(u_int p1, u_int p2) // vertex indices
{
    //------------------------------------------------------------------------
    if(p1 != p2)
    {
        if((int)size() <= 2)
            reverse();
        else
        {
            u_int                i;
            LaserBoy_segment   segment  (p_space),
                               reversed (p_space);
            //----------------------------------------------------------------
            if(p2 < p1)
            {
                i  = p1;
                p1 = p2 - 1;
                p2 = i;
            }
            //----------------------------------------------------------------
            for(i = 0; i < p1; i++)
                segment += at(i);
            //----------------------------------------------------------------
            for(i = p1; i <= p2; i++)
                reversed += at(i);
            //----------------------------------------------------------------
            reversed.reverse();
            segment += reversed;
            //----------------------------------------------------------------
            for(i = p2 + 1; i < size(); i++)
                segment += at(i);
            //----------------------------------------------------------------
            *this = segment;
        }
    }
    //------------------------------------------------------------------------
    return *this;
}


//############################################################################
LaserBoy_segment& LaserBoy_segment::reorder_from(u_int vertex_index)
{
    if(    vertex_index
        && size() > 1
      )
    {
        u_int             i;
        LaserBoy_segment  reordered(p_space);
        //--------------------------------------------------------------------
        for(i = vertex_index; i < size(); i++)
            reordered += at(i);

        reordered.front().blank();

        for(i = 0; i <= vertex_index; i++)
            reordered += at(i);

        *this = reordered;
        //--------------------------------------------------------------------
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::randomize_segments()
{
    if(number_of_segments() > 1)
    {
        LaserBoy_segment segment(p_space),
                         randomized(p_space);
        vector<bool>     been_here(number_of_segments());
        u_int            i,
                         total_segments = number_of_segments();
        //--------------------------------------------------------------------
        u_int random_index = rand() % total_segments;
        //--------------------------------------------------------------------
        for(i = 0; i < total_segments; i++)
            been_here[i] = false;
        //--------------------------------------------------------------------
        for(i = 0; i < total_segments; i++)
        {
            while(been_here[random_index])
            {
                random_index = rand() % total_segments;
            }
            segment = copy_segment(random_index);
            if(rand() % 2)
                segment.reverse();
            randomized += segment;
            been_here[random_index] = true;
        }
        //--------------------------------------------------------------------
        *this = randomized;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::conglomerate_lit_segments()
{
    if(number_of_segments() > 1)
    {
        bool                match_found = false;
        int                 i,
                            j,
                            least_angle_index;
        double              angle,
                            least_angle;
        vector<int>         match_index;
        vector<char>        match_index_type;
        LaserBoy_segment    copy(*this);
        LaserBoy_frame      frame(p_space);
        LaserBoy_frame_set  lit_vectors(p_space);
        //--------------------------------------------------------------------
        if(p_space->fracture_b4_conglomerate)
            copy.fracture_segments();
        lit_vectors = copy.explode_segments();
        lit_vectors.minimize();
        //--------------------------------------------------------------------
        p_space->p_GUI->display_state("conglomerating segments");
        do
        {
            p_space->p_GUI->display_progress(lit_vectors.number_of_frames());
            for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            {
                match_found = false;
                match_index.clear();
                match_index_type.clear();
                for(j = i + 1; j < (int)lit_vectors.number_of_frames(); j++)
                {
                    if(         lit_vectors[i].last_lit_vector().as_3D_short()
                             == lit_vectors[j].first_lit_anchor().as_3D_short()
                      )
                    {
                        match_found = true;
                        match_index_type.push_back('a');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].last_lit_vector().as_3D_short()
                             == lit_vectors[j].last_lit_vector().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('b');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_short()
                             == lit_vectors[j].last_lit_vector().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('c');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_short()
                             == lit_vectors[j].first_lit_anchor().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('d');
                        match_index.push_back(j);
                    }
                } // end for(j = i; j < (int)lit_vectors.number_of_frames(); j++)
                //------------------------------------------------------------
                angle             = pi;
                least_angle       = pi;
                least_angle_index = 0;
                //------------------------------------------------------------
                if(match_index.size())
                {
                    for(j = 0; j < (int)match_index.size(); j++)
                    {
                        switch(match_index_type[j])
                        {
                            case 'a':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].first_lit_vector()
                                                         );
                                break;
                            case 'b':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'c':
                                angle = three_point_angle(lit_vectors[i].first_lit_vector(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'd':
                                angle = three_point_angle(lit_vectors[i].first_lit_vector(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].first_lit_vector()
                                                         );
                                break;
                        } // end switch(match_index_type[j])
                        if(angle <= least_angle)
                        {
                            least_angle = angle;
                            least_angle_index = j;
                        }
                    } // end for(j = 0; j < (int)match_index.size(); j++)
                    //--------------------------------------------------------
                    switch(match_index_type[least_angle_index])
                    {
                        case 'a':
                            break;
                        case 'b':
                            lit_vectors[match_index[least_angle_index]].reverse();
                            break;
                        case 'c':
                            lit_vectors.swap_frames(i, match_index[least_angle_index]);
                            break;
                        case 'd':
                            lit_vectors[i].reverse();
                            break;
                    } // end switch(match_index_type[least_angle_index])
                    //--------------------------------------------------------
                    lit_vectors[i].is_selected = true;
                    lit_vectors[match_index[least_angle_index]].is_selected = true;
                    lit_vectors.collapse_selected_frames();
                    lit_vectors[i].bond_segments();
                    //--------------------------------------------------------
                    break;
                } // end if(match_found)
                //------------------------------------------------------------
            } // end for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
        } while(match_found);
        //--------------------------------------------------------------------
        lit_vectors.minimize();
        for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            frame += lit_vectors[i];
        *this = frame;
    }
    return *this;
}

//############################################################################
LaserBoy_segment LaserBoy_segment::shortest_path_of_segments(const LaserBoy_segment& previous_frame)
{
    LaserBoy_frame_set  segments = explode_segments();
    long int            j,
                        points_away,
                        points_away_temp;
    LaserBoy_3D_double  _0,
                        _1,
                        _2;
    //------------------------------------------------------------------------
    if(segments.number_of_frames() > 1)
    {
        vector<bool>        been_here;
        int                 i,
                            segments_index,
                            closest_segment_index = 0;
        double              temp_distance,
                            shortest_distance = 1000000.0; // more positive than possible
        LaserBoy_segment    reordered(p_space, palette_index, true);
        //--------------------------------------------------------------------
        reordered.reserve(size());
        //--------------------------------------------------------------------
        for(i = 0; i < (int)segments.number_of_frames(); i++)
        {
            been_here.push_back(false);
            // ---------------------------------------------------------------
            temp_distance =   segments[i].first_lit_anchor()
                            | previous_frame.last_lit_vector();
            if(temp_distance < shortest_distance)
            {
                shortest_distance = temp_distance;
                closest_segment_index = (i + 1); // can not be zero
            }
            else if(temp_distance == shortest_distance)
            {
                if(   segments[   (closest_segment_index > 0)
                                ? (closest_segment_index - 1)
                                : (-(closest_segment_index - 1))
                              ].total_magnitude()
                    > segments[i].total_magnitude()
                  )
                   closest_segment_index = (i + 1); // can not be zero
            }
            // ---------------------------------------------------------------
            temp_distance =   segments[i].last_lit_vector()
                            | previous_frame.last_lit_vector();
            if(temp_distance < shortest_distance)
            {
                shortest_distance = temp_distance;
                closest_segment_index = -(i + 1);
            }
            else if(temp_distance == shortest_distance)
            {
                if(   segments[   (closest_segment_index > 0)
                                ? (closest_segment_index - 1)
                                : (-(closest_segment_index - 1))
                              ].total_magnitude()
                    > segments[i].total_magnitude()
                  )
                    closest_segment_index = -(i + 1);
            }
        } // end for(i = 0; i < segments.number_of_frames(); i++)
        //--------------------------------------------------------------------
        if(closest_segment_index > 0)
        {
            reordered = segments[closest_segment_index - 1];
            been_here[closest_segment_index - 1] = true;
        }
        else
        {
            reordered = (segments[(-closest_segment_index) - 1].reverse());
            been_here[(-closest_segment_index) - 1] = true;
        }
        //--------------------------------------------------------------------
        for(segments_index = 1; segments_index < (int)segments.number_of_frames(); segments_index++)
        {
            points_away           = LONG_MAX;
            closest_segment_index = 0;
            for(i = 0; i < (int)segments.number_of_frames(); i++)
            {
                if(!been_here[i])
                {
                    //--------------------------------------------------------
                    // this end of the next segment
                    //--------------------------------------------------------
                    _0 = reordered.last_lit_anchor();
                    _1 = reordered.last_lit_vector();
                    _2 = segments[i].first_lit_anchor();
                    j  = reordered.last_lit_anchor_index();
                    while(    (_0 == _1)
                           && (--j >= 0)
                         )
                        _0 = reordered.at(j);
                    if(    (_0 == _1)
                        || (_1 == _2)
                      )
                        points_away_temp = 0;
                    else if(_0 == _2)
                        points_away_temp = end_dwell_vertices(pi,
                                                              p_space->sample_rate,
                                                              p_space->max_dwell_microsec
                                                             );
                    else
                        points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                          _0 | _1,
                                                                          _1 | _2
                                                                         ),
                                                              p_space->sample_rate,
                                                              p_space->max_dwell_microsec
                                                             );
                    //--------------------------------------------------------
                    if((reordered.last_lit_vector() | segments[i].first_lit_anchor()) > p_space->insignificant_distance)
                    {
                        //----------------------------------------------------
                        points_away_temp += linear_steps(reordered.last_lit_vector(),
                                                         segments[i].first_lit_anchor(),
                                                         p_space->blank_delta_max
                                                        );
                        //----------------------------------------------------
                        _0 = reordered.last_lit_vector();
                        _1 = segments[i].first_lit_anchor();
                        _2 = segments[i].first_lit_vector();
                        j  = segments[i].first_lit_vector_index();
                        while(    (_1 == _2)
                               && (++j < (int)segments[i].size())
                             )
                            _2 = segments[i].at(j);

                        j  = reordered.last_lit_vector_index();
                        while(    (_0 == _1)
                               && (--j >= 0)
                             )
                            _0 = reordered.at(j);


                        if(    (_0 == _1)
                            || (_1 == _2)
                          )
                            points_away_temp += 0;
                        else if(_0 == _2)
                            points_away_temp += start_dwell_vertices(pi,
                                                                     p_space->sample_rate,
                                                                     p_space->max_dwell_microsec
                                                                    );
                        else
                            points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                                 _0 | _1,
                                                                                 _1 | _2
                                                                                ),
                                                                     p_space->sample_rate,
                                                                     p_space->max_dwell_microsec
                                                                    );
                    }
                    //--------------------------------------------------------
                    if(points_away_temp < points_away)
                    {
                        points_away = points_away_temp;
                        closest_segment_index = (i + 1);
                    }
                    else if(points_away_temp == points_away) // it's a toss up between two positions
                    {
                        if(   segments[   (closest_segment_index > 0)
                                        ? (closest_segment_index - 1)
                                        : (-(closest_segment_index - 1))
                                      ].total_magnitude()
                            > segments[i].total_magnitude()
                          )
                        {
                            points_away = points_away_temp;
                            closest_segment_index = (i + 1);
                        }
                    }
                    //--------------------------------------------------------
                    // if the next segment ends are far enough apart
                    //--------------------------------------------------------
                    if(segments[i].total_magnitude() > p_space->insignificant_distance)
                    {
                        //----------------------------------------------------
                        // look at the other end of the next segment
                        //----------------------------------------------------
                        _0 = reordered.last_lit_anchor();
                        _1 = reordered.last_lit_vector();
                        _2 = segments[i].last_lit_vector();
                        j  = segments[i].last_lit_vector_index();
                        while(    (_1 == _2)
                               && (--j >= 0)
                             )
                            _2 = segments[i].at(j);
                        j  = reordered.last_lit_anchor_index();
                        while(    (_0 == _1)
                               && (--j >= 0)
                             )
                            _0 = reordered.at(j);
                        if(    (_0 == _1)
                            || (_1 == _2)
                          )
                            points_away_temp = 0;
                        else if(_0 == _2)
                            points_away_temp = end_dwell_vertices(pi,
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                        else
                            points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                              _0 | _1,
                                                                              _1 | _2
                                                                             ),
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                        //----------------------------------------------------
                        if((reordered.last_lit_vector() | segments[i].last_lit_vector()) > p_space->insignificant_distance)
                        {
                            //------------------------------------------------
                            points_away_temp += linear_steps(reordered.last_lit_vector(),
                                                             segments[i].last_lit_vector(),
                                                             p_space->blank_delta_max
                                                            );
                            //------------------------------------------------
                            _0 = reordered.last_lit_vector();
                            _1 = segments[i].last_lit_vector();
                            _2 = segments[i].last_lit_anchor();
                            j  = segments[i].last_lit_anchor_index();
                            while(    (_1 == _2)
                                   && (++j < (int)segments[i].size())
                                 )
                                _2 = segments[i].at(j);
                            j  = reordered.last_lit_vector_index();
                            while(    (_0 == _1)
                                   && (--j >= 0)
                                 )
                                _0 = reordered.at(j);
                            if(    (_0 == _1)
                                || (_1 == _2)
                              )
                                points_away_temp += 0;
                            else if(_0 == _2)
                                points_away_temp += start_dwell_vertices(pi,
                                                                         p_space->sample_rate,
                                                                         p_space->max_dwell_microsec
                                                                        );
                            else
                                points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                                     _0 | _1,
                                                                                     _1 | _2
                                                                                    ),
                                                                         p_space->sample_rate,
                                                                         p_space->max_dwell_microsec
                                                                        );
                        }
                        //----------------------------------------------------
                        if(points_away_temp < points_away)
                        {
                            points_away = points_away_temp;
                            closest_segment_index = -(i + 1);
                        }
                        else if(points_away_temp == points_away) // it's a toss up between two positions
                        {
                            if(   segments[   (closest_segment_index > 0)
                                            ? (closest_segment_index - 1)
                                            : (-(closest_segment_index - 1))
                                          ].total_magnitude()
                                > segments[i].total_magnitude()
                              )
                            {
                                points_away = points_away_temp;
                                closest_segment_index = -(i + 1);
                            }
                        }
                    } // end if the segment ends are far enough apart
                }
            }
            if(closest_segment_index > 0)
            {
                reordered += segments[closest_segment_index - 1];
                been_here[closest_segment_index - 1] = true;
            }
            else // (closest_segment_index < 0)
            {
                reordered += (segments[(-closest_segment_index) - 1].reverse());
                been_here[(-closest_segment_index) - 1] = true;
            }
        }
        //--------------------------------------------------------------------
//        while(reordered.back().is_blank())
//            reordered.pop_back();
        return reordered;
        //--------------------------------------------------------------------
    } // end if(segments.number_of_frames() > 1)
    else // there is only one lit segment
    {
        //--------------------------------------------------------
        // this end of the segment
        //--------------------------------------------------------
        _0 = previous_frame.last_lit_anchor();
        _1 = previous_frame.last_lit_vector();
        _2 = segments[0].first_lit_anchor();
        j  = previous_frame.last_lit_anchor_index();
        while(    (_0 == _1)
               && (--j >= 0)
             )
            _0 = previous_frame.at(j);
        if(    (_0 == _1)
            || (_1 == _2)
          )
            points_away = 0;
        else if(_0 == _2)
            points_away = end_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
        else
            points_away = end_dwell_vertices(delta_angle(_0 | _2,
                                                         _0 | _1,
                                                         _1 | _2
                                                        ),
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
        //--------------------------------------------------------
        if(   (   previous_frame.last_lit_vector()
                | segments[0].first_lit_anchor()
              )
            > p_space->insignificant_distance
          )
        {
            //----------------------------------------------------
            points_away += linear_steps(previous_frame.last_lit_vector(),
                                        segments[0].first_lit_anchor(),
                                        p_space->blank_delta_max
                                       );
            //----------------------------------------------------
            _0 = previous_frame.last_lit_vector();
            _1 = segments[0].first_lit_anchor();
            _2 = segments[0].first_lit_vector();
            j  = segments[0].first_lit_vector_index();
            while(    (_1 == _2)
                   && (++j < (int)segments[0].size())
                 )
                _2 = segments[0].at(j);

            j  = previous_frame.last_lit_vector_index();
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = previous_frame.at(j);


            if(    (_0 == _1)
                || (_1 == _2)
              )
                points_away += 0;
            else if(_0 == _2)
                points_away += start_dwell_vertices(pi,
                                                    p_space->sample_rate,
                                                    p_space->max_dwell_microsec
                                                   );
            else
                points_away += start_dwell_vertices(delta_angle(_0 | _2,
                                                                _0 | _1,
                                                                _1 | _2
                                                               ),
                                                    p_space->sample_rate,
                                                    p_space->max_dwell_microsec
                                                   );
        }
        //--------------------------------------------------------
        // if the segment ends are far enough apart
        //--------------------------------------------------------
        if(segments[0].total_magnitude() > p_space->insignificant_distance)
        {
            //----------------------------------------------------
            // look at the other end of the segment
            //----------------------------------------------------
            _0 = previous_frame.last_lit_anchor();
            _1 = previous_frame.last_lit_vector();
            _2 = segments[0].last_lit_vector();
            j  = segments[0].last_lit_vector_index();
            while(    (_1 == _2)
                   && (--j >= 0)
                 )
                _2 = segments[0].at(j);
            j  = previous_frame.last_lit_anchor_index();
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = previous_frame.at(j);
            if(    (_0 == _1)
                || (_1 == _2)
              )
                points_away_temp = 0;
            else if(_0 == _2)
                points_away_temp = end_dwell_vertices(pi,
                                                      p_space->sample_rate,
                                                      p_space->max_dwell_microsec
                                                     );
            else
                points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                  _0 | _1,
                                                                  _1 | _2
                                                                 ),
                                                      p_space->sample_rate,
                                                      p_space->max_dwell_microsec
                                                     );
            //----------------------------------------------------
            if((previous_frame.last_lit_vector() | segments[0].last_lit_vector()) > p_space->insignificant_distance)
            {
                //------------------------------------------------
                points_away_temp += linear_steps(previous_frame.last_lit_vector(),
                                                 segments[0].last_lit_vector(),
                                                 p_space->blank_delta_max
                                                );
                //------------------------------------------------
                _0 = previous_frame.last_lit_vector();
                _1 = segments[0].last_lit_vector();
                _2 = segments[0].last_lit_anchor();
                j  = segments[0].last_lit_anchor_index();
                while(    (_1 == _2)
                       && (++j < (int)segments[0].size())
                     )
                    _2 = segments[0].at(j);
                j  = previous_frame.last_lit_vector_index();
                while(    (_0 == _1)
                       && (--j >= 0)
                     )
                    _0 = previous_frame.at(j);
                if(    (_0 == _1)
                    || (_1 == _2)
                  )
                    points_away_temp += 0;
                else if(_0 == _2)
                    points_away_temp += start_dwell_vertices(pi,
                                                             p_space->sample_rate,
                                                             p_space->max_dwell_microsec
                                                            );
                else
                    points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                         _0 | _1,
                                                                         _1 | _2
                                                                        ),
                                                             p_space->sample_rate,
                                                             p_space->max_dwell_microsec
                                                            );
            }
            //----------------------------------------------------
            if(points_away_temp < points_away)
                segments[0].reverse();
        } // end if the segment ends are far enough apart
        return segments[0];
    }
}

//############################################################################
void LaserBoy_segment::reduce_blank_vectors()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  minimum_blanking(p_space, palette_index, false);
        //--------------------------------------------------------------------
        minimum_blanking.reserve(size());
        //--------------------------------------------------------------------
        minimum_blanking.push_back(front());
        for(i = 1; i < size(); i++)
        {
            if(    at(i).is_blank()
                && minimum_blanking.back().is_blank()
              )
                minimum_blanking.back() = at(i); // the new, last blank
            else
                minimum_blanking.push_back(at(i));
        }
        //--------------------------------------------------------------------
        while(minimum_blanking.size() && minimum_blanking.back().is_blank())
            minimum_blanking.pop_back();
        *this = minimum_blanking;
    }
    return;
}

//############################################################################
void LaserBoy_segment::omit_equivalent_vectors()
{
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set  segments(p_space);
        u_int               i,
                            j;
        //--------------------------------------------------------------------
        fracture_segments();
        segments = explode_segments();
        segments.minimize();
        //--------------------------------------------------------------------
        for(i = 0; i < segments.number_of_frames() - 1; i++)
        {
            for(j = i + 1; j < segments.number_of_frames(); j++)
                if(    segments[i] == segments[j]
                    || segments[i] == segments[j].reverse()
                  )
                    segments[j].clear();
        }
        //--------------------------------------------------------------------
        *this = segments.sum_of_frames();
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_segment::remove_dots()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  no_dots(p_space, palette_index, false);
        //--------------------------------------------------------------------
        no_dots.reserve(size());
        //--------------------------------------------------------------------
        no_dots.push_back(front());
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at_index_has_magnitude(i))
                no_dots.push_back(at(i));
        }
        //--------------------------------------------------------------------
        *this = no_dots;
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::enhance_dots()
{
    if(size() > 1)
    {
        LaserBoy_segment  more_dots(p_space, palette_index, false);
        //--------------------------------------------------------------------
        more_dots.reserve(size());
        //--------------------------------------------------------------------
        more_dots.push_back(front());
        //--------------------------------------------------------------------
        for(u_int i = 1; i < size(); i++)
        {
            if(    at(i).is_color(p_space->black_level)
                && !at_index_has_magnitude(i)
              )
            {
                for(u_int j = 1; j < p_space->dwell_on_dot; j++)
                    more_dots.push_back(at(i));
            }
            more_dots.push_back(at(i));
        }
        //--------------------------------------------------------------------
        if(more_dots.size() <= LASERBOY_MAX_USHORT)
            *this = more_dots;
        else
            return LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return LASERBOY_OK;
}

//############################################################################
void LaserBoy_segment::remove_dwell_vertices()
{
    if(size() > 2)
    {
        u_int             i;
        LaserBoy_segment  no_dwell_vertices(p_space, palette_index, false);
        //--------------------------------------------------------------------
        no_dwell_vertices.reserve(size());
        no_dwell_vertices.push_back(front());
        //--------------------------------------------------------------------
        for(i = 1; i < size() - 1; i++)
        {
            if(    (    at(i) != at(i - 1)
                     || at(i) != at(i + 1)
                   )
                && (    at_index_has_magnitude(i)
                     || at(i + 1).is_blank()
                   )
              )
                no_dwell_vertices.push_back(at(i));
        }
        //--------------------------------------------------------------------
        no_dwell_vertices.push_back(back());
        //--------------------------------------------------------------------
        *this = no_dwell_vertices;
    }
    return;
}

//############################################################################
void LaserBoy_segment::remove_short_vectors()
{
    if(size() > 2)
    {
        int               i,
                          index;
        LaserBoy_segment  no_short_vectors(*this);
        //--------------------------------------------------------------------
        if(no_short_vectors.size() > 2)
        {
            do
            {
                index = -1;
                for(i = no_short_vectors.size() - 2; i > 1; i--)
                {
                    if(    no_short_vectors.at(i).is_color(p_space->black_level)
                        &&     no_short_vectors.at(i    ).color_of(p_space->black_level)
                            == no_short_vectors.at(i + 1).color_of(p_space->black_level)
                        && no_short_vectors.vector_magnitude(i) <= p_space->insignificant_distance
                      )
                    {
                        index = i;
                        break;
                    }
                }
                if(index != -1)
                {
                    if(index > 1)
                        no_short_vectors.at(index - 1) = (LaserBoy_3D_short) // just coordinates
                                                         no_short_vectors.at(index - 1).blend(no_short_vectors.at(index), 0.50);
                    no_short_vectors.remove_vertex(index);
                }
            } while(index != -1);
            //----------------------------------------------------------------
            *this = no_short_vectors;
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::reduce_lit_vectors()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  segment_1(p_space, palette_index, false),
                          segment_2(p_space, palette_index, false);
        //--------------------------------------------------------------------
        segment_1.reserve(size());
        segment_2.reserve(size());
        //--------------------------------------------------------------------
        segment_1.push_back(front());
        for(i = 1; i < size(); i++)
        {
            if(at(i) != segment_1.back())
                segment_1.push_back(at(i));
        }
        //--------------------------------------------------------------------
        while(segment_1.size() && segment_1.back().is_blank())
            segment_1.pop_back();
        //--------------------------------------------------------------------
        if(segment_1.size())
        {
            segment_2.push_back(segment_1.front());
            for(i = 1; i < segment_1.size() - 1; i++)
            {
                if(    (    segment_1.at(i    ).color_of(p_space->black_level)
                         != segment_1.at(i + 1).color_of(p_space->black_level)
                       )
                    || (    segment_1.vector_angle(i)
                          > p_space->insignificant_angle
                       )
                  )
                    segment_2.push_back(segment_1.at(i));
            }
            //----------------------------------------------------------------
            segment_2.push_back(segment_1.at(i));
            while(segment_2.size() && segment_2.back().is_blank())
                segment_2.pop_back();
        }
        //--------------------------------------------------------------------
        *this = segment_2;
    }
    return;
}

//############################################################################
void LaserBoy_segment::impose_bit_resolution()
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
        {
            at(i).x &=  short_bit_mask[p_space->signal_bit_mask[0]];
            at(i).y &=  short_bit_mask[p_space->signal_bit_mask[1]];
            at(i).z &=  short_bit_mask[p_space->signal_bit_mask[5]];
            at(i).r &= (short_bit_mask[p_space->signal_bit_mask[2]] >> 7);
            at(i).g &= (short_bit_mask[p_space->signal_bit_mask[3]] >> 7);
            at(i).b &= (short_bit_mask[p_space->signal_bit_mask[4]] >> 7);
        }
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::reorder_segments(const LaserBoy_segment& previous_frame)
{
    LaserBoy_segment reordered = shortest_path_of_segments(previous_frame);
    if(reordered.size() <= LASERBOY_MAX_USHORT)
        *this = reordered;
    else
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::fracture_segments()
{
    if(number_of_lit_vectors() > 1)
    {
        u_int             i;
        LaserBoy_segment  fractured(p_space, palette_index, false);
        fractured.reserve(3 * size());
        reduce_blank_vectors();
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            fractured.push_back(at(i));
            fractured.push_back(at(i));
            fractured.back().blank();
        }
        //--------------------------------------------------------------------
        fractured.pop_back();
        fractured.reduce_blank_vectors();
        if(fractured.size() <= LASERBOY_MAX_USHORT)
            *this = fractured;
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::bond_segments()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  bonded(p_space, palette_index, false);
        //--------------------------------------------------------------------
        bonded.reserve(size());
        bonded.push_back(front());
        for(i = 1; i < size() - 1; i++)
        {
            if( !(     at(i).is_blank()
                   &&    (   at(i)
                           | at(i - 1)
                         )
                       < p_space->insignificant_distance
              )  )
                bonded += at(i);
        }
        bonded.push_back(back());
        //--------------------------------------------------------------------
        *this = bonded;
    }
    return LASERBOY_OK;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_vertex(u_int vertex_index)
{
    if(!size()) // there is nothing
    {
        reserve(2);
        push_back(LaserBoy_vertex());
        push_back(LaserBoy_vertex()); // add blank, zero vector
    }
    else if(vertex_index < size())
    {
        u_int             i;
        LaserBoy_segment  segment(p_space, palette_index, false);
        
        segment.reserve(size() + 1);
        if(vertex_index < size() - 1) // not the last vertex
        {
            for(i = 0; i <= vertex_index; i++)
                segment += at(i);

            if((at(vertex_index) | at(vertex_index + 1)) > p_space->insignificant_distance)
                segment += LaserBoy_vertex(   scale_vertex_on_coordinates(   at(vertex_index + 1)
                                                                           , at(vertex_index)
                                                                           , LaserBoy_3D_double(0.5, 0.5, 0.5)
                                                                         )
                                            , at(vertex_index) // is an rgb
                                            , at(vertex_index).k
                                            , at(vertex_index).c
                                          );

            for(i = vertex_index + 1; i < size(); i++)
                segment += at(i);
        }
        else // it is the last vertex
        {
            segment = *this;
            segment += LaserBoy_vertex(   scale_vertex_on_coordinates(   back()
                                                                       , at(size() - 2)
                                                                       , LaserBoy_3D_double(2.0, 2.0, 2.0)
                                                                     )
                                        , back() // is an rgb
                                        , back().k
                                        , back().c
                                      );
        }
        if(segment.size() <= LASERBOY_MAX_USHORT)
            *this = segment;
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::break_segment(u_int& vertex_index)
{
    if(    vertex_index > 0
        && vertex_index < size() - 1
      )
    {
        u_int             i;
        LaserBoy_segment  segment(p_space, palette_index, false);

        segment.reserve(size() + 1);
        for(i = 0; i <= vertex_index; i++)
            segment += at(i);

        segment += at(vertex_index);
        segment.back().blank();

        for(i = vertex_index + 1; i < size(); i++)
            segment += at(i);

        if(segment.size() <= LASERBOY_MAX_USHORT)
        {
            vertex_index += 2; // changes the value of spider in frame class
            *this = segment;
        }
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::connect_the_dots(u_int p0, u_int p1) // vertex indices
{
    if((size() + 2) <= LASERBOY_MAX_USHORT)
    {
        if(    p0 != p1
            && p0 > 0
            && p0 < size()
            && p1 > 0
            && p1 < size()
            && (  (   at(p0)
                    | at(p1)
                  )
                > p_space->insignificant_distance
               )
          )
        {
            push_back(at(p0));
            back().blank();
            push_back(at(p1));
            back().unblank();
            back().c = p_space->selected_color_index;
            back().r = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].r;
            back().g = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].g;
            back().b = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].b;
        }
    }
    else
        segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    return segment_error;
}

//############################################################################
double LaserBoy_segment::vector_angle(u_int vertex_index) const
{
    double angle = 0.0;
    if(vertex_index != 0)
    {
        if(size() > 1)
        {
            int _0  = 0,
                _1  = 0,
                _2  = 0;
           //-----------------------------------------------------------------
            if(vertex_index > 0 && vertex_index < size() - 1)
            {
                _0 = vertex_index - 1;
                _1 = vertex_index    ;
                _2 = vertex_index + 1;
            }
           //-----------------------------------------------------------------
            else if(vertex_index == size() - 1)
            {
                _0 = size() - 2;
                _1 = 0                     ;
                _2 = 1                     ;
            }
            //----------------------------------------------------------------
            angle = delta_angle( at(_0) | at(_2), // distance between vertices 0,2
                                 at(_0) | at(_1),
                                 at(_1) | at(_2)
                               );
        }
    }
    return angle;
}

//############################################################################
double LaserBoy_segment::total_angle() const
{
    double total_angle = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        total_angle += vector_angle(i);
    return total_angle;
}

//############################################################################
double LaserBoy_segment::max_angle() const
{
    double max_angle = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        if(max_angle < vector_angle(i))
            max_angle = vector_angle(i);
    return max_angle;
}

//############################################################################
double LaserBoy_segment::vector_magnitude(u_int vertex) const
{
    if(size() > 1)
    {
        int _1 = 0,
            _2 = 0;
        //--------------------------------------------------------------------
        if(vertex >= 0 && vertex < size() - 1)
        {
            _1 = vertex    ;
            _2 = vertex + 1;
        }
        else if(vertex == size() - 1)
        {
            _1 = size() - 1;
            _2 = 0;
        }
        //--------------------------------------------------------------------
        return at(_1) | at(_2);
    }
    return 0;
}

//############################################################################
double LaserBoy_segment::total_distance() const
{
    double total_distance = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        total_distance += vector_magnitude(i);
    return total_distance;
}

//############################################################################
double LaserBoy_segment::max_distance() const
{
    double max_distance = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        if(max_distance < vector_magnitude(i))
            max_distance = vector_magnitude(i);
    return max_distance;
}

//############################################################################
double LaserBoy_segment::max_color_distance() const
{
    double max_distance = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        if(    at(i + 1).is_color(p_space->black_level)
            && max_distance < vector_magnitude(i)
          )
            max_distance = vector_magnitude(i);
    return max_distance;
}

//############################################################################
double LaserBoy_segment::max_dark_distance() const
{
    double max_distance = 0.0;
    u_int  i;
    //------------------------------------------------------------------------
    for(i = 0; i < size() - 1; i++)
        if(    at(i + 1).is_dark(p_space->black_level)
            && max_distance < vector_magnitude(i)
          )
            max_distance = vector_magnitude(i);
    return max_distance;
}

//############################################################################
double LaserBoy_segment::length_in_time() const
{
    return double(size()) / p_space->sample_rate;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_dwell()
{
    if(size() > 1)
    {
        int                 i,
                            j,
                            dwell_vertex_count,
                            dwell_vertex_total;
        LaserBoy_3D_double  _0,
                            _1,
                            _2;
        LaserBoy_vertex     vertex,
                            black_vertex;
        LaserBoy_segment    angle_optimized(p_space, palette_index, false);
        //--------------------------------------------------------------------
        angle_optimized.push_back(front());
        for(i = 1; i < (int)size() - 1; i++)
        {
            angle_optimized.push_back(at(i));
            _0 = at(i - 1);
            _1 = at(i);
            _2 = at(i + 1);
            j  = i - 1;
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = at(j);
            if(_0 == _1)
                _0 = 0.0;

            if(    (_0 == _1)
                || (_1 == _2)
              )
                dwell_vertex_total = 0;
            else
            {
                if(    at(i    ).is_lit()
                    && at(i + 1).is_blank()
                  )
                {
                    if(_0 == _2)
                        dwell_vertex_total = end_dwell_vertices(pi,
                                                                p_space->sample_rate,
                                                                p_space->max_dwell_microsec
                                                               );
                    else
                        dwell_vertex_total = end_dwell_vertices(delta_angle(_0 | _2,
                                                                            _0 | _1,
                                                                            _1 | _2
                                                                           ),
                                                                p_space->sample_rate,
                                                                p_space->max_dwell_microsec
                                                               );
                    if(dwell_vertex_total)
                    {
                        vertex = black_vertex = at(i);
                        if(    p_space->black_dwell_vertices
                            && black_vertex.color_of(p_space->black_level) // not blank or black
                          )
                        {
                            black_vertex.c = p_space->palette_picker(palette_index).black;
                            black_vertex.r =
                            black_vertex.g =
                            black_vertex.b = 0x00;
                        }
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            if(dwell_vertex_count < (int)p_space->lit_dwell_overhang)
                                angle_optimized.push_back(vertex);
                            else
                                angle_optimized.push_back(black_vertex);
                    }
                }
                else if(    at(i    ).is_blank()
                         && at(i + 1).is_lit()
                       )
                {
                    if(_0 == _2)
                        dwell_vertex_total = start_dwell_vertices(pi,
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                    else
                        dwell_vertex_total = start_dwell_vertices(delta_angle(_0 | _2,
                                                                              _0 | _1,
                                                                              _1 | _2
                                                                             ),
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                    if(dwell_vertex_total)
                    {
                        vertex = at(i);
                        vertex.blank();
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            angle_optimized.push_back(vertex);
                    }
                }
                else if(    at(i    ).is_lit()
                         && at(i + 1).is_lit()
                       )
                {
                    if(_0 == _2)
                        dwell_vertex_total = inline_dwell_vertices(pi,
                                                                   p_space->sample_rate,
                                                                   p_space->max_dwell_microsec
                                                                  );
                    else
                        dwell_vertex_total = inline_dwell_vertices(delta_angle(_0 | _2,
                                                                               _0 | _1,
                                                                               _1 | _2
                                                                              ),
                                                                   p_space->sample_rate,
                                                                   p_space->max_dwell_microsec
                                                                  );
                    if(dwell_vertex_total)
                    {
                        vertex = black_vertex = at(i);
                        if(    p_space->black_dwell_vertices
                            && black_vertex.color_of(p_space->black_level) > 0x00 // not blank or black
                          )
                        {
                            black_vertex.c = p_space->palette_picker(palette_index).black;
                            black_vertex.r =
                            black_vertex.g =
                            black_vertex.b = 0x00;
                        }
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            if(dwell_vertex_count < (int)p_space->lit_dwell_overhang)
                                angle_optimized.push_back(vertex);
                            else
                                angle_optimized.push_back(black_vertex);
                    }
                }
            }
        }
        angle_optimized.push_back(back());
        //--------------------------------------------------------------------
        if(angle_optimized.size() <= LASERBOY_MAX_USHORT)
            *this = angle_optimized;
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_lit_span_vertices()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  distance_optimized(p_space, palette_index, false);
        //--------------------------------------------------------------------
        for(i = 0; i < size() - 1; i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_lit()
                && vector_magnitude(i) > p_space->lit_delta_max
              )
                distance_optimized += LaserBoy_segment(p_space, at(i), at(i + 1));
        }
        distance_optimized.push_back(at(size() - 1));
        //--------------------------------------------------------------------
        if(distance_optimized.size() <= LASERBOY_MAX_USHORT)
            *this = distance_optimized;
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_blank_span_vertices()
{
    if(size() > 1)
    {
        u_int             i;
        LaserBoy_segment  distance_optimized(p_space, palette_index, false);
        //--------------------------------------------------------------------
        for(i = 0; i < size() - 1; i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_blank()
                && vector_magnitude(i) > p_space->blank_delta_max
              )
                distance_optimized += LaserBoy_segment(p_space, at(i), at(i + 1));
        }
        distance_optimized.push_back(at(size() - 1));
        //--------------------------------------------------------------------
        if(distance_optimized.size() <= LASERBOY_MAX_USHORT)
            *this = distance_optimized;
        else
            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return segment_error;
}

//############################################################################
void LaserBoy_segment::flip(u_int plane)
{
    if(size() > 1)
    {
        u_int i;
        switch(plane)
        {
           case 0: // X
                   for(i = 0; i < size(); i++)
                       at(i).x = -at(i).x;
                   break;
           //-----------------------------------------------------------------
           case 1: // Y
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
void LaserBoy_segment::quarter_turn(u_int plane, u_int turns)
{
    if(size() > 1)
    {
        u_int   i, j;
        short   temp;
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
void LaserBoy_segment::z_order_vertices(unsigned short span)
{
    if(size() > 1)
    {
        u_int i,
              step = span / size();
        for(i = 0; i < size(); i++)
            at(i).z = (short)((i * step) - (span / 2));
    }
    return;
}

//############################################################################
void LaserBoy_segment::flatten_z()
{
    if(size() > 1)
    {
        for(u_int i = 0; i < size(); i++)
            at(i).z = 0;
    }
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  center = mean_of_coordinates();

        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i), center, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates(at(i), center, a),
                                     (LaserBoy_color)at(i),
                                     at(i).k,
                                     at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_around_origin(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex(at(i), a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex(at(i), a),
                                     (LaserBoy_color)at(i),
                                     at(i).k,
                                     at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates(LaserBoy_3D_double p, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i), p, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates(at(i), p, a),
                                     (LaserBoy_color)at(i),
                                     at(i).k,
                                     at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_x(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_x(at(i), p, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates_x(at(i), p, a),
                                   (LaserBoy_color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_y(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_y(at(i), p, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates_y(at(i), p, a),
                                     (LaserBoy_color)at(i),
                                     at(i).k,
                                     at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_z(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;
        for(i = 0; i < size(); i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_z(at(i), p, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates_z(at(i), p, a),
                                     (LaserBoy_color)at(i),
                                     at(i).k,
                                     at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_around_origin()
{
    return rotate_around_origin(p_space->view_angle);
}

//############################################################################
void LaserBoy_segment::ripple(int direction, double amplitude, double freq, double phase)
{
    if(size() > 1)
    {
        u_int i;
        switch(direction)
        {
            case 0: for(i = 0; i < size(); i++) // parellel through x
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).x / 32768.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 1: for(i = 0; i < size(); i++) // parellel through y
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).y / 32768.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 2: for(i = 0; i < size(); i++) // x * y, the egg carton effect
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).x * at(i).y / 1073741824.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 3: for(i = 0; i < size(); i++) // circular, concentric to the origin
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(sqrt(double(at(i).x * at(i).x + at(i).y * at(i).y)) / 32768.0 * two_pi * freq + phase));
            //----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
LaserBoy_palette LaserBoy_segment::as_color_table() const
{
    LaserBoy_palette color_table(p_space);
    color_table.reserve(size());
    for(u_int i = 0; i < size(); i++)
        if(at(i).is_lit())
            color_table.push_back(at(i));
        else
            color_table.push_back(LaserBoy_color(0,0,0));
    return color_table;
}

//############################################################################
void LaserBoy_segment::strip_color()
{
    palette_index = LASERBOY_ILDA_DEFAULT;
    if(size() > 1)
        for(u_int i = 1; i < size(); i++)
        {
            if(at(i).k & LASERBOY_BLANKING_BIT)
            {
                at(i).c = 0;
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
            else if(at(i).is_black(p_space->black_level))
            {
                at(i).c = 64;
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
            else
            {
                at(i).c =  55; // LASERBOY_ILDA_DEFAULT white
                at(i).r = 255;
                at(i).g = 255;
                at(i).b = 255;
            }
        }
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    return;
}

//############################################################################
void LaserBoy_segment::strip_color_or()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR) // if it is alread palette
            set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
        for(u_int i = 1; i < size(); i++)
        {
            at(i).r =
            at(i).g =
            at(i).b = (at(i).r | at(i).g | at(i).b);
        }
        best_match_palette(LASERBOY_GRAYS);
    }
    return;
}

//############################################################################
void LaserBoy_segment::strip_color_avg()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR) // if it is alread palette
            set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
        for(u_int i = 1; i < size(); i++)
        {
            at(i).r =
            at(i).g =
            at(i).b = ((76 * at(i).r + 150 * at(i).g + 28 * at(i).b) / 254);
        }
        best_match_palette(LASERBOY_GRAYS);
    }
    return;
}

//############################################################################
void LaserBoy_segment::to_palette_by_index(u_int index)
{
    if(    (int)index != LASERBOY_TRUE_COLOR
        && (int)index != palette_index
        && index < p_space->number_of_palettes()
      )
    {
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::to_target_palette_by_index()
{
    if(    (int)p_space->target_palette_index != LASERBOY_TRUE_COLOR
        && (int)p_space->target_palette_index != palette_index
        && p_space->target_palette_index < p_space->number_of_palettes()
      )
    {
        palette_index = p_space->target_palette_index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::best_match_palette(u_int index)
{
    if(    (int)index != palette_index
        && index <  p_space->number_of_palettes()
      )
    {
        if(size() > 1)
        {
            if(!p_space->allow_lit_black)
                convert_black_to_blank();
            else
                impose_black_level();

            for(u_int i = 1; i < size(); i++)
                 at(i).c = p_space->palette_picker(index).best_match(at(i));
        }
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::best_match_target_palette()
{
    if(    (int)p_space->target_palette_index != palette_index
        && p_space->target_palette_index < p_space->number_of_palettes()
      )
    {
        if(size() > 1)
            for(u_int i = 1; i < size(); i++)
                 at(i).c = p_space->palette_picker(p_space->target_palette_index).best_match(at(i));
        palette_index = p_space->target_palette_index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::convert_black_to_blank()
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
//    while(back().is_blank())
//        pop_back();
    return;
}

//############################################################################
void LaserBoy_segment::convert_blank_to_black()
{
    for(u_int i = 1; i < size(); i++) // Leave the zero index blank!
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
void LaserBoy_segment::set_vertex_to_black(u_int index)
{
    if(size() && index < size())
    {
        at(index).r = 0;
        at(index).g = 0;
        at(index).b = 0;
        at(index).c = p_space->palette_picker(palette_index).black;
    }
    return;
}

//############################################################################
void LaserBoy_segment::impose_black_level()
{
    for(u_int i = 0; i < size(); i++)
    {
        if(at(i).as_LaserBoy_color().intensity() < p_space->black_level)
        {
            at(i).r = 0;
            at(i).g = 0;
            at(i).b = 0;
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::rainbow_recolor(int effect)
{
    if(size() > 1)
    {
        if(palette_index == LASERBOY_TRUE_COLOR)
            best_match_palette(p_space->target_palette_index);
        //--------------------------------------------------------------------
        u_int  i,
               j,
               span        =   p_space->palette_picker(palette_index).last
                             - p_space->palette_picker(palette_index).first,
               offset      =   p_space->palette_picker(palette_index).first;
        u_char color_index =   p_space->palette_picker(palette_index).white;
        //--------------------------------------------------------------------
        if(effect > 10) effect =  0;
        if(effect <  0) effect = 10;
        //--------------------------------------------------------------------
        switch(effect)
        {
            case 0: for(i = 1; i < size(); i++) // span through x
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)
                                        (   (at(i).x + 32767)
                                          * p_space->recolor_span_factor
                                          / 65536.0
                                          * span
                                        )
                                      % span
                                      + offset;
                    break;
           //-----------------------------------------------------------------
            case 1: for(i = 1; i < size(); i++) // span through y
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)
                                        (   (at(i).y + 32767)
                                          * p_space->recolor_span_factor
                                          / 65536.0
                                          * span
                                        )
                                      % span
                                      + offset;
                    break;
           //-----------------------------------------------------------------
            case 2: for(i = 1; i < size(); i++) // span through z
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)
                                        (   (at(i).z + 32767)
                                          * p_space->recolor_span_factor
                                          / 65536.0
                                          * span
                                        )
                                      % span
                                      + offset;
                    break;
           //-----------------------------------------------------------------
            case 3: for(i = 1; i < size(); i++) // radially from origin
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)
                                        (   (at(i).magnitude())
                                          * p_space->recolor_span_factor
                                          / 32767.0
                                          * span
                                        )
                                      % span
                                      + offset;
                    break;
           //-----------------------------------------------------------------
            case 4: for(i = 1; i < size(); i++) // angular around origin
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)
                                        (   atan2(at(i).y, at(i).x)
                                          * p_space->recolor_span_factor
                                          / two_pi
                                          * span
                                        )
                                      % span
                                      + offset;
                    break;
           //-----------------------------------------------------------------
            case 5: for(i = 1; i < size(); i++) // span through vectors
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)((i * p_space->recolor_span_factor / (double)size()) * span) % span + offset;
                    break;
           //-----------------------------------------------------------------
            case 6: for(i = 1; i < size(); i++) // index through vectors
                        if(at(i).is_color(p_space->black_level))
                            at(i).c = (u_char)(i % span + offset);
                    break;
           //-----------------------------------------------------------------
            case 7: // index through segments
                    {
                        if(number_of_segments() > 1)
                        {
                            LaserBoy_frame_set segments = explode_segments();
                            for(i = 0; i < number_of_segments(); i++)
                            {
                                color_index = u_char(((i * p_space->recolor_span_factor) / number_of_segments()) * span) % span + offset;
                                for(j = 1; j < segments[i].size(); j++)
                                    if(segments[i].at(j).is_color(p_space->black_level))
                                        segments[i].at(j).c = color_index;
                            }
                            *this = segments.sum_of_frames();
                        }
                        else
                        {
                            for(j = 1; j < size(); j++)
                                if(at(j).is_color(p_space->black_level))
                                    at(j).c = color_index;
                        }
                    }
                    break;
           //-----------------------------------------------------------------
            case 8: // span through segments
                    {
                        if(number_of_segments() > 1)
                        {
                            LaserBoy_frame_set segments = explode_segments();
                            for(i = 0; i < segments.number_of_frames(); i++)
                            {
                                color_index = (i % span) + offset;
                                for(j = 1; j < segments[i].size(); j++)
                                    if(segments[i].at(j).is_color(p_space->black_level))
                                        segments[i].at(j).c = color_index;
                            }
                           *this = segments.sum_of_frames();
                        }
                        else
                        {
                            for(j = 1; j < size(); j++)
                                if(at(j).is_color(p_space->black_level))
                                    at(j).c = color_index;
                        }
                    }
                    break;
           //-----------------------------------------------------------------
            case 9: // random color vectors
                    {
                        for(i = 1; i < size(); i++)
                            if(at(i).is_color(p_space->black_level))
                                at(i).c = (rand() % span) + offset;
                    }
                    break;
           //-----------------------------------------------------------------
            case 10: // random color segments
                    {
                        if(number_of_segments() > 1)
                        {
                            LaserBoy_frame_set segments = explode_segments();
                            for(i = 0; i < segments.number_of_frames(); i++)
                            {
                                color_index = (rand() % span) + offset;
                                for(j = 1; j < segments[i].size(); j++)
                                    if(segments[i].at(j).is_color(p_space->black_level))
                                        segments[i].at(j).c = color_index;
                            }
                            *this = segments.sum_of_frames();
                        }
                        else
                        {
                            color_index = (rand() % span) + offset;
                            for(j = 1; j < size(); j++)
                                if(at(j).is_color(p_space->black_level))
                                    at(j).c = color_index;
                        }
                    }
                    break;
           //-----------------------------------------------------------------
        }
        set_rgb_from_palette();
    }
    return;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::rotate_colors(int steps)
{
    if(    palette_index != LASERBOY_TRUE_COLOR
        && size() > 1
      )
    {
        int offset =   p_space->palette_picker(palette_index).first,

            span   =   p_space->palette_picker(palette_index).last
                     - p_space->palette_picker(palette_index).first;
        //--------------------------------------------------------------------
        for(u_int i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = ((at(i).c - offset + steps) % span) + offset;
        set_rgb_from_palette();
    }
    return *this;
}

//############################################################################
bool LaserBoy_segment::find_rgb_in_palette(const LaserBoy_palette& palette)
{
    u_int         i,
                  j;
    vector<bool>  match(size(), false);
    bool          all_colors_found = true;
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
        for(j = 0; j < palette.number_of_colors(); j++)
            if((LaserBoy_color)at(i) == palette.at(j))
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
void LaserBoy_segment::set_rgb_from_palette()
{
    for(u_int i = 0; i < size(); i++)
    {
        at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
        at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
        at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
    }
    return;
}

//############################################################################
void LaserBoy_segment::set_palette_to_332()
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
void LaserBoy_segment::sync_rgb_and_palette()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR) // if it is alread palette
            set_rgb_from_palette();        // set r, g, b values according to palette
        //--------------------------------------------------------------------
        else
        {
            u_int             i               ,
                              j               ;
            LaserBoy_palette  palette(p_space);
            //----------------------------------------------------------------
            palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
            //----------------------------------------------------------------
            for(i = first_lit_vector_index() + 1; i < size(); i++)
            {
                for(j = 0; j < palette.number_of_colors(); j++)
                    if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                        break;
                if(    j == palette.number_of_colors()
                    && at(i).is_lit()
                  )
                    palette.push_back((LaserBoy_color)at(i));
                if(palette.number_of_colors() > LASERBOY_PALETTE_MAX)
                    break;
            }
            //----------------------------------------------------------------
            if(palette.number_of_colors() <= LASERBOY_PALETTE_MAX) // it's a palette
            {
                palette.reorder();
                palette.find_factors();
                find_rgb_in_palette(palette);
                palette.name = p_space->GUID8char();
                p_space->push_back_palette(palette);
                palette_index = p_space->number_of_palettes() - 1;
            }
            //----------------------------------------------------------------
            else // (palette.number_of_colors() > LASERBOY_PALETTE_MAX) // it is true color
                set_palette_to_332(); // palette_index is still LASERBOY_TRUE_COLOR
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_segment::bit_reduce_to_palette()
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
void LaserBoy_segment::best_reduce_to_palette()
{
    if(    number_of_color_vectors() >= 1
//        && palette_index == LASERBOY_TRUE_COLOR
      )
    {
        u_int             i               ,
                          j               ;
        LaserBoy_palette  palette(p_space);
        //--------------------------------------------------------------------
        if(!p_space->allow_lit_black)
            convert_black_to_blank();
        else
            impose_black_level();
        //--------------------------------------------------------------------
        palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
        //--------------------------------------------------------------------
        for(i = first_lit_vector_index() + 1; i < size(); i++)
        {
            for(j = 0; j < palette.number_of_colors(); j++)
                if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                    break;
            if(    j == palette.number_of_colors()
                && at(i).is_lit()
              )
                palette.push_back((LaserBoy_color)at(i));
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
void LaserBoy_segment::promote_to_true_color()
{
    if(palette_index != LASERBOY_TRUE_COLOR)
    {
        set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
    }
    return;
}

//############################################################################
void LaserBoy_segment::shade(u_char shade) // 0 shade is no change 255 is black
{
    if(shade)
    {
        sync_rgb_and_palette();
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            LaserBoy_palette  palette(p_space->palette_picker(palette_index));
            //----------------------------------------------------------------
            palette.shade(shade);
            //----------------------------------------------------------------
            palette.name = p_space->GUID8char();
            p_space->push_back_palette(palette);
            //----------------------------------------------------------------
            palette_index = p_space->number_of_palettes() - 1;
            set_rgb_from_palette();
        }
        else
        {
            for(u_int i = 1; i < size(); i++)
            {
                (at(i).r - shade > 0) ? (at(i).r -= shade) : (at(i).r = 0);
                (at(i).g - shade > 0) ? (at(i).g -= shade) : (at(i).g = 0);
                (at(i).b - shade > 0) ? (at(i).b -= shade) : (at(i).b = 0);
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::tint(u_char tint) // 0 tint is no change 255 is white
{
    if(tint)
    {
        sync_rgb_and_palette();
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            LaserBoy_palette   palette(p_space->palette_picker(palette_index));
            //----------------------------------------------------------------
            palette.tint(tint);
            //----------------------------------------------------------------
            palette.name = p_space->GUID8char();
            p_space->push_back_palette(palette);
            //----------------------------------------------------------------
            palette_index = p_space->number_of_palettes() - 1;
            set_rgb_from_palette();
        }
        else
        {
            for(u_int i = 1; i < size(); i++)
            {
                (at(i).r + tint < 255) ? (at(i).r += tint) : (at(i).r = 255);
                (at(i).g + tint < 255) ? (at(i).g += tint) : (at(i).g = 255);
                (at(i).b + tint < 255) ? (at(i).b += tint) : (at(i).b = 255);
            }
        }
    }
    return;
}

//############################################################################
bool LaserBoy_segment::color_from_bmp(const string& file)
{
    char file_name[81];
    struct LaserBoy_bmp bmp;

    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        color_from_bmp(&bmp);
        bmp_free(&bmp);
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_segment::color_from_bmp(struct LaserBoy_bmp* bmp)
{
    if(size() > 1)
    {
        u_int             i,
                          color,
                          factor = (65536 / bmp->xres);

        LaserBoy_segment  line;
        LaserBoy_vertex   vertex;
        //--------------------------------------------------------------------
        if(bmp->bpp <= 8)
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                color = bmp->get_pixel(  bmp,
                                         (vertex.x + 32767) / factor,
                                         (vertex.y + 32767) / factor
                                      );
                at(i).r = bmp_get_palette_index_r(bmp, color);
                at(i).g = bmp_get_palette_index_g(bmp, color);
                at(i).b = bmp_get_palette_index_b(bmp, color);
            }
        //--------------------------------------------------------------------
        else
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                color = bmp->get_pixel(  bmp,
                                         (vertex.x + 32767) / factor,
                                         (vertex.y + 32767) / factor
                                      );
                at(i).r = bmp->r_from_rgb(color);
                at(i).g = bmp->g_from_rgb(color);
                at(i).b = bmp->b_from_rgb(color);
            }
        //--------------------------------------------------------------------
        palette_index = LASERBOY_TRUE_COLOR;
        sync_rgb_and_palette();
    }
    return;
}

//############################################################################
bool LaserBoy_segment::subtract_bmp(const string& file)
{
    char file_name[81];
    struct LaserBoy_bmp bmp;

    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        subtract_bmp(&bmp);
        bmp_free(&bmp);
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_segment::subtract_bmp(struct LaserBoy_bmp* bmp)
{
    if(size() > 1)
    {
        u_char            mask;
        u_int             i,
                          pixle_color,
                          factor = (65536 / bmp->xres);
        LaserBoy_segment  line;
        LaserBoy_vertex   vertex;
        //--------------------------------------------------------------------
        if(bmp->bpp <= 8)
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();

                pixle_color = bmp->get_pixel(  bmp,
                                               (vertex.x + 32767) / factor,
                                               (vertex.y + 32767) / factor
                                            );

                mask = 255 - bmp_get_palette_index_r(bmp, pixle_color);
                (at(i).r - mask > 0) ? (at(i).r -= mask) : (at(i).r = 0);

                mask = 255 - bmp_get_palette_index_g(bmp, pixle_color);
                (at(i).g - mask > 0) ? (at(i).g -= mask) : (at(i).g = 0);

                mask = 255 - bmp_get_palette_index_b(bmp, pixle_color);
                (at(i).b - mask > 0) ? (at(i).b -= mask) : (at(i).b = 0);
            }
        //--------------------------------------------------------------------
        else
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();

                pixle_color = bmp->get_pixel(  bmp,
                                               (vertex.x + 32767) / factor,
                                               (vertex.y + 32767) / factor
                                            );

                mask = 255 - bmp->r_from_rgb(pixle_color);
                (at(i).r - mask > 0) ? (at(i).r -= mask) : (at(i).r = 0);

                mask = 255 - bmp->g_from_rgb(pixle_color);
                (at(i).g - mask > 0) ? (at(i).g -= mask) : (at(i).g = 0);

                mask = 255 - bmp->b_from_rgb(pixle_color);
                (at(i).b - mask > 0) ? (at(i).b -= mask) : (at(i).b = 0);
            }
        //--------------------------------------------------------------------
        palette_index = LASERBOY_TRUE_COLOR;
        sync_rgb_and_palette();
    }
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move(LaserBoy_3D_double d, bool check_bounds)
{
    if(size() > 1)
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        //--------------------------------------------------------------------
        if(check_bounds)
        {
            for(i = 0; i < size(); i++)
            {                                             // double + double
                if((out_of_bounds = LaserBoy_bounds_check(d + (LaserBoy_3D_double)at(i).as_3D_short(), LASERBOY_CUBE)))
                    return out_of_bounds;
            }
            for(i = 0; i < size(); i++)
                at(i) = LaserBoy_vertex(d + at(i).as_3D_short(), // double + short
                                        at(i).as_LaserBoy_color(),
                                        at(i).k,
                                        at(i).c
                                       );
        }
        else
            for(i = 0; i < size(); i++)
                at(i) = LaserBoy_vertex(at(i).as_3D_short() + d, // short + double
                                        at(i).as_LaserBoy_color(),
                                        at(i).k,
                                        at(i).c
                                       );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move()
{
    return move(p_space->view_offset);
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale(LaserBoy_3D_double s)
{
    LaserBoy_3D_double center = mean_of_coordinates();
    return scale_on_coordinates(center, s);
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_on_coordinates(LaserBoy_3D_double p, LaserBoy_3D_double s)
{
    if(size() > 1)
    {
        u_int            i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        for(i = 0; i < size(); i++)
            if((out_of_bounds |= LaserBoy_bounds_check((((LaserBoy_3D_double)(at(i).as_3D_short()) - p) * s) + p, LASERBOY_CUBE)))
                return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) = LaserBoy_vertex((((LaserBoy_3D_double)(at(i).as_3D_short()) - p) * s) + p,
                                    at(i).as_LaserBoy_color(),
                                    at(i).k,
                                    at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_around_origin(LaserBoy_3D_double f)
{
    if(size() > 1)
    {
        LaserBoy_segment  segment(*this);
        LaserBoy_Bounds   out_of_bounds = LASERBOY_IN_BOUNDS;
        u_int             i,
                          segment_index;
        for(segment_index = 0; segment_index < segment.number_of_segments(); segment_index++)
            out_of_bounds |= segment.scale_segment_around_origin(segment_index, f);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            at(i) *= f;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_around_origin()
{
    return scale_around_origin(p_space->view_scale);
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::rectangular_center_of() const
{
    LaserBoy_3D_double center;
    if(size() > 1)
    {
        int            max_x = -40000, // more negative than any short
                       min_x =  40000, // more positive than any short
                       max_y = -40000,
                       min_y =  40000,
                       max_z = -40000,
                       min_z =  40000;

        for(u_int i = 0; i < size(); i++)
        {
            if(at(i).x > max_x)    max_x = at(i).x;
            if(at(i).x < min_x)    min_x = at(i).x;
            if(at(i).y > max_y)    max_y = at(i).y;
            if(at(i).y < min_y)    min_y = at(i).y;
            if(at(i).z > max_z)    max_z = at(i).z;
            if(at(i).z < min_z)    min_z = at(i).z;
        }
        center.x = max_x - ((max_x - min_x) / 2);
        center.y = max_y - ((max_y - min_y) / 2);
        center.z = max_z - ((max_z - min_z) / 2);
    }
    return center;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::mean_of_coordinates() const
{
    LaserBoy_3D_double mean;
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
u_int LaserBoy_segment::number_of_segments() const // a segment is a series of lit vertices
{
    u_int i,
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
LaserBoy_segment LaserBoy_segment::blend(const LaserBoy_segment& segment, double ratio)
{
    if(ratio == 0.0)
        return *this;
    else if(ratio == 1.0)
        return segment;
    else if(    size() > 1
             && segment.size() > 1
           )
    {
        u_int            i,
                         diff;
        LaserBoy_segment this_one(*this),
                         other_one(segment),
                         combo(p_space);
                         
        this_one.convert_blank_to_black();
        other_one.convert_blank_to_black();
        combo.palette_index = LASERBOY_TRUE_COLOR;
        if(this_one.size() > other_one.size())
            while(this_one.size() / other_one.size() > 1)
                other_one += other_one;
        else
            while(other_one.size() / this_one.size() > 1)
                this_one += this_one;
        if(this_one.size() > other_one.size())
        {
            diff = (this_one.size() - other_one.size());
            for(i = 0; i < diff; i++)
                other_one += other_one[i];
        }
        else if(this_one.size() < other_one.size())
        {
            diff = (other_one.size() - this_one.size());
            for(i = 0; i < diff; i++)
                this_one += this_one[i];
        }
        combo.reserve(this_one.size());
        for(i = 0; i < this_one.size(); i++)
            combo += this_one.at(i).blend(other_one.at(i), ratio);
        return combo;
    }
    else
        return *this;
}

//############################################################################
bool LaserBoy_segment::find_segment_at_index(u_int segment_index, u_int& start, u_int& end) const
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
                    end = i;
                //------------------------------------------------------------
                segment_count++;
                if(segment_count == (int)segment_index)
                    return true;
            }
        } // segment_index out of range
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
bool LaserBoy_segment::find_segment_of_vertex(u_int vertex_index, u_int& start, u_int& end, u_int& segment_index) const
{
    start = end = segment_index = 0;
    if(size() > 1)
    {
        u_int  i,
               segment_count = 0; // the first segment is number zero!
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                end = i - 1;

                if(    vertex_index >= start
                    && vertex_index <= end
                  )
                {
                    segment_index = segment_count;
                    return true;
                }

                if(vertex_index < start)
                {
                    segment_index = segment_count;
                    return false;
                }
                segment_count++;
            }
        }
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
LaserBoy_segment LaserBoy_segment::copy_segment(u_int segment_index) const
{
    if(size() > 1)
    {
        u_int             start,
                          end;
        LaserBoy_segment  segment(p_space, palette_index, false);
        find_segment_at_index(segment_index, start, end);
        segment.reserve(end - start + 1);
        for(u_int i = start; i <= end; i++)
            segment.push_back(at(i));
        return segment;
    }
    return blank_segment(p_space);
}

//############################################################################
LaserBoy_frame_set LaserBoy_segment::explode_segments() const
{
    LaserBoy_frame_set segments(p_space);
    //------------------------------------------------------------------------
    if(size() > 1)
    {
        u_int           i,
                        j,
                        start,
                        end;
        LaserBoy_frame  frame(p_space, palette_index, false);
        //--------------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1; // anchor to the first lit vertex
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                frame.clear();
                frame.reserve(end - start + 1);
                for(j = start; j <= end; j++)
                    frame += at(j);
                segments.push_back(frame);
            }
        }
    }
    //------------------------------------------------------------------------
    return segments;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::rectangular_center_of_segment(u_int segment_index) const
{
    return (copy_segment(segment_index)).rectangular_center_of();
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::mean_of_coordinates_of_segment(u_int segment_index) const
{
    return (copy_segment(segment_index)).mean_of_coordinates();
}

//############################################################################
u_int LaserBoy_segment::segment_index_of_vertex(u_int vertex_index) const
{
    u_int start, end, segment_index;
    find_segment_of_vertex(vertex_index, start, end, segment_index);
    return segment_index;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move_segment(u_int segment_index, LaserBoy_3D_double f)
{
    if(size() > 1)
    {
        u_int            i,
                         start,
                         end;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;
        find_segment_at_index(segment_index, start, end);
        for(i = start; i <= end; i++)
            out_of_bounds |= LaserBoy_bounds_check(f + at(i), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) += f;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_segment(u_int segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int              i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short  center;

        find_segment_at_index(segment_index, start, end);
        center = mean_of_coordinates_of_segment(segment_index);

        for(i = start; i <= end; i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i), center, a), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = LaserBoy_vertex( rotate_vertex_on_coordinates(at(i), center, a),
                                   (LaserBoy_color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_segment_around_origin(u_int segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        u_int            i,
                         start,
                         end;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        find_segment_at_index(segment_index, start, end);
        for(i = start; i <= end; i++)
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex(at(i), a), LASERBOY_SPHERE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = LaserBoy_vertex( rotate_vertex(at(i), a),
                                   (LaserBoy_color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_segment(u_int segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        u_int              i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short  center;

        find_segment_at_index(segment_index, start, end);
        center = mean_of_coordinates_of_segment(segment_index);

        for(i = start; i <= end; i++)
            out_of_bounds |= LaserBoy_bounds_check(scale_vertex_on_coordinates(at(i), center, m), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = LaserBoy_vertex(scale_vertex_on_coordinates(at(i), center, m),
                                    (LaserBoy_color)at(i),
                                    at(i).k,
                                    at(i).c
                                   );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_segment_around_origin(u_int segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        u_int            i,
                         start,
                         end;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;

        find_segment_at_index(segment_index, start, end);
        for(i = start; i <= end; i++)
            out_of_bounds |= LaserBoy_bounds_check(m * at(i), LASERBOY_CUBE);
        //--------------------------------------------------------------------
        if(out_of_bounds)
            return out_of_bounds;
        //--------------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = LaserBoy_vertex( m * at(i),
                                   (LaserBoy_color)at(i),
                                   at(i).k,
                                   at(i).c
                                 );
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
void LaserBoy_segment::to_fstream_wave(fstream&              out,
                                       LaserBoy_wave_header& header,
                                       bool                  end_of_frame,
                                       bool                  unique_frame
                                      )
{
    if(size() > 1)
    {
        u_int i;
        if(p_space->invert_wave_output)
        {
            for(i = 0; i < size() - 1; i++)
                at(i).to_fstream_wave_inverted(out,
                                               header,
                                               p_space->signal_bit_mask,
                                               false,
                                               false
                                              );
            at(i).to_fstream_wave_inverted(out,
                                           header,
                                           p_space->signal_bit_mask,
                                           end_of_frame,
                                           unique_frame
                                          );
        }
        else
        {
            for(i = 0; i < size() - 1; i++)
                at(i).to_fstream_wave(out,
                                      header,
                                      p_space->signal_bit_mask,
                                      false,
                                      false
                                     );
            at(i).to_fstream_wave(out,
                                  header,
                                  p_space->signal_bit_mask,
                                  end_of_frame,
                                  unique_frame
                                 );
        }
        if(end_of_frame)
            header.num_frames++;
    }
    return;
}

//############################################################################






//############################################################################
LaserBoy_segment blank_segment(LaserBoy_space* ps)
{
    LaserBoy_segment segment(ps, LASERBOY_ILDA_DEFAULT, false);
    segment.reserve(2);
    segment.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
    return segment;
}

//############################################################################
LaserBoy_segment NULL_segment(LaserBoy_space* ps) // vector word NULL
{
    LaserBoy_segment segment(ps, LASERBOY_ILDA_DEFAULT, false);
    segment.reserve(17);
    segment.push_back(LaserBoy_vertex(-19976, -8000, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex(-19976, 10000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( -9976, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( -9976, 10000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( -7976, 10000, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex( -7976, -6000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( -5976, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( -1976, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex(    24, -6000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex(    24, 10000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex(  2024, 10000, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex(  2024, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( 10024, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( 12024, 10000, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex( 12024, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex( 20024, -8000, 0, 255, 255, 255,  0, 55));
    segment.push_back(LaserBoy_vertex(-19976, -8000, 0, 255, 255, 255, 64, 55));
    return segment;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
