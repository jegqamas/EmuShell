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
Mapper073::Mapper073()
{
    this->Name = "VRC3";
    this->MapperNumber = 73;
}
void Mapper073::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    irq_mode_8 = false;
    irq_enable = false;
    irq_enable_on_ak = false;
    irq_reload = 0;
    irq_counter = 0;
}
void Mapper073::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF000)
    {
    case 0x8000:
        irq_reload = (irq_reload & 0xFFF0) | (value & 0xF);
        break;
    case 0x9000:
        irq_reload = (irq_reload & 0xFF0F) | ((value & 0xF) << 4);
        break;
    case 0xA000:
        irq_reload = (irq_reload & 0xF0FF) | ((value & 0xF) << 8);
        break;
    case 0xB000:
        irq_reload = (irq_reload & 0x0FFF) | ((value & 0xF) << 12);
        break;
    case 0xC000:
    {
        irq_mode_8 = (value & 0x4) == 0x4;
        irq_enable = (value & 0x2) == 0x2;
        irq_enable_on_ak = (value & 0x1) == 0x1;
        if (irq_enable)
            irq_counter = irq_reload;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xD000:
    {
        irq_enable = irq_enable_on_ak;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF000:
        Switch16KPRG(value & 0xF, 0x8000, true);
        break;

    }
}
void Mapper073::OnCPUClock()
{
    if (irq_enable)
    {
        if (irq_mode_8)
        {
            irq_counter = irq_counter & 0xFF00 | ((irq_counter & 0xFF) + 1);
            if ((irq_counter & 0xFF) == 0xFF)
            {
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
                irq_counter = (irq_counter & 0xFF00) | (irq_reload & 0xFF);
            }
        }
        else
        {
            irq_counter++;
            if (irq_counter == 0xFFFF)
            {
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
                irq_counter = irq_reload;
            }
        }
    }
}
void Mapper073::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_mode_8);
    state_str->Write(irq_enable);
    state_str->Write( irq_enable_on_ak);
    state_str->Write( irq_reload);
    state_str->Write(irq_counter);
}
void Mapper073::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_mode_8);
    state_str->Read(irq_enable);
    state_str->Read( irq_enable_on_ak);
    state_str->Read( irq_reload);
    state_str->Read(irq_counter);
}
}



