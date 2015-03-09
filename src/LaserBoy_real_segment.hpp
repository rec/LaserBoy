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
// LaserBoy_real_segment.hpp is part of LaserBoy.
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
#include "LaserBoy_utility.hpp"
#include "LaserBoy_palette.hpp"
#include "LaserBoy_vertex.hpp"

//############################################################################
class LaserBoy_frame_set;
class LaserBoy_space;

//############################################################################
class LaserBoy_real_segment : public LaserBoy_real_segment_base
{
public:
    LaserBoy_real_segment(LaserBoy_space* ps = NULL)
                 : p_space            (ps                   ),
                   palette_index      (LASERBOY_ILDA_DEFAULT),
                   real_segment_error (LASERBOY_OK          )
                                                        {}
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_space* ps,
                          const int& palette_index,
                          bool       add_origin
                         )
                 : p_space            (ps           ),
                   palette_index      (palette_index),
                   real_segment_error (LASERBOY_OK  )
                    {
                        if(add_origin)
                        {
                            push_back(LaserBoy_real_vertex());
                            push_back(LaserBoy_real_vertex());
                        }
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const LaserBoy_real_segment& segment)
                 : p_space            (segment.p_space      ),
                   palette_index      (segment.palette_index),
                   real_segment_error (LASERBOY_OK          )
                    {
                        clear();
                        reserve(segment.size());
                        insert(end(), segment.begin(), segment.end());
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_space*      ps,
                          LaserBoy_real_vertex from,
                          LaserBoy_real_vertex to
                         ); // 3D line function
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_real_segment() {}
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
    bool operator == (const LaserBoy_real_segment& segment)
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
    LaserBoy_real_segment&  operator += (const LaserBoy_real_segment& segment)
                        {
                            insert(end(), segment.begin(), segment.end());
                            return *this;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator += (const LaserBoy_real_vertex& vertex)
                        {
                            push_back(vertex);
                            return *this;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator += (const LaserBoy_3D_double& float_3D)
                        {
                            for(u_int i = 0; i < size(); i++)
                                at(i) += float_3D;
                            return *this;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator *= (const LaserBoy_3D_double& float_3D)
                        {
                            for(u_int i = 0; i < size(); i++)
                                at(i) *= float_3D;
                            return *this;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   operator + (const LaserBoy_3D_double& float_3D)
                        {
                            LaserBoy_real_segment real_segment(p_space);
                            for(u_int i = 0; i < size(); i++)
                                real_segment.push_back(at(i) + float_3D);
                            return real_segment;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   operator * (const LaserBoy_3D_double& float_3D)
                        {
                            LaserBoy_real_segment real_segment(p_space);
                            for(u_int i = 0; i < size(); i++)
                                real_segment.push_back(at(i) * float_3D);
                            return real_segment;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator =  (const LaserBoy_real_segment& segment)
                        {
                            clear();
//                            palette_index = segment.palette_index;
                            reserve(segment.size());
                            insert(begin(), segment.begin(), segment.end());
                            p_space = segment.p_space;
                            return *this;
                        }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex  first_lit_vector() const
                        {
                            for(u_int i = 1; i < size(); i++)
                                if(at(i).is_lit())
                                    return at(i);
                            return LaserBoy_real_vertex(0);
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
    LaserBoy_real_vertex first_lit_anchor() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return LaserBoy_real_vertex(0);
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
    LaserBoy_real_vertex last_lit_anchor() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return LaserBoy_real_vertex(0);
                    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex last_lit_vector() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i);
                        return LaserBoy_real_vertex(0);
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
                LaserBoy_palette  palette(p_space);
                if(first_lit_vector().is_lit()) // DUH!
                {
                    palette.push_back((LaserBoy_color)first_lit_vector());
                    for(i = first_lit_vector_index() + 1; i < size(); i++)
                    {
                        for(j = 0; j < palette.number_of_colors(); j++)
                        {
                            if(palette[j] == (LaserBoy_color)at(i))
                                break;
                        }
                        if(    j == palette.number_of_colors() // color not found
                            && at(i).is_lit()
                          )
                            palette.push_back((LaserBoy_color)at(i));
                    }
                    return palette.number_of_colors();
                }
                return 0;
            }
        }
    //------------------------------------------------------------------------
    LaserBoy_3D_double      segment_front                 () const ;
    LaserBoy_3D_double      segment_back                  () const ;
    LaserBoy_3D_double      segment_top                   () const ;
    LaserBoy_3D_double      segment_bottom                () const ;
    LaserBoy_3D_double      segment_right                 () const ;
    LaserBoy_3D_double      segment_left                  () const ;
    double                  segment_height                () const ;
    double                  segment_width                 () const ;
    double                  segment_depth                 () const ;
    double                  segment_size                  () const ;
    LaserBoy_real_segment&  reverse                       ();
    void                    blank_all_vertices            ();
    void                    unblank_all_vertices          ();
    void                    flip                          (u_int plane);
    void                    quarter_turn                  (u_int plane, u_int turns);
    void                    z_order_vertices              (short span);
    void                    flatten_z                     ();
    LaserBoy_Bounds         rotate                        (LaserBoy_3D_double a);
    void                    rotate_around_origin          (LaserBoy_3D_double a);
    //------------------------------------------------------------------------
    void                    rotate_on_coordinates         (LaserBoy_3D_double p,
                                                           LaserBoy_3D_double a
                                                          );
    //------------------------------------------------------------------------
    bool                    find_rgb_in_palette           (const LaserBoy_palette& palette);
    void                    set_rgb_from_palette          ();
    void                    set_palette_to_332            ();
    void                    sync_rgb_and_palette          ();
    void                    best_match_palette            (u_int index);
    void                    bit_reduce_to_palette         ();
    void                    best_reduce_to_palette        ();
    void                    convert_black_to_blank        ();
    void                    convert_blank_to_black        ();
    void                    impose_black_level            ();
    void                    move                          (LaserBoy_3D_double d);
    void                    scale                         (LaserBoy_3D_double s);
    //------------------------------------------------------------------------
    void                    scale_on_coordinates          (LaserBoy_3D_double p,
                                                           LaserBoy_3D_double s
                                                          );
    //------------------------------------------------------------------------
    void                    scale_around_origin           (LaserBoy_3D_double s);
    LaserBoy_3D_double      rectangular_center_of         () const ;
    LaserBoy_3D_double      mean_of_coordinates           () const ;
    u_int                   number_of_segments            () const ;
    //------------------------------------------------------------------------
    bool                    find_segment_at_index         (u_int  index,
                                                           u_int& start,
                                                           u_int& end
                                                          )           const ;
    //------------------------------------------------------------------------
    LaserBoy_real_segment   copy_segment                  (u_int index) const ;
    LaserBoy_3D_double      rectangular_center_of_segment (u_int index) const ;
    LaserBoy_3D_double      mean_of_coordinates_of_segment(u_int index) const ;
    void                    move_segment                  (u_int index, LaserBoy_3D_double f);
    void                    rotate_segment                (u_int index, LaserBoy_3D_double a);
    void                    rotate_segment_around_origin  (u_int index, LaserBoy_3D_double a);
    void                    scale_segment                 (u_int index, LaserBoy_3D_double m);
    void                    scale_segment_around_origin   (u_int index, LaserBoy_3D_double m);
    LaserBoy_Error_Code     from_ifstream_dxf             (ifstream& in);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     from_ifstream_txt             (ifstream&    in,
                                                           const u_int& group_type,
                                                           u_int&       line_number
                                                          );
    //------------------------------------------------------------------------
    void                    normalize                     ();
    //------------------------------------------------------------------------
    LaserBoy_space*      p_space           ;
    int                  palette_index     ;
    LaserBoy_Error_Code  real_segment_error;
    //------------------------------------------------------------------------
};

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
class LaserBoy_real_segment_set : public vector<LaserBoy_real_segment>
{
public:
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_space* ps = NULL)
        : p_space(ps)              {}
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_real_segment_set()
            { clear(); }
    //------------------------------------------------------------------------
    void normalize();
    //------------------------------------------------------------------------
    LaserBoy_space*   p_space;
    //------------------------------------------------------------------------
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
