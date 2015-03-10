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
// LaserBoy_bmp.cpp is part of LaserBoy.
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
#include <LaserBoy/bmp.hpp>

//############################################################################
// UTILITY ROUTINES
void bmp_fix_rectangle            (struct lb_rectangle* area, int max_x, int max_y);
// BITMAP IN MEMORY
void bmp_set_functions            (struct LaserBoy_bmp* bmp, u_short bpp);
void bmp_malloc                   (struct LaserBoy_bmp* bmp);
void bmp_find_black_gray_white    (struct LaserBoy_bmp* bmp);
// BITMAP MEMORY AND FILE
int  bmp_open_for_read            (struct LaserBoy_bmp* bmp, char* file_name);
int  bmp_read_file_header         (struct LaserBoy_bmp* bmp);
int  bmp_read_file_palette        (struct LaserBoy_bmp* bmp);
int  bmp_read_file_image          (struct LaserBoy_bmp* bmp);
int  bmp_open_for_write           (struct LaserBoy_bmp* bmp, char* file_name);
int  bmp_write_file_header        (struct LaserBoy_bmp* bmp);
int  bmp_write_palette            (struct LaserBoy_bmp* bmp);
int  bmp_write_image              (struct LaserBoy_bmp* bmp);

//############################################################################





//############################################################################
// UTILITY ROUTINES
//############################################################################
void bmp_fix_rectangle(struct lb_rectangle* area, int max_x, int max_y)
{
    int swap;
    if(0 > area->x1)            area->x1 = 0;
    if(0 > area->x2)            area->x2 = max_x;
    if(0 > area->y1)            area->y1 = 0;
    if(0 > area->y2)            area->y2 = max_y;
    if(area->x1 > area->x2)  {  swap = area->x2;  area->x2 = area->x1;  area->x1 = swap;  }
    if(area->y1 > area->y2)  {  swap = area->y2;  area->y2 = area->y1;  area->y1 = swap;  }
    if(max_x < area->x2)        area->x2 = max_x;
    if(max_y < area->y2)        area->y2 = max_y;
    area->height = area->y2 - area->y1 + 1;
    area->width  = area->x2 - area->x1 + 1;
    return;
}

//############################################################################
void dump_lb_rectangle(struct lb_rectangle* area, char* message)
{
    printf("\n             %s\n"         , message      );
    printf(  "             x1     = %d\n", area->x1     );
    printf(  "             y1     = %d\n", area->y1     );
    printf(  "             x2     = %d\n", area->x2     );
    printf(  "             y2     = %d\n", area->y2     );
    printf(  "             width  = %d\n", area->width  );
    printf(  "             height = %d\n", area->height );
    printf("\n");
    return;
}

//############################################################################
void bmp_dump_header(struct LaserBoy_bmp* bmp, char* file_name, char* message)
{
    printf("\n%s %s\n", message, file_name);
    //printf(  "             B              = %c\n"  , bmp->B              );
    //printf(  "             M              = %c\n"  , bmp->M              );
    printf(  "             file_size      = %d\n"  , bmp->file_size      );
    //printf(  "             reserved       = %d\n"  , bmp->reserved       );
    //printf(  "             jump           = %d\n"  , bmp->jump           );
    //printf(  "             to_end         = %d\n"  , bmp->to_end         );
    printf(  "             xres           = %d\n"  , bmp->xres           );
    printf(  "             yres           = %d\n"  , bmp->yres           );
    //printf(  "             planes         = %d\n"  , bmp->planes         );
    printf(  "             bpp            = %d\n"  , bmp->bpp            );
    //printf(  "             comp           = %d\n"  , bmp->comp           );
    printf(  "             image_size     = %d\n"  , bmp->image_size     );
    printf(  "             xppm           = %d\n"  , bmp->xppm           );
    printf(  "             yppm           = %d\n"  , bmp->yppm           );
    printf(  "             c_used         = %d\n"  , bmp->c_used         );
    printf(  "             c_important    = %d\n"  , bmp->c_important    );
    printf(  "             black          = %d\n"  , bmp->black          );
    printf(  "             gray           = %d\n"  , bmp->gray           );
    printf(  "             white          = %d\n"  , bmp->white          );
    //printf(  "             fd             = %d\n"  , bmp->fd             );
    //printf(  "             bytes_per_line = %d\n"  , bmp->bytes_per_line );
    printf(  "             palette_size   = %d\n"  , bmp->palette_size   );
    printf(  "\n"                                             );
    return;
}

//############################################################################







//############################################################################
u_int LaserBoy_hue(struct LaserBoy_bmp* bmp, u_short hue)
{
    u_short r = 0, g = 0, b = 0;
    u_char  shift;
    hue %= 1530;
    shift = hue % 255;
    if(hue < 255)
    {
        r = 255;
        g = shift;
    }
    else if(hue < 510)
    {
        r = 256 - shift;
        g = 255;
    }
    else if(hue < 765)
    {
        g = 255;
        b = shift;
    }
    else if(hue < 1020)
    {
        g = 256 - shift;
        b = 255;
    }
    else if(hue < 1275)
    {
        r = shift;
        b = 255;
    }
    else
    {
        r = 255;
        b = 256 - shift;
    }
    return bmp->make_rgb(r, g, b);
}

//############################################################################
u_int LaserBoy_tint(struct LaserBoy_bmp* bmp, u_short hue, u_char tint)
{
    u_short r = 0,
            g = 0,
            b = 0;
    u_char  shift;
    hue %= 1530;
    shift = hue % 255;
    if(hue < 255)
    {
        r = 255;
        g = (u_short)(shift + (tint * (255 - shift) / 255.0));
        g = (g > 255) ? (255) : (g);
        b = tint;
    }
    else if(hue < 510)
    {
        r = (u_short)((256 - shift) + (tint * shift / 255.0));
        r = (r > 255) ? (255) : (r);
        g = 255;
        b = tint;
    }
    else if(hue < 765)
    {
        r = tint;
        g = 255;
        b = (u_short)(shift + (tint * (255 - shift) / 255.0));
        b = (b > 255) ? (255) : (b);
    }
    else if(hue < 1020)
    {
        r = tint;
        g = (u_short)((256 - shift) + (tint * shift / 255.0));
        g = (g > 255) ? (255) : (g);
        b = 255;
    }
    else if(hue < 1275)
    {
        r = (u_short)(shift + (tint * (255 - shift) / 255.0));
        r = (r > 255) ? (255) : (r);
        g = tint;
        b = 255;
    }
    else
    {
        r = 255;
        g = tint;
        b = (u_short)((256 - shift) + (tint * shift / 255.0));
        b = (b > 255) ? (255) : (b);
    }
    return bmp->make_rgb(r, g, b);
}

//############################################################################
u_int LaserBoy_shade(struct LaserBoy_bmp* bmp, u_short hue, u_char shade)
{
    u_short r = 0,
            g = 0,
            b = 0;
    u_char  shift;
    double  factor = (255 - shade) / 255.0;
    hue %= 1530;
    shift = hue % 255;
    if(hue < 255)
    {
        r = (u_short)(255   * factor);
        g = (u_short)(shift * factor);
    }
    else if(hue < 510)
    {
        r = (u_short)((255 - shift) * factor);
        g = (u_short) (255          * factor);
    }
    else if(hue < 765)
    {
        g = (u_short)(255   * factor);
        b = (u_short)(shift * factor);
    }
    else if(hue < 1020)
    {
        g = (u_short)((255 - shift) * factor);
        b = (u_short)( 255          * factor);
    }
    else if(hue < 1275)
    {
        r = (u_short)(shift * factor);
        b = (u_short)(255   * factor);
    }
    else
    {
        r = (u_short)( 255          * factor);
        b = (u_short)((255 - shift) * factor);
    }
    return bmp->make_rgb(r, g, b);
}

//############################################################################














//############################################################################
// BITMAP IN MEMORY
//############################################################################
u_int bmp_make_rgb__1bit(u_char r, u_char g, u_char b)
{
    return ((r + g + b) > 384) ? (1) : (0); // this is a palette index!
}

//############################################################################
u_int bmp_make_rgb__4bit(u_char r, u_char g, u_char b)
{   // this returns a palette index!
    switch(r / 0x40)
    {
        case 0: // no red
                 switch(g / 0x40)
                 {
                     case 0: // no green
                     case 1: // 1/4 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  0; // 1/4 blue
                                  case 2: return  1; // 1/2 blue
                                  case 3: return  9; // all blue
                             }
                     case 2: // 1/2 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  2; // 1/4 blue
                                  case 2: return  3; // 1/2 blue
                                  case 3: return 11; // all blue
                             }
                     case 3: // all green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 10; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 11; // all blue
                             }
                 }
        case 1: // 1/4 red
                 switch(g / 0x40)
                 {
                     case 0: // no green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  0; // 1/4 blue
                                  case 2: return  1; // 1/2 blue
                                  case 3: return  9; // all blue
                             }
                     case 1: // 1/4 green
                             switch(b / 0x40)
                             {
                                  case 0: return  0; //  no blue
                                  case 1: return  8; // 1/4 blue
                                  case 2: return  1; // 1/2 blue
                                  case 3: return  9; // all blue
                             }
                     case 2: // 1/2 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  2; // 1/4 blue
                                  case 2: return  3; // 1/2 blue
                                  case 3: return 11; // all blue
                             }
                     case 3: // all green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 10; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 11; // all blue
                             }
                 }
        case 2: // 1/2 red
                 switch(g / 0x40)
                 {
                     case 0: // no green
                     case 1: // 1/4 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  4; // 1/4 blue
                                  case 2: return  5; // 1/2 blue
                                  case 3: return 13; // all blue
                             }
                     case 2: // 1/2 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return  6; // 1/4 blue
                                  case 2: return  7; // 1/2 blue
                                  case 3: return 15; // all blue
                             }
                     case 3: // all green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 14; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 15; // all blue
                             }
                 }
        case 3: // all red
                 switch(g / 0x40)
                 {
                     case 0: // no green
                     case 1: // 1/4 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 12; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 13; // all blue
                             }
                     case 2: // 1/2 green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 14; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 15; // all blue
                             }
                     case 3: // all green
                             switch(b / 0x40)
                             {
                                  case 0:            //  no blue
                                  case 1: return 14; // 1/4 blue
                                  case 2:            // 1/2 blue
                                  case 3: return 15; // all blue
                             }
                 }
    }
    return 0;
}

//############################################################################
u_int bmp_make_rgb__8bit(u_char r, u_char g, u_char b)
{   // this returns a palette index!
    return (    (r & 0xe0)
             | ((g & 0xe0) >> 3)
             | ((b & 0xc0) >> 6)
           );
}

//############################################################################
u_int bmp_make_rgb_16bit(u_char r, u_char g, u_char b)
{   // packed pixels
    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
}

//############################################################################
u_int bmp_make_rgb_32bit(u_char r, u_char g, u_char b)
{
    return   (0xff << ALPHA_BIT_SHIFT)
           | (r    << RED_BIT_SHIFT  )
           | (g    << GREEN_BIT_SHIFT)
           | (b    << BLUE_BIT_SHIFT );
}

//############################################################################
u_char bmp_r_from_rgb_16bit(u_int rgb)
{   //1111100000000000
    return ((rgb & 0xf800) >> 8) | ((rgb & 0xf800) >> 13);
}

//############################################################################
u_char bmp_r_from_rgb_32bit(u_int rgb)
{
    return (rgb & (0xff << BMP_RED_BIT_SHIFT)) >> BMP_RED_BIT_SHIFT;
}

//############################################################################
u_char bmp_r_from_palette(u_int palette_value)
{
    return ((palette_value & 0x00ff0000) >> 16);
}

//############################################################################
u_char bmp_g_from_rgb_16bit(u_int rgb)
{
    //0000011111100000
    return ((rgb & 0x07e0) >> 3) | ((rgb & 0x07e0) >> 7);
}

//############################################################################
u_char bmp_g_from_rgb_32bit(u_int rgb)
{
    return (rgb & (0xff << BMP_GREEN_BIT_SHIFT)) >> BMP_GREEN_BIT_SHIFT;
}

//############################################################################
u_char bmp_g_from_palette(u_int palette_value)
{
    return ((palette_value & 0x0000ff00) >> 8);
}

//############################################################################
u_char bmp_b_from_rgb_16bit(u_int rgb)
{
    //0000000000011111
    return ((rgb & 0x001f) << 3) | ((rgb & 0x001f) >> 2);
}

//############################################################################
u_char bmp_b_from_rgb_32bit(u_int rgb)
{
    return (rgb & (0xff << BMP_BLUE_BIT_SHIFT)) >> BMP_BLUE_BIT_SHIFT;
}

//############################################################################
u_char bmp_b_from_palette(u_int palette_value)
{
    return (palette_value & 0x000000ff);
}

//############################################################################
u_int bmp_put_pixel__1bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
         xx = x / 8;
         switch(x % 8)
         {
            case 7: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x01) | ((c & 0x01)     ); break;
            case 6: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x02) | ((c & 0x01) << 1); break;
            case 5: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x04) | ((c & 0x01) << 2); break;
            case 4: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x08) | ((c & 0x01) << 3); break;
            case 3: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x10) | ((c & 0x01) << 4); break;
            case 2: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x20) | ((c & 0x01) << 5); break;
            case 1: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x40) | ((c & 0x01) << 6); break;
            case 0: bmp->uchar_row[y][xx] = (bmp->uchar_row[y][xx] & ~0x80) | ((c & 0x01) << 7); break;
         }
         return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_put_pixel__4bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x / 2;
        bmp->uchar_row[y][xx] =    (x & 0x01) // even
                                ? ((bmp->uchar_row[y][xx] & 0xF0) |  (c & 0x0F)      )
                                : ((bmp->uchar_row[y][xx] & 0x0F) | ((c & 0x0F) << 4));
        return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_put_pixel__8bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        bmp->uchar_row[y][x] = (u_char)c;
        return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_put_pixel_16bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        bmp->ushort_row[y][x] = (u_short)c;
        return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_put_pixel_24bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x * 3;
        bmp->uchar_row[y][xx    ] = (c & 0x000000ff)       ;
        bmp->uchar_row[y][xx + 1] = (c & 0x0000ff00) >> 8  ;
        bmp->uchar_row[y][xx + 2] = (c & 0x00ff0000) >> 16 ;
        return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_put_pixel_32bit(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_int c)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        bmp->uint_row[y][x] = c;
        return 1;
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel__1bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x / 8;
        switch(x % 8)
        {
            case 0: return (bmp->uchar_row[y][xx] & 0x80) >> 7;
            case 1: return (bmp->uchar_row[y][xx] & 0x40) >> 6;
            case 2: return (bmp->uchar_row[y][xx] & 0x20) >> 5;
            case 3: return (bmp->uchar_row[y][xx] & 0x10) >> 4;
            case 4: return (bmp->uchar_row[y][xx] & 0x08) >> 3;
            case 5: return (bmp->uchar_row[y][xx] & 0x04) >> 2;
            case 6: return (bmp->uchar_row[y][xx] & 0x02) >> 1;
            case 7: return (bmp->uchar_row[y][xx] & 0x01)     ;
        }
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel__4bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x / 2;
        return     (x & 0x01) // odd
                 ? ( bmp->uchar_row[y][xx] & 0x0F)
                 : ((bmp->uchar_row[y][xx] & 0xF0) >> 4);
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel__8bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
        return (u_int)bmp->uchar_row[y][x];
    return 0;
}

//############################################################################
u_int bmp_get_pixel_16bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
        return (u_int)bmp->ushort_row[y][x];
    return 0;
}

//############################################################################
u_int bmp_get_pixel_24bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x * 3;
        return    bmp->uchar_row[y][xx    ]
               | (bmp->uchar_row[y][xx + 1] <<  8)
               | (bmp->uchar_row[y][xx + 2] << 16);
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel_32bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
        return bmp->uint_row[y][x];
    return 0;
}

//############################################################################
u_int bmp_get_pixel_rgb__1bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x / 8;
        switch(x % 8)
        {                                  // palette index 0 or 1
            case 0: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x80) != 0)];
            case 1: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x40) != 0)];
            case 2: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x20) != 0)];
            case 3: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x10) != 0)];
            case 4: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x08) != 0)];
            case 5: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x04) != 0)];
            case 6: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x02) != 0)];
            case 7: return bmp->palette[(int)((bmp->uchar_row[y][xx] & 0x01) != 0)];
        }
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel_rgb__4bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_int xx;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        xx = x / 2;
        return bmp->palette[(u_int)(    (x & 0x01) // odd
                                     ?  (bmp->uchar_row[y][xx] & 0x0F)
                                     : ((bmp->uchar_row[y][xx] & 0xF0) >> 4)
                                   )
                           ];
    }
    return 0;
}

//############################################################################
u_int bmp_get_pixel_rgb__8bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
        return bmp->palette[bmp->uchar_row[y][x]];
    return 0;
}

//############################################################################
u_int bmp_get_pixel_rgb_16bit(struct LaserBoy_bmp* bmp, u_int x, u_int y)
{
    u_char r, g, b;
    if(bmp->data && (x < bmp->xres) && (y < bmp->yres))
    {
        // 1111 1000 0000 0000
        r =   ((bmp->ushort_row[y][x] & 0xF800) >>  8)  // 1111 1000
            | ((bmp->ushort_row[y][x] & 0xF800) >> 13); // 0000 0111

        // 0000 0111 1110 0000
        g =   ((bmp->ushort_row[y][x] & 0x07e0) >>  3)  // 1111 1100
            | ((bmp->ushort_row[y][x] & 0x07e0) >>  9); // 0000 0011

        // 0000 0000 0001 1111
        b =   ((bmp->ushort_row[y][x] & 0x001f) <<  3)  // 1111 1000
            | ((bmp->ushort_row[y][x] & 0x001f) >>  2); // 0000 0111

        return   (r    << RED_BIT_SHIFT)
               | (g    << GREEN_BIT_SHIFT)
               | (b    << BLUE_BIT_SHIFT)
               | (0xff << ALPHA_BIT_SHIFT);
    }
    return 0;
}

//############################################################################
void bmp_init(struct LaserBoy_bmp* bmp, u_int x, u_int y, u_short bpp)
{
    bmp->B          = 'B';
    bmp->M          = 'M';
    bmp->reserved   = 0;
    bmp->to_end     = LASERBOY_BMP_FILE_HEADER_SIZE - 14;
    bmp->xres       = x;
    bmp->yres       = y;
    bmp->planes     = 1;
    bmp->comp       = 0;
    bmp->xppm       = 2835;
    bmp->yppm       = 2835;
    bmp->black      = 0;

    switch(bpp)
    {
        case  1: bmp->bytes_per_line    = (int)(ceil((bmp->xres * 0.125) / 4.0) * 4);
                 bmp->bpp               =   1;
                 bmp->palette_size      =   8;
                 bmp->c_used            =   2;
                 bmp->c_important       =   2;
                 break;
        // -------------------------------------------------------------------
        case  4: bmp->bytes_per_line    = (int)(ceil((bmp->xres * 0.5) / 4.0) * 4);
                 bmp->bpp               =   4;
                 bmp->palette_size      =  64;
                 bmp->c_used            =  16;
                 bmp->c_important       =  16;
                 break;
        // -------------------------------------------------------------------
        case  8: bmp->bytes_per_line    = (int)(ceil( bmp->xres / 4.0) * 4);
                 bmp->bpp               =    8;
                 bmp->palette_size      = 1024;
                 bmp->c_used            =  256;
                 bmp->c_important       =  256;
                 break;
        // -------------------------------------------------------------------
        case 16: bmp->bytes_per_line    = (int)(ceil((bmp->xres * 2.0) / 4.0) * 4);
                 bmp->bpp               =         16;
                 bmp->palette_size      =          0;
                 bmp->c_used            =          0;
                 bmp->c_important       =          0;
                 break;
        // -------------------------------------------------------------------
        case 24: bmp->bytes_per_line    = (int)(ceil((bmp->xres * 3.0) / 4.0) * 4);
                 bmp->bpp               =         24;
                 bmp->palette_size      =          0;
                 bmp->c_used            =          0;
                 bmp->c_important       =          0;
                 break;
        // -------------------------------------------------------------------
        case 32: bmp->bytes_per_line    = (int)(ceil((bmp->xres * 4.0) / 4.0) * 4);
                 bmp->bpp               =         32;
                 bmp->palette_size      =          0;
                 bmp->c_used            =          0;
                 bmp->c_important       =          0;
                 break;
        // -------------------------------------------------------------------
        default: exit(54);
        // -------------------------------------------------------------------
    }
    bmp->image_size = bmp->yres * bmp->bytes_per_line;
    bmp->jump       = LASERBOY_BMP_FILE_HEADER_SIZE + bmp->palette_size;
    bmp->file_size  = bmp->image_size + bmp->jump;
    bmp_set_functions         (bmp, bpp);
    bmp_malloc                (bmp);
    bmp_find_black_gray_white (bmp);
    return;
}

//############################################################################
void bmp_set_functions(struct LaserBoy_bmp* bmp, u_short bpp)
{
    switch(bpp)
    {
        case  1: bmp->make_rgb      = bmp_make_rgb__1bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_32bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_32bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_32bit   ;
                 bmp->put_pixel     = bmp_put_pixel__1bit    ;
                 bmp->get_pixel     = bmp_get_pixel__1bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_rgb__1bit;
                 break;
        // -------------------------------------------------------------
        case  4: bmp->make_rgb      = bmp_make_rgb__4bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_32bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_32bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_32bit   ;
                 bmp->put_pixel     = bmp_put_pixel__4bit    ;
                 bmp->get_pixel     = bmp_get_pixel__4bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_rgb__4bit;
                 break;
        // -------------------------------------------------------------
        case  8: bmp->make_rgb      = bmp_make_rgb__8bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_32bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_32bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_32bit   ;
                 bmp->put_pixel     = bmp_put_pixel__8bit    ;
                 bmp->get_pixel     = bmp_get_pixel__8bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_rgb__8bit;
                 break;
        // -------------------------------------------------------------
        case 16: bmp->make_rgb      = bmp_make_rgb_16bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_16bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_16bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_16bit   ;
                 bmp->put_pixel     = bmp_put_pixel_16bit    ;
                 bmp->get_pixel     = bmp_get_pixel_16bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_rgb_16bit;
                 break;
        // -------------------------------------------------------------
        case 24: bmp->make_rgb      = bmp_make_rgb_32bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_32bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_32bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_32bit   ;
                 bmp->put_pixel     = bmp_put_pixel_24bit    ;
                 bmp->get_pixel     = bmp_get_pixel_24bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_24bit    ;
                 break;
        // -------------------------------------------------------------
        case 32: bmp->make_rgb      = bmp_make_rgb_32bit     ;
                 bmp->r_from_rgb    = bmp_r_from_rgb_32bit   ;
                 bmp->g_from_rgb    = bmp_g_from_rgb_32bit   ;
                 bmp->b_from_rgb    = bmp_b_from_rgb_32bit   ;
                 bmp->put_pixel     = bmp_put_pixel_32bit    ;
                 bmp->get_pixel     = bmp_get_pixel_32bit    ;
                 bmp->get_pixel_rgb = bmp_get_pixel_32bit    ;
                 break;
        // -------------------------------------------------------------
        default: exit(55);
        // -------------------------------------------------------------
    }
    return;
}

//############################################################################
void bmp_malloc(struct LaserBoy_bmp* bmp)
{
    u_int y;
    bmp->data = (u_char*)malloc(bmp->palette_size + bmp->image_size);

    if(bmp->bpp <= 8)
        bmp->palette = (u_int*)bmp->data;
    else
        bmp->palette = (u_int*)NULL;

    bmp->image      = bmp->data + bmp->palette_size;
    bmp->uchar_row  = (u_char** )NULL;
    bmp->ushort_row = (u_short**)NULL;
    bmp->uint_row   = (u_int**  )NULL;

    switch(bmp->bpp)
    {
        case  1: bmp->uchar_row = (u_char**)malloc(bmp->yres * sizeof(u_char*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->uchar_row[y] = &(bmp->image[y * bmp->xres / 8]);
                 break;
        //----------------------------------------------------------------
        case  4: bmp->uchar_row = (u_char**)malloc(bmp->yres * sizeof(u_char*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->uchar_row[y] = &(bmp->image[y * bmp->xres / 2]);
                 break;
        //----------------------------------------------------------------
        case  8: bmp->uchar_row = (u_char**)malloc(bmp->yres * sizeof(u_char*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->uchar_row[y] = &(bmp->image[y * bmp->xres]);
                 break;
        //----------------------------------------------------------------
        case 16: bmp->ushort_row = (u_short**)malloc(bmp->yres * sizeof(u_short*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->ushort_row[y] = (u_short*)(&(bmp->image[y * bmp->xres * 2]));
                 break;
        //----------------------------------------------------------------
        case 24: bmp->uchar_row = (u_char**)malloc(bmp->yres * sizeof(u_char*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->uchar_row[y] = &(bmp->image[y * bmp->xres * 3]);
                 break;
        //----------------------------------------------------------------
        case 32: bmp->uint_row = (u_int**)malloc(bmp->yres * sizeof(u_int*));
                 for(y = 0; y < bmp->yres; y++)
                    bmp->uint_row[y] = (u_int*)(&(bmp->image[y * bmp->xres * 4]));
                 break;
        //----------------------------------------------------------------
        default: exit(10);
    }
    return;
}

//############################################################################
void bmp_find_black_gray_white(struct LaserBoy_bmp* bmp)
{
    bmp->black = bmp->make_rgb(  0,   0,   0);
    bmp->gray  = bmp->make_rgb(127, 127, 127);
    bmp->white = bmp->make_rgb(255, 255, 255);
    return;
}

//############################################################################
long double bmp_put_line(struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c)
{
    int          n;
    long double  m,
                 b,
                 dx = x2 - x1,
                 dy = y2 - y1;
    //------------------------------------------------------------------------
    if(0.0 == dx)
    {
        if(dy < 0.0)
            for(n = y1; n >= y2; n--)
                bmp->put_pixel(bmp, x1, n, c);
        else
            for(n = y1; n <= y2; n++)
                bmp->put_pixel(bmp, x1, n, c);
    }
    //------------------------------------------------------------------------
    else if(0.0 == dy)
    {
        if(dx < 0.0)
            for(n = x1; n >= x2; n--)
                bmp->put_pixel(bmp, n, y1, c);
        else
            for(n = x1; n <= x2; n++)
                bmp->put_pixel(bmp, n, y1, c);
    }
    //------------------------------------------------------------------------
    else if(fabs(dx) >= fabs(dy))
    {
        m = dy / dx;
        b = y1 - m * x1;
        if(dx < 0.0)
            for(n = x1; n >= x2; n--)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
        else
            for(n = x1; n <= x2; n++)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
    }
    //------------------------------------------------------------------------
    else
    {
        m = dx / dy;
        b = x1 - m * y1;
        if(dy < 0.0)
            for(n = y1; n >= y2; n--)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
        else
            for(n = y1; n <= y2; n++)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
    }
    //------------------------------------------------------------------------
    return sqrt((dx * dx) + (dy * dy)); // magnitude of line
}

//############################################################################
void bmp_put_line_fast(struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c)
{
    int    n;
    double m,
           b,
           dx = x2 - x1,
           dy = y2 - y1;
    //------------------------------------------------------------------------
    if(0.0 == dx)
    {
        if(dy < 0.0)
            for(n = y1; n >= y2; n--)
                bmp->put_pixel(bmp, x1, n, c);
        else
            for(n = y1; n <= y2; n++)
                bmp->put_pixel(bmp, x1, n, c);
    }
    //------------------------------------------------------------------------
    else if(0.0 == dy)
    {
        if(dx < 0.0)
            for(n = x1; n >= x2; n--)
                bmp->put_pixel(bmp, n, y1, c);
        else
            for(n = x1; n <= x2; n++)
                bmp->put_pixel(bmp, n, y1, c);
    }
    //------------------------------------------------------------------------
    else if(fabs(dx) >= fabs(dy))
    {
        m = dy / dx;
        b = y1 - m * x1;
        if(dx < 0.0)
            for(n = x1; n >= x2; n--)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
        else
            for(n = x1; n <= x2; n++)
                bmp->put_pixel(bmp, n, (int)(n * m + b + 0.5), c);
    }
    //------------------------------------------------------------------------
    else
    {
        m = dx / dy;
        b = x1 - m * y1;
        if(dy < 0.0)
            for(n = y1; n >= y2; n--)
                bmp->put_pixel(bmp, (int)(n * m + b + 0.5), n, c);
        else
            for(n = y1; n <= y2; n++)
                bmp->put_pixel(bmp, (int)(n * m + b + 0.5), n, c);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
long double bmp_put_ray(struct LaserBoy_bmp* bmp, u_int x, u_int y, long double r, long double t, u_int c)
{
    return bmp_put_line(bmp,
                        x,
                        y,
                        (int)(x + r * cos(t)),
                        (int)(y + r * sin(t)),
                        c
                       );
}

//############################################################################
void bmp_put_ray_fast(struct LaserBoy_bmp* bmp, u_int x, u_int y, long double r, long double t, u_int c)
{
    bmp_put_line_fast(bmp,
                      x,
                      y,
                      (int)(x + r * cos(t)),
                      (int)(y + r * sin(t)),
                      c
                     );
    return;
}

//############################################################################
void bmp_put_rectangle(struct LaserBoy_bmp* bmp, int x1, int y1, int x2, int y2, u_int c, int filled)
{
    if(filled)
    {
        int y, step;
        step = (y1 <= y2) ? 1 : -1;
        if((x1 != x2) && (y1 != y2))
            for(y = y1; y <= y2; y += step)
                bmp_put_line_fast(bmp, x1, y, x2, y, c);
        else if(x1 != x2)
            bmp_put_line_fast(bmp, x1, y1, x2, y1, c);
        else
            bmp->put_pixel(bmp, x1, y1, c);
    }
    else
    {
        if((x1 != x2) && (y1 != y2))
        {
            bmp_put_line_fast(bmp, x1, y1, x2, y1, c);
            bmp_put_line_fast(bmp, x1, y2, x2, y2, c);
            bmp_put_line_fast(bmp, x1, y1, x1, y2, c);
            bmp_put_line_fast(bmp, x2, y1, x2, y2, c);
        }
        else if(x1 != x2)
            bmp_put_line_fast(bmp, x1, y1, x2, y1, c);
        else
            bmp->put_pixel(bmp, x1, y1, c);
    }
    return;
}

//############################################################################
void bmp_put_vertex(struct LaserBoy_bmp* bmp, int x, int y, u_int c)
{
    bmp_put_line_fast(bmp, x - 1, y - 1, x + 1, y - 1, c);
    bmp_put_line_fast(bmp, x - 1, y + 1, x + 1, y + 1, c);
    bmp_put_line_fast(bmp, x + 1, y + 1, x + 1, y - 1, c);
    bmp_put_line_fast(bmp, x - 1, y + 1, x - 1, y - 1, c);
    return;
}

//############################################################################
void bmp_put_select(struct LaserBoy_bmp* bmp, int x, int y, u_int c)
{
    bmp_put_line_fast(bmp, x - 3, y - 3, x + 3, y - 3, c);
    bmp_put_line_fast(bmp, x - 3, y + 3, x + 3, y + 3, c);
    bmp_put_line_fast(bmp, x + 3, y + 3, x + 3, y - 3, c);
    bmp_put_line_fast(bmp, x - 3, y + 3, x - 3, y - 3, c);
    return;
}

//############################################################################
int bmp_put_byte_pattern(struct LaserBoy_bmp* bmp,
                         u_char byte,
                         int    x,
                         int    y,
                         u_int  fc,
                         u_int  bc,
                         u_int  dir,
                         u_int  size
                        )
{
    u_int   i,
            j,
            ret = 0;

    switch(dir)
    {
        default:
        case 0:
        case 2: for(i = 0; i < 8; i++)
                    for(j = 0; j < size; j++)
                        ret = bmp->put_pixel(bmp, x + i * size + j, y, (byte & (1 << (7 - i))) ? fc : bc);
                return x + 8 * size * ret;
        // --------------------------------------------------------------------------
        case 1:
        case 3: for(i = 0; i < 8; i++)
                    for(j = 0; j < size; j++)
                        ret = bmp->put_pixel(bmp, x, y + i * size + j, (byte & (1 << (7 - i))) ? fc : bc);
                return y + 8 * size * ret;
        // --------------------------------------------------------------------------
        case 4:
        case 6: for(i = 0; i < 8; i++)
                    for(j = 0; j < size; j++)
                        ret = (byte & (1 << (7 - i))) ? bmp->put_pixel(bmp, x + i * size + j, y, fc) : 1;
                return x + 8 * size * ret;
        // --------------------------------------------------------------------------
        case 5:
        case 7: for(i = 0; i < 8; i++)
                    for(j = 0; j < size; j++)
                        ret = (byte & (1 << (7 - i))) ? bmp->put_pixel(bmp, x, y + i * size + j, fc) : 1;
                return y + 8 * size * ret;
        // --------------------------------------------------------------------------
    }
}

//############################################################################
int bmp_put_ascii(struct LaserBoy_bmp* bmp,
                  u_char ascii,
                  int    x,
                  int    y,
                  u_int  fc,
                  u_int  bc,
                  u_int  dir,
                  u_int  size
                 )
{
    u_int    i,
             j,
             ret = 0;

    switch(dir)
    {
        default:
        case  0:
        case  4:
                    for(i = 0; i < 8; i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_byte_pattern(bmp, fontdata_8x8[ascii * 8 + i], x, y + i * size + j, fc, bc, dir, size);
                    break;
        // --------------------------------------------------------------------------
        case  1:
        case  5:    for(i = 0; i < 8; i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_byte_pattern(bmp, fontdata_8x8[ascii * 8 + (7 - i)], x + i * size + j, y, fc, bc, dir, size);
                    break;
        // --------------------------------------------------------------------------
        case  2:
        case  6:    for(i = 0; i < 8; i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_byte_pattern(bmp, fontdata_8x8[ascii * 8 + i], x, y - i * size - j, fc, bc, dir, size);
                    break;
        // --------------------------------------------------------------------------
        case  3:
        case  7:    for(i = 0; i < 8; i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_byte_pattern(bmp, fontdata_8x8[ascii * 8 + i], x + i * size + j, y, fc, bc, dir, size);
                    break;
        // --------------------------------------------------------------------------
    }
    return ret;
}

//############################################################################
int bmp_put_string(struct LaserBoy_bmp* bmp,
                   char* str,
                   int   x,
                   int   y,
                   u_int fc,
                   u_int bc,
                   u_int dir,
                   u_int size
                  )
{
    u_int i,
          j,
          ret = 0;

    switch(dir)
    {
        default:
        case  0:
        case  4:    for(i = 0; i < strlen(str); i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_ascii(bmp, str[i], x + i * 8 * size, y + j, fc, bc, dir, size);
                    break;
        // ----------------------------------------------------------------------
        case  1:
        case  5:    for(i = 0; i < strlen(str); i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_ascii(bmp, str[i], x + j, y + i * 8 * size, fc, bc, dir, size);
                    break;
        // ----------------------------------------------------------------------
        case  2:
        case  6:    for(i = 0; i < strlen(str); i++)
                         for(j = 0; j < size; j++)
                           ret = bmp_put_ascii(bmp, str[i], x - i * 8 * size, y + j, fc, bc, dir, size);
                    break;
        // ----------------------------------------------------------------------
        case  3:
        case  7:    for(i = 0; i < strlen(str); i++)
                        for(j = 0; j < size; j++)
                            ret = bmp_put_ascii(bmp, str[i], x + j, y - i * 8 * size, fc, bc, dir, size);
                    break;
        // ----------------------------------------------------------------------
    }
    return ret;
}

//############################################################################
int bmp_printf(struct LaserBoy_bmp* bmp,
               int   x,
               int   y,
               u_int fc,
               u_int bc,
               u_int dir,
               u_int size,
               const char* text_format,
               ...
              )
{
    char formatted_text[1024];
    va_list var_arg_list;
    va_start(var_arg_list, text_format);
    vsprintf(formatted_text, text_format, var_arg_list);
    va_end(var_arg_list);
    return bmp_put_string(bmp, formatted_text, x, y, fc, bc, dir, size);
}


//############################################################################
void bmp_dump_palette(struct LaserBoy_bmp* bmp)
{
    u_int i;
    if(bmp->bpp <= 8)
    {
        for(i = 0; i < (bmp->palette_size / sizeof(u_int)); i++)
            printf("\n%3d : a %2X : r %2X : g %2X : b %2X",
                   i,
                   (bmp->palette[i] & 0xff000000) >> 24,
                   (bmp->palette[i] & 0x00ff0000) >> 16,
                   (bmp->palette[i] & 0x0000ff00) >>  8,
                   (bmp->palette[i] & 0x000000ff)
                  );
        printf("\n\n");
    }
    else
        printf( "\ntrue color image has no palette\n");
    return;
}

//############################################################################
void bmp_dump_image(struct LaserBoy_bmp* bmp)
{
    u_int i;
    printf("\nBITMAP %d x %d : %d bytes:", bmp->xres, bmp->yres, bmp->image_size);
    for(i = 0; i < bmp->image_size; i++)
    {
        if(0 == (i % 64))
        {
            printf("\n");
        }
        printf(" %2X" , bmp->image[i]);
    }
    printf("\n\n");
    return;
}

//############################################################################
void bmp_clear(struct LaserBoy_bmp* bmp, u_char c)
{
    memset((void*)bmp->image, c, bmp->image_size);
    return;
}

//############################################################################
void bmp_fill(struct LaserBoy_bmp* bmp, u_char r, u_char g, u_char b)
{
    u_int x, y;
    for(y = 0; y < bmp->yres; y++)
        for(x = 0; x < bmp->xres; x++)
            bmp->put_pixel(bmp, x, y, bmp->make_rgb(r, g, b));
}

//############################################################################
void bmp_copy(struct LaserBoy_bmp* target, struct LaserBoy_bmp* original)
{
    if(    target->bpp  == original->bpp
        && target->xres == original->xres
        && target->yres == original->yres
      )
    {
        if(original->bpp <= 8)
            memcpy((void*)target->palette, (void*)original->palette, original->palette_size);
        memcpy((void*)target->image, (void*)original->image, original->image_size);
    }
    else
    {
        bmp_free(target);
        bmp_init(target, original->xres, original->yres, original->bpp);
        if(original->bpp <= 8)
            memcpy((void*)target->palette, (void*)original->palette, original->palette_size);
        memcpy((void*)target->image, (void*)original->image, original->image_size);
    }
    return;
}

//############################################################################
void bmp_flip(struct LaserBoy_bmp* bmp)
{
    u_int x, y;
    struct LaserBoy_bmp flipped = {0};

    bmp_init(&flipped, bmp->xres, bmp->yres, bmp->bpp);
    if(bmp->bpp <= 8)
        memcpy((void*)flipped.palette, (void*)bmp->palette, bmp->palette_size);
    for(y = 0; y < bmp->yres; y++)
        for(x = 0; x < bmp->xres; x++)
            bmp->put_pixel(&flipped, x, flipped.yres - y - 1, bmp->get_pixel(bmp, x, y));
    bmp_copy(bmp, &flipped);
    return;
}

//############################################################################
void bmp_into_bmp(struct LaserBoy_bmp* target,
                  struct LaserBoy_bmp* insert,
                  int    scale
                 )
{
    int x,
        y,
        x_off  = 0,
        y_off  = 0,
        last_x = target->xres,
        last_y = target->yres;
    //------------------------------------------------------------------------
    if(    target->yres >= (insert->xres * scale)
        && target->yres >= (insert->yres * scale)
      ) // insert * scale is smaller
    {
        last_x = insert->xres * scale;
        last_y = insert->yres * scale;
        x_off  = (target->yres - (insert->xres * scale)) / 2; // centers it in the left square of the display
        y_off  = (target->yres - (insert->yres * scale)) / 2;
    }
    else if(target->yres >= (insert->xres * scale)) // insert * scale is narrower
    {
        last_x = insert->xres * scale;
        x_off  = (target->yres - (insert->xres * scale)) / 2;
    }
    else if(target->yres >= (insert->yres * scale)) // insert * scale is shorter
    {
        last_y = insert->yres * scale;
        y_off  = (target->yres - (insert->yres * scale)) / 2;
    }
    //------------------------------------------------------------------------
    for(y = 0; y < last_y; y += scale)
        for(x = 0; x < last_x; x += scale)

            if(scale > 1)
                bmp_put_rectangle(target,
                                  x + x_off,
                                  y + y_off,
                                  x + x_off + scale - 1,
                                  y + y_off + scale - 1,
                                  target->make_rgb(insert->r_from_rgb(insert->get_pixel_rgb(insert, x / scale, y / scale)),
                                                   insert->g_from_rgb(insert->get_pixel_rgb(insert, x / scale, y / scale)),
                                                   insert->b_from_rgb(insert->get_pixel_rgb(insert, x / scale, y / scale))
                                                  ),
                                  1
                                 );
            else
                target->put_pixel(target,
                                  x + x_off,
                                  y + y_off,
                                  target->make_rgb(insert->r_from_rgb(insert->get_pixel_rgb(insert, x, y)),
                                                   insert->g_from_rgb(insert->get_pixel_rgb(insert, x, y)),
                                                   insert->b_from_rgb(insert->get_pixel_rgb(insert, x, y))
                                                  )
                                 );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void bmp_free(struct LaserBoy_bmp* bmp)
{
    if(bmp->data     )    free(bmp->data     );
    if(bmp->uchar_row)    free(bmp->uchar_row);
    if(bmp->uint_row )    free(bmp->uint_row );
    bmp->data      = NULL;
    bmp->palette   = NULL;
    bmp->image     = NULL;
    bmp->uchar_row = NULL;
    bmp->uint_row  = NULL;
    return;
}

//############################################################################









//############################################################################
// BITMAP MEMORY AND FILE
//############################################################################
int bmp_from_file(struct LaserBoy_bmp* bmp, char* file_name)
{
    if(bmp_open_for_read(bmp, file_name))
    {
        if(bmp_read_file_header(bmp))
        {
            bmp_set_functions (bmp, bmp->bpp);
            bmp_malloc        (bmp);
            if(bmp_read_file_palette(bmp))
                bmp_read_file_image(bmp);
        }
        close(bmp->fd);
        return 1;
    }
    return 0;
}

//############################################################################
int bmp_open_for_read(struct LaserBoy_bmp* bmp, char* file_name)
{
    if(0 > (bmp->fd = open(file_name, O_RDONLY | O_BINARY))) // open the bitmap file
    {
        perror("\nERROR: opening bmp file for reading failed");
        return 0;
    }
    return 1;
}

//############################################################################
int bmp_read_file_header(struct LaserBoy_bmp* bmp)
{
    int bytes_read = 0;
    // file descriptor must be open, valid and in bmp->fd
    lseek(bmp->fd, 0, SEEK_SET); // start at the top
    bytes_read += read(bmp->fd, (void*)(&bmp->B           ), sizeof(char) );
    bytes_read += read(bmp->fd, (void*)(&bmp->M           ), sizeof(char) );
    bytes_read += read(bmp->fd, (void*)(&bmp->file_size   ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->reserved    ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->jump        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->to_end      ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->xres        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->yres        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->planes      ), sizeof(short));
    bytes_read += read(bmp->fd, (void*)(&bmp->bpp         ), sizeof(short));
    bytes_read += read(bmp->fd, (void*)(&bmp->comp        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->image_size  ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->xppm        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->yppm        ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->c_used      ), sizeof(int)  );
    bytes_read += read(bmp->fd, (void*)(&bmp->c_important ), sizeof(int)  );

    bmp->bytes_per_line = (int)(ceil((bmp->bpp / 8.0) * bmp->xres / 4.0) * 4);
    bmp->palette_size   = bmp->jump - LASERBOY_BMP_FILE_HEADER_SIZE;
    bmp->black          = 0;
    bmp->white          = 0;

    return (bytes_read == LASERBOY_BMP_FILE_HEADER_SIZE);
}

//############################################################################
int bmp_read_file_palette(struct LaserBoy_bmp* bmp)
{
    if(bmp->data && bmp->bpp <= 8)
    {
        lseek(bmp->fd, 14 + bmp->to_end, SEEK_SET);
        return (    bmp->palette_size
                 == (u_int)read(bmp->fd, (void*)(bmp->palette), bmp->palette_size)
               );
    }
    if(bmp->data && bmp->bpp == 24)
    {
        bmp_find_black_gray_white(bmp);
        return 1;
    }
    return 0;
}

//############################################################################
int bmp_read_file_image(struct LaserBoy_bmp* bmp)
{
    if(bmp->data)
    {
        u_int bytes_read = 0,
              i;
        lseek(bmp->fd, bmp->jump, SEEK_SET);
        switch(bmp->bpp)
        {
            case  1: for(i = 0; i < bmp->yres; i++)
                     {
                         bytes_read += read(bmp->fd, (void*)(bmp->image + bytes_read), bmp->xres / 8);
                         lseek(bmp->fd, bmp->bytes_per_line - bmp->xres / 8, SEEK_CUR);
                     }
                     break;
            // -------------------------------------------------------------------
            case  4: for(i = 0; i < bmp->yres; i++)
                     {
                         bytes_read += read(bmp->fd, (void*)(bmp->image + bytes_read), bmp->xres / 2);
                         lseek(bmp->fd, bmp->bytes_per_line - bmp->xres / 2, SEEK_CUR);
                     }
                     break;
            // -------------------------------------------------------------------
            case  8: for(i = 0; i < bmp->yres; i++)
                     {
                         bytes_read += read(bmp->fd, (void*)(bmp->image + bytes_read), bmp->xres);
                         lseek(bmp->fd, bmp->bytes_per_line - bmp->xres, SEEK_CUR);
                     }
                     break;
            // -------------------------------------------------------------------
            case 24: for(i = 0; i < bmp->yres; i++)
                     {
                         bytes_read += read(bmp->fd, (void*)(bmp->image + bytes_read), bmp->xres * 3);
                         lseek(bmp->fd, bmp->bytes_per_line - bmp->xres * 3, SEEK_CUR);
                     }
                     break;
            // -------------------------------------------------------------------
        }
        return (bmp->image_size == bytes_read);
    }
    return 0;
}

//############################################################################
int bmp_open_for_write(struct LaserBoy_bmp* bmp, char* file_name)
{
#ifdef WIN32
    if(0 > (bmp->fd = open(file_name, (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY))))
#else
    if(0 > (bmp->fd = open(file_name,
                           (O_WRONLY | O_CREAT),
                           (   S_IRWXU // owner rwx
                             | S_IRGRP // group r
                             | S_IWGRP // group w
                             | S_IROTH // other r
                             | S_IWOTH // other w
                           )  // mode 755
                          )
           )
      )
#endif
    {
        perror("\nERROR: opening bmp file for writing failed");
        return 0;
    }
    return 1;
}

//############################################################################
int bmp_write_file_header(struct LaserBoy_bmp* bmp)
{
    int bytes_written = 0;
    if(bmp->fd)
    {
        bytes_written += write(bmp->fd, (void*)(&bmp->B           ), sizeof(char) );
        bytes_written += write(bmp->fd, (void*)(&bmp->M           ), sizeof(char) );
        bytes_written += write(bmp->fd, (void*)(&bmp->file_size   ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->reserved    ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->jump        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->to_end      ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->xres        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->yres        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->planes      ), sizeof(short));
        bytes_written += write(bmp->fd, (void*)(&bmp->bpp         ), sizeof(short));
        bytes_written += write(bmp->fd, (void*)(&bmp->comp        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->image_size  ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->xppm        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->yppm        ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->c_used      ), sizeof(int)  );
        bytes_written += write(bmp->fd, (void*)(&bmp->c_important ), sizeof(int)  );
    }
    return (bytes_written == (14 + bmp->to_end));
}

//############################################################################
int bmp_write_palette(struct LaserBoy_bmp* bmp)
{
    return (    bmp->palette_size
             == (u_int)write(bmp->fd, (void*)(bmp->palette), bmp->palette_size));
}

//############################################################################
int bmp_write_image(struct LaserBoy_bmp* bmp)
{
    u_int   yy,
            j,
            xx,
            bytes_written  = 0,
            zero           = 0;
    u_char  c;

    switch(bmp->bpp)
    {
        case  1:    for(yy = 0; yy < bmp->yres; yy++)
                    {
                        bytes_written += write(   bmp->fd
                                                , (void*)&(bmp->image[yy * bmp->xres / 8])
                                                , bmp->bytes_per_line
                                              );
                        bytes_written += write(   bmp->fd
                                                , (void*)&(zero)
                                                , (bmp->bytes_per_line - bmp->xres / 8)
                                              );
                    }
                    break;
        // -------------------------------------------------------------------
        case  4:    for(yy = 0; yy < bmp->yres; yy++)
                    {
                        for(xx = 0; xx < bmp->xres / 2; xx++)
                        {
                            j = yy * bmp->xres / 2 + xx;
                            c =   ((bmp->image[j] & 0x0F) << 4)
                                + ((bmp->image[j] & 0xF0) >> 4); // nibble swap!
                            bytes_written += write(   bmp->fd
                                                    , (void*)&c
                                                    , sizeof(char)
                                                  );
                        }
                        bytes_written += write(   bmp->fd
                                                , (void*)&(zero)
                                                , (bmp->bytes_per_line - bmp->xres / 2)
                                              );
                    }
                    break;
        // -------------------------------------------------------------------
        case  8:    for(yy = 0; yy < bmp->yres; yy++)
                    {
                        bytes_written += write(   bmp->fd
                                                , (void*)&(bmp->image[yy * bmp->xres])
                                                , bmp->bytes_per_line
                                              );
                        bytes_written += write(   bmp->fd
                                                , (void*)&(zero)
                                                , (bmp->bytes_per_line - bmp->xres)
                                              );
                    }
                    break;
        // -------------------------------------------------------------------
        case 24:    for(yy = 0; yy < bmp->yres; yy++)
                    {
                        bytes_written += write(   bmp->fd
                                                , (void*)&(bmp->image[yy * bmp->xres * 3])
                                                , bmp->bytes_per_line
                                              );
                        bytes_written += write(   bmp->fd
                                                , (void*)&(zero)
                                                , bmp->bytes_per_line - bmp->xres * 3
                                              );
                    }
                    break;
        // -------------------------------------------------------------------
    }
    return (bytes_written == bmp->image_size);
}

//############################################################################
int bmp_save_as(struct LaserBoy_bmp* bmp, char* file_name)
{
    bmp_open_for_write    (bmp, file_name);
    bmp_write_file_header (bmp           );
    bmp_write_palette     (bmp           );
    bmp_write_image       (bmp           );
    close                 (bmp->fd       );
    return 1;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
