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
Mapper091::Mapper091()
{
    this->Name = "HK-SF3";
    this->MapperNumber = 91;
    // These are important for MMC4 irqs.
    this->enabled_ppuA12ToggleTimer=true;
    this->ppuA12TogglesOnRaisingEdge=true;
}
void Mapper091::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
}
void Mapper091::WriteSRM(uint16_t& address, uint8_t& value)
{
    switch (address & 0x7003)
    {
    case 0x6000:
        Switch02KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    case 0x6001:
        Switch02KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    case 0x6002:
        Switch02KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x6003:
        Switch02KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x7000:
        Switch08KPRG(value & 0xF, 0x8000, true);
        break;
    case 0x7001:
        Switch08KPRG(value & 0xF, 0xA000, true);
        break;
    case 0x7002:
        irq_enabled = false;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    case 0x7003:
    {
        irq_enabled = true;
        irq_reload = 0x7;
        irq_counter = 0;
        break;
    }
    }
}
void Mapper091::OnPPUA12RaisingEdge()
{
    old_irq_counter = irq_counter;

    if (irq_counter == 0 || irq_clear)
        irq_counter = irq_reload;
    else
        irq_counter = (uint8_t)(irq_counter - 1);

    if ((old_irq_counter != 0 || irq_clear) && irq_counter == 0 && irq_enabled)
        dev_cpu->pin_irq |= IRQ_FLAG_BOARD;

    irq_clear = false;
}
void Mapper091::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_enabled);
    state_str->Write(irq_counter);
    state_str->Write(old_irq_counter);
    state_str->Write(irq_reload);
    state_str->Write(irq_clear);
}
void Mapper091::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_enabled);
    state_str->Read(irq_counter);
    state_str->Read(old_irq_counter);
    state_str->Read(irq_reload);
    state_str->Read(irq_clear);
}
}

