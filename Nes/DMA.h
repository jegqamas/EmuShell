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

// I suspect the SN74LS373N chip: "OCTAL TRANSPARENT LATCH WITH 3-STATE OUTPUTS; OCTAL D-TYPE FLIP-FLOP
// WITH 3-STATE OUTPUT"
// http://html.alldatasheet.com/html-pdf/28021/TI/SN74LS373N/24/1/SN74LS373N.html
// This chip (somehow, not confirmed yet) is responsible for dma operations inside nes
// This class emulate the dma behaviors, not the real chip.

#include <stdint.h>

namespace NES
{
class DMAUnit
{
public:
    void HardReset();
    void SoftReset();
    void AssertDMCDMA();
    void AssertOAMDMA(unsigned short dmaAddress);
    void SaveState();
    void LoadState();
    void Clock();

private:
    uint32_t dmaDMCDMAWaitCycles;
    uint32_t dmaOAMDMAWaitCycles;
    bool isOamDma;
    uint32_t oamdma_i;
    bool dmaDMCOn;
    bool dmaOAMOn;
    bool dmaDMC_occurring;
    bool dmaOAM_occurring;
    uint32_t dmaOAMFinishCounter;
    uint16_t dmaOamaddress;
    uint32_t OAMCYCLE;
    uint8_t latch;
};
}
