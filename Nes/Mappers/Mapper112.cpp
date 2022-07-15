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
Mapper112::Mapper112()
{
    this->Name = "Asder";
    this->MapperNumber = 112;
}
void Mapper112::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper112::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE001)
    {
    case 0x8000:
    {
        address_A000 = value & 0x7;
        break;
    }
    case 0xA000:
    {
        switch (address_A000)
        {
        case 0:
            Switch08KPRG(value, 0x8000, true);
            break;
        case 1:
            Switch08KPRG(value, 0xA000, true);
            break;
        case 2:
            Switch02KCHR(value >> 1, 0x0000, chr_01K_rom_count > 0);
            break;
        case 3:
            Switch02KCHR(value >> 1, 0x0800, chr_01K_rom_count > 0);
            break;
        case 4:
            Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        case 5:
            Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
            break;
        case 6:
            Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
            break;
        case 7:
            Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
            break;
        }
        break;
    }
    case 0xE000:
        SwitchNMT((value & 1) == 1 ? MIRRORING_Horz : MIRRORING_Vert);
        break;

    }
}
void Mapper112::SaveState()
{
    Board::SaveState();
    state_str->Write(address_A000);
}
void Mapper112::LoadState()
{
    Board::LoadState();
    state_str->Read(address_A000);
}
}

