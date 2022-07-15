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

namespace NES
{
const uint32_t ChannelNoise::FrequencyTable[3][16] =
{
    { 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 }, // NTSC
    { 4, 7, 14, 30, 60, 88, 118, 148, 188, 236, 354, 472, 708,  944, 1890, 3778 }, // PALB
    { 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 }  // DENDY (acts like NTSC)
};

ChannelNoise::~ChannelNoise()
{
}
void ChannelNoise::HardReset()
{
    APUInternalChannel::HardReset();

    mode = false;
    shiftRegister = 1;
    frequency = FrequencyTable[NES::dev_apu->systemIndex][0];
    cycles = frequency;
    feedback = 0;
    Output = 0;
}
void ChannelNoise::Clock(bool& is_clocking_duration)
{
    APUInternalChannel::Clock(is_clocking_duration);
    if (--cycles <=0)
    {
        cycles = frequency;

        if (mode)
            feedback = (shiftRegister >> 6 & 0x1) ^ (shiftRegister & 0x1);
        else
            feedback = (shiftRegister >> 1 & 0x1) ^ (shiftRegister & 0x1);
        shiftRegister >>= 1;
        shiftRegister = (shiftRegister & 0x3FFF) | ((feedback & 1) << 14);

        if (duration_counter > 0 && (shiftRegister & 1) == 0)
        {
            Output = envelope;
        }
        else
            Output = 0;
        NES::dev_apu->PlaybackSampleNeeded = true;
    }
}
void ChannelNoise::WriteReg3(uint8_t& value)
{
    frequency = FrequencyTable[NES::dev_apu->systemIndex][value & 0x0F];
    mode = (value & 0x80) == 0x80;
}
void ChannelNoise::SaveState()
{
    APUInternalChannel::SaveState();

    state_str->Write(mode);
    state_str->Write(shiftRegister);
    state_str->Write(feedback);
    state_str->Write(frequency);
    state_str->Write(cycles);
}
void ChannelNoise::LoadState()
{
    APUInternalChannel::LoadState();

    state_str->Read(mode);
    state_str->Read(shiftRegister);
    state_str->Read(feedback);
    state_str->Read(frequency);
    state_str->Read(cycles);
}
}
