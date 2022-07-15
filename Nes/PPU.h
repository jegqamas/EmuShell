// This file is part of EmuShell
// A multi video game consoles emulator.
//
// Copyright(C) 2015 Ala Ibrahim Hadid
// E-mail: mailto:ahdsoftwares@hotmail.com
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
#define PALETTE_USAGE_AUTO 0
#define PALETTE_USAGE_NTSC 1
#define PALETTE_USAGE_PALB 2
#define PALETTE_USAGE_FILE 3

#define FPS_NTSC 60.0988
#define FPS_PALB 50.070
#define FPS_DENDY 50.070

#include <string>
#include <stdint.h>

namespace NES
{
class PPU2C02
{
public:
    ~PPU2C02();
    void Initialize(int system);
    void HardReset();// Hard reset the ppu
    void SoftReset();// Soft reset the ppu
    void Clock();// Clock ppu one PPU CYCLE
    void SaveState();
    void LoadState();
    void Shutdown();
    void Write2000(uint8_t& value);// Write at register $2000
    void Write2001(uint8_t& value);// Write at register $2001
    void Write2003(uint8_t& value);// Write at register $2003
    void Write2004(uint8_t& value);// Write at register $2004
    void Write2005(uint8_t& value);// Write at register $2005
    void Write2006(uint8_t& value);// Write at register $2006
    void Write2007(uint8_t& value);// Write at register $2007
    uint8_t Read2002();// Read at register $2002
    uint8_t Read2004();// Read at register $2004
    uint8_t Read2007();// Read at register $2007
    bool IsRenderingOn();// Indicates if the rendering is on
    bool IsInRender();// Indicates if the ppu is in render range
    bool IsSpriteSize16();// Get if the sprite size is 16 (true means it is 16)
    void SetupPaletteIndexes(uint32_t * indexes);
    bool FrameFinished;
    int PaletteUsageMode;
    std::string PaletteFile;
private:
    int tvSystem;
    bool initialized;// Set to true after hard reset.
    static const uint8_t reverseLookup[256];
    uint32_t* paletteIndexes;
    uint32_t* palette;
    uint32_t* bkg_pixels;
    uint32_t* spr_pixels;
    bool nmi_enabled;
    bool nmi_old;
    bool vbl_flag;
    bool vbl_flag_temp;
    uint8_t* oam_ram;
    uint8_t* oam_secondary;
    uint8_t* palettes_bank;
    static const uint8_t palettes_bank_initial[32];
    // Frame Timing
    uint32_t vbl_vclock_Start ;
    uint32_t vbl_vclock_End ;
    uint32_t frameEnd ;
    uint32_t VClock;
    uint32_t HClock;
    bool UseOddFrame ;
    bool oddSwap;
    uint32_t current_pixel;
    uint32_t temp;
    uint32_t temp_comparator;
    uint32_t bkg_pos;
    uint32_t spr_pos;
    uint32_t object0;
    uint32_t infront;
    uint32_t bkgPixel;
    uint32_t sprPixel;
    uint16_t bkg_fetch_address;
    uint8_t bkg_fetch_nametable;
    uint8_t bkg_fetch_attr;
    uint8_t bkg_fetch_bit0;
    uint8_t bkg_fetch_bit1;
    uint16_t spr_fetch_address;
    uint8_t spr_fetch_bit0;
    uint8_t spr_fetch_bit1;
    uint8_t spr_fetch_attr;
    bool* spr_zero_buffer;
    // VRAM Address register
    uint16_t vram_temp;
    uint16_t vram_address;
    uint16_t vram_address_temp_access;
    uint16_t vram_address_temp_access1;
    uint16_t vram_increament;
    bool vram_flipflop;
    uint8_t vram_fine;
    uint8_t reg2007buffer;
    // Background
    bool bkg_enabled;
    bool bkg_clipped;
    uint16_t bkg_patternAddress;
    // Sprites
    bool spr_enabled;
    bool spr_clipped;
    uint16_t spr_patternAddress;
    uint16_t spr_size16;
    bool spr_0Hit;
    bool spr_overflow;
    // Colors
    uint8_t grayscale;
    uint8_t emphasis;
    // Others
    uint8_t ppu_2002_temp;
    uint8_t ppu_2004_temp;
    uint8_t ppu_2007_temp;
    // OAM
    uint8_t oam_address;
    uint8_t oam_fetch_data;
    uint8_t oam_evaluate_slot;
    uint8_t oam_evaluate_count;
    bool oam_fetch_mode;
    uint8_t oam_phase_index;
    uint32_t spr_render_i;
    uint32_t bkg_render_i;
    uint32_t spr_evaluation_i;
    uint32_t spr_render_temp_pixel;

    inline void bkg_fetch_1();
    inline void bkg_fetch_2();
    inline void bkg_fetch_3();
    inline void bkg_fetch_4();
    inline void bkg_fetch_5();
    inline void bkg_fetch_6();
    inline void bkg_fetch_7();
    inline void bkg_fetch_8();

    inline void spr_fetch_1();
    inline void spr_fetch_2();
    inline void spr_fetch_3();
    inline void spr_fetch_4();
    inline void IncrementY();
    inline void IncrementX();
    inline void render_bkg_tile();
    inline void render_spr_tile();
    inline void render_pixel();

    inline void oam_eva_fetch();
    inline void oam_eva_0();
    inline void oam_eva_1();
    inline void oam_eva_2();
    inline void oam_eva_3();
    inline void oam_eva_4();
    inline void oam_eva_5();
    inline void oam_eva_6();
    inline void oam_eva_7();
    inline void oam_eva_8();
    inline void oam_eva_9();
    inline void oam_eva_begin();
    inline void oam_eva_reset();
};
}
