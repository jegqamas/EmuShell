
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
Mapper082::Mapper082()
{
    this->Name = "Taito X1-17";
    this->MapperNumber = 82;
}
void Mapper082::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper082::WriteSRM(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x7EF0:
        Switch02KCHR(value >> 1, chr_mode ? 0x1000 : 0x0000, chr_01K_rom_count > 0);
        break;
    case 0x7EF1:
        Switch02KCHR(value >> 1, chr_mode ? 0x1800 : 0x0800, chr_01K_rom_count > 0);
        break;
    case 0x7EF2:
        Switch01KCHR(value, chr_mode ? 0x0000 : 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x7EF3:
        Switch01KCHR(value, chr_mode ? 0x0400 : 0x1400, chr_01K_rom_count > 0);
        break;
    case 0x7EF4:
        Switch01KCHR(value, chr_mode ? 0x0800 : 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x7EF5:
        Switch01KCHR(value, chr_mode ? 0x0C00 : 0x1C00, chr_01K_rom_count > 0);
        break;
    case 0x7EF6:
        SwitchNMT((value & 0x1) == 0x1 ? MIRRORING_Vert : MIRRORING_Horz);
        chr_mode = (value & 0x2) == 0x2;
        break;
    case 0x7EFA:
        Switch08KPRG(value >> 2, 0x8000, true);
        break;
    case 0x7EFB:
        Switch08KPRG(value >> 2, 0xA000, true);
        break;
    case 0x7EFC:
        Switch08KPRG(value >> 2, 0xC000, true);
        break;
    }
}
void Mapper082::SaveState()
{
    Board::SaveState();
    state_str->Write(chr_mode);
}
void Mapper082::LoadState()
{
    Board::LoadState();
    state_str->Read(chr_mode);
}
}

