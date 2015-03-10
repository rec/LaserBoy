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
// LaserBoy_bmp.hpp is part of LaserBoy.
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
#ifndef __BMP_FUNCTIONS__
#define __BMP_FUNCTIONS__

#include <LaserBoy/common.hpp>
#include <LaserBoy/font.hpp>
#include <LaserBoy/color.hpp>

//############################################################################
extern "C"
{
//############################################################################
struct LaserBoy_bmp
{                               //  defaults     offset
    char      B               ; //  'B'          0 :
    char      M               ; //  'M'          1 :
    u_int     file_size       ; //               2 :
    int       reserved        ; //  0            6 :
    int       jump            ; //              10 : offset to pixel data
    int       to_end          ; //  40          14 : from here to end of header
    u_int     xres            ; //              18 :
    u_int     yres            ; //              22 :
    u_short   planes          ; //  1           26 :
    u_short   bpp             ; //              28 : bits per pixel
    int       comp            ; //  0           30 : no compression
    u_int     image_size      ; //              34 : in bytes
    u_int     xppm            ; //  2835        38 : pixels per meter
    u_int     yppm            ; //  2835        42 : approx. 72 dots per inch
    u_int     c_used          ; //              46 :
    u_int     c_important     ; //              50 : ................
    u_int     black           ; //              54 : non standard   |
    u_int     white           ; //              58 : added for ezfb |
    u_int     gray            ; //              58 :                |
    int       fd              ; //              62 :                |
    u_int     bytes_per_line  ; //              66 :                |
    u_int     palette_size    ; //              70 :                |
    u_char*   data            ; //  null        74 :                |
    u_int*    palette         ; //  null        78 :                |
    u_char*   image           ; //  null        82 :                |
    u_char**  uchar_row       ; //  null        86 :                |
    u_short** ushort_row      ; //  null        90 :                |
    u_int**   uint_row        ; //  null        94 :                |
    // function pointers
    u_int  (*make_rgb     )(u_char r, u_char g, u_char b);
    u_char (*r_from_rgb   )(u_int rgb                   );
    u_char (*g_from_rgb   )(u_int rgb                   );
    u_char (*b_from_rgb   )(u_int rgb                   );
    u_int  (*put_pixel    )(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c);
    u_int  (*get_pixel    )(struct LaserBoy_bmp* bmp, u_int x, u_int y);
    u_int  (*get_pixel_rgb)(struct LaserBoy_bmp* bmp, u_int x, u_int y);
};

//############################################################################
struct lb_rectangle
{
    int x1    ;
    int y1    ;
    int x2    ;
    int y2    ;
    int height;
    int width ;
};

//############################################################################
//  UTILITY
void  dump_lb_rectangle   (struct lb_rectangle* area, char* message);
void  bmp_dump_header     (struct LaserBoy_bmp* bmp, char* file_name, char* message);

u_int LaserBoy_hue        (struct LaserBoy_bmp* bmp, u_short hue);
u_int LaserBoy_tint       (struct LaserBoy_bmp* bmp, u_short hue, u_char tint );
u_int LaserBoy_shade      (struct LaserBoy_bmp* bmp, u_short hue, u_char shade);

//############################################################################
inline int bmp_set_palette_index(struct LaserBoy_bmp* bmp, u_int index, u_char r, u_char g, u_char b)
{
    if(     bmp->bpp <= 8
        && (index < (u_int)(0x01 << bmp->bpp))
      )
    {
        bmp->palette[index] = ((r << 16) | (g << 8) | (b));
        return 1;
    }
    else
        return 0;
}

//############################################################################
inline u_int bmp_get_palette_index(struct LaserBoy_bmp* bmp, u_int index)
{
    if(     bmp->bpp <= 8
        && (index < (u_int)(0x01 << bmp->bpp))
      )
        return bmp->palette[index];
    return 0;
}

//############################################################################
inline u_char bmp_get_palette_index_r(struct LaserBoy_bmp* bmp, u_int index)
{
    return (bmp_get_palette_index(bmp, index) & 0x00ff0000) >> 16;
}

//############################################################################
inline u_char bmp_get_palette_index_g(struct LaserBoy_bmp* bmp, u_int index)
{
    return (bmp_get_palette_index(bmp, index) & 0x0000ff00) >> 8;
}

//############################################################################
inline u_char bmp_get_palette_index_b(struct LaserBoy_bmp* bmp, u_int index)
{
    return bmp_get_palette_index(bmp, index) & 0x000000ff;
}

//############################################################################
inline int bmp_is_same_size(struct LaserBoy_bmp* a, struct LaserBoy_bmp* b)
{
    if(    a->bpp  == b->bpp
        && a->xres == b->xres
        && a->yres == b->yres
      )
        return 1;
    return 0;
}

//############################################################################
//  BITMAP IN MEMORY
void        bmp_init              (struct LaserBoy_bmp* bmp, u_int x, u_int y, u_short bpp);
long double bmp_put_line          (struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c);
void        bmp_put_line_fast     (struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c);
long double bmp_put_ray           (struct LaserBoy_bmp* bmp, u_int x, u_int y, long double r, long double t, u_int c);
void        bmp_put_ray_fast      (struct LaserBoy_bmp* bmp, u_int x, u_int y, long double r, long double t, u_int c);
void        bmp_put_rectangle     (struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c, int filled);
void        bmp_put_vertex        (struct LaserBoy_bmp* bmp, int x, int y, u_int c);
void        bmp_put_select        (struct LaserBoy_bmp* bmp, int x, int y, u_int c);
//----------------------------------------------------------------------------
int         bmp_put_byte_pattern  (struct LaserBoy_bmp* bmp,
                                   u_char byte,
                                   int    x,
                                   int    y,
                                   u_int  fc,
                                   u_int  bc,
                                   u_int  dir,
                                   u_int  size
                                  );
//----------------------------------------------------------------------------
int         bmp_put_ascii         (struct LaserBoy_bmp* bmp,
                                   u_char ascii,
                                   int    x,
                                   int    y,
                                   u_int  fc,
                                   u_int  bc,
                                   u_int  dir,
                                   u_int  size
                                  );
//----------------------------------------------------------------------------
int         bmp_put_string        (struct LaserBoy_bmp* bmp,
                                   char*  str,
                                   int    x,
                                   int    y,
                                   u_int  fc,
                                   u_int  bc,
                                   u_int  dir,
                                   u_int  size
                                  );
//----------------------------------------------------------------------------
int         bmp_printf            (struct LaserBoy_bmp* bmp,
                                   int    x,
                                   int    y,
                                   u_int  fc,
                                   u_int  bc,
                                   u_int  dir,
                                   u_int  size,
                                   const char* text_format,
                                   ...
                                  );
//----------------------------------------------------------------------------
void        bmp_dump_palette      (struct LaserBoy_bmp* bmp);
void        bmp_dump_image        (struct LaserBoy_bmp* bmp);
void        bmp_clear             (struct LaserBoy_bmp* bmp, u_char c);
void        bmp_fill              (struct LaserBoy_bmp* bmp, u_char r, u_char g, u_char b);
void        bmp_copy              (struct LaserBoy_bmp* target, struct LaserBoy_bmp* original);
void        bmp_flip              (struct LaserBoy_bmp* bmp);
void        bmp_into_bmp          (struct LaserBoy_bmp* target, struct LaserBoy_bmp* insert, int size);
void        bmp_free              (struct LaserBoy_bmp* bmp);
//----------------------------------------------------------------------------
//  BITMAP MEMORY AND FILE
int         bmp_from_file         (struct LaserBoy_bmp* bmp, char* file_name);
int         bmp_save_as           (struct LaserBoy_bmp* bmp, char* file_name);
//############################################################################
} // end extern "C"

#endif // __BMP_FUNCTIONS__

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
