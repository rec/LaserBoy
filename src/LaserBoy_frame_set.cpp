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
// LaserBoy_frame_set.cpp is part of LaserBoy.
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
LaserBoy_frame_set::LaserBoy_frame_set(LaserBoy_space* ps,
                                       LaserBoy_vertex(*F)(int, int),
                                       int vertices_per_frame,
                                       int num_frames
                                      )
    : p_space            (ps)
    , frame_set_error    (LASERBOY_OK)
    , num_2D_frames      ( 0)
    , num_3D_frames      ( 0)
    , frame_index        ( 0)
    , frame_select_start (-1)
    , frame_select_end   (-1)
    , null_frame         (ps)
    , ild_file_GUID      (p_space->install_GUID)
{
    null_frame = NULL_frame(ps);
    reserve(num_frames);
    for(int i = 0; i < num_frames; i++)
    {
        push_back(LaserBoy_frame(p_space, F, vertices_per_frame, i));
        if(back().is_2D())
            num_2D_frames++;
        else
            num_3D_frames++;
    }
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::operator += (const LaserBoy_frame& frame)
{
    if(p_space->show_effects_generation)
    {
        bmp_copy(&(p_space->bmp), &(p_space->background));
        frame.render(0);
        p_space->p_GUI->display_space();
    }
    push_back(frame);
    return *this;
}

//############################################################################
void LaserBoy_frame_set::render_frame(const int& index) const
{
    bool     rotate_view = (p_space->view_angle  != 0.0),
             offset_view = (p_space->view_offset != 0  ),
             scale_view  = (p_space->view_scale  != 1.0);

    int      bmp_center = (   (p_space->show_quad_view)
                            ? (p_space->bmp.yres / 4)
                            : (p_space->bmp.yres / 2)
                          ),

             bmp_3X_center = bmp_center * 3,

             bigger =   (   ((p_space->show_bridge) ? (at(index).bridge.size()) : (0))
                          > ((p_space->show_coda)   ? (at(index).coda.size())   : (0))
                        )
                      ? ((p_space->show_bridge) ? (at(index).bridge.size()) : (0))
                      : ((p_space->show_coda)   ? (at(index).coda.size())   : (0));

    u_int    bmp_bounds  ,
             bmp_blank   ,
             egg_color   ,
             spider_color,
             bmp_bg_color = p_space->bmp.make_rgb(p_space->bg_color.r,
                                                  p_space->bg_color.g,
                                                  p_space->bg_color.b
                                                 );

    double   deep = LASERBOY_SHORT_SPAN,
             half_deep = deep / 2.0,

             scale = (   (p_space->show_quad_view)
                       ? (p_space->bmp.yres / (deep * 2))
                       : (p_space->bmp.yres / deep)
                     ),

             z_depth_scale =   deep
                             / (   at(index).size()
                                 + ((p_space->show_intro) ? (at(index).intro.size()) : (0))
                                 + bigger
                               ),

             z_depth_body_offset =   half_deep
                                   - (   z_depth_scale
                                       * ((p_space->show_intro) ? (at(index).intro.size()) : (0))
                                     );

    LaserBoy_3D_double _0,
                       _1;

    LaserBoy_color     bit_masked;
    //------------------------------------------------------------------------
    bmp_bounds = p_space->bmp.make_rgb(p_space->rendered_bounds.r,
                                       p_space->rendered_bounds.g,
                                       p_space->rendered_bounds.b
                                      );
    bmp_blank  = p_space->bmp.make_rgb(p_space->rendered_blank.r,
                                       p_space->rendered_blank.g,
                                       p_space->rendered_blank.b
                                      );
    //------------------------------------------------------------------------
//    p_space->clean_screen();
    //------------------------------------------------------------------------
    if(p_space->show_fixed_bounds)
    {
        if(p_space->show_quad_view)
        {
            bmp_put_rectangle(&(p_space->bmp),
                              0,
                              0,
                              p_space->bmp.yres / 2 - 1,
                              p_space->bmp.yres / 2 - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(&(p_space->bmp),
                              p_space->bmp.yres / 2,
                              0,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres / 2 - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(&(p_space->bmp),
                              0,
                              p_space->bmp.yres / 2,
                              p_space->bmp.yres / 2 - 1,
                              p_space->bmp.yres - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(&(p_space->bmp),
                              p_space->bmp.yres / 2,
                              p_space->bmp.yres / 2,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres - 1,
                              bmp_bounds,
                              0
                             );
        } // end if(p_space->show_quad_view)
        else
            bmp_put_rectangle(&(p_space->bmp),
                              0,
                              0,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres - 1,
                              bmp_bounds,
                              0
                             );
    } // end if(p_space->show_fixed_bounds)
    //------------------------------------------------------------------------
    if(p_space->show_fixed_origin)
    {
        if(p_space->show_quad_view)
        {
            bmp_put_line_fast(&(p_space->bmp),
                              0,
                              p_space->bmp.yres / 4,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres / 4,
                              bmp_bounds
                             );
            bmp_put_line_fast(&(p_space->bmp),
                              0,
                              p_space->bmp.yres / 4 * 3,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres / 4 * 3,
                              bmp_bounds
                             );
            bmp_put_line_fast(&(p_space->bmp),
                              p_space->bmp.yres / 4,
                              0,
                              p_space->bmp.yres / 4,
                              p_space->bmp.yres - 1,
                              bmp_bounds
                             );
            bmp_put_line_fast(&(p_space->bmp),
                              p_space->bmp.yres / 4 * 3,
                              0,
                              p_space->bmp.yres / 4 * 3,
                              p_space->bmp.yres - 1,
                              bmp_bounds
                             );
        }
        else
        {
            bmp_put_line_fast(&(p_space->bmp),
                              0,
                              p_space->bmp.yres / 2,
                              p_space->bmp.yres - 1,
                              p_space->bmp.yres / 2,
                              bmp_bounds
                             );
            bmp_put_line_fast(&(p_space->bmp),
                              p_space->bmp.yres / 2,
                              0,
                              p_space->bmp.yres / 2,
                              p_space->bmp.yres - 1,
                              bmp_bounds
                             );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_floating_axis)
    {
        LaserBoy_3D_double  x1, x2, y1, y2, z1, z2;

        x1.x = LASERBOY_MIN_SHORT;
        x2.x = LASERBOY_MAX_SHORT;
        y1.y = LASERBOY_MIN_SHORT;
        y2.y = LASERBOY_MAX_SHORT;
        z1.z = LASERBOY_MIN_SHORT;
        z2.z = LASERBOY_MAX_SHORT;

        if(rotate_view)
        {
            x1 = rotate_vertex(x1, p_space->view_angle);
            x2 = rotate_vertex(x2, p_space->view_angle);
            y1 = rotate_vertex(y1, p_space->view_angle);
            y2 = rotate_vertex(y2, p_space->view_angle);
            z1 = rotate_vertex(z1, p_space->view_angle);
            z2 = rotate_vertex(z2, p_space->view_angle);
        }
        if(offset_view)
        {
            x1 += p_space->view_offset;
            x2 += p_space->view_offset;
            y1 += p_space->view_offset;
            y2 += p_space->view_offset;
            z1 += p_space->view_offset;
            z2 += p_space->view_offset;
        }
        if(scale_view)
        {
            x1 *= p_space->view_scale;
            x2 *= p_space->view_scale;
            y1 *= p_space->view_scale;
            y2 *= p_space->view_scale;
            z1 *= p_space->view_scale;
            z2 *= p_space->view_scale;
        }
        // X axis
        bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(x1.y * scale) + bmp_center)
                              ,                      (int)(x2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(x2.y * scale) + bmp_center)
                              , bmp_bounds
                         );
        // X axis icon
        bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((x2.y - 500) * scale) + bmp_center)
                              ,                      (int)((x2.x + 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((x2.y + 500) * scale) + bmp_center)
                              , p_space->bmp.make_rgb(255,0,0)
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((x2.y + 500) * scale) + bmp_center)
                              ,                      (int)((x2.x + 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((x2.y - 500) * scale) + bmp_center)
                              , p_space->bmp.make_rgb(255,0,0)
                         );
        // Y axis
        bmp_put_line_fast(&(p_space->bmp),           (int)(y1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_center)
                              ,                      (int)(y2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_center)
                              , bmp_bounds
                         );
        // Y axis icon
        bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x - 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_center)
                              ,                      (int)((y2.x      ) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                              , p_space->bmp.make_rgb(0,255,0)
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x + 500) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_center)
                              ,                      (int)((y2.x      ) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                              , p_space->bmp.make_rgb(0,255,0)
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x      ) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                              ,                      (int)((y2.x      ) * scale) + bmp_center
                              , p_space->bmp.yres - ((int)((y2.y - 500) * scale) + bmp_center)
                              , p_space->bmp.make_rgb(0,255,0)
                         );
        // Z axis
        bmp_put_line_fast(&(p_space->bmp),           (int)(z1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(z1.y * scale) + bmp_center)
                              ,                      (int)(z2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(z2.y * scale) + bmp_center)
                              , bmp_bounds
                         );
        if(p_space->show_Z_as_order)
        {
        // T axis icon
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(255,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x      ) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.x      ) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(255,255,0)
                             );
        }
        else
        {
        // Z axis icon
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  ,                      (int)((z2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
        }
        if(p_space->show_quad_view)
        {
            x1 = x2 = y1 = y2 = z1 = z2 = 0;
            x1.x = LASERBOY_MIN_SHORT;
            x2.x = LASERBOY_MAX_SHORT;
            y1.y = LASERBOY_MIN_SHORT;
            y2.y = LASERBOY_MAX_SHORT;
            z1.z = LASERBOY_MIN_SHORT;
            z2.z = LASERBOY_MAX_SHORT;

            if(offset_view)
            {
                x1 += p_space->view_offset;
                x2 += p_space->view_offset;
                y1 += p_space->view_offset;
                y2 += p_space->view_offset;
                z1 += p_space->view_offset;
                z2 += p_space->view_offset;
            }
            if(scale_view)
            {
                x1 *= p_space->view_scale;
                x2 *= p_space->view_scale;
                y1 *= p_space->view_scale;
                y2 *= p_space->view_scale;
                z1 *= p_space->view_scale;
                z2 *= p_space->view_scale;
            }
            // X axis front
            bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(x1.y * scale) + bmp_3X_center)
                                  ,                      (int)(x2.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(x2.y * scale) + bmp_3X_center)
                                  , bmp_bounds
                             );
            // X axis icon front
            bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((x2.y - 500) * scale) + bmp_3X_center)
                                  ,                      (int)((x2.x + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((x2.y + 500) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((x2.y + 500) * scale) + bmp_3X_center)
                                  ,                      (int)((x2.x + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((x2.y - 500) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            // Y axis front
            bmp_put_line_fast(&(p_space->bmp),           (int)(y1.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_3X_center)
                                  ,                      (int)(y2.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_3X_center)
                                  , bmp_bounds
                             );
            // Y axis icon front
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_3X_center)
                                  ,                      (int)((y2.x      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_3X_center)
                                  ,                      (int)((y2.x      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.x      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                  ,                      (int)((y2.x      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y - 500) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            // X axis top
            bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(x1.z * scale) + bmp_3X_center)
                                  ,                      (int)(x2.x * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(x2.z * scale) + bmp_3X_center)
                                  , bmp_bounds
                             );
            // X axis icon top
            bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((x2.z - 500) * scale) + bmp_3X_center)
                                  ,                      (int)((x2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((x2.z + 500) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((x2.x - 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((x2.z + 500) * scale) + bmp_3X_center)
                                  ,                      (int)((x2.x + 500) * scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((x2.z - 500) * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            // Z axis top
            bmp_put_line_fast(&(p_space->bmp),           (int)(z1.x * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(z1.z * -scale) + bmp_3X_center)
                                  ,                      (int)(z2.x * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(z2.z * -scale) + bmp_3X_center)
                                  , bmp_bounds
                             );
            // Z axis icon top
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x - 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                  ,                      (int)((z2.x + 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x + 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                  ,                      (int)((z2.x - 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.x - 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                  ,                      (int)((z2.x + 500) * -scale) + bmp_center
                                  , p_space->bmp.yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            // Y axis side
            bmp_put_line_fast(&(p_space->bmp),           (int)(y1.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_center)
                                  ,                      (int)(y2.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_center)
                                  , bmp_bounds
                             );
            // Y axis icon side
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.z - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((y2.z      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.z + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((y2.z      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((y2.z      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y      ) * scale) + bmp_center)
                                  ,                      (int)((y2.z      ) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((y2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            // Z axis side
            bmp_put_line_fast(&(p_space->bmp),           (int)(z1.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(z1.y * scale) + bmp_center)
                                  ,                      (int)(z2.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(z2.y * scale) + bmp_center)
                                  , bmp_bounds
                             );
            // Z axis icon side
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.z - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.z + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.z + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                  ,                      (int)((z2.z - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)((z2.z - 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  ,                      (int)((z2.z + 500) * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_floating_bounds)
    {
        LaserBoy_3D_double  a(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            b(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            c(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            d(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            e(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            f(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            g(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT),
                            h(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT);

        if(rotate_view)
        {
            a = rotate_vertex(a, p_space->view_angle);
            b = rotate_vertex(b, p_space->view_angle);
            c = rotate_vertex(c, p_space->view_angle);
            d = rotate_vertex(d, p_space->view_angle);
            e = rotate_vertex(e, p_space->view_angle);
            f = rotate_vertex(f, p_space->view_angle);
            g = rotate_vertex(g, p_space->view_angle);
            h = rotate_vertex(h, p_space->view_angle);
        }
        if(offset_view)
        {
            a += p_space->view_offset;
            b += p_space->view_offset;
            c += p_space->view_offset;
            d += p_space->view_offset;
            e += p_space->view_offset;
            f += p_space->view_offset;
            g += p_space->view_offset;
            h += p_space->view_offset;
        }
        if(scale_view)
        {
            a *= p_space->view_scale;
            b *= p_space->view_scale;
            c *= p_space->view_scale;
            d *= p_space->view_scale;
            e *= p_space->view_scale;
            f *= p_space->view_scale;
            g *= p_space->view_scale;
            h *= p_space->view_scale;
        }

        a *= scale;
        b *= scale;
        c *= scale;
        d *= scale;
        e *= scale;
        f *= scale;
        g *= scale;
        h *= scale;

        bmp_put_line_fast(&(p_space->bmp),           (int)a.x + bmp_center
                              , p_space->bmp.yres - ((int)a.y + bmp_center)
                              ,                      (int)b.x + bmp_center
                              , p_space->bmp.yres - ((int)b.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)b.x + bmp_center
                              , p_space->bmp.yres - ((int)b.y + bmp_center)
                              ,                      (int)c.x + bmp_center
                              , p_space->bmp.yres - ((int)c.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)c.x + bmp_center
                              , p_space->bmp.yres - ((int)c.y + bmp_center)
                              ,                      (int)d.x + bmp_center
                              , p_space->bmp.yres - ((int)d.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)d.x + bmp_center
                              , p_space->bmp.yres - ((int)d.y + bmp_center)
                              ,                      (int)a.x + bmp_center
                              , p_space->bmp.yres - ((int)a.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)a.x + bmp_center
                              , p_space->bmp.yres - ((int)a.y + bmp_center)
                              ,                      (int)e.x + bmp_center
                              , p_space->bmp.yres - ((int)e.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)b.x + bmp_center
                              , p_space->bmp.yres - ((int)b.y + bmp_center)
                              ,                      (int)f.x + bmp_center
                              , p_space->bmp.yres - ((int)f.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)c.x + bmp_center
                              , p_space->bmp.yres - ((int)c.y + bmp_center)
                              ,                      (int)g.x + bmp_center
                              , p_space->bmp.yres - ((int)g.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)d.x + bmp_center
                              , p_space->bmp.yres - ((int)d.y + bmp_center)
                              ,                      (int)h.x + bmp_center
                              , p_space->bmp.yres - ((int)h.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)e.x + bmp_center
                              , p_space->bmp.yres - ((int)e.y + bmp_center)
                              ,                      (int)f.x + bmp_center
                              , p_space->bmp.yres - ((int)f.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)f.x + bmp_center
                              , p_space->bmp.yres - ((int)f.y + bmp_center)
                              ,                      (int)g.x + bmp_center
                              , p_space->bmp.yres - ((int)g.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)g.x + bmp_center
                              , p_space->bmp.yres - ((int)g.y + bmp_center)
                              ,                      (int)h.x + bmp_center
                              , p_space->bmp.yres - ((int)h.y + bmp_center)
                              , bmp_bounds
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)h.x + bmp_center
                              , p_space->bmp.yres - ((int)h.y + bmp_center)
                              ,                      (int)e.x + bmp_center
                              , p_space->bmp.yres - ((int)e.y + bmp_center)
                              , bmp_bounds
                         );
        if(p_space->show_quad_view)
        {
            a.x = LASERBOY_MIN_SHORT;
            a.y = LASERBOY_MIN_SHORT;
            a.z = 0;
            b.x = LASERBOY_MAX_SHORT;
            b.y = LASERBOY_MAX_SHORT;
            b.z = 0;
            if(offset_view)
            {
                a += p_space->view_offset;
                b += p_space->view_offset;
            }
            if(scale_view)
            {
                a *= p_space->view_scale;
                b *= p_space->view_scale;
            }
            a *= scale;
            b *= scale;

            bmp_put_rectangle(&(p_space->bmp),
                              (int)a.x + bmp_center,
                              (int)a.y + bmp_center,
                              (int)b.x + bmp_center,
                              (int)b.y + bmp_center,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(&(p_space->bmp),
                              (int)a.x + bmp_3X_center,
                              (int)a.y + bmp_center,
                              (int)b.x + bmp_3X_center,
                              (int)b.y + bmp_center,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(&(p_space->bmp),
                              (int)a.x + bmp_3X_center,
                              (int)a.y + bmp_3X_center,
                              (int)b.x + bmp_3X_center,
                              (int)b.y + bmp_3X_center,
                              bmp_bounds,
                              0
                             );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_fulcrum)
    {
        LaserBoy_3D_double  x1, x2, y1, y2, z1, z2;

        x1.x = -5000;
        x2.x =  5000;
        y1.y = -5000;
        y2.y =  5000;
        z1.z = -5000;
        z2.z =  5000;

        x1 = x1 += p_space->fulcrum;
        x2 = x2 += p_space->fulcrum;
        y1 = y1 += p_space->fulcrum;
        y2 = y2 += p_space->fulcrum;
        z1 = z1 += p_space->fulcrum;
        z2 = z2 += p_space->fulcrum;

        if(rotate_view)
        {
            x1 = rotate_vertex(x1, p_space->view_angle);
            x2 = rotate_vertex(x2, p_space->view_angle);
            y1 = rotate_vertex(y1, p_space->view_angle);
            y2 = rotate_vertex(y2, p_space->view_angle);
            z1 = rotate_vertex(z1, p_space->view_angle);
            z2 = rotate_vertex(z2, p_space->view_angle);
        }
        if(offset_view)
        {
            x1 += p_space->view_offset;
            x2 += p_space->view_offset;
            y1 += p_space->view_offset;
            y2 += p_space->view_offset;
            z1 += p_space->view_offset;
            z2 += p_space->view_offset;
        }
        if(scale_view)
        {
            x1 *= p_space->view_scale;
            x2 *= p_space->view_scale;
            y1 *= p_space->view_scale;
            y2 *= p_space->view_scale;
            z1 *= p_space->view_scale;
            z2 *= p_space->view_scale;
        }
        bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(x1.y * scale) + bmp_center)
                              ,                      (int)(x2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(x2.y * scale) + bmp_center)
                              , p_space->bmp.make_rgb(255,0,0)
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)(y1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_center)
                              ,                      (int)(y2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_center)
                              , p_space->bmp.make_rgb(0,255,0)
                         );
        bmp_put_line_fast(&(p_space->bmp),           (int)(z1.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(z1.y * scale) + bmp_center)
                              ,                      (int)(z2.x * scale) + bmp_center
                              , p_space->bmp.yres - ((int)(z2.y * scale) + bmp_center)
                              , p_space->bmp.make_rgb(0,0,255)
                         );
        if(p_space->show_quad_view)
        {
            x1 = x2 = y1 = y2 = z1 = z2 = 0;

            x1.x = -5000;
            x2.x =  5000;
            y1.y = -5000;
            y2.y =  5000;
            z1.z = -5000;
            z2.z =  5000;

            x1 = x1 += p_space->fulcrum;
            x2 = x2 += p_space->fulcrum;
            y1 = y1 += p_space->fulcrum;
            y2 = y2 += p_space->fulcrum;
            z1 = z1 += p_space->fulcrum;
            z2 = z2 += p_space->fulcrum;

            if(offset_view)
            {
                x1 += p_space->view_offset;
                x2 += p_space->view_offset;
                y1 += p_space->view_offset;
                y2 += p_space->view_offset;
                z1 += p_space->view_offset;
                z2 += p_space->view_offset;
            }
            if(scale_view)
            {
                x1 *= p_space->view_scale;
                x2 *= p_space->view_scale;
                y1 *= p_space->view_scale;
                y2 *= p_space->view_scale;
                z1 *= p_space->view_scale;
                z2 *= p_space->view_scale;
            }
            // front XY
            bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(x1.y * scale) + bmp_3X_center)
                                  ,                      (int)(x2.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(x2.y * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)(y1.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_3X_center)
                                  ,                      (int)(y2.x * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            // top XZ
            bmp_put_line_fast(&(p_space->bmp),           (int)(x1.x *  scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(x1.z * -scale) + bmp_3X_center)
                                  ,                      (int)(x2.x *  scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(x2.z * -scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(255,0,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)(z1.x *  scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(z1.z * -scale) + bmp_3X_center)
                                  ,                      (int)(z2.x *  scale) + bmp_center
                                  , p_space->bmp.yres - ((int)(z2.z * -scale) + bmp_3X_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
            // side YZ
            bmp_put_line_fast(&(p_space->bmp),           (int)(y1.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y1.y * scale) + bmp_center)
                                  ,                      (int)(y2.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(y2.y * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,255,0)
                             );
            bmp_put_line_fast(&(p_space->bmp),           (int)(z1.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(z1.y * scale) + bmp_center)
                                  ,                      (int)(z2.z * scale) + bmp_3X_center
                                  , p_space->bmp.yres - ((int)(z2.y * scale) + bmp_center)
                                  , p_space->bmp.make_rgb(0,0,255)
                             );
        }
    }
    //------------------------------------------------------------------------
    // ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------
    if(at(index).size() > 1)
    {
        if(p_space->show_onion_skin)
        {
            int skins =   p_space->number_of_skins <= p_space->number_of_frames()
                        ? p_space->number_of_skins
                        : p_space->number_of_frames();
            for(int skin = skins; skin >= 0; skin--)
            {
                if((index - skin) >= 0)
                    at(index - skin).render(skin);
                else if(p_space->loop_animation)
                    at(number_of_frames() + index - skin).render(skin);
            }
        }
        else
            at(index).render(0);
        //--------------------------------------------------------------------
        // END ACTUAL VECTOR ART !!!
        //--------------------------------------------------------------------
        if(p_space->show_cursors)
        {
            bit_masked = at(index).at(at(index).egg).bit_masked_color(p_space->signal_bit_mask);
            egg_color  = at(index).at(at(index).egg).is_blank()
                                 ? bmp_blank
                                 :   at(index).at(at(index).egg).is_black(p_space->black_level)
                                   ? p_space->bmp.make_rgb(0xff, 0xff, 0xff)
                                   : p_space->bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
            //----------------------------------------------------------------
            bit_masked = at(index).at(at(index).spider).bit_masked_color(p_space->signal_bit_mask);
            spider_color   =   at(index).at(at(index).spider).is_blank()
                                 ? bmp_blank
                                 :   at(index).at(at(index).spider).is_black(p_space->black_level)
                                   ? p_space->bmp.make_rgb(0xff, 0xff, 0xff)
                                   : p_space->bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
            //----------------------------------------------------------------
            _0 = at(index).at(at(index).egg   ).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(index).at(at(index).spider).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = at(index).egg    * z_depth_scale - z_depth_body_offset;
                _1.z = at(index).spider * z_depth_scale - z_depth_body_offset;
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
            //----------------------------------------------------------------
            // egg cursor
            bmp_put_rectangle(&(p_space->bmp),
                                                   (int)((_0.x - 1000) * scale) + bmp_center,
                              p_space->bmp.yres - ((int)((_0.y - 1000) * scale) + bmp_center),
                                                   (int)((_0.x + 1000) * scale) + bmp_center,
                              p_space->bmp.yres - ((int)((_0.y + 1000) * scale) + bmp_center),
                              egg_color,
                              0
                             );
            if(p_space->show_indices)
                bmp_printf(&(p_space->bmp),
                           (int)( _0.x * scale) + bmp_center,
                           (int)(-_0.y * scale) + bmp_3X_center,
                           bmp_bg_color,
                           egg_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           at(index).egg
                          );
            //----------------------------------------------------------------
            // spider cursor
            bmp_put_line_fast (&(p_space->bmp),
                                                    (int)((_1.x       ) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                    (int)((_1.x       ) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                               spider_color
                              );
            bmp_put_line_fast (&(p_space->bmp),
                                                    (int)((_1.x + 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_center),
                                                    (int)((_1.x - 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_center),
                               spider_color
                              );
            bmp_put_line_fast (&(p_space->bmp),
                                                    (int)((_1.x - 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                    (int)((_1.x + 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                               spider_color
                              );
            bmp_put_line_fast (&(p_space->bmp),
                                                    (int)((_1.x + 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                    (int)((_1.x - 1000) * scale) + bmp_center,
                               p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                               spider_color
                              );
            if(p_space->show_indices)
                bmp_printf(&(p_space->bmp),
                           (int)( _1.x * scale) + bmp_center,
                           (int)(-_1.y * scale) + bmp_3X_center,
                           bmp_bg_color,
                           spider_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           at(index).spider

                          );
            //----------------------------------------------------------------
            if(p_space->show_quad_view)
            {
                _0 = at(index).at(at(index).egg   ).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(index).at(at(index).spider).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = at(index).egg    * z_depth_scale - z_depth_body_offset;
                    _1.z = at(index).spider * z_depth_scale - z_depth_body_offset;
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
                //------------------------------------------------------------
                // egg cursor front
                bmp_put_rectangle(&(p_space->bmp),
                                                       (int)((_0.x - 1000) * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)((_0.y - 1000) * scale) + bmp_3X_center),
                                                       (int)((_0.x + 1000) * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)((_0.y + 1000) * scale) + bmp_3X_center),
                                  egg_color,
                                  0
                                 );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)( _0.x * scale) + bmp_3X_center,
                               (int)(-_0.y * scale) + bmp_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor front
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x       ) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_3X_center),
                                                        (int)((_1.x       ) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_3X_center),
                                                        (int)((_1.x - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_3X_center),
                                                        (int)((_1.x + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_3X_center),
                                                        (int)((_1.x - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_3X_center),
                                   spider_color
                                  );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)( _1.x * scale) + bmp_3X_center,
                               (int)(-_1.y * scale) + bmp_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
                //------------------------------------------------------------
                // egg cursor top
                bmp_put_rectangle(&(p_space->bmp),
                                                       (int)((_0.x - 1000) *  scale) + bmp_center,
                                  p_space->bmp.yres - ((int)((_0.z - 1000) * -scale) + bmp_3X_center),
                                                       (int)((_0.x + 1000) *  scale) + bmp_center,
                                  p_space->bmp.yres - ((int)((_0.z + 1000) * -scale) + bmp_3X_center),
                                  egg_color,
                                  0
                                 );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)( _0.x * scale) + bmp_center,
                               (int)( _0.z * scale) + bmp_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor top
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x       ) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z - 1000) * -scale) + bmp_3X_center),
                                                        (int)((_1.x       ) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z + 1000) * -scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x + 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z       ) * -scale) + bmp_3X_center),
                                                        (int)((_1.x - 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z       ) * -scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x - 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z - 1000) * -scale) + bmp_3X_center),
                                                        (int)((_1.x + 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z + 1000) * -scale) + bmp_3X_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.x + 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z - 1000) * -scale) + bmp_3X_center),
                                                        (int)((_1.x - 1000) *  scale) + bmp_center,
                                   p_space->bmp.yres - ((int)((_1.z + 1000) * -scale) + bmp_3X_center),
                                   spider_color
                                  );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)(_1.x * scale) + bmp_center,
                               (int)(_1.z * scale) + bmp_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
                //------------------------------------------------------------
                // egg cursor side
                bmp_put_rectangle(&(p_space->bmp),
                                                       (int)((_0.z - 1000) * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)((_0.y - 1000) * scale) + bmp_center),
                                                       (int)((_0.z + 1000) * scale) + bmp_3X_center,
                                  p_space->bmp.yres - ((int)((_0.y + 1000) * scale) + bmp_center),
                                  egg_color,
                                  0
                                 );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)( _0.z * scale) + bmp_3X_center,
                               (int)(-_0.y * scale) + bmp_3X_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor side
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.z       ) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                        (int)((_1.z       ) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.z + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_center),
                                                        (int)((_1.z - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y       ) * scale) + bmp_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.z - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                        (int)((_1.z + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                                   spider_color
                                  );
                bmp_put_line_fast (&(p_space->bmp),
                                                        (int)((_1.z + 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y - 1000) * scale) + bmp_center),
                                                        (int)((_1.z - 1000) * scale) + bmp_3X_center,
                                   p_space->bmp.yres - ((int)((_1.y + 1000) * scale) + bmp_center),
                                   spider_color
                                  );
                if(p_space->show_indices)
                    bmp_printf(&(p_space->bmp),
                               (int)( _1.z * scale) + bmp_3X_center,
                               (int)(-_1.y * scale) + bmp_3X_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
            }
            //----------------------------------------------------------------
        } // end if(p_space->show_cursors)
        //--------------------------------------------------------------------
    } // end if(at(index).size() > 1)
    return;
}

//############################################################################
bool LaserBoy_frame_set::from_nothing()
{
    LaserBoy_frame frame(p_space);
    push_back(frame);
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    num_2D_frames = 0;
    num_3D_frames = 0;
    p_space->p_GUI->display_state("NO DATA");
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ild_file(const string& file,
                                                      long int&     bytes_skipped
                                                     )
{
    ifstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        from_ifstream_ild(in, bytes_skipped);
        in.close();
        if(number_of_frames() == 0) // create an empty frame set
        {
            from_nothing();
            frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
        }
    }
    else // file failed to open
    {
        from_nothing();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    return frame_set_error;
}

//############################################################################
void LaserBoy_frame_set::superimpose_frame_set(LaserBoy_frame_set& overlay)
{
    u_int               i;
    LaserBoy_frame_set  sum    (p_space);

    if(overlay.number_of_frames() > 1 && number_of_frames() > 1)
    {
        if(number_of_frames() > overlay.number_of_frames())
        {
            for(i = 0; i < overlay.number_of_frames(); i++)
                sum += (frame(i) + overlay.frame(i));

            for(i = overlay.number_of_frames(); i < number_of_frames(); i++)
                sum += frame(i);
        }
        else if(number_of_frames() < overlay.number_of_frames())
        {
            for(i = 0; i < number_of_frames(); i++)
                sum += (frame(i) + overlay.frame(i));

            for(i = number_of_frames(); i < overlay.number_of_frames(); i++)
                sum += overlay.frame(i);
        }
        else
        {
            for(i = 0; i < number_of_frames(); i++)
                sum += (frame(i) + overlay.frame(i));
        }
    }
    else if(overlay.number_of_frames() == 1)
    {
        for(i = 0; i < number_of_frames(); i++)
            sum += (frame(i) + overlay.frame(0));
    }
    else if(number_of_frames() == 1)
    {
        for(i = 0; i < overlay.number_of_frames(); i++)
            sum += (frame(0) + overlay.frame(i));
    }
    *this = sum;
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::superimpose_from_txt_file(const string& file,
                                                                  int &new_frame_count,
                                                                  int &new_palette_count
                                                                 )
{
    LaserBoy_frame_set  overlay(p_space),
                        sum    (p_space);
    u_int               i = p_space->number_of_palettes();

    overlay.from_txt_file(file);
    new_frame_count   = overlay.number_of_frames();
    new_palette_count = p_space->number_of_palettes() - i;
    
    if(new_frame_count)
    {
        if(overlay.number_of_frames() > 1 && number_of_frames() > 1)
        {
            if(number_of_frames() > overlay.number_of_frames())
            {
                for(i = 0; i < overlay.number_of_frames(); i++)
                    sum += (frame(i) + overlay.frame(i));

                for(i = overlay.number_of_frames(); i < number_of_frames(); i++)
                    sum += frame(i);
            }
            else if(number_of_frames() < overlay.number_of_frames())
            {
                for(i = 0; i < number_of_frames(); i++)
                    sum += (frame(i) + overlay.frame(i));

                for(i = number_of_frames(); i < overlay.number_of_frames(); i++)
                    sum += overlay.frame(i);
            }
            else
            {
                for(i = 0; i < number_of_frames(); i++)
                    sum += (frame(i) + overlay.frame(i));
            }
        }
        else if(overlay.number_of_frames() == 1)
        {
            for(i = 0; i < number_of_frames(); i++)
                sum += (frame(i) + overlay.frame(0));
        }
        else if(number_of_frames() == 1)
        {
            for(i = 0; i < overlay.number_of_frames(); i++)
                sum += (frame(0) + overlay.frame(i));
        }
        *this = sum;
    }
    frame_set_error = overlay.frame_set_error;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_dxf_file(const string& file, bool append)
{
    ifstream in(file.c_str(), ios::in);
    //------------------------------------------------------------------------
    if(in.is_open())
    {
        LaserBoy_frame frame(p_space);
        frame_set_error = LASERBOY_OK;
        if(!append)
        {
            clear();
            num_2D_frames = 0;
            num_3D_frames = 0;
            frame_index   = 0;
        }
        frame_set_error |= frame.from_ifstream_dxf(in);
        if(frame_set_error == LASERBOY_OK)
        {
            p_space->palette_index = frame.palette_index;
            push_back(frame);
            if(back().find_rgb_in_palette(p_space->palette_picker(LASERBOY_DXF)))
                back().palette_index = LASERBOY_DXF;
        }
        else
        {
            from_nothing();
            frame_index = 0;
        }
        in.close();
    }
    else
    {
        if(!append)
        {
            from_nothing();
            frame_index = 0;
        }
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
bool LaserBoy_frame_set::from_dxf_directory(const string& dir, bool append)
{
    int                     opened_frames = 0;
    DIR                    *pDIR;
    struct dirent          *pDirEnt;
    vector<string>          file_list;
    //------------------------------------------------------------------------
    pDIR = opendir(dir.c_str());
    //------------------------------------------------------------------------
    if(pDIR == NULL)
    {
        if(!append)
        {
            from_nothing();
            frame_index = 0;
            frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        }
        else
            frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    } // end if(pDIR == NULL)
    else
    {
        pDirEnt = readdir(pDIR);
        while(pDirEnt != NULL)
        {
            if(    strcmp(pDirEnt->d_name, "." ) // not the dot
                && strcmp(pDirEnt->d_name, "..") // not the dot dot
              )
                file_list.push_back(pDirEnt->d_name);
            pDirEnt = readdir(pDIR);
        }
        closedir(pDIR);
        if(file_list.size())
        {
            bool  valid_data = false;
            int   i, j;

            LaserBoy_real_segment      real_vertices(p_space);
            LaserBoy_real_segment_set  real_segments(p_space);
            LaserBoy_vertex            vertex;
            LaserBoy_frame             frame(p_space);
            //----------------------------------------------------------------
            frame.format        = LASERBOY_3D_FRAME ;
            frame.palette_index = LASERBOY_TRUE_COLOR;
            //----------------------------------------------------------------
            if(!append)
            {
                clear();
                num_2D_frames = 0;
                num_3D_frames = 0;
            }
            //----------------------------------------------------------------
            // for systems that don't return file lists in alpha-numeric order!
            sort(file_list.begin(), file_list.end());
            //----------------------------------------------------------------
            p_space->p_GUI->display_state("opening DXF files");
            for(i = 0; i < (int)file_list.size(); i++)
            {
                if(file_list[i].size() >= 5) // ?.dxf
                {
                    ifstream in((dir + '/' + file_list[i]).c_str(), ios::in);
                    if(in.is_open())
                    {
                        real_vertices.from_ifstream_dxf(in);
                        if(real_vertices.size() >= 2) // there is a picture in there
                            valid_data = true;
                        real_segments.push_back(real_vertices);
                        real_vertices.clear();
                        in.close();
                        p_space->p_GUI->display_progress(file_list.size() - i);
                    }
                }
            }
            //----------------------------------------------------------------
            if(valid_data)
            {
                real_segments.normalize();
                reserve(real_segments.size());
                //------------------------------------------------------------
                p_space->p_GUI->display_state("normalizing DXF frames");
                for(j = 0; j < (int)real_segments.size(); j++)
                {
                    if(real_segments[j].size() >= 2)
                    {
                        for(i = 2; i < (int)real_segments[j].size(); i++)
                        {
                            vertex = (LaserBoy_vertex)real_segments[j][i];
                            if(real_segments[j][i].is_blank())
                                vertex.blank();
                            else
                                vertex.unblank();
                            frame.push_back(vertex);
                            opened_frames++;
                            vertex.clear();
                        }
                    } // end if(real_segments[j].size() >= 2)
                    else
                    {
                        frame.push_back(vertex);
                        frame.push_back(vertex);
                    }
                    if(frame.find_rgb_in_palette(p_space->palette_picker(LASERBOY_DXF)))
                        frame.palette_index = LASERBOY_DXF;
                    frame.is_2D();
                    push_back(frame);
                    frame.clear();
                    p_space->p_GUI->display_progress(real_segments.size() - j);
                }
                //------------------------------------------------------------
            } // end if(valid_data)
            else
            {
                frame.push_back(vertex);
                frame.push_back(vertex);
                frame.palette_index = LASERBOY_DXF;
                push_back(frame);
                opened_frames++;
            }
        //--------------------------------------------------------------------
        } // end if(file_list.size())
        if(!opened_frames)
        {
            if(!append)
            {
                from_nothing();
                frame_index = 0;
                frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
            }
            else
                frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        }
    } // end else of if(pDIR == NULL)
    if(!opened_frames)
    {
        from_nothing();
        frame_index = 0;
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::from_LaserBoy_wave_file(const string& file, bool append)
{
    fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index = 0;
        //--------------------------------------------------------------------
        if(header.version == "!LaserBoy!")
        {
            frame_set_error |= LASERBOY_LB_WAVE_FORMAT_ERROR;
            return false;
        }
        //--------------------------------------------------------------------
        else if(header.LaserBoy_wave_mode & LASERBOY_WAVE_END_OF_FRAME)
            frame_set_error |= from_fstream_LaserBoy_wave(header, in, append);
        //--------------------------------------------------------------------
        else
            frame_set_error |= from_fstream_unframed_wave(header, in, append);
        //--------------------------------------------------------------------
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::from_unformatted_wave_file(const string& file, bool global_polarity, bool append)
{
    fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index      = 0;
        frame_set_error |= from_fstream_unformatted_wave(header, in, global_polarity, append);
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::from_qm_wave_file(const string& file, bool global_polarity, bool append)
{
    fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index      = 0;
        frame_set_error |= from_fstream_qm_wave(header, in, global_polarity, append);
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ifstream_ild(ifstream& in,
                                                          long int& bytes_skipped
                                                         )
{
    LaserBoy_ild_header  header           ;
    LaserBoy_frame       frame   (p_space);
    LaserBoy_palette     palette (p_space);
    int                  file_sections = 0;
    //------------------------------------------------------------------------
    clear();
    num_2D_frames = 0;
    num_3D_frames = 0;
    //------------------------------------------------------------------------
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening ILDA file sections");
    while(header.from_ifstream_ild(in, bytes_skipped))
    {
        if(header.quantity)
        {
            switch(header.format)
            {
                //----------------------------------------------------------------
                case LASERBOY_3D_FRAME: // format 0
                case LASERBOY_2D_FRAME: // format 1
                    if(frame.from_ifstream_ild(in, header))
                    {
                        frame.palette_index = p_space->palette_index;
                        frame.set_rgb_from_palette();
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //----------------------------------------------------------------
                case LASERBOY_PALETTE: // format 2
                    if(palette.from_ifstream_ild(in, header))
                    {
                        bool built_in_palette = false;
                        for(int i = 0; i < LASERBOY_BUILT_IN_PALETTES; i++)
                            if(header.name == p_space->palette_picker(i).name)
                            {
                                p_space->palette_index = i;
                                built_in_palette = true;
                                break;
                            }
                        if(!built_in_palette)
                        {
                            p_space->push_back_palette(palette);
                            p_space->palette_index = p_space->number_of_palettes() - 1;
                        }
                        p_space->p_GUI->display_progress(++file_sections);
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //----------------------------------------------------------------
                case LASERBOY_TABLE: // format 3
                    if(frame.from_ifstream_format_3(in, header, bytes_skipped))
                    {
                        p_space->palette_index = LASERBOY_TRUE_COLOR;
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                        frame_set_error |= frame.segment_error;
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //----------------------------------------------------------------
                case LASERBOY_3D_FRAME_RGB: // format 4
                case LASERBOY_2D_FRAME_RGB: // format 5
                    if(frame.from_ifstream_ild(in, header))
                    {
                        frame.palette_index = LASERBOY_TRUE_COLOR;
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //----------------------------------------------------------------
            } // end switch(header.format)
        } // end if(header.quantity)
        else
            ild_file_GUID = header.owner;
    } // end while(header.from_ifstream_ild(in))
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_txt_file(const string& file_name, bool append)
{
    u_int                      i,
                               frame_count   = 0,
                               section_count = 0,
                               line_number   = 1;
    vector<int>                real_frames_index;
    string                     word;
    LaserBoy_frame             frame   (p_space);
    LaserBoy_palette           palette (p_space);
    LaserBoy_real_segment      real_segment (p_space);
    LaserBoy_real_segment_set  real_segments(p_space);

    ifstream in       (file_name.c_str(), ios::in ); // not binary!
    ofstream error_log("txt_in_errors.txt", ios::out);
    //------------------------------------------------------------------------
    if(in.is_open())
    {
        frame_index     = 0;
        frame_set_error = LASERBOY_OK;
    }
    else
    {
        error_log << file_name
                  << " error: file does not exist"
                  << ENDL;
        error_log.close();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        return frame_set_error;
    }
    //------------------------------------------------------------------------
    if(!append)
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening text file sections");
    while(in.good())
    {
        clear_to_alpha(in, line_number);
        if(get_next_word(in, word, line_number))
        {
            //----------------------------------------------------------------
            // frame
            //----------------------------------------------------------------
            if(word == "frame")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "xy") // 2D
                    {
                        // frame xy
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xy rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy rgb short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy rgb unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy rgb ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy rgb short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "hex")
                            {
                                // frame xy hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy hex short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy hex unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token.(frame xy hex ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy hex short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "palette")
                            {
                                // frame xy palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy palette short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy palette unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. ( frame xy palette ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy palette short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xy table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy table short
                                        if(number_of_frames())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xy table short ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy table unit
                                        if(number_of_frames())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_UNIT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xy table unit ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, line_number);
                                        for(u_int i = 0; i < back().size(); i++)
                                        {
                                            real_segment[i + 2].r = back()[i].r;
                                            real_segment[i + 2].g = back()[i].g;
                                            real_segment[i + 2].b = back()[i].b;
                                            real_segment[i + 2].c = back()[i].c;
                                            real_segment[i + 2].k = back()[i].k;
                                        }
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xy table real ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy table ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy table short
                                    back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                    if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                    {
                                        error_log << file_name
                                                  << " error before line: "
                                                  << line_number
                                                  << " color table to vertex count mismatch. (frame xy table short ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                    }
                                    frame_count++;
                                }
                            } // end else if(word == "table")
                            else
                            {
                                // frame xy bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xy ERROR)"
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                        else
                        {
                            // frame xy rgb short
                            frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                            push_back(frame);
                            frame_count++;
                        }
                    }
                    else if(word == "xyz") // 3D
                    {
                        // frame xyz
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xyz rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz rgb short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz rgb unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz rgb ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz rgb short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }

                            }
                            else if(word == "hex")
                            {
                                // frame xyz hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz hex short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz hex unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz hex ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz hex short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }

                            }
                            else if(word == "palette")
                            {
                                // frame xyz palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz palette short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz palette unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, line_number);
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz palette ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz palette short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xyz table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz table short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                        if(frame.segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz table unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_UNIT, line_number);
                                        if(frame.segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table unit ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, line_number);
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table real ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        for(u_int i = 0; i < back().size(); i++)
                                        {
                                            real_segment[i + 2].r = back()[i].r;
                                            real_segment[i + 2].g = back()[i].g;
                                            real_segment[i + 2].b = back()[i].b;
                                            real_segment[i + 2].c = back()[i].c;
                                            real_segment[i + 2].k = back()[i].k;
                                        }
                                        real_segments.push_back(real_segment);
                                        real_frames_index.push_back(frame_count);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz table ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz table short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                    if(frame.segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                    {
                                        error_log << file_name
                                                  << " error before line: "
                                                  << line_number
                                                  << " color table to vertex count mismatch. (frame xyz table short ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                    }
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else
                            {
                                // frame xyz bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xyz ERROR)"
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                        else
                        {
                            // frame xyz rgb short
                            frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                            push_back(frame);
                            frame_count++;
                        }
                    }
                    else
                    {
                        // frame bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (frame ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                else
                {
                    // frame xy rgb short
                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                    push_back(frame);
                    frame_count++;
                }
            }
            //----------------------------------------------------------------
            // palette
            //----------------------------------------------------------------
            else if(word == "palette")
            {
                // palette
                if(get_next_word(in, word, line_number))
                {
                    if(word == "named")
                    {
                        if(get_next_word(in, word, line_number))
                        {   // palette named name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_OK:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else
                        {
                            // palette named [no-name]
                            error_log << file_name
                                      << " error on line: "
                                      << line_number
                                      << " LaserBoy palette name is missing. (palette named [no-name])"
                                      << ENDL;
                            frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // this is impossible
                                        break;
                                case LASERBOY_OK:
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "rgb")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette rgb name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_IN_USE;
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette rgb [no-name]
                        {
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // impossible
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "hex")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette hex name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_IN_USE;
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette hex [no-name]
                        {
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // impossible
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else
                    {
                        // palette bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (palette ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number))
                else // there was no word after palette
                {
                    // palette rgb [no-name]
                    switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                    {
                        case LASERBOY_TXT_PALETTE_NAME_IN_USE:
                        case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                // impossible
                                break;
                        case LASERBOY_OK:
                                // this is what we expect.
                                break;
                        case LASERBOY_TXT_PALETTE_FAILED:
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " invalid palette data."
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                break;
                    }
                }
            }
            //----------------------------------------------------------------
            // table
            //----------------------------------------------------------------
            else if(word == "table")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "rgb")
                    {
                        // table rgb
                        frame.from_ifstream_txt(in, LASERBOY_TABLE, LASERBOY_RGB, line_number);
                        push_back(frame);
                    }
                    else if(word == "hex")
                    {
                        // table hex
                        frame.from_ifstream_txt(in, LASERBOY_TABLE, LASERBOY_HEX, line_number);
                        push_back(frame);
                    }
                    else
                    {
                        // table bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (table ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                else
                {
                    // table rgb
                    frame.from_ifstream_txt(in, LASERBOY_TABLE, LASERBOY_RGB, line_number);
                    push_back(frame);
                }
            }
            else
            {
                // bad_word!
                error_log << file_name
                          << " error on line: "
                          << line_number
                          << " \'"
                          << word
                          << "\' is not a LaserBoy text format token. (ERROR)"
                          << ENDL;
                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
            }
        } // end if(get_next_word(in, word, line_number))
        p_space->p_GUI->display_progress(++section_count);
    } // end while(in.good())
    //------------------------------------------------------------------------
    if(real_segments.size())
    {
        real_segments.normalize();
        for(i = 0; i < real_frames_index.size(); i++)
            at(real_frames_index[i]) = (LaserBoy_segment)real_segments[i];
    }
    //------------------------------------------------------------------------
    in.close();
    error_log.close();
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_LaserBoy_wave(LaserBoy_wave_header header,
                                                                   fstream& in,
                                                                   bool append
                                                                  )
{
    u_int                         i,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame(p_space);
    LaserBoy_vertex               vertex;
    LaserBoy_wave_sample_pointer  roll_over;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening formatted wave");
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    span  = (offset_max - offset_min) + 2; // size of sample_window
    last  = span - 1                     ; // last index of sample_window
    red   = -header.offset[2] + 1        ; // index of red
    green = -header.offset[3] + 1        ; // index of green
    blue  = -header.offset[4] + 1        ; // index of blue
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;

        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;

        if((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) == 0)
            sample_window[last]->negate();
        //--------------------------------------------------------------------
        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        if(header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
            vertex.z = sample_window[1]->channel[5];
        else
            vertex.z = 0;
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.r = rescale_to_index(header.color_rescale_r, sample_window[red]->channel[2]);
        else
        {
            if(sample_window[red]->channel[2]) // != 0
                vertex.r = (u_char)abs(sample_window[red]->channel[2] >> 7);
            else
                vertex.r = 0;
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
            vertex.g = rescale_to_index(header.color_rescale_g, sample_window[green]->channel[3]);
        else
        {
            if(sample_window[green]->channel[3])
                vertex.g = (u_char)abs(sample_window[green]->channel[3] >> 7);
            else
                vertex.g = 0;
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
            vertex.b = rescale_to_index(header.color_rescale_b, sample_window[blue]->channel[4]);
        else
        {
            if(sample_window[blue]->channel[4])
                vertex.b = (u_char)abs(sample_window[blue]->channel[4] >> 7);
            else
                vertex.b = 0;
        }
        //--------------------------------------------------------------------
        vertex.k = 0;
        vertex.c = 0;
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        frame.push_back(vertex);
        //--------------------------------------------------------------------
        if(sample_window[red]->channel[2] & 0x0001) // end of frame
        {
            frame.is_unique = true;
            if(    (header.LaserBoy_wave_mode & LASERBOY_WAVE_UNIQUE_FRAME)  // if we are looking for it
                && (!(sample_window[green]->channel[3] & 0x0001)) // and we do not find it
              )
                frame.is_unique = false;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
        //--------------------------------------------------------------------
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    delete sample_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_unframed_wave(LaserBoy_wave_header header,
                                                                   fstream& in,
                                                                   bool append
                                                                  )
{
    u_int                         i,
                                  vertices = 0,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame(p_space);
    LaserBoy_vertex               vertex        ;
    LaserBoy_wave_sample_pointer  roll_over    ;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening unframed wave");
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    span  = (offset_max - offset_min) + 2; // size of sample_window
    last  = span - 1                     ; // last index of sample_window
    red   = -header.offset[2] + 1        ; // index of red
    green = -header.offset[3] + 1        ; // index of green
    blue  = -header.offset[4] + 1        ; // index of blue
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;

        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;

        if((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) == 0)
            sample_window[last]->negate();
        //--------------------------------------------------------------------
        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        if(header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
            vertex.z = sample_window[1]->channel[5];
        else
            vertex.z = 0;
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.r = rescale_to_index(header.color_rescale_r, sample_window[red]->channel[2]);
        else
        {
            if(sample_window[red  ]->channel[2] > 0)
                vertex.r = (u_char)abs(sample_window[red  ]->channel[2] >> 7);
            else
                vertex.r = 0;
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.g = rescale_to_index(header.color_rescale_g, sample_window[green]->channel[3]);
        else
        {
            if(sample_window[green]->channel[3] > 0)
                vertex.g = (u_char)abs(sample_window[green]->channel[3] >> 7);
            else
                vertex.g = 0;
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.b = rescale_to_index(header.color_rescale_b, sample_window[blue]->channel[4]);
        else
        {
            if(sample_window[blue ]->channel[4] > 0)
                vertex.b = (u_char)abs(sample_window[blue ]->channel[4] >> 7);
            else
                vertex.b = 0;
        }
        //--------------------------------------------------------------------
        vertex.k = 0;
        vertex.c = 0;
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //--------------------------------------------------------------------
        frame.push_back(vertex);
        vertices++;
        if(!(vertices % p_space->unframed_wave_stride))
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    delete sample_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_qm_wave(LaserBoy_wave_header header,
                                                             fstream& in,
                                                             bool global_polarity,
                                                             bool append
                                                            )
{
    u_int            i,
                     color_sum,
                     static_sample_count = 0,
                     verticies = 0,
                     span,
                     last;
    int              red,
                     green,
                     blue,
                     offset_max = p_space->wave_offsets[0],
                     offset_min,
                     total_frames = 0;
    double           red_gain   = 1.0 / 128.0,
                     green_gain = 1.0 / 128.0,
                     blue_gain  = 1.0 / 128.0;
    LaserBoy_frame   frame(p_space);
    LaserBoy_vertex  vertex,
                     vertex_1,
                     vertex_2,
                     vertex_3,
                     vertex_4;

    LaserBoy_wave_sample_pointer  roll_over    ;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening blanked wave");
    //------------------------------------------------------------------------
    for(i = 1; i < 6; i++)
        if(p_space->wave_offsets[i] > offset_max)
            offset_max = p_space->wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        p_space->wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = p_space->wave_offsets[i]; // use current system offsets
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    //------------------------------------------------------------------------
    span  = (offset_max - offset_min) + 2; // at least 2 samples!
    last  = span - 1;
    red   = -header.offset[2] + 1;
    green = -header.offset[3] + 1;
    blue  = -header.offset[4] + 1;
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;
        verticies++;
        if(global_polarity)
            sample_window[last]->negate();
        //--------------------------------------------------------------------
        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        vertex.z = 0;
        color_sum =   sample_window[red  ]->channel[2]
                    + sample_window[green]->channel[3]
                    + sample_window[blue ]->channel[4];
        if(color_sum < 2000) // very close to black!
        {
            vertex.r = 0;
            vertex.g = 0;
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
            vertex.c = 0; // 24 bit

            if(    (vertex | vertex_2) <= p_space->insignificant_distance
                && (vertex | vertex_4) <= p_space->insignificant_distance
              )
                static_sample_count++;

            if(static_sample_count > 80) // we found a frame boundary
            {
                frame.is_unique = true;
                if((int)frame.size() > 1)
                {
                    frame.at(1).blank();
                    push_back(frame);
                    frame.clear();
                }
                verticies = 0;
                static_sample_count = 0;
                while(true)
                {
                    roll_over = sample_window[0];
                    for(i = 0; i < last; i++)
                        sample_window[i] = sample_window[i + 1];
                    sample_window[last] = roll_over;
                    if(!sample_window[last]->from_fstream_wave(in)) // if EOF
                        break;
                    verticies++;
                    if(global_polarity)
                        sample_window[last]->negate();
                    color_sum =   sample_window[red  ]->channel[2]
                                + sample_window[green]->channel[3]
                                + sample_window[blue ]->channel[4];
                    vertex.x = sample_window[1]->channel[0];
                    vertex.y = sample_window[1]->channel[1];
                    vertex.z = 0;
                    if(color_sum > 2000) // we have color!
                        break;
                    frame.push_back(vertex);
                }
            }
            vertex_4 = vertex_3;
            vertex_3 = vertex_2;
            vertex_2 = vertex_1;
            vertex_1 = vertex;
        }
        else
        {
            vertex.r = (u_char)((sample_window[red  ]->channel[2] > 0) ? (sample_window[red  ]->channel[2] *   red_gain) : (0));
            vertex.g = (u_char)((sample_window[green]->channel[3] > 0) ? (sample_window[green]->channel[3] * green_gain) : (0));
            vertex.b = (u_char)((sample_window[blue ]->channel[4] > 0) ? (sample_window[blue ]->channel[4] *  blue_gain) : (0));
            vertex.k = 0;
            vertex.c = 0;
            static_sample_count = 0;
        }
        //--------------------------------------------------------------------
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //--------------------------------------------------------------------
        frame.push_back(vertex);
        if(verticies == p_space->unframed_wave_stride)
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            verticies = 0;
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    delete sample_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_unformatted_wave(LaserBoy_wave_header header,
                                                                      fstream&             in,
                                                                      bool                 global_polarity,
                                                                      bool                 append
                                                                     )
{
    u_int                         i,
                                  vertices = 0,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame(p_space);
    LaserBoy_vertex               vertex;
    LaserBoy_wave_sample_pointer  roll_over;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening unformatted wave");
    //------------------------------------------------------------------------
    offset_max = p_space->wave_offsets[0];
    for(i = 1; i < 6; i++)
        if(p_space->wave_offsets[i] > offset_max)
            offset_max = p_space->wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        p_space->wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = p_space->wave_offsets[i]; // use current system offsets
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    //------------------------------------------------------------------------
    span  = (offset_max - offset_min) + 2; // at least 2 samples!
    last  = span - 1;
    red   = -header.offset[2] + 1;
    green = -header.offset[3] + 1;
    blue  = -header.offset[4] + 1;
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;

        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;

        vertices++;

        if(global_polarity)
            sample_window[last]->negate();

        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        vertex.z = 0;
        switch(header.num_channels)
        {
            case 2: vertex.r =
                    vertex.g =
                    vertex.b = 255;
                    break;
            //----------------------------------------------------------------
            case 4: vertex.r =
                    vertex.g =
                    vertex.b = (u_char) abs(sample_window[red  ]->channel[2] / 128);
                    break;
            //----------------------------------------------------------------
            case 6:
            case 8: if(p_space->z_from_unformatted)
                    {
                        vertex.z = sample_window[1]->channel[5];
                        header.signal_id[5] = LASERBOY_SIGNAL_Z_POSITION;
                    }
                    vertex.r = (u_char) abs(sample_window[red  ]->channel[2] / 128);
                    vertex.g = (u_char) abs(sample_window[green]->channel[3] / 128);
                    vertex.b = (u_char) abs(sample_window[blue ]->channel[4] / 128);
                    break;
            //----------------------------------------------------------------
        }
        vertex.k = 0;
        vertex.c = 0;
        //--------------------------------------------------------------------
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //--------------------------------------------------------------------
        frame.push_back(vertex);

        if(!(vertices % p_space->unframed_wave_stride))
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    return frame_set_error;
}

//############################################################################
bool LaserBoy_frame_set::is_2D()
{
    p_space->p_GUI->display_state("checking for 2D");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        if(!at(i).is_2D())
            return false;
    }
    return true;
}

//############################################################################
void LaserBoy_frame_set::flatten_z()
{
    p_space->p_GUI->display_state("flattening Z");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).flatten_z();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::z_order_vertices(unsigned short span)
{
    p_space->p_GUI->display_state("Z ordering");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).z_order_vertices(span);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::unblank_all_vertices()
{
    p_space->p_GUI->display_state("unblanking all");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).unblank_all_vertices();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::convert_black_to_blank()
{
    p_space->p_GUI->display_state("converting blank to black");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).convert_black_to_blank();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::convert_blank_to_black()
{
    p_space->p_GUI->display_state("converting black to blank");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).convert_blank_to_black();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::impose_black_level()
{
    p_space->p_GUI->display_state("imposing black level");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).impose_black_level();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::reduce_blank_vectors()
{
    p_space->p_GUI->display_state("reducing blank vectors");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).reduce_blank_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::remove_dots()
{
    p_space->p_GUI->display_state("removing dots");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).remove_dots();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::enhance_dots()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("enhancing dots");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).enhance_dots();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::remove_dwell_vertices()
{
    p_space->p_GUI->display_state("removing dwell");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).remove_dwell_vertices();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::remove_short_vectors()
{
    p_space->p_GUI->display_state("removing short vectors");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).remove_short_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::reduce_lit_vectors()
{
    p_space->p_GUI->display_state("reducing lit vectors");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).reduce_lit_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::minimize()
{
    p_space->p_GUI->display_state("minimizing frames");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).minimize(i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_dots()
{
    p_space->p_GUI->display_state("converting to dots");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).to_dots();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::impose_bit_resolution()
{
    p_space->p_GUI->display_state("bit reducing");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).impose_bit_resolution();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::explode_current_frame()
{
    u_int               i;
    LaserBoy_frame_set  results(p_space),
                        exploded = current_frame().explode_segments();

    for(i = 0; i < frame_index; i++)
        results += at(i);

    results += exploded;

    for(i = frame_index + 1; i < number_of_frames(); i++)
        results += at(i);

    *this = results;
    return;
}

//############################################################################
void LaserBoy_frame_set::explode_all_frames()
{
    u_int               i;
    LaserBoy_frame_set  results (p_space),
                        exploded(p_space);
    p_space->p_GUI->display_state("exploding frames");
    for(i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        results +=  at(i).explode_segments();
    }

    *this = results;
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::collapse_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame  collapsed(p_space);
        int             first_selected_frame_index = -1;

        for(u_int i = 0; i < number_of_frames(); i++)
        {
            if(at(i).is_selected)
            {
                if(first_selected_frame_index == -1)
                {
                    first_selected_frame_index = i;
                    collapsed.palette_index = at(i).palette_index;
                }
                collapsed += at(i);
            }
        }
        if(collapsed.size() <= LASERBOY_MAX_USHORT)
        {
            if(first_selected_frame_index != -1)
            {
                frame(first_selected_frame_index) = collapsed;
                frame(first_selected_frame_index).is_selected = false;
                delete_selected_frames();
            }
        }
        else
            return LASERBOY_VERTEX_COUNT_OVERFLOW;
    }
    return LASERBOY_OK;
}

//############################################################################
void LaserBoy_frame_set::delete_every_nth_frame(unsigned short n)
{
    LaserBoy_frame_set results(p_space);
    p_space->p_GUI->display_state("omitting frames");
    if(n > 1 && n < number_of_frames())
    {
        for(u_int i = 0; i < number_of_frames(); i++)
        {
            p_space->p_GUI->display_progress(number_of_frames() - i);
            if(i % n)
                results += at(i);
        }
        *this = results;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::keep_every_nth_frame(unsigned short n)
{
    LaserBoy_frame_set results(p_space);
    p_space->p_GUI->display_state("omitting frames");
    if(n > 1 && n < number_of_frames())
    {
        for(u_int i = 0; i < number_of_frames(); i++)
        {
            p_space->p_GUI->display_progress(number_of_frames() - i);
            if(!(i % n))
                results += at(i);
        }
        *this = results;
        normalize_cursors();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::fracture_segments()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("fracturing segments");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).fracture_segments();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::omit_equivalent_vectors()
{
    p_space->p_GUI->display_state("blanking equal vectors");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).omit_equivalent_vectors();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::bond_segments()
{
    p_space->p_GUI->display_state("bonding segments in frame");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).bond_segments();
    }
    return LASERBOY_OK;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::reorder_segments()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    LaserBoy_frame origin;
    origin.push_back(LaserBoy_vertex());
    origin.push_back(LaserBoy_vertex());

    stat |= front().reorder_segments(origin);

    p_space->p_GUI->display_state("reordering segments in frame");
    for(u_int i = 1; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).reorder_segments(at(i - 1));
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::randomize_segments()
{
    p_space->p_GUI->display_state("randomizing segments in frame");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).randomize_segments();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::conglomerate_lit_segments()
{
    p_space->p_GUI->display_state("conglomerating segments in frame");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).conglomerate_lit_segments();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_dwell()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding dwell");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).add_dwell();
    }
    return stat;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_lit_span_vertices()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding lit span");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).add_lit_span_vertices();
    }
    return stat;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_blank_span_vertices()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding blank span");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        stat |= at(i).add_blank_span_vertices();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::delete_redundant_frames()
{
    LaserBoy_frame_set reduced(p_space);
    p_space->p_GUI->display_state("omitting frames");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        if(at(i).is_unique)
            reduced.push_back(at(i));
    }
    *this = reduced;
    normalize_cursors();
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::optimize()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    if(number_of_frames())
    {
        u_int            i;
        LaserBoy_vertex  origin;
        LaserBoy_frame   frame(p_space);
        //--------------------------------------------------------------------
        p_space->p_GUI->display_state("optimizing frame set");
        //--------------------------------------------------------------------
        front().minimize(0); // at(0).
        stat |= front().optimize(origin);
        //--------------------------------------------------------------------
        for(i = 1; i < number_of_frames(); i++)
        {
                    at(i    ).minimize(i);
                    at(i - 1).add_coda(at(i    ).front().as_3D_short());
            stat |= at(i    ).optimize(at(i - 1).back().as_3D_short());
            p_space->p_GUI->display_progress(size() - i);
        }
        back().add_coda(origin);
        back().coda += LaserBoy_segment(p_space, back().coda.back(), origin);
    } // end if(number_of_frames())
    return stat;
}

//############################################################################
void LaserBoy_frame_set::rainbow_recolor(int effect)
{
    p_space->p_GUI->display_state("palette recoloring");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).rainbow_recolor(effect);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::quarter_turn(u_int plane, u_int turns)
{
    p_space->p_GUI->display_state("turning");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).quarter_turn(plane, turns);
    }
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::scale_around_origin(LaserBoy_3D_double factor)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("scaling");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).scale_around_origin(factor);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::scale_around_origin()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("scaling");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).scale_around_origin();
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::rotate_around_origin(LaserBoy_3D_double angle)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("rotating");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).rotate_around_origin(angle);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::rotate_around_origin()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("rotating");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).rotate_around_origin();
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::move(LaserBoy_3D_double offset)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("moving");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).move(offset);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::move()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("moving");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        out_of_bounds |= at(i).move();
    }
    return out_of_bounds;
}

//############################################################################
void LaserBoy_frame_set::toggle_frame_select_start()
{
    int i;
    if(frame_select_start == -1) // was unset
    {
        frame_select_start = frame_index; // set
        if(frame_select_end != -1)
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = true;
            at(frame_select_end).is_selected = true;
        }
    }
    else // was set
    {
        if(frame_select_end != -1) // was set
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = false;
            at(frame_select_end).is_selected = false;
        }
        frame_select_start = -1; // unset
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::toggle_frame_select_end()
{
    int i;
    if(frame_select_end == -1) // was unset
    {
        frame_select_end = frame_index; // set
        if(frame_select_start != -1)
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = true;
            at(frame_select_end).is_selected = true;
        }
    }
    else // was set
    {
        if(frame_select_start != -1) // was set
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = false;
            at(frame_select_end).is_selected = false;
        }
        frame_select_end = -1; // unset
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::invert_frame_selections()
{
    for(u_int i = 0; i < number_of_frames(); i++)
        at(i).is_selected = !at(i).is_selected;
    return;
}

//############################################################################
void LaserBoy_frame_set::delete_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame_set reduced(p_space);
        for(u_int i = 0; i < number_of_frames(); i++)
        {
            if(!at(i).is_selected)
                reduced.push_back(at(i));
        }
        if(    frame_select_start != -1
            && frame_select_end   != -1
          )
        {
            frame_index = (frame_select_start - 1);
            frame_select_start = -1;
            frame_select_end   = -1;
        }
        if(frame_index > reduced.number_of_frames())
            frame_index = reduced.number_of_frames() - 1;
        *this = reduced;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::trim_to_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame_set reduced(p_space);
        reduced.reserve(size());
        for(u_int i = 0; i < number_of_frames(); i++)
        {
            if(at(i).is_selected)
                reduced.push_back(at(i));
        }
        if(    frame_select_start != -1
            && frame_select_end   != -1
          )
        {
            frame_select_start = -1;
            frame_select_end   = -1;
        }
        frame_index = 0;
        *this = reduced;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::bit_reduce_to_palette()
{
    p_space->p_GUI->display_state("bit palette reducing");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).bit_reduce_to_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_reduce_to_palette()
{
    p_space->p_GUI->display_state("best palette reducing");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).best_reduce_to_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::promote_to_true_color()
{
    p_space->p_GUI->display_state("promoting to 24-bit");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).promote_to_true_color();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_match_palette(u_int index)
{
    p_space->p_GUI->display_state("best matching palette");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).best_match_palette(index);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_match_target_palette()
{
    p_space->p_GUI->display_state("best matching target");
    if(!p_space->allow_lit_black)
        convert_black_to_blank();
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).best_match_target_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color()
{
    p_space->p_GUI->display_state("stripping color to white");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).strip_color();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color_or()
{
    p_space->p_GUI->display_state("stripping color by logical OR");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).strip_color_or();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color_avg()
{
    p_space->p_GUI->display_state("stripping color by averaging");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).strip_color_avg();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_palette_by_index(u_int index)
{
    p_space->p_GUI->display_state("converting to palette by index");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).to_palette_by_index(index);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_target_palette_by_index()
{
    p_space->p_GUI->display_state("converting to target by index");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).to_target_palette_by_index();
    }
    return;
}

//############################################################################
bool LaserBoy_frame_set::color_from_bmp(const string& file)
{
    char file_name[256];
    struct LaserBoy_bmp bmp;

    strcpy(file_name, file.c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        color_from_bmp(&bmp);
        bmp_free(&bmp);
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_frame_set::color_from_bmp(struct LaserBoy_bmp* bmp)
{
    p_space->p_GUI->display_state("bitmap coloring");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).color_from_bmp(bmp);
    }
    return;
}

//############################################################################
bool LaserBoy_frame_set::subtract_bmp(const string& file)
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
void LaserBoy_frame_set::subtract_bmp(struct LaserBoy_bmp* bmp)
{
    p_space->p_GUI->display_state("bitmap masking");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).subtract_bmp(bmp);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::flip(int plane)
{
    p_space->p_GUI->display_state("flipping frames");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).flip(plane);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::renumber()
{
    u_int i;
    int   local_palette_index = LASERBOY_ILDA_DEFAULT,
          added_palettes      = 0,
          frame_id            = 0;
    bool  in_true_color       = false;
    char  number[8];
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
    {
        if(at(i).size() > 1)
        {
            if(!p_space->dumb_file_format)
            {
                if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                {
                    if(!in_true_color)
                    {
                        frame_id++;
                        added_palettes++; // add LASERBOY_REDUCED_332
                        local_palette_index = LASERBOY_REDUCED_332;
                        in_true_color = true;
                    }
                    frame_id++;
                    added_palettes++; // add true_color
                }
                else if(at(i).palette_index != local_palette_index)
                {
                    frame_id++;
                    added_palettes++;
                    local_palette_index = at(i).palette_index;
                    in_true_color = false;
                }
            }
            at(i).identity = frame_id;
            if(!(at(i).name.size()))
            {
                sprintf(number, "%05d", frame_id);
                at(i).name     = "ESI" + string(number);
            }
            frame_id++;
        }
        else
            delete_frame(i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
        at(i).total = (u_short)(number_of_frames() + added_palettes);
    if(frame_index >= number_of_frames())
        frame_index = number_of_frames() - 1;
    return;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::reverse()
{
    LaserBoy_frame_set reversed(p_space);
    p_space->p_GUI->display_state("reversing frame order");
    for(u_int i = 1; i <= number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        reversed.push_back(at(number_of_frames() - i));
    }
    clear();
    insert(begin(), reversed.begin(), reversed.end());
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::make_current_first()
{
    u_int              i;
    LaserBoy_frame_set new_order(p_space);
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("reordering frames");
    for(i = frame_index; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        new_order += at(i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < frame_index; i++)
    {
        p_space->p_GUI->display_progress(frame_index - i);
        new_order += at(i);
    }
    //------------------------------------------------------------------------
    *this = new_order;
    frame_index = 0;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::swap_frames(int i, int j)
{
    if(    (i >= (int)number_of_frames())
        || (i < 0)
        || (j >= (int)number_of_frames())
        || (j < 0)
      )
        return *this;
    //------------------------------------------------------------------------
    LaserBoy_frame frame = at(i);
    at(i) = at(j);
    at(j) = frame;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::reverse_selected()
{
    u_int              i,
                       j;
    LaserBoy_frame_set selected(p_space);
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("reversing selected frame order");
    for(i = 0; i < number_of_frames(); i++)
        if(at(i).is_selected)
            selected += at(i);
    //------------------------------------------------------------------------
    j = selected.number_of_frames();
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        if(at(i).is_selected)
            at(i) = selected[--j];
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::random_order()
{
    LaserBoy_frame_set random(p_space);
    vector<bool>       been_here(number_of_frames());
    //------------------------------------------------------------------------
    int random_index = rand() % number_of_frames();
    //------------------------------------------------------------------------
    for(u_int i = 0; i < number_of_frames(); i++)
        been_here[i] = false;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("randomizing frame order");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        while(been_here[random_index])
        {
            random_index = rand() % number_of_frames();
        }
        random.push_back(at(random_index));
        been_here[random_index] = true;
    }
    //------------------------------------------------------------------------
    clear();
    insert(begin(), random.begin(), random.end());
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::frame_reverse()
{
    p_space->p_GUI->display_state("reversing frame vectors");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        at(i).reverse();
    }
    return *this;
}

//############################################################################
bool LaserBoy_frame_set::save_as_ild(const string& file)
{
    ofstream out(file.c_str(), ios::out | ios::binary);
    if(out.is_open())
    {
        LaserBoy_frame_set copy(*this);
        if(p_space->auto_minimize)
        {
            copy.minimize();
            copy.renumber();
            copy.p_space->sync_rgb_and_palette();
            copy.to_ofstream_ild(out);
        }
        else
        {
            copy.renumber();
            copy.p_space->sync_rgb_and_palette();
            copy.to_ofstream_ild(out);
        }
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::save_as_txt(const string& file)
{
    ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        if(p_space->auto_minimize)
        {
            LaserBoy_frame_set copy(*this);
            copy.minimize();
            copy.to_ofstream_txt(out);
        }
        else
            to_ofstream_txt(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
void LaserBoy_frame_set::to_ofstream_ild(ofstream& out)
{
    bool                 in_true_color       = false;
    char                 number[8];
    int                  file_section_number = 0,
                         local_palette_index = LASERBOY_ILDA_DEFAULT;
    LaserBoy_ild_header  header;
    //------------------------------------------------------------------------
    header.format = LASERBOY_PALETTE;
    header.total  = front().total;
    header.owner  = "LaserBoy";
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("writing ILDA file");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        if(!p_space->dumb_file_format)
        {
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
            {
                if(!in_true_color)
                {
                    header.name         = p_space->palette_picker(LASERBOY_REDUCED_332).name;
                    header.quantity     = p_space->palette_picker(LASERBOY_REDUCED_332).number_of_colors();
                    header.identity     = file_section_number;
                    p_space->palette_picker(LASERBOY_REDUCED_332).to_ofstream_ild(out, header);
                    local_palette_index = LASERBOY_REDUCED_332;
                    file_section_number++;
                    in_true_color = true;
                }
                file_section_number++;
            }
            else if(at(i).palette_index != local_palette_index)
            {
                header.name         = p_space->palette_picker(at(i).palette_index).name;
                header.quantity     = p_space->palette_picker(at(i).palette_index).number_of_colors();
                header.identity     = file_section_number;
                p_space->palette_picker(at(i).palette_index).to_ofstream_ild(out, header);
                local_palette_index = at(i).palette_index;
                file_section_number++;
                in_true_color = false;
            }
        }
        at(i).owner = "LaserBoy";
        if(!(at(i).name.size()))
        {
            sprintf(number, "%05d", file_section_number);
            at(i).name = "ESI" + string(number);
        }
        at(i).is_2D(); // checks for z and sets format accordingly;
        at(i).to_ofstream_ild(out);
        file_section_number++;
    }
    //------------------------------------------------------------------------
    if(p_space->dumb_file_format)
        header.format = LASERBOY_3D_FRAME_RGB;
    else
        header.format = LASERBOY_3D_FRAME  ;
    header.name     = "ILDA_End"           ;
    header.owner    = p_space->install_GUID;
    header.quantity = 0                    ;
    header.identity = file_section_number  ;
    header.to_ofstream_ild(out)            ;
    return;
}

//############################################################################
void LaserBoy_frame_set::to_ofstream_txt(ofstream& out)
{
    int  local_palette_index = LASERBOY_TRUE_COLOR;
    p_space->p_GUI->display_state("writing text file");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        p_space->p_GUI->display_progress(number_of_frames() - i);
        if(!p_space->save_txt_with_color)
        {
            if(    at(i).palette_index != local_palette_index
                && at(i).palette_index != LASERBOY_TRUE_COLOR
              )
            {
                if(    p_space->save_txt_named_palettes
                    || at(i).palette_index >= LASERBOY_BUILT_IN_PALETTES
                  )
                    p_space->palette_picker(at(i).palette_index).to_ofstream_txt(out);
                else
                {
                    out << "palette named "
                        << p_space->palette_picker(at(i).palette_index).name
                        << ENDL
                        << ENDL;
                }
                local_palette_index = at(i).palette_index;
            }
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                at(i).to_ofstream_txt_color_table(out);
        }
        //--------------------------------------------------------------------
        at(i).to_ofstream_txt(out, i);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame_set::save_as_wave(const string& file, bool optimized, bool timed)
{
    if(number_of_frames() > 1)
    {
        fstream out(file.c_str(), ios::out | ios::binary);
        if(out.is_open())
        {
            to_fstream_wave(out, optimized, timed);
            out.close();
            return true;
        }
        else
            return false;
    }
    else // there is only one frame
        return front().save_as_wave(file, optimized, timed);
}

//############################################################################
void LaserBoy_frame_set::to_fstream_wave(fstream& out, bool optimized, bool timed)
{
    u_int                 i;
    LaserBoy_frame_set    copy(*this);
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
    //------------------------------------------------------------------------
    if(p_space->invert_wave_output)
        header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE;
    //------------------------------------------------------------------------
    header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION; // default values
    header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION;
    header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG;
    header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG;
    header.signal_id [5] = LASERBOY_SIGNAL_Z_POSITION;
    header.signal_id [6] = LASERBOY_SIGNAL_UNDEFINED;
    header.signal_id [7] = LASERBOY_SIGNAL_UNDEFINED;
    //------------------------------------------------------------------------
    header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED; // default values
    header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [2] = LASERBOY_LSB_END_OF_FRAME;
    header.LSB_tag   [3] = LASERBOY_LSB_UNIQUE_FRAME;
    header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;
    //------------------------------------------------------------------------
    header.resolution[0] = 16 - p_space->signal_bit_mask[0];
    header.resolution[1] = 16 - p_space->signal_bit_mask[1];
    header.resolution[2] = 16 - p_space->signal_bit_mask[2];
    header.resolution[3] = 16 - p_space->signal_bit_mask[3];
    header.resolution[4] = 16 - p_space->signal_bit_mask[4];
    header.resolution[5] = 16 - p_space->signal_bit_mask[5];
    header.resolution[6] = 16 - p_space->signal_bit_mask[6];
    header.resolution[7] = 16 - p_space->signal_bit_mask[7];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        for(i = 0; i < 256; i++)
            header.color_rescale_r[i] = p_space->color_rescale_r[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        for(i = 0; i < 256; i++)
            header.color_rescale_g[i] = p_space->color_rescale_g[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        for(i = 0; i < 256; i++)
            header.color_rescale_b[i] = p_space->color_rescale_b[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
        for(i = 0; i < 256; i++)
            header.color_rescale_i[i] = p_space->color_rescale_i[i];
    //------------------------------------------------------------------------
    if(p_space->view_angle != 0.0)
        for(i = 0; i < copy.number_of_frames(); i++)
            copy[i].rotate_around_origin();
    //------------------------------------------------------------------------
    if(is_2D() || p_space->auto_flatten_z)
    {
        header.signal_id [5] = p_space->signal_if_not_z;
        header.resolution[5] = 9;
        copy.flatten_z();
    }
    //------------------------------------------------------------------------
    if(optimized)
    {
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
        copy.optimize();
    }
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("writing wave file");
    //------------------------------------------------------------------------
    header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    if(optimized)
    {
        bool              not_last_frame;
        u_int             chill_samples = (u_int)ceil(   p_space->sample_rate
                                                       * (   p_space->max_dwell_microsec
                                                           / 500000.0 // make it twice the max_dwell_microsec
                                                         )
                                                     );
        LaserBoy_vertex   origin;
        LaserBoy_segment  chill(p_space);
        if(chill_samples < 10)
            chill_samples = 10;
        for(i = 0; i < chill_samples; i++)
            chill.push_back(origin);
        chill.to_fstream_wave(out, header, false, false);
        //--------------------------------------------------------------------
        for(i = 0; i < copy.number_of_frames(); i++)
        {
            not_last_frame = i < (copy.number_of_frames() - 1);
            copy.at(i).intro.to_fstream_wave(out, header, false, false);
            //----------------------------------------------------------------
            if(timed)
            {
                int  frame_scan,
                     total_frame_scans = 1;
                total_frame_scans = (int)ceil(   (   p_space->sample_rate
                                                   * (   1.0
                                                       / p_space->frames_per_second
                                                     )
                                                 )
                                               / (   copy.at(i).size()
                                                   + copy.at(i).bridge.size()
                                                 )
                                             );
                ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, copy.at(i).is_wagged, copy.at(i).is_wagged);
                if(total_frame_scans > 1)
                {
                    for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
                    {
                        copy.at(i).bridge.to_fstream_wave(out, header, true, !copy.at(i).is_wagged && (frame_scan == 1)); // frame scan 1 is unique
                        ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, copy.at(i).is_wagged, false);
                    }
                    copy.at(i).coda.to_fstream_wave(out, header, !copy.at(i).is_wagged && not_last_frame, false);
                }
                else
                    copy.at(i).coda.to_fstream_wave(out, header, not_last_frame, not_last_frame); // only 1 frame scan is unique
            } // end if(p_space->frames_per_second > 0.0)
            else
            {
                ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, false, false);
                copy.at(i).coda.to_fstream_wave(out, header, not_last_frame, not_last_frame); // only 1 frame scan is unique
            }
            //----------------------------------------------------------------
            p_space->p_GUI->display_progress(copy.number_of_frames() - i);
        } // end for(i = 0; i < copy.number_of_frames(); i++)
        chill.to_fstream_wave(out, header, true, true);
    } // end if(optimized)
    //------------------------------------------------------------------------
    else if(timed)
    {
        int  frame_scan,
             total_frame_scans;
        for(i = 0; i < copy.number_of_frames(); i++)
        {
            total_frame_scans = (int)ceil(   (   p_space->sample_rate
                                               * (   1.0
                                                   / p_space->frames_per_second
                                                 )
                                             )
                                           / copy.at(i).size()
                                         );
            ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, true);
            for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
            {
                ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, false);
                p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
            }
            //----------------------------------------------------------------
            p_space->p_GUI->display_progress(copy.number_of_frames() - i);
        } // end for(i = 0; i < copy.number_of_frames(); i++)
    } // end if(optimized)
    //------------------------------------------------------------------------
    else // not optimized or timed!
        for(i = 0; i < copy.number_of_frames(); i++)
        {
            ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, true);
            p_space->p_GUI->display_progress(copy.number_of_frames() - i);
        }
    //------------------------------------------------------------------------
    header.to_fstream_wave(out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame_set::save_as_bmp_directory(const string& dir) const
{
    char name[8];
#ifndef WIN32
    mkdir(dir.c_str(), 0777);
#else
    mkdir(dir.c_str());
#endif
    p_space->p_GUI->display_state("writing bmp file");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        sprintf(name, "%06d", i);
        at(i).save_as_bmp(string(dir + '/' + name + ".bmp"));
        p_space->p_GUI->display_progress(number_of_frames() - i);
    }
    return true;
}

//############################################################################
bool LaserBoy_frame_set::save_as_dxf_directory(const string& dir) const
{
    bool all_saved = true;
    char name[8];
#ifndef WIN32
    mkdir(dir.c_str(), 0777);
#else
    mkdir(dir.c_str());
#endif
    p_space->p_GUI->display_state("writing DXF file");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        sprintf(name, "%06d", i);
        all_saved &= at(i).save_as_dxf(string(dir + '/' + name + ".dxf"));
        p_space->p_GUI->display_progress(number_of_frames() - i);
    }
    return all_saved;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
