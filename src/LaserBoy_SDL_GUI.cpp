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
// LaserBoy_SDL_GUI.cpp is part of LaserBoy.
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
#include "LaserBoy_SDL_GUI.hpp"

//############################################################################
void bmp_clone_SDL_Surface(struct LaserBoy_bmp* bmp, struct SDL_Surface* screen, int copy)
{
    bmp_init(bmp, screen->w, screen->h, screen->format->BitsPerPixel);
    if(screen->format->BitsPerPixel <= 8)
        memcpy((void*)(bmp->palette), (const void*)(screen->format->palette->colors), bmp->palette_size);
    if(copy)
        memcpy((void*)(bmp->image), (const void*)(screen->pixels), bmp->image_size);
    return;
}

//############################################################################
void bmp_to_SDL_Surface(struct LaserBoy_bmp* bmp, struct SDL_Surface* screen)
{
    if(screen->format->BitsPerPixel <= 8)
        memcpy((void*)(screen->format->palette->colors),
               (const void*)(bmp->palette),
               bmp->palette_size
              );
    memcpy((void*)(screen->pixels),
           (const void*)(bmp->image),
           bmp->image_size
          );
    SDL_UnlockSurface(screen);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    SDL_LockSurface(screen);
    return;
}

//############################################################################
void SDL_Surface_to_bmp(struct SDL_Surface* screen, struct LaserBoy_bmp* bmp)
{
    if(screen->format->BitsPerPixel <= 8)
        memcpy((void*)(bmp->palette),
               (const void*)(screen->format->palette->colors),
               bmp->palette_size
              );
    memcpy((void*)(bmp->image),
           (const void*)(screen->pixels),
           bmp->image_size
          );
    return;
}

//############################################################################
static void SDL_bailout(int rc)
{
    SDL_Quit();
    exit(rc);
}

//############################################################################
LaserBoy_SDL_GUI::LaserBoy_SDL_GUI(int x, int y)
                : LaserBoy_GUI_base(x, y)
                , screen(NULL)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        SDL_bailout(2);
    if((screen = SDL_SetVideoMode(xres,
                                  yres,
                                  0,
                                    SDL_HWSURFACE
                                  | SDL_ANYFORMAT
                                 )
       ) == NULL
      )
        SDL_bailout(2);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(250, 1);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_PRESSED    , SDL_IGNORE);
    SDL_EventState(SDL_RELEASED   , SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT , SDL_IGNORE);
    SDL_WM_SetCaption("LaserBoy Multiplatform (SDL) by James Lehman : Extra Stimulus Inc.", NULL);
    if(SDL_LockSurface(screen) < 0)
        SDL_bailout(2);
    bmp_clone_SDL_Surface(space.p_bmp(), screen, 0);
    bmp_copy(space.p_background()    , space.p_bmp());
    bmp_copy(space.p_background_bmp(), space.p_bmp());
    space.recolor_background();
    if(y < 960)
        space.font_size_factor = 1;
}

//############################################################################
LaserBoy_SDL_GUI::~LaserBoy_SDL_GUI()
{
    SDL_FreeSurface(screen);
    SDL_Quit       ();
}

//############################################################################
void LaserBoy_SDL_GUI::capture_screen()
{
    u_int x, y;
    struct LaserBoy_bmp screen_capture = {0};
    bmp_init(&screen_capture, space.p_bmp()->xres, space.p_bmp()->yres, 24);
    for(y = 0; y < space.p_bmp()->yres; y++)
        for(x = 0; x < space.p_bmp()->xres; x++)
            screen_capture.put_pixel(   &screen_capture
                                      , x
                                      , screen_capture.yres - y - 1 // flipped
                                      , screen_capture.make_rgb(   space.p_bmp()->r_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                                 , space.p_bmp()->g_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                                 , space.p_bmp()->b_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                               )
                                    );
    bmp_save_as(&screen_capture, (char*)("screen_capture.bmp"));
    bmp_free(&screen_capture);
}


//############################################################################
string LaserBoy_SDL_GUI::display_prompt_file_with_auto_complete(const string& prompt, const string& part)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = part.size(),
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input,
           input        = part;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.white, bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.make_rgb(0,255,255), bmp_bg_color, 0, space.font_size_factor,
               (input.size()) ? (input.c_str()) : ("_") // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        if(character > 0)
                            character--;
                        space.clean_screen();
                        space.overlay_file_list(input);
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s",
                                   input.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    {
                        pre_input = input;
                        pre_input += event.key.keysym.unicode;
                        if(STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), pre_input))
                        {
                            input = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_file_list(input) == 1)
                            {
                                for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                                {
                                    if(STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), input))
                                    {
                                        input = space.working_directory_list[list_item].path().filename().string();
                                        character = input.size() - 1;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   space.bmp.make_rgb(255,255,0),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   input.c_str()
                                                  );
                                        break;
                                    }
                                }
                            } // if(space.overlay_file_list(input) == 1)
                            else
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           space.bmp.make_rgb(0,255,255),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s",
                                           input.c_str()
                                          );
                            display_space();
                            break;
                        } // if(event.key.keysym.unicode == space.working_directory_list[list_item].path().filename().string()[character])
                    } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_SDL_GUI::display_prompt_dir_with_auto_complete(const string& prompt)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = 0,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input,
           input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.white, bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.make_rgb(0,255,255), bmp_bg_color, 0, space.font_size_factor,
               "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        if(character > 0)
                            character--;
                        space.clean_screen();
                        space.overlay_directory_list(input);
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s",
                                   input.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    {
                        pre_input = input;
                        pre_input += event.key.keysym.unicode;
                        if(    is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string())
                            && STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), pre_input)
                          )
                        {
                            input = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_directory_list(input) == 1)
                            {
                                for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                                {
                                    if(    is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string())
                                        && STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), input))
                                    {
                                        input = space.working_directory_list[list_item].path().filename().string();
                                        character = input.size() - 1;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   space.bmp.make_rgb(255,255,0),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   input.c_str()
                                                  );
                                        break;
                                    }
                                } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                            } // end if(space.overlay_directory_list(input) == 1)
                            else
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           space.bmp.make_rgb(0,255,255),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s",
                                           input.c_str()
                                          );
                            display_space();
                            break;
                        } // END if(is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string()) ....
                    } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_SDL_GUI::display_prompt_f_effect_with_auto_complete(const string& prompt)
{
    bool   done           = false;
    u_int  x_off          = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off          = space.bmp.yres - 24 * space.font_size_factor,
           x_gap          = 8 * space.font_size_factor,
           y_line         = x_gap,
           list_item      = 0,
           character      = 0,
           bmp_bg_color   = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input,
           input;
    //------------------------------------------------------------------------
    prompt_escaped   = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.white, bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.make_rgb(0,255,255), bmp_bg_color, 0, space.font_size_factor,
               "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        if(character > 0)
                            character--;
                        space.clean_screen();
                        space.overlay_frame_effects(input);
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s",
                                   input.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    {
                        pre_input = input;
                        pre_input += event.key.keysym.unicode;
                        if(starts_with(space.frame_effect_names[list_item], pre_input))
                        {
                            input = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_frame_effects(input) == 1)
                            {
                                for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                                {
                                    if(starts_with(space.frame_effect_names[list_item], input))
                                    {
                                        input = space.frame_effect_names[list_item];
                                        character = input.size() - 1;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   space.bmp.make_rgb(255,255,0),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   input.c_str()
                                                  );
                                    }
                                }
                            } // end if(space.overlay_frame_effects(input) == 1)
                            else
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           space.bmp.make_rgb(0,255,255),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s",
                                           input.c_str()
                                          );
                            display_space();
                            break;
                        } // end if(event.key.keysym.unicode == space.frame_effect_names[list_item][character])
                    } // end for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_SDL_GUI::display_prompt_fs_effect_with_auto_complete(const string& prompt)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = 0,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input,
           input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.white, bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.make_rgb(0,255,255), bmp_bg_color, 0, space.font_size_factor,
               "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        if(character > 0)
                            character--;
                        space.clean_screen();
                        space.overlay_frame_set_effects(input);
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s",
                                   input.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.frame_set_effect_names.size(); list_item++)
                    {
                        pre_input = input;
                        pre_input += event.key.keysym.unicode;
                        if(starts_with(space.frame_set_effect_names[list_item], pre_input))
                        {
                            input = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_frame_set_effects(input) == 1)
                            {
                                for(list_item = 0; list_item < space.frame_set_effect_names.size(); list_item++)
                                {
                                    if(starts_with(space.frame_set_effect_names[list_item], input))
                                    {
                                        input = space.frame_set_effect_names[list_item];
                                        character = input.size() - 1;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   space.bmp.make_rgb(255,255,0),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   input.c_str()
                                                  );
                                    }
                                }
                            } // end if(space.overlay_frame_effects(input) == 1)
                            else
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           space.bmp.make_rgb(0,255,255),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s",
                                           input.c_str()
                                          );
                            display_space();
                            break;
                        } // end if(event.key.keysym.unicode == space.frame_effect_names[list_item][character])
                    } // end for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_SDL_GUI::display_prompt_and_echo_name(const string& prompt)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.white, bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.bmp.make_rgb(0,255,255), bmp_bg_color, 0, space.font_size_factor,
               "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isalnum(event.key.keysym.unicode)
                        || event.key.keysym.unicode == '_'
                        || event.key.keysym.unicode == '-'
                        || event.key.keysym.unicode == '.'
                      )
                    {
                        input += event.key.keysym.unicode;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        x_off += x_gap;
                        display_space();
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_SDL_GUI::display_prompt_and_echo_string(const string& prompt)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;

    prompt_escaped = false;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.white,
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                  space.current_palette()[space.selected_color_index].g,
                                  space.current_palette()[space.selected_color_index].b
                                 ),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isgraph(event.key.keysym.unicode)
                        || event.key.keysym.unicode == ' '
                      )
                    {
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        display_space();
                        input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
double LaserBoy_SDL_GUI::display_prompt_and_echo_double(const string& prompt, double value)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    double result;
    string input;

    prompt_escaped = false;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.white,
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(0,255,255),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return 0.0;
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isdigit(event.key.keysym.unicode)
                        || (    event.key.keysym.unicode == '.'
                             && input.find('.') == string::npos
                           )
                        || (    event.key.keysym.unicode == '+'
                             && !input.size()
                           )
                        || (    event.key.keysym.unicode == '-'
                             && !input.size()
                           )
                      )
                    {
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        display_space();
                        input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%lf", &result);
    else
        result = value;
    return result;
}

//############################################################################
int LaserBoy_SDL_GUI::display_prompt_and_echo_int(const string& prompt, int default_value)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           result;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;

    prompt_escaped = false;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.white,
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(0,255,255),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return 0;
        }
        if(event.key.keysym.unicode == SDLK_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_DELETE:
                case SDLK_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isdigit(event.key.keysym.unicode)
                        || (    event.key.keysym.unicode == '+'
                             && !input.size()
                           )
                        || (    event.key.keysym.unicode == '-'
                             && !input.size()
                           )
                      )
                    {
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(0,255,255),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        display_space();
                        input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == SDLK_RETURN)
    } // end while(!done)
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               space.bmp.make_rgb(255,0,0),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%d", &result);
    else
        result = default_value;
    return result;
}

//############################################################################
int LaserBoy_SDL_GUI::display_prompt_and_echo_nibble(const string& prompt)
{
    bool   done         = false;
    u_int  result       = 0x00, // dud return value
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);

    prompt_escaped = false;

    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.white,
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.bmp.make_rgb(0,255,255),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        if(event.key.keysym.unicode == SDLK_ESCAPE)
        {
            prompt_escaped = true;
            return -1;
        }
        switch(event.key.keysym.unicode)
        {
            case '0':    return 0x00;
            case '1':    return 0x01;
            case '2':    return 0x02;
            case '3':    return 0x03;
            case '4':    return 0x04;
            case '5':    return 0x05;
            case '6':    return 0x06;
            case '7':    return 0x07;
            case '8':    return 0x08;
            case '9':    return 0x09;
            case 'a':    return 0x0a;
            case 'b':    return 0x0b;
            case 'c':    return 0x0c;
            case 'd':    return 0x0d;
            case 'e':    return 0x0e;
            case 'f':    return 0x0f;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    } // end while(!done)
    return result;
}

//############################################################################
bool LaserBoy_SDL_GUI::display_prompt_and_echo_bool(const string& prompt)
{
    int    x_off  = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off  = space.bmp.yres - 24 * space.font_size_factor;

    prompt_escaped = false;
    bmp_printf(   &(space.bmp)
                , x_off
                , y_off
                , space.bmp.white
                , space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b)
                , 0
                , space.font_size_factor
                , prompt.c_str()
              );
    display_space();
    do
    {
        SDL_WaitEvent(&event);
    } while(event.type != SDL_KEYDOWN);
    if(event.key.keysym.unicode == SDLK_ESCAPE)
    {
        prompt_escaped = true;
        return false;
    }
    if(    event.key.keysym.unicode == 'y'
        || event.key.keysym.unicode == 'Y' // this can only happen if the caps lock key is on!
        || event.key.keysym.unicode == '1'
      )
        return true;
    return false;
}

//############################################################################
bool LaserBoy_SDL_GUI::report_ild_file_open(LaserBoy_frame_set& frame_set, const string& file_name)
{
    long int       bytes_skipped = 0;
    ostringstream  output_message;
    
    frame_set.from_ild_file(LASERBOY_ILD_SHARE + file_name, bytes_skipped);
    if(frame_set.frame_set_error & LASERBOY_FILE_OPEN_FAILED)
    {
        display_error(LASERBOY_ILD_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    else if(frame_set.frame_set_error & LASERBOY_NO_FRAMES_FOUND)
    {
        if(bytes_skipped == -2)
        {
            space.clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << file_name
                           << " is empty, zero bytes";
            display_error(output_message.str());
        }
        else if(bytes_skipped > 0)
        {
            space.clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << file_name
                           << " : "
                           << bytes_skipped
                           << " bytes no frames found";
            display_error(output_message.str());
        }
        else
            display_error(LASERBOY_ILD_SHARE + file_name + " no frames found");
        wait_4_Esc();
        return false;
    }
    else if(frame_set.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
    {
        if(bytes_skipped > 0)
        {
            space.clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << file_name
                           << " : "
                           << bytes_skipped
                           << " bytes unrecognised data";
            display_error(output_message.str());
        }
        display_error(LASERBOY_ILD_SHARE + file_name + " table to frame quantity mismatch");
        wait_4_Esc();
    }
    else if(bytes_skipped > 0)
    {
        space.clean_screen();
        output_message << LASERBOY_ILD_SHARE
                       << file_name
                       << " : "
                       << bytes_skipped
                       << " bytes of unrecognised data";
        display_error(output_message.str());
        wait_4_Esc();
    }
    return true;
}

//############################################################################
void LaserBoy_SDL_GUI::display_error(const string& error)
{
    space.TUI_clue = "oops!";
    //------------------------------------------------------------------------
    bmp_printf(   &(space.bmp) // top of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor + 8 * space.font_size_factor
                , space.bmp.white
                , space.bmp.make_rgb(192, 0, 0)
                , 0
                , space.font_size_factor
                , "%s                                                                                                                                                   "
                , error.c_str()
              );
    //------------------------------------------------------------------------
    bmp_printf(   &(space.bmp) // bottom of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor
                , space.bmp.yres - 24 * space.font_size_factor
                , space.bmp.white
                , space.bmp.make_rgb(192, 0, 0)
                , 0
                , space.font_size_factor
                , "[Esc] key to continue                                                                                                                                                          "
              );
    //------------------------------------------------------------------------
    display_space();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::display_message(const string& message)
{
    space.TUI_clue = "Notice!";
    //------------------------------------------------------------------------
    bmp_printf(   &(space.bmp) // top of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor + 16 * space.font_size_factor
                , space.bmp.make_rgb(255, 255, 0)
                , space.bmp.make_rgb(0, 0, 192)
                , 0
                , space.font_size_factor
                , "%s                                                                                                                                                   "
                , message.c_str()
              );
    //------------------------------------------------------------------------
    bmp_printf(   &(space.bmp) // bottom of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor
                , space.bmp.yres - 24 * space.font_size_factor
                , space.bmp.make_rgb(255, 255, 0)
                , space.bmp.make_rgb(0, 0, 192)
                , 0
                , space.font_size_factor
                , "any key to continue                                                                                                                                                          "
              );
    //------------------------------------------------------------------------
    display_space();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::wait_4_Esc()
{
    while(true)
    {
        SDL_WaitEvent(&event);
        if(    event.type == SDL_KEYDOWN
            && event.key.keysym.unicode == SDLK_ESCAPE
          )
            break;
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::wait_4_any_key()
{
    while(true)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
            break;
    }
    return;
}

//############################################################################
// wave functions that require direct access to space.bmp
//############################################################################
bool LaserBoy_SDL_GUI::display_wave()
{
    fstream  in;
    do // clear key strokes from event queue!
    {
        SDL_PollEvent(&event);
    } while(event.type == SDL_KEYDOWN);

    in.open(file_name.c_str(), ios::in | ios::binary);
    if(!in.is_open())
        return false;

    LaserBoy_wave_header header(in);
    if(    header.version == "!LaserBoy!"
        || !(header.LaserBoy_wave_mode & LASERBOY_WAVE_END_OF_FRAME)
      )
        display_unframed_wave(in, header);
    else
        display_LaserBoy_wave(in, header);
    in.close();
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::display_unframed_wave()
{
    fstream in;
    do // clear key strokes from event queue!
    {
        SDL_PollEvent(&event);
    } while(event.type == SDL_KEYDOWN);

    in.open(file_name.c_str(), ios::in | ios::binary);
    if(!in.is_open())
        return false;

    LaserBoy_wave_header header(in);
    display_unframed_wave(in, header);
    in.close();
    return true;
}

//############################################################################
void LaserBoy_SDL_GUI::display_LaserBoy_wave(fstream& in, LaserBoy_wave_header header)
{
    bool                     playing      = true         ;

    int                      i                           ,
                             span                        ,
                             last                        ,
                             red                         ,
                             green                       ,
                             blue                        ,
                             sample_count = 0            ,
                             offset_max                  ,
                             offset_min                  ,
                             center       = screen->h / 2;

    u_int                    time_zero                   ,
                             color        = space.bmp.white,
                             bmp_bg_color = space.bmp.make_rgb(space.bg_color.r,
                                                               space.bg_color.g,
                                                               space.bg_color.b
                                                              );
    double                  scale  = screen->h / 65536.0;
    struct  LaserBoy_bmp    frame_buffer = {0}          ;

    LaserBoy_wave_sample_pointer   roll_over    ;
    LaserBoy_wave_sample_pointer*  sample_window;
    //------------------------------------------------------------------------
    if(header.num_channels < 6)
    {
        bmp_printf(&(space.bmp),
                   LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
                   LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
                   space.bmp.white,
                   bmp_bg_color,
                   0,
                   space.font_size_factor,
                   "this is not a LaserBoy wave        "
                  );
        display_space();
        prompt_escaped = true;
        return;
    }
    //------------------------------------------------------------------------
    if(!header.LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS) // if no offsets in header
        for(i = 0; i < 6; i++)
            header.offset[i] = 0; // give me some!
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
    sample_window = new LaserBoy_wave_sample_pointer[span]; // at least 2
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    bmp_clone_SDL_Surface(&frame_buffer, screen, 0);
    bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
    //------------------------------------------------------------------------
    while(playing)
    {
        time_zero = SDL_GetTicks();
        //--------------------------------------------------------------------
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        //--------------------------------------------------------------------
        if(space.wave_show_inverted)
            playing = sample_window[last]->from_fstream_wave_inverted(in);
        else
            playing = sample_window[last]->from_fstream_wave(in);
        //--------------------------------------------------------------------
        sample_count++;
        //--------------------------------------------------------------------
        color = frame_buffer.make_rgb((   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                                        ? (rescale_to_index(header.color_rescale_r, sample_window[red]->channel[2]))
                                        : (sample_window[red]->channel[2] >> 7)
                                      ),
                                      (   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                                        ? (rescale_to_index(header.color_rescale_g, sample_window[green]->channel[3]))
                                        : (sample_window[green]->channel[3] >> 7)
                                      ),
                                      (   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                                        ? (rescale_to_index(header.color_rescale_b, sample_window[blue]->channel[4]))
                                        : (sample_window[blue]->channel[4] >> 7)
                                      )
                                     );
        //--------------------------------------------------------------------
        if(sample_window[red]->channel[2] & 0x0001)
            color = 0;
        //--------------------------------------------------------------------
        if(color)
        {
            bmp_put_line_fast(   &frame_buffer
                               , (u_int)((sample_window[0]->channel[0] *  scale) + center)
                               , (u_int)((sample_window[0]->channel[1] * -scale) + center)
                               , (u_int)((sample_window[1]->channel[0] *  scale) + center)
                               , (u_int)((sample_window[1]->channel[1] * -scale) + center)
                               , color
                             );
            if(space.wave_show_vertices)
                bmp_put_vertex(   &frame_buffer
                                , (u_int)((sample_window[1]->channel[0] *  scale) + center)
                                , (u_int)((sample_window[1]->channel[1] * -scale) + center)
                                , color
                              );
        }
        //--------------------------------------------------------------------
        else if(space.wave_show_blanks)
        {
            bmp_put_line_fast(   &frame_buffer
                               , (u_int)((sample_window[0]->channel[0] *  scale) + center)
                               , (u_int)((sample_window[0]->channel[1] * -scale) + center)
                               , (u_int)((sample_window[1]->channel[0] *  scale) + center)
                               , (u_int)((sample_window[1]->channel[1] * -scale) + center)
                               , frame_buffer.gray
                             );
            if(space.wave_show_vertices)
                bmp_put_vertex(   &frame_buffer
                                , (u_int)((sample_window[1]->channel[0] *  scale) + center)
                                , (u_int)((sample_window[1]->channel[1] * -scale) + center)
                                , frame_buffer.gray
                              );
        }
        //----------------------------------------------------------------
        if(sample_window[red]->channel[2] & 0x0001)
        {
            if(space.wave_real_time)
            {
                do
                {
                     SDL_Delay(1); // based on 64hz, or 15.625 ms / tick
                }
                while(   SDL_GetTicks() - time_zero
                       < (u_int)(sample_count / (header.sample_rate / 1000.0))
                     );
            }
            bmp_to_SDL_Surface(&frame_buffer, screen);
            if(space.wave_clean_screen)
                bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
            SDL_PollEvent(&event);
            if(event.type == SDL_KEYDOWN)
                playing = false;
            sample_count = 0;
        }
        //--------------------------------------------------------------------
    } // end while(playing)
    bmp_free(&frame_buffer);
    for(i = 0; i < span; i++)
        if(sample_window[i])
            delete sample_window[i];
    if(sample_window)
        delete sample_window;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::display_unframed_wave(fstream& in, LaserBoy_wave_header header)
{
    bool                     playing      = true         ;

    int                      i                           ,
                             span                        ,
                             last                        ,
                             red                         ,
                             green                       ,
                             blue                        ,
                             sample_count = 0            ,
                             offset_max                  ,
                             offset_min                  ,
                             center       = screen->h / 2;

    u_int                    time_zero                     ,
                             color        = space.bmp.white;

    double                   scale  = screen->h / 65536.0;
    struct  LaserBoy_bmp     frame_buffer = {0}          ;

    LaserBoy_wave_sample_pointer   roll_over    ;
    LaserBoy_wave_sample_pointer*  sample_window;
    //------------------------------------------------------------------------
    offset_max = space.wave_offsets[0];
    for(i = 1; i < 6; i++)
        if(space.wave_offsets[i] > offset_max)
            offset_max = space.wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        space.wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = space.wave_offsets[i]; // use current system offsets
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
    sample_window = new LaserBoy_wave_sample_pointer[span]; // at least 2
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    bmp_clone_SDL_Surface(&frame_buffer, screen, 0);
    bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
    //------------------------------------------------------------------------
    while(playing)
    {
        time_zero = SDL_GetTicks();
        //--------------------------------------------------------------------
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        //--------------------------------------------------------------------
        if(space.wave_show_inverted)
            playing = sample_window[last]->from_fstream_wave_inverted(in);
        else
            playing = sample_window[last]->from_fstream_wave(in);
        //--------------------------------------------------------------------
        sample_count++;
        //--------------------------------------------------------------------
        if(header.num_channels >= 6)
            color = frame_buffer.make_rgb(   (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[green]->channel[3] > 0)
                                               ? (sample_window[green]->channel[3] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[blue ]->channel[4] > 0)
                                               ? (sample_window[blue ]->channel[4] / 128)
                                               : (0)
                                             )
                                         );
        //--------------------------------------------------------------------
        else if(header.num_channels >= 4)
            color = frame_buffer.make_rgb(   (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                         );
        //--------------------------------------------------------------------
        else
            color = frame_buffer.make_rgb(255, 255, 255);
        //--------------------------------------------------------------------
        bmp_put_line_fast(&frame_buffer,
                          (u_int)((sample_window[0]->channel[0] *  scale) + center),
                          (u_int)((sample_window[0]->channel[1] * -scale) + center),
                          (u_int)((sample_window[1]->channel[0] *  scale) + center),
                          (u_int)((sample_window[1]->channel[1] * -scale) + center),
                          color
                         );

        if(space.wave_show_vertices)
            bmp_put_vertex(&frame_buffer,
                           (u_int)((sample_window[1]->channel[0] *  scale) + center),
                           (u_int)((sample_window[1]->channel[1] * -scale) + center),
                           color
                          );
        //--------------------------------------------------------------------
        if(!(sample_count % space.unframed_wave_stride))
        {
            if(space.wave_real_time)
            {
                do
                {
                     SDL_Delay(1); // based on 64hz, or 15.625 ms / tick
                }
                while(   SDL_GetTicks() - time_zero
                       < (u_int)(sample_count / (header.sample_rate / 1000.0))
                     );
            }

            bmp_to_SDL_Surface(&frame_buffer, screen);

            if(space.wave_clean_screen)
                bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
            SDL_PollEvent(&event);
            if(event.type == SDL_KEYDOWN)
                playing = false;
            sample_count = 0;
        }
        //--------------------------------------------------------------------
    } // end while(playing)
    bmp_free(&frame_buffer);
    for(i = 0; i < span; i++)
        if(sample_window[i])
            delete sample_window[i];
    if(sample_window)
        delete sample_window;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::check_for_common_nav_keys(SDLKey key)
{
    switch(key)
    {
        default:
            //space.TUI_clue = key;
            space.TUI_clue = "bad key";
            break;
        //--------------------------------------------------------------------
        case SDLK_RSHIFT:
        case SDLK_LSHIFT:
            space.TUI_clue = "";
            break;
        //--------------------------------------------------------------------
        case SDLK_UP: // show next palette
            if(space.show_target_palette)
            {
                space.target_palette_index++;
                if(space.target_palette_index >= space.number_of_palettes())
                    space.target_palette_index = 0;
                space.TUI_clue = "+ target palette";
            }
            break;
        //--------------------------------------------------------------------
        case SDLK_DOWN: // show previous palette
            if(space.show_target_palette)
            {
                if(space.target_palette_index == 0)
                    space.target_palette_index = space.number_of_palettes();
                space.target_palette_index--;
                space.TUI_clue = "- target palette";
            }
            break;
        //--------------------------------------------------------------------
        case SDLK_RIGHT: // show previous frame
            if(    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
                space.pre_incr_current_frame();
            space.TUI_clue = "+ frame";
            break;
        //--------------------------------------------------------------------
        case SDLK_LEFT: // show next frame
            if(    space.loop_animation
                || (space.frame_index > 0)
              )
                space.pre_decr_current_frame();
            space.TUI_clue = "- frame";
            break;
        //--------------------------------------------------------------------
        case SDLK_TAB:
            space.set_current_menu_name("value settings", 0x00400000, true);
            system_settings_menu();
            break;
        //--------------------------------------------------------------------
        case SDLK_F3:
            capture_screen();
            space.TUI_clue = "Screen Captured";
            break;
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::animate_forward()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "+ show";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.pre_incr_current_frame();
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::animate_reverse()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "- show";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.pre_decr_current_frame();
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::rotate_forward()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "+ orbit";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.view_angle += one_degree;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::rotate_reverse()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "- orbit";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.view_angle -= one_degree;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::animate_rotate_forward()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "+ show & orbit";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.pre_incr_current_frame();
        space.view_angle += one_degree;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::animate_rotate_reverse()
{
    u_int  time_zero;
    SDL_Delay(250);
    SDL_PollEvent(&event);
    space.TUI_clue = "- show & orbit";
    while(    (event.type != SDL_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = SDL_GetTicks();
        space.pre_decr_current_frame();
        space.view_angle -= one_degree;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   SDL_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        SDL_PollEvent(&event);
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::system_settings_menu()
{
    bool   in_menu = true;
    int    temp_int;
    double temp_double;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.overlay_value_settings_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case SDLK_RSHIFT:
                case SDLK_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case SDLK_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    temp_double = (display_prompt_and_echo_double("insignificant angle in degrees") * one_degree);
                    if(prompt_escaped)
                        break;
                    space.insignificant_angle = temp_double;
                    break;
                //------------------------------------------------------------
                case '2':
                    temp_double = display_prompt_and_echo_double("insignificant distance in points");
                    if(prompt_escaped)
                        break;
                    space.insignificant_distance = temp_double;
                    break;
                //------------------------------------------------------------
                case '3':
                    temp_double = display_prompt_and_echo_double("maximum lit vector length in points");
                    if(prompt_escaped)
                        break;
                    space.lit_delta_max = temp_double;
                    break;
                //------------------------------------------------------------
                case '4':
                    temp_double = display_prompt_and_echo_double("maximum blank vector length in points");
                    if(prompt_escaped)
                        break;
                    space.blank_delta_max = temp_double;
                    break;
                //------------------------------------------------------------
                case '5':
                    temp_double = display_prompt_and_echo_double("dwell in microseconds for 180 degree angle");
                    if(prompt_escaped)
                        break;
                    space.max_dwell_microsec = temp_double;
                    break;
                //------------------------------------------------------------
                case '6':
                    temp_int = display_prompt_and_echo_int("lit dwell overhang");
                    if(prompt_escaped)
                        break;
                    space.lit_dwell_overhang = temp_int;
                    break;
                //------------------------------------------------------------
                case '7':
                    dots_setting_menu();
                    break;
                //------------------------------------------------------------
                case '8':
                    temp_int = display_prompt_and_echo_int("dwell on dot");
                    if(prompt_escaped)
                        break;
                    space.dwell_on_dot = temp_int;
                    break;
                //------------------------------------------------------------
                case '9':
                    temp_double = display_prompt_and_echo_double("max time for wagging in seconds");
                    if(prompt_escaped)
                        break;
                    space.max_wag_microsec = temp_double * 1000000.0;
                    break;
                //------------------------------------------------------------
                case '0':
                    temp_int = display_prompt_and_echo_int("move points per key");
                    if(prompt_escaped)
                        break;
                    space.displacement_step = temp_int;
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    temp_double = display_prompt_and_echo_double("rotate degrees per key hit");
                    if(prompt_escaped)
                        break;
                    space.rotation_step = temp_double * one_degree;
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    temp_double = display_prompt_and_echo_double("scale %% per key hit");
                    if(prompt_escaped)
                        break;
                    space.magnitude_step = temp_double / 100.00;
                    space.magnitude_step_up = 1.00 + space.magnitude_step;
                    space.magnitude_step_dn = 1.00 - space.magnitude_step;
                    break;
                //------------------------------------------------------------
                case 'c':
                case 'C':
                    temp_double = display_prompt_and_echo_double("rendered arc segment angle in degrees");
                    if(prompt_escaped)
                        break;
                    space.rendered_arc_angle = temp_double;
                    break;
                //------------------------------------------------------------
                case 'd':
                case 'D':
                    temp_double = display_prompt_and_echo_double("%% rendered mono font field space");
                    if(prompt_escaped)
                        break;
                    space.rendered_mono_font_space = temp_double / 100.00;
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    temp_double = display_prompt_and_echo_double("%% rendered variable font gaps");
                    if(prompt_escaped)
                        break;
                    space.rendered_vari_font_gap = temp_double / 100.00;
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    temp_double = display_prompt_and_echo_double("frames per second");
                    if(prompt_escaped)
                        break;
                    space.frames_per_second = temp_double;
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    temp_double = display_prompt_and_echo_double("wave frame still output duration in seconds");
                    if(prompt_escaped)
                        break;
                    space.still_frame_duration = temp_double;
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    temp_int = display_prompt_and_echo_int("output wave sample rate");
                    if(prompt_escaped)
                        break;
                    space.sample_rate = temp_int;
                    break;
                //------------------------------------------------------------
                case 'i':
                case 'I':
                    temp_int = display_prompt_and_echo_int("unframed wave samples per frame");
                    if(prompt_escaped)
                        break;
                    space.unframed_wave_stride = temp_int;
                    break;
                //------------------------------------------------------------
                case 'j':
                case 'J':
                    signal_if_not_z_menu();
                    break;
                //------------------------------------------------------------
                case 'k':
                case 'K':
                    temp_int = display_prompt_and_echo_int("output bitmap size in pixels square");
                    if(prompt_escaped)
                        break;
                    space.output_bmp_size = temp_int;
                    break;
                //------------------------------------------------------------
                case 'l':
                case 'L':
                    temp_int = display_prompt_and_echo_int("menu font size factor");
                    if(prompt_escaped)
                        break;
                    space.font_size_factor = temp_int;
                    break;
                //------------------------------------------------------------
                case 'm':
                case 'M':
                    temp_int = display_prompt_and_echo_int("background bitmap size factor");
                    if(prompt_escaped)
                        break;
                    space.background_bmp_scale = temp_int;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case 'n':
                case 'N':
                    edit_background_color_menu();
                    break;
                //------------------------------------------------------------
                case 'o':
                case 'O':
                    temp_int = display_prompt_and_echo_int("screen background gray scale");
                    if(prompt_escaped)
                        break;
                    space.bg_color = (u_char)temp_int;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case 'p':
                case 'P':
                    temp_double = display_prompt_and_echo_double("onion skin fade %");
                    if(prompt_escaped)
                        break;
                    space.onion_skin_fade = temp_double / 100.0;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case 'q':
                case 'Q':
                    temp_int = display_prompt_and_echo_int("onion skin depth");
                    if(prompt_escaped)
                        break;
                    space.number_of_skins = (u_char)temp_int;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    temp_double = display_prompt_and_echo_double("recolor palette span factor");
                    if(prompt_escaped)
                        break;
                    space.recolor_span_factor = temp_double;
                    break;
                //------------------------------------------------------------
                case 's':
                case 'S':
                    temp_int = display_prompt_and_echo_int("black level");
                    if(prompt_escaped)
                        break;
                    space.black_level = (u_char)temp_int;
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case 't':
                case 'T':
                    space.set_current_menu_name("bit resolution", 0x00040000, true);
                    bit_resolution_menu();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'v':
                case 'V':
                    space.set_current_menu_name("apply view", 0x00300030, true);
                    apply_view_menu();
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::edit_background_color_menu()
{
    space.clean_screen();
    space.render_space();
    int r = display_prompt_and_echo_int("red (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int g = display_prompt_and_echo_int("green (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int b = display_prompt_and_echo_int("blue (0-255)");
    if(prompt_escaped)
        return;

    space.bg_color.r = (u_char)r;
    space.bg_color.g = (u_char)g;
    space.bg_color.b = (u_char)b;
    space.recolor_background();
    space.TUI_clue = "background color";

    return;
}

//############################################################################
void LaserBoy_SDL_GUI::signal_if_not_z_menu()
{
    bool bad_key   = true;
    space.TUI_clue = "signal if not Z";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_if_not_z_options();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    space.TUI_clue = "action canceled";
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '1':
                    space.signal_if_not_z = LASERBOY_NO_SIGNAL;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '2':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_TTL;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '3':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_AVG_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '4':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_OR_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '5':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '6':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_O_SCOPE;
                    bad_key = false;
                    break;
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::dots_setting_menu()
{
    bool bad_key   = true;
    space.TUI_clue = "dots setting";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_dots_options();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    space.TUI_clue = "action canceled";
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '1':
                    space.dots_setting = LASERBOY_DOTS_IGNORE;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '2':
                    space.dots_setting = LASERBOY_DOTS_REMOVE;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '3':
                    space.dots_setting = LASERBOY_DOTS_ENHANCE;
                    bad_key = false;
                    break;
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::bit_resolution_menu()
{
    int temp_int;
    do
    {
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        //--------------------------------------------------------------------
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for X");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[0] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for Y");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[1] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for Z or other");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[5] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for red");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[2] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for green");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[3] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for blue");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[4] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for L audio");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[6] = (u_int)temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        temp_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for R audio");
        if(prompt_escaped)
            break;
        space.signal_bit_mask[7] = (u_int)temp_int;
    } while(0);
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::visuals_settings_menu()
{
    bool in_menu = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_visuals_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit UI visuals";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case '1':
                    space.show_vertices = !space.show_vertices;
                    if(space.show_vertices)
                        space.TUI_clue = "show vertices";
                    else
                        space.TUI_clue = "hide vertices";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.show_blanking = !space.show_blanking;
                    if(space.show_blanking)
                        space.TUI_clue = "show blanking";
                    else
                        space.TUI_clue = "hide blanking";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.show_intro = !space.show_intro;
                    if(space.show_intro)
                        space.TUI_clue = "show intro";
                    else
                        space.TUI_clue = "hide intro";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.show_bridge = !space.show_bridge;
                    if(space.show_bridge)
                        space.TUI_clue = "show bridge";
                    else
                        space.TUI_clue = "hide bridge";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.show_coda = !space.show_coda;
                    if(space.show_coda)
                        space.TUI_clue = "show coda";
                    else
                        space.TUI_clue = "hide coda";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.show_indices = !space.show_indices;
                    if(space.show_indices)
                        space.TUI_clue = "show indices";
                    else
                        space.TUI_clue = "hide indices";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.show_palette = !space.show_palette;
                    if(space.show_palette)
                        space.TUI_clue = "show palette";
                    else
                        space.TUI_clue = "hide palette";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.show_target_palette = !space.show_target_palette;
                    if(space.show_target_palette)
                        space.TUI_clue = "show target palette";
                    else
                        space.TUI_clue = "hide target palette";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.show_stats = !space.show_stats;
                    if(space.show_stats)
                        space.TUI_clue = "show stats";
                    else
                        space.TUI_clue = "hide stats";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.transparent_menu_font = (!space.transparent_menu_font) * 4;
                    if(space.transparent_menu_font == 4)
                        space.TUI_clue = "transparent menu font";
                    else
                        space.TUI_clue = "opaque menu font";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.show_fixed_bounds = !space.show_fixed_bounds;
                    if(space.show_fixed_bounds)
                        space.TUI_clue = "show fixed bounds";
                    else
                        space.TUI_clue = "hide fixed bounds";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.show_fixed_origin = !space.show_fixed_origin;
                    if(space.show_fixed_origin)
                        space.TUI_clue = "show fixed origin";
                    else
                        space.TUI_clue = "hide fixed origin";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.show_floating_axis = !space.show_floating_axis;
                    if(space.show_floating_axis)
                        space.TUI_clue = "show floating axis";
                    else
                        space.TUI_clue = "hide floating axis";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.show_floating_bounds = !space.show_floating_bounds;
                    if(space.show_floating_bounds)
                        space.TUI_clue = "show floating bounds";
                    else
                        space.TUI_clue = "hide floating bounds";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.show_onion_skin = !space.show_onion_skin;
                    if(space.show_onion_skin)
                        space.TUI_clue = "show onion skin";
                    else
                        space.TUI_clue = "hide onion skin";
                    break;
                //------------------------------------------------------------
                case 'f':
                    space.show_onion_visuals = !space.show_onion_visuals;
                    if(space.show_onion_visuals)
                        space.TUI_clue = "show onion visuals";
                    else
                        space.TUI_clue = "hide onion visuals";
                    break;
                //------------------------------------------------------------
                case 'g':
                    space.show_Z_as_order = !space.show_Z_as_order;
                    if(space.show_Z_as_order)
                        space.TUI_clue = "show order in Z";
                    else
                        space.TUI_clue = "show normal 3D";
                    break;
                //------------------------------------------------------------
                case 'h':
                    space.show_quad_view = !space.show_quad_view;
                    if(space.show_quad_view)
                        space.TUI_clue = "show quad view";
                    else
                        space.TUI_clue = "hide quad view";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.show_TUI_clue = !space.show_TUI_clue;
                    if(space.show_TUI_clue)
                        space.TUI_clue = "show UI hint";
                    else
                        space.TUI_clue = "hide UI hint";
                    break;
                //------------------------------------------------------------
                case 'j':
                    space.ui_visuals_flip_flop = !space.ui_visuals_flip_flop;
                    space.show_vertices        =  space.ui_visuals_flip_flop;
                    space.show_blanking        =  space.ui_visuals_flip_flop;
                    space.show_palette         =  space.ui_visuals_flip_flop;
                    space.show_target_palette  =  space.ui_visuals_flip_flop;
                    space.show_stats           =  space.ui_visuals_flip_flop;
                    space.show_fixed_bounds    =  space.ui_visuals_flip_flop;
                    space.show_fixed_origin    =  space.ui_visuals_flip_flop;
                    space.show_floating_axis   =  space.ui_visuals_flip_flop;
                    space.show_floating_bounds =  space.ui_visuals_flip_flop;
                    space.show_onion_skin      =  space.ui_visuals_flip_flop;
                    space.show_onion_visuals   =  space.ui_visuals_flip_flop;
                    space.show_Z_as_order      =  space.ui_visuals_flip_flop;
                    space.show_quad_view       =  space.ui_visuals_flip_flop;
                    space.show_TUI_clue        =  space.ui_visuals_flip_flop;
                    if(space.ui_visuals_flip_flop)
                        space.TUI_clue = "show visuals";
                    else
                        space.TUI_clue = "hide visuals";
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::switch_settings_menu()
{
    bool in_menu   = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.overlay_switch_settings_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case SDLK_RSHIFT:
                case SDLK_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case SDLK_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit switch settings";
                    break;
                //------------------------------------------------------------
                // animation
                //------------------------------------------------------------
                case '1':
                    space.loop_animation = !space.loop_animation;
                    if(space.loop_animation)
                        space.TUI_clue = "loop animation";
                    else
                        space.TUI_clue = "stop animatiion at ends";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.approximate_frame_rate = !space.approximate_frame_rate;
                    if(space.approximate_frame_rate)
                        space.TUI_clue = "aproximate frame rate";
                    else
                        space.TUI_clue = "as fast as possible";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.show_effects_generation = !space.show_effects_generation;
                    if(space.show_effects_generation)
                        space.TUI_clue = "show live rendering";
                    else
                        space.TUI_clue = "hide live rendering";
                    break;
                //------------------------------------------------------------
                // input
                //------------------------------------------------------------
                case '4':
                    space.auto_scale_real = !space.auto_scale_real;
                    if(space.auto_scale_real)
                        space.TUI_clue = "scale dxf import";
                    else
                        space.TUI_clue = "no scale dxf import";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.maintain_real_origin = !space.maintain_real_origin;
                    if(space.maintain_real_origin)
                        space.TUI_clue = "maintain dxf origin";
                    else
                        space.TUI_clue = "ignore dxf origin";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.z_from_unformatted = !space.z_from_unformatted;
                    if(space.z_from_unformatted)
                        space.TUI_clue = "Z in unformatted wave";
                    else
                        space.TUI_clue = "no Z unformatted wave";
                    break;
                //------------------------------------------------------------
                // optimization
                //------------------------------------------------------------
                case '7':
                    space.no_equivalent_vectors = !space.no_equivalent_vectors;
                    if(space.no_equivalent_vectors)
                        space.TUI_clue = "+ equal vector remove";
                    else
                        space.TUI_clue = "- equal vector remove";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.conglomerate_in_minimize = !space.conglomerate_in_minimize;
                    if(space.conglomerate_in_minimize)
                        space.TUI_clue = "+ conglomerate in minimize";
                    else
                        space.TUI_clue = "- conglomerate in minimize";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.fracture_b4_conglomerate = !space.fracture_b4_conglomerate;
                    if(space.fracture_b4_conglomerate)
                        space.TUI_clue = "+ fracture conglomerate";
                    else
                        space.TUI_clue = "- fracture conglomerate";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.black_dwell_vertices = !space.black_dwell_vertices;
                    if(space.black_dwell_vertices)
                        space.TUI_clue = "black dwell";
                    else
                        space.TUI_clue = "lit dwell";
                    break;
                //------------------------------------------------------------
                // color
                //------------------------------------------------------------
                case 'a':
                    space.no_black_match = !space.no_black_match;
                    if(space.no_black_match)
                        space.TUI_clue = "no black match";
                    else
                        space.TUI_clue = "black match palette";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.allow_lit_black = !space.allow_lit_black;
                    if(space.allow_lit_black)
                        space.TUI_clue = "allow lit black";
                    else
                        space.TUI_clue = "blank black";
                    break;
                //------------------------------------------------------------
                // output
                //------------------------------------------------------------
                case 'c':
                    space.auto_minimize = !space.auto_minimize;
                    if(space.auto_minimize)
                        space.TUI_clue = "minimize ild, dxf, txt output";
                    else
                        space.TUI_clue = "raw ild, dxf, txt output";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.dumb_file_format = !space.dumb_file_format;
                    if(space.dumb_file_format)
                        space.TUI_clue = "ild 4,5";
                    else
                        space.TUI_clue = "ild 0,1,2,(!3)";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.save_true_color_dxf = !space.save_true_color_dxf;
                    if(space.save_true_color_dxf)
                        space.TUI_clue = "dxf 24-bit";
                    else
                        space.TUI_clue = "dxf palette";
                    break;
                //------------------------------------------------------------
                case 'f':
                    space.auto_flatten_z = !space.auto_flatten_z;
                    if(space.auto_flatten_z)
                        space.TUI_clue = "flatten Z for wave";
                    else
                        space.TUI_clue = "keep Z for wave";
                    break;
                //------------------------------------------------------------
                case 'g':
                    space.invert_wave_output = !space.invert_wave_output;
                    if(space.invert_wave_output)
                        space.TUI_clue = "inverted wave output";
                    else
                        space.TUI_clue = "positive wave output";
                    break;
                //------------------------------------------------------------
                case 'h':
                    space.auto_apply_offsets = !space.auto_apply_offsets;
                    if(space.auto_apply_offsets)
                        space.TUI_clue = "apply system offsets to wave";
                    else
                        space.TUI_clue = "make wave with no offsets";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.auto_apply_rescales = !space.auto_apply_rescales;
                    if(space.auto_apply_rescales)
                        space.TUI_clue = "apply system rescales to wave";
                    else
                        space.TUI_clue = "make wave linear color";
                    break;
                //------------------------------------------------------------
                case 'j':
                    if(space.channels_of_wav_out == 8)
                    {
                        space.channels_of_wav_out = 6;
                        space.TUI_clue = "6 channel wave out";
                    }
                    else
                    {
                        space.channels_of_wav_out = 8;
                        space.TUI_clue = "8 channel wave out";
                    }
                    break;
                //------------------------------------------------------------
                case 'k':
                    space.save_txt_with_color = !space.save_txt_with_color;
                    if(space.save_txt_with_color)
                        space.TUI_clue = "save txt rgb";
                    else
                        space.TUI_clue = "save txt palette & tables";
                    break;
                //------------------------------------------------------------
                case 'l':
                    space.save_txt_unit = !space.save_txt_unit;
                    if(space.save_txt_unit)
                        space.TUI_clue = "save txt unit";
                    else
                        space.TUI_clue = "save txt short";
                    break;
                //------------------------------------------------------------
                case 'm':
                    space.save_txt_color_hex = !space.save_txt_color_hex;
                    if(space.save_txt_color_hex)
                        space.TUI_clue = "save txt hex";
                    else
                        space.TUI_clue = "save txt r g b";
                    break;
                //------------------------------------------------------------
                case 'n':
                    space.save_txt_named_palettes = !space.save_txt_named_palettes;
                    if(space.save_txt_named_palettes)
                        space.TUI_clue = "save named palettes";
                    else
                        space.TUI_clue = "assume named palettes";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::apply_view_menu()
{
    bool in_menu = true;
    space.simplify_view_angle();
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_veiw_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit view";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case '1':
                    space.view_angle.x += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    space.TUI_clue = "+ orbit X";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.view_angle.x -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    space.TUI_clue = "- orbit X";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.view_angle.y += space.rotation_step;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    space.TUI_clue = "+ orbit Y";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.view_angle.y -= space.rotation_step;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    space.TUI_clue = "- orbit Y";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.view_angle.z += space.rotation_step;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit Z";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.view_angle.z -= space.rotation_step;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit Z";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.view_angle += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.view_angle -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.view_angle   = 0;
                    space.TUI_clue = "front";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.view_angle.x = 0;
                    space.view_angle.y = pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "back";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.view_angle.x = 0;
                    space.view_angle.y = half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "left side";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.view_angle.x = 0;
                    space.view_angle.y = -half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "right side";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.view_angle.x = half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "top";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.view_angle.x = -half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "bottom";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.view_angle += space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index < space.number_of_frames() - 1)
                      )
                        space.pre_incr_current_frame();
                    space.TUI_clue = "+ frame & orbit";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.view_angle -= space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index > 0)
                      )
                        space.pre_decr_current_frame();
                    space.TUI_clue = "- frame & orbit";
                    break;
                //------------------------------------------------------------
                case '9': // go to first frame
                    space.frame(0);
                    space.TUI_clue = "first frame";
                    break;
                //------------------------------------------------------------
                case '0': // go to last frame
                    space.frame(space.number_of_frames() - 1);
                    space.TUI_clue = "last frame";
                    break;
                //------------------------------------------------------------
                case 'q':
                    space.view_offset.x += space.displacement_step;
                    space.TUI_clue = "+ pan X";
                    break;
                //------------------------------------------------------------
                case 'Q':
                    space.view_offset.x -= space.displacement_step;
                    space.TUI_clue = "- pan X";
                    break;
                //------------------------------------------------------------
                case 'w':
                    space.view_offset.y += space.displacement_step;
                    space.TUI_clue = "+ pan Y";
                    break;
                //------------------------------------------------------------
                case 'W':
                    space.view_offset.y -= space.displacement_step;
                    space.TUI_clue = "- pan Y";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.view_offset += space.displacement_step;
                    space.TUI_clue = "+ pan XY";
                    break;
                //------------------------------------------------------------
                case 'E':
                    space.view_offset -= space.displacement_step;
                    space.TUI_clue = "- pan XY";
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    space.view_offset = 0;
                    space.TUI_clue = "on center";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.view_scale.x *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom X";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.view_scale.x *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom X";
                    break;
                //------------------------------------------------------------
                case 's':
                    space.view_scale.y *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom Y";
                    break;
                //------------------------------------------------------------
                case 'S':
                    space.view_scale.y *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom Y";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.view_scale *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom XY";
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.view_scale *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom XY";
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.view_scale = 1.0;
                    space.TUI_clue = "no zoom";
                    break;
                //------------------------------------------------------------
                case 'o':
                    if(!space.current_frame().rotate_around_origin())
                    {
                        space.view_angle = 0.0;
                        space.TUI_clue = "set rotate frame";
                    }
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'O':
                    space.TUI_clue = "rotating all";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_frame_set current(space);
                        if(!space.rotate_around_origin())
                        {
                            space.view_angle = 0.0;
                            space.TUI_clue = "set rotate all";
                        }
                        else
                        {
                            space = current;
                            display_error("out of bounds!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 't':
                    if(!space.current_frame().move())
                    {
                        space.view_offset = 0;
                        space.TUI_clue = "set offset frame";
                    }
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'T':
                    space.TUI_clue = "offsetting all";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_frame_set current(space);
                        if(!space.move())
                        {
                            space.view_offset = 0;
                            space.TUI_clue = "set offset all";
                        }
                        else
                        {
                            space = current;
                            display_error("out of bounds!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(!space.current_frame().scale_around_origin())
                    {
                        space.view_scale = 1.0;
                        space.TUI_clue = "set scale frame";
                    }
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'G':
                    space.TUI_clue = "scaling all";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_frame_set current(space);
                        if(!space.scale_around_origin())
                        {
                            space.view_scale = 1.0;
                            space.TUI_clue = "set scale all";
                        }
                        else
                        {
                            space = current;
                            display_error("out of bounds!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::palette_transforms_menu()
{
    bool in_menu                 = true,
         show_menu_was           = space.show_menu,
         show_palette_was        = space.show_palette,
         show_target_palette_was = space.show_target_palette;
    space.show_menu              = true;
    space.show_palette           = true;
    space.show_target_palette    = true;
    space.show_color_cursor      = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_palette_tranz_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu                   = false;
                    space.show_color_cursor   = false;
                    space.show_menu           = show_menu_was;
                    space.show_palette        = show_palette_was;
                    space.show_target_palette = show_target_palette_was;
                    space.TUI_clue            = "exit palette transforms";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                // palette transforms (to frame)
                //------------------------------------------------------------
                case '1':
                    space.current_frame().rainbow_recolor(0);
                    space.TUI_clue = "span X frame";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.TUI_clue = "span X all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(0);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.current_frame().rainbow_recolor(1);
                    space.TUI_clue = "span Y frame";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.TUI_clue = "span Y all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(1);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.current_frame().rainbow_recolor(2);
                    space.TUI_clue = "span Z frame";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.TUI_clue = "span Z all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(2);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.current_frame().rainbow_recolor(3);
                    space.TUI_clue = "span radially from origin";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.TUI_clue = "span radially from origin";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(3);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.current_frame().rainbow_recolor(4);
                    space.TUI_clue = "span angular around origin";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.TUI_clue = "span angular around origin";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(4);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.current_frame().rainbow_recolor(5);
                    space.TUI_clue = "span vertices frame";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.TUI_clue = "span vertices all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(5);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.current_frame().rainbow_recolor(6);
                    space.TUI_clue = "index vertices frame";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.TUI_clue = "index vertices all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(6);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.current_frame().rainbow_recolor(7);
                    space.TUI_clue = "span segments frame";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.TUI_clue = "span segments all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(7);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.current_frame().rainbow_recolor(8);
                    space.TUI_clue = "index segments frame";
                    break;
                //------------------------------------------------------------
                case '(':
                    space.TUI_clue = "index segments all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(8);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.current_frame().rainbow_recolor(9);
                    space.TUI_clue = "random vertices frame";
                    break;
                //------------------------------------------------------------
                case ')':
                    space.TUI_clue = "random vertices all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(9);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.current_frame().rainbow_recolor(10);
                    space.TUI_clue = "random segments frame";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.TUI_clue = "random segments all";
                    space.render_space();
                    display_space();
                    space.rainbow_recolor(10);
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.current_frame().best_match_target_palette();
                    space.TUI_clue = "best match frame";
                    break;
                //------------------------------------------------------------
                case 'B':
                    space.TUI_clue = "best match all";
                    space.render_space();
                    display_space();
                    space.best_match_target_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.current_frame().to_target_palette_by_index();
                    space.TUI_clue = "by index frame";
                    break;
                //------------------------------------------------------------
                case 'C':
                    space.TUI_clue = "by index all";
                    space.render_space();
                    display_space();
                    space.to_target_palette_by_index();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.current_frame().strip_color();
                    space.TUI_clue = "strip color frame";
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.TUI_clue = "strip color all";
                    space.render_space();
                    display_space();
                    space.strip_color();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.current_frame().strip_color_or();
                    space.TUI_clue = "strip color frame";
                    break;
                //------------------------------------------------------------
                case 'E':
                    space.TUI_clue = "strip color all";
                    space.render_space();
                    display_space();
                    space.strip_color_or();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'f':
                    space.current_frame().strip_color_avg();
                    space.TUI_clue = "strip color frame";
                    break;
                //------------------------------------------------------------
                case 'F':
                    space.TUI_clue = "strip color all";
                    space.render_space();
                    display_space();
                    space.strip_color_avg();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'g':
                    space.current_frame().sync_rgb_and_palette();
                    space.TUI_clue = "24-bit to palette frame";
                    break;
                //------------------------------------------------------------
                case 'G':
                    space.TUI_clue = "24-bit to palette all";
                    space.render_space();
                    display_space();
                    space.minimize_tables_and_palettes();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'h':
                    space.current_frame().bit_reduce_to_palette();
                    space.TUI_clue = "888 to 332 frame";
                    break;
                //------------------------------------------------------------
                case 'H':
                    space.TUI_clue = "888 to 332 all";
                    space.render_space();
                    display_space();
                    space.bit_reduce_to_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.current_frame().best_reduce_to_palette();
                    space.TUI_clue = "best reduce to minimum palette";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.TUI_clue = "best reduce to minimum palette";
                    space.render_space();
                    display_space();
                    space.best_reduce_to_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'j':
                    space.current_frame().promote_to_true_color();
                    space.TUI_clue = "promote to 24bit frame";
                    break;
                //------------------------------------------------------------
                case 'J':
                    space.TUI_clue = "promote to 24bit all";
                    space.render_space();
                    display_space();
                    space.promote_to_true_color();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                // palette transforms (to palettes and set)
                //------------------------------------------------------------
                case 'k':
                case 'K':
                    space.TUI_clue = "best reduce all to 1";
                    space.render_space();
                    display_space();
                    space.best_reduce_all_to_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'l':
                case 'L':
                    space.TUI_clue = "clear unused palettes";
                    space.render_space();
                    display_space();
                    space.clear_unused_palettes();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'm':
                case 'M':
                    space.add_palette();
                    break;
                //------------------------------------------------------------
                case 'n':
                case 'N':
                    if(space.target_palette_index <  LASERBOY_BUILT_IN_PALETTES)
                    {
                        display_error("built in");
                        wait_4_Esc();
                    }
                    else if(!space.omit_palette(space.target_palette_index))
                    {
                        display_error("in use");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'o':
                case 'O':
                    space.copy_palette(space.target_palette_index);
                    break;
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.selected_color_index >= space.greater_number_of_colors())
                        space.selected_color_index = 0;
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.selected_color_index == 0)
                        space.selected_color_index = space.current_palette().number_of_colors();
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case 'q':
                case 'Q':
                    add_color_to_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    edit_color_in_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 's':
                case 'S':
                    insert_color_in_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 't':
                case 'T':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        space.add_palette();
                        space.palette_picker(space.number_of_palettes() - 1) = space.target_palette();
                        space.target_palette_index = space.number_of_palettes() - 1;
                        space.target_palette().name = space.GUID8char();
                        space.target_palette().erase_index(space.selected_color_index);
                    }
                    else
                        space.target_palette().erase_index(space.selected_color_index);
                    space.sync_rgb_and_palette();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        int temp = space.target_palette_index;
                        space.add_palette();
                        string name = space.target_palette().name;
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = name;
                        space.target_palette().clear_from_index(space.selected_color_index);
                    }
                    else
                        space.target_palette().clear_from_index(space.selected_color_index);
                    space.sync_rgb_and_palette();
                    break;
                //------------------------------------------------------------
                case 'v':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        int temp = space.target_palette_index;
                        space.add_palette();
                        string name = space.target_palette().name;
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = name;
                    }
                    space.target_palette().straight_blend();
                    break;
                //------------------------------------------------------------
                case 'V':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        int temp = space.target_palette_index;
                        space.add_palette();
                        string name = space.target_palette().name;
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = name;
                    }
                    space.target_palette().circular_blend();
                    break;
                //------------------------------------------------------------
                case '.': // show frame set stats
                    if(space.palette_index >= 0)
                    {
                        space.target_palette_index = space.palette_index;
                        space.TUI_clue = "target to current";
                    }
                    else
                        space.TUI_clue = "current is -24-bit-";
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            }
        }
    }
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::add_color_to_target_palette_menu()
{
    if(space.target_palette().number_of_colors() >= 255)
    {
        display_error("palette is full");
        wait_4_Esc();
        return;
    }

    space.clean_screen();
    space.render_space();
    int r = display_prompt_and_echo_int("red (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int g = display_prompt_and_echo_int("green (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int b = display_prompt_and_echo_int("blue (0-255)");
    if(prompt_escaped)
        return;

    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
    }
    space.target_palette().add_color(LaserBoy_color((u_char)r, (u_char)g, (u_char)b));
    space.TUI_clue = "new color";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::insert_color_in_target_palette_menu()
{
    if(space.target_palette().number_of_colors() >= 255)
    {
        display_error("palette is full");
        wait_4_Esc();
        return;
    }

    space.clean_screen();
    space.render_space();
    int r = display_prompt_and_echo_int("red (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int g = display_prompt_and_echo_int("green (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int b = display_prompt_and_echo_int("blue (0-255)");
    if(prompt_escaped)
        return;

    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
    }
    space.target_palette().insert_color(LaserBoy_color((u_char)r, (u_char)g, (u_char)b), space.selected_color_index);
    int frame_index = space.frame_index;
    space.sync_rgb_and_palette();
    space.frame_index = frame_index;
    space.TUI_clue = "new color";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::edit_color_in_target_palette_menu()
{
    if(space.selected_color_index > space.target_palette().number_of_colors())
    {
        display_error("color index out of range");
        wait_4_Esc();
        return;
    }

    space.clean_screen();
    space.render_space();
    int r = display_prompt_and_echo_int("red (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int g = display_prompt_and_echo_int("green (0-255)");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int b = display_prompt_and_echo_int("blue (0-255)");
    if(prompt_escaped)
        return;

    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
        space.target_palette().set_index_color(space.selected_color_index, (u_char)r, (u_char)g, (u_char)b);
    }
    else
        space.target_palette().set_index_color(space.selected_color_index, r, g, b);
    int frame_index = space.frame_index;
    space.sync_rgb_and_palette();
    space.frame_index = frame_index;
    space.TUI_clue = "edit color";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::frame_transforms_menu()
{
    bool in_menu           = true,
         show_vertices_was = space.show_vertices,
         show_blanking_was = space.show_blanking,
         show_menu_was     = space.show_menu;
    space.show_vertices    = true;
    space.show_blanking    = true;
    space.show_menu        = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_frame_tranz_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.show_vertices = show_vertices_was;
                    space.show_blanking = show_blanking_was;
                    space.TUI_clue = "exit frame transforms";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case '1':
                    space.current_frame().flatten_z();
                    space.TUI_clue = "flatten Z";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.TUI_clue = "flattening Z";
                    space.render_space();
                    display_space();
                    space.flatten_z();
                    space.TUI_clue = "flatten Z all";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.current_frame().z_order_vertices(64000);
                    space.TUI_clue = "z order vertices";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.TUI_clue = "Z ordering vertices";
                    space.render_space();
                    display_space();
                    space.z_order_vertices(64000);
                    space.TUI_clue = "Z order vertices all";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.current_frame().flip(0);
                    space.TUI_clue = "mirror";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.TUI_clue = "mirroring";
                    space.render_space();
                    display_space();
                    space.flip(0);
                    space.TUI_clue = "mirror all";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.current_frame().flip(1);
                    space.TUI_clue = "flip";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.TUI_clue = "flipping";
                    space.render_space();
                    display_space();
                    space.flip(1);
                    space.TUI_clue = "flip all";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.current_frame().quarter_turn(0, 1);
                    space.TUI_clue = "quarter turn CW";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.TUI_clue = "quarter turning CW";
                    space.render_space();
                    display_space();
                    space.quarter_turn(0, 1);
                    space.TUI_clue = "quarter turn CW all";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.current_frame().quarter_turn(0, 3);
                    space.TUI_clue = "quarter turn CCW";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.TUI_clue = "quarter turning CCW";
                    space.render_space();
                    display_space();
                    space.quarter_turn(0, 3);
                    space.TUI_clue = "quarter turn CCW all";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.current_frame().reverse();
                    space.TUI_clue = "reverse vector order";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.TUI_clue = "reversing vector order";
                    space.render_space();
                    display_space();
                    space.frame_reverse();
                    space.TUI_clue = "reverse vector order all";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.current_frame().impose_bit_resolution();
                    space.TUI_clue = "imposed bit resolution";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.TUI_clue = "imposing bit resolution";
                    space.render_space();
                    display_space();
                    space.impose_bit_resolution();
                    space.TUI_clue = "imposed bit resolution all";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.current_frame().convert_black_to_blank();
                    space.TUI_clue = "black to blank";
                    break;
                //------------------------------------------------------------
                case '(':
                    space.TUI_clue = "black to blank";
                    space.render_space();
                    display_space();
                    space.convert_black_to_blank();
                    space.TUI_clue = "black to blank all";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.current_frame().convert_blank_to_black();
                    space.TUI_clue = "blank to black";
                    break;
                //------------------------------------------------------------
                case ')':
                    space.TUI_clue = "black to blank";
                    space.render_space();
                    display_space();
                    space.convert_blank_to_black();
                    space.TUI_clue = "blank to black all";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.current_frame().impose_black_level();
                    space.TUI_clue = "apply black level";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.TUI_clue = "applying black level";
                    space.render_space();
                    display_space();
                    space.impose_black_level();
                    space.TUI_clue = "black leveled all";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.current_frame().to_dots();
                    space.TUI_clue = "to dots";
                    break;
                //------------------------------------------------------------
                case 'B':
                    space.TUI_clue = "converting to dots";
                    space.render_space();
                    display_space();
                    space.to_dots();
                    space.TUI_clue = "to dots all";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.current_frame().reduce_blank_vectors();
                    space.TUI_clue = "reduced blank span";
                    break;
                //------------------------------------------------------------
                case 'C':
                    space.TUI_clue = "reduce blank vectors";
                    space.render_space();
                    display_space();
                    space.reduce_blank_vectors();
                    space.TUI_clue = "reduced blank span all";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.current_frame().remove_dwell_vertices();
                    space.TUI_clue = "removed angle dwell";
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.TUI_clue = "removing dwell vertices";
                    space.render_space();
                    display_space();
                    space.remove_dwell_vertices();
                    space.TUI_clue = "removed angle dwell all";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.current_frame().remove_dots();
                    space.TUI_clue = "removed dots";
                    break;
                //------------------------------------------------------------
                case 'E':
                    space.TUI_clue = "removing dots";
                    space.render_space();
                    display_space();
                    space.remove_dots();
                    space.TUI_clue = "removed dots all";
                    break;
                //------------------------------------------------------------
                case 'f':
                    space.current_frame().remove_short_vectors();
                    space.TUI_clue = "removed short vectors";
                    break;
                //------------------------------------------------------------
                case 'F':
                    space.TUI_clue = "removing short vectors";
                    space.render_space();
                    display_space();
                    space.remove_short_vectors();
                    space.TUI_clue = "removed short vectors all";
                    break;
                //------------------------------------------------------------
                case 'g':
                    space.current_frame().reduce_lit_vectors();
                    space.TUI_clue = "reduced vectors";
                    break;
                //------------------------------------------------------------
                case 'G':
                    space.TUI_clue = "reducing vectors";
                    space.render_space();
                    display_space();
                    space.reduce_lit_vectors();
                    space.TUI_clue = "reduced vectors all";
                    break;
                //------------------------------------------------------------
                case 'h':
                    space.current_frame().omit_equivalent_vectors();
                    space.TUI_clue = "removed equal vectors";
                    break;
                //------------------------------------------------------------
                case 'H':
                    space.TUI_clue = "removing equal vectors";
                    space.render_space();
                    display_space();
                    space.omit_equivalent_vectors();
                    space.TUI_clue = "removed equal vectors";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.current_frame().conglomerate_lit_segments();
                    space.TUI_clue = "conglomerate segments";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.TUI_clue = "conglomerate segments";
                    space.render_space();
                    display_space();
                    space.conglomerate_lit_segments();
                    space.TUI_clue = "conglomerated segments all";
                    break;
                //------------------------------------------------------------
                case 'j':
                    space.current_frame().bond_segments();
                    space.TUI_clue = "bonded segments";
                    break;
                //------------------------------------------------------------
                case 'J':
                    space.TUI_clue = "bonding segments";
                    space.render_space();
                    display_space();
                    space.bond_segments();
                    space.TUI_clue = "bonded segments all";
                    break;
                //------------------------------------------------------------
                case 'k':
                    space.current_frame().minimize(0);
                    space.TUI_clue = "minimized vectors";
                    break;
                //------------------------------------------------------------
                case 'K':
                    space.TUI_clue = "minimizing vectors";
                    space.render_space();
                    display_space();
                    space.minimize();
                    space.TUI_clue = "minimized vectors all";
                    break;
                //------------------------------------------------------------
                case 'l':
                    if(space.current_frame().fracture_segments())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "fracture segments fail";
                    }
                    else
                        space.TUI_clue = "fractured segments";
                    break;
                //------------------------------------------------------------
                case 'L':
                    space.TUI_clue = "fracturing segments";
                    space.render_space();
                    display_space();
                    space.fracture_segments();
                    space.TUI_clue = "fractured segments all";
                    break;
                //------------------------------------------------------------
                case 'm':
                    space.current_frame().randomize_segments();
                    space.TUI_clue = "randomize segments";
                    break;
                //------------------------------------------------------------
                case 'M':
                    space.TUI_clue = "randomizing segments";
                    space.render_space();
                    display_space();
                    space.randomize_segments();
                    space.TUI_clue = "randomized segments all";
                    break;
                //------------------------------------------------------------
                case 'n':
                {
                    LaserBoy_segment origin;
                    origin.push_back(LaserBoy_vertex());
                    origin.push_back(LaserBoy_vertex());
                    if(space.current_frame().reorder_segments(origin))
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "reorder segments fail";
                    }
                    else
                        space.TUI_clue = "reordered segments";
                    break;
                }
                //------------------------------------------------------------
                case 'N':
                    space.TUI_clue = "reordering segments";
                    space.render_space();
                    display_space();
                    if(space.reorder_segments())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "reorder segments fail";
                    }
                    else
                        space.TUI_clue = "reordered segments all";
                    break;
                //------------------------------------------------------------
                case 'o':
                    if(space.current_frame().add_dwell())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "angle dwell fail";
                    }
                    else
                        space.TUI_clue = "added angle dwell";
                    break;
                //------------------------------------------------------------
                case 'O':
                    space.TUI_clue = "adding angle dwell";
                    space.render_space();
                    display_space();
                    if(space.add_dwell())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "angle dwell fail";
                    }
                    else
                        space.TUI_clue = "added angle dwell all";
                    break;
                //------------------------------------------------------------
                case 'p':
                    if(space.current_frame().add_lit_span_vertices())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "distance span fail";
                    }
                    else
                        space.TUI_clue = "added lit distance span";
                    break;
                //------------------------------------------------------------
                case 'P':
                    space.TUI_clue = "adding lit distance span";
                    space.render_space();
                    display_space();
                    if(space.add_lit_span_vertices())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "distance span fail";
                    }
                    else
                        space.TUI_clue = "added lit distance span all";
                    break;
                //------------------------------------------------------------
                case 'q':
                    if(space.current_frame().add_blank_span_vertices())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "distance span fail";
                    }
                    else
                        space.TUI_clue = "added blank distance span";
                    break;
                //------------------------------------------------------------
                case 'Q':
                    space.TUI_clue = "adding blank distance span";
                    space.render_space();
                    display_space();
                    if(space.add_blank_span_vertices())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "distance span fail";
                    }
                    else
                        space.TUI_clue = "added blank distance span all";
                    break;
                //------------------------------------------------------------
                case 'r':
                    if(space.current_frame().enhance_dots())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "enhance dots fail";
                    }
                    else
                        space.TUI_clue = "enhanced dots";
                    break;
                //------------------------------------------------------------
                case 'R':
                    space.TUI_clue = "enhancing dots";
                    space.render_space();
                    display_space();
                    if(space.enhance_dots())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "enhance dots fail";
                    }
                    else
                        space.TUI_clue = "enhanced dots";
                    break;
                //------------------------------------------------------------
                case 's':
                    space.current_frame().minimize(0);
                    if(space.current_frame().optimize(0))
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "optimize fail";
                    }
                    else
                        space.TUI_clue = "optimized";
                    break;
                //------------------------------------------------------------
                case 'S':
                    space.TUI_clue = "optimizing";
                    space.render_space();
                    display_space();
                    if(space.optimize())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "optimize fail";
                    }
                    else
                        space.TUI_clue = "optimized all";
                    break;
                //------------------------------------------------------------
                case 't':
                    space.current_frame().name = display_prompt_and_echo_name("new frame name [8 char or less]").substr(0, 8);
                    space.TUI_clue = "rename";
                    break;
                //------------------------------------------------------------
                case 'T':
                    space.current_frame().name = display_prompt_and_echo_name("new frames prefix [3 char or less]").substr(0, 3);
                    space.rename_all_frames(space.current_frame().name);
                    space.TUI_clue = "rename";
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            }
        }
    }
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::frame_set_transforms_menu()
{
    bool in_menu       = true,
         show_menu_was = space.show_menu;
    int  temp_int;
    space.show_menu    = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_frame_set_tranz_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit frame set transforms";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case '1':
                    space.TUI_clue = "inverting frame selections";
                    space.render_space();
                    display_space();
                    space.invert_frame_selections();
                    space.TUI_clue = "inverted frame selections";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.TUI_clue = "deleting selected frames";
                    space.render_space();
                    display_space();
                    space.delete_selected_frames();
                    space.TUI_clue = "deleted selected frames";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.TUI_clue = "trimming to selected frames";
                    space.render_space();
                    display_space();
                    space.trim_to_selected_frames();
                    space.TUI_clue = "trimmed to selected frames";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.TUI_clue = "deleting redundant frames";
                    space.render_space();
                    display_space();
                    space.delete_redundant_frames();
                    space.TUI_clue = "redundant frames deleted";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.TUI_clue = "copying selected frames";
                    space.render_space();
                    display_space();
                    space = space.selected_frames() + space;
                    space.TUI_clue = "copied frames to begining";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.TUI_clue = "copying selected frames";
                    space.render_space();
                    display_space();
                    space += space.selected_frames();
                    space.TUI_clue = "copied frames to end";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.TUI_clue = "moving selected frames";
                    space.render_space();
                    display_space();
                    {
                        LaserBoy_frame_set frame_set(space.selected_frames());
                        space.delete_selected_frames();
                        frame_set += space;
                        space = frame_set;
                    }
                    space.TUI_clue = "moved frames to begining";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.TUI_clue = "moving selected frames";
                    space.render_space();
                    display_space();
                    {
                        LaserBoy_frame_set frame_set(space.selected_frames());
                        space.delete_selected_frames();
                        space += frame_set;
                    }
                    space.TUI_clue = "moved frames to end";
                    break;
                //------------------------------------------------------------
                case '9':
                    {
                        LaserBoy_frame_set frame_set(&space);
                        frame_set += LaserBoy_frame(&space, space.target_palette_index, true); // not quite an empty frame!
                        space = frame_set + space;
                    }
                    space.TUI_clue = "add frame to begining";
                    break;
                //------------------------------------------------------------
                case '0':
                    {
                        u_int              i;
                        LaserBoy_frame_set frame_set(&space);
                        for(i = 0; i <= space.frame_index; i++)
                            frame_set += space[i];
                        frame_set += LaserBoy_frame(&space, space.target_palette_index, true);
                        for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                            frame_set += space[i];
                        space = frame_set;
                    }
                    space.TUI_clue = "add after current frame";
                    break;
                //------------------------------------------------------------
                case 'a':
                    {
                        LaserBoy_frame frame(&space, space.target_palette_index, true);
                        LaserBoy_frame_set frame_set(space);
                        frame_set += frame;
                        space = frame_set;
                    }
                    space.TUI_clue = "add frame to end";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.TUI_clue = "reversing order of frames";
                    space.render_space();
                    display_space();
                    space.reverse();
                    space.TUI_clue = "reverse order";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.TUI_clue = "reversing selected frames";
                    space.render_space();
                    display_space();
                    space.reverse_selected();
                    space.TUI_clue = "reverse selected";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.TUI_clue = "new first frame";
                    space.render_space();
                    display_space();
                    space.make_current_first();
                    space.TUI_clue = "new first frame";
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.TUI_clue = "random order frames";
                    space.render_space();
                    display_space();
                    space.random_order();
                    space.TUI_clue = "random order";
                    break;
                //------------------------------------------------------------
                case 'f':
                    space.TUI_clue = "remove every n-th frames";
                    space.render_space();
                    display_space();
                    temp_int = display_prompt_and_echo_int("n");
                    if(prompt_escaped)
                        break;
                    space.delete_every_nth_frame(temp_int);
                    space.TUI_clue = "n-th frame omit";
                    break;
                //------------------------------------------------------------
                case 'g':
                    space.TUI_clue = "keep only every n-th frames";
                    space.render_space();
                    display_space();
                    temp_int = display_prompt_and_echo_int("n");
                    if(prompt_escaped)
                        break;
                    space.keep_every_nth_frame(temp_int);
                    space.TUI_clue = "n-th frame keep";
                    break;
                //------------------------------------------------------------
                case '>': // explode current frame
                    space.explode_current_frame();
                    space.TUI_clue = "explode segments";
                    break;
                //------------------------------------------------------------
                case '<': // collapse selected frames
                    if(space.collapse_selected_frames())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "collapse segments fail";
                    }
                    else
                        space.TUI_clue = "collapse segments";
                    break;
                //------------------------------------------------------------
                case '!': // explode all frames
                    space.explode_all_frames();
                    space.TUI_clue = "explode segments";
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            }
        }
    }
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_color_blank_menu()
{
    bool    in_menu           = true,
            show_vertices_was = space.show_vertices,
            show_blanking_was = space.show_blanking,
            show_intro_was    = space.show_intro,
            show_bridge_was   = space.show_bridge,
            show_coda_was     = space.show_coda,
            show_palette_was  = space.show_palette,
            show_menu_was     = space.show_menu;
    int     temp_int;
    double  temp_double;
    LaserBoy_3D_double temp_3D_float;
    //------------------------------------------------------------------------
    space.show_menu         = true;
    space.show_vertices     = true;
    space.show_blanking     = true;
    space.show_intro        = false;
    space.show_bridge       = false;
    space.show_coda         = false;
    space.show_palette      = true;
    space.show_cursors      = true;
    space.show_color_cursor = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_draw_color_blank_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    in_menu                 = false;
                    space.show_cursors      = false;
                    space.show_color_cursor = false;
                    space.show_vertices     = show_vertices_was;
                    space.show_blanking     = show_blanking_was;
                    space.show_intro        = show_intro_was;
                    space.show_bridge       = show_bridge_was;
                    space.show_coda         = show_coda_was;
                    space.show_palette      = show_palette_was;
                    space.TUI_clue          = "exit draw color blank";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.current_frame().remove_at_spider();
                    space.TUI_clue = "omit vertex";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case '1':
                    space.view_angle.x += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    space.TUI_clue = "+ orbit X";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.view_angle.x -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    space.TUI_clue = "- orbit X";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.view_angle.y += space.rotation_step;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    space.TUI_clue = "+ orbit Y";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.view_angle.y -= space.rotation_step;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    space.TUI_clue = "- orbit Y";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.view_angle.z += space.rotation_step;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit Z";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.view_angle.z -= space.rotation_step;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit Z";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.view_angle += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.view_angle -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.view_angle   = 0;
                    space.TUI_clue     = "front";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.view_angle.x = 0;
                    space.view_angle.y = pi;
                    space.view_angle.z = 0;
                    space.TUI_clue     = "back";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.view_angle.x = 0;
                    space.view_angle.y = half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue     = "left side";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.view_angle.x = 0;
                    space.view_angle.y = -half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue     = "right side";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.view_angle.x = half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue     = "top";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.view_angle.x = -half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue     = "bottom";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.view_angle += space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index < space.number_of_frames() - 1)
                      )
                        space.pre_incr_current_frame();
                    space.TUI_clue = "+ frame & orbit";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.view_angle -= space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index > 0)
                      )
                        space.pre_decr_current_frame();
                    space.TUI_clue = "- frame & orbit";
                    break;
                //------------------------------------------------------------
                case '[':
                    space.current_frame().increment_egg();
                    space.TUI_clue = "+ start vertex select";
                    break;
                //------------------------------------------------------------
                case '{':
                    space.current_frame().deccrement_egg();
                    space.TUI_clue = "- start vertex select";
                    break;
                //------------------------------------------------------------
                case ']':
                    space.current_frame().increment_spider();
                    space.TUI_clue = "+ end vertex select";
                    break;
                //------------------------------------------------------------
                case '}':
                    space.current_frame().deccrement_spider();
                    space.TUI_clue = "- end vertex select";
                    break;
                //------------------------------------------------------------
                case'\\':
                    space.current_frame().spider = (space.current_frame().size() - 1);
                    space.TUI_clue = "last vertex select";
                    break;
                //------------------------------------------------------------
                case '|':
                    space.current_frame().egg = 0;
                    space.TUI_clue = "first vertex select";
                    break;
                //------------------------------------------------------------
                case 'u':
                    space.current_frame().next_segment_select();
                    space.TUI_clue = "+ lit segment";
                    break;
                //------------------------------------------------------------
                case 'U':
                    space.current_frame().previous_segment_select();
                    space.TUI_clue = "- lit segment";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.current_frame().next_segment_egg();
                    space.TUI_clue = "+ start lit segment";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.current_frame().previous_segment_egg();
                    space.TUI_clue = "- start lit segment";
                    break;
                //------------------------------------------------------------
                case 'o':
                    space.current_frame().next_segment_spider();
                    space.TUI_clue = "+ end lit segment";
                    break;
                //------------------------------------------------------------
                case 'O':
                    space.current_frame().previous_segment_spider();
                    space.TUI_clue = "- end lit segment";
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case 'q':
                    temp_int = display_prompt_and_echo_int("set X");
                    if(prompt_escaped)
                        break;
                    space.current_frame().at(space.current_frame().spider).x = temp_int;
                    space.TUI_clue = "X set";
                    break;
                //------------------------------------------------------------
                case 'w':
                    temp_int = display_prompt_and_echo_int("set Y");
                    if(prompt_escaped)
                        break;
                    space.current_frame().at(space.current_frame().spider).y = temp_int;
                    space.TUI_clue = "Y set";
                    break;
                //------------------------------------------------------------
                case 'e':
                    temp_int = display_prompt_and_echo_int("set Z");
                    if(prompt_escaped)
                        break;
                    space.current_frame().at(space.current_frame().spider).z = temp_int;
                    space.TUI_clue = "Z set";
                    break;
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.selected_color_index >= space.current_palette().number_of_colors())
                        space.selected_color_index = 0;
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.selected_color_index == 0)
                        space.selected_color_index = space.current_palette().number_of_colors();
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.current_frame().unblank_all_vertices();
                    space.TUI_clue = "unblank all vectors";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.unblank_all_vertices();
                    space.TUI_clue = "unblank all frames";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.current_frame().blank_vertices();
                    space.TUI_clue = "blank selected vectors";
                    break;
                //------------------------------------------------------------
                case 'B':
                    space.current_frame().unblank_vertices();
                    space.TUI_clue = "unblank selected vectors";
                    break;
                //------------------------------------------------------------
                case 'n':
                    space.current_frame().black_vertices();
                    space.TUI_clue = "bond black selected vectors";
                    break;
                //------------------------------------------------------------
                case 'N':
                    space.current_frame().unblack_vertices();
                    space.TUI_clue = "unbond unblack selected vectors";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.current_frame().color_select();
                    space.TUI_clue = "color selected vectors";
                    break;
                //------------------------------------------------------------
                case 'C':
                    space.selected_color_index = space.current_frame().at(space.current_frame().spider).c;
                    space.TUI_clue = "get color";
                    break;
                //------------------------------------------------------------
                case '-':
                    space.current_frame().color_index_select();
                    space.TUI_clue = "color index selected vectors";
                    break;
                //------------------------------------------------------------
                case '_':
                    space.current_frame().color_span_select();
                    space.TUI_clue = "color span selected vectors";
                    break;
                //------------------------------------------------------------
                case '~':
                    space.current_frame().color_black_in_select();
                    space.TUI_clue = "color black selected vectors";
                    break;
                //------------------------------------------------------------
                case '=':
                    space.current_frame().color_index_black();
                    space.TUI_clue = "color index black";
                    break;
                //------------------------------------------------------------
                case '+':
                    space.current_frame().color_span_black();
                    space.TUI_clue = "color span black";
                    break;
                //------------------------------------------------------------
                case 'h':
                    space.current_frame().color_cycle_select(1);
                    space.TUI_clue = "+ color cycle";
                    break;
                //------------------------------------------------------------
                case 'H':
                    space.current_frame().color_cycle_select(-1);
                    space.TUI_clue = "- color cycle";
                    break;
                //------------------------------------------------------------
                case '.':
                    if(space.current_frame().add_vertex(space.current_frame().spider))
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "add vertex fail";
                    }
                    else
                    {
                        if(space.current_frame().spider < (space.current_frame().size() - 1))
                            space.TUI_clue = "split vector";
                        else
                            space.TUI_clue = "add vector";
                    }
                    break;
                //------------------------------------------------------------
                case '>':
                    if(space.current_frame().break_segment())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "break segment fail";
                    }
                    else
                        space.TUI_clue = "break segment";
                    break;
                //------------------------------------------------------------
                case ',':
                    space.current_frame().reorder_from_egg();
                    space.TUI_clue = "reorder from egg";
                    break;
                //------------------------------------------------------------
                case '<':
                    space.current_frame().reorder_from_spider();
                    space.TUI_clue = "reorder from spider";
                    break;
                //------------------------------------------------------------
                case 't':
                    space.current_frame().selected_to_dots();
                    space.TUI_clue = "to dots";
                    break;
                //------------------------------------------------------------
                case '\'':
                    {
                        int index = display_prompt_and_echo_int("place egg at index");
                        if(prompt_escaped)
                            break;
                        space.current_frame().egg = index;
                        space.current_frame().normalize_cursors();
                    }
                    break;
                //------------------------------------------------------------
                case '"':
                    {
                        int index = display_prompt_and_echo_int("place spider at index");
                        if(prompt_escaped)
                            break;
                        space.current_frame().spider = index;
                        space.current_frame().normalize_cursors();
                    }
                    break;
                //------------------------------------------------------------
                case 'j':
                    space.current_frame().set_spider_to_egg();
                    space.TUI_clue = "spider to egg";
                    break;
                //------------------------------------------------------------
                case 'J':
                    space.current_frame().set_egg_to_spider();
                    space.TUI_clue = "egg to spider";
                    break;

                //------------------------------------------------------------
                case 'l':
                    if(space.current_frame().connect_the_dots())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "line egg to spider fail";
                    }
                    else
                        space.TUI_clue = "line egg to spider";
                    break;
                //------------------------------------------------------------
                case 'x':
                    temp_double = space.current_frame().at(space.current_frame().spider).x + space.displacement_step;
                    if(temp_double <= LASERBOY_MAX_SHORT)
                        space.current_frame().at(space.current_frame().spider).x = (u_short)temp_double;
                    space.TUI_clue = "+ X";
                    break;
                //------------------------------------------------------------
                case 'X':
                    temp_double = space.current_frame().at(space.current_frame().spider).x - space.displacement_step;
                    if(temp_double >= LASERBOY_MIN_SHORT)
                        space.current_frame().at(space.current_frame().spider).x = (u_short)temp_double;
                    space.TUI_clue = "- X";
                    break;
                //------------------------------------------------------------
                case 'y':
                    temp_double = space.current_frame().at(space.current_frame().spider).y + space.displacement_step;
                    if(temp_double <= LASERBOY_MAX_SHORT)
                        space.current_frame().at(space.current_frame().spider).y = (u_short)temp_double;
                    space.TUI_clue = "+ Y";
                    break;
                //------------------------------------------------------------
                case 'Y':
                    temp_double = space.current_frame().at(space.current_frame().spider).y - space.displacement_step;
                    if(temp_double >= LASERBOY_MIN_SHORT)
                        space.current_frame().at(space.current_frame().spider).y = (u_short)temp_double;
                    space.TUI_clue = "- Y";
                    break;
                //------------------------------------------------------------
                case 'z':
                    temp_double = space.current_frame().at(space.current_frame().spider).z + space.displacement_step;
                    if(temp_double <= LASERBOY_MAX_SHORT)
                        space.current_frame().at(space.current_frame().spider).z = (u_short)temp_double;
                    space.TUI_clue = "+ Z";
                    break;
                //------------------------------------------------------------
                case 'Z':
                    temp_double = space.current_frame().at(space.current_frame().spider).z - space.displacement_step;
                    if(temp_double >= LASERBOY_MIN_SHORT)
                        space.current_frame().at(space.current_frame().spider).z = (u_short)temp_double;
                    space.TUI_clue = "- Z";
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(space.current_frame().spider) // is not the origin
                    {
                       temp_3D_float
                        = rotate_vertex_on_coordinates_z(space.current_frame().at(space.current_frame().spider),
                                                         space.current_frame().at(space.current_frame().spider - 1),
                                                         space.rotation_step
                                                        );
                        if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                            space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "+ rotate on Z axis";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case 'F':
                    if(space.current_frame().spider) // is not the origin
                    {

                        temp_3D_float
                        = rotate_vertex_on_coordinates_z(space.current_frame().at(space.current_frame().spider),
                                                         space.current_frame().at(space.current_frame().spider - 1),
                                                         -space.rotation_step
                                                        );
                        if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                            space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "- rotate on Z axis";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(space.current_frame().spider) // is not the origin
                    {
                        temp_3D_float
                        = rotate_vertex_on_coordinates_y(space.current_frame().at(space.current_frame().spider),
                                                         space.current_frame().at(space.current_frame().spider - 1),
                                                         space.rotation_step
                                                        );
                        if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                            space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "+ rotate on Y axis";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case 'G':
                    if(space.current_frame().spider) // is not the origin
                    {
                        temp_3D_float
                        = rotate_vertex_on_coordinates_y(space.current_frame().at(space.current_frame().spider),
                                                         space.current_frame().at(space.current_frame().spider - 1),
                                                         -space.rotation_step
                                                        );
                        if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                            space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "- rotate on Y axis";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(space.current_frame().spider) // is not the origin
                    {
                    temp_3D_float
                    = scale_vertex_on_coordinates(space.current_frame().at(space.current_frame().spider),
                                                  space.current_frame().at(space.current_frame().spider - 1),
                                                  LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)
                                                 );
                    if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                        space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "+ magnitude of vector";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(space.current_frame().spider) // is not the origin
                    {
                        temp_3D_float
                        = scale_vertex_on_coordinates(space.current_frame().at(space.current_frame().spider),
                                                      space.current_frame().at(space.current_frame().spider - 1),
                                                      LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)
                                                     );
                        if(!LaserBoy_bounds_check(temp_3D_float, LASERBOY_CUBE))
                            space.current_frame().at(space.current_frame().spider) = temp_3D_float;
                        space.TUI_clue = "- magnitude of vector";
                    }
                    else
                        space.TUI_clue = "not at zero vertex";
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            } // end switch(event.key.keysym.unicode)
        } // end else if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::move_scale_rotate_menu()
{
    bool    in_menu              = true,
            show_vertices_was    = space.show_vertices,
            show_blanking_was    = space.show_blanking,
            show_intro_was       = space.show_intro,
            show_bridge_was      = space.show_bridge,
            show_coda_was        = space.show_coda,
            show_menu_was        = space.show_menu;
    char    formatted_string[80] = {0};
    u_short offset;
    double  temp_double;
    //------------------------------------------------------------------------
    space.show_menu         = true;
    space.show_vertices     = true;
    space.show_blanking     = true;
    space.show_intro        = false;
    space.show_bridge       = false;
    space.show_coda         = false;
    space.show_cursors      = true;
    space.show_fulcrum      = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_move_scale_rotate_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu             = false;
                    space.show_vertices = show_vertices_was;
                    space.show_blanking = show_blanking_was;
                    space.show_intro    = show_intro_was;
                    space.show_bridge   = show_bridge_was;
                    space.show_coda     = show_coda_was;
                    space.show_cursors  = false;
                    space.show_fulcrum  = false;
                    space.TUI_clue = "exit move scale rotate";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case '1':
                    space.view_angle.x += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    space.TUI_clue = "+ orbit X";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.view_angle.x -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    space.TUI_clue = "- orbit X";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.view_angle.y += space.rotation_step;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    space.TUI_clue = "+ orbit Y";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.view_angle.y -= space.rotation_step;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    space.TUI_clue = "- orbit Y";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.view_angle.z += space.rotation_step;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit Z";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.view_angle.z -= space.rotation_step;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit Z";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.view_angle += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.view_angle -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.view_angle   = 0;
                    space.TUI_clue = "front";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.view_angle.x = 0;
                    space.view_angle.y = pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "back";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.view_angle.x = 0;
                    space.view_angle.y = half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "left side";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.view_angle.x = 0;
                    space.view_angle.y = -half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "right side";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.view_angle.x = half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "top";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.view_angle.x = -half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "bottom";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.view_angle += space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index < space.number_of_frames() - 1)
                      )
                        space.pre_incr_current_frame();
                    space.TUI_clue = "+ show & orbit";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.view_angle -= space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index > 0)
                      )
                        space.pre_decr_current_frame();
                    space.TUI_clue = "- show & orbit";
                    break;
                //------------------------------------------------------------
                case '[':
                    space.current_frame().increment_egg();
                    space.TUI_clue = "+ start vertex  select";
                    break;
                //------------------------------------------------------------
                case '{':
                    space.current_frame().deccrement_egg();
                    space.TUI_clue = "- start vertex select";
                    break;
                //------------------------------------------------------------
                case ']':
                    space.current_frame().increment_spider();
                    space.TUI_clue = "+ end vertex  select";
                    break;
                //------------------------------------------------------------
                case '}':
                    space.current_frame().deccrement_spider();
                    space.TUI_clue = "- end vertex  select";
                    break;
                //------------------------------------------------------------
                case'\\':
                    space.current_frame().spider = (space.current_frame().size() - 1);
                    space.TUI_clue = "last vertex select";
                    break;
                //------------------------------------------------------------
                case '|':
                    space.current_frame().egg = 0;
                    space.TUI_clue = "first vertex select";
                    break;
                //------------------------------------------------------------
                case 'u':
                    space.current_frame().next_segment_select();
                    space.TUI_clue = "+ lit segment";
                    break;
                //------------------------------------------------------------
                case 'U':
                    space.current_frame().previous_segment_select();
                    space.TUI_clue = "- lit segment";
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.current_frame().next_segment_egg();
                    space.TUI_clue = "+ start lit segment";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.current_frame().previous_segment_egg();
                    space.TUI_clue = "- start lit segment";
                    break;
                //------------------------------------------------------------
                case 'o':
                    space.current_frame().next_segment_spider();
                    space.TUI_clue = "+ end lit segment";
                    break;
                //------------------------------------------------------------
                case 'O':
                    space.current_frame().previous_segment_spider();
                    space.TUI_clue = "- end lit segment";
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case '<':
                    space.current_frame().reverse_selected();
                    space.TUI_clue = "reversed selection";
                    break;
                //------------------------------------------------------------
                case 'c': //
                    if(space.current_frame().append_selected_segment())
                    {
                        display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "copy segment fail";
                    }
                    else
                    {
                        sprintf(formatted_string, "copied %d vertices to end of frame", (int)space.current_frame().selected_segment().size());
                        space.TUI_clue = formatted_string;
                    }
                    break;
                //------------------------------------------------------------
                case 'q':
                    offset = display_prompt_and_echo_int("set center X");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(offset - space.current_frame().selected_segment().mean_of_coordinates().x,
                                                                                0,
                                                                                0
                                                                               )
                                                            )
                      )
                        space.TUI_clue = "set center X";
                    else
                    {
                        display_error("X out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'w':
                    offset = display_prompt_and_echo_int("set center Y");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0,
                                                                            offset - space.current_frame().selected_segment().mean_of_coordinates().y,
                                                                            0
                                                                           )
                                                            )
                      )
                        space.TUI_clue = "set center Y";
                    else
                    {
                        display_error("Y out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'e':
                    offset = display_prompt_and_echo_int("set center Z");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0,
                                                                                0,
                                                                                offset - space.current_frame().selected_segment().mean_of_coordinates().z
                                                                               )
                                                            )
                      )
                        space.TUI_clue = "set center Z";
                    else
                    {
                        display_error("Z out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'x':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(space.displacement_step, 0, 0)))
                        space.TUI_clue = "+ X move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'X':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(-space.displacement_step, 0, 0)))
                        space.TUI_clue = "- X move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'y':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, space.displacement_step, 0)))
                        space.TUI_clue = "+ Y move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'Y':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, -space.displacement_step, 0)))
                        space.TUI_clue = "- Y move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'z':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, 0, space.displacement_step)))
                        space.TUI_clue = "+ Z move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'Z':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, 0,-space.displacement_step)))
                        space.TUI_clue = "- Z move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, 0, space.rotation_step)))
                        space.TUI_clue = "+ rotate Z";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'F':
                    if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, 0, -space.rotation_step)))
                        space.TUI_clue = "- rotate Z";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, space.rotation_step, 0)))
                        space.TUI_clue = "+ rotate y";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'G':
                    if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0,-space.rotation_step, 0)))
                        space.TUI_clue = "- rotate y";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'r':
                    if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, 0, space.rotation_step)))
                        space.TUI_clue = "+ rotate Z on fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'R':
                    if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, 0, -space.rotation_step)))
                        space.TUI_clue = "- rotate Z on fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 't':
                    if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, space.rotation_step, 0)))
                        space.TUI_clue = "+ rotate Y on fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'T':
                    if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, -space.rotation_step, 0)))
                        space.TUI_clue = "- rotate Y on fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '9':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_up, 1, 1)))
                        space.TUI_clue = "+ scale X";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '(':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_dn, 1, 1)))
                        space.TUI_clue = "- scale X";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '0':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, space.magnitude_step_up, 1)))
                        space.TUI_clue = "+ scale Y";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case ')':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, space.magnitude_step_dn, 1)))
                        space.TUI_clue = "- scale Y";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '-':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, 1, space.magnitude_step_up)))
                        space.TUI_clue = "+ scale Z";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '_':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, 1, space.magnitude_step_dn)))
                        space.TUI_clue = "- scale Z";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '=':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)))
                        space.TUI_clue = "+ scale XYZ";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case '+':
                    if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)))
                        space.TUI_clue = "- scale XYZ";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'v':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_up, 1, 1)))
                        space.TUI_clue = "+ scale X by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'V':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_dn, 1, 1)))
                        space.TUI_clue = "- scale X by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'b':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, space.magnitude_step_up, 1)))
                        space.TUI_clue = "+ scale Y by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'B':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, space.magnitude_step_dn, 1)))
                        space.TUI_clue = "- scale Y by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'n':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, 1, space.magnitude_step_up)))
                        space.TUI_clue = "+ size Z by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'N':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, 1, space.magnitude_step_dn)))
                        space.TUI_clue = "- scale Z by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)))
                        space.TUI_clue = "+ scale XYZ by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)))
                        space.TUI_clue = "- scale XYZ by fulcrum";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.fulcrum.x += space.displacement_step;
                    space.TUI_clue = "+ X fulcrum";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.fulcrum.x -= space.displacement_step;
                    space.TUI_clue = "- X fulcrum";
                    break;
                //------------------------------------------------------------
                case 's':
                    space.fulcrum.y += space.displacement_step;
                    space.TUI_clue = "+ Y fulcrum";
                    break;
                //------------------------------------------------------------
                case 'S':
                    space.fulcrum.y -= space.displacement_step;
                    space.TUI_clue = "- Y fulcrum";
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.fulcrum.z += space.displacement_step;
                    space.TUI_clue = "+ Z fulcrum";
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.fulcrum.z -= space.displacement_step;
                    space.TUI_clue = "- Z fulcrum";
                    break;
                //------------------------------------------------------------
                case 'j':
                    temp_double = display_prompt_and_echo_double("set fulcrum X");
                    if(prompt_escaped)
                        break;
                    space.fulcrum.x = temp_double;
                    space.TUI_clue = "vertex X set";
                    break;
                //------------------------------------------------------------
                case 'k':
                    temp_double = display_prompt_and_echo_double("set fulcrum Y");
                    if(prompt_escaped)
                        break;
                    space.fulcrum.y = temp_double;
                    space.TUI_clue = "vertex Y set";
                    break;
                //------------------------------------------------------------
                case 'l':
                    temp_double = display_prompt_and_echo_double("set fulcrum Z");
                    if(prompt_escaped)
                        break;
                    space.fulcrum.z = temp_double;
                    space.TUI_clue = "vertex Z set";
                    break;
                //------------------------------------------------------------
                case ',':
                    space.fulcrum = space.current_frame().at(space.current_frame().egg);
                    space.TUI_clue = "fulcrum to egg";
                    break;
                //------------------------------------------------------------
                case '.':
                    space.fulcrum = space.current_frame().at(space.current_frame().spider);
                    space.TUI_clue = "fulcrum to spider";
                    break;
                //------------------------------------------------------------
                case '\'':
                    {
                        int index = display_prompt_and_echo_int("place egg at index");
                        if(prompt_escaped)
                            break;
                        space.current_frame().egg = index;
                        space.current_frame().normalize_cursors();
                    }
                    break;
                //------------------------------------------------------------
                case '"':
                    {
                        int index = display_prompt_and_echo_int("place spider at index");
                        if(prompt_escaped)
                            break;
                        space.current_frame().spider = index;
                        space.current_frame().normalize_cursors();
                    }
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            } // end switch(event.key.keysym.unicode)
        } // end else if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::direct_draw_segments_menu()
{
    bool    in_menu           = true,
            show_palette_was  = space.show_palette,
            show_menu_was     = space.show_menu;

    space.show_menu           = true;
    space.show_palette        = true;
    space.show_color_cursor   = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_direct_draw_segments_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    in_menu                 = false;
                    space.show_color_cursor = false;
                    space.show_palette      = show_palette_was;
                    space.TUI_clue = "exit render segments";
                    break;
                //------------------------------------------------------------
                case SDLK_BACKSPACE:
                case SDLK_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_common_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'x':
                case 'X':
                    space.set_current_menu_name("switch settings", 0x00003030, true);
                    switch_settings_menu();
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case '1':
                    space.view_angle.x += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    space.TUI_clue = "+ orbit X";
                    break;
                //------------------------------------------------------------
                case '!':
                    space.view_angle.x -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    space.TUI_clue = "- orbit X";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.view_angle.y += space.rotation_step;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    space.TUI_clue = "+ orbit Y";
                    break;
                //------------------------------------------------------------
                case '@':
                    space.view_angle.y -= space.rotation_step;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    space.TUI_clue = "- orbit Y";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.view_angle.z += space.rotation_step;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit Z";
                    break;
                //------------------------------------------------------------
                case '#':
                    space.view_angle.z -= space.rotation_step;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit Z";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.view_angle += space.rotation_step;
                    if(space.view_angle.x > two_pi)
                        space.view_angle.x -= two_pi;
                    if(space.view_angle.y > two_pi)
                        space.view_angle.y -= two_pi;
                    if(space.view_angle.z > two_pi)
                        space.view_angle.z -= two_pi;
                    space.TUI_clue = "+ orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '$':
                    space.view_angle -= space.rotation_step;
                    if(space.view_angle.x < -two_pi)
                        space.view_angle.x += two_pi;
                    if(space.view_angle.y < -two_pi)
                        space.view_angle.y += two_pi;
                    if(space.view_angle.z < -two_pi)
                        space.view_angle.z += two_pi;
                    space.TUI_clue = "- orbit XYZ";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.view_angle   = 0;
                    space.TUI_clue = "front";
                    break;
                //------------------------------------------------------------
                case '%':
                    space.view_angle.x = 0;
                    space.view_angle.y = pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "back";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.view_angle.x = 0;
                    space.view_angle.y = half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "left side";
                    break;
                //------------------------------------------------------------
                case '^':
                    space.view_angle.x = 0;
                    space.view_angle.y = -half_pi;
                    space.view_angle.z = 0;
                    space.TUI_clue = "right side";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.view_angle.x = half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "top";
                    break;
                //------------------------------------------------------------
                case '&':
                    space.view_angle.x = -half_pi;
                    space.view_angle.y = 0;
                    space.view_angle.z = 0;
                    space.TUI_clue = "bottom";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.view_angle += space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index < space.number_of_frames() - 1)
                      )
                        space.pre_incr_current_frame();
                    space.TUI_clue = "+ show & orbit";
                    break;
                //------------------------------------------------------------
                case '*':
                    space.view_angle -= space.rotation_step;
                    if(    space.loop_animation
                        || (space.frame_index > 0)
                      )
                        space.pre_decr_current_frame();
                    space.TUI_clue = "- show & orbit";
                    break;
                //------------------------------------------------------------
                case 'a':
                    draw_point_menu();
                    break;
                //------------------------------------------------------------
                case 'b':
                    draw_line_menu();
                    break;
                //------------------------------------------------------------
                case 'c':
                    draw_rectangle_menu();
                    break;
                //------------------------------------------------------------
                case 'd':
                    draw_polygon_menu();
                    break;
                //------------------------------------------------------------
                case 'e':
                    draw_polyline_menu();
                    break;
                //------------------------------------------------------------
                case 'f':
                    draw_polystar_menu();
                    break;
                //------------------------------------------------------------
                case 'g':
                    draw_circular_arc_menu();
                    break;
                //------------------------------------------------------------
                case 'h':
                    draw_elliptical_arc_menu();
                    break;
                //------------------------------------------------------------
                case 'i':
                    draw_rhodonea_menu();
                    break;
                //------------------------------------------------------------
                case 'j':
                    draw_epicycloid_menu();
                    break;
                //------------------------------------------------------------
                case 'k':
                    draw_epitrochoid_menu();
                    break;
                //------------------------------------------------------------
                case 'l':
                    draw_hypocycloid_menu();
                    break;
                //------------------------------------------------------------
                case 'm':
                    draw_hypotrochoid_menu();
                    break;
                //------------------------------------------------------------
                case 'n':
                    draw_lissajous_menu();
                    break;
                //------------------------------------------------------------
                case 'o':
                    draw_mono_spaced_font_menu();
                    break;
                //------------------------------------------------------------
                case 'q':
                    draw_variable_spaced_font_menu();
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.selected_color_index >= space.current_palette().number_of_colors())
                        space.selected_color_index = 0;
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.selected_color_index == 0)
                        space.selected_color_index = space.current_palette().number_of_colors();
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case '/': // show frame set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case SDLK_RETURN:
                    space.show_menu = !space.show_menu;
            } // end switch(event.key.keysym.unicode)
        } // end else if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_point_menu()
{
    LaserBoy_vertex _0;

    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_int("X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_int("  Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.z = display_prompt_and_echo_int("    Z [0]");
    if(prompt_escaped)
        return;

    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _0.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _0.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _0.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _0.c = space.selected_color_index;
        _0.r = space.palette_picker(space.palette_index)[_0.c].r;
        _0.g = space.palette_picker(space.palette_index)[_0.c].g;
        _0.b = space.palette_picker(space.palette_index)[_0.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    space.current_frame() += _0;
    _0.unblank();
    space.current_frame() += _0;

    space.TUI_clue = "point";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_line_menu()
{
    LaserBoy_vertex _0,
                    _1;

    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_int("start X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_int("start   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.z = display_prompt_and_echo_int("start     Z [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_int("end X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_int("end   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.z = display_prompt_and_echo_int("end     Z [0]");
    if(prompt_escaped)
        return;

    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _1.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _1.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _1.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _1.c = space.selected_color_index;
        _1.r = space.palette_picker(space.palette_index)[_1.c].r;
        _1.g = space.palette_picker(space.palette_index)[_1.c].g;
        _1.b = space.palette_picker(space.palette_index)[_1.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    _1.unblank();
    space.current_frame() += _0;
    space.current_frame() += _1;

    space.TUI_clue = "line";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_rectangle_menu()
{
    LaserBoy_vertex _0,
                    _1,
                    _2;

    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_int("first corner X [-30000]", -30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_int("first corner   Y [10000]", 10000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_int("second corner X [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_int("second corner   Y [-10000]", -10000);
    if(prompt_escaped)
        return;

    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _2.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _2.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _2.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _2.c = space.selected_color_index;
        _2.r = space.palette_picker(space.palette_index)[_2.c].r;
        _2.g = space.palette_picker(space.palette_index)[_2.c].g;
        _2.b = space.palette_picker(space.palette_index)[_2.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    _2.unblank();
    space.current_frame() += _0;
    _2   = _0.as_3D_short();
    _2.x = _1.x;
    _2.z = _1.z;
    space.current_frame() += _2;
    _2   = _1.as_3D_short();
    space.current_frame() += _2;
    _2.x = _0.x;
    _2.z = _0.z;
    space.current_frame() += _2;
    _2   = _0.as_3D_short();
    space.current_frame() += _2;

    space.TUI_clue = "rectangle";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_polygon_menu()
{
    LaserBoy_3D_short center,
                      vertex;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    vertex.x = display_prompt_and_echo_int("first vertex X [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    vertex.y = display_prompt_and_echo_int("first vertex   Y [0]");
    if(prompt_escaped)
        return;
        
    u_int number_of_sides = (u_int)display_prompt_and_echo_int("number of sides [8]", 8);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center,
                                              vertex,
                                              number_of_sides
                                             );

    space.TUI_clue = "polygon";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_polyline_menu()
{
    int             number_of_vertecies = 0;
    char            text_number[16];
    LaserBoy_vertex _0,
                    _1;
    _1.unblank();
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _1.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _1.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _1.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _1.c = space.selected_color_index;
        _1.r = space.palette_picker(space.palette_index)[_1.c].r;
        _1.g = space.palette_picker(space.palette_index)[_1.c].g;
        _1.b = space.palette_picker(space.palette_index)[_1.c].b;
    }
    while(true)
    {
        sprintf(text_number, "%d", number_of_vertecies);
        space.clean_screen();
        space.render_space();
        _1.x = display_prompt_and_echo_int(string("vertex ") + text_number + " X [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;

        space.clean_screen();
        space.render_space();
        _1.y = display_prompt_and_echo_int(string("vertex ") + text_number + "   Y [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;

        space.clean_screen();
        space.render_space();
        _1.z = display_prompt_and_echo_int(string("vertex ") + text_number + "     Z [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;

        space.current_frame() += _1;
        if(number_of_vertecies == 0)
        {
            _0 = _1;
            space.current_frame().back().blank();
        }
        number_of_vertecies++;
    }
    space.clean_screen();
    space.render_space();
    bool close_polyline = display_prompt_and_echo_bool("close polyline ? (y | [any])");
    if(prompt_escaped)
        return;
    if(close_polyline)
        space.current_frame() += _0;

    space.TUI_clue = "polyline";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_polystar_menu()
{
    LaserBoy_3D_short center,
                      vertex;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    vertex.x = display_prompt_and_echo_int("first vertex X [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    vertex.y = display_prompt_and_echo_int("first vertex   Y [0]");
    if(prompt_escaped)
        return;


    space.clean_screen();
    space.render_space();
    u_int number_of_points = (u_int)display_prompt_and_echo_int("number of points [9]", 9);
    if(prompt_escaped)
        return;

    double ratio = display_prompt_and_echo_double("inner / outer ratio [0.5]", 0.5);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center,
                                              vertex,
                                              number_of_points,
                                              ratio
                                             );

    space.TUI_clue = "polystar";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_circular_arc_menu()
{
    LaserBoy_3D_short center,
                      radius;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    radius.x = display_prompt_and_echo_int("radius vertex X [32767]", 32767);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    radius.y = display_prompt_and_echo_int("radius vertex  Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    double  arc_angle = display_prompt_and_echo_double("arc angle in degrees, (0 = 360) [0]");
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space, center, radius, arc_angle);

    space.TUI_clue = "circular arc";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_elliptical_arc_menu()
{
    LaserBoy_3D_short center,
                      radius;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    radius.x = display_prompt_and_echo_int("radius vertex X [32767]", 32767);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    radius.y = display_prompt_and_echo_int("radius vertex  Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    double  arc_angle = display_prompt_and_echo_double("arc angle in degrees, (0 = 360) [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    double radii_ratio = display_prompt_and_echo_double("minor / major radii ratio [0.5]", 0.5);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space, center, radius, arc_angle, radii_ratio);

    space.TUI_clue = "elliptical arc";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_rhodonea_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int radius = display_prompt_and_echo_int("radius [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int pedals_numerator = display_prompt_and_echo_int("pedal factor numerator [11]", 11);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int pedals_denominator = display_prompt_and_echo_int("pedal factor denominator [17]", 17);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center,
                                              radius,
                                              pedals_numerator,
                                              pedals_denominator
                                             );

    space.TUI_clue = "rhodonea";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_epicycloid_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int center_radius = display_prompt_and_echo_int("fixed circle radius [3000]", 3200);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_radius = display_prompt_and_echo_int("rolling circle radius [10000]", 10000);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center_radius,
                                              center,
                                              roller_radius,
                                              roller_radius
                                             );

    space.TUI_clue = "epicycloid";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_epitrochoid_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int center_radius = display_prompt_and_echo_int("fixed circle radius [3100]", 3200);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_radius = display_prompt_and_echo_int("rolling circle radius [10000]", 10000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_offset = display_prompt_and_echo_int("distance from rolling center [9000]", 9000);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center_radius,
                                              center,
                                              roller_radius,
                                              roller_offset
                                             );

    space.TUI_clue = "epitrochoid";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_hypocycloid_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int center_radius = display_prompt_and_echo_int("fixed circle radius [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_radius = display_prompt_and_echo_int("rolling circle radius [13500]", 13500);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center_radius,
                                              roller_radius,
                                              roller_radius,
                                              center
                                             );

    space.TUI_clue = "hypocycloid";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_hypotrochoid_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int center_radius = display_prompt_and_echo_int("fixed circle radius [15000]", 15000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_radius = display_prompt_and_echo_int("rolling circle radius [8000]", 8000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int roller_offset = display_prompt_and_echo_int("distance from rolling center [11000]", 11000);
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center_radius,
                                              roller_radius,
                                              roller_offset,
                                              center
                                             );

    space.TUI_clue = "hypotrochoid";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_lissajous_menu()
{
    LaserBoy_3D_short center;

    space.clean_screen();
    space.render_space();
    center.x = display_prompt_and_echo_int("center X [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_int("center   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    center.z = display_prompt_and_echo_int("center     Z [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int amplitude_x = display_prompt_and_echo_int("amplitude in X [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int amplitude_y = display_prompt_and_echo_int("amplitude in Y [30000]", 30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int frequency_x = display_prompt_and_echo_int("frequency in X [4]", 4);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    int frequency_y = display_prompt_and_echo_int("frequency in Y [5]", 5);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    double phase_x = display_prompt_and_echo_int("phase shift in X [0] degrees");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    double phase_y = display_prompt_and_echo_int("phase shift in Y [0] degrees");
    if(prompt_escaped)
        return;

    space.current_frame() += LaserBoy_segment(&space,
                                              center,
                                              amplitude_x,
                                              amplitude_y,
                                              frequency_x,
                                              frequency_y,
                                              phase_x,
                                              phase_y
                                             );

    space.TUI_clue = "Lissajous curv";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_harmonograph_menu()
{
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_mono_spaced_font_menu()
{
    char buffer[30];
    string prompt;
    LaserBoy_3D_short _0,
                      _1;

    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_int("baseline start X [-30000]", -30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_int("baseline start   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_int("baseline end X [30000]", 30000);
    if(prompt_escaped)
        return;

    sprintf(buffer, "%d", _0.y);
    prompt = "baseline end   Y [";
    prompt.append(buffer);
    prompt.append("]");
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_int(prompt.c_str(), _0.y);
    if(prompt_escaped)
        return;

    space.get_directory_list(LASERBOY_ILD_SHARE, ".ild");
    space.clean_screen();
    space.overlay_file_list("font_");
    string font = display_prompt_file_with_auto_complete("font name [font_wide_vector.ild]", string("font_"));
    if(prompt_escaped)
        return;

    if(font.size() == 5)
        font = "font_wide_vector.ild";
    else if(font.size() < 10 || font.substr(font.size() - 4) != ".ild")
        font += ".ild";
    //------------------------------------------------------------------------
    space.clean_screen();
    space.render_space();
    string text = display_prompt_and_echo_string("text to render");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    space.current_frame() += LaserBoy_segment(&space,
                                              _0,
                                              _1,
                                              LASERBOY_ILD_SHARE + font,
                                              text
                                             );
    //------------------------------------------------------------------------
    space.TUI_clue = "text rendered";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::draw_variable_spaced_font_menu()
{
    char buffer[30];
    string prompt;
    LaserBoy_3D_short _0,
                      _1;

    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_int("baseline start X [-30000]", -30000);
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_int("baseline start   Y [0]");
    if(prompt_escaped)
        return;

    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_int("baseline end X [30000]", 30000);
    if(prompt_escaped)
        return;

    sprintf(buffer, "%d", _0.y);
    prompt = "baseline end   Y [";
    prompt.append(buffer);
    prompt.append("]");
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_int(prompt.c_str(), _0.y);
    if(prompt_escaped)
        return;

    space.get_directory_list(LASERBOY_ILD_SHARE, ".ild");
    space.clean_screen();
    space.overlay_file_list("font_");
    string font = display_prompt_file_with_auto_complete("font name [font_arial.ild]", string("font_"));
    if(prompt_escaped)
        return;

    if(font.size() == 5)
        font = "font_arial.ild";
    else if(font.size() < 10 || font.substr(font.size() - 4) != ".ild")
        font += ".ild";
    //------------------------------------------------------------------------
    space.clean_screen();
    space.render_space();
    string text = display_prompt_and_echo_string("text to render");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    space.current_frame() += LaserBoy_segment(&space,
                                              _0,
                                              _1,
                                              LASERBOY_ILD_SHARE + font,
                                              text,
                                              0
                                             );
    //------------------------------------------------------------------------
    space.TUI_clue = "text rendered";
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::wave_utilities_menu()
{
    bool in_menu   = true,
         good_call = false;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.set_current_menu_name("wave utilities", 0x00002040);
        space.clean_screen();
        space.overlay_wave_utilities_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit wave utilities";
                    break;
                //------------------------------------------------------------
                case '1': // wave utilities settings
                    space.set_current_menu_name("wave show settings", 0x00002040, true);
                    wave_show_settings_menu();
                    break;
                //------------------------------------------------------------
                case '2': // show wave on screen
                    space.set_current_menu_name("wave show options", 0x00002040, true);
                    show_wave_options_menu();
                    break;
                //------------------------------------------------------------
                case '3': // set LaserBoy wave offsets
                    space.set_current_menu_name("set system offsets", 0x00002040, true);
                    set_wave_offsets_menu();
                    break;
                //------------------------------------------------------------
                case '4': // apply LaserBoy wave sample offsets
                    space.set_current_menu_name("apply system offsets", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to apply offsets");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    display_space();
                    if(space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                        if(good_call)
                            display_space();
                        else
                            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '5': // overwrite LaserBoy wave sample offsets
                    space.set_current_menu_name("overwrite system offsets", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to overwrite offsets");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    display_space();
                    if(space.overwirte_wave_offsets(LASERBOY_WAV_SHARE + file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                        if(good_call)
                            display_space();
                        else
                            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '6': // set color rescale files
                    space.set_current_menu_name("set color rescales", 0x00002040, true);
                    set_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '7': // apply color rescales to wave
                    space.set_current_menu_name("apply color rescales", 0x00002040, true);
                    apply_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '8': // omit color rescal tables in header
                    space.set_current_menu_name("omit color rescales", 0x00002040, true);
                    omit_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '9': // save color rescal tables
                    space.set_current_menu_name("saving rescales", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to save color rescales");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    display_space();
                    if(space.save_color_rescales(LASERBOY_WAV_SHARE     + file_name,
                                                 LASERBOY_RESCALE_SHARE + file_name.substr(0, file_name.size() - 4))
                                                )
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                        if(good_call)
                            display_space();
                        else
                            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '0': // impose byte value colors and on wave
                    space.set_current_menu_name("black level to wave", 0x00002040, true);
                    black_level_to_wave_menu();
                    break;
                //------------------------------------------------------------
                case 'a': // impose bit resolution reduction on wave
                    space.set_current_menu_name("bit resolution to wave", 0x00002040, true);
                    bit_resolution_to_wave_menu();
                    break;
                //------------------------------------------------------------
                case 'b': // split or join waves menu
                    space.set_current_menu_name("split or join waves", 0x00002040, true);
                    split_or_join_waves_menu();
                    break;
                //------------------------------------------------------------
                case 'c': // invert LaserBoy signal polarities
                    space.set_current_menu_name("set signal polarities", 0x00002040, true);
                    set_signal_polarities_menu();
                    break;
                //------------------------------------------------------------
                case 'd': // clear LaserBoy wave polarity matrix
                    space.set_current_menu_name("clear polarities list", 0x00002040, true);
                    clear_wave_polarities_menu();
                    break;
                //------------------------------------------------------------
                case 'e': // invert LaserBoy wave polarity
                    space.set_current_menu_name("invert wave", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to globally invert");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    display_space();
                    if(space.invert_wave_prep(LASERBOY_WAV_SHARE + file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                        if(good_call)
                            display_space();
                        else
                            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'E': // invert unformatted wave polarity
                    space.set_current_menu_name("invert wave", 0x00002040, true);
                    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to globally invert");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    display_space();
                    if(space.invert_wave_prep(LASERBOY_WAV_SHARE + file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                        if(good_call)
                            display_space();
                        else
                            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'f': // Show LaserBoy wave stats
                    space.set_current_menu_name("show stats", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("LaserBoy wave file name to see stats");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
                    if(good_call)
                        display_space();
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'F': // Show unformatted wave stats
                    space.set_current_menu_name("show stats", 0x00002040, true);
                    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("unformatted wave file name to see stats");
                    if(prompt_escaped)
                        break;
                    space.clean_screen();
                    good_call = space.overlay_wave_header_stats(LASERBOY_UNFORMATTED_SHARE + file_name);
                    if(good_call)
                        display_space();
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'g': // analyze LaserBoy wave
                    space.set_current_menu_name("analyse wave", 0x00002040, true);
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("LaserBoy wave file name to analyze");
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "wave analysis";
                    space.clean_screen();
                    good_call = space.overlay_wave_analysis(LASERBOY_WAV_SHARE + file_name);
                    if(good_call)
                        display_space();
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'G': // analyze unfortmatted wave
                    space.set_current_menu_name("analyse wave", 0x00002040, true);
                    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("unformatted wave file name to analyze");
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "wave analysis";
                    space.clean_screen();
                    good_call = space.overlay_wave_analysis(LASERBOY_UNFORMATTED_SHARE + file_name);
                    if(good_call)
                        display_space();
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::split_or_join_waves_menu()
{
    bool   in_menu = true;
    space.TUI_clue = "split or join waves";
    while(in_menu)
    {
        space.clean_screen();
        space.overlay_show_split_or_join_waves_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit split or join waves";
                    break;
                //------------------------------------------------------------
                case '1': // split wave
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to split");
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "splitting wave";
                    display_space();
                    if(space.split_wave_XY_rg_bi_LR_prep(file_name,
                                                         LASERBOY_WAV_SHARE,
                                                         LASERBOY_UNFORMATTED_SHARE,
                                                         LASERBOY_AUDIO_SHARE
                                                        )
                      )
                    {
                        space.TUI_clue = "split wave";
                        display_space();
                    }
                    else
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case '2': // split wave
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to split");
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "splitting wave";
                    display_space();
                    if(space.split_wave_XY_r_g_b_i_LR_prep(file_name,
                                                           LASERBOY_WAV_SHARE,
                                                           LASERBOY_UNFORMATTED_SHARE,
                                                           LASERBOY_AUDIO_SHARE
                                                          )
                      )
                    {
                        space.TUI_clue = "split wave";
                        display_space();
                    }
                    else
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case '3': // add audio to wave
                    add_audio_to_wave_menu();
                    break;
                //------------------------------------------------------------


                //------------------------------------------------------------
                case '4': // join waves
                    join_XY_r_g_b_i_waves_menu();
                    break;
                //------------------------------------------------------------
                case '5': // join waves
                    join_XY_rg_b_waves_menu();
                    break;
                //------------------------------------------------------------
                case '6': // join waves
                    join_XY_rg_b_LR_waves_menu();
                    break;
                //------------------------------------------------------------


                //------------------------------------------------------------
                case '7': // join waves
                    break;
                //------------------------------------------------------------
                case '8': // join waves
                    break;
                //------------------------------------------------------------
                case '9': // join waves
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::wave_show_settings_menu()
{
    bool   in_menu = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.set_current_menu_name("wave show settings", 0x00002040);

        space.clean_screen();
        space.overlay_wave_show_settings_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case SDLK_ESCAPE:
                    space.TUI_clue = "exit wave show settings";
                    in_menu = false;
                    break;
                //------------------------------------------------------------
                case '1':
                    space.wave_show_vertices = !space.wave_show_vertices;
                    if(space.wave_show_vertices)
                        space.TUI_clue = "show vertices";
                    else
                        space.TUI_clue = "hide vertices";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.wave_show_blanks = !space.wave_show_blanks;
                    if(space.wave_show_blanks)
                        space.TUI_clue = "show blanking";
                    else
                        space.TUI_clue = "hide blanking";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.wave_real_time = !space.wave_real_time;
                    if(space.wave_real_time)
                        space.TUI_clue = "real time";
                    else
                        space.TUI_clue = "as fast as possible";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.wave_show_inverted = !space.wave_show_inverted;
                    if(space.wave_show_inverted)
                        space.TUI_clue = "inverted";
                    else
                        space.TUI_clue = "non-inverted";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.wave_clean_screen = !space.wave_clean_screen;
                    if(space.wave_clean_screen)
                        space.TUI_clue = "clean screen";
                    else
                        space.TUI_clue = "no clean screen";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.wave_loop = !space.wave_loop;
                    if(space.wave_loop)
                        space.TUI_clue = "loop";
                    else
                        space.TUI_clue = "no loop";
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::show_wave_options_menu()
{
    while(true)
    {
        space.set_current_menu_name("wave show options", 0x00002040, true);
        space.clean_screen();
        space.overlay_show_wave_options_menu();
        display_space();
        //--------------------------------------------------------------------
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case SDLK_RSHIFT:
                case SDLK_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case SDLK_ESCAPE:
                    space.TUI_clue = "exit wave show options";
                    return;
                //------------------------------------------------------------
                case '1': // show LaserBoy wave
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to show");
                    if(prompt_escaped)
                        break;
                    file_name = LASERBOY_WAV_SHARE + file_name;
                    if(display_wave())
                    {
                        while(space.wave_loop)
                        {
                            display_wave();
                            SDL_PollEvent(&event);
                            if(event.type == SDL_KEYDOWN)
                                break;
                        }
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    break;
                //------------------------------------------------------------
                case '2': // show LaserBoy wave as unformatted
                    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to show");
                    if(prompt_escaped)
                        break;
                    file_name = LASERBOY_WAV_SHARE + file_name;
                    if(display_unframed_wave())
                    {
                        while(space.wave_loop)
                        {
                            display_unframed_wave();
                            SDL_PollEvent(&event);
                            if(event.type == SDL_KEYDOWN)
                                break;
                        }
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    break;
                //------------------------------------------------------------
                case '3': // show unformatted wave
                    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.overlay_file_list();
                    file_name = display_prompt_file_with_auto_complete("wave file name to show");
                    if(prompt_escaped)
                        break;
                    file_name = LASERBOY_UNFORMATTED_SHARE + file_name;
                    if(display_unframed_wave())
                    {
                        while(space.wave_loop)
                        {
                            display_unframed_wave();
                            SDL_PollEvent(&event);
                            if(event.type == SDL_KEYDOWN)
                                break;
                        }
                    }
                    else
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
            wait_4_Esc();
        } // end if(event.type == SDL_KEYDOWN)
    } // end while(true)
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::set_wave_offsets_menu()
{
    int temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for both X and Y");
    if(prompt_escaped)
        return;
    space.wave_offsets[0] = temp_int;
    space.wave_offsets[1] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for red");
    if(prompt_escaped)
        return;
    space.wave_offsets[2] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for green");
    if(prompt_escaped)
        return;
    space.wave_offsets[3] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for blue");
    if(prompt_escaped)
        return;
    space.wave_offsets[4] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for auxiliary signal");
    if(prompt_escaped)
        return;
    space.wave_offsets[5] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    temp_int = display_prompt_and_echo_int("wave sample offset for stereo audio");
    if(prompt_escaped)
        return;
    space.wave_offsets[6] = temp_int;
    space.wave_offsets[7] = temp_int;
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_wave_offsets();
    display_space();
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::set_color_rescales_menu()
{
    LaserBoy_Rescale_Error_Code  rescale_file_error;
    string                       file_name;
    //------------------------------------------------------------------------
    space.get_directory_list(LASERBOY_RESCALE_SHARE, ".txt");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    file_name = display_prompt_file_with_auto_complete("color rescale file for red");
    if(prompt_escaped)
        return;

    if(file_name == "")
        ; // leave it!
    else if(file_name == ".")
        space.color_rescale_file[0] = ".";
    else
    {
        space.color_rescale_file[0] = LASERBOY_RESCALE_SHARE + file_name;
        if((rescale_file_error = space.load_color_rescale_file(0)))
        {
            display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    file_name = display_prompt_file_with_auto_complete("color rescale file for green");
    if(prompt_escaped)
        return;

    if(file_name == "")
        ; // leave it!
    else if(file_name == ".")
        space.color_rescale_file[1] = ".";
    else
    {
        space.color_rescale_file[1] = LASERBOY_RESCALE_SHARE + file_name;
        if((rescale_file_error = space.load_color_rescale_file(1)))
        {
            display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    file_name = display_prompt_file_with_auto_complete("color rescale file for blue");
    if(prompt_escaped)
        return;
    if(file_name == "")
        ; // leave it!
    else if(file_name == ".")
        space.color_rescale_file[2] = ".";
    else
    {
        space.color_rescale_file[2] = LASERBOY_RESCALE_SHARE + file_name;
        if((rescale_file_error = space.load_color_rescale_file(2)))
        {
            display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    file_name = display_prompt_file_with_auto_complete("color rescale file for auxilary");
    if(prompt_escaped)
        return;
    if(file_name == "")
        ; // leave it!
    else if(file_name == ".")
        space.color_rescale_file[3] = ".";
    else
    {
        space.color_rescale_file[3] = LASERBOY_RESCALE_SHARE + file_name;
        if((rescale_file_error = space.load_color_rescale_file(3)))
        {
            display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_color_rescale_stats();
    display_space();
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::apply_color_rescales_menu()
{
    bool good_call = false;
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    file_name = display_prompt_file_with_auto_complete("wave file name to apply color rescales");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    space.clean_screen();
    display_space();
    if(space.apply_color_rescales_prep(LASERBOY_WAV_SHARE + file_name))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
        if(good_call)
            display_space();
        else
            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::omit_color_rescales_menu()
{
    bool good_call = false;
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    file_name = display_prompt_file_with_auto_complete("wave file name to omit color rescales");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    space.clean_screen();
    display_space();
    if(space.omit_color_rescales(LASERBOY_WAV_SHARE + file_name))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name);
        if(good_call)
            display_space();
        else
            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::set_signal_polarities_menu()
{
    short  wave_channels;
    //------------------------------------------------------------------------
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    file_name = display_prompt_file_with_auto_complete("wave file name to signal invert");
    if(prompt_escaped)
        return;

    if(!file_exists(LASERBOY_WAV_SHARE + file_name))
    {
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    for(u_int i = 0; i < 8; i++)
        space.signal_polarity[i] = -1; // not set!
    //------------------------------------------------------------------------
    wave_channels = space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
    if(wave_channels)
    {
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[0] = display_prompt_and_echo_bool("invert X ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[1] = display_prompt_and_echo_bool("invert Y ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[2] = display_prompt_and_echo_bool("invert red ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[3] = display_prompt_and_echo_bool("invert green ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[4] = display_prompt_and_echo_bool("invert blue ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        space.signal_polarity[5] = display_prompt_and_echo_bool("invert Z or other ? (y | [any])");
        if(prompt_escaped)
            return;
        if(wave_channels > 7)
        {
            space.clean_screen();
            space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
            space.signal_polarity[6] = display_prompt_and_echo_bool("invert L audio ? (y | [any])");
            if(prompt_escaped)
                return;
            space.clean_screen();
            space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
            space.signal_polarity[7] = display_prompt_and_echo_bool("invert R audio ? (y | [any])");
            if(prompt_escaped)
                return;
        }
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + file_name, space.signal_polarity);
        display_space();
        SDL_Delay(250);
        space.TUI_clue = "inverting signals";
        display_space();
        //--------------------------------------------------------------------
        space.invert_signals_prep(LASERBOY_WAV_SHARE + file_name);
        //--------------------------------------------------------------------
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
        else
            display_space();
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::clear_wave_polarities_menu()
{
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_name = display_prompt_file_with_auto_complete("wave file name to clear");
    if(prompt_escaped)
        return;

    if(!file_exists(LASERBOY_WAV_SHARE + file_name))
    {
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    bool global_flip_flop = display_prompt_and_echo_bool("flip global polarity status? (y | [any])");
    if(prompt_escaped)
        return;

    space.TUI_clue = "clearing polarities";
    display_space();
    //------------------------------------------------------------------------
    if(space.clear_wave_polarity_list(LASERBOY_WAV_SHARE + file_name, global_flip_flop))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
            display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
        else
            display_space();
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::black_level_to_wave_menu()
{
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_in = display_prompt_file_with_auto_complete("wave file name to black level");
    if(prompt_escaped)
        return;

    if(file_in.size() < 5 || file_in.substr(file_in.size() - 4) != ".wav")
        file_in += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_in))
    {
        display_error(LASERBOY_WAV_SHARE + file_in + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for output wave");
    if(prompt_escaped)
        return;

    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";

    space.TUI_clue = "black leveling";
    display_space();
    //------------------------------------------------------------------------
    if(space.black_level_to_wave_prep(LASERBOY_WAV_SHARE + file_in, LASERBOY_WAV_SHARE + file_out))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::bit_resolution_to_wave_menu()
{
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_in = display_prompt_file_with_auto_complete("wave file name to bit reduce");
    if(prompt_escaped)
        return;
    if(file_in.size() < 5 || file_in.substr(file_in.size() - 4) != ".wav")
        file_in += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_in))
    {
        display_error(LASERBOY_WAV_SHARE + file_in + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for output wave");
    if(prompt_escaped)
        return;

    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    space.TUI_clue = "bit reducing";
    display_space();
    //------------------------------------------------------------------------
    if(space.bit_resolution_to_wave_prep(LASERBOY_WAV_SHARE + file_in, LASERBOY_WAV_SHARE + file_out))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::join_XY_r_g_b_i_waves_menu()
{
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.TUI_clue = "join XY r g b i";
    space.clean_screen();
    space.overlay_file_list();
    string file_XY = display_prompt_file_with_auto_complete("XY wave");
    if(prompt_escaped)
        return;

    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_r = display_prompt_file_with_auto_complete("red wave");
    if(prompt_escaped)
        return;
    if(file_r.size()   < 5 || file_r.substr(file_r.size() - 4) != ".wav")
        file_r   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_r))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_r + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_g = display_prompt_file_with_auto_complete("green wave");
    if(prompt_escaped)
        return;
    if(file_g.size()   < 5 || file_g.substr(file_g.size() - 4) != ".wav")
        file_g   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_g))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_g + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_b = display_prompt_file_with_auto_complete("blue wave");
    if(prompt_escaped)
        return;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_i = display_prompt_file_with_auto_complete("intensity wave");
    if(prompt_escaped)
        return;
    if(file_i.size()   < 5 || file_i.substr(file_i.size() - 4) != ".wav")
        file_i   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_i))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_i + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out");
    if(prompt_escaped)
        return;
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return;
    space.TUI_clue = "joining waves";
    display_space();
    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    //------------------------------------------------------------------------
    if(space.join_XY_r_g_b_i_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                        LASERBOY_UNFORMATTED_SHARE + file_r,
                                        LASERBOY_UNFORMATTED_SHARE + file_g,
                                        LASERBOY_UNFORMATTED_SHARE + file_b,
                                        LASERBOY_UNFORMATTED_SHARE + file_i,
                                        LASERBOY_WAV_SHARE         + file_out,
                                        global_polarity
                                       )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::join_XY_rg_b_waves_menu()
{
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.TUI_clue = "join XY rg b";
    space.clean_screen();
    space.overlay_file_list();
    string file_XY = display_prompt_file_with_auto_complete("XY wave");
    if(prompt_escaped)
        return;

    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_rg = display_prompt_file_with_auto_complete("red + green wave");
    if(prompt_escaped)
        return;
    if(file_rg.size()  < 5 || file_rg.substr(file_rg.size() - 4) != ".wav")
        file_rg   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_rg))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_rg + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_b = display_prompt_file_with_auto_complete("blue wave");
    if(prompt_escaped)
        return;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out");
    if(prompt_escaped)
        return;

    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return;
    space.TUI_clue = "joining waves";
    display_space();
    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    //------------------------------------------------------------------------
    if(space.join_XY_rg_b_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                     LASERBOY_UNFORMATTED_SHARE + file_rg,
                                     LASERBOY_UNFORMATTED_SHARE + file_b,
                                     LASERBOY_WAV_SHARE         + file_out,
                                     global_polarity
                                    )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error("something horrible and unexplainable just happened.");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::join_XY_rg_b_LR_waves_menu()
{
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.TUI_clue = "join XY rg b LR";
    space.clean_screen();
    space.overlay_file_list();

    string file_XY = display_prompt_file_with_auto_complete("XY wave");
    if(prompt_escaped)
        return;

    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_rg = display_prompt_file_with_auto_complete("red + green wave");
    if(prompt_escaped)
        return;

    if(file_rg.size()  < 5 || file_rg.substr(file_rg.size() - 4) != ".wav")
        file_rg   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_rg))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_rg + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_b = display_prompt_file_with_auto_complete("blue wave");
    if(prompt_escaped)
        return;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.get_directory_list(LASERBOY_AUDIO_SHARE, ".wav");
    space.clean_screen();
    space.overlay_file_list();
    string file_LR = display_prompt_and_echo_name("stereo audio wave (_0 for silence)");
    if(prompt_escaped)
        return;
    if(    file_LR != "_0"
        && (    file_LR.size() < 5
             || file_LR.substr(file_LR.size() - 4) != ".wav"
           )
      )
        file_LR += ".wav";
    if(    file_LR != "_0"
        && !file_exists(LASERBOY_AUDIO_SHARE + file_LR)
      )
    {
        display_error(LASERBOY_AUDIO_SHARE + file_LR + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out");
    if(prompt_escaped)
        return;

    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return;

    space.TUI_clue = "joining waves";
    display_space();

    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    //------------------------------------------------------------------------
    if(space.join_XY_rg_b_LR_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                        LASERBOY_UNFORMATTED_SHARE + file_rg,
                                        LASERBOY_UNFORMATTED_SHARE + file_b,
                                        LASERBOY_AUDIO_SHARE       + file_LR,
                                        LASERBOY_WAV_SHARE         + file_out,
                                        global_polarity
                                       )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::add_audio_to_wave_menu()
{
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");

    space.TUI_clue = "add audio to wave";

    space.clean_screen();
    space.overlay_file_list();

    string file_laser = display_prompt_file_with_auto_complete("6 channel laser wave");
    if(prompt_escaped)
        return;

    if(file_laser.size() < 5 || file_laser.substr(file_laser.size() - 4) != ".wav")
        file_laser += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_laser))
    {
        display_error(LASERBOY_WAV_SHARE + file_laser + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.get_directory_list(LASERBOY_AUDIO_SHARE, ".wav");
    space.overlay_file_list();

    string file_LR = display_prompt_and_echo_name("stereo audio wave (_0 for silence)");
    if(prompt_escaped)
        return;

    if(    file_LR != "_0"
        && (    file_LR.size() < 5
             || file_LR.substr(file_LR.size() - 4) != ".wav"
           )
      )
        file_LR += ".wav";
    if(    file_LR != "_0"
        && !file_exists(LASERBOY_AUDIO_SHARE + file_LR)
      )
    {
        display_error(LASERBOY_AUDIO_SHARE + file_LR + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
    space.overlay_file_list();

    string file_out = display_prompt_and_echo_name("new name for wave out");
    if(prompt_escaped)
        return;

    space.TUI_clue = "adding audio";
    display_space();
    //------------------------------------------------------------------------
    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    //------------------------------------------------------------------------
    if(space.add_audio_to_wave_prep(LASERBOY_WAV_SHARE   + file_laser,
                                    LASERBOY_AUDIO_SHARE + file_LR,
                                    LASERBOY_WAV_SHARE   + file_out
                                   )
      )
    {
        space.TUI_clue = "added audio";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::file_open_menu()
{
    bool bad_key = true;
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_file_type_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input file";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '1': // open ild
                space.current_menu_name = "open ild";
                if(ild_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // open dxf
                space.current_menu_name = "open dxf";
                if(dxf_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open wav
                space.current_menu_name = "open wav";
                if(wav_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // open txt
                space.current_menu_name = "open txt";
                if(txt_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // open bmp
                space.current_menu_name = "open bmp";
                if(bmp_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // open wtf
                space.current_menu_name = "open wtf";
                space.get_directory_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.overlay_file_list();

                file_name = display_prompt_file_with_auto_complete("wtf file name");
                if(prompt_escaped)
                    break;

                if(!space.load_wtf_file(LASERBOY_WTF_SHARE + file_name))
                {
                    display_error(LASERBOY_WTF_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "wtf open";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return;
}

//############################################################################
bool LaserBoy_SDL_GUI::ild_file_open_menu()
{
    bool bad_key = true;
    
    space.get_directory_list(LASERBOY_ILD_SHARE, ".ild");
    space.clean_screen();
    space.overlay_file_list();
    file_name = display_prompt_file_with_auto_complete("ild file name");
    if(prompt_escaped)
        return false;

    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_ild_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input ild";
                return false;
            //----------------------------------------------------------------
            case '1': // replace
                space.TUI_clue = "ild replace";
                space.clean_screen();
                display_space();
                { // scope
                    LaserBoy_frame_set frame_set(&space);
                    if(!report_ild_file_open(frame_set, file_name))
                        return false;
                    space.frame_index = 0;
                    space = frame_set;
                } // end scope
                space.TUI_clue = LASERBOY_ILD_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // prepend
                space.TUI_clue = "ild prepend";
                space.clean_screen();
                display_space();
                { // scope
                    LaserBoy_frame_set frame_set(&space);
                    if(!report_ild_file_open(frame_set, file_name))
                        return false;
                    space.frame_index = 0;
                    frame_set += space;
                    space = frame_set;
                }
                space.TUI_clue = LASERBOY_ILD_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // insert
                space.TUI_clue = "ild insert";
                space.clean_screen();
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set frame_set(&space),
                                       result(&space);
                    if(!report_ild_file_open(frame_set, file_name))
                        return false;
                    for(i = 0; i <= space.frame_index; i++)
                        result += space[i];
                    for(i = 0; i < frame_set.number_of_frames(); i++)
                        result += frame_set[i];
                    for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                        result += space[i];
                    space.frame_index = 0;
                    space = result;
                }
                space.TUI_clue = LASERBOY_ILD_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append
                space.TUI_clue = "ild append";
                space.clean_screen();
                display_space();
                {
                    LaserBoy_frame_set frame_set(&space);
                    if(!report_ild_file_open(frame_set, file_name))
                        return false;
                    space.frame_index = 0;
                    space += frame_set;
                }
                space.TUI_clue = LASERBOY_ILD_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // superimpose
                space.TUI_clue = "ild superimpose";
                space.clean_screen();
                display_space();
                {
                    LaserBoy_frame_set frame_set(&space);
                    if(!report_ild_file_open(frame_set, file_name))
                        return false;
                    space.frame_index = 0;
                    space.superimpose_frame_set(frame_set);
                }
                space.TUI_clue = LASERBOY_ILD_SHARE + file_name;
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
        //--------------------------------------------------------------------
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::dxf_file_open_menu()
{
    bool   bad_key = true;
    space.get_directory_list(LASERBOY_DXF_SHARE, ".dxf");
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_dxf_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input dxf";
                return false;
            //----------------------------------------------------------------
            case '1': // open dxf
                space.clean_screen();
                space.overlay_file_list();
                file_name = display_prompt_file_with_auto_complete("dxf file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "dxf replace";
                space.clean_screen();
                display_space();
                if(space.from_dxf_file(LASERBOY_DXF_SHARE + file_name) != LASERBOY_OK)
                {
                    display_error(LASERBOY_DXF_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // append dxf
                space.clean_screen();
                space.overlay_file_list();
                file_name = display_prompt_file_with_auto_complete("dxf file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "dxf append";
                space.clean_screen();
                display_space();
                if(space.from_dxf_file(LASERBOY_DXF_SHARE + file_name, true) != LASERBOY_OK)
                {
                    display_error(LASERBOY_DXF_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open dxf directory
                space.clean_screen();
                space.overlay_directory_list();
                file_name = display_prompt_dir_with_auto_complete("dxf directory name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "dxf set replace";
                display_space();
                if(!space.from_dxf_directory(LASERBOY_DXF_SHARE + file_name))
                {
                    display_error(LASERBOY_DXF_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append dxf directory
                space.clean_screen();
                space.overlay_directory_list();
                file_name = display_prompt_dir_with_auto_complete("dxf directory name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "dxf set append";
                display_space();
                if(!space.from_dxf_directory(LASERBOY_DXF_SHARE + file_name, true))
                {
                    display_error(LASERBOY_DXF_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + file_name;
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::wav_file_open_menu()
{
    bool bad_key = true;

    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_wav_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input wav";
                return false;
            //----------------------------------------------------------------
            case '1': // replace frame set with LaserBoy formatted wave
                space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.overlay_file_list();
                file_name = display_prompt_file_with_auto_complete("wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "opening wave";
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_WAV_SHARE + file_name))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        display_error(file_name + " is not a LaserBoy formatted wave");
                    else
                        display_error(file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // append frame set with LaserBoy formatted wave
                space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.overlay_file_list();
                file_name = display_prompt_file_with_auto_complete("wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "opening wave";
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_WAV_SHARE + file_name, true))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        display_error(file_name + " is not a LaserBoy formatted wave");
                    else
                        display_error(file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open LaserBoy wave as unformatted
            {
                space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.overlay_file_list();
                string file_name = display_prompt_file_with_auto_complete("wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "opening wave";
                if(!file_exists(LASERBOY_WAV_SHARE + file_name))
                {
                    display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }

                bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
                if(prompt_escaped)
                    break;
                space.clean_screen();
                display_space();
                if(!space.from_unformatted_wave_file(LASERBOY_WAV_SHARE + file_name, global_polarity, false))
                {
                    display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + file_name;
                bad_key = false;
                break;
            }
            //----------------------------------------------------------------
            case '4': // replace frame set with unformatted wave
                if(wav_unformatted_open_menu(false))
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // append frame set with unformatted wave
                if(wav_unformatted_open_menu(true))
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // open qm blanked wave
                if(wav_qm_open_menu(false))
                    bad_key = false;
                break;
        }
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::wav_unformatted_open_menu(bool append)
{
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.overlay_file_list();
    display_space();
    string file_name = display_prompt_file_with_auto_complete("wave file name");
    if(prompt_escaped)
        return false;
    space.TUI_clue = "opening wave";

    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return false;
    display_space();
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_name))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    if(!space.from_unformatted_wave_file(LASERBOY_UNFORMATTED_SHARE + file_name, global_polarity, append))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    space.TUI_clue = LASERBOY_UNFORMATTED_SHARE + file_name;
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::wav_qm_open_menu(bool append)
{
    space.get_directory_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.overlay_file_list();
    display_space();
    string file_name = display_prompt_file_with_auto_complete("wave file name");
    if(prompt_escaped)
        return false;
    space.TUI_clue = "opening wave";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return false;
    display_space();
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_name))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    if(!space.from_qm_wave_file(LASERBOY_UNFORMATTED_SHARE + file_name, global_polarity, append))
    {
        display_error(LASERBOY_UNFORMATTED_SHARE + file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    space.TUI_clue = LASERBOY_UNFORMATTED_SHARE + file_name;
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::txt_file_open_menu()
{
    bool          bad_key = true;
    int           current_palette_count = space.number_of_palettes(),
                  new_palette_count,
                  new_frame_count  ;
    stringstream  strings;
    
    space.get_directory_list(LASERBOY_TXT_SHARE, ".txt");
    space.clean_screen();
    space.overlay_file_list();
    file_name = display_prompt_file_with_auto_complete("ASCII txt file name");
    if(prompt_escaped)
        return false;
        
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_file_list();
        space.overlay_in_txt_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        space.clean_screen();
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input txt";
                return false;
            //----------------------------------------------------------------
            case '1': // replace
                space.TUI_clue = "txt replace";
                display_space();
                {
                    LaserBoy_frame_set current(&space);
                    current = space; // make a copy
                    space.from_txt_file(LASERBOY_TXT_SHARE + file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = space.number_of_frames();
                    if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(space.number_of_frames() == 0) // no frames in txt file
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " no frames found"
                                         );
                            space = current;
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // syntax error in txt
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " syntax error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                          );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << file_name;
                            display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file open did not fail)
                }
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // prepend
                space.TUI_clue = "txt prepend";
                display_space();
                { // scope
                    LaserBoy_frame_set insert(&space);
                    insert.from_txt_file(LASERBOY_TXT_SHARE + file_name);
                    new_frame_count   = insert.number_of_frames();
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            insert += space;
                            space = insert;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << file_name;
                            display_message(strings.str());
                            wait_4_any_key();
                        }
                    }
                }
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // insert
                space.TUI_clue = "txt insert";
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set insert(&space),
                                       result(&space);
                    insert.from_txt_file(LASERBOY_TXT_SHARE + file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = insert.number_of_frames();
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            for(i = 0; i <= space.frame_index; i++)
                                result += space[i];
                            for(i = 0; i < insert.number_of_frames(); i++)
                                result += insert[i];
                            for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                                result += space[i];
                            space = result;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << file_name;
                            display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file opened)
                } // end scope
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append
                space.TUI_clue = "txt append";
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set insert(&space),
                                       result(&space);
                    insert.from_txt_file(LASERBOY_TXT_SHARE + file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = insert.number_of_frames();
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            display_error(   LASERBOY_TXT_SHARE
                                           + file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            for(i = 0; i < insert.number_of_frames(); i++)
                                result += insert[i];
                            for(i = 0; i < space.number_of_frames(); i++)
                                result += space[i];
                            space = result;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << file_name;
                            display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file opened)
                } // end scope
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // superimpose
                space.TUI_clue = "txt superimpose";
                display_space();
                space.superimpose_from_txt_file(LASERBOY_TXT_SHARE + file_name, new_frame_count, new_palette_count);
                if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                {
                    display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    return false;
                }
                else
                {
                    if(new_frame_count == 0) // no frames in txt file
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " no frames found"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " format error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " palette name error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " color table vertex mismatch (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " palette name in use (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(new_palette_count)
                    {
                        strings << new_palette_count
                                << " palette"
                                << (   (new_palette_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                    }
                    if(new_frame_count)
                    {
                        strings << new_frame_count
                                << " frame"
                                << (   (new_frame_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                    }
                    if(new_palette_count || new_frame_count)
                    {
                        strings << "found in "
                                << LASERBOY_TXT_SHARE
                                << file_name;
                        display_message(strings.str());
                        wait_4_any_key();
                    }
                } // end else (file opened)
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // superimpose
                space.TUI_clue = "txt palettes only";
                display_space();
                space.palettes_from_txt_file(LASERBOY_TXT_SHARE + file_name);
                new_palette_count = space.number_of_palettes() - current_palette_count;
                if(space.palette_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                {
                    display_error(LASERBOY_TXT_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    return false;
                }
                else
                {
                    if(space.palette_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " format error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.palette_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " palette name error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.palette_set_error & LASERBOY_TXT_PALETTE_NAME_IN_USE)
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " palette name in use (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                    {
                        display_error(   LASERBOY_TXT_SHARE
                                       + file_name
                                       + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(new_palette_count)
                    {
                        strings << new_palette_count
                                << " palette"
                                << (   (new_palette_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                        strings << "found in "
                                << LASERBOY_TXT_SHARE
                                << file_name;
                        display_message(strings.str());
                        wait_4_any_key();
                    }
                } // end else (file opened)
                space.TUI_clue = LASERBOY_TXT_SHARE + file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::bmp_file_open_menu()
{
    bool bad_key = true;
    space.get_directory_list(LASERBOY_BMP_SHARE, ".bmp");
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_file_list();
        space.overlay_in_bmp_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit input bmp";
                return false;
            //----------------------------------------------------------------
            case '1':
                file_name = display_prompt_file_with_auto_complete("bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "loading bmp";
                display_space();
                if(!space.current_frame().color_from_bmp(LASERBOY_BMP_SHARE + file_name))
                {
                    display_error(LASERBOY_BMP_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "color over bmp frame";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                file_name = display_prompt_file_with_auto_complete("bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "loading bmp";
                display_space();
                if(!space.color_from_bmp(LASERBOY_BMP_SHARE + file_name))
                {
                    display_error(LASERBOY_BMP_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "color over bmp all";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                file_name = display_prompt_file_with_auto_complete("bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "loading bmp";
                display_space();
                if(!space.current_frame().subtract_bmp(LASERBOY_BMP_SHARE + file_name))
                {
                    display_error(LASERBOY_BMP_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "subtract bmp frame";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                file_name = display_prompt_file_with_auto_complete("bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "loading bmp";
                display_space();
                if(!space.subtract_bmp(LASERBOY_BMP_SHARE + file_name))
                {
                    display_error(LASERBOY_BMP_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "subtract bmp all";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                file_name = display_prompt_file_with_auto_complete("bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "loading bmp";
                display_space();
                if(!space.load_background_bitmap(LASERBOY_BMP_SHARE + file_name))
                {
                    display_error(LASERBOY_BMP_SHARE + file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "background bmp";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.clear_background_bitmap();
                space.TUI_clue = "clear background";
                bad_key = false;
                break;
        }
    }
    return true;
}

//############################################################################
void LaserBoy_SDL_GUI::save_as_file_menu()
{
    bool bad_key = true;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_file_type_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output file";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '1':
                space.current_menu_name = "save as ild";
                if(save_as_ild_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.current_menu_name = "save as dxf";
                if(save_as_dxf_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.current_menu_name = "save as wav";
                if(save_as_wav_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.current_menu_name = "save as txt";
                if(save_as_txt_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                space.current_menu_name = "save as bmp";
                if(save_as_bmp_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.get_directory_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wtf file name [LaserBoy.wtf]");
                if(prompt_escaped)
                    break;
                if(file_name.size() == 0)
                    file_name = "LaserBoy.wtf";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wtf")
                   file_name += ".wtf";
                if(    file_exists(LASERBOY_WTF_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WTF_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_wtf_file(LASERBOY_WTF_SHARE + file_name))
                    {
                        display_error(LASERBOY_WTF_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "wtf saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return;
}

//############################################################################
bool LaserBoy_SDL_GUI::save_as_ild_menu()
{
    bool   bad_key = true;
    space.get_directory_list(LASERBOY_ILD_SHARE, ".ild");
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_ild_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output ild";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new ild file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving ild";
                display_space();
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".ild")
                    file_name += ".ild";
                if(    file_exists(LASERBOY_ILD_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_ILD_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_ild(LASERBOY_ILD_SHARE + file_name))
                    {
                        display_error(LASERBOY_ILD_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ild saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new ild file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving ild";
                display_space();
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".ild")
                    file_name += ".ild";
                if(    file_exists(LASERBOY_ILD_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_ILD_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.selected_frames().save_as_ild(LASERBOY_ILD_SHARE + file_name))
                    {
                        display_error(LASERBOY_ILD_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ild saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new ild file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving ild";
                display_space();
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".ild")
                    file_name += ".ild";
                if(    file_exists(LASERBOY_ILD_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_ILD_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_ild(LASERBOY_ILD_SHARE + file_name))
                    {
                        display_error(LASERBOY_ILD_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ild saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.clean_screen();
                space.overlay_frame_effects();
                file_name = display_prompt_f_effect_with_auto_complete("frame effect name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving ild";
                space.render_space();
                display_space();
                if(    file_exists(LASERBOY_ILD_SHARE + file_name + ".ild")
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_ILD_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_effect(file_name);
                    if(out_of_bounds & LASERBOY_EFFECT_NAME_NOT_FOUND)
                    {
                        display_error(file_name + " is not a registered LaserBoy frame effect");
                        wait_4_Esc();
                        break;
                    }
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        display_error(file_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ild saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                space.clean_screen();
                space.overlay_frame_set_effects();
                file_name = display_prompt_fs_effect_with_auto_complete("frame set effect name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving ild";
                space.render_space();
                display_space();
                if(    file_exists(LASERBOY_ILD_SHARE + file_name + ".ild")
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_ILD_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_set_effect(file_name);
                    if(out_of_bounds & LASERBOY_EFFECT_NAME_NOT_FOUND)
                    {
                        display_error(file_name + " is not a registered LaserBoy frame effect");
                        wait_4_Esc();
                        break;
                    }
                    if(out_of_bounds & LASERBOY_NO_SELECTED_FRAMES)
                    {
                        display_error(file_name + " requires selected frames");
                        wait_4_Esc();
                        break;
                    }
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        display_error(file_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ild saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::save_as_dxf_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save dxf";
    space.get_directory_list(LASERBOY_DXF_SHARE, ".dxf");
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_dxf_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output dxf";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new dxf file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving dxf";
                space.render_space();
                display_space();
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".dxf")
                    file_name += ".dxf";
                if(    file_exists(LASERBOY_DXF_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_DXF_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_dxf(LASERBOY_DXF_SHARE + file_name))
                    {
                        display_error(LASERBOY_DXF_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "dxf saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.clean_screen();
                space.render_space();
                space.overlay_directory_list();
                file_name = display_prompt_and_echo_name("new dxf directory name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving dxf";
                space.render_space();
                display_space();
                if(directory_exists(LASERBOY_DXF_SHARE + file_name))
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_DXF_SHARE + file_name + " exists! can not be overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.selected_frames().save_as_dxf_directory(LASERBOY_DXF_SHARE + file_name))
                    {
                        display_error(LASERBOY_DXF_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "dxf directory saved";
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.overlay_directory_list();
                file_name = display_prompt_and_echo_name("new dxf directory name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving dxf";
                space.render_space();
                display_space();
                if(directory_exists(LASERBOY_DXF_SHARE + file_name))
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_DXF_SHARE + file_name + " exists! can not be overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_dxf_directory(LASERBOY_DXF_SHARE + file_name))
                    {
                        display_error(LASERBOY_DXF_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "dxf directory saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::save_as_wav_menu()
{
    bool    bad_key = true;
    string  file_name;
    space.TUI_clue = "save as file";
    space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");

    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_wav_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output wav";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + file_name, false, false))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + file_name, false, true))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + file_name, false, false))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + file_name, false, true))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            case '5':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + file_name, true, false))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + file_name, true, true))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '7':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + file_name, true, false))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wave file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving wave";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(    file_exists(LASERBOY_WAV_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WAV_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + file_name, true, true))
                    {
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::save_as_txt_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save ASCII";
    space.get_directory_list(LASERBOY_TXT_SHARE, ".txt");
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_txt_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output txt";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new txt file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving txt";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".txt")
                    file_name += ".txt";
                if(    file_exists(LASERBOY_TXT_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_TXT_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_txt(LASERBOY_TXT_SHARE + file_name))
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "txt saved";
                }

                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new txt file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving txt";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".txt")
                    file_name += ".txt";
                if(    file_exists(LASERBOY_TXT_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_TXT_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.selected_frames().save_as_txt(LASERBOY_TXT_SHARE + file_name))
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new txt file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving txt";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".txt")
                    file_name += ".txt";
                if(    file_exists(LASERBOY_TXT_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_TXT_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_txt(LASERBOY_TXT_SHARE + file_name))
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new palette file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving txt";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".txt")
                    file_name += ".txt";
                if(    file_exists(LASERBOY_TXT_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_TXT_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(!space.current_frame().save_as_txt_table(LASERBOY_TXT_SHARE + file_name))
                        {
                            display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    else if(!space.palette_picker(space.current_frame().palette_index).save_as_txt(LASERBOY_TXT_SHARE + file_name))
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new palette file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving txt";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".txt")
                    file_name += ".txt";
                if(    file_exists(LASERBOY_TXT_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_TXT_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.palette_picker(space.target_palette_index).save_as_txt(LASERBOY_TXT_SHARE + file_name))
                    {
                        display_error(LASERBOY_TXT_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.get_directory_list(LASERBOY_WAV_SHARE, ".wav");
                space.overlay_file_list();
                file_name = display_prompt_file_with_auto_complete("wave file name to save color rescales");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving rescales";
                space.clean_screen();
                space.render_space();
                display_space();
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wav")
                    file_name += ".wav";
                if(space.save_color_rescales(LASERBOY_WAV_SHARE     + file_name,
                                             LASERBOY_RESCALE_SHARE + file_name.substr(0, file_name.size() - 4))
                                            )
                {
                    space.TUI_clue = "wave stats";
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_name))
                        display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                    else
                        display_space();
                }
                else
                    display_error(LASERBOY_WAV_SHARE + file_name + " failed to open");
                wait_4_Esc();
                break;
            //----------------------------------------------------------------
            case '7':
                space.get_directory_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new wtf file name [LaserBoy.wtf]");
                if(prompt_escaped)
                    break;
                if(file_name.size() == 0)
                    file_name = "LaserBoy.wtf";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".wtf")
                   file_name += ".wtf";
                if(    file_exists(LASERBOY_WTF_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_WTF_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_wtf_file(LASERBOY_WTF_SHARE + file_name))
                    {
                        display_error(LASERBOY_WTF_SHARE + file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "wtf saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_SDL_GUI::save_as_bmp_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save bmp";
    space.get_directory_list(LASERBOY_BMP_SHARE, ".bmp");
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_bmp_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while(event.type != SDL_KEYDOWN);
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                space.TUI_clue = "exit output bmp";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.overlay_file_list();
                file_name = display_prompt_and_echo_name("new bitmap file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving bitmap";
                if(file_name.size() < 5 || file_name.substr(file_name.size() - 4) != ".bmp")
                    file_name += ".bmp";
                if(    file_exists(LASERBOY_BMP_SHARE + file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_BMP_SHARE + file_name + " not overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                    space.current_frame().save_as_bmp(LASERBOY_BMP_SHARE + file_name);
                space.TUI_clue = "bitmap saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.clean_screen();
                space.render_space();
                space.overlay_directory_list();
                file_name = display_prompt_and_echo_name("new bitmap directory file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving bitmap directory";
                if(directory_exists(LASERBOY_BMP_SHARE + file_name))
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_BMP_SHARE + file_name + " exists! can not be overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                    space.selected_frames().save_as_bmp_directory(LASERBOY_BMP_SHARE + file_name);
                space.TUI_clue = "bitmap directory saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.overlay_directory_list();
                file_name = display_prompt_and_echo_name("new bitmap directory file name");
                if(prompt_escaped)
                    break;
                space.TUI_clue = "saving bitmap directory";
                if(directory_exists(LASERBOY_BMP_SHARE + file_name))
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    display_error(LASERBOY_BMP_SHARE + file_name + " exists! can not be overwritten! File NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                    space.save_as_bmp_directory(LASERBOY_BMP_SHARE + file_name);
                space.TUI_clue = "bitmap directory saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
void LaserBoy_SDL_GUI::display_rescale_file_error(LaserBoy_Rescale_Error_Code error)
{
    switch(error)
    {
        case LASERBOY_RESCALE_INCOMPLETE_DATA:
            display_error("not enough data");
            break;
        case LASERBOY_RESCALE_IMPROPER_DATA:
            display_error("data out of range");
            break;
        case LASERBOY_RESCALE_ODD_DATA:
            display_error("data contains odd numbers");
            break;
        case LASERBOY_RESCALE_NONZERO_ZERO:
            display_error("first value is not zero");
            break;
        case LASERBOY_RESCALE_REDUNDANT_DATA:
            display_error("redundant data elements");
            break;
        case LASERBOY_RESCALE_FILE_FAILED:
            display_error("file open failed");
            break;
    }
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::process_terminate_request()
{
    LaserBoy_color bg_was = space.bg_color;
    //------------------------------------------------------------------------
    space.bg_color = (u_int)0x00ff0000;
    space.TUI_clue = "terminating";
    space.recolor_background();
    space.clean_screen();
    //------------------------------------------------------------------------
    if(display_prompt_and_echo_bool("Do you really want to terminate LaserBoy? (y | [any])"))
        running = false;
    else
    {
        teminate_requested = false;
        space.TUI_clue     = "terminate canceled";
        space.bg_color     = bg_was;
        space.recolor_background();
        space.clean_screen();
        space.render_space();
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_SDL_GUI::Laserboy_terminating()
{
    space.bg_color = (u_int)0x000000ff;
    space.TUI_clue = "terminating";
    space.recolor_background();
    space.clean_screen();
    //------------------------------------------------------------------------
    if(!display_prompt_and_echo_bool("Exit without saving OUT.ild? (y | [any])"))
    {
        space.TUI_clue = "saving OUT.ild";
        display_space();
        if(space.auto_minimize)
            space.minimize();
        space.save_as_ild(LASERBOY_ILD_SHARE + "OUT.ild");
    }
    //------------------------------------------------------------------------
    else
    {
        space.TUI_clue = "LaserBoy terminated";
        display_space();
    }
    //------------------------------------------------------------------------
    SDL_Delay(500);
    return;
}

//############################################################################
int LaserBoy_SDL_GUI::start_menu_loop()
{
    char           formatted_string[80] = {0};
    long int       bytes_skipped = 0;
    ostringstream  output_message;

    running = true;
    string version_string;
    if(    !LaserBoy_version_check(version_string, space.install_GUID, space.app_runs_count) // zero means we got a version_string
        && (version_string.size() == 19)
        && (version_string != LASERBOY_VERSION)
        && (version_string.substr(0, 9) == "LaserBoy-")
      )
    {
        display_message(version_string + string(" is now available (README.txt)"));
        wait_4_any_key();
    }
    space.TUI_clue = "NO DATA!";
    space.from_ild_file(LASERBOY_ILD_SHARE + "in.ild", bytes_skipped);

    if(space.frame_set_error == LASERBOY_OK)
        space.TUI_clue = LASERBOY_ILD_SHARE + "in.ild";
    else if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
    {
        display_error(LASERBOY_ILD_SHARE + file_name + " table to frame quantity mismatch");
        wait_4_Esc();
    }
    else if(space.frame_set_error & LASERBOY_NO_FRAMES_FOUND)
    {
        display_error(LASERBOY_ILD_SHARE + file_name + " no frames found");
        wait_4_Esc();
    }
    else if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED)
    {
        display_error(LASERBOY_ILD_SHARE + file_name + " failed to open");
        wait_4_Esc();
    }
    //--------------------------------------------------------------------
    if(bytes_skipped > 0)
    {
        space.clean_screen();
        output_message << LASERBOY_ILD_SHARE
                       << "in.ild : "
                       << bytes_skipped
                       << " bytes unrecognized data";
        display_error(output_message.str());
        wait_4_Esc();
    }
    //--------------------------------------------------------------------
    display_space();
    while(running)
    {
        space.clean_screen();
        space.set_current_menu_name("main menu", 0x00000000);
        space.render_space();
        if(space.show_menu)
            space.overlay_program_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            SDL_WaitEvent(&event);
        } while((event.type != SDL_KEYDOWN));
        //--------------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            //----------------------------------------------------------------
            case SDLK_ESCAPE:
                teminate_requested = true;
                break;
            //----------------------------------------------------------------
            case SDLK_RETURN:
                space.show_menu = !space.show_menu;
                if(!space.show_menu)
                    space.TUI_clue = "[Enter] menus";
                break;
            //----------------------------------------------------------------
            case SDLK_BACKSPACE:
            case SDLK_DELETE:
                space.delete_current_frame();
                space.TUI_clue = "deleted";
                break;
            //----------------------------------------------------------------
            default:
                check_for_common_nav_keys(event.key.keysym.sym);
                break;
            //----------------------------------------------------------------
            case '`':
                animate_forward();
                break;
            //----------------------------------------------------------------
            case '~':
                animate_reverse();
                break;
            //----------------------------------------------------------------
            case '-':
                rotate_forward();
                break;
            //----------------------------------------------------------------
            case '_':
                rotate_reverse();
                break;
            //----------------------------------------------------------------
            case '=':
                animate_rotate_forward();
                break;
            //----------------------------------------------------------------
            case '+':
                animate_rotate_reverse();
                break;
            //----------------------------------------------------------------
            case '/': // show frame set stats
                space.show_frame_set_stats = !space.show_frame_set_stats;
                break;
            //----------------------------------------------------------------
            case '?': // show frame stats
                space.show_frame_stats = !space.show_frame_stats;
                break;
            //----------------------------------------------------------------
            case ';': // toggle bitmap background
                space.show_background_bitmap = !space.show_background_bitmap;
                if(space.show_background_bitmap)
                    space.TUI_clue = "show bitmap";
                else
                    space.TUI_clue = "hide bitmap";
                break;
            //----------------------------------------------------------------
            case '1':
                space.view_angle.x += space.rotation_step;
                if(space.view_angle.x > two_pi)
                    space.view_angle.x -= two_pi;
                space.TUI_clue = "+ orbit X";
                break;
            //----------------------------------------------------------------
            case '!':
                space.view_angle.x -= space.rotation_step;
                if(space.view_angle.x < -two_pi)
                    space.view_angle.x += two_pi;
                space.TUI_clue = "- orbit X";
                break;
            //----------------------------------------------------------------
            case '2':
                space.view_angle.y += space.rotation_step;
                if(space.view_angle.y > two_pi)
                    space.view_angle.y -= two_pi;
                space.TUI_clue = "+ orbit Y";
                break;
            //----------------------------------------------------------------
            case '@':
                space.view_angle.y -= space.rotation_step;
                if(space.view_angle.y < -two_pi)
                    space.view_angle.y += two_pi;
                space.TUI_clue = "- orbit Y";
                break;
            //----------------------------------------------------------------
            case '3':
                space.view_angle.z += space.rotation_step;
                if(space.view_angle.z > two_pi)
                    space.view_angle.z -= two_pi;
                space.TUI_clue = "+ orbit Z";
                break;
            //----------------------------------------------------------------
            case '#':
                space.view_angle.z -= space.rotation_step;
                if(space.view_angle.z < -two_pi)
                    space.view_angle.z += two_pi;
                space.TUI_clue = "- orbit Z";
                break;
            //----------------------------------------------------------------
            case '4':
                space.view_angle += space.rotation_step;
                if(space.view_angle.x > two_pi)
                    space.view_angle.x -= two_pi;
                if(space.view_angle.y > two_pi)
                    space.view_angle.y -= two_pi;
                if(space.view_angle.z > two_pi)
                    space.view_angle.z -= two_pi;
                space.TUI_clue = "+ orbit XYZ";
                break;
            //----------------------------------------------------------------
            case '$':
                space.view_angle -= space.rotation_step;
                if(space.view_angle.x < -two_pi)
                    space.view_angle.x += two_pi;
                if(space.view_angle.y < -two_pi)
                    space.view_angle.y += two_pi;
                if(space.view_angle.z < -two_pi)
                    space.view_angle.z += two_pi;
                space.TUI_clue = "- orbit XYZ";
                break;
            //----------------------------------------------------------------
            case '5':
                space.view_angle   = 0;
                space.TUI_clue = "front";
                break;
            //----------------------------------------------------------------
            case '%':
                space.view_angle.x = 0;
                space.view_angle.y = pi;
                space.view_angle.z = 0;
                space.TUI_clue = "back";
                break;
            //----------------------------------------------------------------
            case '6':
                space.view_angle.x = 0;
                space.view_angle.y = half_pi;
                space.view_angle.z = 0;
                space.TUI_clue = "left side";
                break;
            //----------------------------------------------------------------
            case '^':
                space.view_angle.x = 0;
                space.view_angle.y = -half_pi;
                space.view_angle.z = 0;
                space.TUI_clue = "right side";
                break;
            //----------------------------------------------------------------
            case '7':
                space.view_angle.x = half_pi;
                space.view_angle.y = 0;
                space.view_angle.z = 0;
                space.TUI_clue = "top";
                break;
            //----------------------------------------------------------------
            case '&':
                space.view_angle.x = -half_pi;
                space.view_angle.y = 0;
                space.view_angle.z = 0;
                space.TUI_clue = "bottom";
                break;
            //----------------------------------------------------------------
            case '8':
                space.view_angle += space.rotation_step;
                if(    space.loop_animation
                    || (space.frame_index < space.number_of_frames() - 1)
                  )
                    space.pre_incr_current_frame();
                space.TUI_clue = "+ frame & orbit";
                break;
            //----------------------------------------------------------------
            case '*':
                space.view_angle -= space.rotation_step;
                if(    space.loop_animation
                    || (space.frame_index > 0)
                  )
                    space.pre_decr_current_frame();
                space.TUI_clue = "- frame & orbit";
                break;
            //----------------------------------------------------------------
            case '9':
                space.frame(0);
                space.TUI_clue = "first frame";
                break;
            //----------------------------------------------------------------
            case '0':
                space.frame(space.number_of_frames() - 1);
                space.TUI_clue = "last frame";
                break;
            //----------------------------------------------------------------
            case '(': // set, unset start frame select
                space.toggle_frame_select_start();
                if(space.frame_select_start != -1)
                {
                    sprintf(formatted_string, "start = %d", space.frame_select_start);
                    space.TUI_clue = formatted_string;
                }
                else
                    space.TUI_clue = "start unset";
                break;
            //----------------------------------------------------------------
            case ')': // set, unset end frame select
                space.toggle_frame_select_end();
                if(space.frame_select_end != -1)
                {
                    sprintf(formatted_string, "end = %d", space.frame_select_end);
                    space.TUI_clue = formatted_string;
                }
                else
                    space.TUI_clue = "end unset";
                break;
            //----------------------------------------------------------------
            case ' ': // [Space] select, deselect current frame
                space.current_frame().is_selected = !(space.current_frame().is_selected);
                if(space.current_frame().is_selected)
                    space.TUI_clue = "select frame";
                else
                    space.TUI_clue = "deselect frame";
                break;
            //----------------------------------------------------------------
            case 'q':
                space.view_offset.x += space.displacement_step;
                space.TUI_clue = "+ pan X";
                break;
            //----------------------------------------------------------------
            case 'Q':
                space.view_offset.x -= space.displacement_step;
                space.TUI_clue = "- pan X";
                break;
            //----------------------------------------------------------------
            case 'w':
                space.view_offset.y += space.displacement_step;
                space.TUI_clue = "+ pan Y";
                break;
            //----------------------------------------------------------------
            case 'W':
                space.view_offset.y -= space.displacement_step;
                space.TUI_clue = "- pan Y";
                break;
            //----------------------------------------------------------------
            case 'e':
                space.view_offset += space.displacement_step;
                space.TUI_clue = "+ pan XY";
                break;
            //----------------------------------------------------------------
            case 'E':
                space.view_offset -= space.displacement_step;
                space.TUI_clue = "- pan XY";
                break;
            //----------------------------------------------------------------
            case 'r':
            case 'R':
                space.view_offset = 0;
                space.TUI_clue = "on center";
                break;
            //----------------------------------------------------------------
            case 'a':
                space.view_scale.x *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom X";
                break;
            //----------------------------------------------------------------
            case 'A':
                space.view_scale.x *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom X";
                break;
            //----------------------------------------------------------------
            case 's':
                space.view_scale.y *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom Y";
                break;
            //----------------------------------------------------------------
            case 'S':
                space.view_scale.y *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom Y";
                break;
            //----------------------------------------------------------------
            case 'd':
                space.view_scale *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom XY";
                break;
            //----------------------------------------------------------------
            case 'D':
                space.view_scale *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom XY";
                break;
            //----------------------------------------------------------------
            case 'f':
            case 'F':
                space.view_scale = 1.00;
                space.TUI_clue = "no zoom";
                break;
            //----------------------------------------------------------------
            case 'y': // go to frame
            case 'Y':
            {
                int temp_int;
                space.TUI_clue = "go to frame";
                temp_int = abs(display_prompt_and_echo_int("frame index"));
                if(prompt_escaped)
                    break;
                space.frame_index = temp_int % space.number_of_frames();
                break;
            }
            //----------------------------------------------------------------
            case 'z': // +100 frames
                if(space.number_of_frames() > 100)
                {
                    if(    space.loop_animation
                        || (space.frame_index <= (space.number_of_frames() - 101))
                      )
                        space.frame_index += 100;
                    while(space.frame_index >= space.number_of_frames())
                        space.frame_index = space.frame_index - space.number_of_frames();
                    space.TUI_clue = "+100 frames";
                }
                break;
            //----------------------------------------------------------------
            case 'Z': // -100 frames
                if(space.number_of_frames() > 100)
                {
                    if(    space.loop_animation
                        || (space.frame_index >= 100)
                      )
                        space.frame_index -= 100;
                    if(space.frame_index < 0)
                        space.frame_index = space.number_of_frames() + space.frame_index;
                    space.TUI_clue = "-100 frames";
                }
                break;
            //----------------------------------------------------------------
            // call out to another menu loop
            //----------------------------------------------------------------
            case 't':
            case 'T':
                space.set_current_menu_name("bit resolution", 0x00040000, true);
                bit_resolution_menu();
                break;
            //----------------------------------------------------------------
            case 'u':
            case 'U':
                space.set_current_menu_name("visual settings", 0x00000040, true);
                visuals_settings_menu();
                break;
            //----------------------------------------------------------------
            case 'x':
            case 'X':
                space.set_current_menu_name("switch settings", 0x00003030, true);
                switch_settings_menu();
                break;
            //----------------------------------------------------------------
            case 'v':
            case 'V':
                space.set_current_menu_name("apply view", 0x00300030, true);
                apply_view_menu();
                break;
            //----------------------------------------------------------------
            case 'p':
            case 'P':
                space.set_current_menu_name("palette transforms", 0x00303000, true);
                palette_transforms_menu();
                break;
            //----------------------------------------------------------------
            case 'h':
            case 'H':
                space.set_current_menu_name("frame transforms", 0x00202020, true);
                frame_transforms_menu();
                break;
            //----------------------------------------------------------------
            case 'j':
            case 'J':
                space.set_current_menu_name("frame set transforms", 0x00400020, true);
                frame_set_transforms_menu();
                break;
            //----------------------------------------------------------------
            case 'k':
            case 'K':
                space.set_current_menu_name("draw color blank", 0x00200040, true);
                draw_color_blank_menu();
                break;
            //----------------------------------------------------------------
            case 'l':
            case 'L':
                space.set_current_menu_name("move scale rotate", 0x00204000, true);
                move_scale_rotate_menu();
                break;
            //----------------------------------------------------------------
            case 'm':
            case 'M':
                space.set_current_menu_name("render segments", 0x00402000, true);
                direct_draw_segments_menu();
                break;
            //----------------------------------------------------------------
            case 'b':
            case 'B':
                space.set_current_menu_name("wave utilities", 0x00002040, true);
                wave_utilities_menu();
                break;
            //----------------------------------------------------------------
            case 'i':
            case 'I':
                space.set_current_menu_name("input file", 0x00004020, true);
                file_open_menu();
                break;
            //----------------------------------------------------------------
            case 'o':
            case 'O':
                space.set_current_menu_name("output file", 0x00204020, true);
                save_as_file_menu();
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
        //--------------------------------------------------------------------
        if(teminate_requested)
            process_terminate_request();
        //--------------------------------------------------------------------
    } // end while(running)
    //------------------------------------------------------------------------
    Laserboy_terminating();
    return 0;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
