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

#include "_Mappers.h"
#include "../../Definations.h"
namespace NES
{
Mapper026::Mapper026()
{
    this->Name = "VRC6";
    this->MapperNumber = 26;
    this->InitializeSoundMixTable();

    pulse1=new VRC6ChannelPulse();
    pulse2=new VRC6ChannelPulse();
    sawtooth=new VRC6ChannelSawtooth();
}
Mapper026::~Mapper026()
{
    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            delete [] mix_table[sq1][sq2];
        }
        delete []  mix_table[sq1];
    }
    delete [] mix_table;
    delete [] pulse1;
    delete [] pulse2;
    delete [] sawtooth;
}
void Mapper026::HardReset()
{
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
// Hard reset the sound channels ..
    pulse1->HardReset();
    pulse2->HardReset();
    sawtooth->HardReset();
    dev_apu->UseExternalSoundChannels=true;
}
void Mapper026::InitializeSoundMixTable()
{
    mix_table = new double**[16];

    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        mix_table[sq1] = new double*[16];

        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            mix_table[sq1][sq2] = new double[256];

            for (int saw = 0; saw < 256; saw++)
            {
                double sqr = (95.88 / (8128.0 / (sq1 + sq2) + 100));
                double tnd = (159.79 / (1.0 / (saw / 22638.0) + 100));

                mix_table[sq1][sq2][saw] = (sqr + tnd);
            }
        }
    }
}
void Mapper026::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x8000:
    case 0x8002:
    case 0x8001:
    case 0x8003:
    {
        Switch16KPRG(value, 0x8000, true);
        break;
    }
    case 0x9000:
    {
        pulse1->WriteReg1( value);
        break;
    }
    case 0x9002:
    {
        pulse1->WriteReg2( value);
        break;
    }
    case 0x9001:
    {
        pulse1->WriteReg3(value);
        break;
    }
    case 0xA000:
    {
        pulse2->WriteReg1( value);
        break;
    }
    case 0xA002:
    {
        pulse2->WriteReg2( value);
        break;
    }
    case 0xA001:
    {
        pulse2->WriteReg3(value);
        break;
    }
    case 0xB000:
    {
        sawtooth->WriteReg1( value);
        break;
    }
    case 0xB002:
    {
        sawtooth->WriteReg2( value);
        break;
    }
    case 0xB001:
    {
        sawtooth->WriteReg3( value);
        break;
    }
    case 0xB003:
    {
        switch ((value & 0xC) >> 2)
        {
        case 0:
            SwitchNMT(MIRRORING_Vert);
            break;
        case 1:
            SwitchNMT(MIRRORING_Horz);
            break;
        case 2:
            SwitchNMT(MIRRORING_OneScA);
            break;
        case 3:
            SwitchNMT(MIRRORING_OneScB);
            break;
        }
        break;
    }
    case 0xC000:
    case 0xC002:
    case 0xC001:
    case 0xC003:
    {
        Switch08KPRG(value, 0xC000, true);
        break;
    }
    case 0xD000:
    {
        Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
        break;
    }
    case 0xD002:
    {
        Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
        break;
    }
    case 0xD001:
    {
        Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
        break;
    }
    case 0xD003:
    {
        Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xE000:
    {
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    }
    case 0xE002:
    {
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    }
    case 0xE001:
    {
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    }
    case 0xE003:
    {
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    }
    case 0xF000:
    {
        irq_reload = value;
        break;
    }
    case 0xF002:
    {
        irq_mode_cycle = (value & 0x4) == 0x4;
        irq_enable = (value & 0x2) == 0x2;
        irq_enable_on_ak = (value & 0x1) == 0x1;
        if (irq_enable)
        {
            irq_counter = irq_reload;
            prescaler = 341;
        }
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        break;
    }
    case 0xF001:
    {
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        irq_enable = irq_enable_on_ak;
        break;
    }
    }
}
void Mapper026::OnCPUClock()
{
    if (irq_enable)
    {
        if (!irq_mode_cycle)
        {
            if (prescaler > 0)
                prescaler -= 3;
            else
            {
                prescaler += 341;
                irq_counter++;
                if (irq_counter == 0xFF)
                {
                    dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
                    irq_counter = irq_reload;
                }
            }
        }
        else
        {
            irq_counter++;
            if (irq_counter == 0xFF)
            {
                dev_cpu ->pin_irq |= IRQ_FLAG_BOARD;
                irq_counter = irq_reload;
            }
        }
    }
}
void Mapper026::ClockExternalSoundChannelsSingle(bool& is_clocking_duration)
{
    pulse1->Clock();
    pulse2->Clock();
    sawtooth->Clock();
}
double Mapper026::GetExternalSoundChannelsOutput()
{
    return mix_table[pulse1->Output]
           [pulse2->Output]
           [sawtooth->Output];
}
void Mapper026::SaveState()
{
    Board::SaveState();
    state_str->Write(irq_reload);
    state_str->Write(irq_counter);
    state_str->Write(irq_mode_cycle);
    state_str->Write(irq_enable);
    state_str->Write(irq_enable_on_ak);
    pulse1->SaveState();
    pulse2->SaveState();
    sawtooth->SaveState();
}
void Mapper026::LoadState()
{
    Board::LoadState();
    state_str->Read(irq_reload);
    state_str->Read(irq_counter);
    state_str->Read(irq_mode_cycle);
    state_str->Read(irq_enable);
    state_str->Read(irq_enable_on_ak);
    pulse1->LoadState();
    pulse2->LoadState();
    sawtooth->LoadState();
}
}

