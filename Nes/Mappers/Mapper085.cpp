
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
// TODO: Add support for VRC7 External sound channels.
Mapper085::Mapper085()
{
    this->Name = "VRC7";
    this->MapperNumber = 85;
}
void Mapper085::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    irq_reload = 0;
    prescaler = 341;
    irq_counter = 0;
    irq_mode_cycle = false;
    irq_enable = false;
    irq_enable_on_ak = false;
}
void Mapper085::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x8000:
        Switch08KPRG(value, 0x8000, true);
        break;
    case 0x8008:
    case 0x8010:
        Switch08KPRG(value, 0xA000, true);
        break;
    case 0x9000:
        Switch08KPRG(value, 0xC000, true);
        break;
    case 0xA000:
        Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    case 0xA008:
    case 0xA010:
        Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
        break;
    case 0xB000:
        Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    case 0xB008:
    case 0xB010:
        Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
        break;
    case 0xC000:
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0xC008:
    case 0xC010:
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    case 0xD000:
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0xD008:
    case 0xD010:
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    case 0xE000:
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
    case 0xE008:
    case 0xE010:
        irq_reload = value;
        break;
    case 0xF000:
    {
        irq_mode_cycle = (value & 0x4) == 0x4;
        irq_enable = (value & 0x2) == 0x2;
        irq_enable_on_ak = (value & 0x1) == 0x1;
        if (irq_enable)
        {
            irq_counter = irq_reload;
            prescaler = 341;
        }
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF008:
    case 0xF010:
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        irq_enable = irq_enable_on_ak;
        break;
    }
}
void Mapper085::OnCPUClock()
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
void Mapper085::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_reload);
    state_str->Write(irq_counter);
    state_str->Write(irq_mode_cycle);
    state_str->Write(irq_enable);
    state_str->Write(irq_enable_on_ak);
}
void Mapper085::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_reload);
    state_str->Read(irq_counter);
    state_str->Read(irq_mode_cycle);
    state_str->Read(irq_enable);
    state_str->Read(irq_enable_on_ak);
}
}


