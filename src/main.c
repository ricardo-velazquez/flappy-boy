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

const enum states {FALLING, JUMPING, JUMP_ENDED, WAITING_TO_START, DEAD};
enum states state;

struct Player player;
struct PipeCol pipes[4];

UINT8 last_sprite_id = 0;

void setup();
void update();


UINT8 wait_acc = 0;
const UINT16 wait_limit = 2;

INT8 button_released = 1;

INT8 current_jump_acc = 0;
INT8 jump_limit =25;
UINT8 acc  = 0;
const UINT8 limit  = 1;



void main () { 
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

    setup_player(&player);

    setup_pipes(pipes, last_sprite_id);
    

    //font_set(new_font);

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

    if (player.x + player.width > pipes[0].upper_x && player.x < pipes[0].upper_x + pipes[0].upper_width && player.y < pipes[0].upper_y && player.y > pipes[0].upper_y - pipes[0].upper_height) {
        printf("Collision");
    }

}
