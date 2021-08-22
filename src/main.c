#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "sprites.c"
#include "bkgs.c"
#include "tilemap.c"


void bootstrap();
void update();


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

void main () { 
    bootstrap();
    while (1) {
        update();
        wait_vbl_done();
        delay(5);
    }
}
void bootstrap() {
    // Setup Player


    set_bkg_data(0, 7, bkgs);
    set_bkg_tiles(0, 0, 40, 18, tilemap);

    set_sprite_data(0, 1, sprites);
    set_sprite_tile(0, 0);
    move_sprite(0, 40, 60);
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
        scroll_sprite(0, 0, -3);
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








