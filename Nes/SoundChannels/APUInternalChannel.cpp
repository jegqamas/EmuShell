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
const uint8_t APUInternalChannel::DurationTable[32] = {
    0x0A, 0xFE, 0x14, 0x02, 0x28, 0x04, 0x50, 0x06, 0xA0, 0x08, 0x3C, 0x0A, 0x0E, 0x0C, 0x1A, 0x0E,
    0x0C, 0x10, 0x18, 0x12, 0x30, 0x14, 0x60, 0x16, 0xC0, 0x18, 0x48, 0x1A, 0x10, 0x1C, 0x20, 0x1E
};

/* The base implementation for apu's sound channel.*/
APUInternalChannel::~APUInternalChannel()
{
}
void APUInternalChannel::HardReset()
{
    envelope = 0;
    env_startflag = false;
    env_counter = 0;
    env_devider = 0;
    constant_volume_flag = false;
    volume_decay_time = 0;
    length_counter_halt_flag = false;
    duration_haltRequset = false;
    duration_counter = 0;
    duration_reloadEnabled = false;
    duration_reload = 0;
    duration_reloadRequst = false;
    Output = 0;
}
void APUInternalChannel::SoftReset()
{
    duration_counter = 0;
    duration_reloadEnabled = false;
}
void APUInternalChannel::ClockEnvelope()
{
    if (env_startflag)
    {
        env_startflag = false;
        env_counter = 0xF;
        env_devider = volume_decay_time + 1;
    }
    else
    {
        if (env_devider > 0)
            env_devider--;
        else
        {
            env_devider = volume_decay_time + 1;
            if (env_counter > 0)
                env_counter--;
            else if (length_counter_halt_flag)
                env_counter = 0xF;
        }
    }
    envelope = constant_volume_flag ? volume_decay_time : env_counter;
}
void APUInternalChannel::ClockDuration()
{
    if (!length_counter_halt_flag)
    {
        if (duration_counter > 0)
        {
            duration_counter--;
        }
    }
}
void APUInternalChannel::Clock(bool& is_clocking_duration)
{
    length_counter_halt_flag = duration_haltRequset;
    if (is_clocking_duration && duration_counter > 0)
        duration_reloadRequst = false;
    if (duration_reloadRequst)
    {
        if (duration_reloadEnabled)
            duration_counter = duration_reload;
        duration_reloadRequst = false;
    }
}
void APUInternalChannel::WriteReg1(uint8_t& value)
{
    volume_decay_time = value & 0xF;
    duration_haltRequset = (value & 0x20) != 0;
    constant_volume_flag = (value & 0x10) != 0;
    envelope = constant_volume_flag ? volume_decay_time : env_counter;
}
void APUInternalChannel::WriteReg2(uint8_t& value)
{
}
void APUInternalChannel::WriteReg3(uint8_t& value)
{
}
void APUInternalChannel::WriteReg4(uint8_t& value)
{
    duration_reload = DurationTable[value >> 3];
    duration_reloadRequst = true;
    env_startflag = true;
}
void APUInternalChannel::SetEnable(bool enable)
{
    duration_reloadEnabled = enable;
    if (!duration_reloadEnabled)
        duration_counter = 0;
}
bool APUInternalChannel::IsEnabled()
{
    return duration_counter > 0;
}
// Used only by DMC channel
bool APUInternalChannel::IsIRQDeltaOccur()
{
    return false;
}
void APUInternalChannel::SaveState()
{
    state_str->Write(envelope);
    state_str->Write(env_startflag);
    state_str->Write(constant_volume_flag);
    state_str->Write(volume_decay_time);
    state_str->Write(env_counter);
    state_str->Write(env_devider);
    state_str->Write(length_counter_halt_flag);
    state_str->Write(duration_haltRequset);
    state_str->Write(duration_counter);
    state_str->Write(duration_reloadEnabled);
    state_str->Write(duration_reload);
    state_str->Write(duration_reloadRequst);
}
void APUInternalChannel::LoadState()
{
    state_str->Read(envelope);
    state_str->Read(env_startflag);
    state_str->Read(constant_volume_flag);
    state_str->Read(volume_decay_time);
    state_str->Read(env_counter);
    state_str->Read(env_devider);
    state_str->Read(length_counter_halt_flag);
    state_str->Read(duration_haltRequset);
    state_str->Read(duration_counter);
    state_str->Read(duration_reloadEnabled);
    state_str->Read(duration_reload);
    state_str->Read(duration_reloadRequst);
}
}
