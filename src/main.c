// Standard functions and GBDK
#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>

#include "declarations.h"

// Resources
#include "res/bkgs.c"
#include "res/sprites.c"
#include "res/tilemap.c"

// Tooling
#include "macros.h"
#include "utils.c"
// Modules
#include "win_layer_funcs.c"
#include "score_funcs.c"
#include "entities/player.c"
#include "entities/pipes.c"

void setup();
void update();
void perform_delay(UINT8 times);

void main()
{

    do
    {
        ENABLE_RAM_MBC1;
        setup();
        while (is_playing)
        {
            update();
            perform_delay(2);
        }
        DISABLE_RAM_MBC1;
        is_playing = true;
    } while (1);
}

void perform_delay(UINT8 times)
{
    for (UINT8 i = 0; i < times; i++)
    {
        wait_vbl_done();
    }
}

void setup()
{

    fadeout();
   
    // Init sprite ids array
    FOR_FROM_ZERO_TO(40)
    {
        sprite_ids[i] = false;
        move_sprite(i, 168, 0);
    }

    FOR_FROM_ZERO_TO(4)
    score[i] = 0;

    if (session[0] != 's')
    {
        for (UINT8 w = 0; w != 4; w++)
        {
            session_highscore[w] = 0;
        }
        session[0] = 's';
    }

    state = WAITING_TO_START;

    set_bkg_data(0, 44, bkgs);
    set_bkg_tiles(0, 0, tilemapWidth, tilemapHeight, tilemap);
    set_sprite_data(0, 4, sprites);

    setup_player();
    setup_pipes();
    setup_win_before_game();

    SHOW_BKG;
    SHOW_SPRITES;
    SHOW_WIN;
    DISPLAY_ON;

     fadein();
}

void update()
{
    if (state == DEAD)
    {
        is_playing = false;
        return;
    }

    if (state != WAITING_TO_START)
    {
        update_display_score();
        if (step_counter == step_limit)
        {
            INT8 index = get_next_slot_col_pipe_available();
            if (index != -1)
            {
                create_pipe(index, 160 + 8);
            }
            step_counter = 0;
        }
        else
        {
            step_counter += 1;
        }
    }

    if (state != WAITING_TO_START && state != DEAD)
    {
        FOR_I(0, num_pipe_cols)
        {
            FOR_J(0, 6)
            {
                // Check for colissions

                if (player.x + 7 > columns[i].pipes[j].x && player.x < columns[i].pipes[j].x + 7 && player.y < columns[i].pipes[j].y + 8 && player.y > columns[i].pipes[j].y - 8)
                {
                    state = DEAD;
                    UINT16 raw_current_score = get_raw_score_from_array(score);
                    UINT16 raw_highscore = get_raw_score_from_array(session_highscore);
                    if (raw_current_score > raw_highscore)
                    {
                        FOR_FROM_ZERO_TO(4)
                        session_highscore[i] = score[i];
                    }
                    delay(300);
                }

                // Move active columns
                if (columns[i].is_created == true)
                {
                    columns[i].pipes[j].x += -1;
                    move_sprite(columns[i].pipes[j].sprite_id, columns[i].pipes[j].x, columns[i].pipes[j].y);
                }
            }

            if (columns[i].is_scored == false && columns[i].is_created == true && player.x - 4 > columns[i].pipes[0].x) // Not visible
            {
                columns[i].is_scored = true;
                add_score();
            }

            if (columns[i].pipes[0].x == 0) // Not visible
            {
                columns[i].is_created = false;

                for (UINT8 j = 0; j != 6; j++)
                {
                    sprite_ids[columns[i].pipes[j].sprite_id] = false;
                }
            }
        }
        scroll_bkg(1, 0);
    }

    if (state == FALLING && player.y > 88)
    {
        state = DEAD;

        UINT16 raw_current_score = get_raw_score_from_array(score);
        UINT16 raw_highscore = get_raw_score_from_array(session_highscore);
        if (raw_current_score > raw_highscore)
        {
            FOR_FROM_ZERO_TO(4)
            session_highscore[i] = score[i];
        }
    }

    if (state == JUMPING)
    {
        current_jump_acc += JUMP_SPEED;
        //scroll_sprite(0, 0, -3);
        move_game_object(0, -JUMP_SPEED);
        if (current_jump_acc >= jump_limit)
        {
            state = FALLING;
            current_jump_acc = 0;
        }
    }

    if (joypad() == J_UP && button_released == true && state == FALLING)
    {
        state = JUMPING;
        button_released = false;
    }

    if (joypad() != J_UP)
    {
        button_released = true;
    }

    if (state == WAITING_TO_START)
    {
        if (joypad() == J_UP)
        {
            state = FALLING;
            setup_win_in_game(session_highscore);
        }
        else
        {
            return;
        }
    }

    if (state == FALLING)
    {
        move_game_object(0, 1);
    }
}


INT8 get_next_available()
{
    INT8 next_available = -1;

    FOR(1, 40)
    {
        if (sprite_ids[i] == false)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
}

