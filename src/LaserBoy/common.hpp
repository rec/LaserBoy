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
// LaserBoy_common.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_COMMON_DEFINITIONS__
#define __LASERBOY_COMMON_DEFINITIONS__

//############################################################################
#include <LaserBoy/macros.hpp>

namespace LaserBoy {

//############################################################################
typedef unsigned int   u_int  ;
typedef unsigned short u_short;
typedef unsigned char  u_char ;

typedef unsigned int   LaserBoy_Bounds    ;
typedef unsigned int   LaserBoy_Error_Code;
typedef unsigned int   LaserBoy_Rescale_Error_Code;
//############################################################################
extern const double    quarter_pi;
extern const double    half_pi;
extern const double    three_quarters_pi;
extern const double    pi;
extern const double    two_pi;

extern const double    one_degree; // in radians
extern const double    one_radian; // in degrees

extern const u_short   short_bit_mask[16];

//############################################################################
//                 delta_02, delta_01, delta_12,
double delta_angle(double a, double b, double c); // sss triangle

//############################################################################
double simple_2D_angle(double x1, double y1, double x2, double y2);

//############################################################################
u_char rescale_to_index(u_short* rescale, u_short value);

//############################################################################
int    greatest_common_devisor  (int x, int y);
int    lowest_common_denominator(int x, int y);

//############################################################################
bool   file_exists     (const string& file);
bool   directory_exists(const string& dir );

//############################################################################
string time_as_string  (double seconds);

//############################################################################
double random_01          ();
double random_neg_to_pos_1();

//############################################################################
bool   get_dxf_pair   (ifstream& in, int& group_code, string& entity_string);

//############################################################################
void   txt_tag        (ofstream& out);

bool   clear_to_alpha (ifstream& in, u_int& line_number);
bool   clear_to_digit (ifstream& in, u_int& line_number);
bool   clear_to_token (ifstream& in, u_int& next_char, u_int& line_number);

bool   get_next_word  (ifstream& in, string& word  , u_int& line_number);
bool   get_next_number(ifstream& in, double& number, u_int& line_number);
bool   get_next_hex   (ifstream& in, double& number, u_int& line_number);

//############################################################################
int    LaserBoy_version_check(string &version_string,
                              string install_GUID,
                              u_int app_runs_count
                             );

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
