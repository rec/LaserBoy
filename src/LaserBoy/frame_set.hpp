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
// FrameSet.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_FRAME_SET_DEFINITIONS__
#define __LASERBOY_FRAME_SET_DEFINITIONS__

//############################################################################
#include <LaserBoy/frame.hpp>

namespace LaserBoy {

//############################################################################
class FrameSet : public FrameSet_base
{
public:
    //------------------------------------------------------------------------
    FrameSet(Space* ps)
                     : p_space            (ps)
                     , frame_set_error    (LASERBOY_OK)
                     , num_2D_frames      ( 0)
                     , num_3D_frames      ( 0)
                     , frame_index        ( 0)
                     , frame_select_start (-1)
                     , frame_select_end   (-1)
                     , null_frame         (ps)
                     , ild_file_GUID      ("--------")
                        {
                            null_frame = NULL_frame(ps);
                        }
    //------------------------------------------------------------------------
    FrameSet(const FrameSet& f)
                     : p_space            (f.p_space)
                     , frame_set_error    (f.frame_set_error)
                     , num_2D_frames      (f.num_2D_frames)
                     , num_3D_frames      (f.num_3D_frames)
                     , frame_index        (f.frame_index)
                     , frame_select_start (f.frame_select_start)
                     , frame_select_end   (f.frame_select_end)
                     , null_frame         (f.p_space)
                     , ild_file_GUID      (f.ild_file_GUID)
                        {
                            null_frame = NULL_frame(f.p_space);
                            reserve(f.size());
                            insert(begin(), f.begin(), f.end());
                        }
    //------------------------------------------------------------------------
    FrameSet(Space* ps,
                       Vertex(*F)(int, int), // pointer to a function that generates a frame set
                       int vertices_per_frame,
                       int num_frames
                      );
    //------------------------------------------------------------------------
virtual
   ~FrameSet() {}
    //------------------------------------------------------------------------
    Frame& frame(int index) //sets frame_index
                    {
                        if(number_of_frames())
                        {
                            if(index < 0)
                                index = number_of_frames() - 1;
                            else if(index >= (int)number_of_frames())
                                index = 0;
                            frame_index = index;
                            return at(frame_index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& next_frame() //sets frame_index
                    {
                        if(number_of_frames())
                        {
                            frame_index++;
                            if(frame_index >= number_of_frames())
                                frame_index = 0;
                            return at(frame_index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& previous_frame() //sets frame_index
                    {
                        if(number_of_frames())
                        {
                            if(frame_index == 0)
                                frame_index = number_of_frames() - 1;
                            else
                                frame_index--;
                            return at(frame_index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& frame_picker(int index) // does not set frame_index
                    {
                        if(number_of_frames())
                        {
                            if(index < 0)
                                index = number_of_frames() - 1;
                            else if(index >= (int)number_of_frames())
                                index = 0;
                            return at(index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& current_frame()
                    {
                        if(number_of_frames())
                            return at(frame_index);
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    void  delete_current_frame()
            {
                if(number_of_frames() > 1)
                {
                    erase(begin() + frame_index);
                    if(frame_index > 0)
                        frame_index--;
                }
                else
                {
                    clear();
                    push_back(null_frame);
                }
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void  delete_frame(u_int index)
            {
                if(number_of_frames() > 1)
                {
                    if(index < number_of_frames())
                    {
                        erase(begin() + index);
                        if(frame_index >= index)
                        {
                            if(frame_index == 0)
                                frame_index = number_of_frames() - 1;
                            else
                                frame_index--;
                        }
                    }
                }
                else
                {
                    clear();
                    push_back(null_frame);
                }
                normalize_cursors();
                return;
            }
    //------------------------------------------------------------------------
    void  copy_frame()
            {
                insert(begin() + frame_index, frame_picker(frame_index));
                frame_index++;
                return;
            }
    //------------------------------------------------------------------------
    void  push_back_frame(const Frame& frame)
            {
                push_back(frame);
                return;
            }
    //------------------------------------------------------------------------
    void  count_frames()
            {
                num_2D_frames = num_3D_frames = 0;
                for(u_int i = 0; i < number_of_frames(); i++)
                    at(i).is_2D() ? num_2D_frames++ : num_3D_frames++;
                return;
            }
    //------------------------------------------------------------------------
    int   total_vertices() const
            {
                u_int  i,
                       vertices = 0;
                for(i = 0; i < number_of_frames(); i++)
                    vertices += (at(i).size());
                return vertices;
            }
    //------------------------------------------------------------------------
    Frame& pre_incr_current_frame()
                    {
                        if(number_of_frames())
                        {
                            frame_index++;
                            if(frame_index >= number_of_frames())
                                frame_index = 0;
                            return at(frame_index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& pre_decr_current_frame()
                    {
                        if(number_of_frames())
                        {
                            if(frame_index == 0)
                                frame_index = number_of_frames() - 1;
                            else
                                frame_index--;
                            return at(frame_index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& post_incr_current_frame(int)
                    {
                        if(number_of_frames())
                        {
                            u_int frame_return = frame_index;
                            frame_index++;
                            if(frame_index >= number_of_frames())
                                frame_index = 0;
                            return at(frame_return);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame& post_decr_current_frame(int)
                    {
                        if(number_of_frames())
                        {
                            u_int frame_return = frame_index;
                            if(frame_index == 0)
                                frame_index = number_of_frames() - 1;
                            else
                                frame_index--;
                            return at(frame_return);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    FrameSet& operator  = (const FrameSet& frames)
                        {
                            clear();
                            reserve(frames.size());
                            insert(begin(), frames.begin(), frames.end());
                            frame_select_start = frames.frame_select_start;
                            frame_select_end   = frames.frame_select_end;
                            num_2D_frames      = frames.num_2D_frames;
                            num_3D_frames      = frames.num_3D_frames;
                            return *this;
                        }
    //------------------------------------------------------------------------
    FrameSet& operator += (const FrameSet& frames)
                        {
                            reserve(size() + frames.size());
                            insert(end(), frames.begin(), frames.end());
                            return *this;
                        }
    //------------------------------------------------------------------------
    FrameSet& operator  = (const Frame& frame)
                        {
                            clear();
                            frame_select_start = -1;
                            frame_select_end   = -1;
                            push_back(frame);
                            return *this;
                        }
    //------------------------------------------------------------------------
    FrameSet& operator += (const Frame& frame);
    //------------------------------------------------------------------------
    FrameSet& operator += (Segment& segment)
                        {
                            ILDHeader header;
                            header.format = segment.is_2D();
                            push_back(Frame(header, segment));
                            return *this;
                        }
    //------------------------------------------------------------------------
    FrameSet selected_frames()
                        {
                            FrameSet  selected(p_space);
                            for(u_int i = 0; i < number_of_frames(); i++)
                                if(at(i).is_selected)
                                    selected.push_back(at(i));
                            return selected;
                        }
    //------------------------------------------------------------------------
    FrameSet  operator +  (const FrameSet& frames) const
                        {
                            FrameSet sum(*this);
                            sum += frames;
                            return sum;
                        }
    //------------------------------------------------------------------------
    Frame& operator [] (u_int index) // does not set frame_index
                    {
                        if(number_of_frames())
                        {
                            if(index >= number_of_frames())
                                index = 0;
                            return at(index);
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Frame selected_frame_index(u_int index)
                    {
                        if(number_of_frames())
                        {
                            u_int selected_count = 0,
                                  i;
                            if(index < number_of_frames())
                            {
                                for(i = 0; i < number_of_frames(); i++)
                                {
                                    if(at(i).is_selected)
                                    {
                                        if(index == selected_count)
                                            return at(i);
                                        selected_count++;
                                    }
                                }
                            }
                        }
                        return null_frame;
                    }
    //------------------------------------------------------------------------
    Segment sum_of_frames()
                        {
                            bool              same_palettes = true;
                            u_int             i;
                            Segment  segment(p_space, front().palette_index, false);
                            for(i = 1; i < number_of_frames(); i++)
                                if(segment.palette_index != at(i).palette_index)
                                {
                                    same_palettes = false;
                                    break;
                                }
                            if(!same_palettes)
                            {
                                segment.promote_to_true_color();
                                promote_to_true_color();
                            }
                            for(i = 0; i < number_of_frames(); i++)
                                segment += at(i);
                            return segment;
                        }
    //------------------------------------------------------------------------
    u_int number_of_frames() const {   return size();   }
    //------------------------------------------------------------------------
    int number_of_selected_frames() const
        {
            u_int selected_count = 0,
                  i;
            for(i = 0; i < number_of_frames(); i++)
                if(at(i).is_selected)
                    selected_count++;
            return selected_count;
        }
    //------------------------------------------------------------------------
    void                 render_frame       (const int& index) const;
    //------------------------------------------------------------------------
    FrameSet&  reverse            ();
    FrameSet&  make_current_first ();
    FrameSet&  swap_frames        (int i, int j);
    FrameSet&  reverse_selected   ();
    FrameSet&  random_order       ();
    FrameSet&  frame_reverse      ();
    //------------------------------------------------------------------------
    bool                 from_nothing       ();
    //------------------------------------------------------------------------
    ErrorCode  from_ild_file                  (const string& file,
                                                         long int&     bytes_skipped
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_txt_file                  (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    void                 superimpose_frame_set          (FrameSet& overlay);
    //------------------------------------------------------------------------
    ErrorCode  superimpose_from_txt_file      (const string& file,
                                                         int &new_frame_count,
                                                         int &new_palette_count
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_dxf_file                  (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_dxf_directory             (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_ifstream_ild              (ifstream& in,
                                                         long int& bytes_skipped
                                                        );
    //------------------------------------------------------------------------
    bool                 from_LaserBoy_wave_file        (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_qm_wave_file              (const string& file,
                                                         bool          global_polarity,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_unformatted_wave_file     (const string& file,
                                                         bool          global_polarity,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_fstream_LaserBoy_wave     (WaveHeader header,
                                                         fstream&             in,
                                                         bool                 append = false
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_fstream_unframed_wave     (WaveHeader header,
                                                         fstream&             in,
                                                         bool                 append = false
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_fstream_qm_wave           (WaveHeader header,
                                                         fstream&             in,
                                                         bool                 global_polarity,
                                                         bool                 append = false
                                                        );
    //------------------------------------------------------------------------
    ErrorCode  from_fstream_unformatted_wave  (WaveHeader header,
                                                         fstream&             in,
                                                         bool                 global_polarity,
                                                         bool                 append = false
                                                        );
    //------------------------------------------------------------------------
    void normalize_cursors()
            {
                if(frame_index >= size())
                    frame_index = 0;
                if(frame_select_start >= (int)size())
                    frame_select_start = -1;
                if(frame_select_end >= (int)size())
                    frame_select_end = -1;
            }
    //------------------------------------------------------------------------
    bool    is_2D                           (                        )       ;
    void    flatten_z                       (                        )       ;
    void    z_order_vertices                (unsigned short span     )       ;
    void    unblank_all_vertices            (                        )       ;
    void    convert_black_to_blank          (                        )       ;
    void    convert_blank_to_black          (                        )       ;
    void    impose_black_level              (                        )       ;
    void    reduce_blank_vectors            (                        )       ;
    void    remove_dots                     (                        )       ;
    //------------------------------------------------------------------------
    ErrorCode     enhance_dots    (                        )       ;
    //------------------------------------------------------------------------
    void    remove_dwell_vertices           (                        )       ;
    void    remove_short_vectors            (                        )       ;
    void    reduce_lit_vectors              (                        )       ;
    void    minimize                        (                        )       ;
    void    to_dots                         (                        )       ;
    void    impose_bit_resolution           (                        )       ;
    void    explode_current_frame           (                        )       ;
    void    explode_all_frames              (                        )       ;
    //------------------------------------------------------------------------
    ErrorCode  collapse_selected_frames(                   )       ;
    //------------------------------------------------------------------------
    void    delete_every_nth_frame          (unsigned short n        )       ;
    void    keep_every_nth_frame            (unsigned short n        )       ;
    //------------------------------------------------------------------------
    ErrorCode  fracture_segments  (                        )       ;
    //------------------------------------------------------------------------
    void    omit_equivalent_vectors         (                        )       ;
    //------------------------------------------------------------------------
    ErrorCode     bond_segments   (                        )       ;
    ErrorCode     reorder_segments(                        )       ;
    //------------------------------------------------------------------------
    void    randomize_segments              (                        )       ;
    void    conglomerate_lit_segments       (                        )       ;
    //------------------------------------------------------------------------
    ErrorCode  add_dwell              (                    )       ;
    ErrorCode  add_lit_span_vertices  (                    )       ;
    ErrorCode  add_blank_span_vertices(                    )       ;
    //------------------------------------------------------------------------
    void    delete_redundant_frames         (                        )       ;
    //------------------------------------------------------------------------
    ErrorCode  optimize           (                        )       ;
    //------------------------------------------------------------------------
    void    rainbow_recolor                 (int effect              )       ;
    void    quarter_turn                    (u_int plane, u_int turns)       ;
    //------------------------------------------------------------------------
    Bounds  scale_around_origin    (Double3d f    )       ;
    Bounds  scale_around_origin    (                        )       ;
    Bounds  rotate_around_origin   (Double3d f    )       ;
    Bounds  rotate_around_origin   (                        )       ;
    Bounds  move                   (Double3d f    )       ;
    Bounds  move                   (                        )       ;
    //------------------------------------------------------------------------
    void    toggle_frame_select_start       (                        )       ;
    void    toggle_frame_select_end         (                        )       ;
    void    invert_frame_selections         (                        )       ;
    void    delete_selected_frames          (                        )       ;
    void    trim_to_selected_frames         (                        )       ;
    void    bit_reduce_to_palette           (                        )       ;
    void    best_reduce_to_palette          (                        )       ;
    void    promote_to_true_color           (                        )       ;
    void    best_match_palette              (u_int index             )       ;
    void    best_match_target_palette       (                        )       ;
    void    strip_color                     (                        )       ;
    void    strip_color_or                  (                        )       ;
    void    strip_color_avg                 (                        )       ;
    void    to_palette_by_index             (u_int index             )       ;
    void    to_target_palette_by_index      (                        )       ;
    bool    color_from_bmp                  (const string& file      )       ;
    void    color_from_bmp                  (struct Bitmap* bmp)       ;
    bool    subtract_bmp                    (const string& file      )       ;
    void    subtract_bmp                    (struct Bitmap* bmp)       ;
    void    flip                            (int plane               )       ;
    void    renumber                        (                        )       ;
    bool    save_as_ild                     (const string& file      )       ;
    bool    save_as_txt                     (const string& file      )       ;
    void    to_ofstream_ild                 (ofstream& out           )       ;
    void    to_ofstream_txt                 (ofstream& out           )       ;
    //------------------------------------------------------------------------
    bool    save_as_wave                    (const string& file,
                                             bool   optimized,
                                             bool   timed
                                            )                                ;
    //------------------------------------------------------------------------
    void    to_fstream_wave                 (fstream& out,
                                             bool     optimized,
                                             bool     timed
                                            )                                ;
    //------------------------------------------------------------------------
    bool    save_as_bmp_directory           (const string& dir       ) const ;
    bool    save_as_dxf_directory           (const string& dir       ) const ;
    //------------------------------------------------------------------------
    Space*      p_space           ;
    ErrorCode  frame_set_error   ;
    u_int                num_2D_frames     ,
                         num_3D_frames     ,
                         frame_index       ;
    int                  frame_select_start,
                         frame_select_end  ;
    Frame       null_frame        ;
    string               ild_file_GUID     ;
};

//############################################################################
typedef Bounds(*Frame_effect    )(Frame*    ); // function prototyes
typedef Bounds(*FrameSet_effect)(FrameSet*);

} // namespace LaserBoy

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
