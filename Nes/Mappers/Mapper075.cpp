// This file is part of EmuShell
// A multi video game systems emulator.
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

#include "_Mappers.h"
#include "../../Definations.h"

namespace NES
{
Mapper075::Mapper075()
{
    this->Name = "VRC1";
    this->MapperNumber = 75;
}
void Mapper075::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper075::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF000)
    {
    case 0x8000:
        Switch08KPRG(value & 0xF, 0x8000, true);
        break;
    case 0xA000:
        Switch08KPRG(value & 0xF, 0xA000, true);
        break;
    case 0xC000:
        Switch08KPRG(value & 0xF, 0xC000, true);
        break;
    case 0x9000:
    {
        SwitchNMT((value & 0x1) == 0x1 ? MIRRORING_Horz : MIRRORING_Vert);
        chr0_reg = (chr0_reg & 0xF) | ((value & 0x2) << 3);
        Switch04KCHR(chr0_reg, 0x0000, chr_01K_rom_count > 0);
        chr1_reg = (chr1_reg & 0xF) | ((value & 0x4) << 2);
        Switch04KCHR(chr1_reg, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xE000:
    {
        chr0_reg = (chr0_reg & 0x10) | (value & 0xF);
        Switch04KCHR(chr0_reg, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xF000:
    {
        chr1_reg = (chr1_reg & 0x10) | (value & 0xF);
        Switch04KCHR(chr1_reg, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    }
}
void Mapper075::SaveState()
{
    Board::SaveState();
    state_str->Write(chr0_reg);
    state_str->Write(chr1_reg);
}
void Mapper075::LoadState()
{
    Board::LoadState();
    state_str->Read(chr0_reg);
    state_str->Read(chr1_reg);
}
}

