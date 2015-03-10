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
// Copyright 2003, 04, 05, 06, 07, 08, 09, 10, 11, 12, 2013 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// LaserBoy_utility.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_UTILITY_DEFINITIONS__
#define __LASERBOY_UTILITY_DEFINITIONS__

//############################################################################
#include <LaserBoy/vertex.hpp>


//############################################################################
inline int linear_steps(const LaserBoy_3D_double p1,
                        const LaserBoy_3D_double p2,
                        const double delta_max
                       )
{
    return (int)(ceil((p1 | p2) / delta_max));
}

//############################################################################
inline int start_dwell_vertices(double angle,
                                int    sample_rate,
                                double max_dwell_microsec
                               ) // zero to pi radians
{
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline int inline_dwell_vertices(double angle,
                                 int    sample_rate,
                                 double max_dwell_microsec
                                ) // zero to pi radians
{
    if(angle < (pi / 8))
        return 0;
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline int end_dwell_vertices(double angle,
                              int    sample_rate,
                              double max_dwell_microsec
                             ) // zero to pi radians
{
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline double three_point_angle(LaserBoy_3D_double _0, LaserBoy_3D_double _1, LaserBoy_3D_double _2)
{
    return delta_angle( _0 | _2, // distance between vertices 0,2
                        _0 | _1,
                        _1 | _2
                      );
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex(LaserBoy_3D_double p, LaserBoy_3D_double a)
{
    LaserBoy_3D_double  sin_a(sin(a.x), sin(a.y), sin(a.z)),
                        cos_a(cos(a.x), cos(a.y), cos(a.z)),
                        rotated;
    //------------------------------------------------------------------------
    rotated.y =       p.y * cos_a.x -  p.z * sin_a.x;
    rotated.z =       p.z * cos_a.x +  p.y * sin_a.x; // rotate around x
    rotated.z = rotated.z * cos_a.y -  p.x * sin_a.y;
    rotated.x =       p.x * cos_a.y + (p.z * cos_a.x +  p.y * sin_a.x) * sin_a.y; // rotate around y
    rotated.x = rotated.x * cos_a.z - (p.y * cos_a.x -  p.z * sin_a.x) * sin_a.z;
    rotated.y = rotated.y * cos_a.z + (p.x * cos_a.y + (p.z * cos_a.x +  p.y * sin_a.x) * sin_a.y) * sin_a.z; // rotate around z
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_x(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.y = p.y * cos_a - p.z * sin_a;
    rotated.z = p.z * cos_a + p.y * sin_a; // rotate around x
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_y(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.x = p.x * cos_a - p.z * sin_a;
    rotated.z = p.z * cos_a + p.x * sin_a; // rotate around y
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_z(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.x = p.x * cos_a - p.y * sin_a;
    rotated.y = p.y * cos_a + p.x * sin_a; // rotate around z
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates(LaserBoy_3D_double p1, LaserBoy_3D_double p2, LaserBoy_3D_double a)
{
    return rotate_vertex(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_x(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_x(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_y(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_y(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_z(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_z(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double scale_vertex_on_coordinates(LaserBoy_3D_double p1, LaserBoy_3D_double p2, LaserBoy_3D_double m)
{
    return (m * (p1 - p2)) + p2;
}

//############################################################################
inline LaserBoy_Bounds LaserBoy_bounds_check(const LaserBoy_3D_double& f,
                                             const LaserBoy_3D_double& max,
                                             const LaserBoy_3D_double& min
                                            )
{
    LaserBoy_Bounds bounds_flags = LASERBOY_IN_BOUNDS;

    if(f.x > max.x)  bounds_flags |= LASERBOY_OUT_POS_X;
    if(f.x < min.x)  bounds_flags |= LASERBOY_OUT_NEG_X;

    if(f.y > max.y)  bounds_flags |= LASERBOY_OUT_POS_Y;
    if(f.y < min.y)  bounds_flags |= LASERBOY_OUT_NEG_Y;

    if(f.z > max.z)  bounds_flags |= LASERBOY_OUT_POS_Z;
    if(f.z < min.z)  bounds_flags |= LASERBOY_OUT_NEG_Z;

    return bounds_flags;
}

//############################################################################
inline LaserBoy_Bounds LaserBoy_bounds_check(const LaserBoy_3D_double& f, const int& space)
{
    LaserBoy_3D_double max, min;
    switch(space)
    {
        case LASERBOY_CUBE          : max =  32767.0;
                                      min = -32767.0;
                                      break;
        //--------------------------------------------------------------------
        case LASERBOY_SPHERE        : max =  23169.0;
                                      min = -23169.0;
                                      break;
        //--------------------------------------------------------------------
        case LASERBOY_7_8THS_CUBE   : max =  28672.0;
                                      min = -28672.0;
                                      break;
        //--------------------------------------------------------------------
        case LASERBOY_7_8THS_SPHERE : max =  20274.0;
                                      min = -20274.0;
                                      break;
        //--------------------------------------------------------------------
    }
    return LaserBoy_bounds_check(f, max, min);
}

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
