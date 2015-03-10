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
// LaserBoy_wave.cpp is part of LaserBoy.
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
#include <LaserBoy/wave.hpp>

namespace LaserBoy {

//############################################################################
LaserBoy_wave_header::LaserBoy_wave_header(const LaserBoy_wave_header& header)
  : num_channels      (header.num_channels      ),
    bits_per_sample   (header.bits_per_sample   ),
    num_samples       (header.num_samples       ),
    sample_rate       (header.sample_rate       ),
    num_frames        (header.num_frames        ),
    LaserBoy_wave_mode(header.LaserBoy_wave_mode),
    version           (header.version           ),
    parms             (                         )
{
    memcpy((void*)offset,     (void*)header.offset,     header.num_channels * sizeof(int)   );
    memcpy((void*)resolution, (void*)header.resolution, header.num_channels * sizeof(u_char));
    memcpy((void*)signal_id,  (void*)header.signal_id,  header.num_channels * sizeof(short) );
    memcpy((void*)LSB_tag,    (void*)header.LSB_tag,    header.num_channels * sizeof(short) );
    memcpy((void*)color_rescale_r, (void*)header.color_rescale_r, 256 * sizeof(short));
    memcpy((void*)color_rescale_g, (void*)header.color_rescale_g, 256 * sizeof(short));
    memcpy((void*)color_rescale_b, (void*)header.color_rescale_b, 256 * sizeof(short));
    memcpy((void*)color_rescale_i, (void*)header.color_rescale_i, 256 * sizeof(short));
    parms.lit_dwell_overhang     = header.parms.lit_dwell_overhang    ;
    parms.lit_delta_max          = header.parms.lit_delta_max         ;
    parms.blank_delta_max        = header.parms.blank_delta_max       ;
    parms.max_dwell_microsec     = header.parms.max_dwell_microsec    ;
    parms.max_wag_microsec       = header.parms.max_wag_microsec      ;
    parms.insignificant_distance = header.parms.insignificant_distance;
    parms.insignificant_angle    = header.parms.insignificant_angle   ;
    parms.frames_per_second      = header.parms.frames_per_second     ;
}

//############################################################################
LaserBoy_wave_header::LaserBoy_wave_header(const LaserBoy_wave_header& header, int)
  : num_channels      (8                        ),
    bits_per_sample   (header.bits_per_sample   ),
    num_samples       (header.num_samples       ),
    sample_rate       (header.sample_rate       ),
    num_frames        (header.num_frames        ),
    LaserBoy_wave_mode(header.LaserBoy_wave_mode),
    version           (header.version           ),
    parms             (                         )
{
    memcpy((void*)offset,     (void*)header.offset,     header.num_channels * sizeof(int)   );
    memcpy((void*)resolution, (void*)header.resolution, header.num_channels * sizeof(u_char));
    memcpy((void*)signal_id,  (void*)header.signal_id,  header.num_channels * sizeof(short) );
    memcpy((void*)LSB_tag,    (void*)header.LSB_tag,    header.num_channels * sizeof(short) );
    memcpy((void*)color_rescale_r, (void*)header.color_rescale_r, 256 * sizeof(short));
    memcpy((void*)color_rescale_g, (void*)header.color_rescale_g, 256 * sizeof(short));
    memcpy((void*)color_rescale_b, (void*)header.color_rescale_b, 256 * sizeof(short));
    memcpy((void*)color_rescale_i, (void*)header.color_rescale_i, 256 * sizeof(short));
    parms.lit_dwell_overhang     = header.parms.lit_dwell_overhang    ;
    parms.lit_delta_max          = header.parms.lit_delta_max         ;
    parms.blank_delta_max        = header.parms.blank_delta_max       ;
    parms.max_dwell_microsec     = header.parms.max_dwell_microsec    ;
    parms.max_wag_microsec       = header.parms.max_wag_microsec      ;
    parms.insignificant_distance = header.parms.insignificant_distance;
    parms.insignificant_angle    = header.parms.insignificant_angle   ;
    parms.frames_per_second      = header.parms.frames_per_second     ;

    offset    [6] = 0;
    offset    [7] = 0;
    resolution[6] = 16;
    resolution[7] = 16;
    signal_id [6] = LASERBOY_SIGNAL_AUDIO_LEFT ;
    signal_id [7] = LASERBOY_SIGNAL_AUDIO_RIGHT;
    LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    LSB_tag   [7] = LASERBOY_LSB_NOT_USED;
}

//############################################################################
LaserBoy_wave_header::LaserBoy_wave_header(fstream& in)
  : num_channels       (0),
    bits_per_sample    (0),
    num_samples        (0),
    sample_rate        (0),
    num_frames         (0),
    LaserBoy_wave_mode (LASERBOY_WAVE_NO_MODE),
    version            ("!LaserBoy!"),
    parms              ()
{
    u_int i;
    for(i = 0; i < 8; i++)
    {
        offset    [i] = 0;
        signal_id [i] = LASERBOY_SIGNAL_UNDEFINED;
        LSB_tag   [i] = LASERBOY_LSB_NOT_USED;
        resolution[i] = 16;
    }
    for(i = 0; i < 256; i++)
    {
        color_rescale_r[i] =
        color_rescale_g[i] =
        color_rescale_b[i] =
        color_rescale_i[i] = 0;
    }
    from_fstream_wave(in);
}

//############################################################################
bool LaserBoy_wave_header::from_fstream_wave(fstream& in)
{
    char    a, b, c, d;

    short   audio_format,
            block_align;

    u_int   i,
            chunk_size          = 0,
            sub_chunk_1_size    = 0,
            sub_chunk_2_size    = 0,
            sub_chunk_3_size    = 0,
            LaserBoy_chunk_size = 0,
            byte_rate;

    string  temp_string,
            version_guess;
    //------------------------------------------------------------------------
    version            = "!LaserBoy!"; // assume the worst!
    version_guess      = "!LaserBoy!";
    LaserBoy_wave_mode =   LASERBOY_WAVE_POSITIVE;
    //------------------------------------------------------------------------
    in.get(a); if(a != 'R') return false;
    in.get(a); if(a != 'I') return false;
    in.get(a); if(a != 'F') return false;
    in.get(a); if(a != 'F') return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    chunk_size = (   (a & 0x000000ff)
                   | (b & 0x000000ff) << 8
                   | (c & 0x000000ff) << 16
                   | (d & 0x000000ff) << 24
                 );
    //------------------------------------------------------------------------
    in.get(a); if(a != 'W') return false;
    in.get(a); if(a != 'A') return false;
    in.get(a); if(a != 'V') return false;
    in.get(a); if(a != 'E') return false;
    //------------------------------------------------------------------------
    in.get(a);
    if(a == 'f')
    {
        in.get(a); if(a != 'm') return false;
        in.get(a); if(a != 't') return false;
        in.get(a); if(a != ' ') return false;
    }
    else if(a == 'L')
    {
        in.get(a); if(a != 'I') return false;
        in.get(a); if(a != 'S') return false;
        in.get(a); if(a != 'T') return false;
        in.get(a); in.get(b); in.get(c); in.get(d);
        sub_chunk_3_size = (   (a & 0x000000ff)
                             | (b & 0x000000ff) << 8
                             | (c & 0x000000ff) << 16
                             | (d & 0x000000ff) << 24
                           );
        in.seekg(sub_chunk_3_size, ios::cur);
        in.get(a); if(a != 'f') return false;
        in.get(a); if(a != 'm') return false;
        in.get(a); if(a != 't') return false;
        in.get(a); if(a != ' ') return false;
    }
    else
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sub_chunk_1_size = (   (a & 0x000000ff)
                         | (b & 0x000000ff) << 8
                         | (c & 0x000000ff) << 16
                         | (d & 0x000000ff) << 24
                       );
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    audio_format = ((b & 0x00ff) << 8 | (a & 0x00ff));
    if(audio_format != 1) return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    num_channels = ((b & 0x00ff) << 8 | (a & 0x00ff));
    if(num_channels < 2)
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sample_rate = (   (a & 0x000000ff)
                    | (b & 0x000000ff) << 8
                    | (c & 0x000000ff) << 16
                    | (d & 0x000000ff) << 24
                  );
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    byte_rate = (   (a & 0x000000ff)
                  | (b & 0x000000ff) << 8
                  | (c & 0x000000ff) << 16
                  | (d & 0x000000ff) << 24
                );
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    block_align = ((b & 0x00ff) << 8 | (a & 0x00ff));
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    bits_per_sample = ((b & 0x00ff) << 8 | (a & 0x00ff));
    //------------------------------------------------------------------------
    if(byte_rate != (sample_rate * num_channels * (bits_per_sample / 8)))
        return false;
    //------------------------------------------------------------------------
    if(block_align != (num_channels * (bits_per_sample / 8)))
        return false;
    //------------------------------------------------------------------------
    // look for LaserBoy
    //------------------------------------------------------------------------
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    //------------------------------------------------------------------------
    if(temp_string == "LBoy")
    {
        //--------------------------------------------------------------------
        in.get(a); in.get(b); in.get(c); in.get(d);
        LaserBoy_chunk_size = (   (a & 0x000000ff)
                                | (b & 0x000000ff) << 8
                                | (c & 0x000000ff) << 16
                                | (d & 0x000000ff) << 24
                              );
        //--------------------------------------------------------------------
        temp_string.clear();
        in.get(a);    temp_string.push_back(a); // L
        in.get(a);    temp_string.push_back(a); // a
        in.get(a);    temp_string.push_back(a); // s
        in.get(a);    temp_string.push_back(a); // e
        in.get(a);    temp_string.push_back(a); // r
        in.get(a);    temp_string.push_back(a); // B
        in.get(a);    temp_string.push_back(a); // o
        in.get(a);    temp_string.push_back(a); // y
    }
    else if(temp_string == "Lase")
    {
        in.get(a);    temp_string.push_back(a); // r
        in.get(a);    temp_string.push_back(a); // B
        in.get(a);    temp_string.push_back(a); // o
        in.get(a);    temp_string.push_back(a); // y
    }
    else
        in.seekg(-4, ios::cur); // Oops! put it back!

    if(temp_string == "LaserBoy")
    {
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        //--------------------------------------------------------------------
        if(    temp_string != "LaserBoy06072008"
            && temp_string != "LaserBoy07132008"
            && temp_string != "LaserBoy10102008"
            && temp_string != "LaserBoy11112008"
            && temp_string != "LaserBoy12082009"
            && temp_string != LASERBOY_WAVE_VERSION
          )
            return false;
        //--------------------------------------------------------------------
        version_guess = temp_string;
        //--------------------------------------------------------------------
        in.get(a); in.get(b); in.get(c); in.get(d);
        LaserBoy_wave_mode = (   (a & 0x000000ff)
                               | (b & 0x000000ff) << 8
                               | (c & 0x000000ff) << 16
                               | (d & 0x000000ff) << 24
                             );
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS)
            for(i = 0; i < num_channels; i++)
            {
                in.get(a); in.get(b); in.get(c); in.get(d);
                offset[i] = (   (a & 0x000000ff)
                              | (b & 0x000000ff) << 8
                              | (c & 0x000000ff) << 16
                              | (d & 0x000000ff) << 24
                            );
            }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            in.get(a); in.get(b); in.get(c); in.get(d);
            parms.lit_dwell_overhang = (   (a & 0x000000ff)
                                         | (b & 0x000000ff) << 8
                                         | (c & 0x000000ff) << 16
                                         | (d & 0x000000ff) << 24
                                       );
            //----------------------------------------------------------------
            if(version_guess == "LaserBoy06072008")
            {   // floats were stored backwards. Oops!
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                parms.blank_delta_max = parms.lit_delta_max;
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            }
            //----------------------------------------------------------------
            else if(version_guess == "LaserBoy10102008")
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                parms.blank_delta_max = parms.lit_delta_max;
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            }
            //----------------------------------------------------------------
            else if(version_guess == "LaserBoy12082009")
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.blank_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            }
            //----------------------------------------------------------------
            else // current version
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.blank_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_wag_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            }
        }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
            for(i = 0; i < num_channels; i++)
            {
                in.get(a); in.get(b);
                signal_id[i] = ((b & 0x00ff) << 8 | (a & 0x00ff));
                //------------------------------------------------------------
                in.get(a); in.get(b);
                LSB_tag  [i] = ((b & 0x00ff) << 8 | (a & 0x00ff));
                //------------------------------------------------------------
                if(signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
                    resolution[5] = 16;
            }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
            for(i = 0; i < num_channels; i++)
            {
                in.get(a);
                resolution[i] = (u_char)a;
            }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            in.get(a); in.get(b); in.get(c); in.get(d);
            num_frames = (   (a & 0x000000ff)
                           | (b & 0x000000ff) << 8
                           | (c & 0x000000ff) << 16
                           | (d & 0x000000ff) << 24
                         );
        }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R && in.good())
            in.read((char*)color_rescale_r, 256 * sizeof(signed short));

        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G && in.good())
            in.read((char*)color_rescale_g, 256 * sizeof(signed short));

        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B && in.good())
            in.read((char*)color_rescale_b, 256 * sizeof(signed short));

        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I && in.good())
            in.read((char*)color_rescale_i, 256 * sizeof(signed short));
        //--------------------------------------------------------------------
    } // end if(temp_string == "LaserBoy")
    d = ' ';
    while(d != 'a' && in.good())
    {
        c = ' ';
        while(c != 't' && in.good())
        {
            b = ' ';
            while(b != 'a' && in.good())
            {
                a = ' ';
                while(a != 'd' && in.good())
                    in.get(a);
                in.get(b);
            }
            in.get(c);
        }
        in.get(d);
    }
    if(!in.good())
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sub_chunk_2_size = (   (a & 0x000000ff)
                         | (b & 0x000000ff) << 8
                         | (c & 0x000000ff) << 16
                         | (d & 0x000000ff) << 24
                       );
    //------------------------------------------------------------------------
    num_samples = sub_chunk_2_size / (num_channels * (bits_per_sample / 8));
    version     = version_guess; // Woohoo !!!
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void LaserBoy_wave_header::to_fstream_wave(fstream& out) const
{
    short   audio_format          = 1,
            block_align           = num_channels * (bits_per_sample / 8);

    int     i,
            sub_chunk_1_size      = 16, // standard wave Subchunk1Size
            LaserBoy_chunk_size   = 0,
            sub_chunk_2_size      = num_samples * num_channels * (bits_per_sample / 8),
            byte_rate             = sample_rate * num_channels * (bits_per_sample / 8),
            chunk_size            = 20 + sub_chunk_1_size + sub_chunk_2_size;
    //------------------------------------------------------------------------
    if(version != "!LaserBoy!") // it is a LaserBoy wave.
    {
        LaserBoy_chunk_size = (   16 // sizeof "LaserBoymmddCCYY"
                                + sizeof(int) // sizeof (int)LaserBoy_wave_mode
                                + (   (LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS)
                                    ? (num_channels * sizeof(int))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
                                    ? (8 * sizeof(int)) // eight 32bit numbers
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
                                    ? (num_channels * 2 * sizeof(short))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
                                    ? (num_channels * sizeof(u_char))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_WAVE_NUM_FRAMES)
                                    ? (sizeof(int))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                                    ? (256 * sizeof(short))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                                    ? (256 * sizeof(short))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                                    ? (256 * sizeof(short))
                                    : (0)
                                  )
                                + (   (LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                                    ? (256 * sizeof(short))
                                    : (0)
                                  )
                              );
        chunk_size += (   4 // sizeof "LBoy"
                        + sizeof(int) // sizeof (int)LaserBoy_chunk_size
                        + LaserBoy_chunk_size
                      );

    }
    //------------------------------------------------------------------------
    out.seekp(0, ios::beg);
    //------------------------------------------------------------------------
    out.put('R');
    out.put('I');
    out.put('F');
    out.put('F');

    out.put((char) (chunk_size & 0x000000ff)       );
    out.put((char)((chunk_size & 0x0000ff00) >> 8 ));
    out.put((char)((chunk_size & 0x00ff0000) >> 16));
    out.put((char)((chunk_size & 0xff000000) >> 24));

    out.put('W');
    out.put('A');
    out.put('V');
    out.put('E');

    out.put('f');
    out.put('m');
    out.put('t');
    out.put(' ');

    out.put((char) (sub_chunk_1_size & 0x000000ff)       );
    out.put((char)((sub_chunk_1_size & 0x0000ff00) >> 8 ));
    out.put((char)((sub_chunk_1_size & 0x00ff0000) >> 16));
    out.put((char)((sub_chunk_1_size & 0xff000000) >> 24));

    out.put((char) (audio_format & 0x00ff)      );
    out.put((char)((audio_format & 0xff00) >> 8)); // 2

    out.put((char) (num_channels & 0x00ff)      );
    out.put((char)((num_channels & 0xff00) >> 8)); // 4

    out.put((char) (sample_rate & 0x000000ff)       );
    out.put((char)((sample_rate & 0x0000ff00) >> 8 ));
    out.put((char)((sample_rate & 0x00ff0000) >> 16));
    out.put((char)((sample_rate & 0xff000000) >> 24)); // 8

    out.put((char) (byte_rate & 0x000000ff)       );
    out.put((char)((byte_rate & 0x0000ff00) >> 8 ));
    out.put((char)((byte_rate & 0x00ff0000) >> 16));
    out.put((char)((byte_rate & 0xff000000) >> 24)); // 12

    out.put((char) (block_align & 0x00ff)      );
    out.put((char)((block_align & 0xff00) >> 8)); // 14

    out.put((char) (bits_per_sample & 0x00ff)      );
    out.put((char)((bits_per_sample & 0xff00) >> 8)); // 16
//----------------------------------------------------------------------------
// extra information added for LaserBoy !!!
//----------------------------------------------------------------------------
    if(version != "!LaserBoy!")
    {
        out.put('L');
        out.put('B');
        out.put('o');
        out.put('y');

        out.put((char) (LaserBoy_chunk_size & 0x000000ff)       );
        out.put((char)((LaserBoy_chunk_size & 0x0000ff00) >> 8 ));
        out.put((char)((LaserBoy_chunk_size & 0x00ff0000) >> 16));
        out.put((char)((LaserBoy_chunk_size & 0xff000000) >> 24)); // 36

        out.put(version[ 0]); // L
        out.put(version[ 1]); // a
        out.put(version[ 2]); // s
        out.put(version[ 3]); // e
        out.put(version[ 4]); // r
        out.put(version[ 5]); // B
        out.put(version[ 6]); // o
        out.put(version[ 7]); // y

        out.put(version[ 8]); // m
        out.put(version[ 9]); // m
        out.put(version[10]); // d
        out.put(version[11]); // d
        out.put(version[12]); // C
        out.put(version[13]); // C
        out.put(version[14]); // Y
        out.put(version[15]); // Y
        //--------------------------------------------------------------------
        out.put((char) (LaserBoy_wave_mode & 0x000000ff)       );
        out.put((char)((LaserBoy_wave_mode & 0x0000ff00) >> 8 ));
        out.put((char)((LaserBoy_wave_mode & 0x00ff0000) >> 16));
        out.put((char)((LaserBoy_wave_mode & 0xff000000) >> 24));
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_OFFSETS)
            for(i = 0; i < num_channels; i++)
            {
                out.put((char)( offset[i] & 0x000000ff)       );
                out.put((char)((offset[i] & 0x0000ff00) >> 8 ));
                out.put((char)((offset[i] & 0x00ff0000) >> 16));
                out.put((char)((offset[i] & 0xff000000) >> 24));
            }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            out.put((char) (parms.lit_dwell_overhang & 0x000000ff)       ); // int
            out.put((char)((parms.lit_dwell_overhang & 0x0000ff00) >> 8 ));
            out.put((char)((parms.lit_dwell_overhang & 0x00ff0000) >> 16));
            out.put((char)((parms.lit_dwell_overhang & 0xff000000) >> 24));

            out.put((char)(((int)(parms.lit_delta_max) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.lit_delta_max) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.lit_delta_max) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.lit_delta_max) & 0x000000ff)       );

            out.put((char)(((int)(parms.blank_delta_max) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.blank_delta_max) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.blank_delta_max) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.blank_delta_max) & 0x000000ff)       );

            out.put((char)(((int)(parms.max_dwell_microsec) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.max_dwell_microsec) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.max_dwell_microsec) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.max_dwell_microsec) & 0x000000ff)       );

            out.put((char)(((int)(parms.max_wag_microsec) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.max_wag_microsec) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.max_wag_microsec) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.max_wag_microsec) & 0x000000ff)       );

            out.put((char)(((int)(parms.insignificant_distance) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.insignificant_distance) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.insignificant_distance) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.insignificant_distance) & 0x000000ff)       );

            out.put((char)(((int)(parms.insignificant_angle) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.insignificant_angle) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.insignificant_angle) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.insignificant_angle) & 0x000000ff)       );

            out.put((char)(((int)(parms.frames_per_second) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.frames_per_second) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.frames_per_second) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.frames_per_second) & 0x000000ff)       );
        }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
            for(i = 0; i < num_channels; i++)
            {
                out.put((char) (signal_id[i] & 0x00ff)      );
                out.put((char)((signal_id[i] & 0xff00) >> 8));

                out.put((char) (LSB_tag[i] & 0x00ff)      );
                out.put((char)((LSB_tag[i] & 0xff00) >> 8));
            }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
            for(i = 0; i < num_channels; i++)
                out.put(resolution[i]);
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            out.put((char)( num_frames & 0x000000ff)       );
            out.put((char)((num_frames & 0x0000ff00) >> 8 ));
            out.put((char)((num_frames & 0x00ff0000) >> 16));
            out.put((char)((num_frames & 0xff000000) >> 24));
        }
        //--------------------------------------------------------------------
        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            out.write((char*)color_rescale_r, 256 * sizeof(signed short));
        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
            out.write((char*)color_rescale_g, 256 * sizeof(signed short));
        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
            out.write((char*)color_rescale_b, 256 * sizeof(signed short));
        if(LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
            out.write((char*)color_rescale_i, 256 * sizeof(signed short));
        //--------------------------------------------------------------------
    } // end if(with_LaserBoy_stuff)
//----------------------------------------------------------------------------
// end of extra information added for LaserBoy !!!
//----------------------------------------------------------------------------
    out.put('d');
    out.put('a');
    out.put('t');
    out.put('a');
    //------------------------------------------------------------------------
    out.put((char) (sub_chunk_2_size & 0x000000ff)       );
    out.put((char)((sub_chunk_2_size & 0x0000ff00) >> 8 ));
    out.put((char)((sub_chunk_2_size & 0x00ff0000) >> 16));
    out.put((char)((sub_chunk_2_size & 0xff000000) >> 24));

    return;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
