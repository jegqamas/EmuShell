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
Mapper048::Mapper048()
{
    this->Name = "Taito TC0190/TC0350";
    this->MapperNumber = 48;
    // These are important for MMC4 irqs.
    this->enabled_ppuA12ToggleTimer=true;
    this->ppuA12TogglesOnRaisingEdge=true;
}
void Mapper048::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    // This is not a hack, some games are mapper 33 and assigned as mapper 48
    // We need to confirm which type given game it is ...
    MODE = false;// Set as mapper 48 mode [board TC0190XXX]
    if (GameDBIndex>=0)
    {
        if (NesDB::ContainChip(GameDBIndex,"TC0350"))
        {
            // Board TC0350XXX mode, mapper 33 ....
            MODE = true;
        }
    }
    // IRQ
    irq_enabled = false;
    irq_counter = 0;
    irq_reload = 0xFF;
    old_irq_counter = 0;
    mmc3_alt_behavior = false;
    irq_clear = false;
}
void Mapper048::WritePRG(uint16_t& address, uint8_t& value)
{
    if (!MODE)
    {
        // Mapper 48 mode
        switch (address & 0xE003)
        {
        case 0x8000:
        {
            Switch08KPRG(value, 0x8000, true);
            break;
        }
        case 0x8001:
        {
            Switch08KPRG(value, 0xA000, true);
            break;
        }
        case 0x8002:
        {
            Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
            break;
        }
        case 0x8003:
        {
            Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
            break;
        }
        case 0xA000:
        {
            Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        }
        case 0xA001:
        {
            Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
            break;
        }
        case 0xA002:
        {
            Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
            break;
        }
        case 0xA003:
        {
            Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
            break;
        }
        case 0xC000:
        {
            irq_reload = (uint8_t)(value ^ 0xFF);
            break;
        }
        case 0xC001:
        {
            if (mmc3_alt_behavior) irq_clear = true;
            irq_counter = 0;
            break;
        }
        case 0xC002:
        {
            irq_enabled = false;
            dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
            break;
        }
        case 0xC003:
        {
            irq_enabled = true;
            break;
        }
        case 0xE000:
        {
            SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Horz : MIRRORING_Vert);
            break;
        }
        }
    }
    else
    {
        // Mapper 33 mode
        switch (address & 0xA003)
        {
        case 0x8000:
        {
            SwitchNMT((value & 0x40) == 0x40 ? MIRRORING_Horz : MIRRORING_Vert);
            Switch08KPRG((value & 0x3F), 0x8000, true);
            break;
        }
        case 0x8001:
        {
            Switch08KPRG((value & 0x3F), 0xA000, true);
            break;
        }
        case 0x8002:
        {
            Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
            break;
        }
        case 0x8003:
        {
            Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
            break;
        }
        case 0xA000:
        {
            Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        }
        case 0xA001:
        {
            Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
            break;
        }
        case 0xA002:
        {
            Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
            break;
        }
        case 0xA003:
        {
            Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
            break;
        }
        }
    }
}
void Mapper048::OnPPUA12RaisingEdge()
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
void Mapper048::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_enabled);
    state_str->Write(irq_counter);
    state_str->Write(old_irq_counter);
    state_str->Write(irq_reload);
    state_str->Write(irq_clear);
    state_str->Write(mmc3_alt_behavior);
}
void Mapper048::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_enabled);
    state_str->Read(irq_counter);
    state_str->Read(old_irq_counter);
    state_str->Read(irq_reload);
    state_str->Read(irq_clear);
    state_str->Read(mmc3_alt_behavior);
}
}

