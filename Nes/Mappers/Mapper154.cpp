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
Mapper154::Mapper154()
{
    this->Name = "NAMCOT-3453";
    this->MapperNumber = 154;
}
void Mapper154::HardReset()
{
    Board::HardReset();
    address_8001 = 0;
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper154::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x8001)
    {
    case 0x8000:
    {
        address_8001 = value & 0x7;
        SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_OneScB : MIRRORING_OneScA);
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0:
            Switch02KCHR((value & 0x3F) >> 1, 0x0000, chr_01K_rom_count > 0);
            break;
        case 1:
            Switch02KCHR((value & 0x3F) >> 1, 0x0800, chr_01K_rom_count > 0);
            break;
        case 2:
            Switch01KCHR(value | 0x40, 0x1000, chr_01K_rom_count > 0);
            break;
        case 3:
            Switch01KCHR(value | 0x40, 0x1400, chr_01K_rom_count > 0);
            break;
        case 4:
            Switch01KCHR(value | 0x40, 0x1800, chr_01K_rom_count > 0);
            break;
        case 5:
            Switch01KCHR(value | 0x40, 0x1C00, chr_01K_rom_count > 0);
            break;
        case 6:
            Switch08KPRG(value, 0x8000, true);
            break;
        case 7:
            Switch08KPRG(value, 0xA000, true);
            break;
        }
        break;
    }
    }
}
void Mapper154::SaveState()
{
    Board::SaveState();
    state_str->Write(address_8001);
}
void Mapper154::LoadState()
{
    Board::LoadState();
    state_str->Read(address_8001);
}
}


