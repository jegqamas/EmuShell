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
// TODO: Test mapper 51
Mapper051::Mapper051()
{
    this->Name = "11-in-1";
    this->MapperNumber = 51;
}
void Mapper051::HardReset()
{
    Board::HardReset();
    bank = 0;
    mode = 1;
    offset = 0;
}
void Mapper051::WriteSRM(uint16_t& address, uint8_t& value)
{
    mode = (value >> 3 & 0x2) | (value >> 1 & 0x1);
    UpdateBanks();
}
void Mapper051::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE000)
    {
    case 0x8000:
    case 0xE000:
    {
        bank = value & 0xF;
        UpdateBanks();
        break;
    }
    case 0xC000:
    {
        bank = value & 0xF;
        mode = (value >> 3 & 0x2) | (mode & 0x1);
        UpdateBanks();
        break;
    }
    }
}
void Mapper051::UpdateBanks()
{
    offset = 0;

    if ((mode & 0x1) == 0x1)
    {
        Switch32KPRG(bank, true);
        offset = 0x23;
    }
    else
    {
        Switch08KPRG((bank << 1) | (mode >> 1), 0x8000, true);
        Switch08KPRG(bank << 1 | 0x7, 0x8000, true);
        offset = 0x2F;
    }
    Switch08KPRG(offset | (bank << 2), 0x6000, true);
    SwitchNMT((mode == 0x3) ? MIRRORING_Horz : MIRRORING_Vert);
}
void Mapper051::SaveState()
{
    Board::SaveState();
    state_str->Write(bank);
    state_str->Write(mode);
    state_str->Write(offset);
}
void Mapper051::LoadState()
{
    Board::LoadState();
    state_str->Read(bank);
    state_str->Read(mode);
    state_str->Read(offset);
}
}

