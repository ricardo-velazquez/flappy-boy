
#include <gb/gb.h>


void setup_sound () { 
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels
}

void play_sound_add_score () {
    NR11_REG = 0x00;
    NR12_REG = 0x81;  
    NR13_REG = 0xB7;   
    NR14_REG = 0x86;
}