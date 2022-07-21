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
// TODO: Mapper 68 "After Burner" access violation
Mapper068::Mapper068()
{
    this->Name = "Sunsoft 4";
    this->MapperNumber = 68;
}
void Mapper068::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper068::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF000)
    {
    case 0x8000:
    {
        Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0x9000:
    {
        Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xA000:
    {
        Switch02KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB000:
    {
        Switch02KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC000:
    {
        nt_reg0 = (value & 0x7F) | 0x80;
        break;
    }
    case 0xD000:
    {
        nt_reg1 = (value & 0x7F) | 0x80;
        break;
    }
    case 0xE000:
    {
        flag_r = (value & 0x10) == 0x10;
        flag_m = (value & 0x01) == 0x01;
        SwitchNMT(flag_m ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
    case 0xF000:
    {
        Switch16KPRG(value, 0x8000, true);
        break;
    }
    }
}
uint8_t Mapper068::ReadNMT(uint16_t& address)
{
    if (!flag_r)
        return nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF];
    else
    {
        switch ((address >> 10) & 3)
        {
        case 0:
            return chr_banks[nt_reg0 + chr_rom_bank_offset][address & 0x3FF];
        case 1:
            return chr_banks[(flag_m ? nt_reg0 : nt_reg1) + chr_rom_bank_offset][address & 0x3FF];
        case 2:
            return chr_banks[(flag_m ? nt_reg1 : nt_reg0) + chr_rom_bank_offset][address & 0x3FF];
        case 3:
            return chr_banks[nt_reg1 + chr_rom_bank_offset][address & 0x3FF];
        }
    }
    return 0;// make compiler happy.
}
void Mapper068::WriteNMT(uint16_t& address, uint8_t& value)
{
    if (!flag_r)
        Board::WriteNMT(address, value);
}
void Mapper068::SaveState()
{
    Board::SaveState();
    state_str->Write(flag_r);
    state_str->Write(flag_m);
    state_str->Write(nt_reg0);
    state_str->Write(nt_reg1);
    state_str->Write(temp);
}
void Mapper068::LoadState()
{
    Board::LoadState();
    state_str->Read(flag_r);
    state_str->Read(flag_m);
    state_str->Read(nt_reg0);
    state_str->Read(nt_reg1);
    state_str->Read(temp);
}
}

