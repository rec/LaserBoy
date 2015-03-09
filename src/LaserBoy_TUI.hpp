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
// LaserBoy_TUI.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_TEXT_USER_INTERFACE_DEFINITIONS__
#define __LASERBOY_TEXT_USER_INTERFACE_DEFINITIONS__

#include "LaserBoy_space.hpp"

//############################################################################
class LaserBoy_TUI : public LaserBoy_space
{
public:
    LaserBoy_TUI(LaserBoy_GUI_base* gui)
                : LaserBoy_space         (gui),
                  TUI_palette_index      (  0),
                  current_menu_name_bc   (  0),
                  current_menu_name      (   ),
                  TUI_clue               (   ),
                  working_directory      (   ),
                  current_extension      (   ),
                  working_directory_list (   )
                                                      {};
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_TUI()                                    {};
    //------------------------------------------------------------------------
    void    render_space                          ();
    void    overlay_palettes                      ();
    void    overlay_frame_stats                   ();
    void    overlay_frame_set_stats               ();
    void    overlay_palette_set_stats             ();
    void    overlay_program_menu                  ();
    void    overlay_in_file_type_menu             ();
    void    overlay_in_ild_action_menu            ();
    void    overlay_in_dxf_action_menu            ();
    void    overlay_in_wav_action_menu            ();
    void    overlay_in_txt_action_menu            ();
    void    overlay_in_bmp_action_menu            ();
    void    overlay_if_not_z_options              ();
    void    overlay_dots_options                  ();
    void    overlay_out_file_type_menu            ();
    void    overlay_out_ild_menu                  ();
    void    overlay_out_dxf_menu                  ();
    void    overlay_out_wav_menu                  ();
    void    overlay_out_txt_menu                  ();
    void    overlay_out_bmp_menu                  ();
    void    overlay_value_settings_menu           ();
    void    overlay_veiw_menu                     ();
    void    overlay_visuals_menu                  ();
    void    overlay_switch_settings_menu          ();
    void    overlay_palette_tranz_menu            ();
    void    overlay_frame_set_tranz_menu          ();
    void    overlay_frame_tranz_menu              ();
    void    overlay_draw_color_blank_menu         ();
    void    overlay_move_scale_rotate_menu        ();
    void    overlay_direct_draw_segments_menu     ();
    void    overlay_wave_utilities_menu           ();
    void    overlay_show_split_or_join_waves_menu ();
    void    overlay_show_wave_options_menu        ();
    void    overlay_wave_show_settings_menu       ();
    //------------------------------------------------------------------------
    bool    overlay_wave_analysis                 (string file_name);
    bool    overlay_wave_header_stats             (string file_name);
    short   overlay_wave_signal_polarities        (string file_name, int polarity[8]);
    void    overlay_wave_offsets                  ();
    void    overlay_color_rescale_stats           ();
    void    overlay_bit_resolution_settings       ();
    //------------------------------------------------------------------------
    void    get_directory_list                    (string dir = "",
                                                   string ext = ""
                                                  );
    //------------------------------------------------------------------------
    int     overlay_file_list                     (string start = "");
    int     overlay_directory_list                (string start = "");
    int     overlay_frame_effects                 (string start = "");
    int     overlay_frame_set_effects             (string start = "");
    //------------------------------------------------------------------------
    void    set_current_menu_name                 (string name, int bc, bool erase_clue = false)
            {
                current_menu_name_bc = bc;
                current_menu_name    = name;
                if(erase_clue)
                    TUI_clue.erase();
                return;
            }
    //------------------------------------------------------------------------
    LaserBoy_TUI& operator = (const LaserBoy_frame_set& frames)
                    {
                        LaserBoy_frame_set::clear();
                        LaserBoy_frame_set::reserve(frames.number_of_frames());
                        LaserBoy_frame_set::insert (LaserBoy_frame_set::end(),
                                                    frames.LaserBoy_frame_set::begin(),
                                                    frames.LaserBoy_frame_set::end()
                                                   );
                        return *this;
                    }
    //------------------------------------------------------------------------
    u_int next_hue()
    {
         TUI_palette_index += 12;
         TUI_palette_index %= 254;
         return bmp.make_rgb(palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).r,
                             palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).g,
                             palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).b
                            );
    }
    //------------------------------------------------------------------------
    int            TUI_palette_index,
                   current_menu_name_bc;
    string         current_menu_name,
                   TUI_clue,
                   working_directory,
                   current_extension;

    vector<directory_entry> working_directory_list;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
