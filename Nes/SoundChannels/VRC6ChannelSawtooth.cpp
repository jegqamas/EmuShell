
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
void VRC6ChannelSawtooth::HardReset()
{
    AccumRate=0;
    accumClock=0;
    accumulationRegister=0;
    frequency=1;
    freqTimer=0;
    cycles=1;
    enabled=false;
}
void VRC6ChannelSawtooth::UpdateFrequency()
{
    freqTimer = (frequency + 1) * 2;
}
void VRC6ChannelSawtooth::Clock()
{
    if (--cycles <= 0)
    {
        cycles = freqTimer;
        if (enabled)
        {
            accumClock++;
            switch (++accumClock)
            {
            case 2:
            case 4:
            case 6:
            case 8:
            case 10:
            case 12:
                accumulationRegister += AccumRate;
                break;

            case 14:
                accumulationRegister = 0;
                accumClock = 0;
                break;
            }

            Output = ((accumulationRegister >> 3) & 0x1F);
        }
        else
            Output =0;
        dev_apu->PlaybackSampleNeeded = true;
    }
}
void VRC6ChannelSawtooth::WriteReg1(uint8_t& value)
{
    AccumRate = (value & 0x3F);
}
void VRC6ChannelSawtooth::WriteReg2(uint8_t& value)
{
    frequency = (frequency & 0x0F00) | value;
    UpdateFrequency();
}
void VRC6ChannelSawtooth::WriteReg3(uint8_t& value)
{
    frequency = (frequency & 0x00FF) | ((value & 0xF) << 8);
    enabled = (value & 0x80) == 0x80;
    UpdateFrequency();
}
void VRC6ChannelSawtooth::SaveState()
{
    state_str->Write(AccumRate);
    state_str->Write(accumClock);
    state_str->Write(accumulationRegister);
    state_str->Write(frequency);
    state_str->Write(freqTimer);
    state_str->Write(cycles);
    state_str->Write(enabled);
}
void VRC6ChannelSawtooth::LoadState()
{
    state_str->Read(AccumRate);
    state_str->Read(accumClock);
    state_str->Read(accumulationRegister);
    state_str->Read(frequency);
    state_str->Read(freqTimer);
    state_str->Read(cycles);
    state_str->Read(enabled);
}

}

