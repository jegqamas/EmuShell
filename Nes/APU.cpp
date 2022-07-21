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
#include "Nes.h"
#include "../Definations.h"
#include "../Common/ConfigHandler.h"
namespace NES
{
const uint32_t APU2A03::SequenceMode0[3][7] =
{
    { 7459, 7456, 7458, 7457, 1, 1, 7457 }, // NTSC
    { 8315, 8314, 8312, 8313, 1, 1, 8313 }, // PALB
    { 7459, 7456, 7458, 7457, 1, 1, 7457 }  // DENDY (acts like NTSC)
};

const uint32_t APU2A03::SequenceMode1[3][5] =
{
    { 1, 7458, 7456, 7458, 14910 }, // NTSC
    { 1, 8314, 8314, 8312, 16626 }, // PALB
    { 1, 7458, 7456, 7458, 14910 }  // DENDY (acts like NTSC)
};

APU2A03::~APU2A03()
{
    delete [] channel_sq1;
    delete [] channel_sq2;
    delete [] channel_trl;
    delete [] channel_nos;
    delete [] channel_dmc;
    delete [] blip_buffer;
    delete [] playback_buffer;
    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            for (int trl = 0; trl < 16; trl++)
            {
                for (int noz = 0; noz < 16; noz++)
                {
                    delete [] dac_table[sq1][sq2][trl][noz];
                }
                delete [] dac_table[sq1][sq2][trl];
            }
            delete [] dac_table[sq1][sq2];
        }
        delete dac_table[sq1];
    }
    delete [] dac_table;
}
void APU2A03::BuildDACTable()
{
    dac_table = new double****[16];
    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        dac_table[sq1] = new double***[16];
        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            dac_table[sq1][sq2] = new double**[16];
            for (int trl = 0; trl < 16; trl++)
            {
                dac_table[sq1][sq2][trl] = new double*[16];
                for (int noz = 0; noz < 16; noz++)
                {
                    dac_table[sq1][sq2][trl][noz] = new double[128];
                    for (int dmc = 0; dmc < 128; dmc++)
                    {
                        double rgroup1_dac_output = (8128.0 / ((double)sq1 + (double)sq2));
                        rgroup1_dac_output = 95.88 / (rgroup1_dac_output + 100.0);

                        double rgroup2_dac_output = 1.0 / (((double)dmc / 22638.0) + ((double)trl / 8827.0) + ((double)noz / 12241.0));
                        rgroup2_dac_output = 159.79 / (rgroup2_dac_output + 100.0);

                        dac_table[sq1][sq2][trl][noz][dmc] =rgroup1_dac_output + rgroup2_dac_output ;
                    }
                }
            }
        }
    }
}
void APU2A03::load_settings()
{
    blip_buffer_bass_freq = CONFIG::GetInt32Value("blip_buffer_bass_freq",16);
    blip_buffer_treble_eq= CONFIG::GetDoubleValue("blip_buffer_treble_eq",-8.0);
    blip_buffer_volume= CONFIG::GetDoubleValue("blip_buffer_volume",0.06);
    blip_buffer_amp = CONFIG::GetInt32Value("blip_buffer_amp",256);
    blip_buffer_size_mesc = CONFIG::GetInt32Value("blip_buffer_size_mesc",500);
}
void APU2A03::save_settings()
{
    CONFIG::SetValue("blip_buffer_bass_freq",blip_buffer_bass_freq);
    CONFIG::SetValue("blip_buffer_treble_eq",blip_buffer_treble_eq);
    CONFIG::SetValue("blip_buffer_volume",blip_buffer_volume);
    CONFIG::SetValue("blip_buffer_amp",blip_buffer_amp);
    CONFIG::SetValue("blip_buffer_size_mesc",blip_buffer_size_mesc);
}

void APU2A03::Initialize(int system)
{
    load_settings();

    systemIndex = system;
    BuildDACTable();

    channel_sq1 = new ChannelPulse(false);
    channel_sq2 = new ChannelPulse(true);
    channel_trl = new ChannelTriangle();
    channel_nos = new ChannelNoise();
    channel_dmc = new ChannelDMC();

    blip_buffer = new Blip_Buffer();
}
void APU2A03::ShutDown()
{
    save_settings();
}
void APU2A03::SetupSoundPlayback(const bool& soundEnabled, const uint32_t& frequency)
{
    playback_buffer_frequency = frequency;
    SoundEnabled = soundEnabled;
    x = x_1 = 0;

    switch (systemIndex)
    {
    case TVSYSTEM_NTSC:
        blip_buffer->clock_rate(NES_CPU_SPEED_NTSC);
        break;
    case TVSYSTEM_PALB:
        blip_buffer->clock_rate(NES_CPU_SPEED_PALB);
        break;
    case TVSYSTEM_DENDY:
        blip_buffer->clock_rate(NES_CPU_SPEED_DENDY);
        break;
    }

    playback_amplitude = blip_buffer_amp;
    blip_buffer->set_sample_rate(playback_buffer_frequency, blip_buffer_size_mesc);
    blip_buffer->bass_freq(blip_buffer_bass_freq);
    blip_buffer_synth.treble_eq(blip_buffer_treble_eq);
    blip_buffer_synth.volume(blip_buffer_volume);
    blip_buffer_synth.output(blip_buffer);
}
void APU2A03::HardReset()
{
    blip_buffer->clear();
    playback_blipbuffer_timer = 0;
    PlaybackSampleNeeded = false;
    x = x_1 = 0;

    Cycles = SequenceMode0[systemIndex][0] - 10;
    FrameIrqFlag = false;
    FrameIrqEnabled = true;
    SequencingMode = false;
    CurrentSeq = 0;
    oddCycle = false;
    isClockingDuration = false;

    channel_sq1->HardReset();
    channel_sq2->HardReset();
    channel_trl->HardReset();
    channel_nos->HardReset();
    channel_dmc->HardReset();
}
void APU2A03::SoftReset()
{
    Cycles = SequenceMode0[systemIndex][0] - 10;
    FrameIrqFlag = false;
    FrameIrqEnabled = true;
    SequencingMode = false;
    CurrentSeq = 0;
    oddCycle = false;
    isClockingDuration = false;

    channel_sq1->SoftReset();
    channel_sq2->SoftReset();
    channel_trl->SoftReset();
    channel_nos->SoftReset();
    channel_dmc->SoftReset();
}
// Clock the apu one CPU cycle.
void APU2A03::Clock()
{
    isClockingDuration = false;
    Cycles--;
    oddCycle = !oddCycle;

    if (Cycles == 0)
    {
        if (!SequencingMode)
        {
            switch (CurrentSeq)
            {
            case 0:
            {
                ClockEnvelope();
                break;
            }
            case 1:
            {
                ClockDuration();
                isClockingDuration = true;
                break;
            }
            case 2:
            {
                ClockEnvelope();
                break;
            }
            case 3:
            {
                CheckIRQ();
                break;
            }
            case 4:
            {
                CheckIRQ();
                ClockDuration();
                isClockingDuration = true;
                break;
            }
            case 5:
            {
                CheckIRQ();
                break;
            }
            }
            CurrentSeq++;
            Cycles += SequenceMode0[systemIndex][CurrentSeq];
            if (CurrentSeq == 6)
                CurrentSeq = 0;
        }
        else
        {
            switch (CurrentSeq)
            {
            case 0:
            case 2:
            {
                ClockDuration();
                isClockingDuration = true;
                break;
            }
            case 1:
            case 3:
            {
                ClockEnvelope();
                break;
            }
            }
            CurrentSeq++;
            Cycles = SequenceMode1[systemIndex][CurrentSeq];
            if (CurrentSeq == 4)
                CurrentSeq = 0;
        }
    }
    // Clock single
    channel_sq1->Clock(isClockingDuration);
    channel_sq2->Clock(isClockingDuration);
    channel_trl->Clock(isClockingDuration);
    channel_nos->Clock(isClockingDuration);
    channel_dmc->Clock(isClockingDuration);
    if (UseExternalSoundChannels)
        dev_board->ClockExternalSoundChannelsSingle(isClockingDuration);
    // Playback
    ClockPlayback();
}
void APU2A03::ClockEnvelope()
{
    channel_sq1->ClockEnvelope();
    channel_sq2->ClockEnvelope();
    channel_trl->ClockEnvelope();
    channel_nos->ClockEnvelope();
    // No clock for DMC
}
void APU2A03::ClockDuration()
{
    ClockEnvelope();
    channel_sq1->ClockDuration();
    channel_sq2->ClockDuration();
    channel_trl->ClockDuration();
    channel_nos->ClockDuration();
    // No clock for DMC
}
void APU2A03::CheckIRQ()
{
    if (FrameIrqEnabled)
        FrameIrqFlag = true;
    if (FrameIrqFlag)
    {
        dev_cpu->pin_irq |= IRQ_FLAG_APU;
    }
}
void APU2A03::ClockPlayback()
{
    playback_blipbuffer_timer++;
    if (PlaybackSampleNeeded)
    {
        PlaybackSampleNeeded = false;
        // Collect the sample
        if (UseExternalSoundChannels)
        {
            x=dac_table
              [channel_sq1->Output]
              [channel_sq2->Output]
              [channel_trl->Output]
              [channel_nos->Output]
              [channel_dmc->Output]+
              dev_board->GetExternalSoundChannelsOutput();
        }
        else
        {
            x=dac_table
              [channel_sq1->Output]
              [channel_sq2->Output]
              [channel_trl->Output]
              [channel_nos->Output]
              [channel_dmc->Output];
        }

        x = x * playback_amplitude;
        // Add delta to the blip-buffer
        if (x != x_1)
        {
            blip_buffer_synth.offset(playback_blipbuffer_timer, (int)(x - x_1));
            x_1 = x;
        }
    }
}
void APU2A03::Write4000(uint8_t& value)
{
    channel_sq1->WriteReg1(value);
}
void APU2A03::Write4001(uint8_t& value)
{
    channel_sq1->WriteReg2(value);
}
void APU2A03::Write4002(uint8_t& value)
{
    channel_sq1->WriteReg3(value);
}
void APU2A03::Write4003(uint8_t& value)
{
    channel_sq1->WriteReg4(value);
}
void APU2A03::Write4004(uint8_t& value)
{
    channel_sq2->WriteReg1(value);
}
void APU2A03::Write4005(uint8_t& value)
{
    channel_sq2->WriteReg2(value);
}
void APU2A03::Write4006(uint8_t& value)
{
    channel_sq2->WriteReg3(value);
}
void APU2A03::Write4007(uint8_t& value)
{
    channel_sq2->WriteReg4(value);
}
void APU2A03::Write4008(uint8_t& value)
{
    channel_trl->WriteReg1(value);
}
void APU2A03::Write400A(uint8_t& value)
{
    channel_trl->WriteReg3(value);
}
void APU2A03::Write400B(uint8_t& value)
{
    channel_trl->WriteReg4(value);
}
void APU2A03::Write400C(uint8_t& value)
{
    channel_nos->WriteReg1(value);
}
void APU2A03::Write400E(uint8_t& value)
{
    channel_nos->WriteReg3(value);
}
void APU2A03::Write400F(uint8_t& value)
{
    channel_nos->WriteReg4(value);
}
void APU2A03::Write4010(uint8_t& value)
{
    channel_dmc->WriteReg1(value);
}
void APU2A03::Write4011(uint8_t& value)
{
    channel_dmc->WriteReg2(value);
}
void APU2A03::Write4012(uint8_t& value)
{
    channel_dmc->WriteReg3(value);
}
void APU2A03::Write4013(uint8_t& value)
{
    channel_dmc->WriteReg4(value);
}
void APU2A03::Write4015(uint8_t& value)
{
    channel_sq1->SetEnable((value & 0x01) != 0);
    channel_sq2->SetEnable((value & 0x02) != 0);
    channel_trl->SetEnable((value & 0x04) != 0);
    channel_nos->SetEnable((value & 0x08) != 0);
    channel_dmc->SetEnable((value & 0x10) != 0);
}
void APU2A03::Write4017(uint8_t& value)
{
    SequencingMode = (value & 0x80) != 0;
    FrameIrqEnabled = (value & 0x40) == 0;

    CurrentSeq = 0;

    if (!SequencingMode)
        Cycles = SequenceMode0[systemIndex][0];
    else
        Cycles = SequenceMode1[systemIndex][0];

    if (!oddCycle)
        Cycles++;
    else
        Cycles += 2;

    if (!FrameIrqEnabled)
    {
        FrameIrqFlag = false;

        dev_cpu->pin_irq &= IRQ_FLAG_APU_OFF;
    }
}
uint8_t APU2A03::Read4015()
{
    temp_4015 = 0;
    // Channels enable
    if (channel_sq1->IsEnabled()) temp_4015 |= 0x01;
    if (channel_sq2->IsEnabled()) temp_4015 |= 0x02;
    if (channel_trl->IsEnabled()) temp_4015 |= 0x04;
    if (channel_nos->IsEnabled()) temp_4015 |= 0x08;
    if (channel_dmc->IsEnabled()) temp_4015 |= 0x10;
    // IRQ
    if (FrameIrqFlag)
    {
        temp_4015 |= 0x40;
    }

    if (channel_dmc->IsIRQDeltaOccur())
    {
        temp_4015 |= 0x80;
    }

    FrameIrqFlag = false;

    dev_cpu->pin_irq &= IRQ_FLAG_APU_OFF;

    return temp_4015;
}
void APU2A03::DoDMCDMA()
{
    channel_dmc->DoDMA();
}

void APU2A03::FinishFrame()
{
    if (SoundEnabled)
    {
        if (!AudioOut::IsPlaying)
            AudioOut::Play();

        // Blip buffer
        blip_buffer->end_frame(playback_blipbuffer_timer);
        playback_blipbuffer_timer = 0;

        // Submit call
        long samples_count = blip_buffer->samples_avail();

        playback_buffer = new short[samples_count];
        blip_buffer->read_samples(playback_buffer, samples_count);

        for (int i = 0; i < samples_count; i++)
            AudioOut::PutSample(playback_buffer[i]);
    }
}
void APU2A03::SaveState()
{
    channel_sq1->SaveState();
    channel_sq2->SaveState();
    channel_trl->SaveState();
    channel_nos->SaveState();
    channel_dmc->SaveState();
    state_str->Write(oddCycle);
    state_str->Write(Cycles);
    state_str->Write(SequencingMode);
    state_str->Write(CurrentSeq);
    state_str->Write(isClockingDuration);
    state_str->Write(FrameIrqEnabled);
    state_str->Write(FrameIrqFlag);
}
void APU2A03::LoadState()
{
    channel_sq1->LoadState();
    channel_sq2->LoadState();
    channel_trl->LoadState();
    channel_nos->LoadState();
    channel_dmc->LoadState();
    state_str->Read(oddCycle);
    state_str->Read(Cycles);
    state_str->Read(SequencingMode);
    state_str->Read(CurrentSeq);
    state_str->Read(isClockingDuration);
    state_str->Read(FrameIrqEnabled);
    state_str->Read(FrameIrqFlag);
}
}
