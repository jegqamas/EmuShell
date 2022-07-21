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
Mapper233::Mapper233()
{
    this->Name = "Unknown";
    this->MapperNumber = 233;
}
void Mapper233::SoftReset()
{
    Board::SoftReset();
    bank = 0;
    title ^= 0x20;
}
void Mapper233::HardReset()
{
    Board::HardReset();
    bank = title = 0;
}
void Mapper233::WritePRG(uint16_t& address, uint8_t& value)
{
    bank = value & 0x1F;

    if ((value & 0x20) == 0x20)
    {
        Switch16KPRG(title | bank, 0x8000, true);
        Switch16KPRG(title | bank, 0xC000, true);
    }
    else
        Switch32KPRG(title >> 1 | bank >> 1, true);

    switch ((value >> 6) & 0x3)
    {
    case 0:
        SwitchNMT(0x80);
        break;
    case 1:
        SwitchNMT(MIRRORING_Vert);
        break;
    case 2:
        SwitchNMT(MIRRORING_Horz);
        break;
    case 3:
        SwitchNMT(MIRRORING_OneScB);
        break;
    }
}
void Mapper233::SaveState()
{
    Board::SaveState();
    state_str->Write(bank);
    state_str->Write(title);
}
void Mapper233::LoadState()
{
    Board::LoadState();
    state_str->Read(bank);
    state_str->Read(title);
}
}


