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
Mapper002::Mapper002()
{
    this->Name = "UxROM";
    this->MapperNumber = 2;
}
void Mapper002::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper002::WritePRG(uint16_t& address, uint8_t& value)
{
    // Depending on rom size (64k, 128k or 256k) the board determines which bits to use !
    // Here, the possible values are 0x3, 0x7 or 0xF
    // Implemented in this method:
    Switch16KPRG(value, 0x8000, true);
}
}
