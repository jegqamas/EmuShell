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
Mapper076::Mapper076()
{
    this->Name = "Namco 109";
    this->MapperNumber = 76;
}
void Mapper076::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask - 1, 0xC000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    address_8001 = 0;
    prg_a = false;
}
void Mapper076::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE001)
    {
    case 0x8000:
    {
        address_8001 = value & 0x7;
        prg_a = (value & 0x40) == 0x40;
        Switch08KPRG(prg_reg, prg_a ? 0xC000 : 0x8000, true);
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0x2:
            Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
            break;
        case 0x3:
            Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
            break;
        case 0x4:
            Switch02KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        case 0x5:
            Switch02KCHR(value, 0x1800, chr_01K_rom_count > 0);
            break;
        case 0x6:
            Switch08KPRG(prg_reg = value, prg_a ? 0xC000 : 0x8000, true);
            break;
        case 0x7:
            Switch08KPRG(value, 0xA000, true);
            break;
        }
        break;
    }
    case 0xA000:
        SwitchNMT((value & 1) == 1 ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
}
void Mapper076::SaveState()
{
    Board::SaveState();
    state_str->Write(address_8001);
    state_str->Write( prg_a);
    state_str->Write( prg_reg);
}
void Mapper076::LoadState()
{
    Board::LoadState();
    state_str->Read(address_8001);
    state_str->Read( prg_a);
    state_str->Read( prg_reg);
}
}

