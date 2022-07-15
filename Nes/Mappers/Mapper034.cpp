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
#include <iostream>

namespace NES
{
Mapper034::Mapper034()
{
    this->Name = "BxROM/NINA-001";
    this->MapperNumber = 34;
}
void Mapper034::HardReset()
{
    Board::HardReset();
    //Board Type: IREM-BNROM
    BxROM = true;// Assume BxROM
    if (GameDBIndex>=0)
    {
        if (NesDB::ConatinBoardType(GameDBIndex,"NINA"))
        {
            BxROM = false;
        }
    }
    std::cout << "NES: Switched to board "<< (BxROM? "BxROM\n": "NINA\n");
}
void Mapper034::WriteSRM(uint16_t& address, uint8_t& value)
{
    Board::WriteSRM(address, value);
    if (!BxROM)// NINA-001
    {
        switch (address)
        {
        case 0x7FFD:
            Switch32KPRG(value, true);
            break;
        case 0x7FFE:
            Switch04KCHR(value, 0x0000, chr_01K_rom_count > 0);
            break;
        case 0x7FFF:
            Switch04KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        }
    }
}
void Mapper034::WritePRG(uint16_t& address, uint8_t& value)
{
    //if (BxROM)
    {
        writeData = (uint8_t)(value & ReadPRG(address));// Bus Conflicts
        Switch32KPRG(writeData, true);
    }
}
}

