#pragma once

#include "../../aliases.hpp"

#include <array>

#define PPU_BEGIN   0x8000
#define PPU_END     0x9fff
#define VRAM_BEGIN  PPU_BEGIN
#define VRAM_END    PPU_END
#define CRAM_BEGIN  PPU_BEGIN
#define CRAM_END    0x97ff
#define TM0_BEGIN   0x9800
#define TM0_END     0x9bff
#define TM1_BEGIN   0x9c00
#define TM1_END     PPU_END
#define OAM_BEGIN   0xfe00
#define OAM_END     0xfe9f
#define PPU_R_BEGIN 0xff40
#define PPU_R_END   0xff4b

namespace gameboy {
    namespace ppu {
        typedef std::array <u8, 0x2000> vram_bank_t;
        typedef std::array <u8, 0xa0>   oam_t;
        typedef std::array <u8, 0x40>   cgb_palette_ram_t;
        typedef std::array <u8, 0xc>    ppu_registers_t;

        u8 cgb_bg_palette_idx = 0,
           cgb_spr_palette_idx = 0,
           dummy = 0;

        std::array <vram_bank_t, 2> vram;
        
        ppu_registers_t   r = { 0 };
        oam_t             oam  = { 0 };
        cgb_palette_ram_t cgb_bg_palette = { 0 };
        cgb_palette_ram_t cgb_spr_palette = { 0 };

        size_t current_bank_idx = 0;
        vram_bank_t* current_bank = &vram[current_bank_idx];

        bool bg_auto_inc = false,
             spr_auto_inc = false;

        int clk;
    }
}