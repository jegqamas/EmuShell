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
Mapper065::Mapper065()
{
    this->Name = "Irem H-3001";
    this->MapperNumber = 65;
}
void Mapper065::HardReset()
{
    Board::HardReset();
    Switch08KPRG(0x00, 0x8000, true);
    Switch08KPRG(0x01, 0xA000, true);
    Switch08KPRG(0xFE, 0xC000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper065::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x8000:
    {
        Switch08KPRG(value, 0x8000, true);
        break;
    }
    case 0x9001:
    {
        SwitchNMT((value & 0x80) == 0x80 ? MIRRORING_Horz : MIRRORING_Vert);
        break;
    }
    case 0x9003:
    {
        irq_enable = (value & 0x80) == 0x80;
        dev_cpu->pin_irq &=IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0x9004:
    {
        irq_counter = irq_reload;
        dev_cpu->pin_irq &=IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0x9005:
    {
        irq_reload = (irq_reload & 0x00FF) | (value << 8);
        break;
    }
    case 0x9006:
    {
        irq_reload = (irq_reload & 0xFF00) | value;
        break;
    }
    case 0xA000:
    {
        Switch08KPRG(value, 0xA000, true);
        break;
    }
    case 0xC000:
    {
        Switch08KPRG(value, 0xC000, true);
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
void Mapper065::OnCPUClock()
{
    if (irq_enable)
    {
        if (irq_counter > 0)
            irq_counter--;
        else if (irq_counter == 0)
        {
            irq_counter = -1;
            dev_cpu->pin_irq |=IRQ_FLAG_BOARD;
        }
    }
}
void Mapper065::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_enable);
    state_str->Write(irq_reload);
    state_str->Write(irq_counter);
}
void Mapper065::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_enable);
    state_str->Read(irq_reload);
    state_str->Read(irq_counter);
}
}

