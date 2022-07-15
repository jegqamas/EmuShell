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
Mapper041::Mapper041()
{
    this->Name = "Caltron 6-in-1";
    this->MapperNumber = 41;
}
void Mapper041::HardReset()
{
    Board::HardReset();
    vromReg = 0;
    enableReg = true;
}
void Mapper041::WriteSRM(uint16_t& address, uint8_t& value)
{
    if (address <= 0x67FF)
    {
        Switch32KPRG(address & 0x7, true);
        enableReg = (address & 0x4) == 0x4;

        vromReg = (vromReg & 0x03) | ((address >> 1) & 0x0C);
        Switch08KCHR(vromReg, chr_01K_rom_count > 0);
        SwitchNMT((address & 0x20) == 0x20 ? MIRRORING_Horz : MIRRORING_Vert);
    }
    else
        Board::WriteSRM(address, value);
}
void Mapper041::WritePRG(uint16_t& address, uint8_t& value)
{
    if (enableReg)
    {
        vromReg = (vromReg & 0x0C) | (value & 0x3);
        Switch08KCHR(vromReg, chr_01K_rom_count > 0);
    }
}
void Mapper041::SaveState()
{
    Board::SaveState();
    state_str->Write(vromReg);
    state_str->Write(enableReg);
}
void Mapper041::LoadState()
{
    Board::LoadState();
    state_str->Read(vromReg);
    state_str->Read(enableReg);
}
}


