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
Mapper070::Mapper070()
{
    this->Name = "Bandai";
    this->MapperNumber = 70;
}
void Mapper070::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_08K_rom_mask, 0xC000, true);
}
void Mapper070::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch16KPRG((value >> 4) & 0xF, 0x8000, true);
    Switch08KCHR(value & 0xF, chr_01K_rom_count > 0);
}
}

