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
Mapper022::Mapper022()
{
    this->Name = "VRC2";
    this->MapperNumber = 22;
}
void Mapper022::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    for (int i=0; i<8; i++)
        chr_reg[i]=0;
}
void Mapper022::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x8000:
    case 0x8002:
    case 0x8001:
    case 0x8003:
    {
        Switch08KPRG(value & 0x0F, 0x8000, true);
        break;
    }
    case 0x9000:
    case 0x9002:
    case 0x9001:
    case 0x9003:
    {
        switch (value & 0x3)
        {
        case 0:
            SwitchNMT(MIRRORING_Vert);
            break;
        case 1:
            SwitchNMT(MIRRORING_Horz);
            break;
        case 2:
            SwitchNMT(MIRRORING_OneScA);
            break;
        case 3:
            SwitchNMT(MIRRORING_OneScB);
            break;
        }
        break;
    }
    case 0xA000:
    case 0xA002:
    case 0xA001:
    case 0xA003:
    {
        Switch08KPRG(value & 0x0F, 0xA000, true);
        break;
    }
    case 0xB000:
    {
        chr_reg[0] = (chr_reg[0] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[0] >> 1, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB002:
    {
        chr_reg[0] = (chr_reg[0] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[0] >> 1, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB001:
    {
        chr_reg[1] = (chr_reg[1] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[1] >> 1, 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xB003:
    {
        chr_reg[1] = (chr_reg[1] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[1] >> 1, 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xC000:
    {
        chr_reg[2] = (chr_reg[2] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[2] >> 1, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC002:
    {
        chr_reg[2] = (chr_reg[2] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[2] >> 1, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC001:
    {
        chr_reg[3] = (chr_reg[3] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[3] >> 1, 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xC003:
    {
        chr_reg[3] = (chr_reg[3] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[3] >> 1, 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xD000:
    {
        chr_reg[4] = (chr_reg[4] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[4] >> 1, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD002:
    {
        chr_reg[4] = (chr_reg[4] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[4] >> 1, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD001:
    {
        chr_reg[5] = (chr_reg[5] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[5] >> 1, 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xD003:
    {
        chr_reg[5] = (chr_reg[5] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[5] >> 1, 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xE000:
    {
        chr_reg[6] = (chr_reg[6] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[6] >> 1, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xE002:
    {
        chr_reg[6] = (chr_reg[6] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[6] >> 1, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xE001:
    {
        chr_reg[7] = (chr_reg[7] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[7] >> 1, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xE003:
    {
        chr_reg[7] = (chr_reg[7] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[7] >> 1, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    }
}
void Mapper022::SaveState()
{
    Board::SaveState();
    for (int i=0; i<8; i++)
        state_str->Write(chr_reg[i]);
}
void Mapper022::LoadState()
{
    Board::LoadState();
    for (int i=0; i<8; i++)
        state_str->Read(chr_reg[i]);
}
}

