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
Mapper006::Mapper006()
{
    this->Name = "FFE F4xxx";
    this->MapperNumber = 6;
}
void Mapper006::HardReset()
{
    Board::HardReset();
    Switch16KPRG(7, 0xC000, true);
}
void Mapper006::WriteEXP(uint16_t& address, uint8_t& value)
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
    }
}
void Mapper006::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch08KCHR(value & 3, chr_01K_rom_count > 0);
    Switch16KPRG((value >> 2) & 0xF, 0x8000, true);
}
void Mapper006::OnCPUClock()
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
void Mapper006::SaveState()
{
    Board::SaveState();
    state_str ->Write(irqEnable);
    state_str->Write(irqCounter);
}
void Mapper006::LoadState()
{
    Board::LoadState();
    state_str ->Read(irqEnable);
    state_str->Read(irqCounter);
}
}
