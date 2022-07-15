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
Mapper105::Mapper105()
{
    this->Name = "NES-EVENT";
    this->MapperNumber = 105;
}
void Mapper105::HardReset()
{
    Board::HardReset();
    TogglePRGRAMEnable(true);
    // Registers
    reg[0] = 0x0C;
    flag_s = flag_p = true;
    reg[1] = reg[2] = reg[3] = 0;
    // Buffers
    buffer = 0;
    shift = 0;
    initialized = false;
    DipSwitchNumber = 0;
    dipswitches = 0x20000000 | (DipSwitchNumber << 22);
}
void Mapper105::SoftReset()
{
    DipSwitchNumber = (DipSwitchNumber + 1) & 0xF;
    dipswitches = 0x20000000 | (DipSwitchNumber << 22);
}
void Mapper105::WritePRG(uint16_t& address, uint8_t& value)
{
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
        buffer |= (uint8_t)(1 << shift);//'d' proceeds as the next bit written in the 5-bit sequence
    if (++shift < 5)
        return;
    // If this completes the 5-bit sequence:
    // - temporary reg is copied to actual internal reg (which reg depends on the last address written to)
    address = (uint16_t)((address & 0x7FFF) >> 13);
    reg[address] = buffer;

    // - temporary reg is reset (so that next write is the "first" write)
    shift = buffer = 0;

    // Update internal registers ...
    switch (address)
    {
    case 0:// $8000-9FFF
    {
        // Flags
        flag_p = (reg[0] & 0x08) != 0;
        flag_s = (reg[0] & 0x04) != 0;
        UpdatePRG();
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
    case 1:// $A000-BFFF
    {
        irq_control = (reg[1] & 0x10) == 0x10;
        if (irq_control)
        {
            initialized = true;
            irq_counter = 0;
            dev_cpu ->pin_irq &= IRQ_FLAG_BOARD_OFF;
        }
        else
            Switch32KPRG(0, true);

        flag_o = (reg[1] & 0x8) == 0x8;
        reg_a = (reg[1] >> 1) & 0x3;
        UpdatePRG();
        break;
    }
    case 3:// $E000-FFFF
    {
        TogglePRGRAMEnable((reg[3] & 0x10) == 0);
        reg_b = reg[3] & 0xF;
        UpdatePRG();
        break;
    }
    }
}
void Mapper105::UpdatePRG()
{
    if (!initialized)
        return;

    if (!flag_o)
    {
        Switch32KPRG(reg_a, true);
    }
    else
    {
        if (!flag_p)
        {
            Switch32KPRG((reg_b >> 1) + 4, true);
        }
        else
        {
            if (!flag_s)
            {
                Switch16KPRG(8, 0x8000, true);
                Switch16KPRG(reg_b + 8, 0xC000, true);
            }
            else
            {
                Switch16KPRG(reg_b + 8, 0x8000, true);
                Switch16KPRG(15, 0xC000, true);
            }
        }
    }
}
void Mapper105::OnCPUClock()
{
    if (!irq_control)
    {
        irq_counter++;
        if (irq_counter == dipswitches)
        {
            irq_counter = 0;
            dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}
void  Mapper105::SaveState()
{
    Board::SaveState();
    state_str->Write(DipSwitchNumber);
    for (int i=0; i<4; i++)
        state_str->Write( reg[i]);
    state_str->Write( shift);
    state_str->Write( buffer);
    state_str->Write( flag_p);
    state_str->Write( flag_s);
    state_str->Write( flag_o);
    state_str->Write( reg_a);
    state_str->Write( reg_b);
    state_str->Write( irq_control);
    state_str->Write( initialized);
    state_str->Write( irq_counter);
    state_str->Write( dipswitches);
}
void  Mapper105::LoadState()
{
    Board::LoadState();
    state_str->Read(DipSwitchNumber);
    for (int i=0; i<4; i++)
        state_str->Read( reg[i]);
    state_str->Read( shift);
    state_str->Read( buffer);
    state_str->Read( flag_p);
    state_str->Read( flag_s);
    state_str->Read( flag_o);
    state_str->Read( reg_a);
    state_str->Read( reg_b);
    state_str->Read( irq_control);
    state_str->Read( initialized);
    state_str->Read( irq_counter);
    state_str->Read( dipswitches);
}
}


