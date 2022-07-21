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

#include "_Mappers.h"
#include "../../Definations.h"

namespace NES
{
Mapper069::Mapper069()
{
    this->Name = "FME-7/Sunsoft 5B";
    this->MapperNumber = 69;
    this->InitializeSoundMixTable();
    this->channel0 = new Sunsoft5BChannel();
    this->channel1 = new Sunsoft5BChannel();
    this->channel2 = new Sunsoft5BChannel();
}
Mapper069::~Mapper069()
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
    delete [] channel0;
    delete [] channel1;
    delete [] channel2;
}
void Mapper069::InitializeSoundMixTable()
{
    mix_table = new double**[16];

    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        mix_table[sq1] = new double*[16];

        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            mix_table[sq1][sq2] = new double[16];

            for (int sq3 = 0; sq3 < 16; sq3++)
            {
                double sqr = (95.88 / (8128.0 / (sq1 + sq2 + sq3) + 100));
                mix_table[sq1][sq2][sq3] = sqr;
            }
        }
    }
}
void Mapper069::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
    address_A000 = 0;
    irq_counter = 0xFFFF;
    irq_count_enabled = false;
    irq_trigger_enabled = false;

    channel0->HardReset();
    channel1->HardReset();
    channel2->HardReset();

    dev_apu->UseExternalSoundChannels=true;
}
void Mapper069::SoftReset()
{
    Board::SoftReset();
    channel0->SoftReset();
    channel1->SoftReset();
    channel2->SoftReset();
}
void Mapper069::WritePRG(uint16_t& address, uint8_t& value)
{
    switch (address & 0xE000)
    {
    case 0x8000:
    {
        address_A000 = value & 0xF;
        break;
    }
    case 0xA000:
    {
        switch (address_A000)
        {
        case 0x0:
            Switch01KCHR(value, 0x0000, chr_01K_rom_count > 0);
            break;
        case 0x1:
            Switch01KCHR(value, 0x0400, chr_01K_rom_count > 0);
            break;
        case 0x2:
            Switch01KCHR(value, 0x0800, chr_01K_rom_count > 0);
            break;
        case 0x3:
            Switch01KCHR(value, 0x0C00, chr_01K_rom_count > 0);
            break;
        case 0x4:
            Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
            break;
        case 0x5:
            Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
            break;
        case 0x6:
            Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
            break;
        case 0x7:
            Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
            break;
        case 0x8:
        {
            TogglePRGRAMEnable((value & 0x80) == 0x80);
            Switch08KPRG(value & 0x3F, 0x6000, (value & 0x40) == 0);
            break;
        }
        case 0x9:
            Switch08KPRG(value, 0x8000, true);
            break;
        case 0xA:
            Switch08KPRG(value, 0xA000, true);
            break;
        case 0xB:
            Switch08KPRG(value, 0xC000, true);
            break;
        case 0xC:
        {
            switch (value & 0x3)
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
        case 0xD:
        {
            irq_count_enabled = (value & 0x80) == 0x80;
            irq_trigger_enabled = (value & 0x1) == 0x1;
            if (!irq_trigger_enabled)
                dev_cpu->pin_irq &=IRQ_FLAG_BOARD_OFF;
            break;
        }
        case 0xE:
        {
            irq_counter = (irq_counter & 0xFF00) | value;
            break;
        }
        case 0xF:
        {
            irq_counter = (irq_counter & 0x00FF) | (value << 8);
            break;
        }
        }
        break;
    }
    case 0xC000:
    {
        address_E000 = value & 0xF;
        break;
    }
    case 0xE000:
    {
        switch (address_E000)
        {
        case 0x0:
            channel0->Write0( value);
            break;
        case 0x1:
            channel0->Write1( value);
            break;
        case 0x2:
            channel1->Write0( value);
            break;
        case 0x3:
            channel1->Write1( value);
            break;
        case 0x4:
            channel2->Write0( value);
            break;
        case 0x5:
            channel2->Write1( value);
            break;
        case 0x7:
        {
            channel0->Enabled = (value & 0x1) == 0;
            channel1->Enabled = (value & 0x2) == 0;
            channel2->Enabled = (value & 0x4) == 0;
            break;
        }
        case 0x8:
        {
            channel0->Volume = (value & 0xF);
            break;
        }
        case 0x9:
        {
            channel1->Volume = (value & 0xF);
            break;
        }
        case 0xA:
        {
            channel2->Volume = (value & 0xF);
            break;
        }
        }
        break;
    }
    }
}
void Mapper069::OnCPUClock()
{
    if (irq_count_enabled)
    {
        irq_counter--;
        if (irq_counter == 0)
        {
            irq_counter = 0xFFFF;
            if (irq_trigger_enabled)
                dev_cpu->pin_irq |=IRQ_FLAG_BOARD;
        }
    }
}
void Mapper069::ClockExternalSoundChannelsSingle(bool& is_clocking_duration)
{
    channel0->ClockSingle();
    channel1->ClockSingle();
    channel2->ClockSingle();
}
double Mapper069::GetExternalSoundChannelsOutput()
{
    return mix_table[channel0->output]
           [channel1->output]
           [channel2->output];
}
void Mapper069::SaveState()
{
    Board::SaveState();
    state_str->Write(address_A000);
    state_str->Write( address_E000);
    state_str->Write( irq_counter);
    state_str->Write( irq_count_enabled);
    state_str->Write( irq_trigger_enabled);
    channel0->SaveState();
    channel1->SaveState();
    channel2->SaveState();
}
void Mapper069::LoadState()
{
    Board::LoadState();
    state_str->Read(address_A000);
    state_str->Read( address_E000);
    state_str->Read( irq_counter);
    state_str->Read( irq_count_enabled);
    state_str->Read( irq_trigger_enabled);
    channel0->LoadState();
    channel1->LoadState();
    channel2->LoadState();
}
}

