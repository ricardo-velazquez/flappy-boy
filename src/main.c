#include <gb/gb.h>
//#include <stdint.h>
#include <stdio.h>
#include "resources/sprites.h"
#include "resources/bkgs.h"
#include "resources/tilemap.h"
#include "entities/player.h"



/*
There are a total of 32 tiles (8x8 pixels) in the with of the map.

The visible space start at (8, 16)

*/


struct Player player;
struct PipeCol {
    UINT8 upper_x;
    UINT8 upper_y;
    UINT8 upper_width;
    UINT8 upper_height;

    UINT8 lower_x;
    UINT8 lower_y;
    UINT8 lower_width;
    UINT8 lower_height;
};
struct PipeCol pipes[4];
UINT8 pipes_index =0;

UINT16 last_sprite_id = 0;

void setup();
void setup_pipes();
void create_column_of_pipes(UINT8 type_height_col_pipe, UINT8 x);
void update();
void move_game_object(struct Player* obj, UINT8 x, UINT8 y);
//INT8 rand_range(INT8 lower, INT8 upper);





const enum states {FALLING, JUMPING, JUMP_ENDED, WAITING_TO_START, DEAD};

enum states state;


UINT8 wait_acc = 0;
const UINT16 wait_limit = 2;

INT8 button_released = 1;
// FALLING 0 
// JUMPING 1
// SUSPENDED ON AIR 2
//INT8 state = 0;

INT8 current_jump_acc = 0;
INT8 jump_limit =25;
UINT8 acc  = 0;
const UINT8 limit  = 1;



void main () { 
    setup_pipes();
    setup();
    
    
    while (1) {
        update();
        wait_vbl_done();
        wait_vbl_done();
  
    }
}
void setup() {

    state = WAITING_TO_START;
    // Setup Player
    set_bkg_data(0, 10, bkgs);
    set_bkg_tiles(0, 0, 32, 18, tilemap);

    set_sprite_data(0, 4, sprites);
    setup_player_draft(&player);
   printf("x: %d, y: %d\n", pipes[0].upper_x, pipes[0].upper_y);
   printf("w: %d, h: %d\n", pipes[0].upper_width, pipes[0].upper_height);

    SHOW_BKG;
    DISPLAY_ON;
    SHOW_SPRITES;

    
    
}
void update() {

    if (wait_acc != wait_limit) {
        //scroll_bkg(1, 0);
        wait_acc++;
    } else {
        wait_acc = 0;
    }

    if (state == WAITING_TO_START) {
        if (joypad() == J_UP){
            state = FALLING;
        } else {
            return;
        }
    }
    
    if (state == FALLING)  {
        move_game_object(&player, 0, 1);
    } 

    if (state == JUMPING) {
        current_jump_acc += 3;
        //scroll_sprite(0, 0, -3);
        move_game_object(&player, 0, -3);
        if (current_jump_acc >= jump_limit) {
            state = JUMP_ENDED;
            current_jump_acc = 0;
        } 
    }

    if (state == JUMP_ENDED && wait_acc >= wait_limit) {
        state = FALLING;
    }

    if (joypad() == J_UP && button_released == 1) {
            state = JUMPING;
            button_released = 0;
    } 

    if (joypad() != J_UP) {
        //printf("x: %d, y: %d", player.x, player.y);
        button_released = 1;
    }

    if (player.x >= pipes[0].upper_x && player.x <= pipes[0].upper_x + pipes[0].upper_width && player.y <= pipes[0].upper_y  && player.y >= pipes[0].upper_y - pipes[0].upper_height) {
        printf("Collision!!");
    }


}
/*
INT8 rand_range(INT8 lower, INT8 upper) {
    INT8 num = (rand() %
           (upper - lower + 1)) + lower;
    return num;
}*/
const UINT8 v_sepration = 32;
const UINT8 v_screen_offset = 16;
const UINT8 h_screen_offset = 8;


void create_column_of_pipes( UINT8 type_height_col_pipe, UINT8 x) {
    // Define the height based on the type of column we want to build 
    UINT8 upper_pipe_limit;
    UINT8 lower_pipe_begining;
    UINT8 lower_pipe_limit;
    UINT8 height;
    switch (type_height_col_pipe)
    {
    case 1:
        height = 4;
        break;
    case 2:
        height = 5;
        break;
    case 3:
        height = 6;
        break; 
    default:
        height = 5;
        break;
    }

    /// UPPER PIPE SETUP
    
    upper_pipe_limit = (height * 8) + v_screen_offset;

    for (UINT8 y = 16+ 24; y < upper_pipe_limit; y+=8) {
        last_sprite_id++;
        set_sprite_tile(last_sprite_id, 2);
        move_sprite(last_sprite_id, x, y);

    }
    last_sprite_id++;
    set_sprite_tile(last_sprite_id, 3);
    move_sprite(last_sprite_id, x, upper_pipe_limit);

    lower_pipe_begining = upper_pipe_limit + v_sepration;

    /// LOWER PIPE SETUP
    last_sprite_id++;
    set_sprite_tile(last_sprite_id, 1);
    move_sprite(last_sprite_id, x, lower_pipe_begining );
    
     lower_pipe_limit = (16 + 144) - (8*8);

   // UINT8 remaining = ((256 - (16 + (8 * height) + v_sepration)) /8) - 12;

    for (UINT8 j = lower_pipe_begining + 8; j < lower_pipe_limit; j+=8) {
        last_sprite_id++;
        set_sprite_tile(last_sprite_id, 2);
        move_sprite(last_sprite_id, x, j);
    }
  


    pipes[pipes_index].upper_y = upper_pipe_limit + 8;
    pipes[pipes_index].upper_x = x;
    pipes[pipes_index].upper_width = 8;
    pipes[pipes_index].upper_height = upper_pipe_limit + 8;


    pipes[pipes_index].lower_y = lower_pipe_begining;
    pipes[pipes_index].lower_x = x;
    pipes[pipes_index].lower_width = 8;
    pipes[pipes_index].lower_height = 16 + 144 -32;
    pipes_index++;

}

void setup_pipes(){
    // High pipe positions
    // Mid pipe positions
    // Low pipe positions

    for (UINT8 i = 1 ; i <= 2; i++) { 
        create_column_of_pipes( i,  (24 * i) + (8 * i) );
    }



}



void move_game_object(struct Player* obj, UINT8 x, UINT8 y) {
    obj->x += x;
    obj->y += y;
    move_sprite(obj->sprite_id, obj->x , obj->y + y);
}







