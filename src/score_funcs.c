
#include <gb/gb.h>

void update_display_score()
{
    FOR_FROM_ZERO_TO(4)
    set_win_tile_xy((i + 1), 1, 12 + score[(4 - i - 1)]);
}

void add_score()
{
    for (UINT8 i = 0; i != 4; i++)
    {
        if (score[i] < 9)
        {
            score[i] += 1;
            break;
        }
        if (score[i] == 9)
        {
            score[i + 1] += 1;
            score[i] = 0;
            break;
        }
    }
}

UINT16 get_raw_score_from_array(UINT8 score[])
{
    UINT16 acc = 0;
    acc += score[0];
    acc += score[1] * 10;
    acc += score[2] * 100;
    acc += score[3] * 1000;
    return acc;
}