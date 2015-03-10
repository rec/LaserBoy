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
// LaserBoy_frame_effects.cpp is part of LaserBoy.
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

//############################################################################
LaserBoy_Bounds tranz(LaserBoy_frame* p_frame)
{
    int                i;
    double             amplitude = 0,
                       phase = half_pi;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(i = 0; i < 60; i++)
    {
        frame.ripple(3, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude += (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(3, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(3, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude -= (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(0, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude += (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(0, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);

    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(0, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude -= (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(1, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude += (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(1, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(1, amplitude, 2, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude -= (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(2, amplitude, 4, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude += (.66 / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(2, amplitude, 4, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
    }
    for(i = 0; i < 60; i++)
    {
        frame.ripple(2, amplitude, 4, phase);
        out += frame;
        frame.rotate_colors(1);
        phase += (two_pi / 61.0);
        amplitude -= (.66 / 61.0);
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "tranz.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds train(LaserBoy_frame* p_frame)
{
    u_int               frame_count       ,
                        vertex_count      ;
    LaserBoy_3D_short   temp_position     ;
    LaserBoy_frame      frame(*p_frame)   ;
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < frame.size(); frame_count++)
    {
        temp_position = frame.front();
        for(vertex_count = 0; vertex_count < frame.size() - 1; vertex_count++)
            frame.at(vertex_count) = frame.at(vertex_count + 1).as_3D_short();
        frame.back() = temp_position;
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "train.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds gleam(LaserBoy_frame* p_frame)
{
    u_int               frame_count         ,
                        vertex_count        ,
                        counter             ;
    LaserBoy_frame      frame_1(*p_frame)   ,
                        frame_2(*p_frame)   ;
    LaserBoy_frame_set  out(frame_1.p_space);
    //------------------------------------------------------------------------
    frame_1.promote_to_true_color();
    frame_2.promote_to_true_color();
    for(frame_count = 0; frame_count < 10; frame_count++)
    {
        frame_2.clear();
        counter = frame_count;
        for(vertex_count = 0; vertex_count < frame_1.size(); vertex_count++)
        {
            frame_2.push_back(frame_1.at(vertex_count));
            if(    frame_1.at(vertex_count).is_color(0)
                && !(counter++ % 10)
              )
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
    out.save_as_ild(LASERBOY_ILD_SHARE + "gleam.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds stars(LaserBoy_frame* p_frame) // individual segments must have no size (dots!)
{
    u_int              frame_count,
                       segment_index;
    LaserBoy_3D_short  d,
                       center;
    LaserBoy_3D_double s(1.1, 1.1, 1.1);
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            if(frame.scale_segment_around_origin(segment_index, s)) // out of bounds
            {
                center = frame.rectangular_center_of_segment(segment_index);
                while(!(d.x = rand() % 5000 - 2500)); //  -2500 to +2500, not zero
                while(!(d.y = rand() % 5000 - 2500));
                while(!(d.z = rand() % 5000 - 2500));
                d -= center;
                frame.move_segment(segment_index, d); // somewhere near the origin
            }
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "stars.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds shrink(LaserBoy_frame* p_frame)
{
    u_int              frame_count;
    LaserBoy_3D_double s(0.96, 0.96, 0.96);
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < 149; frame_count++)
    {
        frame.scale_around_origin(s);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "shrink.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds fill(LaserBoy_frame* p_frame)
{
    u_int              i,
                       segment_index;
    LaserBoy_3D_double s;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    for(segment_index = 0; segment_index < (*p_frame).number_of_segments(); segment_index++)
    {
        frame = (*p_frame).copy_segment(segment_index);
        out += frame;
        s = 0.95;
        for(i = 0; i < 20; i++)
        {
            frame = (*p_frame).copy_segment(segment_index);
            frame.scale_segment(0, s);
            out[out.number_of_frames() - 1] += frame;
            s -= 0.05;
        }
    }
    out.convert_blank_to_black();
    out.save_as_ild(LASERBOY_ILD_SHARE + "fill.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds squeez(LaserBoy_frame* p_frame)
{
    u_int              frame_count;
    LaserBoy_3D_double s;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set temp(frame.p_space),
                       out(frame.p_space);
    //------------------------------------------------------------------------
    s.x = 0.93;
    s.y = 1.00;
    s.z = 1.00;
    temp += frame;
    for(frame_count = 0; frame_count < 49; frame_count++)
    {
        frame.scale_around_origin(s);
        temp += frame;
    }
    out += temp;
    temp.reverse();
    temp.delete_frame(0);
    out += temp;
    temp.clear();
    //------------------------------------------------------------------------
    s.x = 1.00;
    s.y = 0.93;
    s.z = 1.00;
    frame = *(p_frame);
    temp += frame;
    for(frame_count = 0; frame_count < 49; frame_count++)
    {
        frame.scale_around_origin(s);
        temp += frame;
    }
    out += temp;
    temp.reverse();
    temp.delete_frame(0);
    out += temp;
    temp.clear();
    //------------------------------------------------------------------------
    s.x = 1.00;
    s.y = 1.00;
    s.z = 0.93;
    frame = *(p_frame);
    temp += frame;
    for(frame_count = 0; frame_count < 49; frame_count++)
    {
        frame.scale_around_origin(s);
        temp += frame;
    }
    out += temp;
    temp.reverse();
    temp.delete_frame(0);
    out += temp;
    temp.clear();
    //------------------------------------------------------------------------
    s.x = 0.93;
    s.y = 0.93;
    s.z = 0.93;
    frame = *(p_frame);
    temp += frame;
    for(frame_count = 0; frame_count < 49; frame_count++)
    {
        frame.scale_around_origin(s);
        temp += frame;
    }
    out += temp;
    temp.reverse();
    temp.delete_frame(0);
    out += temp;
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "squeez.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds around(LaserBoy_frame* p_frame)
{
    u_int              frame_count,
                       segment_index,
                       total_segments;
    LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
    LaserBoy_3D_double a(0, 0, pi/20);
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    total_segments = frame.number_of_segments();

    for(segment_index = 0; segment_index < total_segments; segment_index++)
    {
        for(frame_count = 0; frame_count < 10; frame_count++)
        {
            out_of_bounds |= frame.rotate_segment(segment_index, a);
            if(out_of_bounds)
                return out_of_bounds;
            out += frame;
        }
    }
    for(segment_index = 0; segment_index < total_segments; segment_index++)
    {
        for(frame_count = 0; frame_count < 10; frame_count++)
        {
            out_of_bounds |= frame.rotate_segment(segment_index, a);
            if(out_of_bounds)
                return out_of_bounds;
            out += frame;
        }
    }
    for(frame_count = 0; frame_count < 20; frame_count++)
    {
        for(segment_index = 0; segment_index < total_segments; segment_index++)
            out_of_bounds |= frame.rotate_segment(segment_index, a);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    for(frame_count = 0; frame_count < 20; frame_count++)
    {
        for(segment_index = 0; segment_index < total_segments; segment_index++)
            out_of_bounds |= frame.rotate_segment(segment_index, a);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    for(frame_count = 0; frame_count < 20; frame_count++)
    {
        for(segment_index = 0; segment_index < total_segments; segment_index++)
            out_of_bounds |= frame.rotate_segment(segment_index, a);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "around.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rotate(LaserBoy_frame* p_frame)
{
    u_int              frame_count;
    LaserBoy_3D_double a;
    LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    a.x = -pi/100;
    a.y = 0;
    a.z = 0;
    for(frame_count = 0; frame_count < 200; frame_count++)
    {
        frame.rotate_colors(1);
        out += frame;
        out_of_bounds |= frame.rotate_around_origin(a);
        if(out_of_bounds)
            return out_of_bounds;
    }
    a.x = 0;
    a.y = pi/100;
    a.z = 0;
    for(frame_count = 0; frame_count < 200; frame_count++)
    {
        frame.rotate_colors(1);
        out += frame;
        out_of_bounds |= frame.rotate_around_origin(a);
        if(out_of_bounds)
            return out_of_bounds;
    }
    a.x = 0;
    a.y = 0;
    a.z = pi/100;
    for(frame_count = 0; frame_count < 200; frame_count++)
    {
        frame.rotate_colors(1);
        out += frame;
        out_of_bounds |= frame.rotate_around_origin(a);
        if(out_of_bounds)
            return out_of_bounds;
    }
    a.x = pi/100;
    a.y = pi/100;
    a.z = pi/100;
    for(frame_count = 0; frame_count < 233; frame_count++)
    {
        frame.rotate_colors(1);
        out += frame;
        out_of_bounds |= frame.rotate_around_origin(a);
        if(out_of_bounds)
            return out_of_bounds;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rotate.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rock(LaserBoy_frame* p_frame)
{
    int                i;
    LaserBoy_3D_double a(0, 0, one_degree);
    LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    for(i = 0; i < 45; i++)
    {
        frame = *p_frame;
        out_of_bounds |= frame.rotate_around_origin(a * i);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    for(i = 45; i > -45; i--)
    {
        frame = *p_frame;
        out_of_bounds |= frame.rotate_around_origin(a * i);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    for(i = -45; i < 0; i++)
    {
        frame = *p_frame;
        out_of_bounds |= frame.rotate_around_origin(a * i);
        if(out_of_bounds)
            return out_of_bounds;
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rock.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds bounce(LaserBoy_frame* p_frame)
{
    u_int              frame_count;
    LaserBoy_Bounds    out_of_bounds;
    LaserBoy_3D_double r;
    LaserBoy_3D_short  d;
    LaserBoy_frame     frame(*p_frame);
    LaserBoy_frame_set out(frame.p_space);
    //------------------------------------------------------------------------
    d.x = short(random_neg_to_pos_1() * 3000);
    d.y = short(random_neg_to_pos_1() * 3000);
    d.z = short(random_neg_to_pos_1() * 3000);
    r.x = random_neg_to_pos_1() * pi/8.0;
    r.y = random_neg_to_pos_1() * pi/8.0;
    r.z = random_neg_to_pos_1() * pi/8.0;
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < 499; frame_count++)
    {
        frame.rotate(r);
        while((out_of_bounds = frame.move(d)))
        {
            if(    out_of_bounds & LASERBOY_OUT_POS_X
                || out_of_bounds & LASERBOY_OUT_NEG_X
              )
            {
                d.x = -d.x;
                r.x = -r.x;
            }
                //--------------------------------------------
            if(    out_of_bounds & LASERBOY_OUT_POS_Y
                || out_of_bounds & LASERBOY_OUT_NEG_Y
              )
            {
                d.y = -d.y;
                r.y = -r.y;
            }
                //--------------------------------------------
            if(    out_of_bounds & LASERBOY_OUT_POS_Z
                || out_of_bounds & LASERBOY_OUT_NEG_Z
              )
            {
                d.z = -d.z;
                r.z = -r.z;
            }
        }
        out += frame;
        frame.rotate_colors(1);
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "bounce.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds spin(LaserBoy_frame* p_frame)
{
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    u_int               frame_count,
                        segment_index,
                        total_segments = frame.number_of_segments();
    LaserBoy_Bounds     out_of_bounds;
    LaserBoy_3D_double* r = new LaserBoy_3D_double[total_segments];
    LaserBoy_3D_short*  d = new LaserBoy_3D_short[total_segments];
    //------------------------------------------------------------------------
    for(segment_index = 0; segment_index < total_segments; segment_index++)
    {
        d[segment_index].x = short(random_neg_to_pos_1() * 1000);
        d[segment_index].y = short(random_neg_to_pos_1() * 1000);
        d[segment_index].z = short(random_neg_to_pos_1() * 1000);
        r[segment_index].x = random_neg_to_pos_1() * pi/20.0;
        r[segment_index].y = random_neg_to_pos_1() * pi/20.0;
        r[segment_index].z = random_neg_to_pos_1() * pi/20.0;
    }
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < total_segments; segment_index++)
        {
            frame.rotate_segment(segment_index, r[segment_index]);
            while((out_of_bounds = frame.move_segment(segment_index, d[segment_index])))
            {
                if(    out_of_bounds & LASERBOY_OUT_POS_X
                    || out_of_bounds & LASERBOY_OUT_NEG_X
                  )
                {
                    d[segment_index].x = -d[segment_index].x;
                    r[segment_index].x = -r[segment_index].x;
                }
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Y
                    || out_of_bounds & LASERBOY_OUT_NEG_Y
                  )
                {
                    d[segment_index].y = -d[segment_index].y;
                    r[segment_index].y = -r[segment_index].y;
                }
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Z
                    || out_of_bounds & LASERBOY_OUT_NEG_Z
                  )
                {
                    d[segment_index].z = -d[segment_index].z;
                    r[segment_index].z = -r[segment_index].z;
                }
            }
        }
        out += frame;
        frame.rotate_colors(1);
    }
    delete d;
    delete r;
    out.save_as_ild(LASERBOY_ILD_SHARE + "spin.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds _break(LaserBoy_frame* p_frame)
{
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    u_int               frame_count,
                        segment_index;
    LaserBoy_Bounds     out_of_bounds;
    LaserBoy_3D_short*  d = new LaserBoy_3D_short[frame.number_of_segments()];
    //------------------------------------------------------------------------
    for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
    {
        d[segment_index].x = short((random_01() - 0.5) * 3000);
        d[segment_index].y = short((random_01() - 0.5) * 3000);
        d[segment_index].z = short((random_01() - 0.5) * 3000);
    }
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
            while((out_of_bounds = frame.move_segment(segment_index, d[segment_index])))
            {
                if(    out_of_bounds & LASERBOY_OUT_POS_X
                    || out_of_bounds & LASERBOY_OUT_NEG_X
                  )
                    d[segment_index].x = -(d[segment_index].x);
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Y
                    || out_of_bounds & LASERBOY_OUT_NEG_Y
                  )
                    d[segment_index].y = -(d[segment_index].y);
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Z
                    || out_of_bounds & LASERBOY_OUT_NEG_Z
                  )
                    d[segment_index].z = -(d[segment_index].z);
            }
        out += frame;
        frame.rotate_colors(1);
    }
    delete d;
    out.save_as_ild(LASERBOY_ILD_SHARE + "break.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rainbow(LaserBoy_frame* p_frame)
{
    int                 i,
                        span =   p_frame->p_space->current_palette().last
                               - p_frame->p_space->current_palette().first;
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    out += *p_frame;
    for(i = 1; i <= span; i++)
    {
        out[i - 1].rotate_colors(1);
        out += out[i - 1];
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rainbow.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds fade(LaserBoy_frame* p_frame)
{
    int                 i;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 1; i < 256; i++)
    {
        frame = *p_frame;
        frame.shade(i);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "fade.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds bleach(LaserBoy_frame* p_frame)
{
    int                 i;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 1; i < 256; i++)
    {
        frame = *p_frame;
        frame.tint(i);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "bleach.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds move_l2r(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_3D_short   d;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = -662;
    out += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);

        frame.move(d, false);
        out += frame;
    }
    out.reverse();
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = 662;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        frame.move(d, false);
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "move_l2r.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds move_t2b(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_3D_short   d;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.y = 662;
    out += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).y >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        frame.move(d, false);
        out += frame;
    }
    out.reverse();
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.y = -662;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).y <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);

        frame.move(d, false);
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "move_t2b.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds wipe(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= (short)((i * 661.98) - 32767.0))
                frame.set_vertex_to_black(j);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "wipe.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds radial_wipe(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 99; i >= 0; i--)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).magnitude() >= (i * 468.09))
                frame.set_vertex_to_black(j);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "radial_wipe.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds angle_wipe(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    frame.flip(0);
    out += frame;
    for(i = 99; i >= 0; i--)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(((atan2(frame.at(j).y, frame.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                frame.set_vertex_to_black(j);
        out += frame;
    }
    out.flip(0);
    out.save_as_ild(LASERBOY_ILD_SHARE + "angle_wipe.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds draw(LaserBoy_frame* p_frame)
{
    u_int               i;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame += p_frame->front();
    frame += p_frame->front();
    out += frame;
    //------------------------------------------------------------------------
    for(i = 1; i < p_frame->size(); i++)
    {
        frame += p_frame->at(i);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "draw.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds wobble(LaserBoy_frame* p_frame)
{
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    u_int               frame_count,
                        segment_index;
    LaserBoy_Bounds     out_of_bounds;
    LaserBoy_3D_double* a = new LaserBoy_3D_double[frame.number_of_segments()];
    //------------------------------------------------------------------------
    for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
    {
        a[segment_index].x = (random_01() - 0.5) * pi/20;
        a[segment_index].y = (random_01() - 0.5) * pi/20;
        a[segment_index].z = (random_01() - 0.5) * pi/20;
    }

    out += frame;
    for(frame_count = 1; frame_count < 300; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
            while((out_of_bounds = frame.rotate_segment(segment_index, a[segment_index])))
            {
                if(    out_of_bounds & LASERBOY_OUT_POS_X
                    || out_of_bounds & LASERBOY_OUT_NEG_X
                  )
                    a[segment_index].x = -a[segment_index].x;
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Y
                    || out_of_bounds & LASERBOY_OUT_NEG_Y
                  )
                    a[segment_index].y = -a[segment_index].y;
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Z
                    || out_of_bounds & LASERBOY_OUT_NEG_Z
                  )
                    a[segment_index].z = -a[segment_index].z;
            }
        out += frame;
        frame.rotate_colors(1);
    }
    delete a;
    out.save_as_ild(LASERBOY_ILD_SHARE + "wobble.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds wiggle(LaserBoy_frame* p_frame)
{
    u_int               frame_count,
                        segment_index;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            do
            {
                d.x = (random_01() - 0.5) * 1000;
                d.y = (random_01() - 0.5) * 1000;
                d.z = (random_01() - 0.5) * 1000;
            } while(frame.move_segment(segment_index, d));
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "wiggle.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds throb(LaserBoy_frame* p_frame)
{
    bool                flip_flop = false;
    u_int               frame_count,
                        segment_index;
    double              s;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            do
            {
                if(flip_flop)
                    s = (random_01() / 10 + 0.9); // 0.9 to 1.0
                else
                    s = (random_01() / 10 + 1.0); // 1.0 to 1.1
                flip_flop = !flip_flop;
            } while(frame.scale_segment(segment_index, s));
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "throb.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds pop(LaserBoy_frame* p_frame)
{
    bool                flip_flop = false;
    u_int               frame_count,
                        segment_index;
    double              s;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            do
            {
                if(flip_flop)
                    s = 0.8;
                else
                    s = 1.2;
                flip_flop = !flip_flop;
            } while(frame.scale_segment(segment_index, s));
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "pop.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds jazz(LaserBoy_frame* p_frame)
{
    bool                flip_flop = false;
    u_int               frame_count,
                        segment_index;
    double              s;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            do
            {
                if(flip_flop)
                    s = (random_01() / 10 + 0.9); // 0.9 to 1.0
                else
                    s = (random_01() / 10 + 1.0); // 1.0 to 1.1
                flip_flop = !flip_flop;
            } while(frame.scale_segment_around_origin(segment_index, s));
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "jazz.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds bump(LaserBoy_frame* p_frame)
{
    bool                flip_flop = false;
    u_int               frame_count,
                        segment_index;
    double              s;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            do
            {
                if(flip_flop)
                    s = 0.8;
                else
                    s = 1.2;
                flip_flop = !flip_flop;
            } while(frame.scale_segment_around_origin(segment_index, s));
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "bump.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds scan(LaserBoy_frame* p_frame)
{
    u_int               frame_count,
                        segment_index;
    LaserBoy_Bounds     out_of_bounds;
    LaserBoy_3D_short   d(1200,  666,     0);
    LaserBoy_3D_double  a(   0,    0, pi/30);
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 0; frame_count < 199; frame_count++)
    {
        for(segment_index = 0; segment_index < frame.number_of_segments(); segment_index++)
        {
            frame.rotate_segment(segment_index, a);
            while((out_of_bounds = frame.move_segment(segment_index, d)))
            {
                if(    out_of_bounds & LASERBOY_OUT_POS_X
                    || out_of_bounds & LASERBOY_OUT_NEG_X
                  )
                    d.x = -d.x;
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Y
                    || out_of_bounds & LASERBOY_OUT_NEG_Y
                  )
                    d.y = -d.y;
                    //--------------------------------------------
            }
        }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "scan.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds fall(LaserBoy_frame* p_frame)
{
    LaserBoy_frame       frame(*p_frame);
    LaserBoy_Bounds      out_of_bounds;
    LaserBoy_3D_double   d(0.0, 0.0, 0.0);
    int                  frame_count,
                         segment_index;
    LaserBoy_frame_set   segments = frame.explode_segments(),
                         out(frame.p_space);
    out += frame;
    for(frame_count = 1; frame_count < 399; frame_count++)
    {
        for(segment_index = 0; segment_index < (int)segments.number_of_frames(); segment_index++)
        {
            d.x = (random_01() - 0.5) * 1000; // -500 to +500
            d.y = (random_01() - 1  ) * 1000; // always negative. DOWN!
            d.z = (random_01() - 0.5) * 1000; // -500 to +500
            while((out_of_bounds = segments[segment_index].move(d)))
            {
                if(    out_of_bounds & LASERBOY_OUT_POS_X
                    || out_of_bounds & LASERBOY_OUT_NEG_X
                  )
                        d.x = -d.x;
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_NEG_Y)
                        d.y = LASERBOY_MAX_SHORT - segments[segment_index].segment_top().y;
                    //--------------------------------------------
                if(    out_of_bounds & LASERBOY_OUT_POS_Z
                    || out_of_bounds & LASERBOY_OUT_NEG_Z
                  )
                        d.z = -d.z;
            }
        }
        frame = segments.sum_of_frames();
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "fall.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rain(LaserBoy_frame* p_frame)
{
    LaserBoy_frame             frame(*p_frame);
    LaserBoy_Bounds            out_of_bounds;
    vector<LaserBoy_3D_double> d;
    int                        frame_count,
                               segment_index;
    LaserBoy_frame_set         segments = frame.explode_segments(),
                               out(frame.p_space);
    for(segment_index = 0; segment_index < (int)segments.number_of_frames(); segment_index++)
    {
        d.push_back(LaserBoy_3D_double());
        d.back().y = (random_01() - 1.1) * 400;
    }
    out += frame;
    for(frame_count = 1; frame_count < 799; frame_count++)
    {
        for(segment_index = 0; segment_index < (int)segments.number_of_frames(); segment_index++)
        {
            while((out_of_bounds = segments[segment_index].move(d[segment_index] * (pow(1.15, ((LASERBOY_MAX_SHORT - segments[segment_index].segment_top().y) / 1000))))))
            {
                if(out_of_bounds & LASERBOY_OUT_NEG_Y)
                {
                    d[segment_index].y = LASERBOY_MAX_SHORT - segments[segment_index].segment_top().y;
                    segments[segment_index].move(d[segment_index]);
                    d[segment_index].y = (random_01() - 1.1) * 400; // always negative. DOWN!
                }
            }
        }
        frame = segments.sum_of_frames();
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "rain.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds melt(LaserBoy_frame* p_frame)
{
    int                 frame_count,
                        i;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        frame.at(0).x = (short)((   frame.back().x  / 10.0
                                  + frame.at(0).x
                                  + frame.at(1).x  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        frame.at(0).y = (short)((   frame.back().y  / 10.0
                                  + frame.at(0).y
                                  + frame.at(1).y  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        frame.at(0).z = (short)((   frame.back().z  / 10.0
                                  + frame.at(0).z
                                  + frame.at(1).z  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        for(i = 1; i < (int)frame.size() - 1; i++)
        {
            frame.at(i).x = (short)((   frame.at(i - 1).x  / 10.0
                                      + frame.at(i).x
                                      + frame.at(i + 1).x  / 10.0
                                    )
                                    / 1.2
                                    + 0.5
                                   );
            frame.at(i).y = (short)((   frame.at(i - 1).y  / 10.0
                                      + frame.at(i).y
                                      + frame.at(i + 1).y  / 10.0
                                    )
                                    / 1.2
                                    + 0.5
                                   );
            frame.at(i).z = (short)((   frame.at(i - 1).z  / 10.0
                                      + frame.at(i).z
                                      + frame.at(i + 1).z  / 10.0
                                    )
                                    / 1.2
                                    + 0.5
                                   );
        }
        frame.back().x = (short)((  frame.at(frame.size() - 2).x  / 10.0
                                  + frame.back().x
                                  + frame.at(0).x  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        frame.back().y = (short)((  frame.at(frame.size() - 2).y  / 10.0
                                  + frame.back().y
                                  + frame.at(0).y  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        frame.back().z = (short)((  frame.at(frame.size() - 2).z  / 10.0
                                  + frame.back().z
                                  + frame.at(0).z  / 10.0
                                )
                                / 1.2
                                + 0.5
                               );
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "melt.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds squirm(LaserBoy_frame* p_frame)
{
    int                 frame_count,
                        i;
    LaserBoy_frame      frame(*p_frame);
    LaserBoy_frame_set  out(frame.p_space);
    //------------------------------------------------------------------------
    out += frame;
    for(frame_count = 1; frame_count < 199; frame_count++)
    {
        frame.at(0).x = (short)((   frame.back().x  / 10.0
                                  + frame.at(0).x
                                  - frame.at(1).x  / 10.0
                                )
                                + 0.5
                               );
        frame.at(0).y = (short)((   frame.back().y  / 10.0
                                  + frame.at(0).y
                                  - frame.at(1).y  / 10.0
                                )
                                + 0.5
                               );
        frame.at(0).z = (short)((   frame.back().z  / 10.0
                                  + frame.at(0).z
                                  - frame.at(1).z  / 10.0
                                )
                                + 0.5
                               );
        for(i = 1; i < (int)frame.size() - 1; i++)
        {
            frame.at(i).x = (short)((   frame.at(i - 1).x  / 10.0
                                      + frame.at(i).x
                                      - frame.at(i + 1).x  / 10.0
                                    )
                                    + 0.5
                                   );
            frame.at(i).y = (short)((   frame.at(i - 1).y  / 10.0
                                      + frame.at(i).y
                                      - frame.at(i + 1).y  / 10.0
                                    )
                                    + 0.5
                                   );
            frame.at(i).z = (short)((   frame.at(i - 1).z  / 10.0
                                      + frame.at(i).z
                                      - frame.at(i + 1).z  / 10.0
                                    )
                                    + 0.5
                                   );
        }
        frame.back().x = (short)((  frame.at(frame.size() - 2).x  / 10.0
                                  + frame.back().x
                                  - frame.at(0).x  / 10.0
                                )
                                + 0.5
                               );
        frame.back().y = (short)((  frame.at(frame.size() - 2).y  / 10.0
                                  + frame.back().y
                                  - frame.at(0).y  / 10.0
                                )
                                + 0.5
                               );
        frame.back().z = (short)((  frame.at(frame.size() - 2).z  / 10.0
                                  + frame.back().z
                                  - frame.at(0).z  / 10.0
                                )
                                + 0.5
                               );
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "squirm.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds twist(LaserBoy_frame* p_frame)
{
    int                   i,
                          j;
    LaserBoy_Bounds       out_of_bounds;
    LaserBoy_3D_double    rotated_coordinate;
    LaserBoy_frame        frame(p_frame->p_space);
    LaserBoy_frame_set    out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= (short)((i * 163.84) - 32767.0))
            {
                rotated_coordinate = rotate_vertex_around_x(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
        out += frame;
    }
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= (short)((i * 163.84) - 32767.0))
            {
                rotated_coordinate = rotate_vertex_around_x(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
            else
                frame.at(j) = p_frame->at(j); // un-effected frame data!
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "twist.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds warp(LaserBoy_frame* p_frame)
{
    int                   i,
                          j;
    LaserBoy_Bounds       out_of_bounds;
    LaserBoy_3D_double    rotated_coordinate;
    LaserBoy_frame        frame(p_frame->p_space);
    LaserBoy_frame_set    out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).magnitude() >= (i * 81.92))
            {
                rotated_coordinate = rotate_vertex_around_x(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
        out += frame;
    }
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).magnitude() <= (i * 81.92))
            {
                rotated_coordinate = rotate_vertex_around_x(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "warp.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds swirl(LaserBoy_frame* p_frame)
{
    int                   i,
                          j;
    LaserBoy_Bounds       out_of_bounds;
    LaserBoy_3D_double    rotated_coordinate;
    LaserBoy_frame        frame(p_frame->p_space);
    LaserBoy_frame_set    out(p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    out += frame;
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).magnitude() <= (i * 81.92))
            {
                rotated_coordinate = rotate_vertex_around_z(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
        out += frame;
    }
    for(i = 0; i < 400; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).magnitude() >= (i * 81.92))
            {
                rotated_coordinate = rotate_vertex_around_z(frame.at(j), pi/200.0);
                out_of_bounds = LaserBoy_bounds_check(rotated_coordinate, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
                else
                    frame.at(j) = rotated_coordinate;
            }
            else
                frame.at(j) = p_frame->at(j);
        out += frame;
    }
    out.save_as_ild(LASERBOY_ILD_SHARE + "swirl.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds half_cylinder(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_3D_short   d;
    double              angle;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  temp (p_frame->p_space),
                        out  (p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = -662;
    temp += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);

        frame.move(d, false);
        temp += frame;
    }
    temp.reverse();
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = 662;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        frame.move(d, false);
        temp += frame;
    }
    //------------------------------------------------------------------------
    temp.flatten_z();
    for(i = 0; i < (int)temp.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)temp[i].size(); j++)
        {
            angle = (((temp[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            frame += LaserBoy_vertex(cos(angle) * 32767.0,
                                     temp[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     temp[i].at(j).r,
                                     temp[i].at(j).g,
                                     temp[i].at(j).b,
                                     temp[i].at(j).k,
                                     temp[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "half_cylinder.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds cylinder(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_3D_short   d;
    double              angle;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  temp (p_frame->p_space),
                        out  (p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = -662;
    temp += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);

        frame.move(d, false);
        temp += frame;
    }
    temp.reverse();
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = 662;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        frame.move(d, false);
        temp += frame;
    }
    //------------------------------------------------------------------------
    temp.flatten_z();
    for(i = 0; i < (int)temp.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)temp[i].size(); j++)
        {
            angle = (((temp[i].at(j).x + 32767.0) / 32767.0) * pi) + pi; // pi to 3pi
            frame += LaserBoy_vertex(cos(angle) * 32767.0,
                                     temp[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     temp[i].at(j).r,
                                     temp[i].at(j).g,
                                     temp[i].at(j).b,
                                     temp[i].at(j).k,
                                     temp[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "cylinder.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds hemisphere(LaserBoy_frame* p_frame)
{
    int                 i,
                        j;
    LaserBoy_3D_short   d;
    double              angle_theta,
                        angle_row;
    LaserBoy_frame      frame(p_frame->p_space);
    LaserBoy_frame_set  temp (p_frame->p_space),
                        out  (p_frame->p_space);
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = -662;
    temp += frame;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);

        frame.move(d, false);
        temp += frame;
    }
    temp.reverse();
    //------------------------------------------------------------------------
    frame = *p_frame;
    d.x = 662;
    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        frame.move(d, false);
        temp += frame;
    }
    //------------------------------------------------------------------------
    temp.flatten_z();
    for(i = 0; i < (int)temp.number_of_frames(); i++)
    {
        frame.clear();
        for(j = 0; j < (int)temp[i].size(); j++)
        {
            angle_theta = (((temp[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            angle_row   = (((temp[i].at(j).y) / 65535.0) * pi); // -pi to pi
            frame += LaserBoy_vertex(cos(angle_row) * cos(angle_theta) * 32767.0,
                                     sin(angle_row) * 32767.0,
                                     cos(angle_row) * sin(angle_theta) * -32767.0,
                                     temp[i].at(j).r,
                                     temp[i].at(j).g,
                                     temp[i].at(j).b,
                                     temp[i].at(j).k,
                                     temp[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    out.save_as_ild(LASERBOY_ILD_SHARE + "hemisphere.ild");
    return LASERBOY_IN_BOUNDS;
}

//############################################################################




































//############################################################################
void LaserBoy_space::load_frame_effects()
{
    //------------------------------------------------------------------------
    frame_effects.push_back(around);
    frame_effect_names.push_back(string("around"));
    //------------------------------------------------------------------------
    frame_effects.push_back(angle_wipe);
    frame_effect_names.push_back(string("angle_wipe"));
    //------------------------------------------------------------------------
    frame_effects.push_back(bleach);
    frame_effect_names.push_back(string("bleach"));
    //------------------------------------------------------------------------
    frame_effects.push_back(bounce);
    frame_effect_names.push_back(string("bounce"));
    //------------------------------------------------------------------------
    frame_effects.push_back(_break);
    frame_effect_names.push_back(string("break"));
    //------------------------------------------------------------------------
    frame_effects.push_back(bump);
    frame_effect_names.push_back(string("bump"));
    //------------------------------------------------------------------------
    frame_effects.push_back(cylinder);
    frame_effect_names.push_back(string("cylinder"));
    //------------------------------------------------------------------------
    frame_effects.push_back(draw);
    frame_effect_names.push_back(string("draw"));
    //------------------------------------------------------------------------
    frame_effects.push_back(fade);
    frame_effect_names.push_back(string("fade"));
    //------------------------------------------------------------------------
    frame_effects.push_back(fill);
    frame_effect_names.push_back(string("fill"));
    //------------------------------------------------------------------------
    frame_effects.push_back(fall);
    frame_effect_names.push_back(string("fall"));
    //------------------------------------------------------------------------
    frame_effects.push_back(gleam);
    frame_effect_names.push_back(string("gleam"));
    //------------------------------------------------------------------------
    frame_effects.push_back(half_cylinder);
    frame_effect_names.push_back(string("half_cylinder"));
    //------------------------------------------------------------------------
    frame_effects.push_back(hemisphere);
    frame_effect_names.push_back(string("hemisphere"));
    //------------------------------------------------------------------------
    frame_effects.push_back(jazz);
    frame_effect_names.push_back(string("jazz"));
    //------------------------------------------------------------------------
    frame_effects.push_back(melt);
    frame_effect_names.push_back(string("melt"));
    //------------------------------------------------------------------------
    frame_effects.push_back(move_l2r);
    frame_effect_names.push_back(string("move_l2r"));
    //------------------------------------------------------------------------
    frame_effects.push_back(move_t2b);
    frame_effect_names.push_back(string("move_t2b"));
    //------------------------------------------------------------------------
    frame_effects.push_back(pop);
    frame_effect_names.push_back(string("pop"));
    //------------------------------------------------------------------------
    frame_effects.push_back(radial_wipe);
    frame_effect_names.push_back(string("radial_wipe"));
    //------------------------------------------------------------------------
    frame_effects.push_back(rain);
    frame_effect_names.push_back(string("rain"));
    //------------------------------------------------------------------------
    frame_effects.push_back(rainbow);
    frame_effect_names.push_back(string("rainbow"));
    //------------------------------------------------------------------------
    frame_effects.push_back(rotate);
    frame_effect_names.push_back(string("rotate"));
    //------------------------------------------------------------------------
    frame_effects.push_back(rock);
    frame_effect_names.push_back(string("rock"));
    //------------------------------------------------------------------------
    frame_effects.push_back(scan);
    frame_effect_names.push_back(string("scan"));
    //------------------------------------------------------------------------
    frame_effects.push_back(shrink);
    frame_effect_names.push_back(string("shrink"));
    //------------------------------------------------------------------------
    frame_effects.push_back(spin);
    frame_effect_names.push_back(string("spin"));
    //------------------------------------------------------------------------
    frame_effects.push_back(squeez);
    frame_effect_names.push_back(string("squeez"));
    //------------------------------------------------------------------------
    frame_effects.push_back(squirm);
    frame_effect_names.push_back(string("squirm"));
    //------------------------------------------------------------------------
    frame_effects.push_back(stars);
    frame_effect_names.push_back(string("stars"));
    //------------------------------------------------------------------------
    frame_effects.push_back(swirl);
    frame_effect_names.push_back(string("swirl"));
    //------------------------------------------------------------------------
    frame_effects.push_back(throb);
    frame_effect_names.push_back(string("throb"));
    //------------------------------------------------------------------------
    frame_effects.push_back(train);
    frame_effect_names.push_back(string("train"));
    //------------------------------------------------------------------------
    frame_effects.push_back(tranz);
    frame_effect_names.push_back(string("tranz"));
    //------------------------------------------------------------------------
    frame_effects.push_back(twist);
    frame_effect_names.push_back(string("twist"));
    //------------------------------------------------------------------------
    frame_effects.push_back(warp);
    frame_effect_names.push_back(string("warp"));
    //------------------------------------------------------------------------
    frame_effects.push_back(wiggle);
    frame_effect_names.push_back(string("wiggle"));
    //------------------------------------------------------------------------
    frame_effects.push_back(wipe);
    frame_effect_names.push_back(string("wipe"));
    //------------------------------------------------------------------------
    frame_effects.push_back(wobble);
    frame_effect_names.push_back(string("wobble"));
    //------------------------------------------------------------------------
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
