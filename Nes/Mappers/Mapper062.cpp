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
// TODO: test mapper 62
Mapper062::Mapper062()
{
    this->Name = "Super 700-in-1";
    this->MapperNumber = 62;
}
void Mapper062::WritePRG(uint16_t& address, uint8_t& value)
{
    prg_page = ((address & 0x3F00) >> 8) | (address & 0x40);
    Switch08KCHR(((address & 0x1F) << 2) | (value & 0x3), chr_01K_rom_count > 0);
    if ((address & 0x20) == 0x20)
    {
        Switch16KPRG(prg_page, 0x8000, true);
        Switch16KPRG(prg_page, 0xC000, true);
    }
    else
        Switch32KPRG(prg_page >> 1, true);
    SwitchNMT((address & 0x80) == 0x80 ? MIRRORING_Horz : MIRRORING_Vert);
}
}

