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
// LaserBoy_space.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SPACE_DEFINITIONS__
#define __LASERBOY_SPACE_DEFINITIONS__

//############################################################################
#include "LaserBoy_frame_set.hpp"
#include "LaserBoy_palette_set.hpp"

//############################################################################
class LaserBoy_GUI_base;

//############################################################################
class LaserBoy_space : public LaserBoy_frame_set, public LaserBoy_palette_set
{
public:
    LaserBoy_space(LaserBoy_GUI_base* gui = NULL);
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_space()
    {
        bmp_free (&bmp);
        bmp_free (&background);
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& operator [] (int index)    // does not set frame_index
                    {                          // eliminates ambiguity of space being
                        if(number_of_frames()) // both a frame set and palette set
                        {
                            if(index >= (int)number_of_frames())
                                index = 0;
                            if(index < 0)
                                index = number_of_frames() - 1;
                            return LaserBoy_frame_set::at(index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    LaserBoy_space& operator += (const LaserBoy_frame_set& frames)
                    {
                        LaserBoy_frame_set::insert(LaserBoy_frame_set::end(),
                                                   frames.begin(), frames.end()
                                                  );
                        return *this;
                    }
    //------------------------------------------------------------------------
    void clear_frames()
                {
                    LaserBoy_frame_set::clear();
                    return;
                }
    //------------------------------------------------------------------------
    void clear_palettes()
                {
                    LaserBoy_palette_set::clear();
                    return;
                }
    //------------------------------------------------------------------------
    bool have_nonzero_offsets()
                {
                    if(wave_offsets[0]) return true;
                    if(wave_offsets[1]) return true;
                    if(wave_offsets[2]) return true;
                    if(wave_offsets[3]) return true;
                    if(wave_offsets[4]) return true;
                    if(wave_offsets[5]) return true;
                    if(wave_offsets[6]) return true;
                    if(wave_offsets[7]) return true;
                    return false;
                }
    //------------------------------------------------------------------------
    void clean_screen                 ();
    //------------------------------------------------------------------------
    void rename_all_frames            (string prefix);
    //------------------------------------------------------------------------
    bool omit_palette                 (int index);
    void clear_unused_palettes        ();
    void sync_rgb_and_palette         ();
    void minimize_tables_and_palettes ();
    void best_reduce_all_to_palette   ();
    //------------------------------------------------------------------------
    bool load_wtf_file                (string wtf_file);
    bool save_wtf_file                (string wtf_file);
    //------------------------------------------------------------------------
    void load_color_rescale_files     ();
    
    LaserBoy_Rescale_Error_Code  load_color_rescale_file(int color_channel);
    
    int  color_rescales_flags         ();
    //------------------------------------------------------------------------
    void load_frame_effects           ();
    void load_frame_set_effects       ();
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  call_frame_effect      (string name);
    LaserBoy_Error_Code  call_frame_set_effect  (string name);
    //------------------------------------------------------------------------
    void load_background_bitmap       (struct LaserBoy_bmp* bmp);
    bool load_background_bitmap       (const string& file      );
    void clear_background_bitmap      ();
    void recolor_background           ();
    //------------------------------------------------------------------------
    void simplify_view_angle()
                {
                    while(view_angle.x < 0)
                     view_angle.x += two_pi;
                    while(view_angle.x > two_pi)
                     view_angle.x -= two_pi;
                    while(view_angle.y < 0)
                     view_angle.y += two_pi;
                    while(view_angle.y > two_pi)
                     view_angle.y -= two_pi;
                    while(view_angle.z < 0)
                     view_angle.z += two_pi;
                    while(view_angle.z > two_pi)
                     view_angle.z -= two_pi;
                }
    //------------------------------------------------------------------------
    LaserBoy_bmp* p_bmp           () { return &bmp;            }
    LaserBoy_bmp* p_background    () { return &background;     }
    LaserBoy_bmp* p_background_bmp() { return &background_bmp; }
    //------------------------------------------------------------------------
    void undo_wave_temp(fstream &in, fstream &out, const string& file_name)
                {
                    if(in.is_open())
                        in.close();
                    if(out.is_open())
                        out.close();
                    remove(file_name.c_str());
                    rename(  (file_name + ".tmp").c_str()
                           , file_name.c_str()
                          );
                    return;
                }
    //------------------------------------------------------------------------
    bool apply_wave_offsets_prep       (const string& file);
    bool overwirte_wave_offsets        (const string& file);
    bool invert_wave_prep              (const string& file);
    bool invert_signals_prep           (const string& file);
    bool clear_wave_polarity_list      (const string& file, bool global_flip_flop);
    bool apply_color_rescales_prep     (const string& file);
    bool omit_color_rescales           (const string& file);
    bool save_color_rescales           (const string& file,    const string& out_name   );
    bool black_level_to_wave_prep      (const string& file_in, const string& file_out   );
    bool bit_resolution_to_wave_prep   (const string& file_in, const string& file_out   );
    //------------------------------------------------------------------------
    bool split_wave_XY_r_g_b_i_LR_prep (      string  file,
                                        const string& dir_wav,
                                        const string& dir_unformatted,
                                        const string& dir_audio
                                       );
    //------------------------------------------------------------------------
    bool split_wave_XY_rg_bi_LR_prep   (      string  file,
                                        const string& dir_wav,
                                        const string& dir_unformatted,
                                        const string& dir_audio
                                       );
    //------------------------------------------------------------------------
    bool join_XY_r_g_b_i_waves_prep    (const string& file_XY,
                                        const string& file_r,
                                        const string& file_g,
                                        const string& file_b,
                                        const string& file_i,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool join_XY_rg_b_waves_prep       (const string& file_XY,
                                        const string& file_rg,
                                        const string& file_b,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool join_XY_rg_b_LR_waves_prep    (const string& file_XY,
                                        const string& file_rg,
                                        const string& file_b,
                                        const string& file_LR,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool add_audio_to_wave_prep        (const string& file_laser,
                                        const string& file_audio,
                                        const string& file_out
                                       );
    //------------------------------------------------------------------------
    void apply_wave_offsets        (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_wave               (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_signals            (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void black_level_to_wave       (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void bit_resolution_to_wave    (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void apply_color_rescales      (fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void split_wave_XY_r_g_b_i_LR  (LaserBoy_wave_header wave_in_header,
                                    fstream&             wave_in,
                                    fstream&             wave_out_XY,
                                    fstream&             wave_out_r,
                                    fstream&             wave_out_g,
                                    fstream&             wave_out_b,
                                    fstream&             wave_out_i,
                                    fstream&             wave_out_LR
                                   );
    //------------------------------------------------------------------------
    void split_wave_XY_rg_bi_LR    (LaserBoy_wave_header wave_in_header,
                                    fstream&             wave_in,
                                    fstream&             wave_out_XY,
                                    fstream&             wave_out_rg,
                                    fstream&             wave_out_bi,
                                    fstream&             wave_out_LR
                                   );
    //------------------------------------------------------------------------
    void join_XY_r_g_b_i_waves     (fstream& in_XY,
                                    fstream& in_r,
                                    fstream& in_g,
                                    fstream& in_b,
                                    fstream& in_i,
                                    fstream& out,
                                    bool     global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_waves        (fstream& in_XY,
                                    fstream& in_rg,
                                    fstream& in_b,
                                    fstream& out,
                                    bool     global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_LR_waves     (fstream& in_XY,
                                    fstream& in_rg,
                                    fstream& in_b,
                                    fstream& in_LR,
                                    fstream& out,
                                    bool     global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_00_waves     (fstream& in_XY,
                                    fstream& in_rg,
                                    fstream& in_b,
                                    fstream& out,
                                    bool     global_polartity
                                   );
    //------------------------------------------------------------------------
    void add_audio_to_wave         (LaserBoy_wave_header wave_in_header_laser,
                                    LaserBoy_wave_header wave_in_header_audio,
                                    fstream&             in_laser,
                                    fstream&             in_audio,
                                    fstream&             out
                                   );
    //------------------------------------------------------------------------
    void add_silence_to_wave       (LaserBoy_wave_header wave_in_header_laser,
                                    fstream&             in_laser,
                                    fstream&             out
                                   );
    //------------------------------------------------------------------------
    void format_wave               (LaserBoy_wave_header wave_in_header_laser,
                                    fstream&             in_laser,
                                    fstream&             out,
                                    bool                 global_polartity,
                                    bool                 write_eof_bits,
                                    int                  channel_5
                                   );
    //------------------------------------------------------------------------
    string LaserBoy_wave_signal_id_to_name  (const short&   signal_id) const;
    string LaserBoy_wave_LSB_tag_to_name    (const u_short& LSB_tag  ) const;
    string LaserBoy_dots_setting_id_to_name (const int&   id       ) const;
    //------------------------------------------------------------------------
//    void   tell(LaserBoy_wave_header header, string label) const;
    //------------------------------------------------------------------------
    LaserBoy_GUI_base *p_GUI;

    struct
    LaserBoy_bmp       bmp           , // a memory clone of screen
                       background    , // empty for clearing screen
                       background_bmp; // possibly a bitmap picture

    string             bg_file_name         ,
                       install_GUID         ,
                       color_rescale_file[4]; // txt file names for each color channel

    LaserBoy_color     bg_color       ,
                       rendered_bounds,
                       rendered_blank ,
                       rendered_black ;

    LaserBoy_3D_double fulcrum     ,
                       view_angle  ,
                       view_scale  ,
                       view_offset ;

    vector<string>                    frame_effect_names    ,
                                      frame_set_effect_names;
    vector<LaserBoy_frame_effect>     frame_effects         ;
    vector<LaserBoy_frame_set_effect> frame_set_effects     ;

    bool    show_vertices            ,
            show_blanking            ,
            show_intro               ,
            show_bridge              ,
            show_coda                ,
            show_cursors             ,
            show_color_cursor        ,
            show_fixed_bounds        ,
            show_fixed_origin        ,
            show_floating_axis       ,
            show_floating_bounds     ,
            show_fulcrum             ,
            show_indices             ,
            show_effects_generation  ,
            show_background_bitmap   ,
            show_onion_skin          ,
            show_onion_visuals       ,
            show_menu                ,
            show_palette             ,
            show_target_palette      ,
            show_stats               ,
            show_TUI_clue            ,
            show_frame_stats         ,
            show_frame_set_stats     ,
            show_Z_as_order          ,
            show_quad_view           ,
            ui_visuals_flip_flop     ,
            loop_animation           ,
            approximate_frame_rate   ,
            wave_show_vertices       ,
            wave_show_blanks         ,
            wave_real_time           ,
            wave_show_inverted       ,
            wave_clean_screen        ,
            wave_loop                ,
            auto_scale_real          ,
            maintain_real_origin     ,
            save_true_color_dxf      ,
            black_dwell_vertices     ,
            auto_flatten_z           ,
            z_from_unformatted       ,
            no_equivalent_vectors    ,
            conglomerate_in_minimize ,
            fracture_b4_conglomerate ,
            invert_wave_output       ,
            auto_minimize            ,
            auto_apply_offsets       ,
            auto_apply_rescales      ,
            no_black_match           ,
            allow_lit_black          ,
            dumb_file_format         ,
            save_txt_with_color      ,
            save_txt_unit            ,
            save_txt_color_hex       ,
            save_txt_named_palettes  ,
            save_fmtd_wav_splits     ;

    short   color_rescale_r     [256],
            color_rescale_g     [256],
            color_rescale_b     [256],
            color_rescale_i     [256];

    short  *color_rescale         [4];

    u_int   black_level              ,
            number_of_skins          ,
            sample_rate              ,
            lit_dwell_overhang       ,
            dots_setting             ,
            dwell_on_dot             ,
            output_bmp_size          ,
            transparent_menu_font    ,
            font_size_factor         ,
            background_bmp_scale     ,
            signal_if_not_z          ,
            unframed_wave_stride     ,
            channels_of_wav_out      ,
            app_runs_count           ,
            signal_bit_mask       [8]; // number of bits to mask from the LSB end

    int     displacement_step        ,
            signal_polarity       [8], // -1 unknown, 0 positive, 1 negative
            wave_offsets          [8]; // in samples

    double  lit_delta_max            ,
            blank_delta_max          ,
            max_dwell_microsec       ,
            max_wag_microsec         ,
            insignificant_distance   ,
            insignificant_angle      ,
            recolor_span_factor      ,
            onion_skin_fade          ,
            rendered_arc_angle       , // in degrees
            rendered_mono_font_space ,
            rendered_vari_font_gap   ,
            frames_per_second        ,
            still_frame_duration     ,
            rotation_step            , // in radians!
            magnitude_step           ,
            magnitude_step_up        ,
            magnitude_step_dn        ;
    //------------------------------------------------------------------------
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
