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
Mapper227::Mapper227()
{
    this->Name = "Unknown";
    this->MapperNumber = 227;
}
void Mapper227::HardReset()
{
    Board::HardReset();
    Switch16KPRG(0, 0xC000, true);
    flag_o = false;
    flag_s = false;
    flag_l = false;
    prg_reg = 0;
    ToggleCHRRAMWritable(true);
}
void Mapper227::WritePRG(uint16_t& address, uint8_t& value)
{
    flag_s = (address & 0x1) == 0x1;
    flag_o = (address & 0x80) == 0x80;
    flag_l = (address & 0x200) == 0x200;
    prg_reg = ((address >> 2) & 0x1F) | ((address >> 3) & 0x20);
    SwitchNMT((address & 0x2) == 0x2 ? MIRRORING_Horz :MIRRORING_Vert);
    ToggleCHRRAMWritable(!flag_o);
    if (flag_o)
    {
        if (!flag_s)
        {
            Switch16KPRG(prg_reg, 0x8000, true);
            Switch16KPRG(prg_reg, 0xC000, true);
        }
        else
        {
            Switch32KPRG(prg_reg >> 1, true);
        }
    }
    else
    {
        if (!flag_l)
        {
            if (!flag_s)
            {
                Switch16KPRG(prg_reg, 0x8000, true);
                Switch16KPRG(prg_reg & 0x38, 0xC000, true);
            }
            else
            {
                Switch16KPRG(prg_reg & 0x3E, 0x8000, true);
                Switch16KPRG(prg_reg & 0x38, 0xC000, true);
            }
        }
        else
        {
            if (!flag_s)
            {
                Switch16KPRG(prg_reg, 0x8000, true);
                Switch16KPRG(prg_reg | 0x7, 0xC000, true);
            }
            else
            {
                Switch16KPRG(prg_reg & 0x3E, 0x8000, true);
                Switch16KPRG(prg_reg | 0x7, 0xC000, true);
            }
        }
    }
}
void Mapper227::SaveState()
{
    Board::SaveState();
    state_str->Write(flag_o);
    state_str->Write(flag_s);
    state_str->Write(flag_l);
    state_str->Write(prg_reg);
}
void Mapper227::LoadState()
{
    Board::LoadState();
    state_str->Read(flag_o);
    state_str->Read(flag_s);
    state_str->Read(flag_l);
    state_str->Read(prg_reg);
}
}


