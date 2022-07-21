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
#include <string>
#include <stdint.h>

namespace NES
{
// Represents a memory bank.
struct MemoryBank
{
public:
    // Create new empty bank
    MemoryBank()
    {
        Size = 0;
        Enabled = false;
        IsRam = false;
        IsWritable = false;
        IsBattery = false;
    }
    // Create new RAM bank
    MemoryBank(int new_size)
    {
        Initialize(new_size);
    }
    // Create new RAM bank
    void Initialize(int new_size)
    {
        Size = new_size;
        Enabled = true;
        IsRam = true;
        IsWritable = true;
        IsBattery = false;
    }

    int Size;// Bank size
    bool Enabled;// Indicates if the bank is enabled (for write and read access)
    bool IsRam;// Indicates if this bank is RAM (writable)
    bool IsWritable;// If the banks is RAM, this flag indicates if the bank can accept writes
    bool IsBattery;// Indicates that this bank is a battery packed.
};

// Represents a NES board / mapper
class Board
{
public:
    ~Board();
    virtual void Initialize(char* &prg_dump, int prg_dump_size, char* &chr_dump, int chr_dump_dize,
                            char* &trainer_dump, int trainer_dump_size, int defaultMirroring,
                            bool battery_backed);
    // Hard reset this board/mapper
    virtual void HardReset();
    // Soft reset this board/mapper
    virtual void SoftReset();
    // Shutdowns the board
    virtual void ShutDown();
    /*CPU accesses*/
    virtual void WriteEXP(uint16_t& address, uint8_t& value);
    virtual void WriteSRM(uint16_t& address, uint8_t& value);
    virtual void WritePRG(uint16_t& address, uint8_t& value);
    virtual uint8_t ReadEXP(uint16_t& address);
    virtual uint8_t ReadSRM(uint16_t& address);
    virtual uint8_t ReadPRG(uint16_t& address);
    /*PPU accesses*/
    virtual void WriteCHR(uint16_t& address, uint8_t& value);
    virtual void WriteNMT(uint16_t& address, uint8_t& value);
    virtual uint8_t ReadCHR(uint16_t& address, bool spriteFetch);
    virtual uint8_t ReadNMT(uint16_t& address);
    virtual void VSUnisystem4016RW(uint8_t& value);
    /*Clocks*/
    virtual void OnCPUClock();
    virtual void OnPPUClock();
    virtual void OnPPUScanlineTick();
    virtual void OnPPUAddressUpdate(uint16_t& address);
    virtual void OnPPUA12RaisingEdge();
    /*SRAM*/
    virtual void SaveSRAMToBuffer(char* &buffer);
    virtual void GetSRAMFromBuffer(char* &buffer);
    virtual uint32_t GetSRAMBufferSize();
    virtual bool SaveSRAMRequired();
    /*External sound channels if any*/
    virtual void ClockExternalSoundChannelsSingle(bool& is_clocking_duration);
    virtual double GetExternalSoundChannelsOutput();
    /*STATE*/
    virtual	void SaveState();
    virtual	void LoadState();
    /*Board/Mapper information*/
    std::string Name;// Board name
    uint32_t MapperNumber;// The INES mapper number. Here, it will be used as an ID.

protected:
    uint8_t** prg_banks;// PRG banks, 8kb each.
    bool* prg_enabled;// Each item determine a prg bank enable flag (at the same index)
    bool* prg_isRam;// Each item determine if a prg bank is ram or not (at the same index)
    bool* prg_isWritable;// Each item determine if a prg bank is writable or not (at the same index)
    bool* prg_isBattery;// Each item determine if a prg bank is battery-backed or not (at the same index)
    uint8_t** chr_banks;// CHR banks, 1kb each.
    bool* chr_enabled;// Each item determine a chr bank enable flag (at the same index)
    bool* chr_isRam;// Each item determine if a chr bank is ram or not (at the same index)
    bool* chr_isWritable;// Each item determine if a chr bank is writable or not (at the same index)
    bool* chr_isBattery;// Each item determine if a chr bank is battery-backed or not (at the same index)
    uint8_t** nmt_banks;// NMT banks, 1kb each.
    /*Indexes*/
    uint32_t prg_indexes[5];
    uint32_t prg_rom_bank_offset;
    uint32_t prg_rom_size_KB;
    uint32_t prg_08K_rom_count;
    uint32_t prg_16K_rom_count;
    uint32_t prg_32K_rom_count;
    uint32_t prg_08K_rom_mask;
    uint32_t prg_16K_rom_mask;
    uint32_t prg_32K_rom_mask;
    uint32_t prg_ram_size_KB;
    uint32_t prg_ram_default_08KB_count;// Used when the rom info about prg ram is not found.
    uint32_t prg_08K_ram_count;
    uint32_t prg_16K_ram_count;
    uint32_t prg_32K_ram_count;
    uint32_t prg_08K_ram_mask;
    uint32_t prg_16K_ram_mask;
    uint32_t prg_32K_ram_mask;
    uint32_t prg_08K_total_count;
    uint32_t prg_08K_total_mask;
    uint32_t chr_indexes[8];
    uint32_t chr_rom_bank_offset;
    uint32_t chr_01K_rom_count;
    uint32_t chr_02K_rom_count;
    uint32_t chr_04K_rom_count;
    uint32_t chr_08K_rom_count;
    uint32_t chr_01K_rom_mask;
    uint32_t chr_02K_rom_mask;
    uint32_t chr_04K_rom_mask;
    uint32_t chr_08K_rom_mask;
    uint32_t chr_ram_1KB_default_banks_count;// Used when the rom info about chr ram is not found.
    uint32_t chr_01K_ram_count;
    uint32_t chr_02K_ram_count;
    uint32_t chr_04K_ram_count;
    uint32_t chr_08K_ram_count;
    uint32_t chr_01K_ram_mask;
    uint32_t chr_02K_ram_mask;
    uint32_t chr_04K_ram_mask;
    uint32_t chr_08K_ram_mask;
    uint32_t nmt_indexes[4];
    uint32_t default_mirroring;

    /*Memory bank initializing*/
    MemoryBank* GetPRGRamBanks(bool battery_backed);
    MemoryBank* GetCHRRamBanks();
    /*Helper methods*/
    void TogglePRGRAMEnable(bool enable);
    void TogglePRGRAMWritable(bool enable);
    void ToggleCHRRAMWritable(bool enable);
    /*Switches*/
    void Switch08KPRG(int index, int area, bool isRom);
    void Switch16KPRG(int index, int area, bool isRom);
    void Switch32KPRG(int index, bool isRom);
    void Switch01KCHR(int index, int area, bool isRom);
    void Switch02KCHR(int index, int area, bool isRom);
    void Switch04KCHR(int index, int area, bool isRom);
    void Switch08KCHR(int index, bool isRom);
    void SwitchNMT(int value);
    /*Internal clocks*/
    bool enabled_ppuA12ToggleTimer;
    bool ppuA12TogglesOnRaisingEdge;
    uint16_t old_vram_address;
    uint16_t new_vram_address;
    uint32_t ppu_cycles_timer;
};

/// Get/Locate board using mapper number.
extern void SetBoard(int mapperNumber);
}
