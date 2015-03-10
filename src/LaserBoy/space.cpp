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
// LaserBoy_space.cpp is part of LaserBoy.
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

//############################################################################
LaserBoy_space::LaserBoy_space(LaserBoy_GUI_base* gui)
            : LaserBoy_frame_set       (this       )
            , LaserBoy_palette_set     (this       )
            , p_GUI                    (gui        )
            , bmp                      (           ) // LaserBoy_bmp
            , background               (           ) // LaserBoy_bmp
            , background_bmp           (           ) // LaserBoy_bmp
            , bg_file_name             ("."        ) // string
            , install_GUID             (GUID8char())
//            , color_rescale_file       (NULL       )
            , bg_color                 (           )
            , rendered_bounds          (           )
            , rendered_blank           (           )
            , rendered_black           (           )
            , fulcrum                  (           )
            , view_angle               (           )
            , view_scale               (1.0        ) // LaserBoy_3D_double
            , view_offset              (           )
            , frame_effect_names       (           )
            , frame_set_effect_names   (           )
            , frame_effects            (           )
            , frame_set_effects        (           )
            , show_vertices            (false      ) // bool
            , show_blanking            (false      )
            , show_intro               (false      )
            , show_bridge              (false      )
            , show_coda                (false      )
            , show_cursors             (false      )
            , show_color_cursor        (false      )
            , show_fixed_bounds        (false      )
            , show_fixed_origin        (false      )
            , show_floating_axis       (false      )
            , show_floating_bounds     (true       )
            , show_fulcrum             (false      )
            , show_indices             (false      )
            , show_effects_generation  (true       )
            , show_background_bitmap   (true       )
            , show_onion_skin          (false      )
            , show_onion_visuals       (false      )
            , show_menu                (true       )
            , show_palette             (false      )
            , show_target_palette      (false      )
            , show_stats               (true       )
            , show_TUI_clue            (true       )
            , show_frame_stats         (false      )
            , show_frame_set_stats     (false      )
            , show_Z_as_order          (false      )
            , show_quad_view           (false      )
            , ui_visuals_flip_flop     (true       )
            , loop_animation           (true       )
            , approximate_frame_rate   (true       )
            , wave_show_vertices       (false      )
            , wave_show_blanks         (false      )
            , wave_real_time           (true       )
            , wave_show_inverted       (true       )
            , wave_clean_screen        (true       )
            , wave_loop                (false      )
            , auto_scale_real          (true       )
            , maintain_real_origin     (false      )
            , save_true_color_dxf      (false      )
            , black_dwell_vertices     (false      )
            , auto_flatten_z           (false      )
            , z_from_unformatted       (false      )
            , no_equivalent_vectors    (false      )
            , conglomerate_in_minimize (false      )
            , fracture_b4_conglomerate (false      )
            , invert_wave_output       (true       )
            , auto_minimize            (true       )
            , auto_apply_offsets       (true       )
            , auto_apply_rescales      (false      )
            , no_black_match           (true       )
            , allow_lit_black          (false      )
            , dumb_file_format         (false      )
            , save_txt_with_color      (false      )
            , save_txt_unit            (false      )
            , save_txt_color_hex       (false      )
            , save_txt_named_palettes  (false      )
            , save_fmtd_wav_splits     (true       )
//            , color_rescale_r          (NULL       )
//            , color_rescale_g          (NULL       )
//            , color_rescale_b          (NULL       )
//            , color_rescale_i          (NULL       )
//            , color_rescale            (NULL       )
            , black_level              (0          ) // int
            , number_of_skins          (4          ) // int
            , sample_rate              (48000      ) // int Hz
            , lit_dwell_overhang       (2          ) // vertices
            , dots_setting             (LASERBOY_DOTS_IGNORE)
            , dwell_on_dot             (10         ) // vertices
            , output_bmp_size          (1000       ) // pixels square
            , transparent_menu_font    (0          )
            , font_size_factor         (1          )
            , background_bmp_scale     (1          )
            , signal_if_not_z          (LASERBOY_SIGNAL_MONO_AVG_ANALOG)
            , unframed_wave_stride     (4800       ) // samples 1/10 of sec
            , channels_of_wav_out      (6          )
            , app_runs_count           (1          )
//            , signal_bit_mask          (NULL       )
            , displacement_step        (100        ) // int points
//            , signal_polarity          (NULL       )
//            , wave_offsets             (NULL       )
            , lit_delta_max            (800        ) // points
            , blank_delta_max          (1000       ) // points
            , max_dwell_microsec       (333        ) // microseconds
            , max_wag_microsec         (50000.0    ) // double 1/20th second
            , insignificant_distance   (60         ) // points
            , insignificant_angle      (one_degree ) // radians
            , recolor_span_factor      (2.0        )
            , onion_skin_fade          (0.70       ) // ratio 0.0 ~ 1.0
            , rendered_arc_angle       (6.0        ) // degrees
            , rendered_mono_font_space (0.70       ) // percent of glyph size
            , rendered_vari_font_gap   (0.20       ) // percent of glyph size
            , frames_per_second        (16.0       ) // 0 == every frame is shown exactly once
            , still_frame_duration     (10.0       ) // seconds
            , rotation_step            (one_degree ) // radians
            , magnitude_step           (0.01       )
            , magnitude_step_up        (1.01       )
            , magnitude_step_dn        (0.99       )
{
    u_int i;
    for(i = 0; i < 8; i++)
    {
        wave_offsets   [i] =  0;
        signal_polarity[i] = -1;
        signal_bit_mask[i] =  0;
    }
    for(i = 2; i <= 4; i++)
    {
        wave_offsets   [i] = -5;
        signal_bit_mask[i] =  7;
    }
    color_rescale_file[0] =
    color_rescale_file[1] =
    color_rescale_file[2] =
    color_rescale_file[3] = ".";
    for(i = 0; i < 256; i++)
    {
        color_rescale_r[i] =
        color_rescale_g[i] =
        color_rescale_b[i] =
        color_rescale_i[i] = 0;
    }
    color_rescale[0] = color_rescale_r;
    color_rescale[1] = color_rescale_g;
    color_rescale[2] = color_rescale_b;
    color_rescale[3] = color_rescale_i;
    rendered_bounds  = LaserBoy_color(160, 160, 160);
    rendered_blank   = LaserBoy_color(128, 128, 128);
    rendered_black   = LaserBoy_color( 16,  16,  16);
    load_wtf_file(LASERBOY_WTF_SHARE + "LaserBoy.wtf");
    load_frame_effects      ();
    load_frame_set_effects  ();
    load_color_rescale_files();
}

//############################################################################
void LaserBoy_space::clean_screen()
{
    if(show_background_bitmap)
        bmp_copy(&bmp, &background_bmp);
    else
        bmp_copy(&bmp, &background);
}

//############################################################################
void LaserBoy_space::rename_all_frames(string prefix)
{
    char number[8];
    u_int i;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
    {
        sprintf(number, "%05d", i);
        (*this)[i].name = prefix + number;
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_space::omit_palette(int index)
{
    u_int  i;
    //------------------------------------------------------------------------
    if(index < LASERBOY_BUILT_IN_PALETTES)
        return false;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
        if(index == frame_picker(i).palette_index)
            return false;
    //------------------------------------------------------------------------
    LaserBoy_palette_set::erase(LaserBoy_palette_set::begin() + index);
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
        if(frame_picker(i).palette_index > index)
            frame_picker(i).palette_index--;
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void LaserBoy_space::clear_unused_palettes()
{
    u_int  i;
    p_GUI->display_state("clearing unused palettes");
    for(i = number_of_palettes() - 1; i >= LASERBOY_BUILT_IN_PALETTES; i--)
        omit_palette(i);
    return;
}

//############################################################################
void LaserBoy_space::sync_rgb_and_palette()
{
    int current_frame_index = frame_index;
    p_GUI->display_state("syncing rgb & palettes");
    for(u_int i = 0; i < number_of_frames(); i++)
    {
        frame(i).sync_rgb_and_palette();
        p_GUI->display_progress(number_of_frames() - i);
    }
    frame_index = current_frame_index;
    return;
}

//############################################################################
void LaserBoy_space::minimize_tables_and_palettes()
{
    u_int                 i = 0,
                          j,
                          super_palette_first_frame = 0,
                          super_palette_last_frame  = 0,
                          current_frame_index       = frame_index;
    LaserBoy_palette_set  reduced_palette_set (this);
    LaserBoy_palette      super_palette       (this);
    //------------------------------------------------------------------------
    p_GUI->display_state("syncing rgb & palettes");
    for(j = 0; j < number_of_frames(); j++)
    {
        frame(j).sync_rgb_and_palette();
        p_GUI->display_progress(number_of_frames() - j);
    }
    //------------------------------------------------------------------------
    p_GUI->display_state("minimizing tables & palettes");
    while(i < number_of_frames())
    {
        if(frame_picker(i).palette_index != LASERBOY_TRUE_COLOR)
        {
            //----------------------------------------------------------------
            super_palette_first_frame = i;
            super_palette = palette_picker(frame_picker(i).palette_index);
            super_palette.name = GUID8char();
            //----------------------------------------------------------------
            while(    i < number_of_frames()
                   && super_palette.unite(palette_picker(frame_picker(i).palette_index))
                 )
                i++;
            //----------------------------------------------------------------
            reduced_palette_set.push_back(super_palette);
            super_palette_last_frame  = i - 1;
            //----------------------------------------------------------------
            for(j = super_palette_first_frame; j <= super_palette_last_frame; j++)
            {
                frame(j).find_rgb_in_palette(super_palette);
                frame(j).palette_index = reduced_palette_set.number_of_palettes() - 1;
            }
            //----------------------------------------------------------------
        }
        else
            i++;
        p_GUI->display_progress(number_of_frames() - i);
    }
    //------------------------------------------------------------------------
    frame(current_frame_index);
    replace_palette_set(reduced_palette_set);
    palette_index = current_frame().palette_index;
    return;
}

//############################################################################
void LaserBoy_space::best_reduce_all_to_palette()
{
    u_int             i,
                      j;

    vector<int>       used_palette_indices;
    LaserBoy_palette  super_palette(this); // this is p_space!
    //------------------------------------------------------------------------
    p_GUI->display_state("reducing to best palette");
    for(i = 0; i < number_of_frames(); i++)
    {
        if(frame_picker(i).palette_index != LASERBOY_TRUE_COLOR)
        {
            if(used_palette_indices.size()) // counted any palette indexes?
            {
                for(j = 0; j < used_palette_indices.size(); j++)
                    if(frame_picker(i).palette_index == used_palette_indices[j]) // alredy counted it
                        break;
                if(j == used_palette_indices.size())
                {
                    used_palette_indices.push_back(frame_picker(i).palette_index); // count it
                    super_palette += palette_picker(frame_picker(i).palette_index);
                }
            }
            else
            {
                used_palette_indices.push_back(frame_picker(i).palette_index); // count it
                super_palette += palette_picker(frame_picker(i).palette_index);
            }
        }
        else
            super_palette += (frame_picker(i).as_color_table().reorder());
        p_GUI->display_progress(number_of_frames() - i);
    }
    //------------------------------------------------------------------------
    super_palette.best_reduction(); // squeeze all that in <=256 colors
    super_palette.name = GUID8char();
    push_back_palette(super_palette);
    //------------------------------------------------------------------------
    best_match_palette(LaserBoy_palette_set::size() - 1);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_space::call_frame_effect(string name)
{
    int i;
    //------------------------------------------------------------------------
    for(i = 0; i < (int)frame_effect_names.size(); i++)
        if(frame_effect_names[i] == name)
            break;
    //------------------------------------------------------------------------
    if(i < (int)frame_effect_names.size())
        return frame_effects[i](&(current_frame()));
    return LASERBOY_EFFECT_NAME_NOT_FOUND;
}

//############################################################################
LaserBoy_Bounds LaserBoy_space::call_frame_set_effect(string name)
{
    int i;
    //------------------------------------------------------------------------
    for(i = 0; i < (int)frame_set_effect_names.size(); i++)
        if(frame_set_effect_names[i] == name)
            break;
    //------------------------------------------------------------------------
    if(i < (int)frame_set_effect_names.size())
        return frame_set_effects[i](this);
    return LASERBOY_EFFECT_NAME_NOT_FOUND;
}

//############################################################################
void LaserBoy_space::load_background_bitmap(struct LaserBoy_bmp* bmp)
{
    bmp_flip(bmp);
    bmp_copy(&background_bmp, &background); // wipe the screen first!
    bmp_into_bmp(&background_bmp, bmp, background_bmp_scale);
    show_background_bitmap = true;
    return;
}

//############################################################################
bool LaserBoy_space::load_background_bitmap(const string& file)
{
    char file_name[81];
    struct LaserBoy_bmp bmp;

    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        load_background_bitmap(&bmp);
        bmp_free(&bmp);
        bg_file_name = file;
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_space::clear_background_bitmap()
{
    bmp_fill(&background    , bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp, bg_color.r, bg_color.g, bg_color.b);
    bg_file_name = ".";
    return;
}

//############################################################################
void LaserBoy_space::recolor_background()
{
    bmp_fill(&background    , bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp, bg_color.r, bg_color.g, bg_color.b);
    if(bg_file_name != ".")
        load_background_bitmap(bg_file_name);
    return;
}

//############################################################################
bool LaserBoy_space::load_wtf_file(string wtf_file)
{
    ifstream in(wtf_file.c_str(), ios::in);
    if(in.is_open())
    {
        bool    junk;
        int     c;
        string  version;
        //--------------------------------------------------------------------
        in >> version;
        if(version == LASERBOY_CFG_VERSION) // "LaserBoy.wtf-01-22-2015"
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> install_GUID            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> save_fmtd_wav_splits    ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        else if(version == "LaserBoy.wtf-12-02-2014")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> install_GUID            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> save_fmtd_wav_splits    ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        else if(version == "LaserBoy.wtf-02-17-2013")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> output_bmp_size         ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> save_fmtd_wav_splits    ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        else if(version == "LaserBoy.wtf-08-21-2011")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> output_bmp_size         ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> save_fmtd_wav_splits    ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        else if(version == "LaserBoy.wtf-06-28-2010")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> output_bmp_size         ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        else if(version == "LaserBoy.wtf-12-08-2009")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> channels_of_wav_out     ;
            in >> output_bmp_size         ;
            in >> signal_if_not_z         ;
            in >> unframed_wave_stride    ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> auto_scale_real         ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> z_from_unformatted      ;
            in >> no_equivalent_vectors   ;
            in >> dots_setting            ;
            in >> invert_wave_output      ;
            in >> junk                    ; // delete!
            in >> auto_minimize           ;
            in >> auto_apply_rescales     ;
            in >> auto_apply_offsets      ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> dumb_file_format        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> wave_show_vertices      ;
            in >> wave_show_blanks        ;
            in >> wave_real_time          ;
            in >> wave_show_inverted      ;
            in >> wave_clean_screen       ;
            in >> wave_loop               ;
            in >> wave_offsets[0]         ;
            in >> wave_offsets[1]         ;
            in >> wave_offsets[2]         ;
            in >> wave_offsets[3]         ;
            in >> wave_offsets[4]         ;
            in >> wave_offsets[5]         ;
            in >> wave_offsets[6]         ;
            in >> wave_offsets[7]         ;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[0]      ;
            in >> signal_bit_mask[1]      ;
            in >> signal_bit_mask[2]      ;
            in >> signal_bit_mask[3]      ;
            in >> signal_bit_mask[4]      ;
            in >> signal_bit_mask[5]      ;
            in >> signal_bit_mask[6]      ;
            in >> signal_bit_mask[7]      ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //--------------------------------------------------------------------
        in.close();
        app_runs_count++;
        save_wtf_file(wtf_file);
        return true;
    } // end if(in.is_open())
    save_wtf_file(wtf_file); // not there, so make one.
    return false;
}

//############################################################################
bool LaserBoy_space::save_wtf_file(string wtf_file)
{
    ofstream out(wtf_file.c_str(), ios::out);
    if(out.is_open())
    {
        out << LASERBOY_CFG_VERSION     << ENDL;
        out << bg_file_name.c_str()     << ENDL;
        out << (int)(bg_color.r)        << ENDL;
        out << (int)(bg_color.g)        << ENDL;
        out << (int)(bg_color.b)        << ENDL;
        out << sample_rate              << ENDL;
        out << lit_dwell_overhang       << ENDL;
        out << dwell_on_dot             << ENDL;
        out << font_size_factor         << ENDL;
        out << background_bmp_scale     << ENDL;
        out << displacement_step        << ENDL;
        out << channels_of_wav_out      << ENDL;
        out << app_runs_count           << ENDL;
        out << output_bmp_size          << ENDL;
        out << transparent_menu_font    << ENDL;
        out << signal_if_not_z          << ENDL;
        out << unframed_wave_stride     << ENDL;
        out << fulcrum.x                << ENDL;
        out << fulcrum.y                << ENDL;
        out << fulcrum.z                << ENDL;
        out << view_angle.x             << ENDL;
        out << view_angle.y             << ENDL;
        out << view_angle.z             << ENDL;
        out << view_scale.x             << ENDL;
        out << view_scale.y             << ENDL;
        out << view_offset.x            << ENDL;
        out << view_offset.y            << ENDL;
        out << lit_delta_max            << ENDL;
        out << blank_delta_max          << ENDL;
        out << max_dwell_microsec       << ENDL;
        out << max_wag_microsec         << ENDL;
        out << insignificant_distance   << ENDL;
        out << insignificant_angle      << ENDL;
        out << recolor_span_factor      << ENDL;
        out << onion_skin_fade          << ENDL;
        out << rendered_arc_angle       << ENDL;
        out << rendered_mono_font_space << ENDL;
        out << rendered_vari_font_gap   << ENDL;
        out << rotation_step            << ENDL;
        out << magnitude_step           << ENDL;
        out << frames_per_second        << ENDL;
        out << still_frame_duration     << ENDL;
        out << install_GUID             << ENDL;
        out << show_vertices            << ENDL;
        out << show_blanking            << ENDL;
        out << show_intro               << ENDL;
        out << show_bridge              << ENDL;
        out << show_coda                << ENDL;
        out << show_cursors             << ENDL;
        out << show_color_cursor        << ENDL;
        out << show_palette             << ENDL;
        out << show_target_palette      << ENDL;
        out << show_stats               << ENDL;
        out << show_fixed_bounds        << ENDL;
        out << show_fixed_origin        << ENDL;
        out << show_floating_axis       << ENDL;
        out << show_floating_bounds     << ENDL;
        out << show_onion_skin          << ENDL;
        out << show_onion_visuals       << ENDL;
        out << show_effects_generation  << ENDL;
        out << show_Z_as_order          << ENDL;
        out << show_quad_view           << ENDL;
        out << auto_scale_real          << ENDL;
        out << maintain_real_origin     << ENDL;
        out << save_true_color_dxf      << ENDL;
        out << loop_animation           << ENDL;
        out << approximate_frame_rate   << ENDL;
        out << black_dwell_vertices     << ENDL;
        out << auto_flatten_z           << ENDL;
        out << z_from_unformatted       << ENDL;
        out << no_equivalent_vectors    << ENDL;
        out << conglomerate_in_minimize << ENDL;
        out << fracture_b4_conglomerate << ENDL;
        out << dots_setting             << ENDL;
        out << invert_wave_output       << ENDL;
        out << auto_minimize            << ENDL;
        out << auto_apply_offsets       << ENDL;
        out << auto_apply_rescales      << ENDL;
        out << no_black_match           << ENDL;
        out << allow_lit_black          << ENDL;
        out << dumb_file_format         << ENDL;
        out << save_txt_with_color      << ENDL;
        out << save_txt_unit            << ENDL;
        out << save_txt_color_hex       << ENDL;
        out << save_txt_named_palettes  << ENDL;
        out << save_fmtd_wav_splits     << ENDL;
        out << wave_show_vertices       << ENDL;
        out << wave_show_blanks         << ENDL;
        out << wave_real_time           << ENDL;
        out << wave_show_inverted       << ENDL;
        out << wave_clean_screen        << ENDL;
        out << wave_loop                << ENDL;
        out << wave_offsets[0]          << ENDL;
        out << wave_offsets[1]          << ENDL;
        out << wave_offsets[2]          << ENDL;
        out << wave_offsets[3]          << ENDL;
        out << wave_offsets[4]          << ENDL;
        out << wave_offsets[5]          << ENDL;
        out << wave_offsets[6]          << ENDL;
        out << wave_offsets[7]          << ENDL;
        out << black_level              << ENDL;
        out << number_of_skins          << ENDL;
        out << signal_bit_mask[0]       << ENDL;
        out << signal_bit_mask[1]       << ENDL;
        out << signal_bit_mask[2]       << ENDL;
        out << signal_bit_mask[3]       << ENDL;
        out << signal_bit_mask[4]       << ENDL;
        out << signal_bit_mask[5]       << ENDL;
        out << signal_bit_mask[6]       << ENDL;
        out << signal_bit_mask[7]       << ENDL;
        out << color_rescale_file[0]    << ENDL;
        out << color_rescale_file[1]    << ENDL;
        out << color_rescale_file[2]    << ENDL;
        out << color_rescale_file[3]    << ENDL;
        out << ENDL;
        out << "If this file is currupted, delete it!" << ENDL;
        out.close();
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_space::load_color_rescale_files()
{
    int flags = color_rescales_flags();
    if(flags & LASERBOY_COLOR_RESCALE_R)
        load_color_rescale_file(0);
    if(flags & LASERBOY_COLOR_RESCALE_G)
        load_color_rescale_file(1);
    if(flags & LASERBOY_COLOR_RESCALE_B)
        load_color_rescale_file(2);
    if(flags & LASERBOY_COLOR_RESCALE_I)
        load_color_rescale_file(3);
    return;
}

//############################################################################
LaserBoy_Rescale_Error_Code LaserBoy_space::load_color_rescale_file(int color_channel)
{
    int       i,
              next_char,
              counter = 0;
    double    number;
    ifstream  in;
    //------------------------------------------------------------------------
    if(color_channel > 3 || color_channel < 0)
        return LASERBOY_RESCALE_BAD_CHANNEL_INDEX;
    //------------------------------------------------------------------------
    in.open(color_rescale_file[color_channel].c_str(), ios::in);
    if(in.is_open())
    {
        while(in.good() && counter < 256)
        {
            next_char = in.peek();
            if(next_char == '#') // beginning of a comment line
            {
                while(in.good())
                {
                    next_char = in.get();
                    if(next_char == '\n')
                        break;
                }
            }
            else if(    !isdigit(next_char)
                     && next_char != '-'
                   )
            {
                in.get(); // eat it!
            }
            else // it is a digit or '-'
            {
                in >> number;
                if(counter == 0 && number != 0)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_NONZERO_ZERO;
                }
                if(number < -32766 || number > 32766)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_IMPROPER_DATA;
                }
                if(short(number) & 0x0001)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_ODD_DATA;
                }
                color_rescale[color_channel][counter] = (short)number;
                for(i = 0; i < counter; i++)
                {
                    if(    color_rescale[color_channel][i]
                        == color_rescale[color_channel][counter]
                      )
                    {
                        color_rescale_file[color_channel] = ".";
                        return LASERBOY_RESCALE_REDUNDANT_DATA;
                    }
                }
                counter++;
            }
        }
        if(counter < 256)
        {
            color_rescale_file[color_channel] = ".";
            return LASERBOY_RESCALE_INCOMPLETE_DATA;
        }
        return LASERBOY_OK; // zero!
    }
    //------------------------------------------------------------------------
    color_rescale_file[color_channel] = ".";
    return LASERBOY_RESCALE_FILE_FAILED;
}

//############################################################################
int LaserBoy_space::color_rescales_flags()
{
    if(auto_apply_rescales)
    {
        int flags = 0;
        if(color_rescale_file[0] != ".")
            flags |= LASERBOY_COLOR_RESCALE_R;
        if(color_rescale_file[1] != ".")
            flags |= LASERBOY_COLOR_RESCALE_G;
        if(color_rescale_file[2] != ".")
            flags |= LASERBOY_COLOR_RESCALE_B;
        if(color_rescale_file[3] != ".")
            flags |= LASERBOY_COLOR_RESCALE_I;
        return flags;
    }
    //------------------------------------------------------------------------
    return 0;
}

//############################################################################
bool LaserBoy_space::apply_wave_offsets_prep(const string& file)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wave_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header(wave_in);
    if(wave_in_header.version == "!LaserBoy!")
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample(wave_in_header.num_channels);
    //------------------------------------------------------------------------
    wave_out.open(file.c_str(), ios::out | ios::binary);
    if(!wave_out.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    apply_wave_offsets(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::overwirte_wave_offsets(const string& file)
{
    fstream  wave_fstream;
    wave_fstream.open(file.c_str(), ios::in | ios::out | ios::binary);
    if(!wave_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_header(wave_fstream);
    if(    wave_header.version == "!LaserBoy!"
        || !(wave_header.LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS)
      )
        return false;
    //------------------------------------------------------------------------
    for(u_int i = 0; i < wave_header.num_channels; i++)
        wave_header.offset[i] = wave_offsets[i];
    wave_header.to_fstream_wave(wave_fstream);
    //------------------------------------------------------------------------
    wave_fstream.close();
    return true;
}

//############################################################################
bool LaserBoy_space::invert_wave_prep(const string& file)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wave_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    LaserBoy_wave_header wave_in_header(wave_in);
    //------------------------------------------------------------------------
    wave_out.open(file.c_str(), ios::out | ios::binary);
    if(!wave_out.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    invert_wave(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::invert_signals_prep(const string& file)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wave_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    LaserBoy_wave_header wave_in_header(wave_in);
    //------------------------------------------------------------------------
    wave_out.open(file.c_str(), ios::out | ios::binary);
    if(!wave_out.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    invert_signals(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::clear_wave_polarity_list(const string& file, bool global_flip_flop)
{
    fstream  wave_fstream;
    //------------------------------------------------------------------------
    wave_fstream.open(file.c_str(), ios::in | ios::out | ios::binary);
    if(!wave_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_header(wave_fstream);
    //------------------------------------------------------------------------
    if(wave_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    if(global_flip_flop)
    {
        if(wave_header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE)
            wave_header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE; // flip it to negative
        else
            wave_header.LaserBoy_wave_mode |= LASERBOY_WAVE_POSITIVE; // flip it to positive
    }
    //------------------------------------------------------------------------
    if(wave_header.LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
        for(u_int i = 0; i < wave_header.num_channels; i++)
            wave_header.signal_id[i] = abs(wave_header.signal_id[i]);
    //------------------------------------------------------------------------
    wave_header.to_fstream_wave(wave_fstream);
    wave_fstream.close();
    return true;
}

//############################################################################
bool LaserBoy_space::apply_color_rescales_prep(const string& file)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wave_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header(wave_in);
    if(    wave_in_header.version == "!LaserBoy!"
        || wave_in_header.num_channels < 6
      )
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample(wave_in_header.num_channels);
    //------------------------------------------------------------------------
    wave_out.open(file.c_str(), ios::out | ios::binary);
    if(!wave_out.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    apply_color_rescales(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::omit_color_rescales(const string& file)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wave_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header(wave_in);
    if(    wave_in_header.version == "!LaserBoy!"
        || wave_in_header.num_channels < 6
      )
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample(wave_in_header.num_channels);
    //------------------------------------------------------------------------
    wave_out.open(file.c_str(), ios::out | ios::binary);
    if(!wave_out.is_open())
    {
        undo_wave_temp(wave_in, wave_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample sample(wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.LaserBoy_wave_mode &= ~LASERBOY_COLOR_RESCALE_R;
    wave_out_header.LaserBoy_wave_mode &= ~LASERBOY_COLOR_RESCALE_G;
    wave_out_header.LaserBoy_wave_mode &= ~LASERBOY_COLOR_RESCALE_B;
    wave_out_header.LaserBoy_wave_mode &= ~LASERBOY_COLOR_RESCALE_I;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("omitting color rescale in wave");
    while(sample.from_fstream_wave(wave_in))
    {
        sample.to_fstream_wave(wave_out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::save_color_rescales(const string& file, const string& out_name)
{
    bool      saved_something = false;
    int       i;
    fstream   wave_fstream;
    ofstream  out;
    //------------------------------------------------------------------------
    wave_fstream.open(file.c_str(), ios::in | ios::binary);
    if(!wave_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_header(wave_fstream);
    if(wave_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    if(wave_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
    {
        out.open((out_name + "_red_rescale.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wave\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wave_header.color_rescale_r[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wave_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
    {
        out.open((out_name + "_green_rescale.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wave\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wave_header.color_rescale_g[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wave_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
    {
        out.open((out_name + "_blue_rescale.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wave\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wave_header.color_rescale_b[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wave_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
    {
        out.open((out_name + "_aux_rescale.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wave\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wave_header.color_rescale_i[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    wave_fstream.close();
    return saved_something;
}

//############################################################################
bool LaserBoy_space::black_level_to_wave_prep(const string& file_in, const string& file_out)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    wave_in.open(file_in.c_str(), ios::in | ios::binary);
    LaserBoy_wave_header wave_in_header(wave_in);
    //------------------------------------------------------------------------
    if(wave_in_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    wave_out.open(file_out.c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    black_level_to_wave(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    return true;
}

//############################################################################
bool LaserBoy_space::bit_resolution_to_wave_prep(const string& file_in, const string& file_out)
{
    fstream  wave_in;
    fstream  wave_out;
    //------------------------------------------------------------------------
    wave_in.open(file_in.c_str(), ios::in | ios::binary);
    LaserBoy_wave_header wave_in_header(wave_in);
    //------------------------------------------------------------------------
    if(wave_in_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    wave_out.open(file_out.c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    bit_resolution_to_wave(wave_in, wave_out, wave_in_header);
    //------------------------------------------------------------------------
    wave_in.close();
    wave_out.close();
    return true;
}

//############################################################################
bool LaserBoy_space::split_wave_XY_r_g_b_i_LR_prep(      string file,
                                                   const string& dir_wave,
                                                   const string& dir_unformatted,
                                                   const string& dir_audio
                                                  )
{
    fstream        wave_in,
                   wave_out_XY,
                   wave_out_r,
                   wave_out_g,
                   wave_out_b,
                   wave_out_i,
                   wave_out_LR;
    //------------------------------------------------------------------------
    wave_in.open((dir_wave + file).c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header(wave_in);
    if(wave_in_header.num_channels < 6)
    {
        wave_in.close();
        return false;
    }
    //------------------------------------------------------------------------
    file = file.substr(0, file.size() - 4); // remove .wav
    //------------------------------------------------------------------------
    if(save_fmtd_wav_splits)
        wave_out_XY.open((dir_wave + file + "_XY.wav").c_str(), ios::out | ios::binary);
    else
        wave_out_XY.open((dir_unformatted + file + "_XY.wav").c_str(), ios::out | ios::binary);
    wave_out_r.open ((dir_unformatted + file + "__r.wav").c_str(), ios::out | ios::binary);
    wave_out_g.open ((dir_unformatted + file + "__g.wav").c_str(), ios::out | ios::binary);
    wave_out_b.open ((dir_unformatted + file + "__b.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(wave_in_header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
        wave_out_i.open((dir_unformatted + file + "__Z.wav").c_str(), ios::out | ios::binary);
    else
        wave_out_i.open((dir_unformatted + file + "__i.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(wave_in_header.num_channels == 8)
        wave_out_LR.open((dir_audio + file + "_LR.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(    wave_out_XY.is_open()
        && wave_out_r.is_open()
        && wave_out_g.is_open()
        && wave_out_b.is_open()
        && wave_out_i.is_open()
        && (    wave_out_LR.is_open()
             || wave_in_header.num_channels != 8
           )
      )
    {
        split_wave_XY_r_g_b_i_LR(wave_in_header,
                                 wave_in,
                                 wave_out_XY,
                                 wave_out_r,
                                 wave_out_g,
                                 wave_out_b,
                                 wave_out_i,
                                 wave_out_LR
                                );
        //--------------------------------------------------------------------
        wave_in.close();
        wave_out_XY.close();
        wave_out_r.close();
        wave_out_g.close();
        wave_out_b.close();
        wave_out_i.close();
        if(wave_in_header.num_channels == 8)
            wave_out_LR.close();
    }
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_space::split_wave_XY_rg_bi_LR_prep(      string  file,
                                                 const string& dir_wave,
                                                 const string& dir_unformatted,
                                                 const string& dir_audio
                                                )
{
    fstream        wave_in;
    fstream        wave_out_XY,
                   wave_out_rg,
                   wave_out_bi,
                   wave_out_LR;
    //------------------------------------------------------------------------
    wave_in.open((dir_wave + file).c_str(), ios::in | ios::binary);
    if(!wave_in.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header(wave_in);
    if(wave_in_header.num_channels < 6)
    {
        wave_in.close();
        return false;
    }
    //------------------------------------------------------------------------
    file = file.substr(0, file.size() - 4); // remove .wav
    //------------------------------------------------------------------------
    if(save_fmtd_wav_splits)
        wave_out_XY.open((dir_wave + file + "_XY.wav").c_str(), ios::out | ios::binary);
    else
        wave_out_XY.open((dir_unformatted + file + "_XY.wav").c_str(), ios::out | ios::binary);
    wave_out_rg.open((dir_unformatted + file + "_rg.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(wave_in_header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
        wave_out_bi.open((dir_unformatted + file + "_bZ.wav").c_str(), ios::out | ios::binary);
    else
        wave_out_bi.open((dir_unformatted + file + "_bi.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(wave_in_header.num_channels == 8)
        wave_out_LR.open((dir_audio + file + "_LR.wav").c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(    wave_out_XY.is_open()
        && wave_out_rg.is_open()
        && wave_out_bi.is_open()
        && (    wave_out_LR.is_open()
             || wave_in_header.num_channels != 8
           )
      )
    {
        split_wave_XY_rg_bi_LR(wave_in_header,
                               wave_in,
                               wave_out_XY,
                               wave_out_rg,
                               wave_out_bi,
                               wave_out_LR
                              );
        //--------------------------------------------------------------------
        wave_in.close();
        wave_out_XY.close();
        wave_out_rg.close();
        wave_out_bi.close();
        if(wave_in_header.num_channels == 8)
            wave_out_LR.close();
    }
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_space::join_XY_r_g_b_i_waves_prep(const string& file_XY,
                                                const string& file_r,
                                                const string& file_g,
                                                const string& file_b,
                                                const string& file_i,
                                                const string& file_out,
                                                bool   global_polarity
                                               )
{
    fstream    in_XY,
               in_r,
               in_g,
               in_b,
               in_i;
    //------------------------------------------------------------------------
    in_XY.open(file_XY.c_str(), ios::in | ios::binary);
    in_r .open(file_r .c_str(), ios::in | ios::binary);
    in_g .open(file_g .c_str(), ios::in | ios::binary);
    in_b .open(file_b .c_str(), ios::in | ios::binary);
    in_i .open(file_i .c_str(), ios::in | ios::binary);
    //------------------------------------------------------------------------
    if(    in_XY.is_open()
        && in_r .is_open()
        && in_g .is_open()
        && in_b .is_open()
        && in_i .is_open()
      )
    {
        fstream out;
        out.open(file_out.c_str(), ios::out | ios::binary);
        //--------------------------------------------------------------------
        join_XY_r_g_b_i_waves(in_XY, in_r, in_g, in_b, in_i, out, global_polarity);
        //--------------------------------------------------------------------
        in_XY.close();
        in_r .close();
        in_g .close();
        in_b .close();
        in_i .close();
        out  .close();
    }
    //------------------------------------------------------------------------
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_space::join_XY_rg_b_waves_prep(const string& file_XY,
                                             const string& file_rg,
                                             const string& file_b,
                                             const string& file_out,
                                             bool   global_polarity
                                            )
{
    fstream    in_XY,
               in_rg,
               in_b;
    //------------------------------------------------------------------------
    in_XY.open(file_XY.c_str(), ios::in | ios::binary);
    in_rg.open(file_rg.c_str(), ios::in | ios::binary);
    in_b .open(file_b .c_str(), ios::in | ios::binary);
    //------------------------------------------------------------------------
    if(    in_XY.is_open()
        && in_rg.is_open()
        && in_b .is_open()
      )
    {
        fstream out;
        out.open(file_out.c_str(), ios::out | ios::binary);
        //--------------------------------------------------------------------
        join_XY_rg_b_waves(in_XY, in_rg, in_b, out, global_polarity);
        //--------------------------------------------------------------------
        in_XY.close();
        in_rg.close();
        in_b .close();
        out  .close();
    }
    //------------------------------------------------------------------------
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_space::join_XY_rg_b_LR_waves_prep(const string& file_XY,
                                                const string& file_rg,
                                                const string& file_b,
                                                const string& file_LR,
                                                const string& file_out,
                                                bool   global_polarity
                                               )
{
    fstream    in_XY,
               in_rg,
               in_b;
    //------------------------------------------------------------------------
    in_XY.open(file_XY.c_str(), ios::in | ios::binary);
    in_rg.open(file_rg.c_str(), ios::in | ios::binary);
    in_b .open(file_b .c_str(), ios::in | ios::binary);
    //------------------------------------------------------------------------
    if(file_LR == LASERBOY_AUDIO_SHARE + "_0")
    {
        //--------------------------------------------------------------------
        if(    in_XY.is_open()
            && in_rg.is_open()
            && in_b .is_open()
          )
        {
            fstream out;
            out.open(file_out.c_str(), ios::out | ios::binary);
            //----------------------------------------------------------------
            join_XY_rg_b_00_waves(in_XY, in_rg, in_b, out, global_polarity);
            //----------------------------------------------------------------
            in_XY.close();
            in_rg.close();
            in_b .close();
            out  .close();
        }
        //--------------------------------------------------------------------
        else
            return false;
    }
    else
    {
        fstream    in_LR;
        //--------------------------------------------------------------------
        in_LR.open(file_LR.c_str(), ios::in | ios::binary);
        //--------------------------------------------------------------------
        if(    in_XY.is_open()
            && in_rg.is_open()
            && in_b .is_open()
            && in_LR.is_open()
          )
        {
            fstream out;
            out.open(file_out.c_str(), ios::out | ios::binary);
            //----------------------------------------------------------------
            join_XY_rg_b_LR_waves(in_XY, in_rg, in_b, in_LR, out, global_polarity);
            //----------------------------------------------------------------
            in_XY.close();
            in_rg.close();
            in_b .close();
            in_LR.close();
            out  .close();
        }
        //--------------------------------------------------------------------
        else
            return false;
    }
    return true;
}

//############################################################################
bool LaserBoy_space::add_audio_to_wave_prep(const string& file_laser,
                                            const string& file_audio,
                                            const string& file_out
                                           )
{
    fstream  in_laser,
             in_audio;
    //------------------------------------------------------------------------
    if(file_audio == LASERBOY_AUDIO_SHARE + "_0")
    {
        in_laser.open(file_laser.c_str(), ios::in | ios::binary);
        if(in_laser.is_open())
        {
            LaserBoy_wave_header wave_in_header_laser(in_laser);
            fstream out;
            out.open(file_out.c_str(), ios::out | ios::binary);
            //----------------------------------------------------------------
            add_silence_to_wave(wave_in_header_laser,
                                in_laser,
                                out
                               );
            //----------------------------------------------------------------
            in_laser.close();
            out.close();
            return true;
        } // end if(in_laser.is_open())
        //--------------------------------------------------------------------
        else
            return false;
        //--------------------------------------------------------------------
    } // end if(file_audio == LASERBOY_AUDIO_SHARE + "_0")
    else
    {
        in_laser.open(file_laser.c_str(), ios::in | ios::binary);
        in_audio.open(file_audio.c_str(), ios::in | ios::binary);
        if(    in_laser.is_open()
            && in_audio.is_open()
          )
        {
            LaserBoy_wave_header wave_in_header_laser(in_laser),
                                 wave_in_header_audio(in_audio);

            fstream              out;
            LaserBoy_wave_header wave_out_header(wave_in_header_laser, 0); // 8ch.
            wave_out_header.num_samples = 0;
            //----------------------------------------------------------------
            out.open(file_out.c_str(), ios::out | ios::binary);
            //----------------------------------------------------------------
            add_audio_to_wave(wave_in_header_laser,
                              wave_in_header_audio,
                              in_laser,
                              in_audio,
                              out
                             );
            //----------------------------------------------------------------
            in_laser.close();
            in_audio.close();
            out.close();
            return true;
        } // end if(in_laser.is_open() && in_audio.is_open())
        //--------------------------------------------------------------------
        else
            return false;
        //--------------------------------------------------------------------
    } // end else of if(file_audio == "_0")
    return true;
}

//############################################################################
void LaserBoy_space::apply_wave_offsets(fstream&             wave_in,
                                        fstream&             wave_out,
                                        LaserBoy_wave_header wave_in_header
                                       )
{
    int                            i                     ,
                                   span                  ,
                                   last                  ,
                                   offset_in_max         ,
                                   offset_in_min         ,
                                   offset_applied_max    ,
                                   offset_applied_min    ,
                                   offset_applied     [8];

    LaserBoy_wave_sample_pointer   roll_over             ;
    LaserBoy_wave_sample_pointer  *sample_window         ;
    LaserBoy_wave_sample           wave_sample(wave_in_header.num_channels);
    LaserBoy_wave_header           wave_out_header(wave_in_header);
    //------------------------------------------------------------------------
    wave_out_header.num_samples = 0;
    wave_out_header.LaserBoy_wave_mode |= LASERBOY_WAVE_OFFSETS;
    //------------------------------------------------------------------------
    for(i = 0; i < wave_in_header.num_channels; i++)
        offset_applied[i] = wave_in_header.offset[i] - wave_offsets[i];
    //------------------------------------------------------------------------
    offset_in_min      = wave_offsets  [0];
    offset_in_max      = wave_offsets  [0];
    offset_applied_min = offset_applied[0];
    offset_applied_max = offset_applied[0];
    //------------------------------------------------------------------------
    for(i = 0; i < wave_in_header.num_channels; i++)
    {
        if(wave_offsets  [i] > offset_in_max     ) offset_in_max      = wave_offsets  [i];
        if(wave_offsets  [i] < offset_in_min     ) offset_in_min      = wave_offsets  [i];
        if(offset_applied[i] > offset_applied_max) offset_applied_max = offset_applied[i];
        if(offset_applied[i] < offset_applied_min) offset_applied_min = offset_applied[i];
    }
    //------------------------------------------------------------------------
    span = (offset_applied_max - offset_applied_min) + 1;
    last = span - 1;
    //------------------------------------------------------------------------
    for(i = 0; i < wave_in_header.num_channels; i++)
    {
        wave_out_header.offset[i] = -(offset_in_max - wave_offsets[i]);
        offset_applied[i] = offset_applied_max - offset_applied[i];
    }
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(wave_in_header.num_channels); // span element array of (6 or 8 channel LaserBoy_wave_samples)
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("applying sample offsets");
    while(true)
    {
        if(last) // span > 1
        {
            roll_over = sample_window[0];
            for(i = 0; i < last; i++)
                sample_window[i] = sample_window[i + 1];
            sample_window[last] = roll_over;
        }
        if(!sample_window[last]->from_fstream_wave(wave_in))
            break;
        for(i = 0; i < wave_in_header.num_channels; i++) // each channel
            wave_sample.channel[i] = sample_window[offset_applied[i]]->channel[i];
        wave_sample.to_fstream_wave(wave_out, wave_out_header); // write it to disk
        if(!(wave_out_header.num_samples % 48000))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    delete sample_window;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::invert_wave(fstream&             wave_in,
                                 fstream&             wave_out,
                                 LaserBoy_wave_header wave_in_header
                                )
{
    LaserBoy_wave_sample wave_sample    (wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    if(wave_in_header.version != "!LaserBoy!")
    {
        if(wave_in_header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE)
            wave_out_header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE;
        else
            wave_out_header.LaserBoy_wave_mode |= LASERBOY_WAVE_POSITIVE;
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("inverting wave");
    while(wave_sample.from_fstream_wave_inverted(wave_in))
    {
        wave_sample.to_fstream_wave(wave_out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::invert_signals(fstream&             wave_in,
                                    fstream&             wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   )
{
    LaserBoy_wave_sample wave_sample    (wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    if(wave_in_header.version != "!LaserBoy!")
    {
        wave_out_header.LaserBoy_wave_mode |= LASERBOY_WAVE_SIGNAL_MATRIX;
        for(u_int i = 0; i < wave_out_header.num_channels; i++)
            if(signal_polarity[i] == 1)
                wave_out_header.signal_id[i] = -wave_out_header.signal_id[i];
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("inverting signals");
    while(wave_sample.from_fstream_wave_polarity(wave_in, signal_polarity))
    {
        wave_sample.to_fstream_wave(wave_out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::black_level_to_wave(fstream&             wave_in,
                                         fstream&             wave_out,
                                         LaserBoy_wave_header wave_in_header
                                        )
{
static const int // channel tags
    red   = 2,
    green = 3,
    blue  = 4;

    u_int r,
          g,
          b;
    //------------------------------------------------------------------------
    wave_in_header.LaserBoy_wave_mode |= LASERBOY_WAVE_SIGNAL_MATRIX;
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample    (wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("imposing black level on wave");
    if(wave_in_header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE)
        while(wave_sample.from_fstream_wave(wave_in))
        {
            r = g = b = 0;
            //----------------------------------------------------------------
            if(wave_sample.channel[red  ] > 0)
                r = wave_sample.channel[red  ] >> 7;
            //----------------------------------------------------------------
            if(wave_sample.channel[green] > 0)
                g = wave_sample.channel[green] >> 7;
            //----------------------------------------------------------------
            if(wave_sample.channel[blue ] > 0)
                b = wave_sample.channel[blue ] >> 7;
            //----------------------------------------------------------------
            if(((r + g + b) / 3) < black_level)
                r = g = b = 0;
            //----------------------------------------------------------------
            wave_sample.channel[red  ] = (r << 7);
            wave_sample.channel[green] = (g << 7);
            wave_sample.channel[blue ] = (b << 7);
            wave_sample.to_fstream_wave(wave_out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
        }
    else
        while(wave_sample.from_fstream_wave_inverted(wave_in))
        {
            r = g = b = 0;
            //----------------------------------------------------------------
            if(wave_sample.channel[red  ] > 0)
                r = wave_sample.channel[red  ] >> 7;
            //----------------------------------------------------------------
            if(wave_sample.channel[green] > 0)
                g = wave_sample.channel[green] >> 7;
            //----------------------------------------------------------------
            if(wave_sample.channel[blue ] > 0)
                b = wave_sample.channel[blue ] >> 7;
            //----------------------------------------------------------------
            if(((r + g + b) / 3) < black_level)
                r = g = b = 0;
            //----------------------------------------------------------------
            wave_sample.channel[red  ] = (r << 7);
            wave_sample.channel[green] = (g << 7);
            wave_sample.channel[blue ] = (b << 7);
            wave_sample.negate();
            wave_sample.to_fstream_wave(wave_out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
        }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::bit_resolution_to_wave(fstream&             wave_in,
                                            fstream&             wave_out,
                                            LaserBoy_wave_header wave_in_header
                                           )
{
    //------------------------------------------------------------------------
    wave_in_header.LaserBoy_wave_mode |= LASERBOY_WAVE_SIGNAL_MATRIX;
    wave_in_header.LaserBoy_wave_mode |= LASERBOY_SIGNAL_BIT_RESOLUTION;
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample    (wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    for(u_int i = 0; i < wave_out_header.num_channels; i++)
        wave_out_header.resolution[i] = 16 - signal_bit_mask[i];
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("bit reducing wave");
    while(wave_sample.from_fstream_wave_bit_resolution(wave_in, signal_bit_mask))
    {
        wave_sample.to_fstream_wave(wave_out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::apply_color_rescales(fstream&             wave_in,
                                          fstream&             wave_out,
                                          LaserBoy_wave_header wave_in_header
                                         )
{
    int i;
    LaserBoy_wave_sample sample_in(wave_in_header.num_channels),
                         sample_out(wave_in_header.num_channels);

    LaserBoy_wave_header wave_out_header(wave_in_header);
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.LaserBoy_wave_mode &= ~(    LASERBOY_COLOR_RESCALE_R
                                              | LASERBOY_COLOR_RESCALE_G
                                              | LASERBOY_COLOR_RESCALE_B
                                              | LASERBOY_COLOR_RESCALE_I
                                           ); // clear those bits
    wave_out_header.LaserBoy_wave_mode |= color_rescales_flags(); // set them
    //------------------------------------------------------------------------
    if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        for(i = 0; i < 256; i++)
            wave_out_header.color_rescale_r[i] = color_rescale_r[i];
    //------------------------------------------------------------------------
    if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        for(i = 0; i < 256; i++)
            wave_out_header.color_rescale_g[i] = color_rescale_g[i];
    //------------------------------------------------------------------------
    if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        for(i = 0; i < 256; i++)
            wave_out_header.color_rescale_b[i] = color_rescale_b[i];
    //------------------------------------------------------------------------
    if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
        for(i = 0; i < 256; i++)
            wave_out_header.color_rescale_i[i] = color_rescale_i[i];
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("color rescaling wave");
    while(sample_in.from_fstream_wave(wave_in))
    {
        if(wave_in_header.LaserBoy_wave_mode & LASERBOY_WAVE_NEGATIVE)
            sample_in.negate();
        sample_out = sample_in;
        //--------------------------------------------------------------------
        if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                sample_out.channel[2] = wave_out_header.color_rescale_r[rescale_to_index(wave_in_header.color_rescale_r, sample_in.channel[2])];
            else
                sample_out.channel[2] = wave_out_header.color_rescale_r[(sample_in.channel[2] >> 7) & 0x000000ff];
        }
        else
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                sample_out.channel[2] = rescale_to_index(wave_in_header.color_rescale_r, sample_in.channel[2]) << 7;
            else
                sample_out.channel[2] = sample_in.channel[2];
        }
        //--------------------------------------------------------------------
        if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                sample_out.channel[3] = wave_out_header.color_rescale_g[rescale_to_index(wave_in_header.color_rescale_g, sample_in.channel[3])];
            else
                sample_out.channel[3] = wave_out_header.color_rescale_g[(sample_in.channel[3] >> 7) & 0x000000ff];
        }
        else
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                sample_out.channel[3] = rescale_to_index(wave_in_header.color_rescale_g, sample_in.channel[3]) << 7;
            else
                sample_out.channel[3] = sample_in.channel[3];
        }
        //--------------------------------------------------------------------
        if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                sample_out.channel[4] = wave_out_header.color_rescale_b[rescale_to_index(wave_in_header.color_rescale_b, sample_in.channel[4])];
            else
                sample_out.channel[4] = wave_out_header.color_rescale_b[(sample_in.channel[4] >> 7) & 0x000000ff];
        }
        else
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                sample_out.channel[4] = rescale_to_index(wave_in_header.color_rescale_b, sample_in.channel[4]) << 7;
            else
                sample_out.channel[4] = sample_in.channel[4];
        }
        //--------------------------------------------------------------------
        if(wave_out_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                sample_out.channel[5] = wave_out_header.color_rescale_i[rescale_to_index(wave_in_header.color_rescale_i, sample_in.channel[5])];
            else
                sample_out.channel[5] = wave_out_header.color_rescale_i[(sample_in.channel[5] >> 7) & 0x000000ff];
        }
        else
        {
            if(wave_in_header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                sample_out.channel[5] = rescale_to_index(wave_in_header.color_rescale_i, sample_in.channel[5]) << 7;
            else
                sample_out.channel[5] = sample_in.channel[5];
        }
        //--------------------------------------------------------------------
        if(wave_in_header.LaserBoy_wave_mode & LASERBOY_WAVE_NEGATIVE)
            sample_out.negate();
        sample_out.to_fstream_wave(wave_out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(wave_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::split_wave_XY_r_g_b_i_LR(LaserBoy_wave_header wave_in_header,
                                              fstream&             wave_in,
                                              fstream&             wave_out_XY,
                                              fstream&             wave_out_r,
                                              fstream&             wave_out_g,
                                              fstream&             wave_out_b,
                                              fstream&             wave_out_i,
                                              fstream&             wave_out_LR
                                             )
{
    LaserBoy_wave_header  wave_out_header_XY(wave_in_header.sample_rate, wave_in_header.LaserBoy_wave_mode, 2, LASERBOY_WAVE_VERSION),
                          wave_out_header_r (wave_in_header.sample_rate, wave_in_header.LaserBoy_wave_mode, 1, LASERBOY_WAVE_VERSION),
                          wave_out_header_g (wave_in_header.sample_rate, wave_in_header.LaserBoy_wave_mode, 1, LASERBOY_WAVE_VERSION),
                          wave_out_header_b (wave_in_header.sample_rate, wave_in_header.LaserBoy_wave_mode, 1, LASERBOY_WAVE_VERSION),
                          wave_out_header_i (wave_in_header.sample_rate, wave_in_header.LaserBoy_wave_mode, 1, LASERBOY_WAVE_VERSION),
                          wave_out_header_LR(wave_in_header.sample_rate, LASERBOY_WAVE_NO_MODE            , 2, "!LaserBoy!");

    LaserBoy_wave_sample  wave_sample(wave_in_header.num_channels),
                          stereo_sample(2),
                          mono_sample  (1);
    //------------------------------------------------------------------------
    p_GUI->display_state("splitting wave");

    if(save_fmtd_wav_splits)
    {
        wave_out_header_XY.resolution[0] = 14;
        wave_out_header_XY.resolution[1] = 14;
        wave_out_header_XY.LSB_tag[0] = LASERBOY_LSB_BLANKING;
        wave_out_header_XY.LSB_tag[1] = LASERBOY_LSB_END_OF_FRAME;
    }
    wave_out_header_XY.to_fstream_wave(wave_out_XY);
    wave_out_header_r.to_fstream_wave (wave_out_r );
    wave_out_header_g.to_fstream_wave (wave_out_g );
    wave_out_header_b.to_fstream_wave (wave_out_b );
    if(    wave_in_header.signal_id[5] != LASERBOY_SIGNAL_UNDEFINED
        || wave_in_header.signal_id[5] != LASERBOY_NO_SIGNAL
      )
        wave_out_header_i.to_fstream_wave(wave_out_i);
    if(wave_in_header.num_channels == 8)
        wave_out_header_LR.to_fstream_wave(wave_out_LR);
    //------------------------------------------------------------------------
    if(save_fmtd_wav_splits)
        while(wave_sample.from_fstream_wave(wave_in))
        {
            stereo_sample.channel[0] = wave_sample.channel[0] & 0xfffd; // clear the 2 LSB
            stereo_sample.channel[1] = wave_sample.channel[1] & 0xfffd;
            if(   (   wave_sample.channel[2] // red
                    + wave_sample.channel[3] // green
                    + wave_sample.channel[4] // blue
                  )
                > (int)black_level
              )
                stereo_sample.channel[1] |= 0x0001; // it's lit!
            if(wave_sample.channel[2] && 0x0001) // red channel end_of_frame
                stereo_sample.channel[1] |= 0x0001;
            stereo_sample.to_fstream_wave(wave_out_XY, wave_out_header_XY);

            mono_sample.channel[0] = wave_sample.channel[2];
            mono_sample.to_fstream_wave(wave_out_r, wave_out_header_r);

            mono_sample.channel[0] = wave_sample.channel[3];
            mono_sample.to_fstream_wave(wave_out_g, wave_out_header_g);

            mono_sample.channel[0] = wave_sample.channel[4];
            mono_sample.to_fstream_wave(wave_out_b, wave_out_header_b);

            if(    wave_in_header.signal_id[5] != LASERBOY_SIGNAL_UNDEFINED
                || wave_in_header.signal_id[5] != LASERBOY_NO_SIGNAL
              )
            {
                mono_sample.channel[0] = wave_sample.channel[5];
                mono_sample.to_fstream_wave(wave_out_i, wave_out_header_i);
            }
            //----------------------------------------------------------------
            if(wave_in_header.num_channels == 8)
            {
                stereo_sample.channel[0] = wave_sample.channel[6];
                stereo_sample.channel[1] = wave_sample.channel[7];
                stereo_sample.to_fstream_wave(wave_out_LR, wave_out_header_LR);
            }
            if(!(wave_out_header_XY.num_samples % 4800))
                p_GUI->display_progress(wave_in_header.num_samples - wave_out_header_XY.num_samples);
        }
    //------------------------------------------------------------------------
    else
        while(wave_sample.from_fstream_wave(wave_in))
        {
            stereo_sample.channel[0] = wave_sample.channel[0];
            stereo_sample.channel[1] = wave_sample.channel[1];
            stereo_sample.to_fstream_wave(wave_out_XY, wave_out_header_XY);

            mono_sample.channel[0] = wave_sample.channel[2];
            mono_sample.to_fstream_wave(wave_out_r, wave_out_header_r);

            mono_sample.channel[0] = wave_sample.channel[3];
            mono_sample.to_fstream_wave(wave_out_g, wave_out_header_g);

            mono_sample.channel[0] = wave_sample.channel[4];
            mono_sample.to_fstream_wave(wave_out_b, wave_out_header_b);

            if(    wave_in_header.signal_id[5] != LASERBOY_SIGNAL_UNDEFINED
                || wave_in_header.signal_id[5] != LASERBOY_NO_SIGNAL
              )
            {
                mono_sample.channel[0] = wave_sample.channel[5];
                mono_sample.to_fstream_wave(wave_out_i, wave_out_header_i);
            }
            //----------------------------------------------------------------
            if(wave_in_header.num_channels == 8)
            {
                stereo_sample.channel[0] = wave_sample.channel[6];
                stereo_sample.channel[1] = wave_sample.channel[7];
                stereo_sample.to_fstream_wave(wave_out_LR, wave_out_header_LR);
            }
            if(!(wave_out_header_XY.num_samples % 4800))
                p_GUI->display_progress(wave_in_header.num_samples - wave_out_header_XY.num_samples);
        }
    //------------------------------------------------------------------------
    wave_out_header_XY.to_fstream_wave(wave_out_XY);
    wave_out_header_r.to_fstream_wave (wave_out_r );
    wave_out_header_g.to_fstream_wave (wave_out_g );
    wave_out_header_b.to_fstream_wave (wave_out_b );
    if(    wave_in_header.signal_id[5] != LASERBOY_SIGNAL_UNDEFINED
        || wave_in_header.signal_id[5] != LASERBOY_NO_SIGNAL
      )
        wave_out_header_i.to_fstream_wave(wave_out_i);
    if(wave_in_header.num_channels == 8)
        wave_out_header_LR.to_fstream_wave(wave_out_LR);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::split_wave_XY_rg_bi_LR(LaserBoy_wave_header wave_in_header,
                                            fstream&             wave_in,
                                            fstream&             wave_out_XY,
                                            fstream&             wave_out_rg,
                                            fstream&             wave_out_bi,
                                            fstream&             wave_out_LR
                                           )
{
    LaserBoy_wave_header  wave_out_header_XY(wave_in_header.sample_rate, LASERBOY_WAVE_NO_MODE, 2, "!LaserBoy!"),
                          wave_out_header_rg(wave_in_header.sample_rate, LASERBOY_WAVE_NO_MODE, 2, "!LaserBoy!"),
                          wave_out_header_bi(wave_in_header.sample_rate, LASERBOY_WAVE_NO_MODE, 2, "!LaserBoy!"),
                          wave_out_header_LR(wave_in_header.sample_rate, LASERBOY_WAVE_NO_MODE, 2, "!LaserBoy!");

    LaserBoy_wave_sample  stereo_sample(2);
    //------------------------------------------------------------------------
    LaserBoy_wave_sample wave_sample(wave_in_header.num_channels);
    //------------------------------------------------------------------------
    p_GUI->display_state("splitting wave");
    while(wave_sample.from_fstream_wave(wave_in))
    {
        stereo_sample.channel[0] = wave_sample.channel[0];
        stereo_sample.channel[1] = wave_sample.channel[1];
        stereo_sample.to_fstream_wave(wave_out_XY, wave_out_header_XY);

        stereo_sample.channel[0] = wave_sample.channel[2];
        stereo_sample.channel[1] = wave_sample.channel[3];
        stereo_sample.to_fstream_wave(wave_out_rg, wave_out_header_rg);

        stereo_sample.channel[0] = wave_sample.channel[4];
        stereo_sample.channel[1] = wave_sample.channel[5];
        stereo_sample.to_fstream_wave(wave_out_bi, wave_out_header_bi);
        //--------------------------------------------------------------------
        if(wave_in_header.num_channels == 8)
        {
            stereo_sample.channel[0] = wave_sample.channel[6];
            stereo_sample.channel[1] = wave_sample.channel[7];
            stereo_sample.to_fstream_wave(wave_out_LR, wave_out_header_LR);
        }
        if(!(wave_out_header_XY.num_samples % 4800))
            p_GUI->display_progress(wave_in_header.num_samples - wave_out_header_XY.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header_XY.to_fstream_wave(wave_out_XY);
    wave_out_header_rg.to_fstream_wave(wave_out_rg);
    wave_out_header_bi.to_fstream_wave(wave_out_bi);
    //------------------------------------------------------------------------
    if(wave_in_header.num_channels == 8)
        wave_out_header_LR.to_fstream_wave(wave_out_LR);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::join_XY_r_g_b_i_waves(fstream& in_XY,
                                           fstream& in_r,
                                           fstream& in_g,
                                           fstream& in_b,
                                           fstream& in_i,
                                           fstream& out,
                                           bool     global_polartity
                                          )
{
    LaserBoy_wave_sample sample_XYrgbi(6),
                         sample_XY    (2),
                         sample_r     (1),
                         sample_g     (1),
                         sample_b     (1),
                         sample_i     (1);
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header_XY(in_XY),
                         wave_in_header_r (in_r ),
                         wave_in_header_g (in_g ),
                         wave_in_header_b (in_b ),
                         wave_in_header_i (in_i );
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_out_header  (  wave_in_header_XY.sample_rate,
                                             ((global_polartity)?(LASERBOY_WAVE_NEGATIVE):(LASERBOY_WAVE_POSITIVE))
                                           | LASERBOY_WAVE_OFFSETS
                                           | LASERBOY_WAVE_SIGNAL_MATRIX
                                           | LASERBOY_SIGNAL_BIT_RESOLUTION,
                                           6,
                                           LASERBOY_WAVE_VERSION
                                          );
    //------------------------------------------------------------------------
    wave_out_header.offset    [0] = wave_offsets[0];
    wave_out_header.offset    [1] = wave_offsets[1];
    wave_out_header.offset    [2] = wave_offsets[2];
    wave_out_header.offset    [3] = wave_offsets[3];
    wave_out_header.offset    [4] = wave_offsets[4];
    wave_out_header.offset    [5] = wave_offsets[5];

    wave_out_header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION  ;
    wave_out_header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION  ;
    wave_out_header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG  ;
    wave_out_header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    wave_out_header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG ;
    wave_out_header.signal_id [5] = LASERBOY_SIGNAL_UNDEFINED   ;

    wave_out_header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [3] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;

    wave_out_header.resolution[0] = 16;
    wave_out_header.resolution[1] = 16;
    wave_out_header.resolution[2] =  9;
    wave_out_header.resolution[3] =  9;
    wave_out_header.resolution[4] =  9;
    wave_out_header.resolution[5] = 16;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("joining waves");
    while(    sample_XY.from_fstream_wave(in_XY)
           && sample_r .from_fstream_wave(in_r )
           && sample_g .from_fstream_wave(in_g )
           && sample_b .from_fstream_wave(in_b )
           && sample_i .from_fstream_wave(in_i )
         )
    {
        sample_XYrgbi =   sample_XY
                        + sample_r
                        + sample_g
                        + sample_b
                        + sample_i; // isn't that neat!

        sample_XYrgbi.to_fstream_wave(out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}

//############################################################################
void LaserBoy_space::join_XY_rg_b_waves(fstream& in_XY,
                                        fstream& in_rg,
                                        fstream& in_b,
                                        fstream& out,
                                        bool     global_polartity
                                       )
{
    LaserBoy_wave_header wave_in_header_XY(in_XY),
                         wave_in_header_rg(in_rg),
                         wave_in_header_b (in_b );
    //------------------------------------------------------------------------
    LaserBoy_wave_sample sample_XYrgb (6),
                         sample_XY    (2),
                         sample_rg    (2),
                         sample_bi    (2),
                         sample_b     (1),
                         sample_0     (1); // silent sample
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_out_header  (  wave_in_header_XY.sample_rate,
                                             ((global_polartity)?(LASERBOY_WAVE_NEGATIVE):(LASERBOY_WAVE_POSITIVE))
                                           | LASERBOY_WAVE_OFFSETS
                                           | LASERBOY_WAVE_SIGNAL_MATRIX
                                           | LASERBOY_SIGNAL_BIT_RESOLUTION,
                                           6,
                                           LASERBOY_WAVE_VERSION
                                          );
    //------------------------------------------------------------------------
    wave_out_header.offset    [0] = wave_offsets[0];
    wave_out_header.offset    [1] = wave_offsets[1];
    wave_out_header.offset    [2] = wave_offsets[2];
    wave_out_header.offset    [3] = wave_offsets[3];
    wave_out_header.offset    [4] = wave_offsets[4];
    wave_out_header.offset    [5] = wave_offsets[5];

    wave_out_header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION  ;
    wave_out_header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION  ;
    wave_out_header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG  ;
    wave_out_header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    wave_out_header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG ;
    wave_out_header.signal_id [5] = LASERBOY_SIGNAL_UNDEFINED   ;

    wave_out_header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [3] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;

    wave_out_header.resolution[0] = 16;
    wave_out_header.resolution[1] = 16;
    wave_out_header.resolution[2] =  9;
    wave_out_header.resolution[3] =  9;
    wave_out_header.resolution[4] =  9;
    wave_out_header.resolution[5] = 16;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("joining waves");
    if(wave_in_header_b.num_channels == 2)
        while(    sample_XY.from_fstream_wave(in_XY)
               && sample_rg.from_fstream_wave(in_rg)
               && sample_bi.from_fstream_wave(in_b )
             )
        {
            sample_XYrgb =   sample_XY
                           + sample_rg
                           + sample_bi; // isn't that neat!

            sample_XYrgb.to_fstream_wave(out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
        }
    else
        while(    sample_XY.from_fstream_wave(in_XY)
               && sample_rg.from_fstream_wave(in_rg)
               && sample_b .from_fstream_wave(in_b )
             )
        {
            sample_XYrgb =   sample_XY
                           + sample_rg
                           + sample_b
                           + sample_0; // isn't that neat!

            sample_XYrgb.to_fstream_wave(out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
        }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}

//############################################################################
void LaserBoy_space::join_XY_rg_b_LR_waves(fstream& in_XY,
                                           fstream& in_rg,
                                           fstream& in_b,
                                           fstream& in_LR,
                                           fstream& out,
                                           bool     global_polartity
                                          )
{
    LaserBoy_wave_header wave_in_header_XY(in_XY),
                         wave_in_header_rg(in_rg),
                         wave_in_header_LR(in_LR),
                         wave_in_header_b (in_b );
    //------------------------------------------------------------------------
    LaserBoy_wave_sample sample_XYrgb_LR (8),
                         sample_XY       (2),
                         sample_rg       (2),
                         sample_bi       (2),
                         sample_b        (1),
                         sample_0        (1), // silent sample
                         sample_LR       (2);
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_out_header  (  wave_in_header_XY.sample_rate,
                                             ((global_polartity)?(LASERBOY_WAVE_NEGATIVE):(LASERBOY_WAVE_POSITIVE))
                                           | LASERBOY_WAVE_OFFSETS
                                           | LASERBOY_WAVE_SIGNAL_MATRIX
                                           | LASERBOY_SIGNAL_BIT_RESOLUTION,
                                           8, // channels!
                                           LASERBOY_WAVE_VERSION
                                          );
    //------------------------------------------------------------------------
    wave_out_header.offset    [0] = wave_offsets[0];
    wave_out_header.offset    [1] = wave_offsets[1];
    wave_out_header.offset    [2] = wave_offsets[2];
    wave_out_header.offset    [3] = wave_offsets[3];
    wave_out_header.offset    [4] = wave_offsets[4];
    wave_out_header.offset    [5] = wave_offsets[5];
    wave_out_header.offset    [6] = wave_offsets[6];
    wave_out_header.offset    [7] = wave_offsets[7];

    wave_out_header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION  ;
    wave_out_header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION  ;
    wave_out_header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG  ;
    wave_out_header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    wave_out_header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG ;
    wave_out_header.signal_id [5] = LASERBOY_SIGNAL_UNDEFINED   ;
    wave_out_header.signal_id [6] = LASERBOY_SIGNAL_AUDIO_LEFT  ;
    wave_out_header.signal_id [7] = LASERBOY_SIGNAL_AUDIO_RIGHT ;

    wave_out_header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [3] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;

    wave_out_header.resolution[0] = 16;
    wave_out_header.resolution[1] = 16;
    wave_out_header.resolution[2] =  9;
    wave_out_header.resolution[3] =  9;
    wave_out_header.resolution[4] =  9;
    wave_out_header.resolution[5] = 16;
    wave_out_header.resolution[6] = 16;
    wave_out_header.resolution[7] = 16;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("joining waves");
    if(wave_in_header_b.num_channels == 2)
        while(    sample_XY.from_fstream_wave(in_XY)
               && sample_rg.from_fstream_wave(in_rg)
               && sample_bi.from_fstream_wave(in_b )
               && sample_LR.from_fstream_wave(in_LR)
             )
        {
            sample_XYrgb_LR =   sample_XY
                              + sample_rg
                              + sample_bi
                              + sample_LR; // isn't that neat!

            sample_XYrgb_LR.to_fstream_wave(out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
        }
    else
        while(    sample_XY.from_fstream_wave(in_XY)
               && sample_rg.from_fstream_wave(in_rg)
               && sample_b .from_fstream_wave(in_b )
               && sample_LR.from_fstream_wave(in_LR)
             )
        {
            sample_XYrgb_LR =   sample_XY
                              + sample_rg
                              + sample_b
                              + sample_0
                              + sample_LR; // isn't that neat!

            sample_XYrgb_LR.to_fstream_wave(out, wave_out_header);
            if(!(wave_out_header.num_samples % 4800))
                p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
        }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}

//############################################################################
void LaserBoy_space::join_XY_rg_b_00_waves(fstream& in_XY,
                                           fstream& in_rg,
                                           fstream& in_b,
                                           fstream& out,
                                           bool     global_polartity
                                          )
{
    LaserBoy_wave_sample sample_XYrgb_00 (8),
                         sample_XY       (2),
                         sample_rg       (2),
                         sample_b        (1),
                         sample_0        (1), // silent sample
                         sample_00       (2);
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_in_header_XY(in_XY),
                         wave_in_header_rg(in_rg),
                         wave_in_header_b (in_b );
    //------------------------------------------------------------------------
    LaserBoy_wave_header wave_out_header  (  wave_in_header_XY.sample_rate,
                                             ((global_polartity)?(LASERBOY_WAVE_NEGATIVE):(LASERBOY_WAVE_POSITIVE))
                                           | LASERBOY_WAVE_OFFSETS
                                           | LASERBOY_WAVE_SIGNAL_MATRIX
                                           | LASERBOY_SIGNAL_BIT_RESOLUTION,
                                           8, // channels!
                                           LASERBOY_WAVE_VERSION
                                          );
    //------------------------------------------------------------------------
    wave_out_header.offset    [0] = wave_offsets[0];
    wave_out_header.offset    [1] = wave_offsets[1];
    wave_out_header.offset    [2] = wave_offsets[2];
    wave_out_header.offset    [3] = wave_offsets[3];
    wave_out_header.offset    [4] = wave_offsets[4];
    wave_out_header.offset    [5] = wave_offsets[5];
    wave_out_header.offset    [6] = 0;
    wave_out_header.offset    [7] = 0;

    wave_out_header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION  ;
    wave_out_header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION  ;
    wave_out_header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG  ;
    wave_out_header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    wave_out_header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG ;
    wave_out_header.signal_id [5] = LASERBOY_SIGNAL_UNDEFINED   ;
    wave_out_header.signal_id [6] = LASERBOY_SIGNAL_AUDIO_LEFT  ;
    wave_out_header.signal_id [7] = LASERBOY_SIGNAL_AUDIO_RIGHT ;

    wave_out_header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [3] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;

    wave_out_header.resolution[0] = 16;
    wave_out_header.resolution[1] = 16;
    wave_out_header.resolution[2] =  9;
    wave_out_header.resolution[3] =  9;
    wave_out_header.resolution[4] =  9;
    wave_out_header.resolution[5] = 16;
    wave_out_header.resolution[6] = 16;
    wave_out_header.resolution[7] = 16;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("joining waves");
    while(    sample_XY.from_fstream_wave(in_XY)
           && sample_rg.from_fstream_wave(in_rg)
           && sample_b .from_fstream_wave(in_b )
         )
    {
        sample_XYrgb_00 =   sample_XY
                          + sample_rg
                          + sample_b
                          + sample_0
                          + sample_00; // isn't that neat!

        sample_XYrgb_00.to_fstream_wave(out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header_XY.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}

//############################################################################
void LaserBoy_space::add_audio_to_wave(LaserBoy_wave_header wave_in_header_laser,
                                       LaserBoy_wave_header wave_in_header_audio,
                                       fstream&             in_laser,
                                       fstream&             in_audio,
                                       fstream&             out
                                      )
{
    LaserBoy_wave_sample sample_laser      (6), // 6 chennels
                         sample_audio      (2), // 2 chennels
                         sample_laser_audio(8); // 8 chennels

    LaserBoy_wave_header wave_out_header   (wave_in_header_laser, 0); // 8ch.
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    p_GUI->display_state("adding audio");
    while(    sample_laser.from_fstream_wave(in_laser)
           && sample_audio.from_fstream_wave(in_audio)
         )
    {
        sample_laser_audio = (sample_laser + sample_audio); // isn't that neat!
        sample_laser_audio.to_fstream_wave(out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header_laser.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}

//############################################################################
void LaserBoy_space::add_silence_to_wave(LaserBoy_wave_header wave_in_header_laser,
                                         fstream&             in_laser,
                                         fstream&             out
                                        )
{
    LaserBoy_wave_sample sample_laser      (6), // 6 chennels
                         sample_audio      (2), // 2 chennels (zero)
                         sample_laser_audio(8); // 8 chennels

    LaserBoy_wave_header wave_out_header(wave_in_header_laser, 0); // 8ch.
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    p_GUI->display_state("adding silence");
    while(sample_laser.from_fstream_wave(in_laser))
    {
        sample_laser_audio = (sample_laser + sample_audio); // isn't that neat!
        sample_laser_audio.to_fstream_wave(out, wave_out_header);
        if(!(wave_out_header.num_samples % 4800))
            p_GUI->display_progress(wave_in_header_laser.num_samples - wave_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}
/*
//############################################################################
void LaserBoy_space::format_wave(LaserBoy_wave_header wave_in_header,
                                 fstream&             in,
                                 fstream&             out,
                                 bool                 global_polartity,
                                 bool                 write_eof_bits,
                                 int                  channel_5
                                )
{
    LaserBoy_wave_sample sample(wave_in_header.num_channels);
    LaserBoy_wave_header wave_out_header(  wave_in_header.sample_rate,
                                           ((global_polartity)?(LASERBOY_WAVE_NEGATIVE):(LASERBOY_WAVE_POSITIVE))
                                         | LASERBOY_WAVE_OFFSETS
                                         | LASERBOY_WAVE_SIGNAL_MATRIX,
                                         wave_in_header.num_channels,
                                         LASERBOY_WAVE_VERSION
                                        );
    //------------------------------------------------------------------------
    wave_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wave_out_header.offset    [0] = wave_offsets[0];
    wave_out_header.offset    [1] = wave_offsets[1];
    wave_out_header.offset    [2] = wave_offsets[2];
    wave_out_header.offset    [3] = wave_offsets[3];
    wave_out_header.offset    [4] = wave_offsets[4];
    wave_out_header.offset    [5] = wave_offsets[5];
    wave_out_header.offset    [6] = wave_offsets[6];
    wave_out_header.offset    [7] = wave_offsets[7];

    wave_out_header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION  ;
    wave_out_header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION  ;
    wave_out_header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG  ;
    wave_out_header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    wave_out_header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG ;
    wave_out_header.signal_id [5] = channel_5                   ;
    wave_out_header.signal_id [6] = LASERBOY_SIGNAL_AUDIO_LEFT  ;
    wave_out_header.signal_id [7] = LASERBOY_SIGNAL_AUDIO_RIGHT ;

    wave_out_header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [3] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    wave_out_header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;

    if(write_eof_bits)
    {
        wave_out_header.LSB_tag   [2] = LASERBOY_LSB_NOT_USED;

        wave_out_header.resolution[0] = 16;
        wave_out_header.resolution[1] = 16;
        wave_out_header.resolution[2] =  9;
        wave_out_header.resolution[3] =  9;
        wave_out_header.resolution[4] =  9;
        wave_out_header.resolution[5] = (channel_5 >= 0x30 && channel_5 < 0x40) ? (9) : (16);
        wave_out_header.resolution[6] = 16;
        wave_out_header.resolution[7] = 16;
    }
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    while(sample.from_fstream_wave(in))
        sample.to_fstream_wave(out, wave_out_header);
    //------------------------------------------------------------------------
    wave_out_header.to_fstream_wave(out);
    return;
}
*/
//############################################################################
string LaserBoy_space::LaserBoy_wave_signal_id_to_name(const short& signal_id) const
{
    switch(abs(signal_id)) // sign indicates polarity of channel data
    {
        default:
        case LASERBOY_NO_SIGNAL:
             return "NO SIGNAL   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_UNDEFINED:
             return "UNDEF SIGNAL";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_X_POSITION:
             return "X POSITION  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_Y_POSITION:
             return "Y POSITION  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_Z_POSITION:
             return "Z POSITION  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_DX_POSITION:
             return "DX POSITION ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_DY_POSITION:
             return "DY POSITION ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_X2_POSITION:
             return "DX POSITION ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_Y2_POSITION:
             return "DY POSITION ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_BEAM_WIDTH:
             return "BEAM WIDTH  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_RED_TTL:
             return "RED   TTL   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_RED_ANALOG:
             return "RED   ANALOG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_YELLOW_TTL:
             return "YELLOW TTL  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_YELLOW_ANALOG:
             return "YELLOW ANALG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_GREEN_TTL:
             return "GREEN TTL   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_GREEN_ANALOG:
             return "GREEN ANALOG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_BLUE_TTL:
             return "BLUE  TTL   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_BLUE_ANALOG:
             return "BLUE  ANALOG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_VIOLET_TTL:
             return "VIOLET TTL  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_VIOLET_ANALOG:
             return "VIOLET ANALG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_MONO_TTL:
             return "MONO  TTL   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
             return "MONO ANL AVG";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_MONO_OR_ANALOG:
             return "MONO ANLG OR";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
             return "MONO ANL WTD";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_MONO_O_SCOPE:
             return "MONO O-SCOPE";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_SMTPE:
             return "AUDIO SMTPE ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_MONO:
             return "AUDIO MONO  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_LEFT:
             return "AUDIO LEFT  ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_RIGHT:
             return "AUDIO RIGHT ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_CENTER:
             return "AUDIO CENTER";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_SUB:
             return "AUDIO SUB   ";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_S_LEFT:
             return "AUDIO S LEFT";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_S_RIGHT:
             return "AUDIO S RGHT";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_R_LEFT:
             return "AUDIO R LEFT";
        //--------------------------------------------------------------------
        case LASERBOY_SIGNAL_AUDIO_R_RIGHT:
             return "AUDIO R RGHT";
        //--------------------------------------------------------------------
    }
    return "";
}

//############################################################################
string LaserBoy_space::LaserBoy_wave_LSB_tag_to_name(const u_short& LSB_tag) const
{
    switch(LSB_tag)
    {
        default:
        case LASERBOY_LSB_NOT_USED:
             return "            ";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_LSB_BLANKING:
             return "BLANKING    ";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_LSB_END_OF_FRAME:
             return "END OF FRAME";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_LSB_UNIQUE_FRAME:
             return "UNIQUE FRAME";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_LSB_UNIQUE_VERTEX:
             return "UNIQUE VERTX";
             break;
        //--------------------------------------------------------------------
    }
    return "";
}

//############################################################################
string LaserBoy_space::LaserBoy_dots_setting_id_to_name(const int& id) const
{
    switch(id)
    {
        case LASERBOY_DOTS_IGNORE:
             return "ignore dots";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_DOTS_REMOVE:
             return "remove dots";
             break;
        //--------------------------------------------------------------------
        case LASERBOY_DOTS_ENHANCE:
             return "enhance dots";
             break;
        //--------------------------------------------------------------------
    }
    return "";
}

/*
//############################################################################
void LaserBoy_space::tell(LaserBoy_wave_header header, string label) const
{
    cout << "----------------------------------------------------------" << ENDL;
    cout << label << ENDL;
    cout << "             num_samples : " << header.num_samples     << ENDL;
    cout << "             sample_rate : " << header.sample_rate     << ENDL;
    cout << "      LaserBoy_wave_mode : ";
    //------------------------------------------------------------------------
    if(header.version != "!LaserBoy!")
    {
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE        )
            cout << "LASERBOY_WAVE_POSITIVE" << ENDL;
        else
            cout << "LASERBOY_WAVE_NEGATIVE" << ENDL;

        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_END_OF_FRAME    )
            cout << "                           LASERBOY_WAVE_END_OF_FRAME"     << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_UNIQUE_FRAME    )
            cout << "                           LASERBOY_WAVE_UNIQUE_FRAME"     << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_UNIQUE_VERTEX   )
            cout << "                           LASERBOY_WAVE_UNIQUE_VERTEX"    << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS         )
            cout << "                           LASERBOY_WAVE_OFFSETS"          << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED       )
            cout << "                           LASERBOY_WAVE_OPTIMIZED"        << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX   )
            cout << "                           LASERBOY_WAVE_SIGNAL_MATRIX"    << ENDL;
        if(header.LaserBoy_wave_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
            cout << "                           LASERBOY_SIGNAL_BIT_RESOLUTION" << ENDL;
        //--------------------------------------------------------------------
        cout << "            num_channels : " << header.num_channels    << ENDL;
        cout << "         bits_per_sample : " << header.bits_per_sample << ENDL;
        cout << "                 version : " << header.version         << ENDL << ENDL;
        //--------------------------------------------------------------------
        cout << " ch. :offset: bits :     : signal name  : LSB tag"          << ENDL;
        cout << "-----:------:------:-----:--------------:-----------------" << ENDL;
        for(int i = 0; i < header.num_channels; i++)
        {
            cout << setw(4)
                 << i
                 << " : ";
            if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS)
                 cout << setw(4) << (int)header.offset[i];
            else
                 cout << " na ";
            cout << " : ";

            if(header.LaserBoy_wave_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
                 cout << setw(4) << (int)header.resolution[i];
            else
                 cout << "    ";

            if(header.LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
                cout << " : "
                     << ((header.signal_id[i] >= 0) ? ("pos") : ("neg"))
                     << " : "
                     << LaserBoy_wave_signal_id_to_name(header.signal_id[i])
                     << " : "
                     << LaserBoy_wave_LSB_tag_to_name(header.LSB_tag[i])
                     << ENDL;
            else
                cout << ENDL;
        }
        //--------------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            cout << ENDL;
            cout << "    lit_dwell_overhang : " << header.parms.lit_dwell_overhang     << ENDL;
            cout << "         lit_delta_max : " << header.parms.lit_delta_max          << ENDL;
            cout << "       blank_delta_max : " << header.parms.blank_delta_max        << ENDL;
            cout << "    max_dwell_microsec : " << header.parms.max_dwell_microsec     << ENDL;
            cout << "      max_wag_microsec : " << header.parms.max_wag_microsec       << ENDL;
            cout << "insignificant_distance : " << header.parms.insignificant_distance << ENDL;
            cout << "   insignificant_angle : " << header.parms.insignificant_angle    << ENDL;
            cout << "     frames_per_second : " << header.parms.frames_per_second      << ENDL;
        }
    }
    else
    {
        cout << "not a LaserBoy wave" << ENDL;
    }
    cout << "----------------------------------------------------------" << ENDL;
    return;
}
*/

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
