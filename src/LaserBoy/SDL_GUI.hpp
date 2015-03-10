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
// LaserBoy_SDL_GUI.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SDL_GUI_DEFINITIONS__
#define __LASERBOY_SDL_GUI_DEFINITIONS__

//############################################################################
#include <LaserBoy/GUI_base.hpp>
#include <SDL/SDL.h>

namespace LaserBoy {

//############################################################################
//  BITMAP MEMORY AND SDL
void bmp_clone_SDL_Surface(struct LaserBoy_bmp* bmp,    struct SDL_Surface*  screen, int copy);
void bmp_to_SDL_Surface   (struct LaserBoy_bmp* bmp,    struct SDL_Surface*  screen);
void SDL_Surface_to_bmp   (struct SDL_Surface*  screen, struct LaserBoy_bmp* bmp);

//############################################################################
class LaserBoy_SDL_GUI : public LaserBoy_GUI_base
{
public:
    LaserBoy_SDL_GUI(int x, int y);
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_SDL_GUI();
    //------------------------------------------------------------------------
virtual  void    display_space()
                    {
                        bmp_to_SDL_Surface(space.p_bmp(), screen);
                    }
    //------------------------------------------------------------------------
virtual  void    display_state(const string& state)
                    {
                        bmp_printf(&(space.bmp), 0, 0,
                                   0xffff00, 0x004000, 0,
                                   space.font_size_factor,
                                   "             %s                                                                                                                                ",
                                   state.c_str()
                                  );
                        //----------------------------------------------------
                        memcpy((void*)(screen->pixels),
                               (const void*)(space.bmp.image),
                               space.bmp.bytes_per_line * 8 * space.font_size_factor
                              );
                        SDL_UnlockSurface(screen);
                        SDL_UpdateRect(screen, 0, 0, space.bmp.xres, 8 * space.font_size_factor);
                        SDL_LockSurface(screen);
                        return;
                    }
    //------------------------------------------------------------------------
virtual  void    display_progress(int countdown)
                    {
                        bmp_printf(&(space.bmp), 0, 0,
                                   0x00ffff00, 0x004000, 0,
                                   space.font_size_factor,
                                   "%12d",
                                   countdown
                                  );
                        //----------------------------------------------------
                        memcpy((void*)(screen->pixels),
                               (const void*)(space.bmp.image),
                               space.bmp.bytes_per_line * 8 * space.font_size_factor
                              );
                        SDL_UnlockSurface(screen);
                        SDL_UpdateRect(screen, 0, 0, space.bmp.xres, 8 * space.font_size_factor);
                        SDL_LockSurface(screen);
                        return;
                    }
    //------------------------------------------------------------------------
virtual  void       capture_screen               ();
    //------------------------------------------------------------------------
virtual  string     display_prompt_file_with_auto_complete     (const string& prompt,
                                                                const string& part = "");
    //------------------------------------------------------------------------
virtual  string     display_prompt_dir_with_auto_complete      (const string& prompt );
virtual  string     display_prompt_f_effect_with_auto_complete (const string& prompt );
virtual  string     display_prompt_fs_effect_with_auto_complete(const string& prompt );
virtual  string     display_prompt_and_echo_name               (const string& prompt );
virtual  string     display_prompt_and_echo_string             (const string& prompt );
virtual  double     display_prompt_and_echo_double             (const string& prompt,
                                                                double        value = 0
                                                               );
    //------------------------------------------------------------------------
virtual  int        display_prompt_and_echo_int                (const string& prompt,
                                                                int           value = 0
                                                               );
    //------------------------------------------------------------------------
virtual  int        display_prompt_and_echo_nibble             (const string& prompt );
virtual  bool       display_prompt_and_echo_bool               (const string& prompt );
    //------------------------------------------------------------------------
virtual  bool       report_ild_file_open                       (LaserBoy_frame_set& frame_set,
                                                                const string& file_name
                                                               );
    //------------------------------------------------------------------------
virtual  void       display_error                              (const string& error  );
virtual  void       display_message                            (const string& message);
    //------------------------------------------------------------------------
    // SDL_key menu system
    //------------------------------------------------------------------------
    void    wait_4_Esc                           ();
    void    wait_4_any_key                       ();
    void    check_for_common_nav_keys            (SDLKey key);
    void    animate_forward                      ();
    void    animate_reverse                      ();
    void    rotate_forward                       ();
    void    rotate_reverse                       ();
    void    animate_rotate_forward               ();
    void    animate_rotate_reverse               ();
    //------------------------------------------------------------------------
    void    system_settings_menu                 ();
    void    edit_background_color_menu           ();
    void    signal_if_not_z_menu                 ();
    void    dots_setting_menu                    ();
    void    bit_resolution_menu                  ();
    void    visuals_settings_menu                ();
    void    switch_settings_menu                 ();
    void    apply_view_menu                      ();
    void    palette_transforms_menu              ();
    void    add_color_to_target_palette_menu     ();
    void    insert_color_in_target_palette_menu  ();
    void    edit_color_in_target_palette_menu    ();
    void    frame_transforms_menu                ();
    void    frame_set_transforms_menu            ();
    void    draw_color_blank_menu                ();
    void    move_scale_rotate_menu               ();
    void    direct_draw_segments_menu            ();
    void    draw_point_menu                      ();
    void    draw_line_menu                       ();
    void    draw_rectangle_menu                  ();
    void    draw_polygon_menu                    ();
    void    draw_polyline_menu                   ();
    void    draw_polystar_menu                   ();
    void    draw_circular_arc_menu               ();
    void    draw_elliptical_arc_menu             ();
    void    draw_rhodonea_menu                   ();
    void    draw_epicycloid_menu                 ();
    void    draw_epitrochoid_menu                ();
    void    draw_hypotrochoid_menu               ();
    void    draw_hypocycloid_menu                ();
    void    draw_lissajous_menu                  ();
    void    draw_harmonograph_menu               ();
    void    draw_mono_spaced_font_menu           ();
    void    draw_variable_spaced_font_menu       ();
    void    wave_utilities_menu                  ();
    void    split_or_join_waves_menu             ();
    void    wave_show_settings_menu              ();
    void    show_wave_options_menu               ();
    void    set_wave_offsets_menu                ();
    void    set_color_rescales_menu              ();
    void    apply_color_rescales_menu            ();
    void    omit_color_rescales_menu             ();
    void    set_signal_polarities_menu           ();
    void    clear_wave_polarities_menu           ();
    void    black_level_to_wave_menu             ();
    void    bit_resolution_to_wave_menu          ();
    void    join_XY_r_g_b_i_waves_menu           ();
    void    join_XY_rg_b_waves_menu              ();
    void    join_XY_rg_b_LR_waves_menu           ();
    void    add_audio_to_wave_menu               ();
    void    file_open_menu                       ();
    bool    ild_file_open_menu                   ();
    bool    dxf_file_open_menu                   ();
    bool    wav_file_open_menu                   ();
    bool    wav_qm_open_menu                     (bool append);
    bool    wav_unformatted_open_menu            (bool append);
    bool    txt_file_open_menu                   ();
    bool    bmp_file_open_menu                   ();
    void    save_as_file_menu                    ();
    bool    save_as_ild_menu                     ();
    bool    save_as_dxf_menu                     ();
    bool    save_as_wav_menu                     ();
    bool    save_as_txt_menu                     ();
    bool    save_as_bmp_menu                     ();
    //------------------------------------------------------------------------
    void    display_rescale_file_error           (LaserBoy_Rescale_Error_Code error);
    void    process_terminate_request            ();
    void    Laserboy_terminating                 ();
    int     start_menu_loop                      ();
    //------------------------------------------------------------------------
    bool    display_wave                         ();
    bool    display_unframed_wave                ();
    void    display_LaserBoy_wave  (fstream& in, LaserBoy_wave_header header);
    void    display_unframed_wave  (fstream& in, LaserBoy_wave_header header);
    //------------------------------------------------------------------------
    // data
    //------------------------------------------------------------------------
    SDL_Surface*  screen; // an SDL graphics window
    SDL_Event     event ; // UI input (keys)
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
