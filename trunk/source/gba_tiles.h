#ifndef GBA_TILES_H
#define GBA_TILES_H

/* #define BG_MOSAIC_ENABLE BIT(6) */
#define BG_COLOR_256 0x80
#define BG_COLOR_16 0
#define CHAR_BASE_BLOCK(n) (((n)*0x4000)+0x6000000)
#define SCREEN_BASE_BLOCK(n) (((n)*0x800)+0x6000000)
#define CHAR_SHIFT 2
#define SCREEN_SHIFT 8
#define TEXTBG_SIZE_256x256 (0 << 14)
#define TEXTBG_SIZE_256x512 (1 << 14)
#define TEXTBG_SIZE_512x256 (2 << 14)
#define TEXTBG_SIZE_512x512 (3 << 14)
#define ROTBG_SIZE_128x128 (0 << 14)
#define ROTBG_SIZE_256x256 (1 << 14)
#define ROTBG_SIZE_512x512 (2 << 14)
#define ROTBG_SIZE_1024x1024 (3 << 14)
#define WRAPAROUND 0x1

#define REG_BG0CNT *(volatile unsigned short*)0x4000008
#define REG_BG1CNT *(volatile unsigned short*)0x400000A
#define REG_BG2CNT *(volatile unsigned short*)0x400000C
#define REG_BG3CNT *(volatile unsigned short*)0x400000E

#endif


