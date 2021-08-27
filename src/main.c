#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include <gb/font.h>
#include <bcd.h>

#include "res/sprites.h"
#include "res/bkgs.h"
#include "res/tilemap.h"

#include "entities/player.h"
//#include "entities/pipes.h"

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

bool sprite_ids[40];
const UINT8 num_pipe_cols = 4;
// GameObjects declaration
struct Player player;
const UINT16 step_limit = 50;
UINT16 step_counter = 50;

struct Pipe
{
    UINT8 sprite_id;
    UINT8 x;
    UINT8 y;
};

struct ColumnOfPipes
{
    bool is_created;
    struct Pipe pipes[6];
};

struct ColumnOfPipes columns[4];
UINT8 index_col_pipes = 0;

// Game logic vars
INT8 button_released = 1;
INT8 current_jump_acc = 0;
INT8 jump_limit = 15;
UINT8 acc = 0;
const UINT8 limit = 1;
UINT8 wait_acc = 0;
const UINT16 wait_limit = 2;
UINT8 last_sprite_id = 0;

void setup();
void update();
void perform_deplay(UINT8 times);
void create_pipe(UINT8 index, UINT8 x);
INT8 get_next_available();
INT8 get_next_slot_col_pipe_available();

void main()
{
    setup();
    while (1)
    {
        update();
        perform_deplay(2);
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
    for (UINT8 i = 0; i != 40; i++)
    {
        sprite_ids[i] = false;
    }

    for (UINT8 j = 0; j != num_pipe_cols; j++)
    {
        columns[j].is_created = false;
    }

    state = WAITING_TO_START;

    // Setup Background
    set_bkg_data(0, 10, bkgs);
    set_bkg_tiles(0, 0, 32, 18, tilemap);

    // Load Sprites
    set_sprite_data(0, 4, sprites);

    setup_player(&player, sprite_ids);

    // create_pipe(160 + 56+ 56);
    //  create_pipe(160 + 56+ 56+ 56);
    //create_pipe(160 + 56+ 56+ 56+ 56);
    UINT8 count = 0;
    for (UINT8 q = 0; q != 40; q++)
    {
        if (sprite_ids[q] == true)
        {
            count++;
        }
    }

    printf("count: %d", count);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}
bool create_new_one = false;
void update()
{
    if (state != WAITING_TO_START)
    {
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

    if (state != WAITING_TO_START)
    {
        for (UINT8 i = 0; i != num_pipe_cols; i++)
        {
            for (UINT8 j = 0; j != 6; j++)
            {
                if (columns[i].is_created == true)
                {
                    columns[i].pipes[j].x += -1;
                    move_sprite(columns[i].pipes[j].sprite_id, columns[i].pipes[j].x, columns[i].pipes[j].y);
                }
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

    if (state == JUMPING)
    {
        current_jump_acc += 4;
        //scroll_sprite(0, 0, -3);
        move_game_object(&player, 0, -4);
        if (current_jump_acc >= jump_limit)
        {
            state = FALLING;
            current_jump_acc = 0;
        }
    }

    if (joypad() == J_UP && button_released == 1 && state == FALLING)
    {
        state = JUMPING;
        button_released = 0;
    }

    if (joypad() != J_UP)
    {
        button_released = 1;
    }

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


/*
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



    if (joypad() != J_UP)
    {
        //printf("x: %d, y: %d", player.x, player.y);
        button_released = 1;
    }*/

    //    if (player.x + player.width > pipes[0].upper_x && player.x < pipes[0].upper_x + pipes[0].upper_width && player.y < pipes[0].upper_y && player.y > pipes[0].upper_y - pipes[0].upper_height) {
    //printf("Collision");
    //  }


    if (joypad() == J_A)
    {
        UINT8 count = 0;
        for (UINT8 q = 0; q != 40; q++)
        {
            if (sprite_ids[q] == true)
            {
                count++;
            }
        }

        printf("count: %d", count);
    }
}

const UINT8 v_sepration = 32;
const UINT8 v_screen_offset = 16;
const UINT8 h_screen_offset = 8;

void create_pipe(UINT8 index, UINT8 x)
{
    UINT8 upper_blank = 0;
    UINT8 upper_count = 3;
    UINT8 space_count = 4;
    UINT8 lower_count = 3;

    UINT8 starting_y_coor = (upper_blank * 8) + v_screen_offset;

    UINT8 upper_pipe_limit = (upper_count * 8) + starting_y_coor;
    UINT8 pipe_count = 0;
    for (UINT8 y = starting_y_coor; y < upper_pipe_limit; y += 8)
    {
        INT8 id = get_next_available();

        sprite_ids[id] = true;
        set_sprite_tile(id, (y + 8) == upper_pipe_limit ? 3 : 2);
        move_sprite(id, x, y);
        columns[index].pipes[pipe_count].sprite_id = id;
        columns[index].pipes[pipe_count].x = x;
        columns[index].pipes[pipe_count].y = y;
        pipe_count++;
    }

    UINT8 lower_pipe_begining = upper_pipe_limit + (space_count * 8);
    UINT8 lower_pipe_limit = lower_pipe_begining + (lower_count * 8);

    for (UINT8 j = lower_pipe_begining; j < lower_pipe_limit; j += 8)
    {
        UINT8 id2 = get_next_available();

        sprite_ids[id2] = true;
        set_sprite_tile(id2, j == lower_pipe_begining ? 1 : 2);
        move_sprite(id2, x, j);
        columns[index].pipes[pipe_count].sprite_id = id2;
        columns[index].pipes[pipe_count].x = x;
        columns[index].pipes[pipe_count].y = j;
        pipe_count++;
    }
    columns[index].is_created = true;
    index_col_pipes++;
}

INT8 get_next_available()
{
    INT8 next_available = -1;
    INT8 i;
    for (i = 0; i != 40; i++)
    {
        if (sprite_ids[i] == false)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
}

INT8 get_next_slot_col_pipe_available()
{
    INT8 next_available = -1;
    INT8 i;
    for (i = 0; i != num_pipe_cols; i++)
    {
        if (columns[i].is_created == false)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
}