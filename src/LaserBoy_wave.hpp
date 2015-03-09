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
// LaserBoy_wave.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_WAVE_DEFINITIONS__
#define __LASERBOY_WAVE_DEFINITIONS__

//############################################################################
#include "LaserBoy_common.hpp"

//############################################################################
class LaserBoy_wave_optimization_parameters
{
public:
    LaserBoy_wave_optimization_parameters()
              : lit_dwell_overhang     (-1), // -1 indicates undefined
                lit_delta_max          (-1),
                blank_delta_max        (-1),
                max_dwell_microsec     (-1),
                max_wag_microsec       (-1),
                insignificant_distance (-1),
                insignificant_angle    (-1),
                frames_per_second      (-1)
                                                      {}
    //------------------------------------------------------------------------
   ~LaserBoy_wave_optimization_parameters()           {}
    //------------------------------------------------------------------------
    int    lit_dwell_overhang;
    float  lit_delta_max,
           blank_delta_max,
           max_dwell_microsec,
           max_wag_microsec,
           insignificant_distance,
           insignificant_angle,
           frames_per_second;
};

//############################################################################
class LaserBoy_wave_header
{
public:
    LaserBoy_wave_header()
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
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_header(int r, int m, short c, string v)
                  : num_channels       ( c),
                    bits_per_sample    (16),
                    num_samples        ( 0),
                    sample_rate        ( r),
                    num_frames         ( 0),
                    LaserBoy_wave_mode ( m),
                    version            ( v),
                    parms              (  )
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
    }
    //------------------------------------------------------------------------
    LaserBoy_wave_header    (const LaserBoy_wave_header& header     );
    LaserBoy_wave_header    (const LaserBoy_wave_header& header, int); // copy 8ch from 6ch
    LaserBoy_wave_header    (fstream& in                            );
    //------------------------------------------------------------------------
   ~LaserBoy_wave_header    ()                        {}
    //------------------------------------------------------------------------
    bool     from_fstream_wave (fstream& in );
    void     to_fstream_wave   (fstream& out) const;
    //------------------------------------------------------------------------
    // data
    //------------------------------------------------------------------------
    u_char   resolution  [8];
    //------------------------------------------------------------------------
    u_short  num_channels         ,
             bits_per_sample      ;
    short    signal_id       [  8];
    u_short  LSB_tag         [  8],
             color_rescale_r [256],
             color_rescale_g [256],
             color_rescale_b [256],
             color_rescale_i [256];
    //------------------------------------------------------------------------
    u_int    num_samples,
             sample_rate,
             num_frames ,
             LaserBoy_wave_mode; // up to 32 binary flags;
    //------------------------------------------------------------------------
    int      offset      [8];
    //------------------------------------------------------------------------
    string   version        ;
    //------------------------------------------------------------------------
    LaserBoy_wave_optimization_parameters parms;
};

//############################################################################










//############################################################################
class LaserBoy_wave_sample
{
public:
    LaserBoy_wave_sample(u_int n)
                        : channel      (new short[n]),
                          num_channels (n           )
        {
            memset((void*)channel, 0x00, num_channels * sizeof(short));
        }
    //------------------------------------------------------------------------
   ~LaserBoy_wave_sample()    {    delete channel;    }
    //------------------------------------------------------------------------
    bool from_fstream_wave(fstream& in)
            {
                if(in.good())
                {
                    in.read((char*)channel, num_channels * sizeof(signed short));
                    return true;
                }
                return false; // returns false if EOF
            }
    //------------------------------------------------------------------------
    bool from_fstream_wave_inverted(fstream& in)
            {
                if(in.good())
                {
                    in.read((char*)channel, num_channels * sizeof(signed short));
                    negate();
                    return true;
                }
                return false; // returns false if EOF
            }
    //------------------------------------------------------------------------
    bool from_fstream_wave_polarity(fstream& in, int polarity[8])
            {
                if(in.good())
                {
                    in.read((char*)channel, num_channels * sizeof(signed short));
                    for(u_int i = 0; i < num_channels; i++)
                        if(polarity[i] == 1) // invert
                        {
                            if(channel[i] >= LASERBOY_MIN_SHORT)
                                channel[i] = -channel[i];
                            else
                                channel[i] = LASERBOY_MAX_SHORT; // there is no +32768!
                        }
                    return true;
                }
                return false; // returns false if EOF
            }
    //------------------------------------------------------------------------
    bool from_fstream_wave_bit_resolution(fstream& in, u_int signal_bit_mask[8])
            {
                if(in.good())
                {
                    in.read((char*)channel, num_channels * sizeof(signed short));
                    for(u_int i = 0; i < num_channels; i++)
                        channel[i] &= short_bit_mask[signal_bit_mask[i]];
                    return true;
                }
                return false; // returns false if EOF
            }
    //------------------------------------------------------------------------
    void to_fstream_wave(fstream& out, LaserBoy_wave_header& header)
            {
                out.write((char*)channel, num_channels * sizeof(signed short));
                header.num_samples++;
                return;
            }
    //------------------------------------------------------------------------
    void to_fstream_wave_inverted(fstream& out, LaserBoy_wave_header& header)
            {
                negate();
                out.write((char*)channel, num_channels * sizeof(signed short));
                negate();
                header.num_samples++;
                return;
            }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample& operator = (const LaserBoy_wave_sample& sample)
            {
                delete channel;
                num_channels = sample.num_channels;
                channel = new short[num_channels];
                memcpy((void*)channel, (void*)sample.channel, num_channels * sizeof(short));
                return *this;
            }
    //------------------------------------------------------------------------
    void negate()
            {
                for(u_int i = 0; i < num_channels; i++)
                {
                    if(channel[i] >= LASERBOY_MIN_SHORT)
                        channel[i] = -channel[i];
                    else
                        channel[i] = LASERBOY_MAX_SHORT; // there is no +32768!
                }
                return;
            }
    //------------------------------------------------------------------------
    LaserBoy_wave_sample  operator + (const LaserBoy_wave_sample& s) // side-by-side as tracks
            {
                LaserBoy_wave_sample sum(num_channels + s.num_channels);
                memcpy(   (void*)sum.channel,
                          (void*)channel,
                          num_channels * sizeof(short)
                      );
                memcpy(   (void*)(sum.channel + num_channels), // pointer math!
                          (void*)s.channel,
                          s.num_channels * sizeof(short)
                      );
                return sum;
            }
    //------------------------------------------------------------------------
    signed short   *channel     ;
    u_int           num_channels;
};

//############################################################################
typedef LaserBoy_wave_sample*  LaserBoy_wave_sample_pointer;

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
