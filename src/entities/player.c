#include <gb/gb.h>
#include "player.h"



void setup_player(struct Player* player) {
    player->sprite_id = 0;
    player->x = 32;
    player->y = 60;
    player->width  = 8;
    player->height = 8;
    set_sprite_tile(player->sprite_id, player->sprite_id);
    move_sprite(player->sprite_id, player->x, player->y);
}

void move_game_object(struct Player* obj, UINT8 x, UINT8 y) {
    obj->x += x;
    obj->y += y;
    move_sprite(obj->sprite_id, obj->x + x, obj->y + y);
}
