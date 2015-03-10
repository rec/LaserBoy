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
// LaserBoy_3D_double.hpp is part of LaserBoy.
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
class LaserBoy_3D_double
{
public:
    LaserBoy_3D_double(const double& f = 0)
               : x(f)
               , y(f)
               , z(f)
                                                {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const double& _x,
                       const double& _y,
                       const double& _z
                      )
               : x(_x)
               , y(_y)
               , z(_z)
                                                {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const LaserBoy_3D_double& f)
               : x(f.x)
               , y(f.y)
               , z(f.z)
                                                {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const LaserBoy_3D_short& s)
               : x(s.x)
               , y(s.y)
               , z(s.z)
                                                {}
    //------------------------------------------------------------------------
   ~LaserBoy_3D_double()                        {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator = (const LaserBoy_3D_double& f)
                    {
                        x = f.x;
                        y = f.y;
                        z = f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator = (const double& f)
                    {
                        x = y = z = f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_3D_double& f) const
            {
                if(    (x != f.x)
                    || (y != f.y)
                    || (z != f.z)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator != (const LaserBoy_3D_double& f) const
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
    bool operator > (const LaserBoy_3D_double& f) const // distance from the origin
            {
                return(   (  x *   x +   y *   y +   z *   z)
                        > (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator >= (const LaserBoy_3D_double& f) const // distance from the origin
            {
                return(    (  x *   x +   y *   y +   z *   z)
                        >= (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator < (const LaserBoy_3D_double& f) const // distance from the origin
            {
                return(   (  x *   x +   y *   y +   z *   z)
                        < (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator <= (const LaserBoy_3D_double& f) const // distance from the origin
            {
                return(    (  x *   x +   y *   y +   z *   z)
                        <= (f.x * f.x + f.y * f.y + f.z * f.z)
                      );
            }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator -  (const LaserBoy_3D_double& f) const
                    {
                        return LaserBoy_3D_double(x - f.x, y - f.y, z - f.z);
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator -= (const LaserBoy_3D_double& f)
                    {
                        x -= f.x;
                        y -= f.y;
                        z -= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator -= (const double& f)
                    {
                        x -= f;
                        y -= f;
                        z -= f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator +  (const LaserBoy_3D_double& f) const
                    {
                        return LaserBoy_3D_double(x + f.x,
                                                  y + f.y,
                                                  z + f.z
                                                 );
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator +  (const LaserBoy_3D_short& s) const
                    {
                        return LaserBoy_3D_double(x + s.x,
                                                  y + s.y,
                                                  z + s.z
                                                 );
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator += (const LaserBoy_3D_double& f)
                    {
                        x += f.x;
                        y += f.y;
                        z += f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator += (const double& f)
                    {
                        x += f;
                        y += f;
                        z += f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator *  (const LaserBoy_3D_double& f) const
                    {
                        return LaserBoy_3D_double(x * f.x, y * f.y, z * f.z);
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const LaserBoy_3D_double& f) const
                    {
                        LaserBoy_3D_double quotient;
                        if(f.x)    quotient.x = x / f.x;
                        if(f.y)    quotient.y = y / f.y;
                        if(f.z)    quotient.z = z / f.z;
                        return quotient;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator *  (const double& f) const
                    {
                        return LaserBoy_3D_double(x * f, y * f, z * f);
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const double& f) const
                    {
                        if(f)
                            return LaserBoy_3D_double(x / f, y / f, z / f);
                        return LaserBoy_3D_double();
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const int& i) const
                    {
                        if(i)
                            return LaserBoy_3D_double(x / i, y / i, z / i);
                        return LaserBoy_3D_double();
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator *= (const LaserBoy_3D_double& f)
                    {
                        x *= f.x;
                        y *= f.y;
                        z *= f.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator *= (const double& f)
                    {
                        x *= f;
                        y *= f;
                        z *= f;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator /= (const LaserBoy_3D_double& f)
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
    LaserBoy_3D_double& operator /= (const double& f)
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
    operator LaserBoy_3D_short() const // cast to LaserBoy_3D_short
                {
                    return LaserBoy_3D_short( (short)round(x),
                                              (short)round(y),
                                              (short)round(z)
                                            );
                }
    //------------------------------------------------------------------------
    LaserBoy_3D_double operator - ()
                {
                    LaserBoy_3D_double negative;
                    negative.x = -x;
                    negative.y = -y;
                    negative.z = -z;
                    return negative;
                }
    //------------------------------------------------------------------------
    double operator | (const LaserBoy_3D_double& f) const // distance from each other
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
                    LaserBoy_3D_double origin;
                    return(origin | *this);
                }
    //------------------------------------------------------------------------
    LaserBoy_3D_double direction() const // z and y rotational angle from origin
            {
                LaserBoy_3D_double origin,
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
