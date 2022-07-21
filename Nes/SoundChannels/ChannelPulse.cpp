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
const uint8_t ChannelPulse::DutyForms[4][8] =
{
    { 0, 1, 0, 0, 0, 0, 0, 0 }, // 12.5%
    { 0, 1, 1, 0, 0, 0, 0, 0 }, // 25.0%
    { 0, 1, 1, 1, 1, 0, 0, 0 }, // 50.0%
    { 1, 0, 0, 1, 1, 1, 1, 1 }  // 75.0% (25.0% negated)
};

ChannelPulse::~ChannelPulse()
{
}
void ChannelPulse::HardReset()
{
    APUInternalChannel::HardReset();

    dutyForm = 0;
    dutyStep = 0;
    sweepDeviderPeriod = 0;
    sweepShiftCount = 0;
    sweepCounter = 0;
    sweepEnable = false;
    sweepReload = false;
    sweepNegateFlag = false;
    frequency = 0;
    sweep = 0;
    cycles = 1;
}
bool ChannelPulse::IsValidFrequency()
{
    return (frequency >= 0x8) && ((sweepNegateFlag) || (((frequency + (frequency >> sweepShiftCount)) & 0x800) == 0));
}
void ChannelPulse::ClockDuration()
{
    APUInternalChannel::ClockDuration();

    sweepCounter--;
    if (sweepCounter == 0)
    {
        sweepCounter = sweepDeviderPeriod + 1;
        if (sweepEnable && (sweepShiftCount > 0) && IsValidFrequency())
        {
            sweep = frequency >> sweepShiftCount;
            if (!sweep_mode_2)
            {
                if (sweepNegateFlag)
                    frequency = frequency + ~sweep;
                else
                    frequency = frequency + sweep;
            }
            else
            {
                if (sweepNegateFlag)
                    frequency = frequency - sweep;
                else
                    frequency = frequency + sweep;
            }
        }
    }
    if (sweepReload)
    {
        sweepReload = false;
        sweepCounter = sweepDeviderPeriod + 1;
    }
}
void ChannelPulse::Clock(bool& is_clocking_duration)
{
    APUInternalChannel::Clock(is_clocking_duration);

    if (--cycles <=0)
    {
        // "Since the period of the timer is t+1 APU cycles and the sequencer has 8 steps,
        // the period of the waveform is 8*(t+1) APU cycles"
        // Its t+1 APU clock, so we add 1 first then shift left by one ((t+1)* 2)
        cycles = (frequency + 1) << 1;
        dutyStep = (dutyStep + 1) & 0x7;
        if (duration_counter > 0 && IsValidFrequency())
        {
            // if (audio_playback_sq1_enabled)
            Output = DutyForms[dutyForm][dutyStep] * envelope;

        }
        else
            Output = 0;
        NES::dev_apu->PlaybackSampleNeeded = true;
    }
}
void ChannelPulse::WriteReg1(uint8_t& value)
{
    APUInternalChannel::WriteReg1(value);
    dutyForm = (value & 0xC0) >> 6;
}
void ChannelPulse::WriteReg2(uint8_t& value)
{
    sweepEnable = (value & 0x80) == 0x80;
    sweepDeviderPeriod = (value >> 4) & 7;
    sweepNegateFlag = (value & 0x8) == 0x8;
    sweepShiftCount = value & 7;
    sweepReload = true;
}
void ChannelPulse::WriteReg3(uint8_t& value)
{
    frequency = (frequency & 0x0700) | value;
}
void ChannelPulse::WriteReg4(uint8_t& value)
{
    APUInternalChannel::WriteReg4(value);
    frequency = (frequency & 0x00FF) | ((value & 7) << 8);
    dutyStep = 0;
}
void ChannelPulse::SaveState()
{
    APUInternalChannel::SaveState();

    state_str->Write(dutyForm);
    state_str->Write(dutyStep);
    state_str->Write(sweepDeviderPeriod);
    state_str->Write(sweepShiftCount);
    state_str->Write(sweepCounter);
    state_str->Write(sweepEnable);
    state_str->Write(sweepReload);
    state_str->Write(sweepNegateFlag);
    state_str->Write(frequency);
    state_str->Write(sweep);
    state_str->Write(cycles);
}
void ChannelPulse::LoadState()
{
    APUInternalChannel::LoadState();

    state_str->Read(dutyForm);
    state_str->Read(dutyStep);
    state_str->Read(sweepDeviderPeriod);
    state_str->Read(sweepShiftCount);
    state_str->Read(sweepCounter);
    state_str->Read(sweepEnable);
    state_str->Read(sweepReload);
    state_str->Read(sweepNegateFlag);
    state_str->Read(frequency);
    state_str->Read(sweep);
    state_str->Read(cycles);
}
}
