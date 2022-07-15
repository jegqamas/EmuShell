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
Mapper228::Mapper228()
{
    this->Name = "Unknown";
    this->MapperNumber = 228;
}
void Mapper228::HardReset()
{
    Board::HardReset();
    for (int i=0; i<4; i++)
        RAM[i]=0;
    bank=0;
}
void Mapper228::WriteEXP(uint16_t& address, uint8_t& value)
{
    RAM[address & 0x3] = (uint8_t)(value & 0xF);
}
uint8_t Mapper228::ReadEXP(uint16_t& address)
{
    return RAM[address & 0x3];
}
void Mapper228::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR(((address & 0xF) << 2) | (value & 0x3), chr_01K_rom_count > 0);
    SwitchNMT((address & 0x2000) == 0x2000 ? MIRRORING_Horz : MIRRORING_Vert);
    bank = (address >> 7 & 0x1F) + (address >> 7 & address >> 8 & 0x10);
    if ((address & 0x0020) == 0x0020)
    {
        Switch16KPRG((bank << 2) | (address >> 5 & 0x2), 0x8000, true);
        Switch16KPRG((bank << 2) | (address >> 5 & 0x2), 0xC000, true);
    }
    else
    {
        Switch32KPRG(bank, true);
    }
}
void Mapper228::SaveState()
{
    Board::SaveState();
    for (int i=0; i<4; i++)
        state_str->Write(RAM[i]);
    state_str->Write(bank);
}
void Mapper228::LoadState()
{
    Board::LoadState();
    for (int i=0; i<4; i++)
        state_str->Read(RAM[i]);
    state_str->Read(bank);
}
}



