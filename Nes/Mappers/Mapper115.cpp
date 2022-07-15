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
#include <iostream>
#include "../../Definations.h"

namespace NES
{
Mapper115::Mapper115()
{
    this->Name = "MMC3 Cart Saint";
    this->MapperNumber = 115;
    // These are important for MMC4 irqs.
    this->enabled_ppuA12ToggleTimer=true;
    this->ppuA12TogglesOnRaisingEdge=true;
}
void Mapper115::HardReset()
{
    Board::HardReset();
    // Flags
    flag_o = flag_c = flag_p = false;
    address_8001 = 0;
    chr_block = 0;
    prg_secreg = 0;
    prg_reg[0] = 0;
    prg_reg[1] = 1;
    prg_reg[2] = prg_08K_rom_mask - 1;
    prg_reg[3] = prg_08K_rom_mask;
    SetupPRG();

    // CHR
    for (int i = 0; i < 6; i++)
        chr_reg[i] = 0;

    // IRQ
    irq_enabled = false;
    irq_counter = 0;
    irq_reload = 0xFF;
    old_irq_counter = 0;
    // mmc3_alt_behavior = false;
    irq_clear = false;

    // THIS IS NOT A HACK !
    // We need to determine the game's chip type to configure
    // the board correctly.
    if (NES::GameDBIndex >=0)
    {
        mmc3_alt_behavior = NesDB::ContainChip(NES::GameDBIndex,"MMC3A");
        if (mmc3_alt_behavior)
            std::cout << "MMC3: Chip is MMC3 A, MMC3 IQR mode switched to RevA\n";
        else
            std::cout << "MMC3: Chip is NOT MMC3 A, MMC3 IQR mode switched to RevB\n";
    }

    dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
}
void Mapper115::WriteSRM(uint16_t& address, uint8_t& value)
{
    switch (address & 0x6001)
    {
    case 0x6000:
    {
        flag_o = (value & 0x80) == 0x80;
        prg_secreg = value & 0xF;
        SetupPRG();
        break;
    }
    case 0x6001:
    {
        chr_block = (value & 1) << 8;
        SetupCHR();
        break;
    }
    }
}
void Mapper115::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE001)
    {
    case 0x8000:
    {
        address_8001 = value & 0x7;
        flag_c = (value & 0x80) != 0;
        flag_p = (value & 0x40) != 0;
        SetupCHR();
        SetupPRG();
        break;
    }
    case 0x8001:
    {
        switch (address_8001)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        {
            chr_reg[address_8001] = value;
            SetupCHR();
            break;
        }
        case 6:
        case 7:
        {
            prg_reg[address_8001 - 6] = value & prg_08K_rom_mask;
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
            SwitchNMT((value & 1) == 1 ? MIRRORING_Horz :  MIRRORING_Vert);
        }
        break;
    }
    case 0xA001:
    {
        TogglePRGRAMEnable((value & 0x80) != 0);
        TogglePRGRAMWritable((value & 0x40) == 0);
        break;
    }
    case 0xC000:
    {
        irq_reload = value;
        break;
    }
    case 0xC001:
    {
        if (mmc3_alt_behavior)
            irq_clear = true;
        irq_counter = 0;
        break;
    }
    case 0xE000:
    {
        irq_enabled = false;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xE001:
        irq_enabled = true;
        break;
    }
}
void Mapper115::SetupCHR()
{
    if (!flag_c)
    {
        Switch02KCHR((chr_reg[0] >> 1) | chr_block, 0x0000, chr_01K_rom_count > 0);
        Switch02KCHR((chr_reg[1] >> 1) | chr_block, 0x0800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[2] | chr_block, 0x1000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3] | chr_block, 0x1400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4] | chr_block, 0x1800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5] | chr_block, 0x1C00, chr_01K_rom_count > 0);
    }
    else
    {
        Switch02KCHR((chr_reg[0] >> 1) | chr_block, 0x1000, chr_01K_rom_count > 0);
        Switch02KCHR((chr_reg[1] >> 1) | chr_block, 0x1800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[2] | chr_block, 0x0000, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[3] | chr_block, 0x0400, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[4] | chr_block, 0x0800, chr_01K_rom_count > 0);
        Switch01KCHR(chr_reg[5] | chr_block, 0x0C00, chr_01K_rom_count > 0);
    }
}
void Mapper115::SetupPRG()
{
    if (!flag_o)
    {
        Switch08KPRG(prg_reg[flag_p ? 2 : 0], 0x8000, true);
        Switch08KPRG(prg_reg[1], 0xA000, true);
    }
    else
    {
        Switch16KPRG(prg_secreg, 0x8000, true);
    }
    Switch08KPRG(prg_reg[flag_p ? 0 : 2], 0xC000, true);
    Switch08KPRG(prg_reg[3], 0xE000, true);

}
void Mapper115::OnPPUA12RaisingEdge()
{
    old_irq_counter = irq_counter;

    if (irq_counter == 0 || irq_clear)
        irq_counter = irq_reload;
    else
        irq_counter --;

    if ((!mmc3_alt_behavior || old_irq_counter != 0 || irq_clear) && (irq_counter == 0) && irq_enabled)
    {
        dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
    }
    irq_clear = false;
}
void Mapper115::SaveState()
{
    Board::SaveState();

    state_str->Write(flag_c);
    state_str->Write(flag_p);
    state_str->Write(address_8001);

    for (int i=0; i<6; i++)
        state_str->Write(chr_reg[i]);

    for (int i=0; i<4; i++)
        state_str->Write( prg_reg[i]);
    // IRQ
    state_str->Write(irq_enabled);
    state_str->Write(irq_counter);
    state_str->Write(old_irq_counter);
    state_str->Write(irq_reload);
    state_str->Write(irq_clear);
    state_str->Write(mmc3_alt_behavior);
    state_str->Write(flag_o);
    state_str->Write(chr_block);
    state_str->Write(prg_secreg);
}
void Mapper115::LoadState()
{
    Board::LoadState();

    state_str->Read(flag_c);
    state_str->Read(flag_p);
    state_str->Read(address_8001);

    for (int i=0; i<6; i++)
        state_str->Read(chr_reg[i]);

    for (int i=0; i<4; i++)
        state_str->Read( prg_reg[i]);
    // IRQ
    state_str->Read(irq_enabled);
    state_str->Read(irq_counter);
    state_str->Read(old_irq_counter);
    state_str->Read(irq_reload);
    state_str->Read(irq_clear);
    state_str->Read(mmc3_alt_behavior);
    state_str->Read(flag_o);
    state_str->Read(chr_block);
    state_str->Read(prg_secreg);
}
}

