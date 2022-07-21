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
Mapper078::Mapper078()
{
    this->Name = "Irem 74HC161/32";
    this->MapperNumber = 78;
}
void Mapper078::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    mirroring_mode_single = false;
    // This is not a hack, we need to check out the board to setup the mirroring configuration.
    if (NES::GameDBIndex>=0)
        if (NesDB::ContainChip(NES::GameDBIndex, "JALECO-JF-16"))
            mirroring_mode_single = true;
}
void Mapper078::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR((value >> 4) & 0xF, chr_01K_rom_count > 0);
    Switch16KPRG(value & 0x7, 0x8000, true);
    if (mirroring_mode_single)
        SwitchNMT((value & 0x8) == 0x8 ? MIRRORING_OneScB : MIRRORING_OneScA);
    else
        SwitchNMT((value & 0x8) == 0x8 ? MIRRORING_Vert :MIRRORING_Horz);
}
}


