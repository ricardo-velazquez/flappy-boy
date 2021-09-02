
#include <gb/gb.h>
#include "macros.h"

#define POS_X_WINDOW 0
#define POS_Y_WINDOW 0



void setup_win_before_game() {
    move_win(8, 120 - 8);
    setup_win_border();
    UINT8 offset = 3;
    set_win_tile_xy(offset + 2, 1, 25);
    set_win_tile_xy(offset + 3, 1, 26);
    set_win_tile_xy(offset + 4, 1, 27);
    set_win_tile_xy(offset + 5, 1, 28);
    set_win_tile_xy(offset + 6, 1, 28);
    //SPACE
    set_win_tile_xy(offset + 9, 1, 29);
    set_win_tile_xy(offset + 10, 1, 25);

}



void setup_win_border() {
    FOR_I(0, 4)
        FOR_J(1, 19)
            set_win_tile_xy(j, i, 0);
    

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

    set_win_tile_xy(19, 0, 33);
    set_win_tile_xy(19, 1, 31);
    set_win_tile_xy(19, 2, 31);
    set_win_tile_xy(19, 3, 35);
}



void setup_win_in_game(UINT8 *session_highscore)
{
    move_win(8, 120 - 8);
    setup_win_border();
   

    // High score label
    set_win_tile_xy(1, 2, 22);
    set_win_tile_xy(2, 2, 23);
    set_win_tile_xy(3, 2, 24);
    // High score value
    FOR_FROM_ZERO_TO(4)
        set_win_tile_xy(3 + (i + 1), 2, 12 + session_highscore[(4 - i - 1)]);

    //set_win_tile_xy(10, 1, 42);
   // set_win_tile_xy(10, 2, 41);
  //  set_win_tile_xy(10, 0, 38);
  //  set_win_tile_xy(9, 1, 40);
  //  set_win_tile_xy(11, 1, 39);


}