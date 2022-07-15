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
#include "../../Core.h"
#include "../../Definations.h"

namespace NES
{
Mapper071::Mapper071()
{
    this->Name = "Camerica";
    this->MapperNumber = 71;
}
void Mapper071::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_ram_mask, 0xC000, true);
    // This is not a hack !
    // This is a qoute from 071.txt of [iNES Mappers by Mapper Number v0.6.1 by Disch]:
    // "One in paticular that needs to be noted is the board
    // used by Fire Hawk -- which has mapper controlled 1-screen mirroring.  On other boards, mirroring is
    // hardwired!  This is yet another one of those terrific mapper number incompatibilities."

    fireHawk = (EmuShell::GameCRC32 == 0x1BC686A8);
}
void Mapper071::WritePRG(uint16_t& address, uint8_t& value)
{
    if (address < 0xA000)
    {
        if (fireHawk)
            SwitchNMT((value & 0x10) == 0x10 ? MIRRORING_OneScB : MIRRORING_OneScA);
    }
    else if (address >= 0xC000)
        Switch16KPRG(value, 0x8000, true);
}
}

