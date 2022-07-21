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
Mapper025::Mapper025()
{
    this->Name = "VRC4";
    this->MapperNumber = 25;
}
void Mapper025::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    prescaler = 341;
    for (int i=0; i<8; i++)
        chr_reg[i]=0;
}
void Mapper025::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x8000:
    case 0x8002:
    case 0x8001:
    case 0x8003:
    case 0x8008:
    case 0x8004:
    case 0x800C:
    {
        prg_reg0 = value& 0x1F;
        Switch08KPRG(prg_mode ? (prg_08K_rom_mask - 1) : prg_reg0, 0x8000, true);
        Switch08KPRG(prg_mode ? prg_reg0 : (prg_08K_rom_mask - 1), 0xC000, true);
        break;
    }
    case 0x9000:
    case 0x9002:
    case 0x9008:
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
    case 0x9001:
    case 0x9003:
    case 0x9004:
    case 0x900C:
    {
        prg_mode = (value & 0x2) == 0x2;
        Switch08KPRG(prg_mode ? (prg_08K_rom_mask - 1) : prg_reg0, 0x8000, true);
        Switch08KPRG(prg_mode ? prg_reg0: (prg_08K_rom_mask - 1), 0xC000, true);
        break;
    }

    case 0xA000:
    case 0xA002:
    case 0xA001:
    case 0xA003:
    case 0xA008:
    case 0xA004:
    case 0xA00C:
    {
        Switch08KPRG(value & 0x1F, 0xA000, true);
        break;
    }
    case 0xB000:
    {
        chr_reg[0] = (chr_reg[0] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
        break;
    }

    case 0xB002:
    case 0xB008:
    {
        chr_reg[0] = (chr_reg[0] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB001:
    case 0xB004:
    {
        chr_reg[1] = (chr_reg[1] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[1], 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xB003:
    case 0xB00C:
    {
        chr_reg[1] = (chr_reg[1] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[1], 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xC000:
    {
        chr_reg[2] = (chr_reg[2] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[2], 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC002:
    case 0xC008:
    {
        chr_reg[2] = (chr_reg[2] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[2], 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC001:
    case 0xC004:
    {
        chr_reg[3] = (chr_reg[3] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[3], 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xC003:
    case 0xC00C:
    {
        chr_reg[3] = (chr_reg[3] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[3], 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xD000:
    {
        chr_reg[4] = (chr_reg[4] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD002:
    case 0xD008:
    {
        chr_reg[4] = (chr_reg[4] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD001:
    case 0xD004:
    {
        chr_reg[5] = (chr_reg[5] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[5], 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xD003:
    case 0xD00C:
    {
        chr_reg[5] = (chr_reg[5] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[5], 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xE000:
    {
        chr_reg[6] = (chr_reg[6] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xE002:
    case 0xE008:
    {
        chr_reg[6] = (chr_reg[6] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xE001:
    case 0xE004:
    {
        chr_reg[7] = (chr_reg[7] & 0xF0) | (value & 0xF);
        Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xE003:
    case 0xE00C:
    {
        chr_reg[7] = (chr_reg[7] & 0x0F) | ((value & 0xF) << 4);
        Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xF000:
    {
        irq_reload = (irq_reload & 0xF0) | (value & 0xF);
        break;
    }
    case 0xF002:
    case 0xF008:
    {
        irq_reload = (irq_reload & 0x0F) | ((value & 0xF) << 4);
        break;
    }
    case 0xF001:
    case 0xF004:
    {
        irq_mode_cycle = (value & 0x4) == 0x4;
        irq_enable = (value & 0x2) == 0x2;
        irq_enable_on_ak = (value & 0x1) == 0x1;
        if (irq_enable)
        {
            irq_counter = irq_reload;
            prescaler = 341;
        }
        dev_cpu ->pin_irq &= IRQ_FLAG_BOARD_OFF;

        break;
    }

    case 0xF003:
    case 0xF00C:
    {
        dev_cpu ->pin_irq &= IRQ_FLAG_BOARD_OFF;
        irq_enable = irq_enable_on_ak;
        break;
    }
    }
}
void Mapper025::OnCPUClock()
{
    if (irq_enable)
    {
        if (!irq_mode_cycle)
        {
            if (prescaler > 0)
                prescaler -= 3;
            else
            {
                prescaler += 341;
                irq_counter++;
                if (irq_counter == 0xFF)
                {
                    dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
                    irq_counter = irq_reload;
                }
            }
        }
        else
        {
            irq_counter++;
            if (irq_counter == 0xFF)
            {
                dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
                irq_counter = irq_reload;
            }
        }
    }
}
void Mapper025::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_mode);
    state_str->Write(prg_reg0);
    for (int i=0; i<8; i++)
        state_str->Write(chr_reg[i]);
    state_str->Write( irq_reload);
    state_str->Write(irq_counter);
    state_str->Write(irq_mode_cycle);
    state_str->Write(irq_enable);
    state_str->Write(irq_enable_on_ak);
}
void Mapper025::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_mode);
    state_str->Read(prg_reg0);
    for (int i=0; i<8; i++)
        state_str->Read(chr_reg[i]);
    state_str->Read( irq_reload);
    state_str->Read(irq_counter);
    state_str->Read(irq_mode_cycle);
    state_str->Read(irq_enable);
    state_str->Read(irq_enable_on_ak);
}
}
