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
#include "gb_helpers.h"
#include "win_layer_panel.c"
#include "macros.h"
#include <gb/drawing.h>
//#include "entities/pipes.h"

/*
There are a total of 32 tiles (8x8 pixels) in the with of the map.

The visible space start at (8, 16)

*/
bool is_playing = true;

extern UINT8 session_highscore[4];
extern UBYTE session[1];

const enum states { FALLING,
                    JUMPING,
                    JUMP_ENDED,
                    WAITING_TO_START,
                    DEAD };
enum states state;

UINT8 score[4];



bool sprite_ids[SPRITE_LIMIT];
const UINT8 num_pipe_cols = 4;
// GameObjects declaration
struct Player player;
const UINT16 step_limit = 45;
UINT16 step_counter = 45;

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
void create_pipe(UINT8 index, UINT8 x);
INT8 get_next_available();
INT8 get_next_slot_col_pipe_available();
void move_game_object(UINT8 x, UINT8 y);
void setup_player();

UINT16 get_raw_score_from_array(UINT8 score[]);




void update_display_score()
{
    FOR_FROM_ZERO_TO(4) set_win_tile_xy((i + 1), 1, 12 + score[(4 - i - 1)]);
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

void main()
{

    do {
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

void setup()
{

    // Init sprite ids array
    FOR_FROM_ZERO_TO(40) {sprite_ids[i] = false; move_sprite(i, 168, 0); }

    FOR_I(0, num_pipe_cols) {
        FOR_J(0, 6) {
            columns[i].pipes[j].x = 168;
            columns[i].pipes[j].y = 0;
        }
    }

    FOR_FROM_ZERO_TO(num_pipe_cols)
    {
        columns[i].is_created = false;
        columns[i].is_scored = false;
    }

        for (UINT8 r = 0; r != 4; r++) 
    {score[r] = 0;
   // printf("%d: %d\n",r, score[r] );
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
    set_bkg_data(0, 44, bkgs);
    set_bkg_tiles(0, 0, 32, 18, tilemap);

    // Load Sprites
    set_sprite_data(0, 4, sprites);

    setup_player();


    setup_win_before_game();

    SHOW_BKG;
    SHOW_SPRITES;
    SHOW_WIN;
    DISPLAY_ON;
}
bool create_new_one = false;

void update()
{

    if (state == DEAD) { is_playing = false; return;}

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

                   printf("ERROR");
              
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

        //delay(300);

        printf("ERROR");
    }

    if (state == JUMPING)
    {
        current_jump_acc += JUMP_SPEED;
        //scroll_sprite(0, 0, -3);
        move_game_object( 0, -JUMP_SPEED);
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
        move_game_object( 0, 1);
    }

     if (joypad() == J_A)
        {
            printf("id: %d\n", player.sprite_id);
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

INT8 get_next_slot_col_pipe_available()
{
    INT8 next_available = -1;

    FOR_FROM_ZERO_TO(num_pipe_cols)
    {
        if (columns[i].is_created == false)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
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

void move_game_object( UINT8 x, UINT8 y) {
    player.x += x;
    player.y += y;
    move_sprite(player.sprite_id, player.x + x, player.y + y);
}

void setup_player() {
    sprite_ids[0] = true;
    player.sprite_id = 0;
    player.x = 32;
    player.y = 60;
    player.width  = 8;
    player.height = 8;
    set_sprite_tile(player.sprite_id, player.sprite_id);
    move_sprite(player.sprite_id, player.x, player.y);
}