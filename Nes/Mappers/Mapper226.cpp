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
Mapper226::Mapper226()
{
    this->Name = "Unknown";
    this->MapperNumber = 226;
}
void Mapper226::HardReset()
{
    Board::HardReset();
    prg_reg = 0;
    prg_mode = false;
}
void Mapper226::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x8001)
    {
    case 0x8000:
    {
        prg_reg = (value & 0x1F) | ((value & 0x80) >> 2) | (prg_reg & 0xC0);
        prg_mode = (value & 0x20) == 0x20;
        SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Vert : MIRRORING_Horz);
        if (prg_mode)
        {
            Switch16KPRG(prg_reg, 0x8000, true);
            Switch16KPRG(prg_reg, 0xC000, true);
        }
        else
        {
            Switch32KPRG(prg_reg >> 1, true);
        }
        break;
    }
    case 0x8001:
    {
        prg_reg = ((value & 0x1) << 6) | (prg_reg & 0x3F);
        if (prg_mode)
        {
            Switch16KPRG(prg_reg, 0x8000, true);
            Switch16KPRG(prg_reg, 0xC000, true);
        }
        else
        {
            Switch32KPRG(prg_reg >> 1, true);
        }
        break;
    }
    }
}
void Mapper226::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_reg);
    state_str->Write(prg_mode);
}
void Mapper226::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_reg);
    state_str->Read(prg_mode);
}
}

