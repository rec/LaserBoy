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
// segment.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SEGMENT_DEFINITIONS__
#define __LASERBOY_SEGMENT_DEFINITIONS__

//############################################################################
#include <LaserBoy/bmp.hpp>
#include <LaserBoy/real_segment.hpp>

namespace LaserBoy {

//############################################################################
class Segment : public SegmentBase
{
public:
    Segment(Space* ps = NULL)
                     : p_space       (ps),
                       palette_index (LASERBOY_ILDA_DEFAULT),
                       segment_error (LASERBOY_OK)           {}
    //------------------------------------------------------------------------
    Segment(Space* ps,
                     const int& palette_index,
                     bool       add_origin
                    )
                     : p_space       (ps),
                       palette_index (palette_index),
                       segment_error (LASERBOY_OK)
                    {
                        if(add_origin)
                        {
                            reserve(2);
                            push_back(Vertex());
                            push_back(Vertex());
                        }
                    }
    //------------------------------------------------------------------------
    Segment(const Segment& segment)
                     : p_space       (segment.p_space),
                       palette_index (segment.palette_index),
                       segment_error (LASERBOY_OK)
                    {
                        clear();
                        reserve(segment.size());
                        insert(end(), segment.begin(), segment.end());
                    }
    //------------------------------------------------------------------------
    Segment(const RealSegment& real_segment)
                     : p_space       (real_segment.p_space),
                       palette_index (real_segment.palette_index),
                       segment_error (LASERBOY_OK)
                    {
                        clear();
                        reserve(real_segment.size());
                        for(u_int i = 0; i < real_segment.size(); i++)
                            push_back((Vertex)real_segment[i]);
                    }
    //------------------------------------------------------------------------
    Segment(Space   *ps,
                     Short3d from,
                     Vertex   to
                    ); // the line
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Vertex   p1,
                     Vertex   p2,
                     string            font,
                     string            text
                    );   // mono spaced font constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Vertex   p1,
                     Vertex   p2,
                     string            font,
                     string            text,
                     int               dud
                    );   // variable spaced font constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     Short3d arc_start,
                     double            arc_angle // OK degrees, whatever!
                    );   // circular arc constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     Short3d arc_start,
                     double            arc_angle,
                     double            radii_ratio
                    );   // elliptical arc constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     Short3d first_vertex,
                     u_int             number_of_sides
                    );   // polygon constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     Short3d first_vertex,
                     u_int             number_of_sides,
                     double            ratio
                    );   // polystar constructor
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     int               radius,
                     int               pedals_numerator,
                     int               pedals_denominator
                    );   // rhodonea
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     int               center_radius,
                     Short3d center,
                     int               roller_radius,
                     int               roller_offset
                    );   // epitrochoid (and epicycloid)
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     int               center_radius,
                     int               roller_radius,
                     int               roller_offset,
                     Short3d center
                    );   // hypotrochoid (and hypocycloid)
    //------------------------------------------------------------------------
    Segment(Space*   ps,
                     Short3d center,
                     int               amplitude_x,
                     int               amplitude_y,
                     int               frequency_x,
                     int               frequency_y,
                     double            phase_x,
                     double            phase_y
                    );   // Lissajous curve
    //------------------------------------------------------------------------
virtual
   ~Segment()                                          {}
    //------------------------------------------------------------------------
    bool operator == (const Segment& segment)
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
    Segment&  operator += (const Segment& segment)
                        {
                            if(palette_index != segment.palette_index)
                                promote_to_true_color();
                            reserve(size() + segment.size());
                            insert(end(), segment.begin(), segment.end());
                            return *this;
                        }
    //------------------------------------------------------------------------
    Segment&  operator += (const Vertex& vertex)
                        {
                            push_back(vertex);
                            return *this;
                        }
    //------------------------------------------------------------------------
    Segment   operator + (const Segment& segment)
                        {
                            Segment sum(*this);
                            if(sum.palette_index != segment.palette_index)
                                sum.promote_to_true_color();
                            sum.reserve(sum.size() + segment.size());
                            sum.insert(end(), segment.begin(), segment.end());
                            return sum;
                        }
    //------------------------------------------------------------------------
    Segment   operator + (const Vertex& vertex)
                        {
                            Segment sum(*this);
                            sum.push_back(vertex);
                            return sum;
                        }
    //------------------------------------------------------------------------
    Segment&  operator =  (const Segment& segment)
                        {
                            clear();
//                            palette_index = segment.palette_index;
                            reserve(segment.size());
                            insert(begin(), segment.begin(), segment.end());
                            p_space = segment.p_space;
                            return *this;
                        }
    //------------------------------------------------------------------------
    bool is_2D() const
            {
                if(size() > 1)
                {
                    for(u_int i = 0; i < size(); i++)
                        if(at(i).z != 0)
                            return false;
                }
                return true;
            }
    //------------------------------------------------------------------------
    bool at_index_has_magnitude(u_int vertex_index) const
            {
                if(    vertex_index
                    && vertex_index < size()
                  )
                {
                    return (bool)(at(vertex_index - 1) | at(vertex_index));
                }
                else
                    return false;
            }
    //------------------------------------------------------------------------
    double total_magnitude() const
            {
                return (front() | back());
            }
    //------------------------------------------------------------------------
    Vertex first_lit_vector() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return at(i);
                        return Vertex(0);
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
    Vertex first_lit_anchor() const
                    {
                        for(u_int i = 1; i < size(); i++)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return Vertex(0);
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
    int last_lit_anchor_index() const
            {
                for(u_int i = size() - 1; i > 0; i--)
                    if(at(i).is_lit())
                        return i - 1;
                return -1;
            }
    //------------------------------------------------------------------------
    Vertex last_lit_anchor() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i - 1);
                        return Vertex(0);
                    }
    //------------------------------------------------------------------------
    Vertex last_lit_vector() const
                    {
                        for(u_int i = size() - 1; i > 0; i--)
                            if(at(i).is_lit())
                                return at(i);
                        return Vertex(0);
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
    int number_of_lit_vectors() const
            {
                int count = 0;
                for(u_int i = 1; i < size(); i++)
                    if(at(i).is_lit())
                        count++;
                return count;
            }
    //------------------------------------------------------------------------
    u_int number_of_color_vectors() const;
    u_int number_of_dark_vectors () const;
    //------------------------------------------------------------------------
    int number_of_blank_vectors() const
            {
                int count = 1; //first vertex is always an anchor
                for(u_int i = 1; i < size(); i++)
                    if(at(i).is_blank())
                        count++;
                return count;
            }
    //------------------------------------------------------------------------
    int number_of_unique_colors() const
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
                }
                return 0;
            }
    //------------------------------------------------------------------------
    Segment& remove_vertex(u_int vertex_index)
            {
                if(size() <= 2) // takes 2 vertices to make a vector!
                    clear();
                else if(    vertex_index >= 0
                         && vertex_index <  size()
                       )
                {
                    u_int            i;
                    Segment segment(p_space);
                    segment.reserve(size());
                    for(i = 0; i < vertex_index; i++)
                        segment += at(i);
                    for(i = vertex_index + 1; i < size(); i++)
                        segment += at(i);
                    *this = segment;
                }
                return *this;
            }
    //------------------------------------------------------------------------
    Short3d    segment_front                 () const ;
    Short3d    segment_back                  () const ;
    Short3d    segment_top                   () const ;
    Short3d    segment_bottom                () const ;
    Short3d    segment_right                 () const ;
    Short3d    segment_left                  () const ;
    Short3d    vertex_farthest_from_origin   () const ;
    short                segment_height                () const ;
    short                segment_width                 () const ;
    short                segment_depth                 () const ;
    short                segment_size                  () const ;
    bool                 is_closed_polygon             () const ;
    Segment&    reverse                       ();
    Segment&    reverse_sub_segment           (u_int p1, u_int p2);
    Segment&    reorder_from                  (u_int vertex_index);
    Segment&    randomize_segments            ();
    Segment&    conglomerate_lit_segments     ();
    Segment     shortest_path_of_segments     (const Segment& previous_frame);
    void                 blank_all_vertices            ();
    void                 unblank_all_vertices          ();
    void                 blacken_vertices              ();
    void                 reduce_blank_vectors          ();
    void                 omit_equivalent_vectors       ();
    void                 remove_dots                   ();
    ErrorCode  enhance_dots                  ();
    void                 remove_dwell_vertices         ();
    void                 remove_short_vectors          ();
    void                 reduce_lit_vectors            ();
    void                 impose_bit_resolution         ();
    ErrorCode  reorder_segments              (const Segment& previous_frame);
    ErrorCode  fracture_segments             ();
    ErrorCode  bond_segments                 ();
    ErrorCode  add_vertex                    (u_int  vertex_index);
    ErrorCode  break_segment                 (u_int& vertex_index);
    ErrorCode  connect_the_dots              (u_int p1, u_int p2); // vertex indices
    double               vector_angle                  (u_int vertex) const ;
    double               total_angle                   () const ;
    double               max_angle                     () const ;
    double               vector_magnitude              (u_int vertex) const ;
    double               total_distance                () const ;
    double               max_distance                  () const ;
    double               max_color_distance            () const ;
    double               max_dark_distance             () const ;
    double               length_in_time                () const ;
    ErrorCode  add_dwell                     ();
    ErrorCode  add_lit_span_vertices         ();
    ErrorCode  add_blank_span_vertices       ();
    void                 flip                          (u_int plane);
    void                 quarter_turn                  (u_int plane, u_int turns);
    void                 z_order_vertices              (unsigned short span);
    void                 flatten_z                     ();
    Bounds      rotate                        (Double3d a);
    Bounds      rotate_around_origin          (Double3d a);
    //------------------------------------------------------------------------
    Bounds      rotate_on_coordinates         (Double3d p,
                                                        Double3d a
                                                       );
    //------------------------------------------------------------------------
    Bounds      rotate_on_coordinates_x       (Double3d p, double a);
    Bounds      rotate_on_coordinates_y       (Double3d p, double a);
    Bounds      rotate_on_coordinates_z       (Double3d p, double a);
    Bounds      rotate_around_origin          ();
    //------------------------------------------------------------------------
    void                 ripple                        (int    direction,
                                                        double amplitude,
                                                        double freq     ,
                                                        double phase
                                                       );
    //------------------------------------------------------------------------
    Palette     as_color_table                () const ;
    void                 strip_color                   ();
    void                 strip_color_or                ();
    void                 strip_color_avg               ();
    void                 to_palette_by_index           (u_int index);
    void                 to_target_palette_by_index    ();
    void                 best_match_palette            (u_int index);
    void                 best_match_target_palette     ();
    void                 convert_black_to_blank        ();
    void                 convert_blank_to_black        ();
    void                 set_vertex_to_black           (u_int index);
    void                 impose_black_level            ();
    void                 rainbow_recolor               (int effect);
    Segment&    rotate_colors                 (int steps );
    bool                 find_rgb_in_palette           (const Palette& palette);
    void                 set_rgb_from_palette          ();
    void                 set_palette_to_332            ();
    void                 sync_rgb_and_palette          ();
    void                 bit_reduce_to_palette         ();
    void                 best_reduce_to_palette        ();
    void                 promote_to_true_color         ();
    void                 shade                         (u_char shade);
    void                 tint                          (u_char tint);
    bool                 color_from_bmp                (const string& file);
    void                 color_from_bmp                (struct Bitmap* bmp);
    bool                 subtract_bmp                  (const string& file);
    void                 subtract_bmp                  (struct Bitmap* bmp);
    //------------------------------------------------------------------------
    Bounds      move                          (Double3d d,
                                                        bool check_bounds = true
                                                       );
    //------------------------------------------------------------------------
    Bounds      move                          ();
    //------------------------------------------------------------------------
    Bounds      scale                         (Double3d s);
    Bounds      scale_on_coordinates          (Double3d p,
                                                        Double3d s
                                                       );
    //------------------------------------------------------------------------
    Bounds      scale_around_origin           (Double3d s);
    Bounds      scale_around_origin           ();
    Double3d   rectangular_center_of         () const ;
    Double3d   mean_of_coordinates           () const ;
    u_int                number_of_segments            () const ;
    //------------------------------------------------------------------------
    Segment     blend                         (const Segment& segment,
                                                        double ratio
                                                       );
    //------------------------------------------------------------------------
    bool                 find_segment_at_index         (u_int  index,
                                                        u_int& start,
                                                        u_int& end
                                                       ) const ;
    //------------------------------------------------------------------------
    bool                 find_segment_of_vertex        (u_int  vertex_index,
                                                        u_int& start,
                                                        u_int& end,
                                                        u_int& segment_index
                                                       ) const ;
    //------------------------------------------------------------------------
    Segment     copy_segment                  (u_int index) const ;
    FrameSet   explode_segments              () const ;
    Double3d   rectangular_center_of_segment (u_int index) const ;
    Double3d   mean_of_coordinates_of_segment(u_int index) const ;
    u_int                segment_index_of_vertex       (u_int index) const ;
    Bounds      move_segment                  (u_int index, Double3d d);
    Bounds      rotate_segment                (u_int index, Double3d a);
    Bounds      rotate_segment_around_origin  (u_int index, Double3d a);
    Bounds      scale_segment                 (u_int index, Double3d m);
    Bounds      scale_segment_around_origin   (u_int index, Double3d m);
    //------------------------------------------------------------------------
    void                 to_fstream_wave               (fstream&  out,
                                                        WaveHeader& header,
                                                        bool end_of_frame = false,
                                                        bool unique_frame = false
                                                       );
    //------------------------------------------------------------------------
    Space*      p_space;

    int                  palette_index;
    ErrorCode  segment_error;
    //------------------------------------------------------------------------
};

//############################################################################
Segment blank_segment(Space* ps);
Segment NULL_segment (Space* ps);

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
