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
Mapper016::Mapper016()
{
    this->Name = "Bandai";
    this->MapperNumber = 16;

    eprom = new Eprom(256);
}

Mapper016::~Mapper016()
{
    delete [] eprom;
}
void Mapper016::HardReset()
{
    Board::HardReset();
// TODO: eprom initialization and hard reset
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    irq_enable = false;
    irq_counter = 0;
    eprom->HardReset();
}
void Mapper016::WriteSRM(uint16_t& address, uint8_t& value)
{
    this->WritePRG(address, value);
}
uint8_t Mapper016::ReadSRM(uint16_t& address)
{
    return eprom->Read(address);
}
void Mapper016::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0x000F)
    {
    case 0x0:
        Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    case 0x1:
        Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
        break;
    case 0x2:
        Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    case 0x3:
        Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
        break;
    case 0x4:
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x5:
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    case 0x6:
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x7:
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    case 0x8:
        Switch16KPRG(value, 0x8000, true);
        break;
    case 0x9:
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
    case 0xA:
    {
        irq_enable = (value & 0x1) == 0x1;
        dev_cpu->pin_irq &=IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xB:
    {
        irq_counter = (irq_counter & 0xFF00) | value;
        break;
    }
    case 0xC:
    {
        irq_counter = (irq_counter & 0x00FF) | (value << 8);
        break;
    }
    case 0xD:
        eprom->Write(address, value);
        break;
    }
}
void Mapper016::OnCPUClock()
{
    if (irq_enable)
    {
        irq_counter--;
        if (irq_counter == 0)
        {
            irq_counter = 0xFFFF;
            dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}
void Mapper016::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_enable);
    state_str->Write(irq_counter);
    eprom->SaveState();
}
void Mapper016::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_enable);
    state_str->Read(irq_counter);
    eprom->LoadState();
}
}



