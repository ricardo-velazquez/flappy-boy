#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "sprites.c"
#include "bkgs.c"
#include "tilemap.c"


void setup();
void setup_player();
void update();

void move_game_object();



UINT8 wait_acc = 0;
UINT16 wait_limit = 2;

INT8 button_released = 1;
// FALLING 0 
// JUMPING 1
// SUSPENDED ON AIR 2
INT8 state = 0;
INT8 current_jump_acc = 0;
INT8 jump_limit =25;


UINT8 acc  = 0;
UINT8 limit  = 1;

struct player {
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
}



void main () { 
    setup();
    while (1) {
        update();
        wait_vbl_done();
        delay(5);
    }
}
void setup() {
    // Setup Player


    set_bkg_data(0, 7, bkgs);
    set_bkg_tiles(0, 0, 40, 18, tilemap);

    set_sprite_data(0, 1, sprites);
    setup_player();
    
    //set_sprite_prop(0, get_sprite_prop(0) & ~S_PRIORITY);
//set_sprite_prop(0, S_PRIORITY);
    SHOW_BKG;
    DISPLAY_ON;
    SHOW_SPRITES;

    
}
void update() {

    if (state != 4) {
        if (wait_acc != wait_limit) {
            scroll_bkg(1, 0);
            wait_acc++;
        } else {
            wait_acc = 0;
        }
    }
    if (state == 0) 
        scroll_sprite(0, 0, 1);
    

    if (state == 1) {
        current_jump_acc += 3;
        //scroll_sprite(0, 0, -3);
        move_game_object(&player, 0, -3);
        if (current_jump_acc >= jump_limit) {
            state = 3;
            current_jump_acc = 0;
        } 
    }

    if (state == 3) {
        if (wait_acc >= wait_limit) {
            state = 0;
            wait_acc=0;
        }
        wait_acc++;
    }

    if (joypad() == J_UP && button_released == 1) {
            state = 1;
            button_released = 0;
    } 

    if (joypad() != J_UP) {
        button_released = 1;
    }
}



void setup_player() {
    player->sprite_id = 0;
    player->x = 40;
    player->y = 60;
    player->width  = 8;
    player->height = 8;
    set_sprite_tile(player->sprite_id, player->sprite_id);
    move_sprite(player->sprite_id, player->x, player->y);
}

void move_game_object(struct* obj, UINT8 x, UINT8 y) {
    move_sprite(obj->sprite_id, obj->x + x , obj->y + y);
}







