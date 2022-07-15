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
// TODO: test mapper 61
Mapper061::Mapper061()
{
    this->Name = "20-in-1";
    this->MapperNumber = 61;
}
void Mapper061::WritePRG(uint16_t& address, uint8_t& value)
{
    if ((address & 0x10) == 0)
        Switch32KPRG(address & 0xF, true);
    else
    {
        Switch16KPRG(((address & 0xF) << 1) | ((address & 0x20) >> 5), 0x8000, true);
        Switch16KPRG(((address & 0xF) << 1) | ((address & 0x20) >> 5), 0xC000, true);
    }
    SwitchNMT((address & 0x80) == 0x80 ? MIRRORING_Horz : MIRRORING_Vert);
}
}

