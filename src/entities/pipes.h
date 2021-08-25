#include <gb/gb.h>

extern struct Pipe {
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

extern struct ColumnOfPipe {
    struct Pipe pipes[5];
};

extern struct ColumnOfPipe columnsOfCol [5];


UINT8 setup_pipes(UBYTE sprite_ids[]);

struct ColumnOfPipe create_column_of_pipes( UBYTE sprite_ids[], UINT8 x);

INT8 get_next_available(UBYTE sprite_ids[]);