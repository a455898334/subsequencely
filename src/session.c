

#include "session.h"

void session_draw(Sequencer* sr)
{
    for (u8 y = 0; y < GRID_SIZE; y++)
    {
        u8 seq_i = row_to_seq(y);
        Sequence* s = &sr->sequences[seq_i];

        for (u8 x = 0; x < GRID_SIZE; x++)
        {
            u8 step = x * STEPS_PER_PAD;
            u8 skip = flag_is_set(s->notes[step].flags, NTE_SKIP);
            u8 index = coord_to_index(x, y);

            if (skip)
            {
                plot_pad(index, off_color);
            }
            else
            {
                u8 dimness = 5 * (s->playhead / STEPS_PER_PAD != x);
                plot_pad_dim(index, sequence_colors[seq_i], dimness);
            }
        }
    }
}


u8 session_handle_press(Sequencer* sr, u8 index, u8 value)
{
    u8 x, y;
    if (value == 0 || !index_to_pad(index, &x, &y))
    {
        return 0;
    }

    u8 seq_i = row_to_seq(y);
    Sequence* s = &sr->sequences[seq_i];
    u8 shift = modifier_held(LP_SHIFT);
    u8 step = x * STEPS_PER_PAD;

    if (modifier_held(LP_CLICK))
    {
        
    }
    else if (modifier_held(LP_UNDO))
    {
        sequence_reverse(s);
    }
    else if (modifier_held(LP_DELETE))
    {
        for (u8 i = 0; i < STEPS_PER_PAD; i++)
        {
            sequence_clear_note(s, step + i);
        }
    }
    else if (modifier_held(LP_QUANTISE))
    {
        u8 skip = !flag_is_set(s->notes[step].flags, NTE_SKIP);
        for (u8 i = 0; i < STEPS_PER_PAD; i++)
        {
            sequence_set_skip(s, step + i, skip);
        }
    }
    else if (modifier_held(LP_DUPLICATE))
    {
        
    }
    else if (modifier_held(LP_DOUBLE))
    {
        
    }
    else
    {
        sequence_queue_or_jump(s, x * STEPS_PER_PAD, shift);
    }

    return 1;
}