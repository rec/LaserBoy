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
// 3D_double.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_3D_DOUBLE_DEFINITIONS__
#define __LASERBOY_3D_DOUBLE_DEFINITIONS__

//############################################################################
#include <LaserBoy/3D_short.hpp>

namespace LaserBoy {

//############################################################################
class Double3d
{
public:
    Double3d(const double& f = 0)
               : x(f)
               , y(f)
               , z(f)
                                                {}
    //------------------------------------------------------------------------
    Double3d(const double& _x,
                       const double& _y,
                       const double& _z
                      )
               : x(_x)
               , y(_y)
               , z(_z)
                                                {}
    //------------------------------------------------------------------------
    Double3d(const Double3d& f)
               : x(f.x)
               , y(f.y)
               , z(f.z)
                                                {}
    //------------------------------------------------------------------------
    Double3d(const Short3d& s)
               : x(s.x)
               , y(s.y)
               , z(s.z)
                                                {}
    //------------------------------------------------------------------------
   ~Double3d()                        {}
    //------------------------------------------------------------------------
    Double3d& operator = (const Double3d& f)
                    {
                        x = f.x;
                        y = f.y;
                        z = f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator = (const double& f)
                    {
                        x = y = z = f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    bool operator == (const Double3d& f) const
            {
                if(    (x != f.x)
                    || (y != f.y)
                    || (z != f.z)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator != (const Double3d& f) const
            {
                if(    (x == f.x)
                    && (y == f.y)
                    && (z == f.z)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator == (const double& f) const
            {
                if(    (x != f)
                    || (y != f)
                    || (z != f)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator != (const double& f) const
            {
                if(    (x == f)
                    && (y == f)
                    && (z == f)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator > (const Double3d& f) const // distance from the origin
            {
                return(   (  x *   x +   y *   y +   z *   z)
                        > (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator >= (const Double3d& f) const // distance from the origin
            {
                return(    (  x *   x +   y *   y +   z *   z)
                        >= (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator < (const Double3d& f) const // distance from the origin
            {
                return(   (  x *   x +   y *   y +   z *   z)
                        < (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator <= (const Double3d& f) const // distance from the origin
            {
                return(    (  x *   x +   y *   y +   z *   z)
                        <= (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    Double3d  operator -  (const Double3d& f) const
                    {
                        return Double3d(x - f.x, y - f.y, z - f.z);
                    }
    //------------------------------------------------------------------------
    Double3d& operator -= (const Double3d& f)
                    {
                        x -= f.x;
                        y -= f.y;
                        z -= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator -= (const double& f)
                    {
                        x -= f;
                        y -= f;
                        z -= f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d  operator +  (const Double3d& f) const
                    {
                        return Double3d(x + f.x,
                                                  y + f.y,
                                                  z + f.z
                                                 );
                    }
    //------------------------------------------------------------------------
    Double3d  operator +  (const Short3d& s) const
                    {
                        return Double3d(x + s.x,
                                                  y + s.y,
                                                  z + s.z
                                                 );
                    }
    //------------------------------------------------------------------------
    Double3d& operator += (const Double3d& f)
                    {
                        x += f.x;
                        y += f.y;
                        z += f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator += (const double& f)
                    {
                        x += f;
                        y += f;
                        z += f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d  operator *  (const Double3d& f) const
                    {
                        return Double3d(x * f.x, y * f.y, z * f.z);
                    }
    //------------------------------------------------------------------------
    Double3d  operator /  (const Double3d& f) const
                    {
                        Double3d quotient;
                        if(f.x)    quotient.x = x / f.x;
                        if(f.y)    quotient.y = y / f.y;
                        if(f.z)    quotient.z = z / f.z;
                        return quotient;
                    }
    //------------------------------------------------------------------------
    Double3d  operator *  (const double& f) const
                    {
                        return Double3d(x * f, y * f, z * f);
                    }
    //------------------------------------------------------------------------
    Double3d  operator /  (const double& f) const
                    {
                        if(f)
                            return Double3d(x / f, y / f, z / f);
                        return Double3d();
                    }
    //------------------------------------------------------------------------
    Double3d  operator /  (const int& i) const
                    {
                        if(i)
                            return Double3d(x / i, y / i, z / i);
                        return Double3d();
                    }
    //------------------------------------------------------------------------
    Double3d& operator *= (const Double3d& f)
                    {
                        x *= f.x;
                        y *= f.y;
                        z *= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator *= (const double& f)
                    {
                        x *= f;
                        y *= f;
                        z *= f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator /= (const Double3d& f)
                    {
                        if(    (f.x != 0.0)
                            && (f.y != 0.0)
                            && (f.z != 0.0)
                          )
                        x /= f.x;
                        y /= f.y;
                        z /= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Double3d& operator /= (const double& f)
                    {
                        if(f != 0.0)
                        {
                            x /= f;
                            y /= f;
                            z /= f;
                        }
                        return *this;
                    }
    //------------------------------------------------------------------------
    operator Short3d() const // cast to Short3d
                {
                    return Short3d( (short)round(x),
                                              (short)round(y),
                                              (short)round(z)
                                            );
                }
    //------------------------------------------------------------------------
    Double3d operator - ()
                {
                    Double3d negative;
                    negative.x = -x;
                    negative.y = -y;
                    negative.z = -z;
                    return negative;
                }
    //------------------------------------------------------------------------
    double operator | (const Double3d& f) const // distance from each other
                {
                    return( sqrt(   (double)(x - f.x) * (x - f.x)
                                  + (double)(y - f.y) * (y - f.y)
                                  + (double)(z - f.z) * (z - f.z)
                                )
                          );
                }
    //------------------------------------------------------------------------
    double magnitude() const // distance from origin
                {
                    Double3d origin;
                    return(origin | *this);
                }
    //------------------------------------------------------------------------
    Double3d direction() const // z and y rotational angle from origin
            {
                Double3d origin,
                                   basline_vertex,
                                   copy = *this,
                                   rotation;
                //............................................................
                basline_vertex.x = 1.0;
                copy.z           = 0.0;
                rotation.z       = (double)delta_angle(  copy | basline_vertex
                                                       , 1.0
                                                       , copy | origin
                                                      );
                if(rotation.y < 0) rotation.z  = two_pi - rotation.z;
                //............................................................
                copy             =  *this;
                copy.y           = 0.0;
                rotation.y       = (double)delta_angle(  copy | basline_vertex
                                                       , 1.0
                                                       , copy | origin
                                                      );
                if(rotation.z < 0) rotation.y  = two_pi - rotation.y;
                //............................................................
                return rotation;
            }
    //------------------------------------------------------------------------
    double sum()    {    return x + y + z;    }
    //------------------------------------------------------------------------
    double x, y, z;
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
