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
Mapper067::Mapper067()
{
    this->Name = "Sunsoft 3";
    this->MapperNumber = 67;
}
void Mapper067::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    irq_enabled = false;
    irq_counter = 0xFFFF;
    odd = false;
}
void Mapper067::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF800)
    {
    case 0x8800:
    {
        Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0x9800:
    {
        Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xA800:
    {
        Switch02KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xB800:
    {
        Switch02KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xC800:
    {
        if (!odd)
            irq_counter = (irq_counter & 0x00FF) | (value << 8);
        else
            irq_counter = (irq_counter & 0xFF00) | value;
        odd = !odd;
        break;
    }
    case 0xD800:
    {
        irq_enabled = (value & 0x10) == 0x10;
        odd = false;
        dev_cpu ->pin_irq &=IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xE800:
    {
        switch (value & 3)
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
    case 0xF800:
    {
        Switch16KPRG(value, 0x8000, true);
        break;
    }
    }
}
void Mapper067::OnCPUClock()
{
    if (irq_enabled)
    {
        irq_counter--;
        if (irq_counter == 0)
        {
            irq_counter = 0xFFFF;
            dev_cpu ->pin_irq |=IRQ_FLAG_BOARD;
            irq_enabled = false;
        }
    }
}
void Mapper067::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_enabled);
    state_str->Write(irq_counter);
    state_str->Write(odd);
}
void Mapper067::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_enabled);
    state_str->Read(irq_counter);
    state_str->Read(odd);
}
}

