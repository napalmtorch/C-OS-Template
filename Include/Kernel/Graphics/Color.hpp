#pragma once
#include <Kernel/Lib/Types.hpp>

extc typedef enum : byte
{
    COL4_BLACK              = 0x00,
    COL4_DARK_BLUE          = 0x01,
    COL4_DARK_GREEN         = 0x02,
    COL4_DARK_CYAN          = 0x03,
    COL4_DARK_RED           = 0x04,
    COL4_DARK_MAGENTA       = 0x05,
    COL4_DARK_YELLOW        = 0x06,
    COL4_GRAY               = 0x07,
    COL4_DARK_GRAY          = 0x08,
    COL4_BLUE               = 0x09,
    COL4_GREEN              = 0x0A,
    COL4_CYAN               = 0x0B,
    COL4_RED                = 0x0C,
    COL4_MAGENTA            = 0x0D,
    COL4_YELLOW             = 0x0E,
    COL4_WHITE              = 0x0F,
} COL4;

extc typedef struct
{
    byte A;
    byte R;
    byte G;
    byte B;
} attr_pack Color;

extc typedef enum
{
    COLORDEPTH_4  = 4,
    COLORDEPTH_8  = 8,
    COLORDEPTH_16 = 16,
    COLORDEPTH_24 = 24,
    COLORDEPTH_32 = 32,
} COLORDEPTH;

static const Color RGB_BLACK              = { 0xFF, 0x00, 0x00, 0x00 };
static const Color RGB_DARK_BLUE          = { 0xFF, 0x00, 0x00, 0x7F };
static const Color RGB_DARK_GREEN         = { 0xFF, 0x00, 0x7F, 0x00 };
static const Color RGB_DARK_CYAN          = { 0xFF, 0x00, 0x7F, 0x7F };
static const Color RGB_DARK_RED           = { 0xFF, 0x7F, 0x00, 0x00 };
static const Color RGB_DARK_MAGENTA       = { 0xFF, 0x7F, 0x00, 0x7F };
static const Color RGB_DARK_YELLOW        = { 0xFF, 0x7F, 0x7F, 0x00 };
static const Color RGB_DARK_GRAY          = { 0xFF, 0x5F, 0x5F, 0x5F };
static const Color RGB_GRAY               = { 0xFF, 0x60, 0x60, 0x60 };
static const Color RGB_WHITE              = { 0xFF, 0xFF, 0xFF, 0xFF };
static const Color RGB_RED                = { 0xFF, 0xFF, 0x00, 0x00 };
static const Color RGB_GREEN              = { 0xFF, 0x00, 0xFF, 0x00 };
static const Color RGB_BLUE               = { 0xFF, 0x00, 0x00, 0xFF };
static const Color RGB_MAGENTA            = { 0xFF, 0xFF, 0x00, 0xFF };
static const Color RGB_CYAN               = { 0xFF, 0x00, 0xFF, 0xFF };
static const Color RGB_YELLOW             = { 0xFF, 0xFF, 0xFF, 0x00 };
static const Color RGB_CORNFLOWER_BLUE    = { 0xFF, 0x93, 0xCC, 0xEA };

static inline uint RGBToPackedValue(Color color)
{
    return (uint)((color.A << 24) | (color.R << 16) | (color.G << 8) | color.B);
}

static inline bool IsColorEqual(Color a, Color b)
{
    if (a.A != b.A) { return false; }
    if (a.R != b.R) { return false; }
    if (a.G != b.G) { return false; }
    if (a.B != b.B) { return false; }
    return true;
}

static inline bool IsColorEqual(uint a, uint b) { return a == b; }

static inline Color COL4ToRGB(COL4 c)
{
    switch (c)
    {
        case COL4_BLACK:        { return RGB_BLACK; }
        case COL4_DARK_BLUE:    { return RGB_DARK_BLUE; }
        case COL4_DARK_GREEN:   { return RGB_DARK_GREEN; }
        case COL4_DARK_CYAN:    { return RGB_DARK_CYAN; }
        case COL4_DARK_RED:     { return RGB_DARK_RED; }
        case COL4_DARK_MAGENTA: { return RGB_DARK_MAGENTA; }
        case COL4_DARK_YELLOW:  { return RGB_DARK_YELLOW; }
        case COL4_GRAY:         { return RGB_GRAY; }
        case COL4_DARK_GRAY:    { return RGB_DARK_GRAY; }
        case COL4_BLUE:         { return RGB_CORNFLOWER_BLUE; }
        case COL4_GREEN:        { return RGB_GREEN; }
        case COL4_CYAN:         { return RGB_CYAN; }
        case COL4_RED:          { return RGB_RED; }
        case COL4_MAGENTA:      { return RGB_MAGENTA; }
        case COL4_YELLOW:       { return RGB_YELLOW; }
        case COL4_WHITE:        { return RGB_WHITE; }
    }
    return RGB_BLACK;
}