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
#include <iostream>
#include "Nes.h"
#include "../Providers/VideoOut.h"
#include "../Definations.h"
#include "Palettes/NTSCPaletteGenerator.h"
#include "Palettes/PALBPaletteGenerator.h"
#include "Palettes/PaletteFile.h"

namespace NES
{
const uint8_t PPU2C02::reverseLookup[256] =
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

const uint8_t PPU2C02::palettes_bank_initial[32] =   // Miscellaneous, real NES loads values similar to these during power up
{
    0x09, 0x01, 0x00, 0x01, 0x00, 0x02, 0x02, 0x0D, 0x08, 0x10, 0x08, 0x24, 0x00, 0x00, 0x04, 0x2C, // Bkg palette
    0x09, 0x01, 0x34, 0x03, 0x00, 0x04, 0x00, 0x14, 0x08, 0x3A, 0x00, 0x02, 0x00, 0x20, 0x2C, 0x08  // Spr palette
};

/*Initializing stuff*/
void PPU2C02::Initialize(int system)
{
    tvSystem = system;
    palette=new uint32_t [512];
    // TODO: palette indexes setup
    paletteIndexes = new uint32_t[512];
    for (int i = 0; i < 512; i++)
        paletteIndexes[i] = i;

    bkg_pixels= new uint32_t [272];
    spr_pixels= new uint32_t [256];
    oam_ram= new uint8_t [256];
    oam_secondary= new uint8_t [32];
    palettes_bank= new uint8_t [32];
    spr_zero_buffer= new bool [8];

    NTSCPaletteGenerator::Initialize();
    PALBPaletteGenerator::Initialize();
}
void PPU2C02::SetupPaletteIndexes(uint32_t * indexes)
{
    for (int i = 0; i < 512; i++)
        paletteIndexes[i] = indexes[i];
}
void PPU2C02::Shutdown()
{
    // Save palettes
    NTSCPaletteGenerator::SaveSettings();
    PALBPaletteGenerator::SaveSettings();
}
PPU2C02::~PPU2C02()
{
    // Free memory ..
    delete [] paletteIndexes;
    delete [] palette;
    delete [] bkg_pixels;
    delete [] spr_pixels;
    delete [] oam_ram;
    delete [] oam_secondary;
    delete [] palettes_bank;
    delete [] spr_zero_buffer;
}
void PPU2C02::HardReset()
{
    // TV format setup
    switch (tvSystem)
    {
    case TVSYSTEM_NTSC:
    {
        vbl_vclock_Start = 241;//20 scanlines for VBL
        vbl_vclock_End = 261;
        frameEnd = 262;
        UseOddFrame = true;
        // Palette
        if (PaletteUsageMode == PALETTE_USAGE_AUTO )
            NTSCPaletteGenerator::GeneratePalette512(palette);

        VideoOut::SetupFrameLimitter(FPS_NTSC);
        VideoOut::SetRenderRectangle(0,8,256,224);// Cut 8 lines from top and bottom.
        break;
    }
    case TVSYSTEM_PALB:
    {
        vbl_vclock_Start = 241;//70 scanlines for VBL
        vbl_vclock_End = 311;
        frameEnd = 312;
        UseOddFrame = false;
        if (PaletteUsageMode == PALETTE_USAGE_AUTO)
            PALBPaletteGenerator::GeneratePalette512(palette);
        VideoOut::SetupFrameLimitter(FPS_PALB);
        VideoOut::SetRenderRectangle(0,1,256,238);
        break;
    }
    case TVSYSTEM_DENDY:
    {
        vbl_vclock_Start = 291;//51 dummy scanlines, 20 VBL's
        vbl_vclock_End = 311;
        frameEnd = 312;
        UseOddFrame = false;
        if (PaletteUsageMode == PALETTE_USAGE_AUTO)
            PALBPaletteGenerator::GeneratePalette512(palette);
        VideoOut::SetupFrameLimitter(FPS_DENDY);
        VideoOut::SetRenderRectangle(0,1,256,238);
        break;
    }
    }

    // Palette customization
    switch(PaletteUsageMode)
    {
    case PALETTE_USAGE_NTSC:
    {
        NTSCPaletteGenerator::GeneratePalette512(palette);
        std::cout << "NES: palette forced to use NTSC palette generator.\n";
        break;
    }
    case PALETTE_USAGE_PALB:
    {
        PALBPaletteGenerator::GeneratePalette512(palette);
        std::cout << "NES: palette forced to use PALB palette generator.\n";
        break;
    }
    case PALETTE_USAGE_FILE:
    {
        // Load file
        std::cout << "NES: reading palette file at: " << this->PaletteFile<<"\n";
        if (palette==NULL)
            palette=new uint32_t [512];
        if (!PaletteFile::LoadPaletteFile(this->PaletteFile, palette))
        {
            std::cout << "NES: palette file read failed. Switching to AUTO palette usage mode.\n";
            // File is not loaded, use auto style
            switch (tvSystem)
            {
            case TVSYSTEM_NTSC:
            {
                NTSCPaletteGenerator::GeneratePalette512(palette);
                break;
            }
            case TVSYSTEM_PALB:
            case TVSYSTEM_DENDY:
            {
                PALBPaletteGenerator::GeneratePalette512(palette);
                break;
            }
            }
        }
        else
        {
            std::cout << "NES: palette file read successes.\n";
        }
        break;
    }
    }
    nmi_enabled = false;
    nmi_old = false;
    vbl_flag = false;
    vbl_flag_temp = false;

    for (int i=0; i<8; i++)
        spr_zero_buffer[i]=0;

    for (int i=0; i<272; i++)
        bkg_pixels[i]=0;

    for (int i=0; i<256; i++)
        spr_pixels[i]=0;

    grayscale = 0xF3;
    vram_flipflop = false;
    emphasis = 0;
    HClock = 0;
    VClock = 0;
    oddSwap = false;
    // Reset evaluation
    oam_fetch_mode = false;
    oam_phase_index = 0;
    oam_evaluate_slot = 0;
    oam_address = 0;
    oam_evaluate_count = 0;
    oam_fetch_data = 0;

    for (int i = 0; i < 256; i++)
        spr_pixels[i] = 0;

    for (int i=0; i<256; i++)
        oam_ram[i]=0;

    for (int i=0; i<32; i++)
        oam_secondary[i]=0;
    for (int i = 0; i < 32; i++)
    {
        palettes_bank[i] = palettes_bank_initial[i];
    }

    current_pixel = 0;
    temp = 0;
    temp_comparator = 0;
    bkg_pos = 0;
    spr_pos = 0;
    object0 = 0;
    infront = 0;
    bkgPixel = 0;
    sprPixel = 0;
    bkg_fetch_address = 0;
    bkg_fetch_nametable = 0;
    bkg_fetch_attr = 0;
    bkg_fetch_bit0 = 0;
    bkg_fetch_bit1 = 0;
    spr_fetch_address = 0;
    spr_fetch_bit0 = 0;
    spr_fetch_bit1 = 0;
    spr_fetch_attr = 0;
    // VRAM Address register
    vram_temp = 0;
    vram_address = 0;
    vram_address_temp_access = 0;
    vram_address_temp_access1 = 0;
    vram_increament = 1;
    vram_flipflop = false;
    vram_fine = 0;
    reg2007buffer = 0;
    // Background
    bkg_enabled = false;
    bkg_clipped = false;
    bkg_patternAddress = 0;
    // Sprites
    spr_enabled = false;
    spr_clipped = false;
    spr_patternAddress = 0;
    spr_size16 = 0;
    spr_0Hit = false;
    spr_overflow = false;
    // Others
    ppu_2002_temp = 0;
    ppu_2004_temp = 0;
    ppu_2007_temp = 0;
    // OAM
    spr_render_i = 0;
    bkg_render_i = 0;
    spr_evaluation_i = 0;
    spr_render_temp_pixel = 0;

// IDK why but the nmi pin will never clear up
// until this line is presented here.
// This problem never occur in VC compiler (visual Studio)
    dev_cpu->pin_nmi = false;
    initialized = true;
}
void PPU2C02::SoftReset()
{

}
/*MEMORY ACCESS !*/
void PPU2C02::Write2000(uint8_t& value)
{
    vram_temp = (vram_temp & 0x73FF) | ((value & 0x3) << 10);
    vram_increament = ((value & 0x4) != 0) ? 32 : 1;
    spr_patternAddress = ((value & 0x8) != 0) ? 0x1000 : 0x0000;
    bkg_patternAddress = ((value & 0x10) != 0) ? 0x1000 : 0x0000;
    spr_size16 = (value & 0x20) != 0 ? 0x0010 : 0x0008;

    nmi_old = nmi_enabled;
    nmi_enabled = (value & 0x80) != 0;

    if (!nmi_enabled)// NMI disable effect only at vbl set period (HClock between 1 and 3)
    {
        if ((VClock == vbl_vclock_Start) && (HClock <= 3))
            NES::dev_cpu->pin_nmi = vbl_flag_temp & nmi_enabled;
    }
    else if (vbl_flag_temp & (!nmi_old))// Special case ! NMI can be enabled anytime if vbl already set
    {
        NES::dev_cpu->pin_nmi = true;
    }
}
void PPU2C02::Write2001(uint8_t& value)
{
    grayscale = (value & 0x01) != 0 ? 0x30 : 0x3F;
    emphasis = (value & 0xE0) << 1;

    bkg_clipped = (value & 0x02) == 0;
    spr_clipped = (value & 0x04) == 0;
    bkg_enabled = (value & 0x08) != 0;
    spr_enabled = (value & 0x10) != 0;
}
void PPU2C02::Write2003(uint8_t& value)
{
    oam_address = value;
}
void PPU2C02::Write2004(uint8_t& value)
{
    if (VClock < 240 && IsRenderingOn())
        value = 0xFF;
    if ((oam_address & 0x03) == 0x02)
        value &= 0xE3;
    oam_ram[oam_address++] = value;
}
void PPU2C02::Write2005(uint8_t& value)
{
    if (!vram_flipflop)
    {
        vram_temp = (vram_temp & 0x7FE0) | ((value & 0xF8) >> 3);
        vram_fine = (value & 0x07);
    }
    else
    {
        vram_temp = (vram_temp & 0x0C1F) | ((value & 0x7) << 12) | ((value & 0xF8) << 2);
    }
    vram_flipflop = !vram_flipflop;
}
void PPU2C02::Write2006(uint8_t& value)
{
    if (!vram_flipflop)
    {
        vram_temp = (vram_temp & 0x00FF) | ((value & 0x3F) << 8);
    }
    else
    {
        vram_temp = (vram_temp & 0x7F00) | value;
        vram_address = vram_temp;
        dev_board->OnPPUAddressUpdate(vram_address);
    }
    vram_flipflop = !vram_flipflop;
}
void PPU2C02::Write2007(uint8_t& value)
{
    vram_address_temp_access = vram_address & 0x3FFF;
    if (vram_address_temp_access < 0x2000)
    {
        NES::dev_board->WriteCHR(vram_address_temp_access, value);
    }
    else if (vram_address_temp_access < 0x3F00)
    {
        NES::dev_board->WriteNMT(vram_address_temp_access, value);
    }
    else
    {
        palettes_bank[vram_address_temp_access & ((vram_address_temp_access & 0x03) == 0 ? 0x0C : 0x1F)] = value;
    }
    vram_address = (vram_address + vram_increament) & 0x7FFF;
    dev_board->OnPPUAddressUpdate( vram_address);
}

uint8_t PPU2C02::Read2002()
{
    ppu_2002_temp = 0;

    if (vbl_flag)
        ppu_2002_temp |= 0x80;
    if (spr_0Hit)
        ppu_2002_temp |= 0x40;
    if (spr_overflow)
        ppu_2002_temp |= 0x20;

    vbl_flag_temp = false;
    vram_flipflop = false;

    // NMI disable effect only at vbl set period (HClock between 1 and 3)
    if ((VClock == vbl_vclock_Start) && (HClock <= 3))
        NES::dev_cpu->pin_nmi = false;

    return ppu_2002_temp;
}
uint8_t PPU2C02::Read2004()
{
    ppu_2004_temp = oam_ram[oam_address];
    if (VClock < 240 && IsRenderingOn())
    {
        if (HClock < 64)
            ppu_2004_temp = 0xFF;
        else if (HClock < 192)
            ppu_2004_temp = oam_ram[((HClock - 64) << 1) & 0xFC];
        else if (HClock < 256)
            ppu_2004_temp = ((HClock & 1) == 1) ? oam_ram[0xFC] : oam_ram[((HClock - 192) << 1) & 0xFC];
        else if (HClock < 320)
            ppu_2004_temp = 0xFF;
        else
            ppu_2004_temp = oam_ram[0];
    }
    return ppu_2004_temp;
}
uint8_t PPU2C02::Read2007()
{
    ppu_2007_temp = 0;

    if ((vram_address & 0x3F00) == 0x3F00)
    {
        // The return value should be from the palettes bank
        ppu_2007_temp = (uint8_t)(palettes_bank[vram_address & ((vram_address & 0x03) == 0 ? 0x0C : 0x1F)] & grayscale);
        // fill buffer from chr or nametables
        vram_address_temp_access1 = vram_address & 0x2FFF;
        if (vram_address_temp_access1 < 0x2000)
        {
            reg2007buffer = NES::dev_board->ReadCHR(vram_address_temp_access1, false);
        }
        else
        {
            reg2007buffer = NES::dev_board->ReadNMT(vram_address_temp_access1);
        }
    }
    else
    {
        ppu_2007_temp = reg2007buffer;
        // fill buffer
        vram_address_temp_access1 = vram_address & 0x3FFF;
        if (vram_address_temp_access1 < 0x2000)
        {
            reg2007buffer = NES::dev_board->ReadCHR(vram_address_temp_access1, false);
        }
        else if (vram_address_temp_access1 < 0x3F00)
        {
            reg2007buffer = NES::dev_board->ReadNMT(vram_address_temp_access1);
        }
        else
        {
            reg2007buffer = palettes_bank[vram_address_temp_access1 & ((vram_address_temp_access1 & 0x03) == 0 ? 0x0C : 0x1F)];
        }
    }

    vram_address = (vram_address + vram_increament) & 0x7FFF;
    dev_board->OnPPUAddressUpdate( vram_address);
    return ppu_2007_temp;
}

/*Rendering stuff*/
bool PPU2C02::IsRenderingOn()
{
    return (bkg_enabled || spr_enabled);
}
bool PPU2C02::IsInRender()
{
    return (VClock < 240) || (VClock == vbl_vclock_End);
}

void PPU2C02::bkg_fetch_1()
{
    // Fetch address of nametable
    bkg_fetch_address = 0x2000 | (vram_address & 0x0FFF);
}
void PPU2C02::bkg_fetch_2()
{
    bkg_fetch_nametable = dev_board->ReadNMT(bkg_fetch_address);
}
void PPU2C02::bkg_fetch_3()
{
    bkg_fetch_address = 0x23C0 | (vram_address & 0xC00) | (vram_address >> 4 & 0x38) | (vram_address >> 2 & 0x7);
}
void PPU2C02::bkg_fetch_4()
{
    bkg_fetch_attr = dev_board->ReadNMT(bkg_fetch_address) >> ((vram_address >> 4 & 0x04) | (vram_address & 0x02));
}
void PPU2C02::bkg_fetch_5()
{
    bkg_fetch_address = bkg_patternAddress | (bkg_fetch_nametable << 4) | 0 | (vram_address >> 12 & 7);
}
void PPU2C02::bkg_fetch_6()
{
    bkg_fetch_bit0 = dev_board->ReadCHR(bkg_fetch_address, false);
}
void PPU2C02::bkg_fetch_7()
{
    bkg_fetch_address = bkg_patternAddress | (bkg_fetch_nametable << 4) | 8 | (vram_address >> 12 & 7);
}
void PPU2C02::bkg_fetch_8()
{
    bkg_fetch_bit1 = dev_board->ReadCHR(bkg_fetch_address, false);
}

void PPU2C02::spr_fetch_1()
{
    temp = HClock >> 3 & 7;
    temp_comparator = (VClock - oam_secondary[temp * 4]) ^ ((oam_secondary[(temp * 4) + 2] & 0x80) != 0 ? 0x0F : 0x00);
    if (spr_size16 == 0x10)
    {
        spr_fetch_address = (oam_secondary[(temp * 4) + 1] << 0x0C & 0x1000) | (oam_secondary[(temp * 4) + 1] << 0x04 & 0x0FE0) |
                            (temp_comparator << 0x01 & 0x0010) | (temp_comparator & 0x0007);
    }
    else
    {
        spr_fetch_address = spr_patternAddress | (oam_secondary[(temp * 4) + 1] << 0x04) | (temp_comparator & 0x0007);
    }
    dev_board->OnPPUAddressUpdate( vram_address);
}
void PPU2C02::spr_fetch_2()
{
    spr_fetch_bit0 = dev_board->ReadCHR(spr_fetch_address, true);
    if ((oam_secondary[((HClock >> 3 & 7) * 4) + 2] & 0x40) != 0)
        spr_fetch_bit0 = reverseLookup[spr_fetch_bit0];
}
void PPU2C02::spr_fetch_3()
{
    spr_fetch_address = spr_fetch_address | 0x08;
}
void PPU2C02::spr_fetch_4()
{
    spr_fetch_bit1 = dev_board->ReadCHR(spr_fetch_address, true);
    if ((oam_secondary[((HClock >> 3 & 7) * 4) + 2] & 0x40) != 0)
        spr_fetch_bit1 = reverseLookup[spr_fetch_bit1];

    spr_fetch_attr = oam_secondary[((HClock >> 3 & 7) * 4) + 2];

}
void PPU2C02::IncrementY()
{
    if ((vram_address & 0x7000) != 0x7000)
    {
        vram_address += 0x1000;
    }
    else
    {
        vram_address ^= 0x7000;

        switch (vram_address & 0x3E0)
        {
        case 0x3A0:
            vram_address ^= 0xBA0;
            break;
        case 0x3E0:
            vram_address ^= 0x3E0;
            break;
        default:
            vram_address += 0x20;
            break;
        }
    }
}
void PPU2C02::IncrementX()
{
    // Increment X
    if ((vram_address & 0x001F) == 0x001F)
        vram_address ^= 0x041F;
    else
        vram_address++;
}
void PPU2C02::render_bkg_tile()
{
    // Render BKG tile
    bkg_pos = (HClock + 9) % 336;

    for (bkg_render_i = 0; bkg_render_i < 8 && bkg_pos < 272; bkg_render_i++, bkg_pos++, bkg_fetch_bit0 <<= 1, bkg_fetch_bit1 <<= 1)
        bkg_pixels[bkg_pos] = (bkg_fetch_attr << 2 & 12) | (bkg_fetch_bit0 >> 7 & 1) | (bkg_fetch_bit1 >> 6 & 2);
}
void PPU2C02::render_spr_tile()
{
    // Render SPR tile
    temp = HClock >> 3 & 7;

    if (oam_secondary[(temp * 4) + 3] == 255)
        return;

    spr_pos = oam_secondary[(temp * 4) + 3];
    object0 = spr_zero_buffer[temp] ? 0x4000 : 0x0000;
    infront = ((oam_secondary[(temp * 4) + 2] & 0x20) == 0) ? 0x8000 : 0x0000;
    for (spr_render_i = 0; spr_render_i < 8 && spr_pos < 256; spr_render_i++, spr_pos++, spr_fetch_bit0 <<= 1, spr_fetch_bit1 <<= 1)
    {
        if (spr_pos > 255)
            break;

        spr_render_temp_pixel = (spr_fetch_attr << 2 & 12) | (spr_fetch_bit0 >> 7 & 1) | (spr_fetch_bit1 >> 6 & 2) | object0 | infront;
        if ((spr_pixels[spr_pos] & 0x03) == 0 && (spr_render_temp_pixel & 0x03) != 0)
            spr_pixels[spr_pos] = spr_render_temp_pixel;
    }
}
void PPU2C02::render_pixel()
{
    if (!bkg_enabled || (bkg_clipped && HClock < 8))
        bkgPixel = 0x3F00;
    else
        bkgPixel = 0x3F00 | bkg_pixels[HClock + vram_fine];
    if (!spr_enabled || (spr_clipped && HClock < 8))
        sprPixel = 0x3F10;
    else
        sprPixel = 0x3F10 | spr_pixels[HClock];

    current_pixel = 0;

    //Priority *******************************
    if ((sprPixel & 0x8000) != 0)
        current_pixel = sprPixel;
    else
        current_pixel = bkgPixel;
    //****************************************

    // Transparency **************************
    if ((bkgPixel & 0x03) == 0)
    {
        current_pixel = sprPixel;
        goto render;
    }

    if ((sprPixel & 0x03) == 0)
    {
        current_pixel = bkgPixel;
        goto render;
    }
    //****************************************

    //Sprite 0 Hit
    if (((sprPixel & 0x4000) != 0) && (HClock < 255) && (bkg_enabled && spr_enabled))
        spr_0Hit = true;

render:
    VideoOut::buffer_pixels[(VClock * 256) + HClock] = palette[paletteIndexes[palettes_bank[current_pixel & ((current_pixel & 0x03) == 0 ? 0x0C : 0x1F)]
            & (grayscale | emphasis)]];
}

void PPU2C02::oam_eva_fetch()
{
    if (!oam_fetch_mode)
        oam_fetch_data = 0xFF;
    else
        oam_fetch_data = oam_ram[oam_address];
}
void PPU2C02::oam_eva_0()
{
    if (HClock <= 64)
    {
        switch (HClock >> 1 & 0x03)
        {
        case 0:
            oam_secondary[((HClock >> 3) * 4) + 0] = 0xFF;
            break;
        case 1:
            oam_secondary[((HClock >> 3) * 4) + 1] = 0xFF;
            break;
        case 2:
            oam_secondary[((HClock >> 3) * 4) + 2] = 0xFF;
            break;
        case 3:
        {
            oam_secondary[((HClock >> 3) * 4) + 3] = 0xFF;
            spr_zero_buffer[HClock >> 3 & 7] = false;
            break;
        }
        }
    }
}
void PPU2C02::oam_eva_1()
{
    if (VClock == vbl_vclock_End)
        return;
    oam_evaluate_count++;
    temp_comparator = (VClock - oam_fetch_data) & 2147483647;

    if (temp_comparator >= spr_size16)
    {
        if (oam_evaluate_count != 64)
        {
            oam_address = (oam_evaluate_count != 2 ? oam_address + 4 : 8);
        }
        else
        {
            oam_address = 0;
            oam_phase_index = 9;
        }
    }
    else
    {
        oam_address++;
        oam_phase_index = 2;
        oam_secondary[oam_evaluate_slot * 4] = oam_fetch_data;

        spr_zero_buffer[oam_evaluate_slot] = (oam_evaluate_count == 1);
    }
}
void PPU2C02::oam_eva_2()
{
    oam_address++;
    oam_phase_index = 3;
    oam_secondary[(oam_evaluate_slot * 4) + 1] = oam_fetch_data;
}
void PPU2C02::oam_eva_3()
{
    oam_address++;
    oam_phase_index = 4;
    oam_secondary[(oam_evaluate_slot * 4) + 2] = oam_fetch_data;
}
void PPU2C02::oam_eva_4()
{
    oam_secondary[(oam_evaluate_slot * 4) + 3] = oam_fetch_data;
    oam_evaluate_slot++;

    if (oam_evaluate_count != 64)
    {
        oam_phase_index = (uint8_t)((oam_evaluate_slot != 8) ? 1 : 5);
        if (oam_evaluate_count != 2)
        {
            oam_address++;
        }
        else
        {
            oam_address = 8;
        }
    }
    else
    {
        oam_address = 0;
        oam_phase_index = 9;
    }
}
void PPU2C02::oam_eva_5()
{
    if (VClock == vbl_vclock_End)
        return;
    temp_comparator = (VClock - oam_fetch_data) & 2147483647;

    if (temp_comparator >= spr_size16)
    {
        oam_address = ((oam_address + 4) & 0xFC) +
                      ((oam_address + 1) & 0x03);

        if (oam_address <= 5)
        {
            oam_phase_index = 9;
            oam_address &= 0xFC;
        }
    }
    else
    {
        oam_phase_index = 6;
        oam_address += 0x01 & 0xFF;
        spr_overflow = true;
    }
}
void PPU2C02::oam_eva_6()
{
    oam_phase_index = 7;
    oam_address ++;
}
void PPU2C02::oam_eva_7()
{
    oam_phase_index = 8;
    oam_address ++;
}
void PPU2C02::oam_eva_8()
{
    oam_phase_index = 9;
    oam_address += (0x01);

    if ((oam_address & 0x03) == 0x03)
        oam_address ++;

    oam_address &= 0xFC;
}
void PPU2C02::oam_eva_9()
{
    oam_address += 0x4;
}
void PPU2C02::oam_eva_begin()
{
    //Evaluation Begin
    oam_fetch_mode = true;
    oam_phase_index = 1;
    oam_evaluate_slot = 0;
    oam_evaluate_count = 0;
}
void PPU2C02::oam_eva_reset()
{
    // Evaluation Reset
    oam_fetch_mode = false;
    oam_phase_index = 0;
    oam_evaluate_slot = 0;
    oam_address = 0;
    oam_evaluate_count = 0;

    //spr_pixels = new int[256];
    for (spr_evaluation_i = 0; spr_evaluation_i < 256; spr_evaluation_i++)
        spr_pixels[spr_evaluation_i] = 0;
    //memset(spr_pixels, 0, 256);
}
bool PPU2C02::IsSpriteSize16()
{
    return (spr_size16 == 0x10);
}

void PPU2C02::Clock()
{
    if (!initialized)
        return;
    dev_board->OnPPUClock();
    if ((VClock < 240) || (VClock == vbl_vclock_End))
    {
        if (bkg_enabled || spr_enabled)
        {
            if (HClock < 256)
            {
                // BKG FETCHES 0 - 255
                // UNUSED AT 248-255
                switch (HClock & 7)
                {
                case 0:
                    bkg_fetch_1();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 1:
                    bkg_fetch_2();
                    break;
                case 2:
                    bkg_fetch_3();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 3:
                    bkg_fetch_4();
                    break;
                case 4:
                    bkg_fetch_5();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 5:
                    bkg_fetch_6();
                    break;
                case 6:
                    bkg_fetch_7();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 7:
                {
                    bkg_fetch_8();
                    if (HClock == 255)
                        IncrementY();
                    else
                        IncrementX();

                    render_bkg_tile();

                    break;
                }
                }
                // Render Pixel
                if (VClock < 240)
                {
                    render_pixel();
                }
                // OAM EVALUATION
                switch (HClock & 1)
                {
                case 0:
                    oam_eva_fetch();
                    break;
                case 1:
                {
                    switch (oam_phase_index)
                    {
                    case 0:
                        oam_eva_0();
                        break;
                    case 1:
                        oam_eva_1();
                        break;
                    case 2:
                        oam_eva_2();
                        break;
                    case 3:
                        oam_eva_3();
                        break;
                    case 4:
                        oam_eva_4();
                        break;
                    case 5:
                        oam_eva_5();
                        break;
                    case 6:
                        oam_eva_6();
                        break;
                    case 7:
                        oam_eva_7();
                        break;
                    case 8:
                        oam_eva_8();
                        break;
                    case 9:
                        oam_eva_9();
                        break;
                    }
                    break;
                }
                }
                if (HClock == 63)
                    oam_eva_begin();

                if (HClock == 255)
                    oam_eva_reset();
            }
            else if (HClock < 320)
            {
                // SPRITE FETCHES + GARBAGE BKG FETCHES 256 - 319
                switch (HClock & 7)
                {
                case 0:
                    bkg_fetch_1();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 1:
                    bkg_fetch_2();
                    break;
                case 2:
                    bkg_fetch_3();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 3:
                    bkg_fetch_4();
                    break;
                case 4:
                    spr_fetch_1();
                    dev_board->OnPPUAddressUpdate(spr_fetch_address);
                    break;
                case 5:
                    spr_fetch_2();
                    break;
                case 6:
                    spr_fetch_3();
                    dev_board->OnPPUAddressUpdate(spr_fetch_address);
                    break;
                case 7:
                    spr_fetch_4();
                    render_spr_tile();
                    break;
                }
                if (HClock == 256)// 257 in the Ntsc_timing diagram
                    vram_address = (vram_address & 0x7BE0) | (vram_temp & 0x041F);
                // 280-304 in the Ntsc_timing diagram
                if (VClock == vbl_vclock_End && HClock >= 279 && HClock <= 303)
                    vram_address = (vram_address & 0x041F) | (vram_temp & 0x7BE0);
            }
            else if (HClock < 336)
            {
                // FIRST 2 BKG TILES FOR 1ST SCANLINE 320 - 335
                switch (HClock & 7)
                {
                case 0:
                    bkg_fetch_1();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 1:
                    bkg_fetch_2();
                    break;
                case 2:
                    bkg_fetch_3();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 3:
                    bkg_fetch_4();
                    break;
                case 4:
                    bkg_fetch_5();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 5:
                    bkg_fetch_6();
                    break;
                case 6:
                    bkg_fetch_7();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 7:
                {
                    bkg_fetch_8();

                    IncrementX();

                    render_bkg_tile();
                    break;
                }
                }
            }
            else if (HClock < 340)
            {
                // DUMMY FETCHES 336 - 339
                switch (HClock & 7)
                {
                case 0:
                    bkg_fetch_1();
                    dev_board->OnPPUAddressUpdate(bkg_fetch_address);
                    break;
                case 1:
                    bkg_fetch_2();
                    break;
                }
            }
            else
            {
                // Idle cycle
            }
        }
        else
        {
            // Rendering is off, draw color at vram address if it in range 0x3F00 - 0x3FFF
            if (HClock < 255 && VClock < 240)
            {
                if ((vram_address & 0x3F00) == 0x3F00)
                    VideoOut::buffer_pixels[(VClock * 256) + HClock] = palette[paletteIndexes[palettes_bank[vram_address & ((vram_address & 0x03) == 0 ? 0x0C : 0x1F)]
                            & (grayscale | emphasis)]];
                else
                    VideoOut::buffer_pixels[(VClock * 256) + HClock] = palette[paletteIndexes[palettes_bank[0] & (grayscale | emphasis)]];
            }
        }
    }

    // Clock Horz
    HClock++;
    // Update vbl flag from latch
    vbl_flag = vbl_flag_temp;

    // Check for nmi
    if ((VClock == vbl_vclock_Start) && (HClock <= 3))
        NES::dev_cpu->pin_nmi = (vbl_flag_temp & nmi_enabled);

    // odd frame in the idle cycle
    if (UseOddFrame)
    {
        if (HClock == 338 && VClock == vbl_vclock_End)
        {
            oddSwap = !oddSwap;
            if (!oddSwap & bkg_enabled)
            {
                HClock++;
            }
        }
    }
    // VBLANK, NMI and frame end
    if (HClock == 341)
    {
        NES::dev_board->OnPPUScanlineTick();
        HClock = 0;
        VClock++;
        //set vbl
        if (VClock == vbl_vclock_Start)
        {
            // VideoOut::LockPixels = false;// Apply the pixels to screen !
            vbl_flag_temp = true;
        }
        //clear vbl
        else if (VClock == vbl_vclock_End)
        {
            spr_0Hit = false;
            vbl_flag_temp = false;
            spr_overflow = false;

            // VideoOut::LockPixels = true;// Lock pixels so that we can render again
        }
        else if (VClock == frameEnd)
        {
            VClock = 0;
            FrameFinished = true;
            // Finished the frame !
            NES::OnFinishFrame();
        }
    }
}

void PPU2C02::SaveState()
{
    state_str->Write(nmi_enabled);
    state_str->Write(nmi_old);
    state_str->Write(vbl_flag);
    state_str->Write(vbl_flag_temp);
    state_str->Write(oam_ram,256);
    state_str->Write(oam_secondary,32);
    state_str->Write(palettes_bank,32);
    state_str->Write(oddSwap);

    state_str->Write(temp);
    state_str->Write(temp_comparator);
    state_str->Write(bkg_pos);
    state_str->Write(spr_pos);
    state_str->Write(object0);
    state_str->Write(infront);
    state_str->Write(bkgPixel);
    state_str->Write(sprPixel);
    state_str->Write(bkg_fetch_address);
    state_str->Write(bkg_fetch_nametable);
    state_str->Write(bkg_fetch_attr);
    state_str->Write(bkg_fetch_bit0);
    state_str->Write(bkg_fetch_bit1);
    state_str->Write(spr_fetch_address);
    state_str->Write(spr_fetch_bit0);
    state_str->Write(spr_fetch_bit1);
    state_str->Write(spr_fetch_attr);
    state_str->Write(spr_zero_buffer,8);
    // VRAM Address register
    state_str->Write(vram_temp);
    state_str->Write(vram_address);
    state_str->Write(vram_address_temp_access);
    state_str->Write(vram_address_temp_access1);
    state_str->Write(vram_increament);
    state_str->Write(vram_flipflop);
    state_str->Write(vram_fine);
    state_str->Write(reg2007buffer);
    // Background
    state_str->Write(bkg_enabled);
    state_str->Write(bkg_clipped);
    state_str->Write(bkg_patternAddress);
    // Sprites
    state_str->Write(spr_enabled);
    state_str->Write(spr_clipped);
    state_str->Write(spr_patternAddress);
    state_str->Write(spr_size16);
    state_str->Write(spr_0Hit);
    state_str->Write(spr_overflow);
    // Colors
    state_str->Write(grayscale);
    state_str->Write(emphasis);
    // Others
    state_str->Write(ppu_2002_temp);
    state_str->Write(ppu_2004_temp);
    state_str->Write(ppu_2007_temp);
    // OAM
    state_str->Write(oam_address);
    state_str->Write(oam_fetch_data);
    state_str->Write(oam_evaluate_slot);
    state_str->Write(oam_evaluate_count);
    state_str->Write(oam_fetch_mode);
    state_str->Write(oam_phase_index);
    state_str->Write(spr_render_i);
    state_str->Write(bkg_render_i);
    state_str->Write(spr_evaluation_i);
    state_str->Write(spr_render_temp_pixel);
}
void PPU2C02::LoadState()
{
    state_str->Read(nmi_enabled);
    state_str->Read(nmi_old);
    state_str->Read(vbl_flag);
    state_str->Read(vbl_flag_temp);
    state_str->Read(oam_ram,256);
    state_str->Read(oam_secondary,32);
    state_str->Read(palettes_bank,32);
    state_str->Read(oddSwap);
    state_str->Read(temp);
    state_str->Read(temp_comparator);
    state_str->Read(bkg_pos);
    state_str->Read(spr_pos);
    state_str->Read(object0);
    state_str->Read(infront);
    state_str->Read(bkgPixel);
    state_str->Read(sprPixel);
    state_str->Read(bkg_fetch_address);
    state_str->Read(bkg_fetch_nametable);
    state_str->Read(bkg_fetch_attr);
    state_str->Read(bkg_fetch_bit0);
    state_str->Read(bkg_fetch_bit1);
    state_str->Read(spr_fetch_address);
    state_str->Read(spr_fetch_bit0);
    state_str->Read(spr_fetch_bit1);
    state_str->Read(spr_fetch_attr);
    state_str->Read(spr_zero_buffer,8);
    // VRAM Address register
    state_str->Read(vram_temp);
    state_str->Read(vram_address);
    state_str->Read(vram_address_temp_access);
    state_str->Read(vram_address_temp_access1);
    state_str->Read(vram_increament);
    state_str->Read(vram_flipflop);
    state_str->Read(vram_fine);
    state_str->Read(reg2007buffer);
    // Background
    state_str->Read(bkg_enabled);
    state_str->Read(bkg_clipped);
    state_str->Read(bkg_patternAddress);
    // Sprites
    state_str->Read(spr_enabled);
    state_str->Read(spr_clipped);
    state_str->Read(spr_patternAddress);
    state_str->Read(spr_size16);
    state_str->Read(spr_0Hit);
    state_str->Read(spr_overflow);
    // Colors
    state_str->Read(grayscale);
    state_str->Read(emphasis);
    // Others
    state_str->Read(ppu_2002_temp);
    state_str->Read(ppu_2004_temp);
    state_str->Read(ppu_2007_temp);
    // OAM
    state_str->Read(oam_address);
    state_str->Read(oam_fetch_data);
    state_str->Read(oam_evaluate_slot);
    state_str->Read(oam_evaluate_count);
    state_str->Read(oam_fetch_mode);
    state_str->Read(oam_phase_index );
    state_str->Read(spr_render_i);
    state_str->Read(bkg_render_i);
    state_str->Read(spr_evaluation_i);
    state_str->Read(spr_render_temp_pixel);
}
}
