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
// TODO: test mapper 57
Mapper057::Mapper057()
{
    this->Name = "6-in-1 (SuperGK)";
    this->MapperNumber = 57;
}
void Mapper057::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x8800)
    {
    case 0x8000:
    {
        chr_aaa = value & 0x7;
        chr_hhh = (value & 0x40) >> 3;
        break;
    }
    case 0x8800:
    {
        chr_bbb = value & 0x7;
        if ((value & 0x10) == 0x10)
        {
            Switch32KPRG((value & 0xE0) >> 6, true);
        }
        else
        {
            Switch16KPRG((value & 0xE0) >> 5, 0x8000, true);
            Switch16KPRG((value & 0xE0) >> 5, 0xC000, true);
        }
        SwitchNMT((value & 0x8) == 0x8 ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
    }
    Switch08KCHR(chr_hhh | (chr_aaa | chr_bbb), chr_01K_rom_count > 0);
}
void Mapper057::SaveState()
{
    Board::SaveState();
    state_str->Write(chr_aaa);
    state_str->Write(chr_bbb);
    state_str->Write(chr_hhh);
}
void Mapper057::LoadState()
{
    Board::LoadState();
    state_str->Read(chr_aaa);
    state_str->Read(chr_bbb);
    state_str->Read(chr_hhh);
}
}

