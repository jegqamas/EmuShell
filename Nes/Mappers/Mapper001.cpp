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
Mapper001::Mapper001()
{
    this->Name = "MMC1";
    this->MapperNumber = 1;
    // We need to modify default settings for this one
    this->prg_ram_default_08KB_count=4;
    this->chr_ram_1KB_default_banks_count=64;
}
void Mapper001::HardReset()
{
    Board::HardReset();

    cpuCycles = 0;
    // Registers
    address_reg = 0;

    reg[0] = 0x0C;
    flag_c = false;
    flag_s = flag_p = true;
    prg_hijackedbit = 0;
    reg[1] = reg[2] = reg[3] = 0;
    // Buffers
    buffer = 0;
    shift = 0;
    // THIS IS NOT A HACK !
    // We need to determine the game's chip type to configure
    // the board correctly.
    if (NES::GameDBIndex >=0)
    {
        bool foundMMC1A=false;
        if (NesDB::ContainChip(NES::GameDBIndex,"MMC1B")||NesDB::ContainChip(NES::GameDBIndex,"MMC1B2"))
        {
            TogglePRGRAMEnable(false);
            std::cout << "MMC1: SRAM Disabled.\n";
        }

        if (NesDB::ContainChip(NES::GameDBIndex,"MMC1A"))
        {
            foundMMC1A=true;
        }

        enable_wram_enable = !foundMMC1A;
        std::cout << "MMC1: enable_wram_enable = " <<enable_wram_enable<<"\n";
    }

    //  use hijacked
    use_hijacked = (prg_16K_rom_mask & 0x10) == 0x10;
    if (use_hijacked)
        prg_hijackedbit = 0x10;
    // SRAM Switch ?
    use_sram_switch = false;
    if (prg_08K_ram_count > 0)
    {
        use_sram_switch = true;
        sram_switch_mask = use_hijacked ? 0x08 : 0x18;
        sram_switch_mask &= prg_08K_ram_mask << 3;

        if (sram_switch_mask == 0)
            use_sram_switch = false;
    }
    Switch16KPRG(0xF | prg_hijackedbit, 0xC000, true);
}
void Mapper001::WritePRG(uint16_t& address, uint8_t& value)
{
    // Too close writes ignored !
    if (cpuCycles > 0)
    {
        return;
    }
    cpuCycles = 3;// Make save cycles ...
    //Temporary reg port ($8000-FFFF):
    //[r... ...d]
    //r = reset flag
    //d = data bit

    //r is set
    if ((value & 0x80) == 0x80)
    {
        reg[0] |= 0x0C;//bits 2,3 of reg $8000 are set (16k PRG mode, $8000 swappable)
        flag_s = flag_p = true;
        shift = buffer = 0;//hidden temporary reg is reset
        return;
    }
    //d is set
    if ((value & 0x01) == 0x01)
        buffer |= (1 << shift);//'d' proceeds as the next bit written in the 5-bit sequence
    if (++shift < 5)
        return;
    // If this completes the 5-bit sequence:
    // - temporary reg is copied to actual internal reg (which reg depends on the last address written to)
    address_reg = (address & 0x7FFF) >> 13;
    reg[address_reg] = buffer;

    // - temporary reg is reset (so that next write is the "first" write)
    shift = buffer = 0;

    // Update internal registers ...
    switch (address_reg)
    {
    case 0:// $8000-9FFF [Flags and mirroring]
    {
        // Flags
        flag_c = (reg[0] & 0x10) != 0;
        flag_p = (reg[0] & 0x08) != 0;
        flag_s = (reg[0] & 0x04) != 0;
        UpdatePRG();
        UpdateCHR();
        // Mirroring
        switch (reg[0] & 3)
        {
        case 0:
            SwitchNMT(MIRRORING_OneScA);
            break;
        case 1:
            SwitchNMT(MIRRORING_OneScB);
            break;
        case 2:
            SwitchNMT(MIRRORING_Vert);
            break;
        case 3:
            SwitchNMT(MIRRORING_Horz);
            break;
        }
        break;
    }
    case 1:// $A000-BFFF [CHR REG 0]
    {
        // CHR
        if (!flag_c)
            Switch08KCHR(reg[1] >> 1, chr_01K_rom_count > 0);
        else
            Switch04KCHR(reg[1], 0x0000, chr_01K_rom_count > 0);
        // SRAM
        if (use_sram_switch)
            Switch08KPRG((reg[1] & sram_switch_mask) >> 3, 0x6000, false);
        // PRG hijack
        if (use_hijacked)
        {
            prg_hijackedbit = reg[1] & 0x10;
            UpdatePRG();
        }
        break;
    }
    case 2:// $C000-DFFF [CHR REG 1]
    {
        // CHR
        if (flag_c)
            Switch04KCHR(reg[2], 0x1000, chr_01K_rom_count > 0);
        // SRAM
        if (use_sram_switch)
            Switch08KPRG((reg[2] & sram_switch_mask) >> 3, 0x6000, false);
        // PRG hijack
        if (use_hijacked)
        {
            prg_hijackedbit = reg[2] & 0x10;
            UpdatePRG();
        }
        break;
    }
    case 3:// $E000-FFFF [PRG REG]
    {
        if (enable_wram_enable)
            TogglePRGRAMEnable((reg[3] & 0x10) == 0);
        UpdatePRG();
        break;
    }
    }
}
void Mapper001::UpdateCHR()
{
    if (!flag_c)
        Switch08KCHR(reg[1] >> 1, chr_01K_rom_count > 0);
    else
    {
        Switch04KCHR(reg[1], 0x0000, chr_01K_rom_count > 0);
        Switch04KCHR(reg[2], 0x1000, chr_01K_rom_count > 0);
    }
    // SRAM
    if (use_sram_switch)
        Switch08KPRG((reg[1] & sram_switch_mask) >> 3, 0x6000, false);
}

void Mapper001::UpdatePRG()
{
    if (!flag_p)
    {
        Switch32KPRG(((reg[3] & 0xF) | prg_hijackedbit) >> 1, true);
    }
    else
    {
        if (flag_s)
        {
            Switch16KPRG((reg[3] & 0xF) | prg_hijackedbit, 0x8000, true);
            Switch16KPRG(0xF | prg_hijackedbit, 0xC000, true);
        }
        else
        {
            Switch16KPRG(prg_hijackedbit, 0x8000, true);
            Switch16KPRG((reg[3] & 0xF) | prg_hijackedbit, 0xC000, true);
        }
    }
}
void Mapper001::OnCPUClock()
{
    if (cpuCycles > 0)
        cpuCycles--;
}
void Mapper001::SaveState()
{
    Board::SaveState();

    state_str->Write(address_reg);
    state_str->Write(reg,4) ;
    state_str->Write(shift);
    state_str->Write(buffer);
    state_str->Write(flag_p);
    state_str->Write(flag_c);
    state_str->Write(flag_s);
    state_str->Write(enable_wram_enable);
    state_str->Write(prg_hijackedbit);
    state_str->Write(use_hijacked);
    state_str->Write(use_sram_switch);
    state_str->Write(sram_switch_mask);
    state_str->Write(cpuCycles);
}
void Mapper001::LoadState()
{
    Board::LoadState();

    state_str->Read(address_reg);
    for (int i=0; i<4; i++)
        state_str->Read(reg[i]) ;

    state_str->Read(shift);
    state_str->Read(buffer);
    state_str->Read(flag_p);
    state_str->Read(flag_c);
    state_str->Read(flag_s);
    state_str->Read(enable_wram_enable);
    state_str->Read(prg_hijackedbit);
    state_str->Read(use_hijacked);
    state_str->Read(use_sram_switch);
    state_str->Read(sram_switch_mask);
    state_str->Read(cpuCycles);
}
}
