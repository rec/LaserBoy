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
// 3D_short.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_3D_SHORT_DEFINITIONS__
#define __LASERBOY_3D_SHORT_DEFINITIONS__

namespace LaserBoy {

//############################################################################
class Short3d
{
public:
    Short3d(const short& s = 0)
               : x(s), y(s), z(s)                       {}
    //------------------------------------------------------------------------
    Short3d( const short& _x,
                       const short& _y,
                       const short& _z
                     )
               : x(_x), y(_y), z(_z)                    {}
    //------------------------------------------------------------------------
    Short3d(const Short3d& p)
               : x(p.x), y(p.y), z(p.z)                 {}
    //------------------------------------------------------------------------
    Short3d(const int& s)
               : x((short)s), y((short)s), z((short)s)  {}
    //------------------------------------------------------------------------
    Short3d& operator = (const Short3d& p)
                    {
                        x = p.x;
                        y = p.y;
                        z = p.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    bool operator == (const Short3d& p) const
            {
                if(    (x != p.x)
                    || (y != p.y)
                    || (z != p.z)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator != (const Short3d& p) const
            {
                if(    (x == p.x)
                    && (y == p.y)
                    && (z == p.z)
                  )
                    return false;
                return true;
            }
    //------------------------------------------------------------------------
    bool operator > (const Short3d& p) const // distance from the origin
            {
                return(   ((double)  x *   x + (double)  y *   y + (double)  z *   z)
                        > ((double)p.x * p.x + (double)p.y * p.y + (double)p.z * p.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator >= (const Short3d& p) const // distance from the origin
            {
                return(    ((double)  x *   x + (double)  y *   y + (double)  z *   z)
                        >= ((double)p.x * p.x + (double)p.y * p.y + (double)p.z * p.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator < (const Short3d& p) const // distance from the origin
            {
                return(   ((double)  x *   x + (double)  y *   y + (double)  z *   z)
                        < ((double)p.x * p.x + (double)p.y * p.y + (double)p.z * p.z)
                      );
            }
    //------------------------------------------------------------------------
    bool operator <= (const Short3d& p) const // distance from the origin
            {
                return(    ((double)  x *   x + (double)  y *   y + (double)  z *   z)
                        <= ((double)p.x * p.x + (double)p.y * p.y + (double)p.z * p.z)
                      );
            }
    //------------------------------------------------------------------------
    Short3d  operator -  (const Short3d& s)
                    {
                        int X, Y, Z;
                        Short3d diff(*this);
                        X = x - s.x;
                        Y = y - s.y;
                        Z = z - s.z;
                        if(X >= LASERBOY_MAX_SHORT)
                            diff.x = LASERBOY_MAX_SHORT;
                        else if(X <= LASERBOY_MIN_SHORT)
                            diff.x = LASERBOY_MIN_SHORT;
                        else
                            diff.x = X;
                        if(Y >= LASERBOY_MAX_SHORT)
                            diff.y = LASERBOY_MAX_SHORT;
                        else if(Y <= LASERBOY_MIN_SHORT)
                            diff.y = LASERBOY_MIN_SHORT;
                        else
                            diff.y = Y;
                        if(Z >= LASERBOY_MAX_SHORT)
                            diff.z = LASERBOY_MAX_SHORT;
                        else if(Z <= LASERBOY_MIN_SHORT)
                            diff.z = LASERBOY_MIN_SHORT;
                        else
                            diff.z = Z;
                        return diff;
                    }
    //------------------------------------------------------------------------
    Short3d& operator -= (const Short3d& p)
                    {
                        x -= p.x;
                        y -= p.y;
                        z -= p.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Short3d  operator +  (const Short3d& s) const
                    {
                        int X, Y, Z;
                        Short3d sum(*this);
                        X = x + s.x;
                        Y = y + s.y;
                        Z = z + s.z;
                        if(X >= LASERBOY_MAX_SHORT)
                            sum.x = LASERBOY_MAX_SHORT;
                        else if(X <= LASERBOY_MIN_SHORT)
                            sum.x = LASERBOY_MIN_SHORT;
                        else
                            sum.x = X;
                        if(Y >= LASERBOY_MAX_SHORT)
                            sum.y = LASERBOY_MAX_SHORT;
                        else if(Y <= LASERBOY_MIN_SHORT)
                            sum.y = LASERBOY_MIN_SHORT;
                        else
                            sum.y = Y;
                        if(Z >= LASERBOY_MAX_SHORT)
                            sum.z = LASERBOY_MAX_SHORT;
                        else if(Z <= LASERBOY_MIN_SHORT)
                            sum.z = LASERBOY_MIN_SHORT;
                        else
                            sum.z = Z;
                        return sum;
                    }
    //------------------------------------------------------------------------
    Short3d& operator += (const Short3d& p)
                    {
                        x += p.x;
                        y += p.y;
                        z += p.z;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Short3d  operator - ()
                    {
                        Short3d negative;
                        negative.x = -x;
                        negative.y = -y;
                        negative.z = -z;
                        return negative;
                    }
    //------------------------------------------------------------------------
    double operator | (const Short3d& p) const // distance from each other
                {
                    double dx = abs(x - p.x),
                           dy = abs(y - p.y),
                           dz = abs(z - p.z);
                    return sqrt(dx*dx + dy*dy + dz*dz);
                }
    //------------------------------------------------------------------------
    double magnitude() const // distance from origin
                {
                    Short3d origin;
                    return(origin | *this);
                }
    //------------------------------------------------------------------------
    Short3d abs_3D() {  return Short3d(abs(x), abs(y), abs(z));  }
    //------------------------------------------------------------------------
    int               sum()    {  return (int)x + (int)y + (int)z;             }
    //------------------------------------------------------------------------
    short x, y, z;
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
