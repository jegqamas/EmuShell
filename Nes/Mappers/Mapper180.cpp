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
    //TODO: Mapper 180's Crazy Climber needs special controller which not implemented yet.
Mapper180::Mapper180()
{
    this->Name = "Nihon Bussan";
    this->MapperNumber = 180;
}
void Mapper180::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch16KPRG(value & 0x7, 0xC000, true);
}
}

