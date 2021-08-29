#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include <gb/font.h>
#include <bcd.h>

#include "res/sprites.h"
#include "res/bkgs.h"
#include "res/tilemap.h"

#include "entities/player.h"
#include "utils.c"
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

UINT8 score[4];
UINT8 high_score[4];
extern UBYTE session_highscore[4];
extern UBYTE session[1];

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
    bool is_scored;
    struct Pipe pipes[6];
};

struct ColumnOfPipes columns[4];
UINT8 index_col_pipes = 0;

// Game logic vars
const UINT8 JUMP_SPEED = 4;
bool button_released = true;
INT8 current_jump_acc = 0;
const INT8 jump_limit = 15;
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
    ENABLE_RAM_MBC1;
    setup();
    while (1)
    {
        update();
        perform_deplay(2);
    }
    DISABLE_RAM_MBC1;
}

void perform_deplay(UINT8 times)
{
    for (UINT8 i = 0; i < times; i++)
    {
        wait_vbl_done();
    }
}

void setup_win()
{
    move_win(8, 120 - 8);

    // High score label
    set_win_tile_xy(1, 2, 22);
    set_win_tile_xy(2, 2, 23);
    set_win_tile_xy(3, 2, 24);
    // High score value
    for (UINT8 i = 0; i != 4; i++)
    {
        //printf("%d", session_highscore[i]);
        set_win_tile_xy(3 + (i + 1), 2, 12 + session_highscore[(4 - i - 1)]);
    }

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

    for (UINT8 j = 1; j <= 18; j++)
    {
        set_win_tile_xy(j, 0, 37);
        set_win_tile_xy(19 - j, 3, 36);
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
        columns[j].is_scored = false;
    }
    for (UINT8 r = 0; r != 4; r++)
    {
        score[r] = 0;
        high_score[r] = 0;
    }

    if (session[0] != 's')
    {
        for (UINT8 w = 0; w != 4; w++)
        {
            session_highscore[w] = 0;
        }                 // Check to see if the variable's ever been saved before
                          // Assign the variable an initial value
        session[0] = 's'; // Assign saved an 's' value so the if statement isn't executed on next load
    }

    state = WAITING_TO_START;

    // Setup Background
    set_bkg_data(0, 38, bkgs);
    set_bkg_tiles(0, 0, 32, 18, tilemap);

    // Load Sprites
    set_sprite_data(0, 4, sprites);

    setup_player(&player, sprite_ids);
    setup_win();
    UINT8 count = 0;
    for (UINT8 q = 0; q != 40; q++)
    {
        if (sprite_ids[q] == true)
        {
            count++;
        }
    }
    // Enable RAM

    //printf("pre: %d \n", session_highscore[0]);
    //ENABLE_RAM_MBC1;

    // RAMPtr = (UBYTE *)0xa000;

    //printf("Hex number -> 0x%x", RAMPtr[0]);
    //printf("pre: %d \n", RAMPtr[0]);
    //  RAMPtr[0]++;
    //printf("post: %d \n", RAMPtr[0]);

    //DISABLE_RAM_MBC1;

    SHOW_BKG;
    SHOW_SPRITES;
    SHOW_WIN;
    DISPLAY_ON;
}
bool create_new_one = false;

void update_dispaly_score()
{
    for (UINT8 i = 0; i != 4; i++)
    {
        set_win_tile_xy((i + 1), 1, 12 + score[(4 - i - 1)]);
    }
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

void update()
{

    update_dispaly_score();

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

    if (state != WAITING_TO_START && state != DEAD)
    {
        for (UINT8 i = 0; i != num_pipe_cols; i++)
        {
            for (UINT8 j = 0; j != 6; j++)
            {
                // Check for colissions

                if (player.x + 7 > columns[i].pipes[j].x && player.x < columns[i].pipes[j].x + 7 && player.y < columns[i].pipes[j].y && player.y > columns[i].pipes[j].y - 8)
                {
                    // printf("Collision");
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

        //ENABLE_RAM_MBC1; // Enable RAM

        session_highscore[0] = score[0];

        //DISABLE_RAM_MBC1;
    }

    if (state == JUMPING)
    {
        current_jump_acc += JUMP_SPEED;
        //scroll_sprite(0, 0, -3);
        move_game_object(&player, 0, -JUMP_SPEED);
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
    UINT8 upper_count = random_range(1, 6);
    UINT8 space_count = 4;
    UINT8 lower_count = 10 - upper_count - space_count - upper_blank;

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
    columns[index].is_scored = false;
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