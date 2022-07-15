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
namespace NES
{
void Sunsoft5BChannel::HardReset()
{
    Volume=0;
    dutyStep = 0;
    freqTimer=1;
    frequency=1;
    cycles=1;
}
void Sunsoft5BChannel::SoftReset()
{
    this->HardReset();
}
void Sunsoft5BChannel::ClockSingle()
{
    if (--cycles <= 0)
    {
        cycles = freqTimer;
        dutyStep = (dutyStep + 1) & 0x1F;

        if (dutyStep <= 15)
        {
            if (Enabled)
                output = Volume;
            else
                output =0;
        }
        else
            output =0;

        dev_apu->PlaybackSampleNeeded=true;
    }
}
void Sunsoft5BChannel::Write0(uint8_t& value)
{
    frequency = (frequency & 0x0F00) | value;
    freqTimer = (frequency + 1) * 2;
}
void Sunsoft5BChannel::Write1(uint8_t& value)
{
    frequency = (frequency & 0x00FF) | ((value & 0xF) << 8);
    freqTimer = (frequency + 1) * 2;
}
void Sunsoft5BChannel::SaveState()
{
    state_str->Write( Enabled);
    state_str->Write(  Volume);
    state_str->Write(  dutyStep );
    state_str->Write(  freqTimer);
    state_str->Write(  frequency);
    state_str->Write(  cycles);
}
void Sunsoft5BChannel::LoadState()
{
    state_str->Read( Enabled);
    state_str->Read(  Volume);
    state_str->Read(  dutyStep );
    state_str->Read(  freqTimer);
    state_str->Read(  frequency);
    state_str->Read(  cycles);
}
}
