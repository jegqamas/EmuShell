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
#include "../Nes.h"
#include "EPROM.h"

namespace NES
{
// TODO: shared mappers header is not a good idea ...
class Mapper000 : public Board
{
public:
    Mapper000();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper001 : public Board
{
public:
    Mapper001();
    void WritePRG(uint16_t& address, uint8_t& value);
    void HardReset();
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    uint8_t address_reg;
    uint8_t reg[4] ;
    uint8_t shift;
    uint8_t buffer;
    bool flag_p;
    bool flag_c;
    bool flag_s;
    bool enable_wram_enable;
    uint8_t prg_hijackedbit;
    bool use_hijacked;
    bool use_sram_switch;
    uint8_t sram_switch_mask;
    uint8_t cpuCycles;
    void UpdateCHR();
    void UpdatePRG();
};

class Mapper002 : public Board
{
public:
    Mapper002();
    void WritePRG(uint16_t& address, uint8_t& value);
    void HardReset();
};

class Mapper003 : public Board
{
public:
    Mapper003();
    void WritePRG(uint16_t& address, uint8_t& value);
private:
    uint8_t data_temp;
};

class Mapper004 : public Board
{
public:
    Mapper004();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper005 : public Board
{
public:
    Mapper005();
    ~Mapper005();
    void HardReset();
    void SoftReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    uint8_t ReadEXP(uint16_t& address);
    uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    uint8_t ReadNMT(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    void OnPPUScanlineTick();
    void SaveState();
    void LoadState();
    void ClockExternalSoundChannelsSingle(bool& is_clocking_duration);
    void ClockExternalSoundChannelsEnvelope();
    void ClockExternalSoundChannelsDuration();
    double GetExternalSoundChannelsOutput();
private:
    uint32_t ram_protectA;
    uint32_t ram_protectB;
    uint32_t ExRAM_mode;
    uint32_t CHROffset_spr[8];
    uint32_t CHROffsetEX[8];
    uint32_t CHROffsetSP[8];
    uint32_t chrRegA[8];
    uint32_t chrRegB[4];
    uint32_t prgReg[4];
    bool useSRAMmirroring;
    uint32_t chr_high;
    uint32_t chr_mode;
    unsigned  int prg_mode;
    bool chr_setB_last;
    uint8_t temp_val;
    uint8_t temp_fill;
    uint32_t lastAccessVRAM;
    uint32_t paletteNo;
    uint32_t shift;
    uint32_t EXtilenumber;
    uint8_t multiplicand;
    uint8_t multiplier;
    uint16_t product;
    bool split_enable;
    bool split_right;
    uint32_t split_tile;
    uint32_t split_yscroll;// The y scroll value for split.
    bool split_doit;// Set to true to make nt changes; get split happening. Used in CHR read access.
    uint32_t split_watch_tile;// A temp tile number, for the split.
    uint8_t irq_line;
    uint8_t irq_enable;
    uint32_t irq_pending;
    uint32_t irq_current_counter;
    uint32_t irq_current_inframe;
    double*** mix_table;
    MMC5ChannelPulse* channel_sq1;
    MMC5ChannelPulse* channel_sq2;
    MMC5ChannelPCM* channel_pcm;
    static const uint32_t sound_seq[7];
    uint32_t sound_seq_cyc;
    uint32_t sound_seq_curr;
    void InitializeSoundMixTable();
    void UpdateRamProtect();
    void Switch04kCHR_bkg(int index, int where);
    void Switch01kCHR_spr(int index, int where);
    void Switch02kCHR_spr(int index, int where);
    void Switch04kCHR_spr(int index, int where);
    void Switch08kCHR_spr(int index);
    void Switch04kCHREX(int index, int where);
    void Switch04kCHRSP(int index, int where);
};

class Mapper006 : public Board
{
public:
    Mapper006();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    bool irqEnable;
    uint32_t irqCounter;
};

class Mapper007 : public Board
{
public:
    Mapper007();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper008 : public Board
{
public:
    Mapper008();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    bool irqEnable;
    uint32_t irqCounter;
};

class Mapper009 : public Board
{
public:
    Mapper009();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    void SaveState();
    void LoadState();
private:
    uint8_t chr_reg0A;
    uint8_t chr_reg0B;
    uint8_t chr_reg1A;
    uint8_t chr_reg1B;
    uint8_t latch_a;
    uint8_t latch_b;
};

class Mapper010 : public Board
{
public:
    Mapper010();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    void SaveState();
    void LoadState();
private:
    uint8_t chr_reg0A;
    uint8_t chr_reg0B;
    uint8_t chr_reg1A;
    uint8_t chr_reg1B;
    uint8_t latch_a;
    uint8_t latch_b;
};

class Mapper011 : public Board
{
public:
    Mapper011();
    void WritePRG(uint16_t& address, uint8_t& value);
private :
    uint8_t writeData;
};

class Mapper013 : public Board
{
public:
    Mapper013();
    void WritePRG(uint16_t& address, uint8_t& value);
private :
    uint8_t writeData;
};

class Mapper015 : public Board
{
public:
    Mapper015();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t temp;
};

class Mapper016 : public Board
{
public:
    Mapper016();
    ~Mapper016();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    Eprom* eprom;
    bool irq_enable;
    uint32_t irq_counter;
};

class Mapper017 : public Board
{
public:
    Mapper017();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    bool irqEnable;
    uint32_t irqCounter;
};

class Mapper018 : public Board
{
public:
    Mapper018();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    uint32_t prg_reg[3];
    uint32_t chr_reg[8];
    uint32_t irqRelaod;
    uint32_t irqCounter;
    bool irqEnable;
    uint32_t irqMask;
};

class Mapper019 : public Board
{
public:
    Mapper019();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteEXP(uint16_t& address, uint8_t& value);
    uint8_t ReadEXP(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadNMT(uint16_t& address);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    uint32_t irq_counter;
    bool irq_enable;
    bool disables_chr_ram_A;
    bool disables_chr_ram_B;
    bool enable_mirroring_switch;
    uint32_t temp_nmt;
    uint8_t temp_val;
    uint8_t temp_i;
};

class Mapper021 : public Board
{
public:
    Mapper021();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    bool prg_mode;
    uint8_t prg_reg0;
    uint32_t chr_reg[8];
    uint32_t irq_reload;
    uint32_t irq_counter;
    int prescaler;
    bool irq_mode_cycle;
    bool irq_enable;
    bool irq_enable_on_ak;
};

class Mapper022 : public Board
{
public:
    Mapper022();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t chr_reg[8];
};

class Mapper023 : public Board
{
public:
    Mapper023();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    void SaveState();
    void LoadState();
private :
    uint32_t chr_reg[8];
    uint8_t security;
};

class Mapper024 : public Board
{
public:
    Mapper024();
    ~Mapper024();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void ClockExternalSoundChannelsSingle(bool& is_clocking_duration);
    double GetExternalSoundChannelsOutput();
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void InitializeSoundMixTable();
    uint32_t irq_reload;
    uint32_t irq_counter;
    int prescaler;
    bool irq_mode_cycle;
    bool irq_enable;
    bool irq_enable_on_ak;
    double*** mix_table;
    VRC6ChannelPulse * pulse1;
    VRC6ChannelPulse * pulse2;
    VRC6ChannelSawtooth * sawtooth;
};

class Mapper025 : public Board
{
public:
    Mapper025();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    bool prg_mode;
    uint8_t prg_reg0;
    uint32_t chr_reg[8];
    uint32_t irq_reload;
    uint32_t irq_counter;
    int prescaler;
    bool irq_mode_cycle;
    bool irq_enable;
    bool irq_enable_on_ak;
};

class Mapper026 : public Board
{
public:
    Mapper026();
    ~Mapper026();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void ClockExternalSoundChannelsSingle(bool& is_clocking_duration);
    double GetExternalSoundChannelsOutput();
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void InitializeSoundMixTable();
    uint32_t irq_reload;
    uint32_t irq_counter;
    int prescaler;
    bool irq_mode_cycle;
    bool irq_enable;
    bool irq_enable_on_ak;
    double*** mix_table;
    VRC6ChannelPulse * pulse1;
    VRC6ChannelPulse * pulse2;
    VRC6ChannelSawtooth * sawtooth;
};

class Mapper032 : public Board
{
public:
    Mapper032();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    bool prg_mode;
    uint8_t prg_reg0;
    bool enable_mirroring_switch;
};

class Mapper033 : public Board
{
public:
    Mapper033();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper034 : public Board
{
public:
    Mapper034();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
private :
    bool BxROM;
    uint8_t writeData;
};

class Mapper041 : public Board
{
public:
    Mapper041();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    bool enableReg;
    uint32_t vromReg ;
};

class Mapper042 : public Board
{
public:
    Mapper042();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    uint32_t SRAM_PRG_Page;
    bool irqEnable;
    uint32_t irqCounter;
};

class Mapper044 : public Board
{
public:
    Mapper044();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t block;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;

    uint32_t prg_and;
    uint32_t prg_or;
    uint32_t chr_and;
    uint32_t chr_or;
    void SetupCHR();
    void SetupPRG();
};

class Mapper045 : public Board
{
public:
    Mapper045();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    bool locked;
    uint32_t regCounter;
    uint32_t prg_and;
    uint32_t prg_or;
    uint32_t chr_and;
    uint32_t chr_or;
    void SetupCHR();
    void SetupPRG();
};

class Mapper046 : public Board
{
public:
    Mapper046();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t prg_reg;
    uint32_t chr_reg;
};

class Mapper047 : public Board
{
public:
    Mapper047();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t block;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;

    uint32_t prg_and;
    uint32_t prg_or;
    uint32_t chr_and;
    uint32_t chr_or;
    void SetupCHR();
    void SetupPRG();
};

class Mapper048 : public Board
{
public:
    Mapper048();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool MODE;// Mapper 33 [TC0350FMR] mode ?
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
};

class Mapper049 : public Board
{
public:
    Mapper049();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    bool prg_32Mode;
    uint32_t prg_32Page;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;

    uint32_t prg_and;
    uint32_t prg_or;
    uint32_t chr_and;
    uint32_t chr_or;
    void SetupCHR();
    void SetupPRG();
};

class Mapper050 : public Board
{
public:
    Mapper050();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    uint32_t prg_page;
    uint32_t irq_counter;
    bool irq_enabled;
};

class Mapper051 : public Board
{
public:
    Mapper051();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t bank;
    uint32_t mode;
    uint32_t offset;
    void UpdateBanks();
};

class Mapper052 : public Board
{
public:
    Mapper052();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    bool locked;
    uint32_t prg_and;
    uint32_t prg_or;
    uint32_t chr_and;
    uint32_t chr_or;
    void SetupCHR();
    void SetupPRG();
};

class Mapper057 : public Board
{
public:
    Mapper057();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t chr_aaa;
    uint32_t chr_bbb;
    uint32_t chr_hhh;
};

class Mapper058 : public Board
{
public:
    Mapper058();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper060 : public Board
{
public:
    Mapper060();
    void HardReset();
    void SoftReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadPRG(uint16_t& address);
    void SaveState();
    void LoadState();
private:
    uint32_t latch;
    uint8_t menu;
};

class Mapper061 : public Board
{
public:
    Mapper061();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper062 : public Board
{
public:
    Mapper062();
    void WritePRG(uint16_t& address, uint8_t& value);
private:
    uint32_t prg_page;
};

class Mapper064 : public Board
{
public:
    Mapper064();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    void SetupCHR();
    void SetupPRG();
    void ClockIRQ();
    bool flag_c;
    bool flag_p;
    bool flag_k;
    uint32_t address_8001;
    uint32_t chr_reg[8];
    uint32_t prg_reg[3];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint8_t irq_reload;
    bool irq_mode;
    bool irq_clear;
    uint32_t irq_prescaler;
};

class Mapper065 : public Board
{
public:
    Mapper065();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    bool irq_enable;
    uint32_t irq_reload;
    uint32_t irq_counter;
};

class Mapper066 : public Board
{
public:
    Mapper066();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper067 : public Board
{
public:
    Mapper067();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    bool irq_enabled;
    uint32_t irq_counter;
    bool odd;
};

class Mapper068 : public Board
{
public:
    Mapper068();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadNMT(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool flag_r;
    bool flag_m;
    uint32_t nt_reg0;
    uint32_t nt_reg1;
    uint32_t temp;
};

class Mapper069 : public Board
{
public:
    Mapper069();
    ~Mapper069();
    void HardReset();
    void SoftReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void ClockExternalSoundChannelsSingle(bool& is_clocking_duration);
    double GetExternalSoundChannelsOutput();
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void InitializeSoundMixTable();
    uint32_t address_A000;
    uint32_t address_E000;
    uint32_t irq_counter;
    bool irq_count_enabled;
    bool irq_trigger_enabled;
    Sunsoft5BChannel *channel0;
    Sunsoft5BChannel *channel1;
    Sunsoft5BChannel *channel2;
    double*** mix_table;
};

class Mapper070 : public Board
{
public:
    Mapper070();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper071 : public Board
{
public:
    Mapper071();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
private :
    bool fireHawk;
};

class Mapper072 : public Board
{
public:
    Mapper072();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t writeData;
    uint32_t chr_reg;
    uint32_t prg_reg;
};

class Mapper073 : public Board
{
public:
    Mapper073();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    bool irq_mode_8;
    bool irq_enable;
    bool irq_enable_on_ak;
    uint32_t irq_reload;
    uint32_t irq_counter;
};

class Mapper074 : public Board
{
public:
    Mapper074();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    void SetupCHR();
    void SetupPRG();
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint8_t irq_reload;
    bool irq_mode;
    bool irq_clear;
    uint8_t old_irq_counter;
    bool mmc3_alt_behavior;
};

class Mapper075 : public Board
{
public:
    Mapper075();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t chr0_reg;
    uint32_t chr1_reg;
};

class Mapper076 : public Board
{
public:
    Mapper076();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private :
    uint32_t address_8001;
    bool prg_a;
    uint8_t prg_reg;
};

class Mapper077 : public Board
{
public:
    Mapper077();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper078 : public Board
{
public:
    Mapper078();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
private:
    bool mirroring_mode_single;
};

class Mapper079 : public Board
{
public:
    Mapper079();
    void WriteEXP(uint16_t& address, uint8_t& value);
};

class Mapper080 : public Board
{
public:
    Mapper080();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper082 : public Board
{
public:
    Mapper082();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool chr_mode;
};

class Mapper085 : public Board
{
public:
    Mapper085();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    uint32_t irq_reload;
    uint32_t irq_counter;
    int prescaler;
    bool irq_mode_cycle;
    bool irq_enable;
    bool irq_enable_on_ak;
};

class Mapper086 : public Board
{
public:
    Mapper086();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper087 : public Board
{
public:
    Mapper087();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper088 : public Board
{
public:
    Mapper088();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t address_8001;
};

class Mapper089 : public Board
{
public:
    Mapper089();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper090 : public Board
{
public:
    Mapper090();
    void HardReset();
    void SoftReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    uint8_t ReadEXP(uint16_t& address);
    uint8_t ReadNMT(uint16_t& address);
    void OnPPUAddressUpdate(uint16_t& address);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void SetupPRG();
    void SetupCHR();
    uint8_t ReverseByte(uint8_t value);
    void ClockIRQCounter();
    bool MAPPER90MODE;// Setting this to true disables the extended nametables control.
    uint32_t prg_reg[4];
    uint32_t chr_reg[8];
    uint32_t nt_reg[4];
    uint32_t prg_mode;
    uint32_t chr_mode;
    bool chr_block_mode;
    uint32_t chr_block;
    bool chr_m;
    bool flag_s;
    uint32_t irqCounter;
    bool IrqEnable;
    bool irqCountDownMode;
    bool irqCountUpMode;
    bool irqFunkyMode;
    bool irqPrescalerSize;
    uint32_t irqSource;
    uint32_t irqPrescaler;
    uint32_t irqPrescalerXOR;
    uint8_t irqFunkyModeReg;
    uint8_t Dipswitch;
    uint8_t multiplication_a;
    uint8_t multiplication_b;
    uint16_t multiplication;
    uint8_t RAM5803;
    bool nt_advanced_enable;
    bool nt_rom_only;
    uint32_t nt_ram_select;
};

class Mapper091 : public Board
{
public:
    Mapper091();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
};

class Mapper092 : public Board
{
public:
    Mapper092();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t chr_reg;
    uint32_t prg_reg;
};

class Mapper093 : public Board
{
public:
    Mapper093();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper094 : public Board
{
public:
    Mapper094();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper095 : public Board
{
public:
    Mapper095();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadNMT(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper096 : public Board
{
public:
    Mapper096();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUAddressUpdate(uint16_t& address);
    void SaveState();
    void LoadState();
private:
    uint32_t flag_c;
};

class Mapper097 : public Board
{
public:
    Mapper097();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
};

// This mapper is for VS system, we need to declare it here then later config it with VS
class Mapper099 : public Board
{
public:
    Mapper099();
};

class Mapper105 : public Board
{
public:
    Mapper105();
    void HardReset();
    void SoftReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private:
    void UpdatePRG();
    uint32_t DipSwitchNumber;
    uint8_t reg[4];
    uint8_t shift;
    uint8_t buffer;
    bool flag_p;
    bool flag_s;
    bool flag_o;
    uint32_t reg_a;
    uint32_t reg_b;
    bool irq_control;
    bool initialized;
    uint32_t irq_counter;
    uint32_t dipswitches;
};

class Mapper107 : public Board
{
public:
    Mapper107();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper112 : public Board
{
public:
    Mapper112();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t address_A000;
};

class Mapper113 : public Board
{
public:
    Mapper113();
    void WriteEXP(uint16_t& address, uint8_t& value);
};

class Mapper115 : public Board
{
public:
    Mapper115();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    bool flag_o;
    uint32_t chr_block;
    uint32_t prg_secreg;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper118 : public Board
{
public:
    Mapper118();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    uint8_t ReadNMT(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper119 : public Board
{
public:
    Mapper119();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper133 : public Board
{
public:
    Mapper133();
    void WriteEXP(uint16_t& address, uint8_t& value);
};

class Mapper140 : public Board
{
public:
    Mapper140();
    void WriteEXP(uint16_t& address, uint8_t& value);
};

class Mapper154 : public Board
{
public:
    Mapper154();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t address_8001;
};

class Mapper159 : public Board
{
public:
    Mapper159();
    ~Mapper159();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    Eprom* eprom;
    bool irq_enable;
    uint32_t irq_counter;
};

class Mapper160 : public Board
{
public:
    Mapper160();
    void HardReset();
    void SoftReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    uint8_t ReadEXP(uint16_t& address);
    uint8_t ReadNMT(uint16_t& address);
    void OnPPUAddressUpdate(uint16_t& address);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void SetupPRG();
    void SetupCHR();
    uint8_t ReverseByte(uint8_t value);
    void ClockIRQCounter();
    bool MAPPER90MODE;// Setting this to true disables the extended nametables control.
    uint32_t prg_reg[4];
    uint32_t chr_reg[8];
    uint32_t nt_reg[4];
    uint32_t prg_mode;
    uint32_t chr_mode;
    bool chr_block_mode;
    uint32_t chr_block;
    bool chr_m;
    bool flag_s;
    uint32_t irqCounter;
    bool IrqEnable;
    bool irqCountDownMode;
    bool irqCountUpMode;
    bool irqFunkyMode;
    bool irqPrescalerSize;
    uint32_t irqSource;
    uint32_t irqPrescaler;
    uint32_t irqPrescalerXOR;
    uint8_t irqFunkyModeReg;
    uint8_t Dipswitch;
    uint8_t multiplication_a;
    uint8_t multiplication_b;
    uint16_t multiplication;
    uint8_t RAM5803;
    bool nt_advanced_enable;
    bool nt_rom_only;
    uint32_t nt_ram_select;
};

class Mapper164 : public Board
{
public:
    Mapper164();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper165 : public Board
{
public:
    Mapper165();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    uint8_t latch_a ;
    uint8_t latch_b ;
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper180 : public Board
{
public:
    Mapper180();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper182 : public Board
{
public:
    Mapper182();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);

    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper184 : public Board
{
public:
    Mapper184();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper185 : public Board
{
public:
    Mapper185();
    void HardReset();
    uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool lockchr;
};

class Mapper189 : public Board
{
public:
    Mapper189();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg;
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
};

class Mapper191 : public Board
{
public:
    Mapper191();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper192 : public Board
{
public:
    Mapper192();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool isRam;
    void SetupCHR();
    void SetupPRG();
};

class Mapper193 : public Board
{
public:
    Mapper193();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper194 : public Board
{
public:
    Mapper194();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    void SetupCHR();
    void SetupPRG();
};

class Mapper200 : public Board
{
public:
    Mapper200();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper201 : public Board
{
public:
    Mapper201();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper202 : public Board
{
public:
    Mapper202();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper203 : public Board
{
public:
    Mapper203();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper204 : public Board
{
public:
    Mapper204();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper205 : public Board
{
public:
    Mapper205();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    uint32_t chr_or;
    uint32_t chr_and;
    uint32_t prg_or;
    uint32_t prg_and;
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    void SetupCHR();
    void SetupPRG();
};

class Mapper206 : public Board
{
public:
    Mapper206();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    void SetupCHR();
    void SetupPRG();
};

class Mapper207 : public Board
{
public:
    Mapper207();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadNMT(uint16_t& address);
    void SaveState();
    void LoadState();
private:
    uint32_t mirroring0;
    uint32_t mirroring1;
};

class Mapper209 : public Board
{
public:
    Mapper209();
    void HardReset();
    void SoftReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WriteSRM(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadSRM(uint16_t& address);
    uint8_t ReadEXP(uint16_t& address);
    uint8_t ReadNMT(uint16_t& address);
    void OnPPUAddressUpdate(uint16_t& address);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    void SetupPRG();
    void SetupCHR();
    uint8_t ReverseByte(uint8_t value);
    void ClockIRQCounter();
    bool MAPPER90MODE;// Setting this to true disables the extended nametables control.
    uint32_t prg_reg[4];
    uint32_t chr_reg[8];
    uint32_t nt_reg[4];
    uint32_t prg_mode;
    uint32_t chr_mode;
    bool chr_block_mode;
    uint32_t chr_block;
    bool chr_m;
    bool flag_s;
    uint32_t irqCounter;
    bool IrqEnable;
    bool irqCountDownMode;
    bool irqCountUpMode;
    bool irqFunkyMode;
    bool irqPrescalerSize;
    uint32_t irqSource;
    uint32_t irqPrescaler;
    uint32_t irqPrescalerXOR;
    uint8_t irqFunkyModeReg;
    uint8_t Dipswitch;
    uint8_t multiplication_a;
    uint8_t multiplication_b;
    uint16_t multiplication;
    uint8_t RAM5803;
    bool nt_advanced_enable;
    bool nt_rom_only;
    uint32_t nt_ram_select;
};

class Mapper210 : public Board
{
public:
    Mapper210();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void WriteEXP(uint16_t& address, uint8_t& value);
    uint8_t ReadEXP(uint16_t& address);
    void WriteNMT(uint16_t& address, uint8_t& value);
    uint8_t ReadNMT(uint16_t& address);
    void OnCPUClock();
    void SaveState();
    void LoadState();
private :
    uint32_t irq_counter;
    bool irq_enable;
    bool disables_chr_ram_A;
    bool disables_chr_ram_B;
    bool enable_mirroring_switch;
    uint32_t temp_nmt;
    uint8_t temp_val;
    uint8_t temp_i;
};

class Mapper212 : public Board
{
public:
    Mapper212();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper213 : public Board
{
public:
    Mapper213();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper214 : public Board
{
public:
    Mapper214();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper216 : public Board
{
public:
    Mapper216();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper222 : public Board
{
public:
    Mapper222();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper225 : public Board
{
public:
    Mapper225();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadEXP(uint16_t& address);
    void SaveState();
    void LoadState();
private:
    uint8_t RAM[4];
};

class Mapper226 : public Board
{
public:
    Mapper226();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t prg_reg;
    bool prg_mode;
};

class Mapper227 : public Board
{
public:
    Mapper227();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    bool flag_o;
    bool flag_s;
    bool flag_l;
    uint32_t prg_reg;
};

class Mapper228 : public Board
{
public:
    Mapper228();
    void HardReset();
    uint8_t ReadEXP(uint16_t& address);
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint8_t RAM[4];
    uint32_t bank;
};

class Mapper229 : public Board
{
public:
    Mapper229();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper230 : public Board
{
public:
    Mapper230();
    void HardReset();
    void SoftReset();
    void WritePRG(uint16_t& address, uint8_t& value);
private:
    bool contraMode;
};

class Mapper231 : public Board
{
public:
    Mapper231();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper232 : public Board
{
public:
    Mapper232();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t prg_reg;
};

class Mapper233 : public Board
{
public:
    Mapper233();
    void HardReset();
    void SoftReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t title ;
    uint32_t bank;
};

class Mapper240 : public Board
{
public:
    Mapper240();
    void WriteEXP(uint16_t& address, uint8_t& value);
};

class Mapper242 : public Board
{
public:
    Mapper242();
    void WritePRG(uint16_t& address, uint8_t& value);
};

class Mapper243 : public Board
{
public:
    Mapper243();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void SaveState();
    void LoadState();
private:
    uint32_t addr;
    uint32_t chr_reg;
};

class Mapper245 : public Board
{
public:
    Mapper245();
    void HardReset();
    void WritePRG(uint16_t& address, uint8_t& value);
    void OnPPUA12RaisingEdge();
    void SaveState();
    void LoadState();
private:
    bool flag_c;
    bool flag_p;
    uint32_t address_8001;
    uint32_t chr_reg[6];
    uint32_t prg_reg[4];
    uint32_t prg_or;
    // IRQ
    bool irq_enabled;
    uint8_t irq_counter;
    uint32_t old_irq_counter;
    uint8_t irq_reload;
    bool irq_clear;
    bool mmc3_alt_behavior;
    void SetupCHR();
    void SetupPRG();
};

class Mapper246 : public Board
{
public:
    Mapper246();
    void HardReset();
    void WriteSRM(uint16_t& address, uint8_t& value);
};

class Mapper255 : public Board
{
public:
    Mapper255();
    void HardReset();
    void WriteEXP(uint16_t& address, uint8_t& value);
    void WritePRG(uint16_t& address, uint8_t& value);
    uint8_t ReadEXP(uint16_t& address);
    void SaveState();
    void LoadState();
private:
    uint8_t RAM[4];
};

}
