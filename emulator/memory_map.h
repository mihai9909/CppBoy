#pragma once

#define BIT_0 1
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

#define VRAM_START 0x8000 // start address of VRAM
#define VRAM_LEN 0x1800 // length of VRAM
#define VRAM_END (VRAM_START + VRAM_LEN)
#define BLOCK_0_START 0x8000
#define BLOCK_1_START 0x8800
#define BLOCK_2_START 0x9000
#define TILE_MAP_LEN 0x400
#define TILE_MAP_1_START 0x9800
#define TILE_MAP_1_END (TILE_MAP_1_START + TILE_MAP_LEN)
#define TILE_MAP_2_START 0x9C00
#define TILE_MAP_2_END (TILE_MAP_2_START + TILE_MAP_LEN)

/*** Registers ***/

#define IE 0xFFFF

#define IF 0xFF0F
/*** Interrupt bits ***/
#define JOYPAD_INT BIT_4
#define SERIAL_INT BIT_3
#define TIMER_INT BIT_2
#define LCD_INT BIT_1
#define VBLANK_INT BIT_0

#define JOYPAD 0xFF00
/*** Joypad button encodings ***/
#define START ~0x28
#define SELECT ~0x24
#define B ~0x22
#define A ~0x21
#define DOWN ~0x18
#define UP ~0x14
#define LEFT ~0x12
#define RIGHT ~0x11

/*** LCD Control ***/
#define LCDC 0xFF40
#define OBJ_ENABLE BIT_1
#define BG_TILE_MAP BIT_3
#define TILE_MAP_ADDRESSING BIT_4
