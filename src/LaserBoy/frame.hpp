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
// frame.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_FRAME_DEFINITIONS__
#define __LASERBOY_FRAME_DEFINITIONS__

//############################################################################
#include <LaserBoy/segment.hpp>

namespace LaserBoy {

//############################################################################
class Frame : public ILDHeader, public Segment
{
public:
    //------------------------------------------------------------------------
    Frame(Space* ps = NULL)
                 : ILDHeader ()
                 , Segment    (ps)
                 , is_selected         (false)
                 , is_unique           (true)
                 , is_wagged           (false)
                 , egg                 (0)
                 , spider              (0)
                 , intro               (ps)
                 , bridge              (ps)
                 , coda                (ps)
                     {}
    //------------------------------------------------------------------------
    Frame(const Frame& frame)
                 : ILDHeader ((ILDHeader)frame)
                 , Segment    ((Segment   )frame)
                 , is_selected         (false)
                 , is_unique           (frame.is_unique)
                 , is_wagged           (frame.is_wagged)
                 , egg                 (frame.egg)
                 , spider              (frame.spider)
                 , intro               (frame.intro)
                 , bridge              (frame.bridge)
                 , coda                (frame.coda)
                     {}
    //------------------------------------------------------------------------
    Frame(Segment& segment)
                 : ILDHeader ()
                 , is_selected         (false)
                 , is_unique           (true)
                 , is_wagged           (false)
                 , egg                 (0)
                 , spider              (0)
                 , intro               (segment.p_space)
                 , bridge              (segment.p_space)
                 , coda                (segment.p_space)
                     {
                         *this = segment;
                         is_2D();
                     }
    //------------------------------------------------------------------------
    Frame(Space* ps,
                   const int& palette_index,
                   bool       add_origin
                  )
                 : ILDHeader ()
                 , Segment    (ps, palette_index, add_origin)
                 , is_selected         (false)
                 , is_unique           (true)
                 , is_wagged           (false)
                 , egg                 (0)
                 , spider              (0)
                 , intro               (ps)
                 , bridge              (ps)
                 , coda                (ps)
                     {}
    //------------------------------------------------------------------------
    Frame(const ILDHeader& header,
                   const Segment&    segment
                  )
                 : ILDHeader (header)
                 , Segment    (segment)
                 , is_selected         (false)
                 , is_unique           (true)
                 , is_wagged           (false)
                 , egg                 (0)
                 , spider              (0)
                 , intro               (segment.p_space)
                 , bridge              (segment.p_space)
                 , coda                (segment.p_space)
                     {}
    //------------------------------------------------------------------------
    Frame(Space* ps,
                   Vertex(*F)(int, int),
                   int vertices_per_frame,
                   int frame_index
                  );
    //------------------------------------------------------------------------
virtual
   ~Frame() {}
    //------------------------------------------------------------------------
    void normalize_cursors()
            {
                if(egg    >= size()) egg    = 0;
                if(spider >= size()) spider = 0;
            }
    //------------------------------------------------------------------------
    bool is_2D()
            {
                if(Segment::is_2D())
                    format = LASERBOY_2D_FRAME;
                else
                    format = LASERBOY_3D_FRAME;
                return (bool)format; // 3D = 0, 2D = 1
            }
    //------------------------------------------------------------------------
    bool is_index_selected(u_int index) const
            {
                if(index < size())
                {
                    if     (    egg   == spider
                             && index == egg
                           )
                        return true;
                    else if(    egg   <  spider
                             && index >= egg
                             && index <= spider
                           )
                        return true;
                    else if(    index <= egg
                             && index >= spider
                           )
                        return true;
                }
                return false;
            }
    //------------------------------------------------------------------------
    int  size_of_selection() const
            {
                if(egg == spider)
                    return 1;
                else if(egg < spider)
                    return spider - egg;
                else
                    return egg - spider;
            }
    //------------------------------------------------------------------------
    Segment selected_segment() const
                        {
                            u_int            i;
                            Segment segment(p_space);
                            segment.reserve(size());
                            for(i = 0; i < size(); i++)
                                if(is_index_selected(i))
                                    segment += at(i);
                            return segment;
                        }
    //------------------------------------------------------------------------
    Frame& operator =  (const Frame& frame)
                    {
                        clear();
                        format        = frame.format;
                        identity      = frame.identity;
                        total         = frame.total;
                        scanner       = frame.scanner;
                        future        = 0;
                        segment_error = frame.segment_error;
                        palette_index = frame.palette_index;
                        reserve(frame.size());
                        insert(begin(), frame.begin(), frame.end());
                        egg           = frame.egg;
                        spider        = frame.spider;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame& operator =  (const ILDHeader& header)
                    {
                        format        = header.format  ;
                        name          = header.name    ;
                        owner         = header.owner   ;
                        quantity      = header.quantity;
                        identity      = header.identity;
                        total         = header.total   ;
                        scanner       = header.scanner ;
                        future        = 0              ;
                        segment_error = LASERBOY_OK    ;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame& operator =  (const Segment& segment)
                    {
                        clear();
                        palette_index = segment.palette_index;
                        reserve(segment.size());
                        insert(begin(), segment.begin(), segment.end());
                        format        = is_2D();
                        segment_error = segment.segment_error;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame& operator += (const Frame& frame)
                    {
                        reserve(size() + frame.size());
                        if(palette_index != frame.palette_index)
                            promote_to_true_color();
                        insert(end(), frame.begin(), frame.end());
                        if(size() > LASERBOY_MAX_USHORT)
                            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame& operator += (const Segment& segment)
                    {
                        reserve(size() + segment.size());
                        if(palette_index != segment.palette_index)
                            promote_to_true_color();
                        insert(end(), segment.begin(), segment.end());
                        if(size() > LASERBOY_MAX_USHORT)
                            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame& operator += (const Vertex& vertex)
                    {
                        push_back(vertex);
                        if(size() > LASERBOY_MAX_USHORT)
                            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return *this;
                    }
    //------------------------------------------------------------------------
    Frame  operator +  (const Frame& frame) const
                    {
                        Frame sum(*this);
                        sum += frame;
                        if(sum.size() > LASERBOY_MAX_USHORT)
                            sum.segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return sum;
                    }
    //------------------------------------------------------------------------
    ErrorCode  add_vertex       (u_int vertex_index);
    ErrorCode  break_segment    ();
    ErrorCode  set_spider_to_egg();
    ErrorCode  set_egg_to_spider();
    ErrorCode  connect_the_dots ();
    Frame&      to_dots          ();
    Frame&      selected_to_dots ();
    Frame&      remove_at_spider ();
    Frame&      remove_vertex    (u_int vertex_index);
    //------------------------------------------------------------------------
    void reduce_blank_vectors()
            {
                Segment::reduce_blank_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void omit_equivalent_vectors()
            {
                Segment::omit_equivalent_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void remove_dots()
            {
                Segment::remove_dots();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    ErrorCode enhance_dots()
            {
                ErrorCode stat = Segment::enhance_dots();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    void remove_dwell_vertices()
            {
                Segment::remove_dwell_vertices();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void remove_short_vectors()
            {
                Segment::remove_short_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void reduce_lit_vectors()
            {
                Segment::reduce_lit_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    Frame& reorder_from_egg()
            {
                Segment::reorder_from(egg);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    Frame& reorder_from_spider()
            {
                Segment::reorder_from(spider);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    ErrorCode  reorder_segments(const Segment& previous_frame)
            {
                Segment::reorder_segments(previous_frame);
                normalize_cursors();
                return segment_error;
            }
    //------------------------------------------------------------------------
    Frame& randomize_segments()
            {
                Segment::randomize_segments();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    Frame& conglomerate_lit_segments()
            {
                Segment::conglomerate_lit_segments();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    ErrorCode  fracture_segments()
            {
                Segment::fracture_segments();
                normalize_cursors();
                return segment_error;
            }
    //------------------------------------------------------------------------
    ErrorCode  bond_segments()
            {
                Segment::bond_segments();
                normalize_cursors();
                return LASERBOY_OK;
            }
    //------------------------------------------------------------------------
    Frame& reverse()
            {
                Segment::reverse();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    Frame& reverse_selected()
            {
                Segment::reverse_sub_segment(egg, spider);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    ErrorCode add_dwell()
            {
                ErrorCode stat = Segment::add_dwell();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    ErrorCode add_lit_span_vertices()
            {
                ErrorCode stat = Segment::add_lit_span_vertices();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    ErrorCode add_blank_span_vertices()
            {
                ErrorCode stat = Segment::add_blank_span_vertices();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    ErrorCode append_selected_segment()
            {
                Frame frame = *this;
                frame += selected_segment();
                if(frame.size() <= LASERBOY_MAX_USHORT)
                    *this = frame;
                else
                    return LASERBOY_VERTEX_COUNT_OVERFLOW;
                normalize_cursors();
                return LASERBOY_OK;
            }
    //------------------------------------------------------------------------
    void  increment_egg()
            {
                egg++;
                if(egg >= size())
                    egg = 0;
            }
    //------------------------------------------------------------------------
    void  deccrement_egg()
            {
                if(egg == 0)
                    egg = size() - 1;
                else
                    egg--;
            }
    //------------------------------------------------------------------------
    void  increment_spider()
            {
                spider++;
                if(spider >= size())
                    spider = 0;
            }
    //------------------------------------------------------------------------
    void  deccrement_spider()
            {
                if(spider == 0)
                    spider = size() - 1;
                else
                    spider--;
            }
    //------------------------------------------------------------------------
    bool  save_as_ild              (const string& file);
    bool  save_as_txt              (const string& file);
    bool  save_as_txt_table        (const string& file);
    //------------------------------------------------------------------------
    ErrorCode  optimize  (Short3d point_of_entry);
    //------------------------------------------------------------------------
    void  add_coda                 (Short3d next_frame_entry);
    //------------------------------------------------------------------------
    bool  from_ifstream_format_3   (ifstream&                  in,
                                    const ILDHeader& header,
                                    long int&                  bytes_skipped
                                   );
    //------------------------------------------------------------------------
    bool  from_ifstream_ild        (ifstream& in,
                                    const ILDHeader& header
                                   );
    //------------------------------------------------------------------------
    ErrorCode  from_ifstream_dxf       (ifstream& in);
    //------------------------------------------------------------------------
    ErrorCode  from_ifstream_txt       (ifstream&     in,
                                                  const u_int& group_type,
                                                  const u_int& element_type,
                                                  u_int&       line_number
                                                 );
    //------------------------------------------------------------------------
    Bounds  move_selection              (Double3d d,
                                                  bool               check_bounds = true
                                                 );
    //------------------------------------------------------------------------
    Bounds  scale_selection             (Double3d m);
    Bounds  scale_selection_on_fulcrum  (Double3d d);
    //------------------------------------------------------------------------
    Bounds  rotate_selection            (Double3d a);
    Bounds  rotate_selection_on_fulcrum (Double3d a);
    //------------------------------------------------------------------------
    void next_segment_select            ();
    void next_segment_egg               ();
    void next_segment_spider            ();
    void previous_segment_select        ();
    void previous_segment_egg           ();
    void previous_segment_spider        ();
    //------------------------------------------------------------------------
    void color_select                   ();
    void color_index_select             ();
    void color_span_select              ();
    void color_index_black              ();
    void color_span_black               ();
    void color_black_in_select          ();
    void color_cycle_select             (int steps);
    void blank_vertices                 ();
    void unblank_vertices               ();
    void black_vertices                 ();
    void unblack_vertices               ();
    void minimize                       (int frame_index);
    void to_ofstream_ild                (ofstream& out);
    void to_ofstream_txt_color_table    (ofstream& out);
    void to_ofstream_txt                (ofstream& out, int frame_index);
    void render                         (int skin                         ) const ;
    //------------------------------------------------------------------------
    bool save_as_wave                   (const string& file_name,
                                         bool optimized,
                                         bool timed
                                        );
    //------------------------------------------------------------------------
    void to_fstream_wave                (fstream& out,
                                         WaveHeader& header,
                                         bool optimized,
                                         bool timed
                                        );
    //------------------------------------------------------------------------
    bool save_as_dxf                    (const string& file_name          ) const ;
    bool save_as_bmp                    (const string& file_name          ) const ;
    void stats                          (bool show_cursors                ) const ;
    //------------------------------------------------------------------------
    bool              is_selected,
                      is_unique  ,
                      is_wagged  ;
    u_int             egg        ,
                      spider     ;
    Segment  intro      ,
                      bridge     ,
                      coda       ;
};

//############################################################################
Frame blank_frame(Space* p_space);
Frame NULL_frame (Space* p_space);

//############################################################################
class FrameSet_base : public vector<Frame>
{
public:
    //------------------------------------------------------------------------
    FrameSet_base()  {}
    //------------------------------------------------------------------------
virtual
   ~FrameSet_base()  { clear(); }
    //------------------------------------------------------------------------
};

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
