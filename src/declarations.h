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



bool sprite_ids[40];

// GameObjects declaration

const UINT16 step_limit = 45;
UINT16 step_counter = 45;




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