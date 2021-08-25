#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include <bcd.h>

#include "res/sprites.h"
#include "res/bkgs.h"
#include "res/tilemap.h"

#include "entities/player.h"
#include "entities/pipes.h"

/*
There are a total of 32 tiles (8x8 pixels) in the with of the map.

The visible space start at (8, 16)

*/

const enum states { FALLING,
                    JUMPING,
                    JUMP_ENDED,
                    WAITING_TO_START,
                    DEAD };
enum states state;

UBYTE sprite_ids[40];
const UINT8 num_pipe_cols = 5;
// GameObjects declaration
struct Player player;
struct Pipe pipes[6 * 5];

struct Pipe last_pipe;

// Game logic vars
INT8 button_released = 1;
INT8 current_jump_acc = 0;
INT8 jump_limit = 25;
UINT8 acc = 0;
const UINT8 limit = 1;
UINT8 wait_acc = 0;
const UINT16 wait_limit = 2;
UINT8 last_sprite_id = 0;

void setup();
void update();
void perform_deplay(UINT8 times);

void main()
{
    setup();
    while (1)
    {
        update();
        perform_deplay(3);
    }
}

void perform_deplay(UINT8 times)
{
    for (UINT8 i = 0; i < times; i++)
    {
        wait_vbl_done();
    }
}

void setup()
{
    // Init sprite ids array
    for (INT8 i = 0; i != 40; i++)
    {
        sprite_ids[i] = 0;
    }

    state = WAITING_TO_START;

    // Setup Background
    set_bkg_data(0, 10, bkgs);
    set_bkg_tiles(0, 0, 32, 18, tilemap);

    // Load Sprites
    set_sprite_data(0, 4, sprites);

    setup_player(&player, sprite_ids);
    setup_pipes(pipes, sprite_ids, num_pipe_cols);
    last_pipe = pipes[num_pipe_cols * 6];

    
    UINT8 count =0;
    for (UINT8 q = 0; q != 40; q++)
    {
        if (sprite_ids[q] == 1 ) {
            count++;
        }
    }

    //printf("count: %d", count);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}
void update()
{

    for (UINT8 i = 0; i < 6 * num_pipe_cols; i++)
    {
        pipes[i].x += -1;
        /*
        if (pipes[i].x < 5)
        {
            for (UINT8 j = 0; j < 6 * num_pipe_cols; j++)
            {
                UINT8 nex_pos = last_pipe.x + 16 + 8;
                if (pipes[i].col_pipe_id == pipes[j].col_pipe_id)
                {
                    pipes[j].x = nex_pos;
                    move_sprite(pipes[j].sprite_id, pipes[j].x, pipes[j].y);
                }
                
            }
            last_pipe = pipes[i];
            
        }*/
        move_sprite(pipes[i].sprite_id, pipes[i].x, pipes[i].y);
    }

    scroll_bkg(1, 0);

    if (state == WAITING_TO_START)
    {
        if (joypad() == J_UP)
        {
            state = FALLING;
        }
        else
        {
            return;
        }
    }

    if (state == FALLING)
    {
        move_game_object(&player, 0, 1);
    }

    if (state == JUMPING)
    {
        current_jump_acc += 3;
        //scroll_sprite(0, 0, -3);
        move_game_object(&player, 0, -3);
        if (current_jump_acc >= jump_limit)
        {
            state = JUMP_ENDED;
            current_jump_acc = 0;
        }
    }

    if (state == JUMP_ENDED && wait_acc >= wait_limit)
    {
        state = FALLING;
    }

    if (joypad() == J_UP && button_released == 1)
    {
        state = JUMPING;
        button_released = 0;
    }

    if (joypad() != J_UP)
    {
        //printf("x: %d, y: %d", player.x, player.y);
        button_released = 1;
    }

    //    if (player.x + player.width > pipes[0].upper_x && player.x < pipes[0].upper_x + pipes[0].upper_width && player.y < pipes[0].upper_y && player.y > pipes[0].upper_y - pipes[0].upper_height) {
    //printf("Collision");
    //  }
}
