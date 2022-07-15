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
// TODO: Test mapper 60
Mapper060::Mapper060()
{
    this->Name = "Unknown";
    this->MapperNumber = 60;
}
void Mapper060::HardReset()
{
    Board::HardReset();
    latch = 0;
    menu = 0;
}
void Mapper060::SoftReset()
{
    Board::SoftReset();
    latch = 0;
    menu = (menu + 1) & 0x3;
    Switch08KCHR(menu, chr_01K_rom_count > 0);
    Switch16KPRG(menu, 0x8000, true);
    Switch16KPRG(menu, 0xC000, true);
}
void Mapper060::WritePRG(uint16_t& address, uint8_t& value)
{
    latch = address & 0x100;
    SwitchNMT((address & 0x8) == 0x8 ? MIRRORING_Horz : MIRRORING_Vert);
    Switch16KPRG((address >> 4) & ~(~address >> 7 & 0x1), 0x8000, true);
    Switch16KPRG((address >> 4) | (~address >> 7 & 0x1), 0xC000, true);
    Switch08KCHR(address, chr_01K_rom_count > 0);
}
uint8_t Mapper060::ReadPRG(uint16_t& address)
{
    if (latch == 0)
        return Board::ReadPRG(address);
    else
        return menu;
}
void Mapper060::SaveState()
{
    Board::SaveState();
    state_str->Write(latch);
    state_str->Write(menu);
}
void Mapper060::LoadState()
{
    Board::LoadState();
    state_str->Read(latch);
    state_str->Read(menu);
}
}

