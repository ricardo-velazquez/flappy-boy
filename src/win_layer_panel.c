
#include <gb/gb.h>
#include "macros.h"


void setup_win(UBYTE session_highscore[])
{
    move_win(8, 120 - 8);

    // High score label
    set_win_tile_xy(1, 2, 22);
    set_win_tile_xy(2, 2, 23);
    set_win_tile_xy(3, 2, 24);
    // High score value
    FOR_FROM_ZERO_TO(4)
        set_win_tile_xy(3 + (i + 1), 2, 12 + session_highscore[(4 - i - 1)]);


    // window of text
    // Left side
    set_win_tile_xy(0, 0, 32);
    set_win_tile_xy(0, 1, 30);
    set_win_tile_xy(0, 2, 30);
    set_win_tile_xy(0, 3, 34);
    // Right side
    set_win_tile_xy(19, 0, 33);
    set_win_tile_xy(19, 1, 31);
    set_win_tile_xy(19, 2, 31);
    set_win_tile_xy(19, 3, 35);

    FOR(1, 19)
    {
        set_win_tile_xy(i, 0, 37);
        set_win_tile_xy(19 - i, 3, 36);
    }
}


void update_display_score(UINT8 score[])
{
    FOR_FROM_ZERO_TO(4)
        set_win_tile_xy((i + 1), 1, 12 + score[(4 - i - 1)]);
    
}


void add_score(UINT8 *score[])
{
    FOR_FROM_ZERO_TO(4)
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

