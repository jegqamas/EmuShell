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

namespace NES
{
const uint8_t ChannelTriangle::DurationTable[32] =
{
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E
};

const uint8_t ChannelTriangle::StepSequence[32] =
{
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15
};

ChannelTriangle::~ChannelTriangle()
{
}
void ChannelTriangle::HardReset()
{
    length_counter_halt_flag = false;
    duration_haltRequset = false;
    duration_counter = 0;
    duration_reloadEnabled = false;
    duration_reload = 0;
    duration_reloadRequst = false;
    linearCounter = 0;
    linearCounterReload = 0;
    step = 0xF;
    linearCounterHalt = false;
    halt = true;
    frequency = 0;
    cycles = 4;
    Output = 0;
}
void ChannelTriangle::Clock(bool& is_clocking_duration)
{
    APUInternalChannel::Clock(is_clocking_duration);

    if (--cycles <=0)
    {
        dev_apu->PlaybackSampleNeeded = true;
        cycles = frequency + 1;
        if (duration_counter > 0 && linearCounter > 0)
        {
            if (frequency >= 4)
            {
                step++;
                step &= 0x1F;
                Output = StepSequence[step];
            }
        }
    }
}
void ChannelTriangle::ClockEnvelope()
{
    if (halt)
    {
        linearCounter = linearCounterReload;
    }
    else
    {
        if (linearCounter > 0)
        {
            linearCounter--;
        }
    }

    halt &= linearCounterHalt;
}
void ChannelTriangle::WriteReg1(uint8_t& value)
{
    linearCounterHalt = duration_haltRequset = (value & 0x80) != 0;
    linearCounterReload = (value & 0x7F);
}
void ChannelTriangle::WriteReg3(uint8_t& value)
{
    frequency = (frequency & 0x700) | value;
}
void ChannelTriangle::WriteReg4(uint8_t& value)
{
    frequency = (frequency & 0x00FF) | ((value & 7) << 8);

    duration_reload = DurationTable[value >> 3];
    duration_reloadRequst = true;
    halt = true;
}
void ChannelTriangle::SaveState()
{
    APUInternalChannel::SaveState();

    state_str->Write(linearCounter );
    state_str->Write(linearCounterReload);
    state_str->Write(step);
    state_str->Write(linearCounterHalt);
    state_str->Write(halt);
    state_str->Write(frequency);
    state_str->Write(cycles);
}
void ChannelTriangle::LoadState()
{
    APUInternalChannel::LoadState();

    state_str->Read(linearCounter );
    state_str->Read(linearCounterReload);
    state_str->Read(step);
    state_str->Read(linearCounterHalt);
    state_str->Read(halt);
    state_str->Read(frequency);
    state_str->Read(cycles);
}
}
