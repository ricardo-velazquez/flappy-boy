#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "resources/sprites.h"
#include "resources/bkgs.h"
#include "resources/tilemap.h"

#include "entities/player.h"


struct Player player;

UINT8 last_sprite_id = 0;

void setup();
void setup_pipes();
void update();
void move_game_object(struct Player* obj, UINT8 x, UINT8 y);

extern struct Pipe {
    UBYTE sprite_id[4];
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

enum states {FALLING, JUMPING, JUMP_ENDED, WAITING_TO_START, DEAD};

enum states state;


UINT8 wait_acc = 0;
UINT16 wait_limit = 2;

INT8 button_released = 1;
// FALLING 0 
// JUMPING 1
// SUSPENDED ON AIR 2
//INT8 state = 0;

INT8 current_jump_acc = 0;
INT8 jump_limit =25;
UINT8 acc  = 0;
UINT8 limit  = 1;



void main () { 
    setup();
    while (1) {
        update();
        wait_vbl_done();
        delay(5);
    }
}
void setup() {

    state = WAITING_TO_START;
    // Setup Player
    set_bkg_data(0, 7, bkgs);
    set_bkg_tiles(0, 0, 40, 18, tilemap);

    set_sprite_data(0, 4, sprites);
    setup_player_draft(&player);
    //setup_pipes();

    
    
    //set_sprite_prop(0, get_sprite_prop(0) & ~S_PRIORITY);
//set_sprite_prop(0, S_PRIORITY);
    SHOW_BKG;
    DISPLAY_ON;
    SHOW_SPRITES;

    
    
}
void update() {


    

    if (wait_acc != wait_limit) {
        scroll_bkg(1, 0);
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
        button_released = 1;
    }
}

void setup_pipes(){

    UINT8 sprite_id = 1;
    UINT8 h_spaceing_between_pipes = 6;
    UINT8 v_spaceing_between_pipes = 6;

    for (UINT8 i = 1; i != (3 + 1); i++) {
        set_sprite_tile(sprite_id, 1);
        move_sprite(sprite_id, (8 * i * h_spaceing_between_pipes ), 8 * 10);
        sprite_id++;

        set_sprite_tile(sprite_id, 2);
        move_sprite(sprite_id, (8 * i * h_spaceing_between_pipes), 8 * 11);
        sprite_id++;


        set_sprite_tile(sprite_id, 3);
        move_sprite(sprite_id, (8 * i * h_spaceing_between_pipes), 8 * 5);
        sprite_id++;

        set_sprite_tile(sprite_id, 2);
        move_sprite(sprite_id, (8 * i * h_spaceing_between_pipes), 8 * 4);
        sprite_id++;
    }




/*

    set_sprite_tile(++last_sprite_id, 1);
    move_sprite(last_sprite_id, 8 * 10, 8 * 10);
    set_sprite_tile(++last_sprite_id, 2);
    move_sprite(last_sprite_id, 8 * 10, 8 * 11);
    set_sprite_tile(++last_sprite_id, 2);
    move_sprite(last_sprite_id, 8 * 10, 8 * 12);*/
}



void move_game_object(struct Player* obj, UINT8 x, UINT8 y) {
    obj->x += x;
    obj->y += y;
    move_sprite(obj->sprite_id, obj->x , obj->y + y);
}







