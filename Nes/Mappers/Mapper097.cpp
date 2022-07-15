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
Mapper097::Mapper097()
{
    this->Name = "Irem - PRG HI";
    this->MapperNumber = 97;
}
void Mapper097::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0x8000, true);
}
void Mapper097::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch16KPRG(value & 0xF, 0xC000, true);
    switch ((address >> 6) & 0x3)
    {
    case 0:
        SwitchNMT(MIRRORING_OneScA);
        break;
    case 1:
        SwitchNMT(MIRRORING_Horz);
        break;
    case 2:
        SwitchNMT(MIRRORING_Vert);
        break;
    case 3:
        SwitchNMT(MIRRORING_OneScB);
        break;
    }
}
}

