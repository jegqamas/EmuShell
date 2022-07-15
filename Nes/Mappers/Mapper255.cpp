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
Mapper255::Mapper255()
{
    this->Name = "Unknown";
    this->MapperNumber = 255;
}
void Mapper255::HardReset()
{
    Board::HardReset();
    for (int i=0; i<4; i++)
        RAM[i]=0;
}
void Mapper255::WriteEXP(uint16_t& address, uint8_t& value)
{
    if (address >= 0x5800)
        RAM[address & 0x3] = (uint8_t)(value & 0xF);
}
uint8_t Mapper255::ReadEXP(uint16_t& address)
{
    if (address >= 0x5800)
        return RAM[address & 0x3];
    return 0;
}
void Mapper255::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR(address & 0x3F, chr_01K_rom_count > 0);
    if ((address & 0x1000) == 0x1000)
    {
        Switch16KPRG(address >> 6 & 0x3F, 0x8000, true);
        Switch16KPRG(address >> 6 & 0x3F, 0xC000, true);
    }
    else
        Switch32KPRG(((address >> 6) & 0x3F) >> 1, true);
    SwitchNMT((address & 0x2000) == 0x2000 ? MIRRORING_Horz : MIRRORING_Vert);
}
void Mapper255::SaveState()
{
    Board::SaveState();
    for (int i=0; i<4; i++)
        state_str->Write(RAM[i]);
}
void Mapper255::LoadState()
{
    Board::LoadState();
    for (int i=0; i<4; i++)
        state_str->Read(RAM[i]);
}
}
