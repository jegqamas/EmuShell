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
    // TODO: Mapper 64: Road Runner hangs ..
Mapper064::Mapper064()
{
    this->Name = "Tengen RAMBO-1";
    this->MapperNumber = 64;
    // These are important for MMC4 irqs.
    this->enabled_ppuA12ToggleTimer=true;
    this->ppuA12TogglesOnRaisingEdge=true;
}
void Mapper064::HardReset()
{
    Board::HardReset();
    // Flags
    flag_c = flag_p = flag_k = false;
    address_8001 = 0;

    prg_reg[0] = 0;
    prg_reg[1] = 1;
    prg_reg[2] = 2;
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    SetupPRG();
    // CHR
    for (int i = 0; i < 8; i++)
        chr_reg[i] = i;
    SetupCHR();

    // IRQ
    irq_enabled = false;
    irq_counter = 0;
    irq_prescaler = 0;
    irq_mode = false;
    irq_reload = 0xFF;
    irq_clear = false;
}
void Mapper064::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE001)
    {
    case 0x8000:
    {
        address_8001 = value & 0xF;
        flag_c = (value & 0x80) != 0;
        flag_p = (value & 0x40) != 0;
        flag_k = (value & 0x20) != 0;
        SetupCHR();
        SetupPRG();
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        {
            chr_reg[address_8001] = value;
            SetupCHR();
            break;
        }
        case 0x6:
        case 0x7:
        {
            prg_reg[address_8001 - 6] = value;
            SetupPRG();
            break;
        }
        case 0x8:
        {
            chr_reg[6] = value;
            SetupCHR();
            break;
        }
        case 0x9:
        {
            chr_reg[7] = value;
            SetupCHR();
            break;
        }
        case 0xF:
        {
            prg_reg[2] = value;
            SetupPRG();
            break;
        }
        }
        break;
    }
    case 0xA000:
    {
        if (default_mirroring != MIRRORING_Full)
        {
            SwitchNMT((value & 1) == 1 ? MIRRORING_Horz : MIRRORING_Vert);
        }
        break;
    }
    case 0xC000:
    {
        irq_reload = value;
        break;
    }
    case 0xC001:
    {
        irq_mode = (value & 0x1) == 0x1;
        irq_clear = true;
        irq_prescaler = 0;
        break;
    }
    case 0xE000:
    {
        irq_enabled = false;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xE001:
    {
        irq_enabled = true;
        break;
    }
    }
}
void Mapper064::SetupCHR()
{
    if (!flag_c)
    {
        if (!flag_k)
        {
            Switch02KCHR(chr_reg[0] >> 1, 0x0000, chr_01K_rom_count > 0);
            Switch02KCHR(chr_reg[1] >> 1, 0x0800, chr_01K_rom_count > 0);
        }
        else
        {
            Switch01KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[6], 0x0400, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[1], 0x0800, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[7], 0x0C00, chr_01K_rom_count > 0);
        }
        Switch01KCHR(chr_reg[2], 0x1000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3], 0x1400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4], 0x1800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5], 0x1C00, chr_01K_rom_count > 0);
    }
    else
    {
        if (!flag_k)
        {
            Switch02KCHR(chr_reg[0] >> 1, 0x1000, chr_01K_rom_count > 0);
            Switch02KCHR(chr_reg[1] >> 1, 0x1800, chr_01K_rom_count > 0);
        }
        else
        {
            Switch01KCHR(chr_reg[0], 0x1000, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[6], 0x1400, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[1], 0x1800, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        }
        Switch01KCHR(chr_reg[2], 0x0000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3], 0x0400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4], 0x0800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5], 0x0C00, chr_01K_rom_count > 0);
    }
}
void Mapper064::SetupPRG()
{
    Switch08KPRG(prg_reg[flag_p ? 2 : 0], 0x8000, true);
    Switch08KPRG(prg_reg[flag_p ? 0 : 1], 0xA000, true);
    Switch08KPRG(prg_reg[flag_p ? 1 : 2], 0xC000, true);
}
void Mapper064::OnPPUA12RaisingEdge()
{
    ClockIRQ();
}
void Mapper064::OnCPUClock()
{
    if (irq_mode)
    {
        irq_prescaler++;
        if (irq_prescaler == 4)
        {
            irq_prescaler = 0;
            ClockIRQ();
        }
    }
}
void Mapper064::ClockIRQ()
{
    if (irq_clear)
    {
        irq_counter = (uint8_t)(irq_reload + 1);
        irq_clear = false;
    }
    else
    {
        if (irq_counter == 0)
        {
            irq_counter = irq_reload;
        }
        else
        {
            if (--irq_counter == 0 && irq_enabled)
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}
void Mapper064::SaveState()
{
    Board::SaveState();
    state_str->Write(address_8001);
    for (int i=0; i<8; i++)
        state_str->Write(chr_reg[i]);
    for (int i=0; i<3; i++)
        state_str->Write(prg_reg[i]);
    // IRQ
    state_str->Write(irq_enabled);
    state_str->Write(irq_counter);
    state_str->Write(irq_reload);
    state_str->Write(irq_mode);
    state_str->Write(irq_clear);
    state_str->Write(irq_prescaler);
}
void Mapper064::LoadState()
{
    Board::LoadState();
    state_str->Read(address_8001);
    for (int i=0; i<8; i++)
        state_str->Read(chr_reg[i]);
    for (int i=0; i<3; i++)
        state_str->Read(prg_reg[i]);
    // IRQ
    state_str->Read(irq_enabled);
    state_str->Read(irq_counter);
    state_str->Read(irq_reload);
    state_str->Read(irq_mode);
    state_str->Read(irq_clear);
    state_str->Read(irq_prescaler);
}
}

