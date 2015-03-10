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
// LaserBoy_TUI.cpp is part of LaserBoy.
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
void LaserBoy_TUI::overlay_palettes()
{
    u_int  i,
           j,
           y_line       = 8 * font_size_factor,
           y_off        =   LASERBOY_PALETTE_MAX * font_size_factor
                          + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    if(palette_index != current_frame().palette_index)
        palette_index = current_frame().palette_index;
    //------------------------------------------------------------------------
    if(palette_index != LASERBOY_TRUE_COLOR) // 8 bit frame palette
    {
        for(i = 0; i < palette_picker(palette_index).number_of_colors(); i++)
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off,
                                  i * font_size_factor + j,
                                  x_off + 50,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(palette_index)[i].r,
                                               palette_picker(palette_index)[i].g,
                                               palette_picker(palette_index)[i].b
                                              )
                                 );
    }
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        for(i = 0; i < palette_picker(target_palette_index).number_of_colors(); i++) // target palette
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off + 70,
                                  i * font_size_factor + j,
                                  x_off + 120,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(target_palette_index)[i].r,
                                               palette_picker(target_palette_index)[i].g,
                                               palette_picker(target_palette_index)[i].b
                                              )
                                 );
    //------------------------------------------------------------------------
    if(show_color_cursor) // selected_color_index example line
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
            bmp_put_line_fast(&bmp,
                              x_off + 56,
                              0,
                              x_off + 56,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(palette_index)[selected_color_index].r,
                                           palette_picker(palette_index)[selected_color_index].g,
                                           palette_picker(palette_index)[selected_color_index].b
                                          )
                             );
        //--------------------------------------------------------------------
        if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
            bmp_put_line_fast(&bmp,
                              x_off + 63,
                              0,
                              x_off + 63,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(target_palette_index)[selected_color_index].r,
                                           palette_picker(target_palette_index)[selected_color_index].g,
                                           palette_picker(target_palette_index)[selected_color_index].b
                                          )
                             );
        //--------------------------------------------------------------------
        for(j = 0; j < font_size_factor; j++) // selected color marker
            bmp_put_line_fast(&bmp,
                              x_off + 40,
                              selected_color_index * font_size_factor + j,
                              x_off + 80,
                              selected_color_index * font_size_factor + j,
                              bmp.white
                             );
    }
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR)
        bmp_printf(   &bmp // current frame palette name
                    , x_off
                    , y_off
                    , bmp_bg_color
                    , bmp.white
                    , 0
                    , font_size_factor
                    , "-24-bit-"
                  );
    else
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.white
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , palette_picker(palette_index).name.c_str()
                  );
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , 0
                    , bmp.white
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , palette_picker(target_palette_index).name.c_str()
                  );
    //------------------------------------------------------------------------
    if(show_color_cursor)
    {
        if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        {
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 10
                        , bmp.make_rgb(0,0,255)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "            %3d"
                        , palette_picker(target_palette_index)[selected_color_index].b
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 10
                        , bmp.make_rgb(0,255,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "        %3d"
                        , palette_picker(target_palette_index)[selected_color_index].g
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 10
                        , bmp.make_rgb(255,0,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "    %3d"
                        , palette_picker(target_palette_index)[selected_color_index].r
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 10
                        , bmp.white
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "%3d"
                        , selected_color_index
                      );
        }
        y_off += y_line;
        bmp_printf(   &bmp // selected color blue value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "            %3d"
                    , palette_picker(palette_index)[selected_color_index].b
                  );
        bmp_printf(   &bmp // selected color green value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "        %3d"
                    , palette_picker(palette_index)[selected_color_index].g
                  );
        bmp_printf(   &bmp // selected color red value
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "    %3d"
                    , palette_picker(palette_index)[selected_color_index].r
                  );
        bmp_printf(   &bmp // selected color index
                    , x_off
                    , y_off
                    , bmp.white
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "%3d"
                    , selected_color_index
                  );
    }
}

//############################################################################
void LaserBoy_TUI::render_space()
{
    LaserBoy_color  bit_masked;

    u_int           select_start_color,
                    select_end_color  ,
                    bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    int             x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                    y_off        =   LASERBOY_PALETTE_MAX * font_size_factor
                                   + (LASERBOY_STATS_DISPLAY_OFFSET + 24) * font_size_factor,
                    y_line       = 8 * font_size_factor;

    double          scale        = bmp.yres / 65536.0;
    //------------------------------------------------------------------------
    render_frame(frame_index);
    //------------------------------------------------------------------------
    if(show_frame_stats)
        overlay_frame_stats();
    if(show_frame_set_stats)
        overlay_frame_set_stats();
    if(show_palette || show_target_palette)
        overlay_palettes();
    //------------------------------------------------------------------------
    if(show_stats)
    {
        if(current_frame().is_unique)
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , bmp.make_rgb(0,255,255)
                        , bmp_bg_color
                        , transparent_menu_font
                        , font_size_factor
                        , "vertices %6d"
                        , (int)current_frame().size()
                      );
        else
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , bmp.make_rgb(0,255,255)
                        , bmp.make_rgb(0,127,0)
                        , transparent_menu_font
                        , font_size_factor
                        , "vertices %6d"
                        , (int)current_frame().size()
                      );
        //--------------------------------------------------------------------
        y_off += y_line;
        if(show_Z_as_order)
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , bmp.make_rgb(255,0,255)
                        , bmp.make_rgb(0,0,128)
                        , transparent_menu_font
                        , font_size_factor
                        , "segments %6d"
                        , current_frame().number_of_segments()
                      );
        else
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , bmp.make_rgb(255,0,255)
                        , bmp_bg_color
                        , transparent_menu_font
                        , font_size_factor
                        , "segments %6d"
                        , current_frame().number_of_segments()
                      );
        //--------------------------------------------------------------------
        y_off += y_line;
        if(current_frame().is_2D())
            current_frame().format = LASERBOY_2D_FRAME;
        else
           current_frame().format = LASERBOY_3D_FRAME;
        if(current_frame().format == LASERBOY_2D_FRAME)
        {
            if(current_frame().is_selected)
                bmp_printf(   &bmp
                            , x_off
                            , y_off
                            , bmp.make_rgb(255,255,0)
                            , bmp.make_rgb(127,0,0)
                            , transparent_menu_font
                            , font_size_factor
                            , "2D frame %6d"
                            , frame_index
                          );
            else
                bmp_printf(   &bmp
                            , x_off
                            , y_off
                            , bmp.make_rgb(255,255,0)
                            , bmp_bg_color
                            , transparent_menu_font
                            , font_size_factor
                            , "2D frame %6d"
                            , frame_index
                          );
        }
        else
        {
            //----------------------------------------------------------------
            if(current_frame().is_selected)
                bmp_printf(   &bmp
                            , x_off
                            , y_off
                            , bmp.make_rgb(255,255,0)
                            , bmp.make_rgb(127,0,0)
                            , transparent_menu_font
                            , font_size_factor
                            , "3D frame %6d"
                            , frame_index
                          );
            else
                bmp_printf(   &bmp
                            , x_off
                            , y_off
                            , bmp.make_rgb(255,255,0)
                            , bmp_bg_color
                            , transparent_menu_font
                            , font_size_factor
                            , "3D frame %6d"
                            , frame_index
                          );
        }
        //--------------------------------------------------------------------
    } // end if(show_stats)
    if(current_frame().size() && show_cursors)
    {
        bit_masked = current_frame().at(current_frame().egg).bit_masked_color(signal_bit_mask);
        select_start_color =   current_frame().at(current_frame().egg).is_blank()
                             ? bmp.gray
                             :   current_frame().at(current_frame().egg).is_black(black_level)
                               ? bmp.white
                               : bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
        //--------------------------------------------------------------------
        bit_masked = current_frame().at(current_frame().spider).bit_masked_color(signal_bit_mask);
        select_end_color   =   current_frame().at(current_frame().spider).is_blank()
                             ? bmp.gray
                             :   current_frame().at(current_frame().spider).is_black(black_level)
                               ? bmp.white
                               : bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
        //--------------------------------------------------------------------
        // icons
        y_off += (2 * y_line); // vertex segment color for start cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.white
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "           %3d"
                    , current_frame().at(current_frame().egg).c
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "      %4d"
                    , current_frame().segment_index_of_vertex(current_frame().egg)
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "  %4d"
                    , current_frame().egg
                  );
        //--------------------------------------------------------------------
        y_off += y_line; // RGB values for start cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "            %3d"
                    , current_frame().at(current_frame().egg).b
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "        %3d"
                    , current_frame().at(current_frame().egg).g
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "    %3d"
                    , current_frame().at(current_frame().egg).r
                  );
        //--------------------------------------------------------------------
        y_off += y_line; // color vertex segment for end cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.white
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "           %3d"
                    , current_frame().at(current_frame().spider).c
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "      %4d"
                    , current_frame().segment_index_of_vertex(current_frame().spider)
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "  %4d"
                    , current_frame().spider
                  );
        //--------------------------------------------------------------------
        y_off += y_line; // RGB values for end cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "            %3d"
                    , current_frame().at(current_frame().spider).b
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "        %3d"
                    , current_frame().at(current_frame().spider).g
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "    %3d"
                    , current_frame().at(current_frame().spider).r
                  );
        //--------------------------------------------------------------------
        y_off -= (3 * y_line);
        bmp_put_rectangle(&bmp, x_off
                              , y_off
                              , x_off + (int)(2000 * scale)
                              , y_off + (int)(2000 * scale)
                              , select_start_color
                              , 0
                         ); // start cursor icon
        //--------------------------------------------------------------------
        y_off += (2 * y_line);
        bmp_put_line_fast     (&bmp, x_off + (int)(1000 * scale)
                                   , y_off
                                   , x_off + (int)(1000 * scale)
                                   , y_off + (int)(2000 * scale)
                                   , select_end_color
                              );
        bmp_put_line_fast     (&bmp, x_off + (int)(2000 * scale)
                                   , y_off + (int)(1000 * scale)
                                   , x_off
                                   , y_off + (int)(1000 * scale)
                                   , select_end_color
                              );
        bmp_put_line_fast     (&bmp, x_off
                                   , y_off
                                   , x_off + (int)(2000 * scale)
                                   , y_off + (int)(2000 * scale)
                                   , select_end_color
                              );
        bmp_put_line_fast     (&bmp, x_off + (int)(2000 * scale)
                                   , y_off
                                   , x_off
                                   , y_off + (int)(2000 * scale)
                                   , select_end_color
                              ); // end cursor icon
    //------------------------------------------------------------------------
    } // end if(show_cursors)
    //------------------------------------------------------------------------
    if(show_TUI_clue)
    {
        bmp_printf(&bmp, // what menu?
                   bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   bmp.yres - 16 * font_size_factor,
                   current_menu_name_bc,
                   bmp.white,
                   0, // must be opaque for bc
                   font_size_factor,
                   "%s                                                     ",
                   current_menu_name.c_str()
                  );
        bmp_printf(&bmp,
                   bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   bmp.yres - 8 * font_size_factor,
                   bmp.white,
                   current_menu_name_bc,
                   0,
                   font_size_factor,
                   "%s                                                     ",
                   TUI_clue.c_str()
                  );
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_stats()
{
    u_int i,
          vertices     = (int)current_frame().size(),
          from_vertex  = 0,
          lit          = current_frame().number_of_color_vectors(),
          blank        = current_frame().number_of_dark_vectors(),
          y_line       = 8 * font_size_factor,
          x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
          y_off        = bmp.yres - 15 * y_line, // 15 lines from the bottom
          bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    for(i = 0; i < frame_index; i++)
        from_vertex += (int)frame_picker(i).size();
    //------------------------------------------------------------------------
    TUI_palette_index = 0;

    if(current_frame().size())
    {
        LaserBoy_vertex    at_egg    = current_frame().at(current_frame().egg   ),
                           at_spider = current_frame().at(current_frame().spider),
                           fathest   = current_frame().vertex_farthest_from_origin();

        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "name=%s ownr=%s id=%d scn=%d",
                   current_frame().name.c_str(),
                   current_frame().owner.c_str(),
                   current_frame().identity,
                   current_frame().scanner
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%d vertices from %d to %d",
                   vertices,
                   from_vertex,
                   from_vertex + vertices - 1
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%d lit %-0.2f%%    %d dark %-0.2f%%    %d unq in [%s]",
                   lit,
                   (lit/(double)(vertices - 1)) * 100.0, // a vector takes two vertices!
                   blank,
                   (blank/(double)(vertices - 1)) * 100.0,
                   current_frame().number_of_unique_colors(),
                      (current_frame().palette_index == LASERBOY_TRUE_COLOR)
                     ?("-24-bit-")
                     :(palette_picker(current_frame().palette_index).name.c_str())
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "max %-0.2f    lit %-0.2f    drk %-0.2f",
                   current_frame().max_distance(),
                   current_frame().max_color_distance(),
                   current_frame().max_dark_distance()
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "avg %-0.2f    tot %-0.2f    frthst vert %d %d %d",
                   current_frame().total_distance() / (vertices - 1),
                   current_frame().total_distance(),
                   fathest.x,
                   fathest.y,
                   fathest.z
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "max angle %-0.2f%c    tot angle %-0.2f%c",
                   (current_frame().max_angle() * one_radian),
                   0xf8,
                   (current_frame().total_angle() * one_radian),
                   0xf8
                  );
        if(show_cursors)
        {
/*
            if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
            {
                at_egg.r    = palette_picker(current_frame().palette_index).at(at_egg.c   ).r;
                at_egg.g    = palette_picker(current_frame().palette_index).at(at_egg.c   ).g;
                at_egg.b    = palette_picker(current_frame().palette_index).at(at_egg.c   ).b;
                at_spider.r = palette_picker(current_frame().palette_index).at(at_spider.c).r;
                at_spider.g = palette_picker(current_frame().palette_index).at(at_spider.c).g;
                at_spider.b = palette_picker(current_frame().palette_index).at(at_spider.c).b;
            }
*/
            at_egg    = at_egg.bit_masked(signal_bit_mask);
            at_spider = at_spider.bit_masked(signal_bit_mask);
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%c %s i=%-6d X=%-7d Y=%-7d Z=%-7d r=%-3d g=%-3d b=%-3d",
                       0x01,
                       (at_egg.is_lit()) ? ("lit  ") : ("blank"),
                       current_frame().egg,
                       at_egg.x,
                       at_egg.y,
                       at_egg.z,
                       at_egg.r,
                       at_egg.g,
                       at_egg.b
                      );
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%c %s i=%-6d X=%-7d Y=%-7d Z=%-7d r=%-3d g=%-3d b=%-3d",
                       0x0f,
                       (at_spider.is_lit()) ? ("lit  ") : ("blank"),
                       current_frame().spider,
                       at_spider.x,
                       at_spider.y,
                       at_spider.z,
                       at_spider.r,
                       at_spider.g,
                       at_spider.b
                      );
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%c-%c %d cntr X=%-0.2f Y=%-0.2f Z=%-0.2f diff=%-0.2f",
                       0x01,
                       0x0f,
                       (int)current_frame().selected_segment().size(),
                       current_frame().selected_segment().mean_of_coordinates().x,
                       current_frame().selected_segment().mean_of_coordinates().y,
                       current_frame().selected_segment().mean_of_coordinates().z,
                       current_frame().at(current_frame().egg) | current_frame().at(current_frame().spider)
                      );
        }
        if(show_fulcrum)
        {
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "fulcrum X=%-0.2f Y=%-0.2f Z=%-0.2f",
                       fulcrum.x,
                       fulcrum.y,
                       fulcrum.z
                      );
        }
    }
    else
    {
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "name: %s ownr: %s id: %d scn: %d",
                   current_frame().name.c_str(),
                   current_frame().owner.c_str(),
                   current_frame().identity,
                   current_frame().scanner
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "This frame has no vertices!"
                  );
    }
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_set_stats()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = bmp.yres - 8 * y_line;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    count_frames();
    TUI_palette_index = 64;
    bmp_printf(&bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "frame set stats"
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "frames    %d",
               number_of_frames()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "2D frames %d",
               num_2D_frames
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "3D frames %d",
               num_3D_frames
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "verticies %d",
               total_vertices()
              );
//    y_off += y_line;
//    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
//               "GUID %s",
//               ild_file_GUID.c_str()
//              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_palette_set_stats()
{
    u_int  i,
           y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    TUI_palette_index = 128;
    bmp_printf(&bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "                  "
              );
    bmp_printf(&bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "palette set stats "
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "total palettes %d ",
               number_of_palettes()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "added palettes %d",
               number_of_palettes() - LASERBOY_BUILT_IN_PALETTES
              );
    for(i = 0; i < number_of_palettes(); i++)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%s          ",
                   LaserBoy_palette_set::at(i).name.c_str()
                  );
    }
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_program_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    TUI_palette_index = 192;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "Welcome to " LASERBOY_VERSION " !!!"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    i o file           input | output"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ` ~ show         forward | reverse"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    - _ orbit        forward | reverse"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    = + show & orbit forward | reverse"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    %c %c frame       previous | next"
                , 0x1b
                , 0x1a
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Space] current frame select | unselect"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ( ) set | unset frames-( | )-frames select"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ? / show | hide    frame | frame set stats"
              );
    if(bg_file_name != ".")
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "      ; background      show | hide bitmap"
                  );
    }
    y_off += y_line;
    if(number_of_frames() >= 100)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    z Z frame           +100 | -100"
                  );
    }
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 0 go to          first | last frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      y go to frame at index"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      k draw, color, blank  vertices"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      l move, scale, rotate segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      m render segments by  coordinates"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      p palette   transforms"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      h frame     transforms"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      j frame set transforms"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      b wave file utilities"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      t signal bit resolution"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      v change and apply view menu (active)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      u user interface visuals"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      x system switch settings"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Tab] system value  settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Bkspc] omit current frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] this menu       show | hide"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit LaserBoy"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_file_type_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 83;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "file input type"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wav"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 txt"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 bmp"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 wtf"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_ild_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 48;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "ild file input action"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 prepend to frame set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 insert after current frame"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append to frame set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 superimpose frame(s) into frame set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_dxf_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 96;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "dxf file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame set with dxf file"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 append  frame set with dxf file"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 replace frame set with dxf directory"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append  frame set with dxf directory"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_wav_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 144;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace current frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 append to current frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 open as unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "unformatted wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 replace current frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 append to current frame set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 reframe based on blanking"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_txt_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 240;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "txt file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 prepend to frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 insert after current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append to frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 superimpose frame(s) into frame set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 import palettes only"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_bmp_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 32;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "bitmap file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 open as frame color map"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 open as frame set color map"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 open as frame mask"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 open as frame set mask"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 open as display background"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 clear display background bitmap"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_if_not_z_options()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 16;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "signal option if not Z"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 nothing"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 monochrome TTL (on or off)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 monochrome simple average of intensity"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 monochrome bitwise OR of intensity"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 monochrome weighted rgb gray scale"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 monochrome weighted gray for o-scope"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_dots_options()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 220;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "what to do with dots"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ignore dots"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 remove dots"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 enhance dots by dwell of %d"
                , dwell_on_dot
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_file_type_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 50;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "output file type"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wav"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 txt"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 bmp"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 wtf"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_ild_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 150;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "laser art output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 ild save selected frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 ild save all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 ild generate & save frame effect"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 ild generate & save frame set effect"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_dxf_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 86;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "CAD output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 dxf save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf save directory selected frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 dxf save directory all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_wav_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 72;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "unoptimized"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 wave of current frame once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 wave of current frame for %.2f seconds"
                , still_frame_duration
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wave of all frames once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 wave of all frames at %.3f fps"
                , frames_per_second
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "optimized"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 wave of current frame once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 wave still of current frame for %.2f seconds"
                , still_frame_duration
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 wave animation of each frame shown once"
                , frames_per_second
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 wave animation at %.3f fps of all frames"
                , frames_per_second
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_txt_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 133;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "plain ASCII text output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 save selected frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 save all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 save current frame palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 save target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 save LaserBoy wave color rescales"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 save system settings (wtf)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_bmp_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 166;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "bitmap output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 bmp save %d%c pxl of current frame"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 bmp save directory %d%c pxl selected frames"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 bmp save directory %d%c pxl all frames"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_value_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "value settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 insignificant angle in degrees ....... %3.2lf%c"
                , insignificant_angle * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 insignificant distance in points ..... %3.2lf"
                , insignificant_distance
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 max optimized lit vector in points ... %3.2lf"
                , lit_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 max optimized blank vector in points . %3.2lf"
                , blank_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 maximum dwell in microseconds ........ %3.2lf"
                , max_dwell_microsec
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 lit dwell overhang in vertices ....... %d"
                , lit_dwell_overhang
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 what to do with dots ................. %s"
                , LaserBoy_dots_setting_id_to_name(dots_setting).c_str()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 dwell on dot in vertices ............. %d"
                , dwell_on_dot
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 max time in seconds for wagging ...... %1.4f"
                , (max_wag_microsec / 1000000.0)
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 move points per key hit .............. %d"
                , displacement_step
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a rotate degrees per key hit ........... %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b scale %% per key hit .................. %3.2f%%"
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c rendered arc step in degrees ......... %3.2f%c"
                , rendered_arc_angle
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d rendered mono font field space ....... %3.2f%%"
                , rendered_mono_font_space * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    e rendered variable font gaps .......... %3.2f%%"
                , rendered_vari_font_gap * 100.00
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    f frames per second .................... %3.2lf"
                , frames_per_second
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    g still frame duration in seconds ...... %3.2lf"
                , still_frame_duration
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    h output wave sample rate .............. %d"
                , sample_rate
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    i unframed wave saples per frame ....... %d"
                , unframed_wave_stride
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j wave signal if not z ................. %s"
                , LaserBoy_wave_signal_id_to_name(signal_if_not_z).c_str()
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    k output bitmap size in square pixels .. %d"
                , output_bmp_size
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    l menu font size factor ................ %d"
                , font_size_factor
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m background bitmap size factor ........ %d"
                , background_bmp_scale
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.make_rgb(0,0,255), bmp_bg_color, transparent_menu_font, font_size_factor
                , "                                                       %3d"
                , bg_color.b
              );
    bmp_printf(   &bmp, x_off, y_off, bmp.make_rgb(0,255,0), bmp_bg_color, transparent_menu_font, font_size_factor
                , "                                                  %3d"
                , bg_color.g
              );
    bmp_printf(   &bmp, x_off, y_off, bmp.make_rgb(255,0,0), bmp_bg_color, transparent_menu_font, font_size_factor
                , "                                             %3d"
                , bg_color.r
              );
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    n display background color ............."
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    o display background gray scale ........ %d"
                , (bg_color.r + bg_color.g + bg_color.b) / 3
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    p onion skin fade ...................... %3.2lf%%"
                , onion_skin_fade * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    q onion skin depth ..................... %d"
                , number_of_skins
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    r recolor palette span factor .......... %3.2lf"
                , recolor_span_factor
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    s black limit for color correction  .... %d"
                , black_level
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit value settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_veiw_menu()
{
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off,
           y_line       =  8 * font_size_factor,
           x_tab        = 26 * y_line,
           x_tab2       =  9 * y_line;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.white
                , bmp_bg_color
                , 0
                , font_size_factor
                , "press & hold view keys, [Shift] reverse"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "1 ! orbit X   %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "5 %% front | back"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "2 @ orbit  Y  %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "6 ^  left | right"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "3 # orbit   Z %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "7 &   top | bottom"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "4 $ orbit XYZ %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "8 * orbit & frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "q Q pan X  %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "a A zoom X  +|-%3.2f%%"
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "w W pan  Y %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "s S zoom  Y +|-%3.2f%%"
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "e E pan XY %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "d D zoom XY +|-%3.2f%%"
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "r R center"
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "f F 1:1"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.white
                , bmp_bg_color
                , 0
                , font_size_factor
                , "current view"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,0,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "X"
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "Y"
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp.make_rgb(0,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "Z"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , " angle                              degrees"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.x * one_radian
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.y * one_radian
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.z * one_radian
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "offset                              points"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-6d"
                , (int)view_offset.x
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-6d"
                , (int)view_offset.y
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp_bg_color
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "      "
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , " scale"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_scale.x
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_scale.y
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp_bg_color
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "      "
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "      o O apply angle  to frame | all"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "      t T apply offset to frame | all"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "      g G apply scale  to frame | all"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.white
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  [Esc] exit view & apply"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_visuals_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 80;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "user interface visuals settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_vertices)
                    ? (" --> 1 vertices")
                    : ("     1 vertices")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_blanking)
                    ? (" --> 2 blanking")
                    : ("     2 blanking")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_intro)
                    ? (" --> 3 intro")
                    : ("     3 intro")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_bridge)
                    ? (" --> 4 bridge")
                    : ("     4 bridge")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_coda)
                    ? (" --> 5 coda")
                    : ("     5 coda")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_indices)
                    ? (" --> 6 indices")
                    : ("     6 indices")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_palette)
                    ? (" --> 7 palette")
                    : ("     7 palette")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_target_palette)
                    ? (" --> 8 target palette")
                    : ("     8 target palette")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_stats)
                    ? (" --> 9 stats")
                    : ("     9 stats")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (transparent_menu_font)
                    ? (" --> 0 transparent menu font")
                    : ("     0 transparent menu font")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_fixed_bounds)
                    ? (" --> a fixed bounds")
                    : ("     a fixed bounds")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_fixed_origin)
                    ? (" --> b fixed origin")
                    : ("     b fixed origin")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_floating_axis)
                    ? (" --> c floating axis")
                    : ("     c floating axis")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_floating_bounds)
                    ? (" --> d floating bounds")
                    : ("     d floating bounds")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_onion_skin)
                    ? (" --> e onion skin")
                    : ("     e onion skin")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_onion_visuals)
                    ? (" --> f onion visuals")
                    : ("     f onion visuals")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_Z_as_order)
                    ? (" --> g display vector order in Z")
                    : ("     g display vector order in Z")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_quad_view)
                    ? (" --> h quad orthographic view")
                    : ("     h quad orthographic view")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_TUI_clue)
                    ? (" --> i TUI clue")
                    : ("     i TUI clue")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     j show | hide all visuals"
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit user interface visuals settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_switch_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 190;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "switch settings"
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "animation"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (loop_animation)
                    ? (" --> 1 loop animation")
                    : ("     1 loop animation")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (approximate_frame_rate)
                    ? (" --> 2 animate at approximated frame rate")
                    : ("     2 animate at approximated frame rate")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_effects_generation)
                    ? (" --> 3 show effect rendering")
                    : ("     3 show effect rendering")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "input"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_scale_real)
                    ? (" --> 4 auto scale real coordinate import")
                    : ("     4 auto scale real coordinate import")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (maintain_real_origin)
                    ? (" --> 5 maintain real coordinate origin")
                    : ("     5 maintain real coordinate origin")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (z_from_unformatted)
                    ? (" --> 6 import Z from unformatted wave")
                    : ("     6 import Z from unformatted wave")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "optimization"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (no_equivalent_vectors)
                    ? (" --> 7 remove equal vectors (slow)")
                    : ("     7 remove equal vectors (slow)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (conglomerate_in_minimize)
                    ? (" --> 8 conglomerate in minimize (slower)")
                    : ("     8 conglomerate in minimize (slower)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (fracture_b4_conglomerate)
                    ? (" --> 9 fracture conglomerate (slowest)")
                    : ("     9 fracture conglomerate (slowest)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (black_dwell_vertices)
                    ? (" --> 0 black dwell")
                    : ("     0 black dwell")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "color"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (no_black_match)
                    ? (" --> a no black palette match")
                    : ("     a no black palette match")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (allow_lit_black)
                    ? (" --> b allow lit black")
                    : ("     b allow lit black")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "output"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_minimize)
                    ? (" --> c minimize ild, dxf, txt output")
                    : ("     c minimize ild, dxf, txt output")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (dumb_file_format)
                    ? (" --> d save ild format 4 or 5")
                    : ("     d save ild format 4 or 5")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_true_color_dxf)
                    ? (" --> e save dxf in true color (420)")
                    : ("     e save dxf in true color (420)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_flatten_z)
                    ? (" --> f flatten Z for wave output")
                    : ("     f flatten Z for wave output")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (invert_wave_output)
                    ? (" --> g invert wave output")
                    : ("     g invert wave output")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_apply_offsets)
                    ? (" --> h apply system offsets to wave")
                    : ("     h apply system offsets to wave")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_apply_rescales)
                    ? (" --> i apply system color rescales to wave")
                    : ("     i apply system color rescales to wave")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (channels_of_wav_out == 8)
                    ? (" --> j save waves with stereo audio silence (8ch)")
                    : ("     j save waves with stereo audio silence (8ch)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_with_color)
                    ? (" --> k save txt frames with integrated color")
                    : ("     k save txt frames with integrated color")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_unit)
                    ? (" --> l save txt in unit coordinates (-1.0 to +1.0)")
                    : ("     l save txt in unit coordinates (-1.0 to +1.0)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_color_hex)
                    ? (" --> m save txt colors as hex")
                    : ("     m save txt colors as hex")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_named_palettes)
                    ? (" --> n save txt named LaserBoy palette values")
                    : ("     n save txt named LaserBoy palette values")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit switch settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_palette_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 45;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "palette transforms (to frames)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  1 2 3 span    X | Y | Z"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 5 span radially | angular from origin"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 7 span | index    vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 9 span | index    segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 a random color    vectors | segments"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b c to target by     best match | index"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  d e f strip color    white | logical OR | gray scale"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  g h i 24-bit to palette    lossless | bits 332 | best"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      j promote to 24-bit"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Shift] key with above applies to all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "palette transforms (to palettes and the set)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    %c %c +|- target palette"
                , 0x18
                , 0x19
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      . set target to current frame's palette"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    p P +|- selected color index"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      k best reduce all frames to 1 palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      l omit unused added palettes"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      m add new target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      n omit target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      o copy target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      q add new color to target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      r set selected color RGB in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      s insert color RGB before selected in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      t omit selected color in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      u omit selected color to end of target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    v V linear | circular    blend up to 7 colors in target"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit palette transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_set_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 170;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame set transforms"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      1 invert frame selections"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      2 omit selected frame(s)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      3 trim to selected frame(s)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      4 omit repeats from formatted wave import"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      5 copy selected frame(s) to begining"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      6 copy selected frame(s) to end"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      7 move selected frame(s) to begining"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      8 move selected frame(s) to end"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      9 add empty frame to begining"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      0 add empty frame after current"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      a add empty frame to end"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      b reverse frame set order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      c reverse order of selected frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      d make current frame first"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      e randomize frame set order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      f omit every n-th frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      g omit all but every n-th frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      > explode frame into segment frame set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      < collapse selected frames into one"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      ! explode all frames into segment frame sets"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Space] select | unselect frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit frame set transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 57;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame transforms"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 2 flatten Z | Z order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 4 mirror | flip"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 6 quarter turn CW | CCW"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      7 reverse vector order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      8 impose bit resolution"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      9 black to blank"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      0 blank to black"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      a impose black level"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      b convert to dots"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      c remove blank span"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      d remove angle dwell"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      e remove dots (not lines)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      f fit to insignificant distace %-6.2f"
                , insignificant_distance
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      g reduce lit vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      h remove equivalent vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      i conglomerate segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      j bond segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      k minimize"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      l fracture segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      m randomize segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      n reorder segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      o add angle dwell"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      p add lit distance span"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      q add blank distance span"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      r enhance dots"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      s optimize"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      t rename frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Shift] key applies to all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit frame transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_draw_color_blank_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 22;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "draw, color, blank"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [{ ]} +|- vertex  %c | %c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   iI oO +|- segment %c | %c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    u  U +|- segment %c-%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    \\  |           %c=0 | %c=end"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    '  \" place %c | %c at index"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "xX yY zZ move      %c X | Y | Z  +|-%d"
                , 0x0f
                , displacement_step
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " q  w  e set       %c X | Y | Z"
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   fF gG rotate    %c Z | Y axis +|-%3.2f%c"
                , 0x0f
                , rotation_step * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m  M magnitude %c            +|-%3.2f%%"
                , 0x0f
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j  J assign    %c=%c | %c=%c"
                , 0x0f
                , 0x01
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     a A unblank all vectors | frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     b B blank | unblank %c-%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     n N bond black | unbond unblack %c-%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     p P +|- selected color"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     c C set %c-%c to selected color | get %c color"
                , 0x01
                , 0x0f
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     - _ set %c-%c to palette by index | span"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     h H +|- cycle colors in %c-%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       ~ set black in %c-%c to selected color"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     = + set black in %c-%c to palette by index | span"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       l draw line between %c-%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       t convert %c-%c to dots"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     , < make %c | %c first vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     . > add vector after %c | add blank at %c"
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Bkspc] omit %c"
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    if(palette_index == LASERBOY_TRUE_COLOR)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    %c %c +|- target palette"
                    , 0x18
                    , 0x19
                  );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       ` copy frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Esc] exit draw, color, blank"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_move_scale_rotate_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 47;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "move, scale, rotate mode entered"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [{ ]}         %c | %c +|- vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   iI oO         %c | %c +|- segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    u  U         %c-%c   +|- segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    \\  |         %c=0 | %c=end"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    '  \" place %c | %c at index"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "xX yY zZ move    %c-%c X | Y | Z             +|-%d"
                , 0x01
                , 0x0f
                , displacement_step
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " q  w  e set     %c-%c X | Y | Z     center"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   fF gG rotate  %c-%c Z | Y axis on center  +|-%3.2f%c"
                , 0x01
                , 0x0f
                , rotation_step * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "9( 0) -_ scale   %c-%c X | Y | Z  on center  +|-%3.2f%%"
                , 0x01
                , 0x0f
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    =  + scale   %c-%c    XYZ     on center  +|-%3.2f%%"
                , 0x01
                , 0x0f
                , magnitude_step * 100.00
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "aA sS dD move fulcrum X | Y | Z +|-%d"
                , displacement_step
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " j  k  l set  fulcrum X | Y | Z"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ,  . move fulcrum  to %c | %c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   rR tT rotate  %c-%c Z | Y axis on fulcrum +|-%3.2f%c"
                , 0x01
                , 0x0f
                , rotation_step * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "vV bB nN scale   %c-%c X | Y | Z  on fulcrum +|-%3.2f%%"
                , 0x01
                , 0x0f
                , magnitude_step * 100.00
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m  M scale   %c-%c    XYZ     on fulcrum +|-%3.2f%%"
                , 0x01
                , 0x0f
                , magnitude_step * 100.00
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       < reverse %c-%c vector order"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       c copy    %c-%c (in place) to end"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       ` copy frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Esc] exit move, scale, rotate mode"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_direct_draw_segments_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 100;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "direct draw segments mode entered"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  3D"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a point"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b line"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  2D"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c rectangle"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d polygon"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    e polyline"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    f polystar"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    g circular arc"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    h elliptical arc"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  wiki the following curves!"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    i rhodonea"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j epicycloid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    k epitrochoid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    l hypocycloid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m hypotrochoid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    n Lissajous curve"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  2D text"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    o mono spaced font"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    q variable spaced font"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    p P +|- selected color"
              );
    if(palette_index == LASERBOY_TRUE_COLOR)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "   %c %c +|- target palette"
                    , 0x18
                    , 0x19
                  );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      ` copy frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit direct draw segments mode"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_utilities_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 222;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave file utilities"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 wave show screen settings"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 show wave on screen"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 set system wave sample offsets"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 apply offsets to wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 overwrite offsets in LaserBoy header"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 set color rescale files"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 apply color rescales to wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 omit color rescale tables in LaserBoy header"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 save color rescale tables to files"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 impose 8-bit color and black level of %d"
                , black_level
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a impose bit resolution on wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b split or join wave channels"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c invert signal polarities of LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d clear signal polarity list in LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "e | E invert polarity of LaserBoy | unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "f | F show LaserBoy | unformatted header information"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "g | G find max vector in LaserBoy | unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit wave file utilities"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_show_split_or_join_waves_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 233;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "split or join waves options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 split wave into XY, r, g, b, i, LR"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 split wave into XY, rg, b (i or Z), LR"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 add stereo audio to wave (6 to 8)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted waves"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 join XY, r, g, b, i or Z, waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 join XY, rg, b[Z], waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 join XY, rg, b[Z], LR waves to 8 channel wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "unformatted waves"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 join XY, r, g, b, i or Z waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 join XY, rg, b[Z], waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 join XY, rg, b[Z], LR waves to 8 channel wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit split or join waves options"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_show_wave_options_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 111;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "show wave options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 show LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 show LaserBoy wave as unformatted"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 show unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit show wave options"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_show_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 37;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave show settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_vertices)?(" --> 1 vertices"):("     1 vertices"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_blanks)?(" --> 2 blanking"):("     2 blanking"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_real_time)?(" --> 3 real time"):("     3 real time"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_inverted)?(" --> 4 inverted"):("     4 inverted"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_clean_screen)?(" --> 5 clean screen"):("     5 clean screen"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_loop)?(" --> 6 loop"):("     6 loop"))
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit wave show settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_TUI::overlay_wave_analysis(string file_name)
{
    fstream in;
    in.open(file_name.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        u_int     //i,
                  y_line        = 8 * font_size_factor,
                  x_off         = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                  y_off         = x_off,
                  sample_count  = 0,
                  max_diff_x    = 0,
                  max_diff_y    = 0,
                  max_x_index   = 0,
                  max_y_index   = 0,
                  bmp_bg_color  = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
//        vector<int>             red_map(1024),
//                                green_map(1024),
//                                blue_map(1024);
//        for(u_int i = 0; i < 1024; i++)
//            red_map[i] = green_map[i] = blue_map[i] = 0;
        LaserBoy_wave_header header;
        header.from_fstream_wave(in);
        LaserBoy_wave_sample sample_0(header.num_channels),
                             sample_1(header.num_channels);
        //--------------------------------------------------------------------
        p_GUI->display_state("analyzing wave");
        while(sample_1.from_fstream_wave(in))
        {
            if((u_int)(abs(sample_1.channel[0] - sample_0.channel[0])) > max_diff_x)
            {
                max_diff_x = abs(sample_1.channel[0] - sample_0.channel[0]);
                max_x_index = sample_count;
            }
            if((u_int)(abs(sample_1.channel[1] - sample_0.channel[1])) > max_diff_y)
            {
                max_diff_y = abs(sample_1.channel[1] - sample_0.channel[1]);
                max_y_index = sample_count;
            }
//            ++(red_map[(sample_1.channel[2] + LASERBOY_MAX_SHORT) / 64]);
//            ++(green_map[(sample_1.channel[3] + LASERBOY_MAX_SHORT) / 64]);
//            ++(blue_map[(sample_1.channel[4] + LASERBOY_MAX_SHORT) / 64]);
            sample_0 = sample_1;
            sample_count++;
            if(!(sample_count % 4800))
                p_GUI->display_progress(header.num_samples - sample_count);
        }
        in.close();
        render_space();
        //--------------------------------------------------------------------
        TUI_palette_index = 14;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                    , "wave analysis"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , file_name.c_str()
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "num_samples .............: %u"
                    , header.num_samples
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "sample_rate .............: %u"
                    , header.sample_rate
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "number of channels ......: %d"
                    , header.num_channels
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "bits per sample .........: %d"
                    , header.bits_per_sample
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "playing time ............: %s hh:mm:ss.ss"
                    , time_as_string((double)header.num_samples / header.sample_rate).c_str()
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "maximum single step in X : %d"
                    , max_diff_x
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "at sample ...............: %d"
                    , max_x_index
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "maximum single step in Y : %d"
                    , max_diff_y
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "at sample ...............: %d"
                    , max_y_index
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                    , "[Esc] to continue"
                  );
        //--------------------------------------------------------------------
/*
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(green_map[i] > max_diff_x)
                max_diff_x = green_map[i];

        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((green_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(0,255,0)
                             );
        //--------------------------------------------------------------------
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(blue_map[i] > max_diff_x)
                max_diff_x = blue_map[i];

        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((blue_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(0,0,255)
                             );
        //--------------------------------------------------------------------
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(red_map[i] > max_diff_x)
                max_diff_x = red_map[i];
        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((red_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(255,0,0)
                             );
        //--------------------------------------------------------------------
*/
        return true;
    }
    else
        return false;
}

//############################################################################
bool LaserBoy_TUI::overlay_wave_header_stats(string file_name)
{
    int       i,
              y_line       = 8 * font_size_factor,
              x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
              y_off        = x_off;
    u_int     bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    fstream   in;
    LaserBoy_wave_header header;
    //------------------------------------------------------------------------
    in.open(file_name.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        header.from_fstream_wave(in);
        in.close();
    }
    else
        return false;
    //------------------------------------------------------------------------
    TUI_palette_index = 19;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave stats"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , file_name.c_str()
              );
    y_off += y_line;
    y_off += y_line;
    if(header.version == "!LaserBoy!")
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "wave has no LaserBoy format information"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "LaserBoy wave header version : %s"
                    , header.version.c_str()
                  );
    if(!(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED))
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                    , "no optimization information"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "num_samples ..........: %u"
                , header.num_samples
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "sample_rate ..........: %u"
                , header.sample_rate
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "number of channels ...: %d"
                , header.num_channels
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "bits per sample ......: %d"
                , header.bits_per_sample
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "playing time .........: %s hh:mm:ss.ss"
                , time_as_string((double)header.num_samples / header.sample_rate).c_str()
              );
    if(header.version != "!LaserBoy!")
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , ((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) ? ("wave polarity ........: positive signal") : ("wave polarity ........: negative signal"))
                  );
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "lit dwell overhang ...: %d"
                        , header.parms.lit_dwell_overhang
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "lit distance delta max: %6.2f"
                        , header.parms.lit_delta_max
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "blank dist delta max .: %6.2f"
                        , header.parms.blank_delta_max
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "max dwell u_sec ......: %6.2f"
                        , header.parms.max_dwell_microsec
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "max scan wag sec .....: %1.4f"
                        , (header.parms.max_wag_microsec / 1000000.0)
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "insignificant distance: %6.2f"
                        , header.parms.insignificant_distance
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "insignificant angle ..: %6.2f"
                        , header.parms.insignificant_angle
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "frames per second ....: %6.2f"
                        , header.parms.frames_per_second
                      );
        }
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "number of frames .....: %d"
                        , header.num_frames
                      );
        }
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                    , "ch: +/- :  contents     : LSB subcode  : offset : bit"
                  );
        for(i = 0; i < header.num_channels; i++)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "%2d: %s : %c%s : %s :   %4d : %3d"
                        , i
                        , ((header.signal_id[i] >= 0) ? ("   ") : ("neg"))
                        , (  (    (i == 2 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                               || (i == 3 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                               || (i == 4 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                               || (i == 5 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                             )
                           ? ('*')
                           : (' ')
                          )
                        , LaserBoy_wave_signal_id_to_name(header.signal_id[i]).c_str()
                        , LaserBoy_wave_LSB_tag_to_name(header.LSB_tag[i]).c_str()
                        , header.offset[i]
                        , header.resolution[i]
                      );
        }
    }
    if(    (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
      )
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                    , "* color signals are rescaled"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] to continue"
              );
    return true;
}

//############################################################################
short LaserBoy_TUI::overlay_wave_signal_polarities(string file_name, int polarity[8])
{
    int            i,
                   y_line       = 8 * font_size_factor,
                   x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   y_off        = x_off;
    u_int          bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    fstream        in;
    //------------------------------------------------------------------------
    in.open(file_name.c_str(), ios::in | ios::binary);
    LaserBoy_wave_header header(in);
    in.close();
    //------------------------------------------------------------------------
    TUI_palette_index = 61;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave stats                                          "
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , file_name.c_str()
              );
    if(header.version != "!LaserBoy!")
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , ((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) ? ("wave polarity ........: positive signal") : ("wave polarity ........: negative signal"))
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "ch: sig : contents"
              );
    for(i = 0; i < header.num_channels; i++)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "%2d: %s : %s"
                    , i
                    , ((header.signal_id[i] >= 0) ? ("   ") : ("neg"))
                    , LaserBoy_wave_signal_id_to_name(header.signal_id[i]).c_str()
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave signal polarity matrix"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [0] X          %s"
                , ((polarity[0] == -1) ? ("") : ((polarity[0]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [1] Y          %s"
                , ((polarity[1] == -1) ? ("") : ((polarity[1]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [2] red        %s"
                , ((polarity[2] == -1) ? ("") : ((polarity[2]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [3] green      %s"
                , ((polarity[3] == -1) ? ("") : ((polarity[3]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [4] blue       %s"
                , ((polarity[4] == -1) ? ("") : ((polarity[4]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [5] Z or other %s"
                , ((polarity[5] == -1) ? ("") : ((polarity[5]) ? ("invert") : ("unchanged")))
              );
    if(header.num_channels == 8)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  [6] L audio    %s"
                    , ((polarity[6] == -1) ? ("") : ((polarity[6]) ? ("invert") : ("unchanged")))
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  [7] R audio    %s"
                    , ((polarity[7] == -1) ? ("") : ((polarity[7]) ? ("invert") : ("unchanged")))
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    return header.num_channels;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_offsets()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 119;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave sample offsets"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [0] X          %d"
                , wave_offsets[0]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [1] Y          %d"
                , wave_offsets[1]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [2] red        %d"
                , wave_offsets[2]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [3] green      %d"
                , wave_offsets[3]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [4] blue       %d"
                , wave_offsets[4]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [5] Z or other %d"
                , wave_offsets[5]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [6] L audio    %d"
                , wave_offsets[6]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [7] R audio    %d"
                , wave_offsets[7]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_color_rescale_stats()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 43;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "color rescales"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        red      %s"
                , ((color_rescale_file[0] == ".") ? ("(no file)") : (color_rescale_file[0].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        green    %s"
                , ((color_rescale_file[1] == ".") ? ("(no file)") : (color_rescale_file[1].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        blue     %s"
                , ((color_rescale_file[2] == ".") ? ("(no file)") : (color_rescale_file[2].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        auxilary %s"
                , ((color_rescale_file[3] == ".") ? ("(no file)") : (color_rescale_file[3].c_str()))
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] to skip"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "      . to clear"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_bit_resolution_settings()
{
    u_int  y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 67;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "signal bit resolution settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [0] X       %d"
                , 16 - signal_bit_mask[0]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [1] Y       %d"
                , 16 - signal_bit_mask[1]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [5] Z       %d"
                , 16 - signal_bit_mask[5]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [2] red     %d"
                , 16 - signal_bit_mask[2]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [3] green   %d"
                , 16 - signal_bit_mask[3]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [4] blue    %d"
                , 16 - signal_bit_mask[4]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [6] L audio %d"
                , 16 - signal_bit_mask[6]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [7] R audio %d"
                , 16 - signal_bit_mask[7]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::get_directory_list(string dir, string ext)
{
    working_directory = dir;
    current_extension = ext;
    working_directory_list.clear();
    //------------------------------------------------------------------------
    if(!exists(dir))
        return;
    directory_iterator end_itr;
    for(directory_iterator itr(dir); itr != end_itr; itr++)
        working_directory_list.push_back(*itr);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
int LaserBoy_TUI::overlay_file_list(string start)
{
    u_int  i,
           y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = y_line,
           files_shown  = 0,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 7;
    bmp_put_rectangle(&bmp, x_off, 0, bmp.xres, bmp.xres, bmp_bg_color, true);
    bmp_printf(&bmp, x_off, 0, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "%s                  ",
               working_directory.c_str()
              );
    //------------------------------------------------------------------------
    for(i = 0; i < working_directory_list.size(); i++)
    {
        if(    is_regular_file(working_directory + working_directory_list[i].path().filename().string())
            && STARTS_WITH(working_directory_list[i].path().filename().string(), start)
            && ENDS_WITH  (working_directory_list[i].path().filename().string(), current_extension)
          )
        {
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%s                  ",
                       working_directory_list[i].path().filename().string().c_str()
                      );
            y_off += y_line;
            files_shown++;
        }
    }
    //------------------------------------------------------------------------
    if(files_shown == 0)
    {
        bmp_printf(&bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor, "none found        ");
        return LASERBOY_PARENT_DIR_EMPTY;
    }
    //------------------------------------------------------------------------
    return files_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_directory_list(string start)
{
    u_int  i,
           y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = y_line,
           dirs_shown   = 0,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 7;
    bmp_put_rectangle(&bmp, x_off, 0, bmp.xres, bmp.xres, bmp_bg_color, true);
    bmp_printf(&bmp, x_off, 0, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "%s                  ",
               working_directory.c_str()
              );
    //------------------------------------------------------------------------
    for(i = 0; i < working_directory_list.size(); i++)
    {
        if(    is_directory(working_directory + working_directory_list[i].path().filename().string())
            && STARTS_WITH(working_directory_list[i].path().filename().string(), start)
          )
        {
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%s                  ",
                       working_directory_list[i].path().filename().string().c_str()
                      );
            y_off += y_line;
            dirs_shown++;
        }
    }
    //------------------------------------------------------------------------
    if(dirs_shown == 0)
    {
        bmp_printf(&bmp, x_off, y_off, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
                   "none found        "
                  );
        return LASERBOY_PARENT_DIR_EMPTY;
    }
    //------------------------------------------------------------------------
    return dirs_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_frame_effects(string start)
{
    int    i,
           y_line        = 8 * font_size_factor,
           x_off         = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off         = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           effects_shown = 0;
    u_int  bmp_bg_color  = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 113;
    bmp_printf(&bmp, x_off, 0, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "frame effects             "
              );
    for(i = 0; i < (int)frame_effect_names.size(); i++)
    {
        if(starts_with(frame_effect_names[i], start))
        {
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%s                 ",
                       frame_effect_names[i].c_str()
                      );
            effects_shown++;
        }
    }
    if(effects_shown == 0)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no frame effects loaded"
                  );
    }
    //------------------------------------------------------------------------
    return effects_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_frame_set_effects(string start)
{
    int    i,
           y_line        = 8 * font_size_factor,
           x_off         = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off         = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           effects_shown = 0;
    u_int  bmp_bg_color  = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 137;
    bmp_printf(&bmp, x_off, 0, bmp.white, bmp_bg_color, transparent_menu_font, font_size_factor,
               "frame set effects          "
              );
    for(i = 0; i < (int)frame_set_effect_names.size(); i++)
    {
        if(starts_with(frame_set_effect_names[i], start))
        {
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                       "%s                 ",
                       frame_set_effect_names[i].c_str()
                      );
            effects_shown++;
        }
    }
    if(effects_shown == 0)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no frame set effects loaded"
                  );
    }
    //------------------------------------------------------------------------
    return effects_shown;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
