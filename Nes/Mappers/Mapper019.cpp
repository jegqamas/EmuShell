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
#include "../../Core.h"

namespace NES
{
// TODO: External sound channels are not implemented for mapper 19
Mapper019::Mapper019()
{
    this->Name = "Namcot 106";
    this->MapperNumber = 19;
}
void Mapper019::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    // This is not a hack, some games should be mapper 210 configured but assigned to mapper 19
    // TODO: find another way to setup Namcot 106 configurations for mapper 210 games
    switch (EmuShell::GameCRC32)
    {
    case 0x1DC0F740:// Wagyan Land 2
    case 0xD323B806:// Wagyan Land 3
    case 0xBD523011:// Dream Master
    case 0xC247CC80:// Family Circuit '91
    {
        enable_mirroring_switch = false;
        break;
    }
    }
}
void Mapper019::WriteEXP(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF800)
    {
// TODO: mapper 19: sound registers go here ...
    case 0x5000:
    {
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        irq_counter = (irq_counter & 0x7F00) | value;
        break;
    }
    case 0x5800:
    {
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        irq_counter = (irq_counter & 0x00FF) | ((value & 0x7F) << 8);
        irq_enable = (value & 0x80) == 0x80;
        break;
    }
    }
}
void Mapper019::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF800)
    {
    case 0x8000:
    {
        if (!disables_chr_ram_A)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x0000, value < 0xE0);
        else
            Switch01KCHR(value, 0x0000, true);
        break;
    }
    case 0x8800:
    {
        if (!disables_chr_ram_A)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x0400, value < 0xE0);
        else
            Switch01KCHR(value, 0x0400, true);
        break;
    }
    case 0x9000:
    {
        if (!disables_chr_ram_A)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x0800, value < 0xE0);
        else
            Switch01KCHR(value, 0x0800, true);
        break;
    }
    case 0x9800:
    {
        if (!disables_chr_ram_A)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x0C00, value < 0xE0);
        else
            Switch01KCHR(value, 0x0C00, true);
        break;
    }
    case 0xA000:
    {
        if (!disables_chr_ram_B)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x1000, value < 0xE0);
        else
            Switch01KCHR(value, 0x1000, true);
        break;
    }
    case 0xA800:
    {
        if (!disables_chr_ram_B)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x1400, value < 0xE0);
        else
            Switch01KCHR(value, 0x1400, true);
        break;
    }
    case 0xB000:
    {
        if (!disables_chr_ram_B)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x1800, value < 0xE0);
        else
            Switch01KCHR(value, 0x1800, true);
        break;
    }
    case 0xB800:
    {
        if (!disables_chr_ram_B)
            Switch01KCHR((value >= 0xE0) ? (value - 0xE0) : value, 0x1C00, value < 0xE0);
        else
            Switch01KCHR(value, 0x1C00, true);
        break;
    }
    case 0xC000:
    {
        if (enable_mirroring_switch)
        {
            nmt_indexes[0] = value;
        }
        break;
    }
    case 0xC800:
    {
        if (enable_mirroring_switch)
        {
            nmt_indexes[1] = value;
        }
        break;
    }
    case 0xD000:
    {
        if (enable_mirroring_switch)
        {
            nmt_indexes[2] = value;
        }
        break;
    }
    case 0xD800:
    {
        if (enable_mirroring_switch)
        {
            nmt_indexes[3] = value;
        }
        break;
    }
    case 0xE000:
        Switch08KPRG(value & 0x3F, 0x8000, true);
        break;
    case 0xE800:
    {
        Switch08KPRG(value & 0x3F, 0xA000, true);
        disables_chr_ram_A = (value & 0x40) == 0x40;
        disables_chr_ram_B = (value & 0x80) == 0x80;
        break;
    }
    case 0xF000:
        Switch08KPRG(value & 0x3F, 0xC000, true);
        break;
    }
}
uint8_t Mapper019::ReadEXP(uint16_t& address)
{
    switch (address & 0xF800)
    {
    case 0x5000:
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        return (irq_counter & 0x00FF);
    case 0x5800:
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        return ((irq_enable ? 0x80 : 0x00) | ((irq_counter & 0x7F00) >> 8));
    }
    return 0;
}
void Mapper019::WriteNMT(uint16_t& address, uint8_t& value)
{
    if (enable_mirroring_switch)
    {
        temp_nmt = nmt_indexes[(address >> 10) & 0x3];
        if (temp_nmt >= 0xE0)
            nmt_banks[(temp_nmt - 0xE0) & 0x1][address & 0x3FF] = value;
    }
    else
        Board::WriteNMT(address, value);
}
uint8_t Mapper019::ReadNMT(uint16_t& address)
{
    if (enable_mirroring_switch)
    {
        temp_nmt = nmt_indexes[(address >> 10) & 0x3];
        if (temp_nmt >= 0xE0)
            return nmt_banks[(temp_nmt - 0xE0) & 0x1][address & 0x3FF];
        else
            return chr_banks[temp_nmt + chr_rom_bank_offset][address & 0x3FF];
    }
    else
        return Board::ReadNMT(address);
}
void Mapper019::OnCPUClock()
{
    if (irq_enable)
    {
        if (irq_counter == 0x7FFF)
        {
            dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
            irq_counter = 0;
        }
        else
        {
            irq_counter++;
        }
    }
}
void Mapper019::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_counter);
    state_str->Write(irq_enable);
    state_str->Write(disables_chr_ram_A);
    state_str->Write(disables_chr_ram_B);
    state_str->Write(enable_mirroring_switch);
    state_str->Write(temp_nmt);
    state_str->Write(temp_val);
    state_str->Write(temp_i);
}
void Mapper019::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_counter);
    state_str->Read(irq_enable);
    state_str->Read(disables_chr_ram_A);
    state_str->Read(disables_chr_ram_B);
    state_str->Read(enable_mirroring_switch);
    state_str->Read(temp_nmt);
    state_str->Read(temp_val);
    state_str->Read(temp_i);
}
}

