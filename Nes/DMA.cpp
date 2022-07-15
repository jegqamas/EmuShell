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

#include "Nes.h"
#include <iostream>
#include "../Common/ConfigHandler.h"

namespace NES
{
void DMAUnit::HardReset()
{
    dmaDMCDMAWaitCycles = 0;
    dmaOAMDMAWaitCycles = 0;
    isOamDma = false;
    oamdma_i = 0;
    dmaDMCOn = false;
    dmaOAMOn = false;
    dmaDMC_occurring = false;
    dmaOAM_occurring = false;
    dmaOAMFinishCounter = 0;
    dmaOamaddress = 0;
    OAMCYCLE = 0;
    latch = 0;
}
void DMAUnit::SoftReset()
{
    dmaDMCDMAWaitCycles = 0;
    dmaOAMDMAWaitCycles = 0;
    isOamDma = false;
    oamdma_i = 0;
    dmaDMCOn = false;
    dmaOAMOn = false;
    dmaDMC_occurring = false;
    dmaOAM_occurring = false;
    dmaOAMFinishCounter = 0;
    dmaOamaddress = 0;
    OAMCYCLE = 0;
    latch = 0;
}
void DMAUnit::AssertDMCDMA()
{
    if (dmaOAM_occurring)
    {
        if (OAMCYCLE < 508)
            // OAM DMA is occurring here
            dmaDMCDMAWaitCycles = NES::BUS_RW ? 1 : 0;
        else
        {
            // Here the oam dma is about to finish
            // Remaining cycles of oam dma determines the dmc dma waiting cycles.
            dmaDMCDMAWaitCycles = 4 - (512 - OAMCYCLE);

        }
    }
    else if (dmaDMC_occurring)
    {
        // DMC occurring now !!? is that possible ?
        // Anyway, let's ignore this call !
        return;
    }
    else
    {
        // Nothing occurring, initialize brand new dma
        // DMC DMA depends on r/w flag for the wait cycles.
        dmaDMCDMAWaitCycles = NES::BUS_RW ? 3 : 2;
        // After 2 cycles of oam dma, add extra cycle for the waiting.
        if (dmaOAMFinishCounter == 3)
            dmaDMCDMAWaitCycles++;
    }
    isOamDma = false;
    dmaDMCOn = true;
}
void DMAUnit::AssertOAMDMA(uint16_t dmaAddress)
{
    if (dmaOAM_occurring) return;
    // Setup
    // OAM DMA depends on apu odd timer for odd cycles
    dmaOAMDMAWaitCycles = dev_apu->oddCycle ? 1 : 2;
    dmaOamaddress = dmaAddress;
    isOamDma = true;
    dmaOAMOn = true;
}
void DMAUnit::Clock()
{
    if (dmaOAMFinishCounter > 0)
        dmaOAMFinishCounter--;
    if (!NES::BUS_RW)// Clocks only on reads
    {
        if (dmaDMCDMAWaitCycles > 0)
            dmaDMCDMAWaitCycles--;
        if (dmaOAMDMAWaitCycles > 0)
            dmaOAMDMAWaitCycles--;
        return;
    }
    if (dmaDMCOn)
    {
        dmaDMC_occurring = true;
        // This is it !
        dmaDMCOn = false;
        // Do wait cycles (extra reads)
        if (dmaDMCDMAWaitCycles > 0)
        {
            if (BUS_ADDRESS == 0x4016 || BUS_ADDRESS == 0x4017)
            {
                MemRead(BUS_ADDRESS);
                if (dmaDMCDMAWaitCycles>0)
                    dmaDMCDMAWaitCycles--;

                while (dmaDMCDMAWaitCycles > 0)
                {
                    ClockComponents();
                    if (dmaDMCDMAWaitCycles>0)
                        dmaDMCDMAWaitCycles--;
                }
            }
            else
            {
                while (dmaDMCDMAWaitCycles > 0)
                {
                    MemRead(BUS_ADDRESS);
                    if (dmaDMCDMAWaitCycles>0)
                        dmaDMCDMAWaitCycles--;
                }
            }
        }
        // Do DMC DMA
        dev_apu->DoDMCDMA();

        dmaDMC_occurring = false;
    }
    if (dmaOAMOn)
    {
        dmaOAM_occurring = true;
        // This is it ! pause the cpu
        dmaOAMOn = false;
        // Do wait cycles (extra reads)
        if (dmaOAMDMAWaitCycles > 0)
        {
            if (NES::BUS_ADDRESS == 0x4016 || NES::BUS_ADDRESS == 0x4017)
            {
                MemRead(BUS_ADDRESS);
                if (dmaOAMDMAWaitCycles>0)
                    dmaOAMDMAWaitCycles--;

                while (dmaOAMDMAWaitCycles > 0)
                {
                    NES::ClockComponents();
                    if (dmaOAMDMAWaitCycles>0)
                        dmaOAMDMAWaitCycles--;
                }
            }
            else
            {
                while (dmaOAMDMAWaitCycles > 0)
                {
                    MemRead(BUS_ADDRESS);
                    if (dmaOAMDMAWaitCycles>0)
                        dmaOAMDMAWaitCycles--;
                }
            }
        }

        // Do OAM DMA
        OAMCYCLE = 0;
        for (oamdma_i = 0; oamdma_i < 256; oamdma_i++)
        {
            latch = MemRead(dmaOamaddress);
            OAMCYCLE++;
            MemWrite(0x2004, latch);
            OAMCYCLE++;
            dmaOamaddress++;
        }
        OAMCYCLE = 0;
        dmaOAMFinishCounter = 5;
        dmaOAM_occurring = false;
    }
}
void DMAUnit::SaveState()
{
    state_str->Write(dmaDMCDMAWaitCycles);
    state_str->Write(dmaOAMDMAWaitCycles);
    state_str->Write(isOamDma);
    state_str->Write(oamdma_i);
    state_str->Write(dmaDMCOn);
    state_str->Write(dmaOAMOn);
    state_str->Write(dmaDMC_occurring);
    state_str->Write(dmaOAM_occurring);
    state_str->Write(dmaOAMFinishCounter);
    state_str->Write(dmaOamaddress);
    state_str->Write(OAMCYCLE);
    state_str->Write(latch);
}
void DMAUnit::LoadState()
{
    state_str->Read(dmaDMCDMAWaitCycles);
    state_str->Read(dmaOAMDMAWaitCycles);
    state_str->Read(isOamDma);
    state_str->Read(oamdma_i);
    state_str->Read(dmaDMCOn);
    state_str->Read(dmaOAMOn);
    state_str->Read(dmaDMC_occurring);
    state_str->Read(dmaOAM_occurring);
    state_str->Read(dmaOAMFinishCounter);
    state_str->Read(dmaOamaddress);
    state_str->Read(OAMCYCLE);
    state_str->Read(latch);
}
}
