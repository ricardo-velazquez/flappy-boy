#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "Export.c"


void bootstrap();
void update();



INT8 button_released = 1;
INT8 is_jumping = 0;
INT8 current_jump_acc = 0;
INT8 jump_limit = 20;

void main () {
    
    bootstrap();
    while (1) {
        update();
        wait_vbl_done();
       
    }
}


void bootstrap() {

    // Setup Player
    set_sprite_data(0, 4, Sprites);
    set_sprite_tile(0, 0);
    move_sprite(0, 40, 72);
    SHOW_SPRITES;
}


void update() {

    if (is_jumping != 1) {
        scroll_sprite(0, 0, 1);
        delay(20); 
        if (joypad() == J_UP && button_released == 1) {
            is_jumping = 1;
            button_released = 0;
        }  
    }

    if (is_jumping == 1) {
        current_jump_acc += 2;
        scroll_sprite(0, 0, -2);
        delay(10);
        if (current_jump_acc >= jump_limit) {
            is_jumping = 0;
            current_jump_acc = 0;
        } 
    }

    if (joypad() != J_UP) {
        button_released = 1;
    }



}








