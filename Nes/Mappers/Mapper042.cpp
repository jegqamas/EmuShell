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
// TODO: Test mapper 42
Mapper042::Mapper042()
{
    this->Name = "Mario Baby";
    this->MapperNumber = 42;
}
void Mapper042::HardReset()
{
    Board::HardReset();
    Switch32KPRG(prg_32K_rom_mask, true);
    SRAM_PRG_Page=0;
    irqEnable=false;
    irqCounter=0;
}
void Mapper042::WritePRG(uint16_t& address, uint8_t& value)
{
    if (address == 0x8000)
    {
        Switch08KCHR(value, chr_01K_rom_count > 0);
    }
    else if (address == 0xF000)
    {
        SRAM_PRG_Page = value << 13;
    }
    else
    {
        switch (address & 0xE003)
        {
        case 0xE000:
        {
            Switch08KPRG(value, 0x6000, true);
            break;
        }
        case 0xE001:
        {
            if ((value & 0x8) == 0x8)
                SwitchNMT(MIRRORING_Horz);
            else
                SwitchNMT(MIRRORING_Vert);
            break;
        }

        case 0xE002:
        {
            irqEnable = (value & 2) == 2;
            if (!irqEnable)
                irqCounter = 0;
            dev_cpu ->pin_irq &= IRQ_FLAG_BOARD_OFF;
            break;
        }
        }
    }
}
void Mapper042::OnCPUClock()
{
    if (irqEnable)
    {
        int prev = irqCounter++;

        if ((irqCounter & 0x6000) != (prev & 0x6000))
        {
            if ((irqCounter & 0x6000) == 0x6000)
                dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
            else
                dev_cpu ->pin_irq &= IRQ_FLAG_BOARD_OFF;
        }
    }
}
void Mapper042::SaveState()
{
    Board::SaveState();
    state_str->Write(SRAM_PRG_Page);
    state_str->Write(irqEnable);
    state_str->Write(irqCounter);
}
void Mapper042::LoadState()
{
    Board::LoadState();
    state_str->Read(SRAM_PRG_Page);
    state_str->Read(irqEnable);
    state_str->Read(irqCounter);
}
}

