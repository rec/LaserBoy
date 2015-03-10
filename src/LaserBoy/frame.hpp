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
// LaserBoy_frame.hpp is part of LaserBoy.
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

//############################################################################
class LaserBoy_frame : public LaserBoy_ild_header, public LaserBoy_segment
{
public:
    //------------------------------------------------------------------------
    LaserBoy_frame(LaserBoy_space* ps = NULL)
                 : LaserBoy_ild_header ()
                 , LaserBoy_segment    (ps)
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
    LaserBoy_frame(const LaserBoy_frame& frame)
                 : LaserBoy_ild_header ((LaserBoy_ild_header)frame)
                 , LaserBoy_segment    ((LaserBoy_segment   )frame)
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
    LaserBoy_frame(LaserBoy_segment& segment)
                 : LaserBoy_ild_header ()
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
    LaserBoy_frame(LaserBoy_space* ps,
                   const int& palette_index,
                   bool       add_origin
                  )
                 : LaserBoy_ild_header ()
                 , LaserBoy_segment    (ps, palette_index, add_origin)
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
    LaserBoy_frame(const LaserBoy_ild_header& header,
                   const LaserBoy_segment&    segment
                  )
                 : LaserBoy_ild_header (header)
                 , LaserBoy_segment    (segment)
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
    LaserBoy_frame(LaserBoy_space* ps,
                   LaserBoy_vertex(*F)(int, int),
                   int vertices_per_frame,
                   int frame_index
                  );
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_frame() {}
    //------------------------------------------------------------------------
    void normalize_cursors()
            {
                if(egg    >= size()) egg    = 0;
                if(spider >= size()) spider = 0;
            }
    //------------------------------------------------------------------------
    bool is_2D()
            {
                if(LaserBoy_segment::is_2D())
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
    LaserBoy_segment selected_segment() const
                        {
                            u_int            i;
                            LaserBoy_segment segment(p_space);
                            segment.reserve(size());
                            for(i = 0; i < size(); i++)
                                if(is_index_selected(i))
                                    segment += at(i);
                            return segment;
                        }
    //------------------------------------------------------------------------
    LaserBoy_frame& operator =  (const LaserBoy_frame& frame)
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
    LaserBoy_frame& operator =  (const LaserBoy_ild_header& header)
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
    LaserBoy_frame& operator =  (const LaserBoy_segment& segment)
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
    LaserBoy_frame& operator += (const LaserBoy_frame& frame)
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
    LaserBoy_frame& operator += (const LaserBoy_segment& segment)
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
    LaserBoy_frame& operator += (const LaserBoy_vertex& vertex)
                    {
                        push_back(vertex);
                        if(size() > LASERBOY_MAX_USHORT)
                            segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return *this;
                    }
    //------------------------------------------------------------------------
    LaserBoy_frame  operator +  (const LaserBoy_frame& frame) const
                    {
                        LaserBoy_frame sum(*this);
                        sum += frame;
                        if(sum.size() > LASERBOY_MAX_USHORT)
                            sum.segment_error = LASERBOY_VERTEX_COUNT_OVERFLOW;
                        return sum;
                    }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  add_vertex       (u_int vertex_index);
    LaserBoy_Error_Code  break_segment    ();
    LaserBoy_Error_Code  set_spider_to_egg();
    LaserBoy_Error_Code  set_egg_to_spider();
    LaserBoy_Error_Code  connect_the_dots ();
    LaserBoy_frame&      to_dots          ();
    LaserBoy_frame&      selected_to_dots ();
    LaserBoy_frame&      remove_at_spider ();
    LaserBoy_frame&      remove_vertex    (u_int vertex_index);
    //------------------------------------------------------------------------
    void reduce_blank_vectors()
            {
                LaserBoy_segment::reduce_blank_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void omit_equivalent_vectors()
            {
                LaserBoy_segment::omit_equivalent_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void remove_dots()
            {
                LaserBoy_segment::remove_dots();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code enhance_dots()
            {
                LaserBoy_Error_Code stat = LaserBoy_segment::enhance_dots();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    void remove_dwell_vertices()
            {
                LaserBoy_segment::remove_dwell_vertices();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void remove_short_vectors()
            {
                LaserBoy_segment::remove_short_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void reduce_lit_vectors()
            {
                LaserBoy_segment::reduce_lit_vectors();
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& reorder_from_egg()
            {
                LaserBoy_segment::reorder_from(egg);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& reorder_from_spider()
            {
                LaserBoy_segment::reorder_from(spider);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  reorder_segments(const LaserBoy_segment& previous_frame)
            {
                LaserBoy_segment::reorder_segments(previous_frame);
                normalize_cursors();
                return segment_error;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& randomize_segments()
            {
                LaserBoy_segment::randomize_segments();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& conglomerate_lit_segments()
            {
                LaserBoy_segment::conglomerate_lit_segments();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  fracture_segments()
            {
                LaserBoy_segment::fracture_segments();
                normalize_cursors();
                return segment_error;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  bond_segments()
            {
                LaserBoy_segment::bond_segments();
                normalize_cursors();
                return LASERBOY_OK;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& reverse()
            {
                LaserBoy_segment::reverse();
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_frame& reverse_selected()
            {
                LaserBoy_segment::reverse_sub_segment(egg, spider);
                normalize_cursors();
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code add_dwell()
            {
                LaserBoy_Error_Code stat = LaserBoy_segment::add_dwell();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code add_lit_span_vertices()
            {
                LaserBoy_Error_Code stat = LaserBoy_segment::add_lit_span_vertices();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code add_blank_span_vertices()
            {
                LaserBoy_Error_Code stat = LaserBoy_segment::add_blank_span_vertices();
                normalize_cursors();
                return stat;
            }
    //------------------------------------------------------------------------
    LaserBoy_Error_Code append_selected_segment()
            {
                LaserBoy_frame frame = *this;
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
    LaserBoy_Error_Code  optimize  (LaserBoy_3D_short point_of_entry);
    //------------------------------------------------------------------------
    void  add_coda                 (LaserBoy_3D_short next_frame_entry);
    //------------------------------------------------------------------------
    bool  from_ifstream_format_3   (ifstream&                  in,
                                    const LaserBoy_ild_header& header,
                                    long int&                  bytes_skipped
                                   );
    //------------------------------------------------------------------------
    bool  from_ifstream_ild        (ifstream& in,
                                    const LaserBoy_ild_header& header
                                   );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ifstream_dxf       (ifstream& in);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ifstream_txt       (ifstream&     in,
                                                  const u_int& group_type,
                                                  const u_int& element_type,
                                                  u_int&       line_number
                                                 );
    //------------------------------------------------------------------------
    LaserBoy_Bounds  move_selection              (LaserBoy_3D_double d,
                                                  bool               check_bounds = true
                                                 );
    //------------------------------------------------------------------------
    LaserBoy_Bounds  scale_selection             (LaserBoy_3D_double m);
    LaserBoy_Bounds  scale_selection_on_fulcrum  (LaserBoy_3D_double d);
    //------------------------------------------------------------------------
    LaserBoy_Bounds  rotate_selection            (LaserBoy_3D_double a);
    LaserBoy_Bounds  rotate_selection_on_fulcrum (LaserBoy_3D_double a);
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
                                         LaserBoy_wave_header& header,
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
    LaserBoy_segment  intro      ,
                      bridge     ,
                      coda       ;
};

//############################################################################
LaserBoy_frame blank_frame(LaserBoy_space* p_space);
LaserBoy_frame NULL_frame (LaserBoy_space* p_space);

//############################################################################
class LaserBoy_frame_set_base : public vector<LaserBoy_frame>
{
public:
    //------------------------------------------------------------------------
    LaserBoy_frame_set_base()  {}
    //------------------------------------------------------------------------
virtual
   ~LaserBoy_frame_set_base()  { clear(); }
    //------------------------------------------------------------------------
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
