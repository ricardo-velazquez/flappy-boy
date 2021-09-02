#define FOR(start, end) for (UINT8 i = start; i != end; i++)
#define FORI(start, end, inc) for (UINT8 i = start; i != end; i+=inc)
#define FOR_FROM_ZERO_TO(end) for (UINT8 i = 0; i != end; i++)




#define SPRITE_LIMIT 40



//#define FOR(start, end, inc) for (UINT8 i = start; i != end; i+= inc)
#define FOR_0_TO(start, end) for (UINT8 i = start; i != end; i++)



#define FOR_I(start, end) for (UINT8 i = start; i != end; i++)
#define FOR_J(start, end) for (UINT8 j = start; j != end; j++)
#define FOR_K(start, end) for (UINT8 k = start; k != end; k++)
#define RESET_GB() __asm__("RST 0")
#define FOR_AND_CALLBACK(start, end, to_do) for (UINT8 i = start; i != end; i++) { to_do }