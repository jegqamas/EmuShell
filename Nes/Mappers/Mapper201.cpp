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

namespace NES
{
Mapper201::Mapper201()
{
    this->Name = "Unknown";
    this->MapperNumber = 201;
}
void Mapper201::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR(address & 0xFF, chr_01K_rom_count > 0);
    Switch32KPRG(address & 0xFF, true);
}
}


