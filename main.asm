;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.1.6 #12439 (MINGW32)
;--------------------------------------------------------
	.module main
	.optsdcc -mgbz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _set_sprite_data
	.globl _set_bkg_tiles
	.globl _set_bkg_data
	.globl _wait_vbl_done
	.globl _joypad
	.globl _delay
	.globl _limit
	.globl _acc
	.globl _jump_limit
	.globl _current_jump_acc
	.globl _state
	.globl _button_released
	.globl _wait_limit
	.globl _wait_acc
	.globl _bkgs
	.globl _sprites
	.globl _player
	.globl _setup
	.globl _update
	.globl _setup_player
	.globl _move_game_object
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
_player::
	.ds 5
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
_sprites::
	.ds 64
_bkgs::
	.ds 112
_wait_acc::
	.ds 1
_wait_limit::
	.ds 2
_button_released::
	.ds 1
_state::
	.ds 1
_current_jump_acc::
	.ds 1
_jump_limit::
	.ds 1
_acc::
	.ds 1
_limit::
	.ds 1
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;src/main.c:44: void main () { 
;	---------------------------------
; Function main
; ---------------------------------
_main::
;src/main.c:45: setup();
	call	_setup
;src/main.c:46: while (1) {
00102$:
;src/main.c:47: update();
	call	_update
;src/main.c:48: wait_vbl_done();
	call	_wait_vbl_done
;src/main.c:49: delay(5);
	ld	de, #0x0005
	push	de
	call	_delay
	pop	hl
;src/main.c:51: }
	jr	00102$
;src/main.c:52: void setup() {
;	---------------------------------
; Function setup
; ---------------------------------
_setup::
;src/main.c:54: set_bkg_data(0, 7, bkgs);
	ld	de, #_bkgs
	push	de
	ld	hl, #0x700
	push	hl
	call	_set_bkg_data
	add	sp, #4
;src/main.c:55: set_bkg_tiles(0, 0, 40, 18, tilemap);
	ld	de, #_tilemap
	push	de
	ld	hl, #0x1228
	push	hl
	xor	a, a
	rrca
	push	af
	call	_set_bkg_tiles
	add	sp, #6
;src/main.c:57: set_sprite_data(0, 1, sprites);
	ld	de, #_sprites
	push	de
	xor	a, a
	inc	a
	push	af
	call	_set_sprite_data
	add	sp, #4
;src/main.c:58: setup_player(&player);
	ld	de, #_player
	push	de
	call	_setup_player
	pop	hl
;src/main.c:62: SHOW_BKG;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x01
	ldh	(_LCDC_REG + 0), a
;src/main.c:63: DISPLAY_ON;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x80
	ldh	(_LCDC_REG + 0), a
;src/main.c:64: SHOW_SPRITES;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x02
	ldh	(_LCDC_REG + 0), a
;src/main.c:67: }
	ret
;src/main.c:68: void update() {
;	---------------------------------
; Function update
; ---------------------------------
_update::
;src/main.c:70: if (state != 4) {
;setupPair	HL
	ld	a, (#_state)
	sub	a, #0x04
	jr	Z, 00105$
;src/main.c:71: if (wait_acc != wait_limit) {
;setupPair	HL
	ld	hl, #_wait_acc
	ld	c, (hl)
	ld	b, #0x00
;setupPair	HL
	ld	hl, #_wait_limit
	ld	a, (hl)
	sub	a, c
	jr	NZ, 00175$
;setupPair	HL
	inc	hl
	ld	a, (hl)
	sub	a, b
	jr	Z, 00102$
00175$:
;C:/gbdk/include/gb/gb.h:860: SCX_REG+=x, SCY_REG+=y;
	ldh	a, (_SCX_REG + 0)
	inc	a
	ldh	(_SCX_REG + 0), a
;src/main.c:73: wait_acc++;
;setupPair	HL
	ld	hl, #_wait_acc
	inc	(hl)
	jr	00105$
00102$:
;src/main.c:75: wait_acc = 0;
;setupPair	HL
	ld	hl, #_wait_acc
	ld	(hl), #0x00
00105$:
;src/main.c:78: if (state == 0)  {
;setupPair	HL
	ld	a, (#_state)
	or	a, a
	jr	NZ, 00107$
;src/main.c:80: move_game_object(&player, 0, 1);
	xor	a, a
	inc	a
	push	af
	ld	de, #_player
	push	de
	call	_move_game_object
	add	sp, #4
00107$:
;src/main.c:85: if (state == 1) {
;setupPair	HL
	ld	a, (#_state)
	dec	a
	jr	NZ, 00111$
;src/main.c:86: current_jump_acc += 3;
;setupPair	HL
	ld	hl, #_current_jump_acc
	inc	(hl)
;setupPair	HL
	inc	(hl)
;setupPair	HL
	inc	(hl)
;src/main.c:88: move_game_object(&player, 0, -3);
	ld	hl, #0xfd00
	push	hl
	ld	de, #_player
	push	de
	call	_move_game_object
	add	sp, #4
;src/main.c:89: if (current_jump_acc >= jump_limit) {
;setupPair	HL
	ld	hl, #_jump_limit
	ld	e, (hl)
;setupPair	HL
	ld	hl, #_current_jump_acc
	ld	d, (hl)
;setupPair	HL
	ld	a, (hl)
;setupPair	HL
	ld	hl, #_jump_limit
	sub	a, (hl)
	bit	7, e
	jr	Z, 00178$
	bit	7, d
	jr	NZ, 00179$
	cp	a, a
	jr	00179$
00178$:
	bit	7, d
	jr	Z, 00179$
	scf
00179$:
	jr	C, 00111$
;src/main.c:90: state = 3;
;setupPair	HL
	ld	hl, #_state
	ld	(hl), #0x03
;src/main.c:91: current_jump_acc = 0;
;setupPair	HL
	ld	hl, #_current_jump_acc
	ld	(hl), #0x00
00111$:
;src/main.c:95: if (state == 3) {
;setupPair	HL
	ld	a, (#_state)
	sub	a, #0x03
	jr	NZ, 00115$
;src/main.c:96: if (wait_acc >= wait_limit) {
;setupPair	HL
	ld	hl, #_wait_acc
	ld	c, (hl)
	ld	b, #0x00
;setupPair	HL
	ld	hl, #_wait_limit
	ld	a, c
	sub	a, (hl)
	inc	hl
	ld	a, b
	sbc	a, (hl)
	jr	C, 00113$
;src/main.c:97: state = 0;
;setupPair	HL
	ld	hl, #_state
	ld	(hl), #0x00
;src/main.c:98: wait_acc=0;
;setupPair	HL
	ld	hl, #_wait_acc
	ld	(hl), #0x00
00113$:
;src/main.c:100: wait_acc++;
;setupPair	HL
	ld	hl, #_wait_acc
	inc	(hl)
00115$:
;src/main.c:103: if (joypad() == J_UP && button_released == 1) {
	call	_joypad
	ld	a, e
	sub	a, #0x04
	jr	NZ, 00117$
;setupPair	HL
	ld	a, (#_button_released)
	dec	a
	jr	NZ, 00117$
;src/main.c:104: state = 1;
;setupPair	HL
	ld	hl, #_state
	ld	(hl), #0x01
;src/main.c:105: button_released = 0;
;setupPair	HL
	ld	hl, #_button_released
	ld	(hl), #0x00
00117$:
;src/main.c:108: if (joypad() != J_UP) {
	call	_joypad
	ld	a, e
	sub	a, #0x04
	ret	Z
;src/main.c:109: button_released = 1;
;setupPair	HL
	ld	hl, #_button_released
	ld	(hl), #0x01
;src/main.c:111: }
	ret
;src/main.c:115: void setup_player(struct Player* player) {
;	---------------------------------
; Function setup_player
; ---------------------------------
_setup_player::
	add	sp, #-6
;src/main.c:116: player->sprite_id = 0;
	ldhl	sp,	#8
	ld	a, (hl+)
	ld	c, a
	ld	b, (hl)
	xor	a, a
	ld	(bc), a
;src/main.c:117: player->x = 40;
	ld	l, c
	ld	h, b
	inc	hl
	push	hl
	ld	a, l
	ldhl	sp,	#4
	ld	(hl), a
	pop	hl
	ld	a, h
	ldhl	sp,	#3
	ld	(hl-), a
	ld	a,	(hl+)
	ld	h, (hl)
	ld	l, a
	ld	(hl), #0x28
;src/main.c:118: player->y = 60;
	ld	hl, #0x0002
	add	hl, bc
	inc	sp
	inc	sp
	push	hl
	ld	(hl), #0x3c
;src/main.c:119: player->width  = 8;
	ld	l, c
;	spillPairReg hl
;	spillPairReg hl
	ld	h, b
;	spillPairReg hl
;	spillPairReg hl
	inc	hl
	inc	hl
	inc	hl
	ld	(hl), #0x08
;src/main.c:120: player->height = 8;
	ld	hl, #0x0004
	add	hl, bc
	ld	(hl), #0x08
;src/main.c:121: set_sprite_tile(player->sprite_id, player->sprite_id);
	ld	a, (bc)
	ld	e, a
	ld	d, a
;C:/gbdk/include/gb/gb.h:1174: shadow_OAM[nb].tile=tile;
	ld	h, #0x00
;	spillPairReg hl
;	spillPairReg hl
	ld	l, d
	add	hl, hl
	add	hl, hl
	push	de
	ld	de, #_shadow_OAM
	add	hl, de
	inc	hl
	inc	hl
	pop	de
	ld	(hl), e
;src/main.c:122: move_sprite(player->sprite_id, player->x, player->y);
	pop	de
	push	de
	ld	a, (de)
	ldhl	sp,	#4
	ld	(hl-), a
	dec	hl
	ld	a, (hl+)
	ld	e, a
	ld	a, (hl+)
	inc	hl
	ld	d, a
	ld	a, (de)
	ld	(hl), a
	ld	a, (bc)
;C:/gbdk/include/gb/gb.h:1247: OAM_item_t * itm = &shadow_OAM[nb];
	ld	l, a
	ld	bc, #_shadow_OAM+0
;	spillPairReg hl
;	spillPairReg hl
	ld	h, #0x00
;	spillPairReg hl
;	spillPairReg hl
	add	hl, hl
	add	hl, hl
	add	hl, bc
	ld	c, l
	ld	b, h
;C:/gbdk/include/gb/gb.h:1248: itm->y=y, itm->x=x;
	ldhl	sp,	#4
	ld	a, (hl+)
	ld	(bc), a
	inc	bc
	ld	a, (hl)
	ld	(bc), a
;src/main.c:122: move_sprite(player->sprite_id, player->x, player->y);
;src/main.c:123: }
	add	sp, #6
	ret
;src/main.c:127: void move_game_object(struct Player* obj, UINT8 x, UINT8 y) {
;	---------------------------------
; Function move_game_object
; ---------------------------------
_move_game_object::
	dec	sp
;src/main.c:128: obj->x += x;
	ldhl	sp,	#3
	ld	a, (hl+)
	ld	e, a
	ld	a, (hl+)
	ld	d, a
	ld	c, e
	ld	b, d
	inc	bc
	ld	a, (bc)
	add	a, (hl)
	ldhl	sp,	#0
	ld	(hl), a
	ld	a, (hl)
	ld	(bc), a
;src/main.c:129: obj->y += y;
	ld	c, e
	ld	b, d
	inc	bc
	inc	bc
	ld	a, (bc)
	ldhl	sp,	#6
	add	a, (hl)
	ld	(bc), a
;src/main.c:130: move_sprite(obj->sprite_id, obj->x , obj->y + y);
	add	a, (hl)
	ld	b, a
	ldhl	sp,	#0
	ld	c, (hl)
	ld	a, (de)
;C:/gbdk/include/gb/gb.h:1247: OAM_item_t * itm = &shadow_OAM[nb];
	ld	l, a
;	spillPairReg hl
;	spillPairReg hl
	ld	h, #0x00
;	spillPairReg hl
;	spillPairReg hl
	add	hl, hl
	add	hl, hl
	ld	de, #_shadow_OAM
	add	hl, de
;C:/gbdk/include/gb/gb.h:1248: itm->y=y, itm->x=x;
	ld	a, b
	ld	(hl+), a
	ld	(hl), c
;src/main.c:130: move_sprite(obj->sprite_id, obj->x , obj->y + y);
;src/main.c:131: }
	inc	sp
	ret
	.area _CODE
	.area _INITIALIZER
__xinit__sprites:
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x78	; 120	'x'
	.db #0x78	; 120	'x'
	.db #0xfc	; 252
	.db #0x84	; 132
	.db #0xfc	; 252
	.db #0x94	; 148
	.db #0xfe	; 254
	.db #0x86	; 134
	.db #0xfc	; 252
	.db #0x84	; 132
	.db #0x78	; 120	'x'
	.db #0x78	; 120	'x'
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x83	; 131
	.db #0x81	; 129
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0x46	; 70	'F'
	.db #0x42	; 66	'B'
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x83	; 131
	.db #0x81	; 129
	.db #0xff	; 255
	.db #0xff	; 255
__xinit__bkgs:
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x02	; 2
	.db #0x02	; 2
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x14	; 20
	.db #0x00	; 0
	.db #0x42	; 66	'B'
	.db #0x00	; 0
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0x7e	; 126
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x0a	; 10
	.db #0x00	; 0
	.db #0x11	; 17
	.db #0x00	; 0
	.db #0x0e	; 14
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x60	; 96
	.db #0x00	; 0
	.db #0x90	; 144
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
__xinit__wait_acc:
	.db #0x00	; 0
__xinit__wait_limit:
	.dw #0x0002
__xinit__button_released:
	.db #0x01	;  1
__xinit__state:
	.db #0x00	;  0
__xinit__current_jump_acc:
	.db #0x00	;  0
__xinit__jump_limit:
	.db #0x19	;  25
__xinit__acc:
	.db #0x00	; 0
__xinit__limit:
	.db #0x01	; 1
	.area _CABS (ABS)
