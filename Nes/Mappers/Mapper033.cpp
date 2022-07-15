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
#include <iostream>

namespace NES
{
Mapper033::Mapper033()
{
    this->Name = "Taito TC0190/TC0350";
    this->MapperNumber = 33;
}
void Mapper033::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper033::WritePRG(uint16_t& address, uint8_t& value)
{
    // Mapper 33 mode
    switch (address & 0xA003)
    {
    case 0x8000:
    {
        SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Horz : MIRRORING_Vert);
        Switch08KPRG((value & 0x3F), 0x8000, true);
        break;
    }
    case 0x8001:
    {
        Switch08KPRG((value & 0x3F), 0xA000, true);
        break;
    }
    case 0x8002:
    {
        Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0x8003:
    {
        Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xA000:
    {
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xA001:
    {
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xA002:
    {
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xA003:
    {
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    }
}
}

