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
// TODO: test mapper 50
Mapper050::Mapper050()
{
    this->Name = "FDS-Port - Alt. Levels";
    this->MapperNumber = 50;
}
void Mapper050::HardReset()
{
    Board::HardReset();
    Switch08KPRG(0xF, 0x6000, true);
    Switch08KPRG(0x8, 0x8000, true);
    Switch08KPRG(0x9, 0xA000, true);
    //Switch08KPRG(0x0, 0xC000, true);
    Switch08KPRG(0xB, 0xE000, true);
}
void Mapper050::WriteEXP(uint16_t& address, uint8_t& value)
{
    switch (address & 0x4120)
    {
    case 0x4020:
    {
        prg_page = (value & 0x8) | ((value & 0x1) << 2) | ((value >> 1) & 0x3);
        Switch08KPRG(prg_page, 0xC000, true);
        break;
    }
    case 0x4120:
    {
        irq_enabled = (value & 1) == 1;
        if (!irq_enabled)
        {
            irq_counter = 0;
            dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        }
        break;
    }
    }
}
void Mapper050::OnCPUClock()
{
    if (irq_enabled)
    {
        irq_counter++;
        if (irq_counter == 0x1000)
        {
            dev_cpu->pin_irq |=IRQ_FLAG_BOARD;
            irq_counter = 0;
        }
    }
}
void Mapper050::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_page);
    state_str->Write(irq_counter);
    state_str->Write(irq_enabled);
}
void Mapper050::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_page);
    state_str->Read(irq_counter);
    state_str->Read(irq_enabled);
}
}

