
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
Mapper088::Mapper088()
{
    this->Name = "Namco 118";
    this->MapperNumber = 88;
}
void Mapper088::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper088::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x8001)
    {
    case 0x8000:
        address_8001 = value & 0x7;
        break;
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0x0:
            Switch02KCHR((value & 0x3F) >> 1, 0x0000, chr_01K_rom_count > 0);
            break;
        case 0x1:
            Switch02KCHR((value & 0x3F) >> 1, 0x0800, chr_01K_rom_count > 0);
            break;
        case 0x2:
            Switch01KCHR(value | 0x40, 0x1000, chr_01K_rom_count > 0);
            break;
        case 0x3:
            Switch01KCHR(value | 0x40, 0x1400, chr_01K_rom_count > 0);
            break;
        case 0x4:
            Switch01KCHR(value | 0x40, 0x1800, chr_01K_rom_count > 0);
            break;
        case 0x5:
            Switch01KCHR(value | 0x40, 0x1C00, chr_01K_rom_count > 0);
            break;
        case 0x6:
            Switch08KPRG(value, 0x8000, true);
            break;
        case 0x7:
            Switch08KPRG(value, 0xA000, true);
            break;
        }
        break;
    }
    }
}
void Mapper088::SaveState()
{
    Board::SaveState();
    state_str->Write(address_8001);
}
void Mapper088::LoadState()
{
    Board::LoadState();
    state_str->Read(address_8001);
}
}


