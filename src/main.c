#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "Export.c"


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
UINT8 limit  = 2;

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
    set_sprite_data(0, 4, Sprites);
    set_sprite_tile(0, 0);
    move_sprite(0, 40, 60);
    SHOW_SPRITES;
}
void update() {
    if (state == 0) {
        //if (acc <= limit) {
            scroll_sprite(0, 0, 1);
        //    acc=0;
        //}
        //acc++;
    }

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








