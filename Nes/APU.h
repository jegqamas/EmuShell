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
#include <stdint.h>
#include "../Providers/AudioOut.h"
#include "../Providers/Blip_Buffer/Blip_Buffer.h"

namespace NES
{
class APUInternalChannel
{
public:
    ~APUInternalChannel();
    virtual void HardReset();// Hard reset the channel
    virtual void SoftReset();// Soft reset the channel
    virtual void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE

    virtual void ClockEnvelope();
    virtual void ClockDuration();
    virtual void SaveState();
    virtual void LoadState();
    virtual void WriteReg1(uint8_t& value);// Write at register #1
    virtual void WriteReg2(uint8_t& value);// Write at register #2
    virtual void WriteReg3(uint8_t& value);// Write at register #3
    virtual void WriteReg4(uint8_t& value);// Write at register #4

    virtual void SetEnable(bool enable);
    virtual bool IsEnabled();
    virtual bool IsIRQDeltaOccur();// Only for DMC channel, check the irq flag status

    int Output;

protected:
    static const uint8_t DurationTable[32];
    uint32_t envelope;
    bool env_startflag;
    bool constant_volume_flag;
    uint32_t volume_decay_time;
    uint32_t env_counter;
    uint32_t env_devider;
    bool length_counter_halt_flag;
    bool duration_haltRequset;
    uint32_t duration_counter;
    bool duration_reloadEnabled;
    uint32_t  duration_reload;
    bool duration_reloadRequst;
};

// INTERNAL CHANNELS /////////////////////////////////////////////
class ChannelPulse : public APUInternalChannel
{
public:
    // Initialize the channel pulse
    // sweep_mode_2: false= this is square channel 1, true= this is square channel 2
    ChannelPulse(bool sweep_mode_2)
    {
        this->sweep_mode_2 = sweep_mode_2;
    }
    ~ChannelPulse();
    void HardReset();// Hard reset the channel
    void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE
    void ClockDuration();
    void SaveState();
    void LoadState();
    void WriteReg1(uint8_t& value);// Write at register #1
    void WriteReg2(uint8_t& value);// Write at register #2
    void WriteReg3(uint8_t& value);// Write at register #3
    void WriteReg4(uint8_t& value);// Write at register #4

private:
    static const uint8_t DutyForms[4][8];
    bool sweep_mode_2;
    uint32_t dutyForm;
    uint32_t dutyStep;
    uint32_t sweepDeviderPeriod;
    uint32_t sweepShiftCount;
    uint32_t sweepCounter ;
    bool sweepEnable;
    bool sweepReload;
    bool sweepNegateFlag;
    uint32_t frequency;
    uint32_t sweep;
    uint32_t cycles;
    bool IsValidFrequency();
};

class ChannelTriangle : public APUInternalChannel
{
public:
    ~ChannelTriangle();
    void HardReset();// Hard reset the channel
    void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE
    void ClockEnvelope();

    void WriteReg1(uint8_t& value);// Write at register #1
    void WriteReg3(uint8_t& value);// Write at register #3
    void WriteReg4(uint8_t& value);// Write at register #4

    void SaveState();
    void LoadState();
private:
    static const uint8_t DurationTable[32];
    static const uint8_t StepSequence[32];
    uint8_t linearCounter;
    uint8_t linearCounterReload;
    uint8_t step;
    bool linearCounterHalt;
    bool halt;
    uint32_t frequency;
    uint32_t cycles;
};

class ChannelNoise : public APUInternalChannel
{
public:
    ~ChannelNoise();
    void HardReset();// Hard reset the channel
    void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE
    void WriteReg3(uint8_t& value);// Write at register #3
    void SaveState();
    void LoadState();
private:
    static const uint32_t FrequencyTable[3][16];
    bool mode;
    uint32_t shiftRegister;
    uint32_t feedback;
    uint32_t frequency;
    uint32_t cycles;
};

class ChannelDMC : public APUInternalChannel
{
public:
    ~ChannelDMC();
    void HardReset();// Hard reset the channel
    void SoftReset();// Soft reset the channel
    void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE
    void DoDMA();
    void WriteReg1(uint8_t& value);// Write at register #1
    void WriteReg2(uint8_t& value);// Write at register #2
    void WriteReg3(uint8_t& value);// Write at register #3
    void WriteReg4(uint8_t& value);// Write at register #4
    void SaveState();
    void LoadState();
    void SetEnable(bool enable);
    bool IsEnabled();
    bool IsIRQDeltaOccur();// Only for DMC channel, check the irq flag status
private:
    static const uint32_t FrequencyTable[3][16];
    bool DeltaIrqOccur;
    bool DMCIrqEnabled;
    bool dmaLooping;
    bool dmaEnabled;
    bool bufferFull ;
    uint32_t dmaAddrRefresh;
    uint32_t dmaSizeRefresh;
    uint32_t dmaSize;
    uint32_t dmaBits;
    uint8_t dmaByte ;
    uint32_t dmaAddr;
    uint8_t dmaBuffer;
    uint8_t dmc_output;
    uint32_t cycles;
    uint32_t freqTimer;
};

// EXTERNAL CHANNELS ////////////////////////////////////////////
// MMC5 (Mapper 5) //////////////////////////////////////////////
class MMC5ChannelPulse : public APUInternalChannel
{
public:
    ~MMC5ChannelPulse();
    void HardReset();// Hard reset the channel
    void Clock(bool& is_clocking_duration);// Clock the channel one CPU CYCLE
    void SaveState();
    void LoadState();
    void Write5000(uint8_t& value);// Write at register #1
    void Write5002(uint8_t& value);// Write at register #3
    void Write5003(uint8_t& value);// Write at register #4

private:
    static const uint8_t DutyForms[4][8];
    uint32_t dutyForm;
    uint32_t dutyStep;
    uint32_t frequency;
    uint32_t cycles;
};

class MMC5ChannelPCM : public APUInternalChannel
{
public:
    uint8_t output;
    void HardReset();
    void SoftReset();
    void Write5010(uint8_t& data);
    uint8_t Read5010();
    void Write5011(uint8_t& data);
    void SaveState();
    void LoadState();
private:
    bool readMode;
    bool PCMIRQenable;
    bool irqTrip;
};

// VRC6 (Mapper 24) /////////////////////////////////////////////
class VRC6ChannelPulse
{
public:
    void HardReset();// Hard reset the channel
    void Clock();// Clock the channel one CPU CYCLE
    void SaveState();
    void LoadState();
    void WriteReg1(uint8_t& value);// Write at register #1
    void WriteReg2(uint8_t& value);// Write at register #2
    void WriteReg3(uint8_t& value);// Write at register #3
    uint8_t Output;
private:
    uint8_t dutyForm;
    uint8_t dutyStep;
    uint32_t freqTimer;
    uint32_t frequency;
    uint32_t cycles;
    bool enabled ;
    bool mode ;
    uint8_t volume;
};

class VRC6ChannelSawtooth
{
public:
    void HardReset();
    void Clock();
    void SaveState();
    void LoadState();
    void WriteReg1(uint8_t& value);
    void WriteReg2(uint8_t& value);
    void WriteReg3(uint8_t& value);
    uint8_t Output;
private:
    uint8_t AccumRate;
    uint32_t accumClock;
    uint8_t accumulationRegister;
    uint32_t frequency;
    uint32_t freqTimer;
    uint32_t cycles;
    bool enabled;
    void UpdateFrequency();
};

// Sunsoft 5B (Mapper 69) ///////////////////////////////////////
class Sunsoft5BChannel
{
public:
    void HardReset();
    void SoftReset();
    void ClockSingle();
    void SaveState();
    void LoadState();
    void Write0(uint8_t& value);
    void Write1(uint8_t& value);
    uint8_t output;
    bool Enabled;
    uint8_t Volume;
private:
    uint32_t dutyStep;
    uint32_t freqTimer;
    uint32_t frequency;
    uint32_t cycles;
};

// THE APU /////////////////////////////////////////////////////
class APU2A03
{
public:
    ~APU2A03();
    void Initialize(int system);
    void ShutDown();
    void SetupSoundPlayback(const bool& soundEnabled, const uint32_t& frequency);
    void HardReset();// Hard reset the apu
    void SoftReset();// Soft reset the apu
    void Clock();// Clock apu one CPU CYCLE
    void SaveState();
    void LoadState();
    void Write4000(uint8_t& value);// Write at register $4000
    void Write4001(uint8_t& value);// Write at register $4001
    void Write4002(uint8_t& value);// Write at register $4002
    void Write4003(uint8_t& value);// Write at register $4003
    void Write4004(uint8_t& value);// Write at register $4004
    void Write4005(uint8_t& value);// Write at register $4005
    void Write4006(uint8_t& value);// Write at register $4006
    void Write4007(uint8_t& value);// Write at register $4007
    void Write4008(uint8_t& value);// Write at register $4008
    void Write400A(uint8_t& value);// Write at register $400A
    void Write400B(uint8_t& value);// Write at register $400B
    void Write400C(uint8_t& value);// Write at register $400C
    void Write400E(uint8_t& value);// Write at register $400E
    void Write400F(uint8_t& value);// Write at register $400F
    void Write4010(uint8_t& value);// Write at register $4010
    void Write4011(uint8_t& value);// Write at register $4011
    void Write4012(uint8_t& value);// Write at register $4012
    void Write4013(uint8_t& value);// Write at register $4013
    void Write4015(uint8_t& value);// Write at register $4015
    void Write4017(uint8_t& value);// Write at register $4017
    void DoDMCDMA();
    void FinishFrame();
    uint8_t Read4015();// Read at register $4015
    uint32_t systemIndex;
    bool PlaybackSampleNeeded;
    bool oddCycle;
    bool SoundEnabled;
    bool UseExternalSoundChannels;
private:
    void load_settings();
    void save_settings();
    static const uint32_t SequenceMode0[3][7];
    static const uint32_t SequenceMode1[3][5];
    ChannelPulse* channel_sq1;
    ChannelPulse* channel_sq2;
    ChannelTriangle* channel_trl;
    ChannelNoise* channel_nos;
    ChannelDMC* channel_dmc;
    Blip_Buffer* blip_buffer;
    Blip_Synth<blip_best_quality, 20> blip_buffer_synth;
    int blip_buffer_bass_freq;
    int blip_buffer_amp;
    int blip_buffer_size_mesc;
    double blip_buffer_treble_eq;
    double blip_buffer_volume;
    double***** dac_table;
    uint32_t Cycles ;
    uint8_t temp_4015;
    bool SequencingMode;
    uint8_t CurrentSeq ;
    bool isClockingDuration ;
    bool FrameIrqEnabled;
    bool FrameIrqFlag;
    uint32_t playback_buffer_frequency;
    uint32_t playback_blipbuffer_timer;
    short* playback_buffer;
    double playback_amplitude;
    double x, x_1;
    void BuildDACTable();
    void ClockEnvelope();
    void ClockDuration();
    void CheckIRQ();
    void ClockPlayback();
};
}
