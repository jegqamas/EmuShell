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
// TODO: Mapper 90 DipSwitch not implemented, the irq modes 2-3 not implemented yet.
Mapper090::Mapper090()
{
    this->Name = "Pirate MMC5-style";
    this->MapperNumber = 90;
}
void Mapper090::HardReset()
{
    Board::HardReset();

    MAPPER90MODE = true;
    prg_mode = chr_mode = 0;
    for (int i = 0; i < 4; i++)
    {
        prg_reg[i] = i;
        nt_reg[i] = i;
    }
    for (int i = 0; i < 8; i++)
        chr_reg[i] = i;
    SetupPRG();
    SetupCHR();
    Dipswitch = 0;// ??
    irqCounter = 0;
    IrqEnable = false;
    irqCountDownMode = false;
    irqCountUpMode = false;
    irqFunkyMode = false;
    irqPrescalerSize = false;
    irqSource = 0;
    irqPrescaler = 0;
    irqPrescalerXOR = 0;
    irqFunkyModeReg = 0;
    RAM5803 = 0;
    flag_s = false;
    multiplication_a = 0;
    multiplication_b = 0;
    multiplication = 0;
}
void Mapper090::SoftReset()
{
    Board::SoftReset();
    if (Dipswitch == 0)
        Dipswitch = 0xFF;
    else
        Dipswitch = 0;
}
void Mapper090::WriteEXP(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x5800:
        multiplication_a = value;
        multiplication = (uint16_t)(multiplication_a * multiplication_b);
        break;
    case 0x5801:
        multiplication_b = value;
        multiplication = (uint16_t)(multiplication_a * multiplication_b);
        break;
    case 0x5803:
        RAM5803 = value;
        break;
    }
}
void Mapper090::WriteSRM(uint16_t& address, uint8_t& value)
{
// NO SRAM !!
}
void Mapper090::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xF007)
    {
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
    case 0x8004:
    case 0x8005:
    case 0x8006:
    case 0x8007:
        prg_reg[address & 0x3] = value & 0x7F;
        SetupPRG();
        break;
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0x9003:
    case 0x9004:
    case 0x9005:
    case 0x9006:
    case 0x9007:
        chr_reg[address & 0x7] = (chr_reg[address & 0x7] & 0xFF00) | value;
        SetupCHR();
        break;
    case 0xA000:
    case 0xA001:
    case 0xA002:
    case 0xA003:
    case 0xA004:
    case 0xA005:
    case 0xA006:
    case 0xA007:
        chr_reg[address & 0x7] = (chr_reg[address & 0x7] & 0x00FF) | (value << 8);
        SetupCHR();
        break;
    case 0xB000:
    case 0xB001:
    case 0xB002:
    case 0xB003:
        nt_reg[address & 0x3] = (nt_reg[address & 0x3] & 0xFF00) | value;
        break;
    case 0xB004:
    case 0xB005:
    case 0xB006:
    case 0xB007:
        nt_reg[address & 0x3] = (nt_reg[address & 0x3] & 0x00FF) | (value << 8);
        break;
    case 0xC000:
    {
        IrqEnable = (value & 1) == 1;
        if (!IrqEnable)
            dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xC001:
    {
        irqCountDownMode = (value & 0x80) == 0x80;
        irqCountUpMode = (value & 0x40) == 0x40;
        irqFunkyMode = (value & 0x8) == 0x8;
        irqPrescalerSize = (value & 0x4) == 0x4;
        irqSource = value & 3;
        break;
    }
    case 0xC002:
        IrqEnable = false;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    case 0xC003:
        IrqEnable = true;
        break;
    case 0xC004:
        irqPrescaler = value ^ irqPrescalerXOR;
        break;
    case 0xC005:
        irqCounter = value ^ irqPrescalerXOR;
        break;
    case 0xC006:
        irqPrescalerXOR = value;
        break;
    case 0xC007:
        irqFunkyModeReg = value;
        break;
    case 0xD000:
    {
        flag_s = (value & 0x80) == 0x80;
        prg_mode = value & 0x7;
        chr_mode = (value >> 3) & 0x3;
        nt_advanced_enable = (value & 0x20) == 0x20;
        nt_rom_only = (value & 0x40) == 0x40;
        SetupPRG();
        SetupCHR();
        break;
    }
    case 0xD001:
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
    case 0xD002:
    {
        nt_ram_select = (value & 0x80);
        break;
    }
    case 0xD003:
    {
        chr_m = (value & 0x80) == 0x80;
        chr_block_mode = (value & 0x20) == 0x20;
        chr_block = (value & 0x1F) << 8;
        SetupCHR();
        break;
    }
    }
}
void Mapper090::WriteNMT(uint16_t& address, uint8_t& value)
{
    if (MAPPER90MODE)
    {
        nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF] = value;
        return;
    }
    if (!nt_advanced_enable)
        nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF] = value;
    else
    {
        if (nt_rom_only)
        {
            // Do nothing ?
        }
        else
        {
            if ((nt_reg[(address >> 10) & 3] & 0x80) == nt_ram_select)
            {
                nmt_banks[nt_reg[(address >> 10) & 3] & 1][address & 0x3FF] = value;
            }
        }
    }
}
uint8_t Mapper090::ReadSRM(uint16_t& address)
{
    if (flag_s)
        return prg_banks[prg_indexes[0]][address & 0x1FFF];
    return 0;
}
uint8_t Mapper090::ReadEXP(uint16_t& address)
{
    switch (address)
    {
    case 0x5000:
        return Dipswitch;
    case 0x5800:
        return (uint8_t)(multiplication & 0x00FF);
    case 0x5801:
        return (uint8_t)((multiplication & 0xFF00) >> 8);
    case 0x5803:
        return RAM5803;
    }
    return 0;
}
uint8_t Mapper090::ReadNMT(uint16_t& address)
{
    if (MAPPER90MODE)
        return nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF];
    if (!nt_advanced_enable)
        return nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF];
    else
    {
        if (nt_rom_only)
            return chr_banks[nt_reg[(address >> 10) & 3] + chr_rom_bank_offset][address & 0x3FF];
        else
        {
            if ((nt_reg[(address >> 10) & 3] & 0x80) != nt_ram_select)
                return chr_banks[nt_reg[(address >> 10) & 3] + chr_rom_bank_offset][address & 0x3FF];
            else
                return nmt_banks[nt_reg[(address >> 10) & 3] & 1][address & 0x3FF];
        }
    }
}
void Mapper090::OnPPUAddressUpdate(uint16_t& address)
{
    if (irqSource == 1)
    {
        old_vram_address = new_vram_address;
        new_vram_address = address & 0x1000;
        if (old_vram_address < new_vram_address)
        {
            if (irqPrescalerSize)//3-bits
            {
                irqPrescaler = (irqPrescaler & 0xF8) | (((irqPrescaler & 0x7) + 1) & 0x7);
                if ((irqPrescaler & 0x7) == 0x7)
                {
                    ClockIRQCounter();
                }
            }
            else//8-bits
            {
                irqPrescaler = (uint8_t)(irqPrescaler + 1);
                if (irqPrescaler == 0xFF)
                {
                    ClockIRQCounter();
                }
            }
        }
    }
}
void Mapper090::OnCPUClock()
{
    if (irqSource == 0)
    {
        if (irqPrescalerSize)//3-bits
        {
            irqPrescaler = (irqPrescaler & 0xF8) | (((irqPrescaler & 0x7) + 1) & 0x7);
            if ((irqPrescaler & 0x7) == 0x7)
            {
                ClockIRQCounter();
            }
        }
        else//8-bits
        {
            irqPrescaler = (uint8_t)(irqPrescaler + 1);
            if (irqPrescaler == 0xFF)
            {
                ClockIRQCounter();
            }
        }
    }
}

void Mapper090::SetupPRG()
{
    switch (prg_mode)
    {
    case 0x0:
    {
        Switch08KPRG((prg_reg[3] * 4) + 3, 0x6000, true);
        Switch32KPRG(prg_32K_rom_mask, true);
        break;
    }
    case 0x1:
    {
        Switch08KPRG((prg_reg[3] * 2) + 1, 0x6000, true);
        Switch16KPRG(prg_reg[1], 0x8000, true);
        Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
        break;
    }
    case 0x2:
    {
        Switch08KPRG(prg_reg[3], 0x6000, true);
        Switch08KPRG(prg_reg[0], 0x8000, true);
        Switch08KPRG(prg_reg[1], 0xA000, true);
        Switch08KPRG(prg_reg[2], 0xC000, true);
        Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
        break;
    }
    case 0x3:
    {
        Switch08KPRG(ReverseByte(prg_reg[3]), 0x6000, true);
        Switch08KPRG(ReverseByte(prg_reg[0]), 0x8000, true);
        Switch08KPRG(ReverseByte(prg_reg[1]), 0xA000, true);
        Switch08KPRG(ReverseByte(prg_reg[2]), 0xC000, true);
        Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
        break;
    }
    case 0x4:
    {
        Switch08KPRG((prg_reg[3] * 4) + 3, 0x6000, true);
        Switch32KPRG(prg_reg[3], true);
        break;
    }
    case 0x5:
    {
        Switch08KPRG((prg_reg[3] * 2) + 1, 0x6000, true);
        Switch16KPRG(prg_reg[1], 0x8000, true);
        Switch16KPRG(prg_reg[3], 0xC000, true);
        break;
    }
    case 0x6:
    {
        Switch08KPRG(prg_reg[3], 0x6000, true);
        Switch08KPRG(prg_reg[0], 0x8000, true);
        Switch08KPRG(prg_reg[1], 0xA000, true);
        Switch08KPRG(prg_reg[2], 0xC000, true);
        Switch08KPRG(prg_reg[3], 0xE000, true);
        break;
    }
    case 0x7:
    {
        Switch08KPRG(ReverseByte(prg_reg[3]), 0x6000, true);
        Switch08KPRG(ReverseByte(prg_reg[0]), 0x8000, true);
        Switch08KPRG(ReverseByte(prg_reg[1]), 0xA000, true);
        Switch08KPRG(ReverseByte(prg_reg[2]), 0xC000, true);
        Switch08KPRG(ReverseByte(prg_reg[3]), 0xE000, true);
        break;
    }
    }
}
void Mapper090::SetupCHR()
{
    switch (chr_mode)
    {
    case 0x0:
    {
        if (chr_block_mode)
            Switch08KCHR(chr_reg[0], chr_01K_rom_count > 0);
        else
            Switch08KCHR((chr_reg[0] & 0x00FF) | chr_block, chr_01K_rom_count > 0);
        break;
    }
    case 0x1:
    {
        if (chr_block_mode)
        {
            Switch04KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
            Switch04KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
        }
        else
        {
            Switch04KCHR((chr_reg[0] & 0x00FF) | chr_block, 0x0000, chr_01K_rom_count > 0);
            Switch04KCHR((chr_reg[4] & 0x00FF) | chr_block, 0x1000, chr_01K_rom_count > 0);
        }
        break;
    }
    case 0x2:
    {
        if (chr_block_mode)
        {
            Switch02KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
            Switch02KCHR(chr_m ? chr_reg[0] : chr_reg[2], 0x0800, chr_01K_rom_count > 0);
            Switch02KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
            Switch02KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
        }
        else
        {
            Switch02KCHR((chr_reg[0] & 0x00FF) | chr_block, 0x0000, chr_01K_rom_count > 0);
            Switch02KCHR(((chr_m ? chr_reg[0] : chr_reg[2]) & 0x00FF) | chr_block, 0x0800, chr_01K_rom_count > 0);
            Switch02KCHR((chr_reg[4] & 0x00FF) | chr_block, 0x1000, chr_01K_rom_count > 0);
            Switch02KCHR((chr_reg[6] & 0x00FF) | chr_block, 0x1800, chr_01K_rom_count > 0);
        }
        break;
    }
    case 0x3:
    {
        if (chr_block_mode)
        {
            Switch01KCHR(chr_reg[0], 0x0000, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[1], 0x0400, chr_01K_rom_count > 0);
            Switch01KCHR(chr_m ? chr_reg[0] : chr_reg[2], 0x0800, chr_01K_rom_count > 0);
            Switch01KCHR(chr_m ? chr_reg[1] : chr_reg[3], 0x0C00, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[4], 0x1000, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[5], 0x1400, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[6], 0x1800, chr_01K_rom_count > 0);
            Switch01KCHR(chr_reg[7], 0x1C00, chr_01K_rom_count > 0);
        }
        else
        {
            Switch01KCHR((chr_reg[0] & 0x00FF) | chr_block, 0x0000, chr_01K_rom_count > 0);
            Switch01KCHR((chr_reg[1] & 0x00FF) | chr_block, 0x0400, chr_01K_rom_count > 0);
            Switch01KCHR(((chr_m ? chr_reg[0] : chr_reg[2]) & 0x00FF) | chr_block, 0x0800, chr_01K_rom_count > 0);
            Switch01KCHR(((chr_m ? chr_reg[1] : chr_reg[3]) & 0x00FF) | chr_block, 0x0C00, chr_01K_rom_count > 0);
            Switch01KCHR((chr_reg[4] & 0x00FF) | chr_block, 0x1000, chr_01K_rom_count > 0);
            Switch01KCHR((chr_reg[5] & 0x00FF) | chr_block, 0x1400, chr_01K_rom_count > 0);
            Switch01KCHR((chr_reg[6] & 0x00FF) | chr_block, 0x1800, chr_01K_rom_count > 0);
            Switch01KCHR((chr_reg[7] & 0x00FF) | chr_block, 0x1C00, chr_01K_rom_count > 0);
        }
        break;
    }
    }
}
uint8_t Mapper090::ReverseByte(uint8_t value)
{
    return (uint8_t)(((value & 0x40) >> 6) | ((value & 0x20) >> 4) | ((value & 0x10) >> 2)
                     | ((value & 0x8)) | ((value & 0x4) << 2) | ((value & 0x2) << 4) | ((value & 0x1) << 6));
}
void Mapper090::ClockIRQCounter()
{
    if (irqCountDownMode && irqCountUpMode)
        return;
    if (irqCountDownMode)
    {
        irqCounter--;
        if (irqCounter == 0)
        {
            irqCounter = 0xFF;
            if (IrqEnable)
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
    else if (irqCountUpMode)
    {
        irqCounter = (uint8_t)(irqCounter + 1);
        if (irqCounter == 0xFF)
        {
            irqCounter = 0;
            if (IrqEnable)
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}

void Mapper090::SaveState()
{
    Board::SaveState();
    for (int i = 0; i < 4; i++)
        state_str->Write(prg_reg[i]);
    for (int i = 0; i < 8; i++)
        state_str->Write(chr_reg[i]);
    for (int i = 0; i < 4; i++)
        state_str->Write(nt_reg[i]);
    state_str->Write(prg_mode);
    state_str->Write(chr_mode);
    state_str->Write(chr_block_mode);
    state_str->Write(chr_block);
    state_str->Write(chr_m);
    state_str->Write(flag_s);
    state_str->Write(irqCounter);
    state_str->Write(IrqEnable);
    state_str->Write(irqCountDownMode);
    state_str->Write(irqCountUpMode);
    state_str->Write(irqFunkyMode);
    state_str->Write(irqPrescalerSize);
    state_str->Write(irqSource);
    state_str->Write(irqPrescaler);
    state_str->Write(irqPrescalerXOR);
    state_str->Write(irqFunkyModeReg);
    state_str->Write(Dipswitch);
    state_str->Write(multiplication_a);
    state_str->Write(multiplication_b);
    state_str->Write(multiplication);
    state_str->Write(RAM5803);
    state_str->Write(nt_advanced_enable);
    state_str->Write(nt_rom_only);
    state_str->Write(nt_ram_select);
}
void Mapper090::LoadState()
{
    Board::LoadState();
    for (int i = 0; i < 4; i++)
        state_str->Read(prg_reg[i]);
    for (int i = 0; i < 8; i++)
        state_str->Read(chr_reg[i]);
    for (int i = 0; i < 4; i++)
        state_str->Read(nt_reg[i]);
    state_str->Read(prg_mode);
    state_str->Read(chr_mode);
    state_str->Read(chr_block_mode);
    state_str->Read(chr_block);
    state_str->Read(chr_m);
    state_str->Read(flag_s);
    state_str->Read(irqCounter);
    state_str->Read(IrqEnable);
    state_str->Read(irqCountDownMode);
    state_str->Read(irqCountUpMode);
    state_str->Read(irqFunkyMode);
    state_str->Read(irqPrescalerSize);
    state_str->Read(irqSource);
    state_str->Read(irqPrescaler);
    state_str->Read(irqPrescalerXOR);
    state_str->Read(irqFunkyModeReg);
    state_str->Read(Dipswitch);
    state_str->Read(multiplication_a);
    state_str->Read(multiplication_b);
    state_str->Read(multiplication);
    state_str->Read(RAM5803);
    state_str->Read(nt_advanced_enable);
    state_str->Read(nt_rom_only);
    state_str->Read(nt_ram_select);

}
}


