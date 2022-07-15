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
#include <stdio.h>
namespace NES
{
// TODO: Access Violation in mapper 18 (windows 10) !!??
Mapper018::Mapper018()
{
    this->Name = "Jaleco SS8806";
    this->MapperNumber = 18;
}
void Mapper018::HardReset()
{
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    for (int i=0; i<3; i++)
        prg_reg[i]=0;
    for (int i=0; i<8; i++)
        chr_reg[i]=0;
    irqRelaod=0;
    irqCounter=0;
    irqEnable=false;
    irqMask = 0x000F;
}
void Mapper018::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF003)
    {
    case 0x8000:
    {
        prg_reg[0] = (prg_reg[0] & 0xF0) | (value & 0x0F);
        Switch08KPRG(prg_reg[0], 0x8000, true);
        break;
    }
    case 0x8001:
    {
        prg_reg[0] = (prg_reg[0] & 0x0F) | ((value & 0x0F) << 4);
        Switch08KPRG(prg_reg[0], 0x8000, true);
        break;
    }
    case 0x8002:
    {
        prg_reg[1] = (prg_reg[1] & 0xF0) | (value & 0x0F);
        Switch08KPRG(prg_reg[1], 0xA000, true);
        break;
    }
    case 0x8003:
    {
        prg_reg[1] = (prg_reg[1] & 0x0F) | ((value & 0x0F) << 4);
        Switch08KPRG(prg_reg[1], 0xA000, true);
        break;
    }
    case 0x9000:
    {
        prg_reg[2] = (prg_reg[2] & 0xF0) | (value & 0x0F);
        Switch08KPRG(prg_reg[2], 0xC000, true);
        break;
    }
    case 0x9001:
    {
        prg_reg[2] = (prg_reg[2] & 0x0F) | ((value & 0x0F) << 4);
        Switch08KPRG(prg_reg[2], 0xC000, true);
        break;
    }
    case 0xA000:
    {
        chr_reg[0] = (chr_reg[0] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[0], 0, chr_01K_rom_count > 0);
        break;
    }
    case 0xA001:
    {
        chr_reg[0] = (chr_reg[0] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[0], 0, chr_01K_rom_count > 0);
        break;
    }
    case 0xA002:
    {
        chr_reg[1] = (chr_reg[1] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[1], 0x400, chr_01K_rom_count > 0);
        break;
    }
    case 0xA003:
    {
        chr_reg[1] = (chr_reg[1] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[1], 0x400, chr_01K_rom_count > 0);
        break;
    }
    case 0xB000:
    {
        chr_reg[2] = (chr_reg[2] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[2], 0x800, chr_01K_rom_count > 0);
        break;
    }
    case 0xB001:
    {
        chr_reg[2] = (chr_reg[2] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[2], 0x800, chr_01K_rom_count > 0);
        break;
    }
    case 0xB002:
    {
        chr_reg[3] = (chr_reg[3] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[3], 0xC00, chr_01K_rom_count > 0);
        break;
    }
    case 0xB003:
    {
        chr_reg[3] = (chr_reg[3] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[3], 0xC00, chr_01K_rom_count > 0);
        break;
    }
    case 0xC000:
    {
        chr_reg[4] = (chr_reg[4] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xC001:
    {
        chr_reg[4] = (chr_reg[4] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xC002:
    {
        chr_reg[5] = (chr_reg[5] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[5], 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xC003:
    {
        chr_reg[5] = (chr_reg[5] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[5], 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xD000:
    {
        chr_reg[6] = (chr_reg[6] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xD001:
    {
        chr_reg[6] = (chr_reg[6] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xD002:
    {
        chr_reg[7] = (chr_reg[7] & 0xF0) | (value & 0x0F);
        Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xD003:
    {
        chr_reg[7] = (chr_reg[7] & 0x0F) | ((value & 0x0F) << 4);
        Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    /*IRQ*/
    case 0xE000:
    {
        irqRelaod = (irqRelaod & 0xFFF0) | (value & 0x0F);
        break;
    }
    case 0xE001:
    {
        irqRelaod = (irqRelaod & 0xFF0F) | ((value & 0x0F) << 4);
        break;
    }
    case 0xE002:
    {
        irqRelaod = (irqRelaod & 0xF0FF) | ((value & 0x0F) << 8);
        break;
    }
    case 0xE003:
    {
        irqRelaod = (irqRelaod & 0x0FFF) | ((value & 0x0F) << 12);
        break;
    }

    case 0xF000:
    {
        irqCounter = irqRelaod;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF001:
    {
        irqEnable = (value & 1) == 1;
        if ((value & 0x8) == 0x8)
            irqMask = 0x000F;
        else if ((value & 0x4) == 0x4)
            irqMask = 0x00FF;
        else if ((value & 0x2) == 0x2)
            irqMask = 0x0FFF;
        else
            irqMask = 0xFFFF;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF002:
    {
        switch (value & 0x3)
        {
        case 0:
            SwitchNMT(MIRRORING_Horz);
            break;
        case 1:
            SwitchNMT(MIRRORING_Vert);
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
    }
}
void Mapper018::OnCPUClock()
{
    if (irqEnable)
    {
        if ((irqCounter & irqMask) > 0)
        {
            irqCounter--;
            if ((irqCounter & irqMask) == 0)
            {
                irqEnable = false;
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
            }
        }
    }
}
void Mapper018::SaveState()
{
    Board::SaveState();
    for (int i=0; i<3; i++)
        state_str->Write (prg_reg[i]);
    for (int i=0; i<8; i++)
        state_str->Write (chr_reg[i]);
    state_str->Write (irqRelaod);
    state_str->Write (irqCounter);
    state_str->Write (irqEnable);
    state_str->Write (irqMask);
}
void Mapper018::LoadState()
{
    Board::LoadState();
    for (int i=0; i<3; i++)
        state_str->Read (prg_reg[i]);
    for (int i=0; i<8; i++)
        state_str->Read (chr_reg[i]);
    state_str->Read (irqRelaod);
    state_str->Read (irqCounter);
    state_str->Read (irqEnable);
    state_str->Read (irqMask);
}
}

