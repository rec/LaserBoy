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
// real_segment.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_REAL_SEGMENT_DEFINITIONS__
#define __LASERBOY_REAL_SEGMENT_DEFINITIONS__

//############################################################################
#include <LaserBoy/utility.hpp>
#include <LaserBoy/palette.hpp>
#include <LaserBoy/vertex.hpp>

namespace LaserBoy {

//############################################################################
class FrameSet;
class Space;

//############################################################################
class RealSegment : public RealSegmentBase
{
public:
    RealSegment(Space* ps = NULL)
                 : p_space            (ps                   ),
                   palette_index      (LASERBOY_ILDA_DEFAULT),
                   real_segment_error (LASERBOY_OK          )
                                                        {}
    //------------------------------------------------------------------------
    RealSegment(Space* ps,
                          const int& palette_index,
                          bool       add_origin
                         )
                 : p_space            (ps           ),
                   palette_index      (palette_index),
                   real_segment_error (LASERBOY_OK  )
                    {
                        if(add_origin)
                        {
                            push_back(RealVertex());
                            push_back(RealVertex());
                        }
                    }
    //------------------------------------------------------------------------
    RealSegment(const RealSegment& segment)
                 : p_space            (segment.p_space      ),
                   palette_index      (segment.palette_index),
                   real_segment_error (LASERBOY_OK          )
                    {
                        clear();
                        reserve(segment.size());
                        insert(end(), segment.begin(), segment.end());
                    }
    //------------------------------------------------------------------------
    RealSegment(Space*      ps,
                          RealVertex from,
                          RealVertex to
                         ); // 3D line function
    //------------------------------------------------------------------------
virtual
   ~RealSegment() {}
    //------------------------------------------------------------------------
    bool is_2D() const
            {
                if(size() > 1)
                {
                    for(u_int i = 0; i < size(); i++)
                        if(at(i).z != 0.0)
                            return false;
                }
                return true;
            }
    //------------------------------------------------------------------------
    bool operator == (const RealSegment& segment)
            {
                if(palette_index != segment.palette_index)
                    return false;
                if(size() != segment.size())
                    return false;
                for(u_int i = 0; i < size(); i++)
                    if(at(i) != segment.at(i))
                        return false;
                return true;
            }
    //------------------------------------------------------------------------
    RealSegment&  operator += (const RealSegment& segment)
                        {
                            insert(end(), segment.begin(), segment.end());
                            return *this;
                        }
    //------------------------------------------------------------------------
    RealSegment&  operator += (const RealVertex& vertex)
                        {
                            push_back(vertex);
                            return *this;
                        }
    //------------------------------------------------------------------------
    RealSegment&  operator += (const Double3d& float_3D)
                        {
                            for(u_int i = 0; i < size(); i++)
                                at(i) += float_3D;
                            return *this;
                        }
    //------------------------------------------------------------------------
    RealSegment&  operator *= (const Double3d& float_3D)
                        {
                            for(u_int i = 0; i < size(); i++)
                                at(i) *= float_3D;
                            return *this;
                        }
    //------------------------------------------------------------------------
    RealSegment   operator + (const Double3d& float_3D)
                        {
                            RealSegment real_segment(p_space);
                            for(u_int i = 0; i < size(); i++)
                                real_segment.push_back(at(i) + float_3D);
                            return real_segment;
                        }
    //------------------------------------------------------------------------
    RealSegment   operator * (const Double3d& float_3D)
                        {
                            RealSegment real_segment(p_space);
                            for(u_int i = 0; i < size(); i++)
                                real_segment.push_back(at(i) * float_3D);
                            return real_segment;
                        }
    //------------------------------------------------------------------------
    RealSegment&  operator =  (const RealSegment& segment)
                        {
                            clear();
//                            palette_index = segment.palette_index;
                            reserve(segment.size());
                            insert(begin(), segment.begin(), segment.end());
                            p_space = segment.p_space;
                            return *this;
                        }
    //------------------------------------------------------------------------
    RealVertex  first_lit_vector() const
                        {
                            for(u_int i = 1; i < size(); i++)
                                if(at(i).is_lit())
                                    return at(i);
                            return RealVertex(0);
                        }
    //------------------------------------------------------------------------
    int first_lit_vector_index() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return i;
                        return -1;
                    }
    //------------------------------------------------------------------------
    RealVertex first_lit_anchor() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return RealVertex(0);
                    }
    //------------------------------------------------------------------------
    int first_lit_anchor_index() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return (i - 1);
                        return -1;
                    }
    //------------------------------------------------------------------------
    RealVertex last_lit_anchor() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return RealVertex(0);
                    }
    //------------------------------------------------------------------------
    RealVertex last_lit_vector() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i);
                        return RealVertex(0);
                    }
    //------------------------------------------------------------------------
    int last_lit_vector_index() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return i;
                        return -1;
                    }
    //------------------------------------------------------------------------
    u_int number_of_lit_vectors() const
                    {
                        u_int count = 0;
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                count++;
                        return count;
                    }
    //------------------------------------------------------------------------
    u_int number_of_color_vectors() const;
    u_int number_of_dark_vectors () const;
    //------------------------------------------------------------------------
    u_int number_of_blank_vectors() const
                    {
                        u_int count = 0;
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_blank())
                                count++;
                        return count;
                    }
    //------------------------------------------------------------------------
    u_int number_of_unique_colors() const
        {
            if(size() > 1)
            {
                u_int             i,
                                  j;
                Palette  palette(p_space);
                if(first_lit_vector().is_lit()) // DUH!
                {
                    palette.push_back((Color)first_lit_vector());
                    for(i = first_lit_vector_index() + 1; i < size(); i++)
                    {
                        for(j = 0; j < palette.number_of_colors(); j++)
                        {
                            if(palette[j] == (Color)at(i))
                                break;
                        }
                        if(    j == palette.number_of_colors() // color not found
                            && at(i).is_lit()
                          )
                            palette.push_back((Color)at(i));
                    }
                    return palette.number_of_colors();
                }
                return 0;
            }
        }
    //------------------------------------------------------------------------
    Double3d      segment_front                 () const ;
    Double3d      segment_back                  () const ;
    Double3d      segment_top                   () const ;
    Double3d      segment_bottom                () const ;
    Double3d      segment_right                 () const ;
    Double3d      segment_left                  () const ;
    double                  segment_height                () const ;
    double                  segment_width                 () const ;
    double                  segment_depth                 () const ;
    double                  segment_size                  () const ;
    RealSegment&  reverse                       ();
    void                    blank_all_vertices            ();
    void                    unblank_all_vertices          ();
    void                    flip                          (u_int plane);
    void                    quarter_turn                  (u_int plane, u_int turns);
    void                    z_order_vertices              (short span);
    void                    flatten_z                     ();
    Bounds         rotate                        (Double3d a);
    void                    rotate_around_origin          (Double3d a);
    //------------------------------------------------------------------------
    void                    rotate_on_coordinates         (Double3d p,
                                                           Double3d a
                                                          );
    //------------------------------------------------------------------------
    bool                    find_rgb_in_palette           (const Palette& palette);
    void                    set_rgb_from_palette          ();
    void                    set_palette_to_332            ();
    void                    sync_rgb_and_palette          ();
    void                    best_match_palette            (u_int index);
    void                    bit_reduce_to_palette         ();
    void                    best_reduce_to_palette        ();
    void                    convert_black_to_blank        ();
    void                    convert_blank_to_black        ();
    void                    impose_black_level            ();
    void                    move                          (Double3d d);
    void                    scale                         (Double3d s);
    //------------------------------------------------------------------------
    void                    scale_on_coordinates          (Double3d p,
                                                           Double3d s
                                                          );
    //------------------------------------------------------------------------
    void                    scale_around_origin           (Double3d s);
    Double3d      rectangular_center_of         () const ;
    Double3d      mean_of_coordinates           () const ;
    u_int                   number_of_segments            () const ;
    //------------------------------------------------------------------------
    bool                    find_segment_at_index         (u_int  index,
                                                           u_int& start,
                                                           u_int& end
                                                          )           const ;
    //------------------------------------------------------------------------
    RealSegment   copy_segment                  (u_int index) const ;
    Double3d      rectangular_center_of_segment (u_int index) const ;
    Double3d      mean_of_coordinates_of_segment(u_int index) const ;
    void                    move_segment                  (u_int index, Double3d f);
    void                    rotate_segment                (u_int index, Double3d a);
    void                    rotate_segment_around_origin  (u_int index, Double3d a);
    void                    scale_segment                 (u_int index, Double3d m);
    void                    scale_segment_around_origin   (u_int index, Double3d m);
    ErrorCode     from_ifstream_dxf             (ifstream& in);
    //------------------------------------------------------------------------
    ErrorCode     from_ifstream_txt             (ifstream&    in,
                                                           const u_int& group_type,
                                                           u_int&       line_number
                                                          );
    //------------------------------------------------------------------------
    void                    normalize                     ();
    //------------------------------------------------------------------------
    Space*      p_space           ;
    int                  palette_index     ;
    ErrorCode  real_segment_error;
    //------------------------------------------------------------------------
};

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
class RealSegmentSet : public vector<RealSegment>
{
public:
    //------------------------------------------------------------------------
    RealSegmentSet(Space* ps = NULL)
        : p_space(ps)              {}
    //------------------------------------------------------------------------
virtual
   ~RealSegmentSet()
            { clear(); }
    //------------------------------------------------------------------------
    void normalize();
    //------------------------------------------------------------------------
    Space*   p_space;
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
