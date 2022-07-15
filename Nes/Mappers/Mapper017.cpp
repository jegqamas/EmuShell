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
Mapper017::Mapper017()
{
    this->Name = "FFE F8xxx";
    this->MapperNumber = 17;
}
void Mapper017::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper017::WriteEXP(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x4501:
    {
        irqEnable = false;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0x4502:
    {
        irqCounter = (irqCounter & 0xFF00) | value;
        break;
    }
    case 0x4503:
    {
        irqEnable = true;
        irqCounter = (irqCounter & 0x00FF) | (value << 8);
        break;
    }
    case 0x4504:
        Switch08KPRG(value, 0x8000, true);
        break;
    case 0x4505:
        Switch08KPRG(value, 0xA000, true);
        break;
    case 0x4506:
        Switch08KPRG(value, 0xC000, true);
        break;
    case 0x4507:
        Switch08KPRG(value, 0xE000, true);
        break;
    case 0x4510:
        Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    case 0x4511:
        Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
        break;
    case 0x4512:
        Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    case 0x4513:
        Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
        break;
    case 0x4514:
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x4515:
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    case 0x4516:
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x4517:
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
}
void Mapper017::OnCPUClock()
{
    if (irqEnable)
    {
        irqCounter++;
        if (irqCounter >= 0xFFFF)
        {
            irqCounter = 0;
            dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}
void Mapper017::SaveState()
{
    Board::SaveState();
    state_str ->Write(irqEnable);
    state_str->Write(irqCounter);
}
void Mapper017::LoadState()
{
    Board::LoadState();
    state_str ->Read(irqEnable);
    state_str->Read(irqCounter);
}
}

