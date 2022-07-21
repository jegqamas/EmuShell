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
const uint8_t MMC5ChannelPulse::DutyForms[4][8] =
{
    { 1, 1, 1, 1, 1, 1, 1, 0 }, // 87.5%
    { 1, 1, 1, 1, 1, 1, 0, 0 }, // 75.0%
    { 1, 1, 1, 1, 0, 0, 0, 0 }, // 50.0%
    { 1, 1, 0, 0, 0, 0, 0, 0 }, // 25.0%
};

MMC5ChannelPulse::~MMC5ChannelPulse()
{
}
void MMC5ChannelPulse::HardReset()
{
    APUInternalChannel::HardReset();

    dutyForm = 0;
    dutyStep = 0;
    frequency = 0;
    cycles = 1;
}
void MMC5ChannelPulse::Clock(bool& is_clocking_duration)
{
    APUInternalChannel::Clock(is_clocking_duration);

    if (--cycles <=0)
    {
        // "Since the period of the timer is t+1 APU cycles and the sequencer has 8 steps,
        // the period of the waveform is 8*(t+1) APU cycles"
        // Its t+1 APU clock, so we add 1 first then shift left by one ((t+1)* 2)
        cycles = (frequency + 1) << 1;
        if (dutyStep==0)
            dutyStep=0x7;
        else
            dutyStep--;

        if (duration_counter > 0)
        {
            // if (audio_playback_sq1_enabled)
            Output = DutyForms[dutyForm][dutyStep] * envelope;

        }
        else
            Output = 0;
        NES::dev_apu->PlaybackSampleNeeded = true;
    }
}


void MMC5ChannelPulse::Write5000(uint8_t& value)
{
    APUInternalChannel::WriteReg1(value);
    dutyForm = (value & 0xC0) >> 6;
}

void MMC5ChannelPulse::Write5002(uint8_t& value)
{
    frequency = (frequency & 0x700) | value;
}
void MMC5ChannelPulse::Write5003(uint8_t& value)
{
    APUInternalChannel::WriteReg4(value);
    frequency = (frequency & 0x00FF) | ((value & 7) << 8);
    dutyStep = 0;
}

void MMC5ChannelPulse::SaveState()
{
    APUInternalChannel::SaveState();
    state_str->Write(dutyForm);
    state_str->Write(dutyStep);
    state_str->Write(frequency);
    state_str->Write(cycles);
}
void MMC5ChannelPulse::LoadState()
{
    APUInternalChannel::LoadState();
    state_str->Read(dutyForm);
    state_str->Read(dutyStep);
    state_str->Read(frequency);
    state_str->Read(cycles);
}
}
