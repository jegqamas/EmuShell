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
Mapper232::Mapper232()
{
    this->Name = "Unknown";
    this->MapperNumber = 232;
}
void Mapper232::HardReset()
{
    Board::HardReset();
    Switch16KPRG(3, 0xC000, true);
}
void Mapper232::WritePRG(uint16_t& address, uint8_t& value)
{
    if (address < 0xC000)
        prg_reg = ((value & 0x18) >> 1) | (prg_reg & 0x3);
    else
        prg_reg = (prg_reg & 0xC) | (value & 0x3);

    Switch16KPRG(prg_reg, 0x8000, true);
    Switch16KPRG(3 | (prg_reg & 0xC), 0xC000, true);
}
void Mapper232::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_reg);
}
void Mapper232::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_reg);
}
}

