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

namespace NES
{
//TODO: Mapper 180's Crazy Climber needs special controller which not implemented yet.
Mapper193::Mapper193()
{
    this->Name = "Unknown";
    this->MapperNumber = 193;
    this->prg_ram_default_08KB_count=1;
    this->chr_ram_1KB_default_banks_count = 32;
}
void Mapper193::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask - 2, 0xA000, true);
    Switch08KPRG(prg_08K_rom_mask - 1, 0xC000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper193::WriteSRM(uint16_t& address, uint8_t& value)
{
    switch (address & 0x6003)
    {
    case 0x6000:
        Switch04KCHR(value >> 2, 0x0000, chr_01K_rom_count > 0);
        break;
    case 0x6001:
        Switch02KCHR(value >> 1, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x6002:
        Switch02KCHR(value >> 1, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x6003:
        Switch08KPRG(value, 0x8000, true);
        break;
    }
}
}



