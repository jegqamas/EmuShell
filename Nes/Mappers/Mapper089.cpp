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
Mapper089::Mapper089()
{
    this->Name = "Sunsoft Early";
    this->MapperNumber = 89;
}
void Mapper089::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper089::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR((value & 0x7) | ((value >> 4) & 0x8), chr_01K_rom_count > 0);
    Switch16KPRG((value >> 4) & 0x7, 0x8000, true);
    SwitchNMT((value & 0x8) == 0x8 ? MIRRORING_OneScB : MIRRORING_OneScA);
}
}

