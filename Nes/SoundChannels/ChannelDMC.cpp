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
const uint32_t ChannelDMC::FrequencyTable[3][16] =
{
    { 428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54 }, // NTSC
    { 398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98,  78,  66,  50 }, // PALB
    { 428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54 }  // DENDY (acts like NTSC)
};

ChannelDMC::~ChannelDMC()
{
}
void ChannelDMC::HardReset()
{


    DeltaIrqOccur = false;
    DMCIrqEnabled = false;
    dmaLooping = false;
    dmaEnabled = false;
    bufferFull = false;
    dmc_output = 0;
    dmaAddr = dmaAddrRefresh = 0xC000;
    dmaSizeRefresh = 0;
    dmaSize = 0;
    dmaBits = 1;
    dmaByte = 1;
    dmaAddr = 0;
    dmaBuffer = 0;
    freqTimer = 0;
    cycles = FrequencyTable[NES::dev_apu->systemIndex][freqTimer];
}
void ChannelDMC::SoftReset()
{
    DeltaIrqOccur = false;
}
void ChannelDMC::Clock(bool& is_clocking_duration)
{
    if (--cycles <=0)
    {
        cycles = FrequencyTable[NES::dev_apu->systemIndex][freqTimer];

        if (dmaEnabled)
        {
            if ((dmaByte & 0x01) != 0)
            {
                if (dmc_output <= 0x7D)
                    dmc_output += 2;
            }
            else
            {
                if (dmc_output >= 0x02)
                    dmc_output -= 2;
            }
            dmaByte >>= 1;
        }
        dmaBits--;
        if (dmaBits == 0)
        {
            dmaBits = 8;
            if (bufferFull)
            {
                bufferFull = false;
                dmaEnabled = true;
                dmaByte = dmaBuffer;
                // RDY ?
                if (dmaSize > 0)
                {
                    NES::dev_dma->AssertDMCDMA();
                }
            }
            else
            {
                dmaEnabled = false;
            }
        }

        Output = dmc_output;
        dev_apu->PlaybackSampleNeeded = true;
    }
}
void ChannelDMC::WriteReg1(uint8_t& value)
{
    DMCIrqEnabled = (value & 0x80) != 0;
    dmaLooping = (value & 0x40) != 0;

    if (DMCIrqEnabled==false)
    {
        DeltaIrqOccur = false;
        dev_cpu->pin_irq &= IRQ_FLAG_DMC_OFF;
    }
    freqTimer = value & 0x0F;
}
void ChannelDMC::WriteReg2(uint8_t& value)
{
    dmc_output = value & 0x7F;
}
void ChannelDMC::WriteReg3(uint8_t& value)
{
    dmaAddrRefresh = (value << 6) | 0xC000;
}
void ChannelDMC::WriteReg4(uint8_t& value)
{
    dmaSizeRefresh = (value << 4) | 0x0001;
}
void ChannelDMC::SetEnable(bool enable)
{
    // DMC
    if (enable)
    {
        if (dmaSize == 0)
        {
            dmaSize = dmaSizeRefresh;
            dmaAddr = dmaAddrRefresh;
        }
    }
    else
    {
        dmaSize = 0;
    }
    // Disable DMC IRQ
    DeltaIrqOccur = false;

    dev_cpu->pin_irq &= IRQ_FLAG_DMC_OFF;
    // RDY ?
    if ((bufferFull==false) && dmaSize > 0)
    {
        dev_dma->AssertDMCDMA();
    }
}
bool ChannelDMC::IsEnabled()
{
    return dmaSize > 0;
}
bool ChannelDMC::IsIRQDeltaOccur()
{
    return DeltaIrqOccur;
}
void ChannelDMC::DoDMA()
{
    bufferFull = true;

    dmaBuffer = MemRead(dmaAddr);

    if (++dmaAddr == 0x10000)
        dmaAddr = 0x8000;
    if (dmaSize > 0)
        dmaSize--;

    if (dmaSize == 0)
    {
        if (dmaLooping)
        {
            dmaAddr = dmaAddrRefresh;
            dmaSize = dmaSizeRefresh;
        }
        else if (DMCIrqEnabled)
        {
            dev_cpu->pin_irq |= IRQ_FLAG_DMC;
            DeltaIrqOccur = true;
        }
    }
}
void ChannelDMC::SaveState()
{
    APUInternalChannel::SaveState();

    state_str->Write(DeltaIrqOccur);
    state_str->Write(DMCIrqEnabled);
    state_str->Write(dmaLooping);
    state_str->Write(dmaEnabled);
    state_str->Write(bufferFull);
    state_str->Write(dmaAddrRefresh);
    state_str->Write(dmaSizeRefresh);
    state_str->Write(dmaSize);
    state_str->Write(dmaBits);
    state_str->Write(dmaByte);
    state_str->Write(dmaAddr);
    state_str->Write(dmaBuffer);
    state_str->Write(dmc_output);
    state_str->Write(cycles);
    state_str->Write(freqTimer);
}
void ChannelDMC::LoadState()
{
    APUInternalChannel::LoadState();

    state_str->Read(DeltaIrqOccur);
    state_str->Read(DMCIrqEnabled);
    state_str->Read(dmaLooping);
    state_str->Read(dmaEnabled);
    state_str->Read(bufferFull);
    state_str->Read(dmaAddrRefresh);
    state_str->Read(dmaSizeRefresh);
    state_str->Read(dmaSize);
    state_str->Read(dmaBits);
    state_str->Read(dmaByte);
    state_str->Read(dmaAddr);
    state_str->Read(dmaBuffer);
    state_str->Read(dmc_output);
    state_str->Read(cycles);
    state_str->Read(freqTimer);
}
}
