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
#include "../../Definations.h"

namespace NES
{
Mapper230::Mapper230()
{
    this->Name = "Unknown";
    this->MapperNumber = 230;
}
void Mapper230::HardReset()
{
    Board::HardReset();

    //set contra mode
    contraMode = true;
    Switch16KPRG(0, 0x8000, true);
    Switch16KPRG(7, 0xC000, true);
}
void Mapper230::SoftReset()
{
    Board::SoftReset();
    contraMode = !contraMode;
    if (contraMode)
    {
        Switch16KPRG(0, 0x8000, true);
        Switch16KPRG(7, 0xC000, true);
        SwitchNMT(MIRRORING_Vert);
    }
    else
    {
        Switch08KCHR(0, chr_01K_rom_count > 0);
        Switch16KPRG(8, 0x8000, true);
        Switch16KPRG(39, 0xC000, true);
    }
}
void Mapper230::WritePRG(uint16_t& address, uint8_t& value)
{
    if (contraMode)
    {
        Switch16KPRG(value & 0x7, 0x8000, true);
        Switch16KPRG(7, 0xC000, true);
        SwitchNMT(MIRRORING_Vert);
    }
    else
    {
        SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Vert : MIRRORING_Horz);

        if ((value & 0x20) == 0x20)
        {
            Switch16KPRG((value & 0x1F) + 8, 0x8000, true);
            Switch16KPRG((value & 0x1F) + 8, 0xC000, true);
        }
        else
            Switch32KPRG(((value & 0x1F) >> 1) + 4, true);
    }
}
}

