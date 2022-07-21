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

#include "../Nes.h"
#include "../../Definations.h"

namespace NES
{
void MMC5ChannelPCM:: HardReset()
{
    output = 0;
    readMode = false;
    PCMIRQenable = false;
    irqTrip = false;
}
void MMC5ChannelPCM::SoftReset()
{
    output = 0;
    readMode = false;
    PCMIRQenable = false;
    irqTrip = false;
}
void MMC5ChannelPCM::Write5010(uint8_t& data)
{
    readMode = (data & 0x1) == 0x1;
    PCMIRQenable = (data & 0x80) == 0x80;
    // Update irq
    if (PCMIRQenable && irqTrip)
        dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
}
uint8_t MMC5ChannelPCM::Read5010()
{
    uint8_t data = (irqTrip & PCMIRQenable) ? 0x80 : 0;

    irqTrip = false;
    dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
    return data;
}
void MMC5ChannelPCM::Write5011(uint8_t& data)
{
    if (!readMode)
    {
        if (data == 0)
        {
            irqTrip = true;
        }
        else
        {
            irqTrip = false;
            output = data;
            dev_apu->PlaybackSampleNeeded=true;
        }
        // Update irq
        if (PCMIRQenable && irqTrip)
            dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
    }
}
void MMC5ChannelPCM::SaveState()
{
    state_str->Write(readMode);
    state_str->Write(PCMIRQenable);
    state_str->Write(irqTrip);
}
void MMC5ChannelPCM::LoadState()
{
    state_str->Read(readMode);
    state_str->Read(PCMIRQenable);
    state_str->Read(irqTrip);
}
}
