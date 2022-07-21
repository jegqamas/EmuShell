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
Mapper202::Mapper202()
{
    this->Name = "150-in-1";
    this->MapperNumber = 202;
}
void Mapper202::WritePRG(uint16_t& address, uint8_t& value)
{
    SwitchNMT((address & 0x1) == 0x1 ? MIRRORING_Horz : MIRRORING_Vert);
    Switch08KCHR((address >> 1) & 0x7, chr_01K_rom_count > 0);
    if ((address & 0xC) == 0xC)
    {
        Switch32KPRG(0x3, true);
    }
    else
    {
        Switch16KPRG((address >> 1) & 0x7, 0x8000, true);
        Switch16KPRG((address >> 1) & 0x7, 0xC000, true);
    }
}
}


