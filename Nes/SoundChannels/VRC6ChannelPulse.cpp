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

#include "../Nes.h"
#include "../../Definations.h"
#include <stdio.h>
namespace NES
{
void VRC6ChannelPulse::HardReset()
{
    dutyForm = 0;
    dutyStep = 0xF;
    enabled = false;
    mode = false;
    Output = 0;
    frequency=0;
    cycles = (frequency + 1) << 1;
}
void VRC6ChannelPulse::Clock()
{
    if (--cycles <= 0)
    {
        cycles = (frequency + 1) << 1;
        if (enabled)
        {
            if (mode)
                Output = volume;
            else
            {
                if (dutyStep>0)
                    dutyStep--;
                else
                    dutyStep = 0xF;

                if (dutyStep <= dutyForm)
                    Output = volume;
                else
                    Output = 0;
            }
        }
        else
            Output = 0;

        dev_apu->PlaybackSampleNeeded = true;
    }
}

void VRC6ChannelPulse::WriteReg1(uint8_t& value)
{
    mode = (value & 0x80) == 0x80;
    dutyForm = ((value & 0x70) >> 4);
    volume = (value & 0xF);
}
void VRC6ChannelPulse::WriteReg2(uint8_t& value)
{
    frequency = (frequency & 0x0F00) | value;
}
void VRC6ChannelPulse::WriteReg3(uint8_t& value)
{
    frequency = (frequency & 0x00FF) | ((value & 0xF) << 8);
    enabled = (value & 0x80) == 0x80;
}
void VRC6ChannelPulse::SaveState()
{
    state_str->Write (dutyForm);
    state_str->Write (dutyStep);
    state_str->Write (freqTimer);
    state_str->Write (frequency);
    state_str->Write (cycles);
    state_str->Write (enabled );
    state_str->Write (mode );
    state_str->Write (volume);
}
void VRC6ChannelPulse::LoadState()
{
    state_str->Read (dutyForm);
    state_str->Read (dutyStep);
    state_str->Read (freqTimer);
    state_str->Read (frequency);
    state_str->Read (cycles);
    state_str->Read (enabled );
    state_str->Read (mode );
    state_str->Read (volume);
}
}

