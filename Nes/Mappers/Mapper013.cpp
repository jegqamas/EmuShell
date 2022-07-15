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
// TODO: Nes game "Videomation" mapper 13 not working well
Mapper013::Mapper013()
{
    this->Name = "CPROM";
    this->MapperNumber = 13;
    // We need to set these for default configuration
    this->prg_ram_default_08KB_count = 1;
    this->chr_ram_1KB_default_banks_count=16;
}
void Mapper013::WritePRG(uint16_t& address, uint8_t& value)
{
    // Bus Conflicts
    writeData = (uint8_t)(value & ReadPRG(address));
    Switch04KCHR(writeData & 0x3, 0x1000, chr_01K_rom_count > 0);
}
}

