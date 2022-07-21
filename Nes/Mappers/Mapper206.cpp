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
#include <iostream>
#include "../../Definations.h"

namespace NES
{
Mapper206::Mapper206()
{
    this->Name = "Unknown";
    this->MapperNumber = 206;
}
void Mapper206::HardReset()
{
    Board::HardReset();
    // Flags
    flag_c = flag_p = false;
    address_8001 = 0;

    prg_reg[0] = 0;
    prg_reg[1] = 1;
    prg_reg[2] = prg_08K_rom_mask - 1;
    prg_reg[3] = prg_08K_rom_mask;
    SetupPRG();

    // CHR
    for (int i = 0; i < 6; i++)
        chr_reg[i] = 0;
}
void Mapper206::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE001)
    {
    case 0x8000:
    {
        address_8001 = value & 0x7;
        flag_c = (value & 0x80) != 0;
        flag_p = (value & 0x40) != 0;
        SetupCHR();
        SetupPRG();
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            chr_reg[address_8001] = value;
            SetupCHR();
            break;
        case 6:
        case 7:
            prg_reg[address_8001 - 6] = value & prg_08K_rom_mask;
            SetupPRG();
            break;
        }
        break;
    }
    }
}
void Mapper206::SetupCHR()
{
    if (!flag_c)
    {
        Switch02KCHR(chr_reg[0] >> 1, 0x0000, chr_01K_rom_count > 0);
        Switch02KCHR(chr_reg[1] >> 1, 0x0800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[2], 0x1000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3], 0x1400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4], 0x1800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5], 0x1C00, chr_01K_rom_count > 0);
    }
    else
    {
        Switch02KCHR(chr_reg[0] >> 1, 0x1000, chr_01K_rom_count > 0);
        Switch02KCHR(chr_reg[1] >> 1, 0x1800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[2], 0x0000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3], 0x0400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4], 0x0800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5], 0x0C00, chr_01K_rom_count > 0);
    }
}
void Mapper206::SetupPRG()
{
    Switch08KPRG(prg_reg[flag_p ? 2 : 0], 0x8000, true);
    Switch08KPRG(prg_reg[1], 0xA000, true);
    Switch08KPRG(prg_reg[flag_p ? 0 : 2], 0xC000, true);
    Switch08KPRG(prg_reg[3], 0xE000, true);
}
void Mapper206::SaveState()
{
    Board::SaveState();

    state_str->Write(flag_c);
    state_str->Write(flag_p);
    state_str->Write(address_8001);

    for (int i=0; i<6; i++)
        state_str->Write(chr_reg[i]);

    for (int i=0; i<4; i++)
        state_str->Write( prg_reg[i]);
}
void Mapper206::LoadState()
{
    Board::LoadState();

    state_str->Read(flag_c);
    state_str->Read(flag_p);
    state_str->Read(address_8001);

    for (int i=0; i<6; i++)
        state_str->Read(chr_reg[i]);

    for (int i=0; i<4; i++)
        state_str->Read( prg_reg[i]);
}
}


