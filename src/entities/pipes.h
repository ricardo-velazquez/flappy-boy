#include <gb/gb.h>


struct PipeCol {
    UBYTE is_empty;
    UBYTE sprite_ids[6];
    UINT8 x;
    UINT8 y[6];
    UINT8 upper_y;
    UINT8 lower_y;
};

void create_pipe(struct PipeCol columns[], UBYTE sprite_ids[], UINT8 x);


INT8 get_next_available(UBYTE sprite_ids[]);

INT8 push_column_of_pipe(struct PipeCol columns[], struct PipeCol* col);