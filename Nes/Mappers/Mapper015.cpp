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
Mapper015::Mapper015()
{
    this->Name = "100-in-1 Contra Function 16";
    this->MapperNumber = 15;
}
void Mapper015::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x3)
    {
    case 0:
    {
        Switch16KPRG(value & 0x3F, 0x8000, true);
        Switch16KPRG((value & 0x3F) | 1, 0xC000, true);
        break;
    }
    case 1:
    {
        Switch16KPRG(value & 0x3F, 0x8000, true);
        Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
        break;
    }
    case 2:
    {
        temp = value << 1;
        temp = ((value & 0x3F) << 1) | ((value >> 7) & 1);
        Switch08KPRG(temp, 0x8000, true);
        Switch08KPRG(temp, 0xA000, true);
        Switch08KPRG(temp, 0xC000, true);
        Switch08KPRG(temp, 0xE000, true);
        break;
    }
    case 3:
    {
        Switch16KPRG(value & 0x3F, 0x8000, true);
        Switch16KPRG(value & 0x3F, 0xC000, true);
        break;
    }
    }
    SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Horz : MIRRORING_Vert);
}
void Mapper015::SaveState()
{
    Board::SaveState();
    state_str->Write(temp);
}
void Mapper015::LoadState()
{
    Board::LoadState();
    state_str->Read(temp);
}
}


