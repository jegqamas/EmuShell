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
#include "../../Core.h"

namespace NES
{
Mapper032::Mapper032()
{
    this->Name = "Irem G-101";
    this->MapperNumber = 32;
}
void Mapper032::HardReset()
{
    Board::HardReset();
    enable_mirroring_switch = true;
    // This is not a hack !! this is from mapper docs v0.6.1 by Disch, 032.txt:
    /*"Major League seems to want hardwired 1-screen mirroring.  As far as I know, there is no seperate mapper
       number assigned to address this issue, so you'll have to rely on a CRC or hash check or something for
       treating Major League as a special case."
     */
    if (EmuShell::GameCRC32 == 0x243A8735)
    {
        enable_mirroring_switch = false;
        SwitchNMT(MIRRORING_OneScA);
    }

    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper032::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF007)
    {
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
    case 0x8004:
    case 0x8005:
    case 0x8006:
    case 0x8007:
    {
        prg_reg0 = value;
        Switch08KPRG(prg_mode ? 0 : prg_reg0, 0x8000, true);
        Switch08KPRG(prg_mode ? prg_reg0 : (prg_08K_rom_mask - 1), 0xC000, true);
        break;
    }
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0x9003:
    case 0x9004:
    case 0x9005:
    case 0x9006:
    case 0x9007:
    {
        prg_mode = (value & 0x2) == 0x2;
        Switch08KPRG(prg_mode ? 0 : prg_reg0, 0x8000, true);
        Switch08KPRG(prg_mode ? prg_reg0 : (prg_08K_rom_mask - 1), 0xC000, true);
        if (enable_mirroring_switch)
            SwitchNMT((value & 1) == 1 ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
    case 0xA000:
    case 0xA001:
    case 0xA002:
    case 0xA003:
    case 0xA004:
    case 0xA005:
    case 0xA006:
    case 0xA007:
    {
        Switch08KPRG(value, 0xA000, true);
        break;
    }
    case 0xB000:
    {
        Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB001:
    {
        Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xB002:
    {
        Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xB003:
    {
        Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xB004:
    {
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB005:
    {
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xB006:
    {
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xB007:
    {
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    }
}
void Mapper032::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_mode);
    state_str->Write( prg_reg0);
}
void Mapper032::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_mode);
    state_str->Read( prg_reg0);
}
}

