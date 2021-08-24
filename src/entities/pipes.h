#include <gb/gb.h>

extern struct PipeCol {
    UINT8 upper_x;
    UINT8 upper_y;
    UINT8 upper_width;
    UINT8 upper_height;

    UINT8 lower_x;
    UINT8 lower_y;
    UINT8 lower_width;
    UINT8 lower_height;
};

void setup_pipes(struct PipeCol pipes[], UINT8 last_sprite_id);

void create_column_of_pipes(struct PipeCol pipes[],  UINT8 last_sprite_id,  UINT8 type_height_col_pipe, UINT8 xx);