// This file is part of EmuShell
// A multi video game systems emulator.
//
// Copyright(C) 2015 - 2022 Alaa Ibrahim Hadid
// E-mail: mailto:alaahadidfreeware@gmail.com
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
#include "_Mappers.h"
#include "../../Definations.h"

namespace NES
{

Mapper009::Mapper009()
{
    this->Name = "MMC2";
    this->MapperNumber = 9;

    latch_a = 0xFE;
    latch_b = 0xFE;
}
void Mapper009::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask - 2, 0xA000, true);
    Switch08KPRG(prg_08K_rom_mask - 1, 0xC000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    chr_reg0B = 4;
}
void Mapper009::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF000)
    {
    case 0xA000:
    {
        Switch08KPRG(value, 0x8000, true);
        break;
    }
    case 0xB000:
    {
        chr_reg0A = value;
        if (latch_a == 0xFD)
            Switch04KCHR(chr_reg0A, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xC000:
    {
        chr_reg0B = value;
        if (latch_a == 0xFE)
            Switch04KCHR(chr_reg0B, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD000:
    {
        chr_reg1A = value;
        if (latch_b == 0xFD)
            Switch04KCHR(chr_reg1A, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xE000:
    {
        chr_reg1B = value;
        if (latch_b == 0xFE)
            Switch04KCHR(chr_reg1B, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xF000:
    {
        SwitchNMT((value & 0x1) == 0x1 ? MIRRORING_Horz: MIRRORING_Vert);
        break;
    }
    }
}
uint8_t Mapper009::ReadCHR(uint16_t& address, bool spriteFetch)
{
    if ((address & 0x1FF0) == 0x0FD0 && latch_a != 0xFD)
    {
        latch_a = 0xFD;
        Switch04KCHR(chr_reg0A, 0x0000, chr_01K_rom_count > 0);
    }
    else if ((address & 0x1FF0) == 0x0FE0 && latch_a != 0xFE)
    {
        latch_a = 0xFE;
        Switch04KCHR(chr_reg0B, 0x0000, chr_01K_rom_count > 0);
    }
    else if ((address & 0x1FF0) == 0x1FD0 && latch_b != 0xFD)
    {
        latch_b = 0xFD;
        Switch04KCHR(chr_reg1A, 0x1000, chr_01K_rom_count > 0);
    }
    else if ((address & 0x1FF0) == 0x1FE0 && latch_b != 0xFE)
    {
        latch_b = 0xFE;
        Switch04KCHR(chr_reg1B, 0x1000, chr_01K_rom_count > 0);
    }
    return Board::ReadCHR(address, spriteFetch);
}
void Mapper009::SaveState()
{
    Board::SaveState();
    state_str->Write(chr_reg0A);
    state_str->Write( chr_reg0B);
    state_str->Write(chr_reg1A);
    state_str->Write(chr_reg1B);
    state_str->Write(latch_a);
    state_str->Write(latch_b);
}
void Mapper009::LoadState()
{
    Board::LoadState();
    state_str->Read(chr_reg0A);
    state_str->Read( chr_reg0B);
    state_str->Read(chr_reg1A);
    state_str->Read(chr_reg1B);
    state_str->Read(latch_a);
    state_str->Read(latch_b);
}
}
