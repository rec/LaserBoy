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
// frame_set_effects.cpp is part of LaserBoy.
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
#include <LaserBoy/space.hpp>

namespace LaserBoy {

//############################################################################
Bounds spokes(FrameSet* p_frame_set)
{
    u_int               frame_count,
                        spoke;
    Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
    Double3d  a(0, 0, pi/6);
    FrameSet  out  (p_frame_set->p_space);
    Frame      frame(p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        out += p_frame_set->at(frame_count);
        for(spoke = 1; spoke < 12; spoke++)
        {
            frame = p_frame_set->at(frame_count);
            out_of_bounds |= frame.rotate_around_origin(a * spoke);
            if(out_of_bounds)
                return out_of_bounds;
            out[frame_count] += frame;
        }
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "spokes.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds gyro(FrameSet* p_frame_set)
{
    u_int               frame_count;
    Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
    Double3d  a(one_degree, one_degree, one_degree);
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        out_of_bounds |= frame.rotate_around_origin(a * frame_count);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "gyro.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds glitter(FrameSet* p_frame_set)
{
    u_int               frame_count,
                        vertex_count;
    Frame      frame_1(p_frame_set->p_space),
                        frame_2(p_frame_set->p_space);
    FrameSet  out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    frame_1.promote_to_true_color();
    frame_2.promote_to_true_color();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame_1 = p_frame_set->at(frame_count);
        frame_2.clear();
        for(vertex_count = 0; vertex_count < frame_1.size(); vertex_count++)
        {
            frame_2.push_back(frame_1.at(vertex_count));
            if(frame_1.at(vertex_count).is_color(0))
            {
                frame_2.push_back(frame_1.at(vertex_count));
                frame_2.back().r =
                frame_2.back().g =
                frame_2.back().b = 255;
                frame_2.push_back(frame_1.at(vertex_count));
                frame_2.back().blank();
            }
        }
        out += frame_2;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "glitter.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds lava(FrameSet* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               frame_count,
                            color_index,
                            vertex_index;
        int                 vertex_count;
        Short3d   temp_position;
        Frame      frame(p_frame_set->p_space),
                            object = p_frame_set->selected_frame_index(0),
                            colors = p_frame_set->selected_frame_index(1);
        FrameSet  out(frame.p_space);
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = vertex_count - (object.size() - frame_count + 1);
                vertex_index = vertex_count;
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)(object.size() + colors.size());
                vertex_count++
               )
            {
                color_index = vertex_count - (object.size() - frame_count + 1);
                vertex_index = vertex_count % object.size();
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < colors.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = colors.size() - (frame_count + 1);
                vertex_count >= 0;
                vertex_count--
               )
            {
                color_index = (colors.size() - 1) - (colors.size() - (frame_count + 1) - vertex_count);
                if(vertex_count < (int)object.size())
                {
                    frame.at(vertex_count).r = colors.at(color_index).r;
                    frame.at(vertex_count).g = colors.at(color_index).g;
                    frame.at(vertex_count).b = colors.at(color_index).b;
                    frame.at(vertex_count).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_count).blank();
                }
            }
            out += frame;
        }
        out.save_as_ild(LASERBOY_ILD_SHARE + "lava.ild");
        return LASERBOY_IN_BOUNDS;
        //--------------------------------------------------------------------
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds lava2(FrameSet* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               frame_count,
                            color_index,
                            vertex_index;
        int                 vertex_count;
        Short3d   temp_position;
        Frame      frame(p_frame_set->p_space),
                            object = p_frame_set->selected_frame_index(0),
                            colors = p_frame_set->selected_frame_index(1);
        FrameSet  out(frame.p_space);
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = (   vertex_count
                                - (   object.size()
                                    - frame_count
                                    + 1
                                  )
                              )
                              % colors.size();

                vertex_index = vertex_count;
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < colors.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = 0;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = (   (object.size() - 1)
                                - (object.size() - (frame_count + 1) - vertex_count)
                              )
                              % colors.size();
                vertex_index = vertex_count % object.size();
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //--------------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - (frame_count + 1);
                vertex_count >= 0;
                vertex_count--
               )
            {
                color_index = (   (object.size() - 1)
                                - (object.size() - (frame_count + 1) - vertex_count)
                              )
                              % colors.size();
                vertex_index = vertex_count % object.size();
                frame.at(vertex_index).r = colors.at(color_index).r;
                frame.at(vertex_index).g = colors.at(color_index).g;
                frame.at(vertex_index).b = colors.at(color_index).b;
                frame.at(vertex_index).c = colors.at(color_index).c;
                if(colors.at(color_index).is_blank())
                    frame.at(vertex_index).blank();
            }
            out += frame;
        }
        //--------------------------------------------------------------------
        out.save_as_ild(LASERBOY_ILD_SHARE + "lava2.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds rocker(FrameSet* p_frame_set)
{
    bool                flip_flop = true;
    int                 i = 0;
    u_int               frame_count;
    Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
    Double3d  a(0, 0, one_degree);
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        out_of_bounds |= frame.rotate_around_origin(a * i);
        if(out_of_bounds)
            return out_of_bounds;
        if(!((frame_count - 45) % 90))
            flip_flop = !flip_flop;
        if(flip_flop)
            i++;
        else
            i--;
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rocker.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds echos(FrameSet* p_frame_set)
{
    u_int               i,
                        echo,
                        delay     =  1,
                        num_echos = 10;
    Frame      empty_frame(p_frame_set->p_space, p_frame_set->front().palette_index, true); // not really!
    FrameSet  out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(i = 0; i < (p_frame_set->number_of_frames() + (delay * num_echos)); i++)
    {
        if(i < p_frame_set->number_of_frames())
            out += p_frame_set->at(i);
        else
            out += empty_frame;
        for(echo = 1; echo <= num_echos; echo++)
            if(    i >= (delay * echo)
                && (i - delay * echo < p_frame_set->number_of_frames())
              )
                out[i] += p_frame_set->at(i - delay * echo);
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "echos.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds gears(FrameSet* p_frame_set)
{ // requires a frame set with three selected frames (Try in.ild, 1, 2 & 3)
    if(p_frame_set->number_of_selected_frames() > 2)
    {
        Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        FrameSet  out(p_frame_set->p_space);
        Frame g20 = p_frame_set->selected_frame_index(0);
        Frame g16 = p_frame_set->selected_frame_index(1);
        Frame g12 = p_frame_set->selected_frame_index(2);
        Frame f20(p_frame_set->p_space);
        Frame f16(p_frame_set->p_space);
        Frame f12(p_frame_set->p_space);
        Double3d a(0, 0, -pi/60);
        Double3d b(0, 0,  pi/48);
        Double3d c(0, 0,  pi/36);
        //--------------------------------------------------------------------
        g20.scale_around_origin(Double3d(0.500, 0.500, 0.500));
        g16.scale_around_origin(Double3d(0.375, 0.375, 0.375));
        g12.scale_around_origin(Double3d(0.300, 0.300, 0.300));
        g20.move(Short3d(-10000,      0, 0));
        g16.move(Short3d( 12200, -15400, 0));
        g12.move(Short3d(  5150,  19300, 0));

        out += (g20 + g16 + g12);
        for(int i = 1; i < 480; i++)
        {
            f20 = g20;
            f16 = g16;
            f12 = g12;
            out_of_bounds |= f20.rotate(a * i);
            out_of_bounds |= f16.rotate(b * i);
            out_of_bounds |= f12.rotate(c * i);
            if(out_of_bounds)
                return out_of_bounds;
            out += (f20 + f16 + f12);
        }
        out.save_as_ild(LASERBOY_ILD_SHARE + "gears.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds blend(FrameSet* p_frame_set)
{ // requires a frame set with at least two selected frames
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               i,
                            k;
        Frame      f1(p_frame_set->p_space),
                            f2(p_frame_set->p_space),
                            frame(p_frame_set->p_space);
        FrameSet  in(p_frame_set->selected_frames()),
                            out(p_frame_set->p_space);
        //--------------------------------------------------------------------
        for(k = 0; k < in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            frame.clear();
            //----------------------------------------------------------------
            for(i = 0; i < 10; i++)
                out += f1;
            for(i = 0; i < 100; i++)
            {
                frame = f1.blend(f2, i / 99.0);
                out += frame;
            }
            //----------------------------------------------------------------
        }
        f1 = in[in.size() - 1];
        f2 = in[0];
        frame.clear();
        //--------------------------------------------------------------------
        for(i = 0; i < 10; i++)
            out += f1;
        for(i = 0; i < 100; i++)
        {
            frame = f1.blend(f2, i / 99.0);
            out += frame;
        }
        //--------------------------------------------------------------------
        out.save_as_ild(LASERBOY_ILD_SHARE + "blend.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds move_l2r(FrameSet* p_frame_set)
{
    int                 i,
                        j,
                        frame_index;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    frame_index = in.number_of_frames() - 1;
    d.x = -662;
    for(i = 0; i < 100; i++)
    {
        frame = in.at(frame_index);
        for(j = 0; j < i; j++)
            frame.move(d, false);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(--frame_index < 0)
            frame_index = in.number_of_frames() - 1;
    }
    out.reverse();
    //------------------------------------------------------------------------
    d.x = 662;
    frame_index = 0;
    for(i = 1; i <= 100; i++)
    {
        frame = in.at(frame_index);
        for(j = 0; j < i; j++)
            frame.move(d, false);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(++frame_index >= (int)in.number_of_frames())
            frame_index = 0;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "move_l2r.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds move_t2b(FrameSet* p_frame_set)
{
    int                 i,
                        j,
                        frame_index;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    frame_index = in.number_of_frames() - 1;
    d.y = 662;
    for(i = 0; i < 100; i++)
    {
        frame = in.at(frame_index);
        for(j = 0; j < i; j++)
            frame.move(d, false);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).y >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(--frame_index < 0)
            frame_index = in.number_of_frames() - 1;
    }
    out.reverse();
    //------------------------------------------------------------------------
    frame_index = 0;
    d.y = -662;
    for(i = 1; i <= 100; i++)
    {
        frame = in.at(frame_index);
        for(j = 0; j < i; j++)
            frame.move(d, false);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).y <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(++frame_index >= (int)in.number_of_frames())
            frame_index = 0;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "move_t2b.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds wipe(FrameSet* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        Frame      f1(p_frame_set->p_space),
                            f2(p_frame_set->p_space);
        FrameSet  s1(p_frame_set->p_space),
                            s2(p_frame_set->p_space),
                            in(p_frame_set->selected_frames()),
                            out(p_frame_set->p_space);
        //--------------------------------------------------------------------
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(f1.at(j).x <= ((i * 661.98) - 32767.0))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            //----------------------------------------------------------------
            f2.flip(0);
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(f2.at(j).x <= ((i * 661.98) - 32767.0))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            //----------------------------------------------------------------
            s2.flip(0);
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //--------------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //--------------------------------------------------------------------
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(f1.at(j).x <= ((i * 661.98) - 32767.0))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        //--------------------------------------------------------------------
        f2.flip(0);
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(f2.at(j).x <= ((i * 661.98) - 32767.0))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        //--------------------------------------------------------------------
        s2.flip(0);
        s2.reverse();
        //--------------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //--------------------------------------------------------------------
        out.save_as_ild(LASERBOY_ILD_SHARE + "wipe.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds radial_wipe(FrameSet* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        Frame      f1(p_frame_set->p_space),
                            f2(p_frame_set->p_space);
        FrameSet  s1(p_frame_set->p_space),
                            s2(p_frame_set->p_space),
                            in(p_frame_set->selected_frames()),
                            out(p_frame_set->p_space);
        //--------------------------------------------------------------------
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(f1.at(j).magnitude() <= (i * 468.09))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            //----------------------------------------------------------------
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(f2.at(j).magnitude() >= (i * 468.09))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //--------------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //--------------------------------------------------------------------
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(f1.at(j).magnitude() <= (i * 468.09))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        //--------------------------------------------------------------------
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(f2.at(j).magnitude() >= (i * 468.09))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        s2.reverse();
        //--------------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //--------------------------------------------------------------------
        out.save_as_ild(LASERBOY_ILD_SHARE + "radial_wipe.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds angle_wipe(FrameSet* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        Frame      f1(p_frame_set->p_space),
                            f2(p_frame_set->p_space);
        FrameSet  s1(p_frame_set->p_space),
                            s2(p_frame_set->p_space),
                            in(p_frame_set->selected_frames()),
                            out(p_frame_set->p_space);
        //--------------------------------------------------------------------
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            f1.flip(0);
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(((atan2(f1.at(j).y, f1.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            s1.flip(0);
            //----------------------------------------------------------------
            f2.quarter_turn(0, 2);
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(((atan2(f2.at(j).y, f2.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            s2.quarter_turn(0, 2);
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //--------------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //--------------------------------------------------------------------
        f1.flip(0);
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(((atan2(f1.at(j).y, f1.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        s1.flip(0);
        //--------------------------------------------------------------------
        f2.quarter_turn(0, 2);
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(((atan2(f2.at(j).y, f2.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        s2.quarter_turn(0, 2);
        s2.reverse();
        //--------------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //--------------------------------------------------------------------
        out.save_as_ild(LASERBOY_ILD_SHARE + "angle_wipe.ild");
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NO_SELECTED_FRAMES;
}

//############################################################################
Bounds vortex(FrameSet* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    Double3d  s(0.80, 0.80, 0.80);
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 19; shrink_frame_count++)
        {
            out += frame;
            frame.scale_around_origin(s);
        }
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "vortex.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds fader(FrameSet* p_frame_set)
{
    double              frame_count,
                        total = p_frame_set->number_of_frames();
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        frame.shade((u_char)(frame_count / total * 255.0));
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "fader.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds fader_z(FrameSet* p_frame_set)
{
    u_int               i,
                        shade;
    double              frame_count,
                        total = p_frame_set->number_of_frames();
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        frame.promote_to_true_color();
        for(i = 1; i < frame.size(); i++)
        {
            shade = (int)(255 - (frame.at(i).z + 22768.0) / 45536.0 * 255.0);
            if(shade > 254)    shade = 255;
            if(shade < 1  )    shade = 0  ;
            (frame.at(i).r - shade > 0) ? (frame.at(i).r -= shade) : (frame.at(i).r = 0);
            (frame.at(i).g - shade > 0) ? (frame.at(i).g -= shade) : (frame.at(i).g = 0);
            (frame.at(i).b - shade > 0) ? (frame.at(i).b -= shade) : (frame.at(i).b = 0);
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "fader_z.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds depth(FrameSet* p_frame_set)
{
    u_int               i;
    double              frame_count,
                        scale,
                        total = p_frame_set->number_of_frames();
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        for(i = 0; i < frame.size(); i++)
        {
            scale = 0.8 + (frame.at(i).z + 32768.0) / 131077.0;
            frame.at(i).x = (short)(frame.at(i).x * scale);
            frame.at(i).y = (short)(frame.at(i).y * scale);
            frame.at(i).z = (short)(frame.at(i).z * scale);
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "depth.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds drain(FrameSet* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    Double3d  s(0.85, 0.85, 0.85),
                        a(0.0,  0.0,  pi/15.0);
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 29; shrink_frame_count++)
        {
            out += frame;
            frame.scale_around_origin(s);
            frame.rotate_around_origin(a);
        }
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "drain.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds diminish(FrameSet* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    Double3d  s(0.75, 0.75, 0.75);
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 19; shrink_frame_count++)
        {
            out += frame;
            frame.scale(s);
        }
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "diminish.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds rainbow(FrameSet* p_frame_set)
{
    u_int               frame_count;
    Frame      frame(p_frame_set->p_space);
    FrameSet  out  (p_frame_set->p_space);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        frame.rotate_colors(frame_count);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rainbow.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds half_cylinder(FrameSet* p_frame_set)
{
    int                 i,
                        j;
    double              angle;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle = (((in[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            frame += Vertex(cos(angle) * 32767.0,
                                     in[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "half_cylinder.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds cylinder(FrameSet* p_frame_set)
{
    int                 i,
                        j;
    double              angle;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle = (((in[i].at(j).x + 32767.0) / 32767.0) * pi) + pi; // pi to 3pi
            frame += Vertex(cos(angle) * 32767.0,
                                     in[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "cylinder.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds hemisphere(FrameSet* p_frame_set)
{
    int                 i,
                        j;
    double              angle_theta,
                        angle_row;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle_theta = (((in[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            angle_row   = (((in[i].at(j).y) / 65535.0) * pi); // -pi to pi
            frame += Vertex(cos(angle_row) * cos(angle_theta) * 32767.0,
                                     sin(angle_row) * 32767.0,
                                     cos(angle_row) * sin(angle_theta) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "hemisphere.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds sphere(FrameSet* p_frame_set)
{
    int                 i,
                        j;
    double              angle_theta,
                        angle_row;
    Double3d  d;
    Frame      frame(p_frame_set->p_space);
    FrameSet  in(*p_frame_set),
                        out(p_frame_set->p_space);
    //------------------------------------------------------------------------
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle_theta = (((in[i].at(j).x + 32767.0) / 32767.0) * pi) + pi; // pi to 4pi
            angle_row   = (((in[i].at(j).y) / 65535.0) * pi); // -pi to pi
            frame += Vertex(cos(angle_row) * cos(angle_theta) * 32767.0,
                                     sin(angle_row) * 32767.0,
                                     cos(angle_row) * sin(angle_theta) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "sphere.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds film_strip(FrameSet* p_frame_set)
{
    int                        i,
                               j,
                               k;
    Short3d          d;
    Frame             frame(p_frame_set->p_space);
    FrameSet         in(*p_frame_set),
                               temp(p_frame_set->p_space),
                               out(p_frame_set->p_space);
    vector<FrameSet> pre_mix;
    //------------------------------------------------------------------------
    for(k = 0; k < (int)in.number_of_frames(); k++)
    {
        frame = in[k];
        d.x = -512;
        for(i = 0; i < 128; i++)
        {
            for(j = 0; j < (int)frame.size(); j++)
                if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                    frame.set_vertex_to_black(j);
            frame.move(d, false);
            temp += frame;
        }
        temp.reverse();
        //--------------------------------------------------------------------
        frame = in[k];
        d.x = 512;
        for(i = 0; i < 128; i++)
        {
            for(j = 0; j < (int)frame.size(); j++)
                if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                    frame.set_vertex_to_black(j);
            frame.move(d, false);
            temp += frame;
        }
        //--------------------------------------------------------------------
        pre_mix.push_back(temp);
        temp.clear();
    }
    //------------------------------------------------------------------------
    for(k = 0; k < (int)in.number_of_frames(); k+=2)
        out += pre_mix[k]; // even numbered frames

    frame.clear();
    for(k = 0; k < 128; k++)
        out += frame; // 120 empty frames

    for(k = 1; k < (int)in.number_of_frames(); k+=2)
        for(j = 0; j < 256; j++)
            out[(k * 128) + j] += pre_mix[k][j]; // odd numbered frames
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "film_strip.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
Bounds chil(FrameSet* p_frame_set)
{
    bool                       is_lit = false;
    int                        i,
                               j;
    Double3d         Z_offset(0.0, 0.0, 100.0);
    Vertex            previous_vertex;
    Frame             frame(p_frame_set->p_space);
    FrameSet         in(*p_frame_set),
                               out(p_frame_set->p_space);
    //--------------------------------------------------------------------
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        if(in[i].size() > 1)
        {
            if(in[i].is_2D())
                in[i].move(Z_offset, false);

            in[i].best_match_palette(LASERBOY_ILDA_DEFAULT);

            frame.push_back(in[i][0]);
            frame.push_back(in[i][0]);

            previous_vertex = in[i][0];

            for(j = 1; j < (int)in[i].size(); j++)
            {
                if(    (!is_lit)
                    && in[i][j].is_lit()
                  )
                {
                    frame += in[i][j];
                    is_lit = true;
                }
                else if(    (is_lit)
                         && in[i][j].is_blank()
                       )
                {
                    frame += in[i][j];
                    is_lit = false;
                }
                else if(    (is_lit)
                         && in[i][j].is_lit()
                         && (    ((Color)in[i][j])
                              != ((Color)previous_vertex)
                            )
                       )
                {
                    frame += in[i][j];
                }
                frame += in[i][j];
                previous_vertex = in[i][j];
            }
            frame += in[i].back();
        }
        out += frame;
    }
    //--------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "chil.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################























//############################################################################
void Space::load_frame_set_effects()
{
    //------------------------------------------------------------------------
    frame_set_effects.push_back(angle_wipe);
    frame_set_effect_names.push_back(string("angle_wipe"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(blend);
    frame_set_effect_names.push_back(string("blend"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(chil);
    frame_set_effect_names.push_back(string("chil"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(cylinder);
    frame_set_effect_names.push_back(string("cylinder"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(depth);
    frame_set_effect_names.push_back(string("depth"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(diminish);
    frame_set_effect_names.push_back(string("diminish"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(drain);
    frame_set_effect_names.push_back(string("drain"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(echos);
    frame_set_effect_names.push_back(string("echos"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(fader);
    frame_set_effect_names.push_back(string("fader"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(fader_z);
    frame_set_effect_names.push_back(string("fader_z"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(film_strip);
    frame_set_effect_names.push_back(string("film_strip"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(gears);
    frame_set_effect_names.push_back(string("gears"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(glitter);
    frame_set_effect_names.push_back(string("glitter"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(gyro);
    frame_set_effect_names.push_back(string("gyro"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(half_cylinder);
    frame_set_effect_names.push_back(string("half_cylinder"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(hemisphere);
    frame_set_effect_names.push_back(string("hemisphere"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(lava);
    frame_set_effect_names.push_back(string("lava"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(lava2);
    frame_set_effect_names.push_back(string("lava2"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(move_l2r);
    frame_set_effect_names.push_back(string("move_l2r"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(move_t2b);
    frame_set_effect_names.push_back(string("move_t2b"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(radial_wipe);
    frame_set_effect_names.push_back(string("radial_wipe"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(rainbow);
    frame_set_effect_names.push_back(string("rainbow"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(rocker);
    frame_set_effect_names.push_back(string("rocker"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(sphere);
    frame_set_effect_names.push_back(string("sphere"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(spokes);
    frame_set_effect_names.push_back(string("spokes"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(vortex);
    frame_set_effect_names.push_back(string("vortex"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(wipe);
    frame_set_effect_names.push_back(string("wipe"));
    //------------------------------------------------------------------------
    return;
}

} // namespace LaserBoy

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
