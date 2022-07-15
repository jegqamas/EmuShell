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
using namespace std;
/*These make things easier to read*/
typedef uint8_t byte;
typedef uint16_t ushort;

namespace NES
{
void Board::Initialize(char* &prg_dump, int prg_dump_size, char* &chr_dump, int chr_dump_dize,
                       char* &trainer_dump, int trainer_dump_size, int defaultMirroring,
                       bool battery_backed)
{
    cout << "NES: Initializing mapper ..." << "\n";
    // Reset settings to defaults
    prg_ram_default_08KB_count = 1;
    chr_ram_1KB_default_banks_count = 8;
    // Nametables, Mirroring
    default_mirroring = defaultMirroring;
    nmt_banks = new uint8_t*[4];
    nmt_banks[0] = new uint8_t[0x400];
    nmt_banks[1] = new uint8_t[0x400];
    nmt_banks[2] = new uint8_t[0x400];
    nmt_banks[3] = new uint8_t[0x400];
    SwitchNMT(default_mirroring);
    // PRG
    MemoryBank* prg_ram_banks = GetPRGRamBanks(battery_backed);
    prg_rom_size_KB = prg_dump_size / 1024;
    prg_08K_rom_count = prg_rom_size_KB / 8;
    prg_16K_rom_count = prg_rom_size_KB / 16;
    prg_32K_rom_count = prg_rom_size_KB / 32;
    prg_08K_rom_mask = prg_08K_rom_count - 1;
    prg_16K_rom_mask = prg_16K_rom_count - 1;
    prg_32K_rom_mask = prg_32K_rom_count - 1;
    cout << "NES: PRG ROM SIZE = " << prg_rom_size_KB << " KB\n";

    prg_rom_bank_offset = prg_08K_ram_count;
    prg_banks = new uint8_t*[prg_08K_ram_count + prg_08K_rom_count];
    prg_enabled = new bool[prg_08K_ram_count + prg_08K_rom_count];
    prg_isBattery = new bool[prg_08K_ram_count + prg_08K_rom_count];
    prg_isRam = new bool[prg_08K_ram_count + prg_08K_rom_count];
    prg_isWritable = new bool[prg_08K_ram_count + prg_08K_rom_count];
    // Add the ram banks !
    for (uint32_t i = 0; i < prg_rom_bank_offset; i++)
    {
        prg_banks[i] = new uint8_t[0x2000];
        prg_enabled[i] = prg_ram_banks[i].Enabled;
        prg_isBattery[i] = prg_ram_banks[i].IsBattery;
        prg_isRam[i] = prg_ram_banks[i].IsRam;
        prg_isWritable[i] = prg_ram_banks[i].IsWritable;
    }
    // Add the rom banks !
    prg_08K_total_count = prg_08K_ram_count + prg_08K_rom_count;
    prg_08K_total_mask = prg_08K_total_count-1;
    for (uint32_t i = prg_08K_ram_count; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        prg_banks[i] = new uint8_t[0x2000];
        // Copy in old-fashion way
        for (int j=0; j<0x2000; j++)
            prg_banks[i][j] = prg_dump[((i-prg_08K_ram_count)*0x2000) + j];

        prg_enabled[i] = true;
        prg_isBattery[i] = false;
        prg_isRam[i] = false;
        prg_isWritable[i] = false;
    }
    cout << "NES: TOTAL PRG BANKS COUNT (x 8KB) = " << (prg_08K_ram_count + prg_08K_rom_count) << "\n";
    // CHR
    MemoryBank* chr_ram_banks = GetCHRRamBanks();
    //MemoryBank* chr_rom_banks = GetCHRRomBanks(chr_dump, chr_dump_dize);
    chr_01K_rom_count = chr_dump_dize / 1024;
    chr_02K_rom_count = chr_dump_dize / (1024 * 2);
    chr_04K_rom_count = chr_dump_dize / (1024 * 4);
    chr_08K_rom_count = chr_dump_dize / (1024 * 8);
    chr_01K_rom_mask = chr_01K_rom_count - 1;
    chr_02K_rom_mask = chr_02K_rom_count - 1;
    chr_04K_rom_mask = chr_04K_rom_count - 1;
    chr_08K_rom_mask = chr_08K_rom_count - 1;
    cout << "NES: CHR ROM SIZE = " << chr_01K_rom_count << " KB\n";

    chr_rom_bank_offset = chr_01K_ram_count;
    chr_banks = new uint8_t*[chr_rom_bank_offset + chr_01K_rom_count];
    chr_enabled = new bool[chr_rom_bank_offset + chr_01K_rom_count];
    chr_isBattery = new bool[chr_rom_bank_offset + chr_01K_rom_count];
    chr_isRam = new bool[chr_rom_bank_offset + chr_01K_rom_count];
    chr_isWritable = new bool[chr_rom_bank_offset + chr_01K_rom_count];
    // Add the ram banks !
    for (uint32_t i = 0; i < chr_rom_bank_offset; i++)
    {
        chr_banks[i] = new uint8_t[0x400];
        chr_enabled[i] = chr_ram_banks[i].Enabled;
        chr_isBattery[i] = chr_ram_banks[i].IsBattery;
        chr_isRam[i] = chr_ram_banks[i].IsRam;
        chr_isWritable[i] = chr_ram_banks[i].IsWritable;
    }
    // Add the rom banks !
    for (uint32_t i = chr_rom_bank_offset; i < chr_rom_bank_offset + chr_01K_rom_count; i++)
    {
        chr_banks[i] = new uint8_t[0x400];
        for (int j=0; j<0x400; j++)
            chr_banks[i][j] = chr_dump[((i-chr_rom_bank_offset)*0x400) + j];

        chr_enabled[i] = true;
        chr_isBattery[i] = false;
        chr_isRam[i] = false;
        chr_isWritable[i] = false;
    }
    cout << "NES: TOTAL CHR BANKS COUNT (x 1KB) = " << (chr_01K_ram_count + chr_01K_rom_count) << "\n";
    if (trainer_dump_size > 0)
    {
        for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
        {
            if (prg_isRam[i])
            {
                // This is the first RAM, copy the trainer into it.
                for (int j=0 ; j< 512; i++)
                {
                    prg_banks[i][j] = trainer_dump[j];
                }
                break;
            }
        }
    }

    // DELETE unneeded stuff ..
    delete []prg_ram_banks;
    delete []chr_ram_banks;
}
// Hard reset this board/mapper
void Board::HardReset()
{
    // PRG switch
    std::cout << "NES: ->Switching 8K PRG; 0 at 0x6000\n";
    Switch08KPRG(0, 0x6000, false);
    std::cout << "NES: ->Switching 32K PRG; 0 at 0x8000\n";
    Switch32KPRG(0, true);
    std::cout << "NES: ->Toggling PRG RAM ON\n";
    TogglePRGRAMEnable(true);
    // CHR switch
    std::cout << "NES: ->Switching 8K CHR; 0 at 0x0000\n";
    Switch08KCHR(0, chr_01K_rom_count > 0);
    // Mirroring
    std::cout << "NES: ->Switching NMT\n";
    for (int i=0; i<4; i++)
        for (int j=0; j<0x400; j++)
            nmt_banks[i][j]=0;

    SwitchNMT(default_mirroring);
    std::cout << "NES: ->BOARD READY !\n";
}
// Soft reset this board/mapper
void Board::SoftReset()
{
    // Board/Mapper should handle this.
}
Board::~Board()
{
    // Free up memory
    delete [] nmt_banks[0];
    delete [] nmt_banks[1];
    delete [] nmt_banks[2];
    delete [] nmt_banks[3];
    delete [] nmt_banks;

    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        delete [] prg_banks[i];
    }
    delete [] prg_banks;
    delete [] prg_enabled;
    delete [] prg_isRam;
    delete [] prg_isWritable;
    delete [] prg_isBattery;

    for (uint32_t i = 0; i < chr_rom_bank_offset + chr_01K_rom_count; i++)
    {
        delete [] chr_banks[i];
    }
    delete [] chr_banks;
    delete [] chr_enabled;
    delete [] chr_isRam;
    delete [] chr_isWritable;
    delete [] chr_isBattery;

}
void Board::ShutDown()
{

}
/*CPU accesses*/
void Board::WriteEXP(uint16_t& address, uint8_t& value)
{
    // Mapper should hanlde this write.
}
void Board::WriteSRM(uint16_t& address, uint8_t& value)
{
    int index(prg_indexes[0]);
    if (prg_enabled[index] && prg_isRam[index] && prg_isWritable[index])
        prg_banks[index][address & 0x1FFF] = value;
}
void Board::WritePRG(uint16_t& address, uint8_t& value)
{
    int index(prg_indexes[((address >> 13) & 3) + 1]);
    if (prg_enabled[index] && prg_isRam[index] && prg_isWritable[index])
        prg_banks[index][address & 0x1FFF] = value;
}
uint8_t Board::ReadEXP(uint16_t& address)
{
    return (0);// Mapper should hanlde this read.
}
uint8_t Board::ReadSRM(uint16_t& address)
{
    int index(prg_indexes[0]);

    if (prg_enabled[index])
        return prg_banks[index][address & 0x1FFF];

    return (0);
}
uint8_t Board::ReadPRG(uint16_t& address)
{
    return prg_banks[prg_indexes[((address >> 13) & 3) + 1]][address & 0x1FFF];
}
/*PPU accesses*/
void Board::WriteCHR(uint16_t& address, uint8_t& value)
{
    int index(chr_indexes[(address >> 10) & 7]);
    if (chr_enabled[index] && chr_isRam[index] && chr_isWritable[index])
        chr_banks[index][address & 0x3FF] = value;
}
void Board::WriteNMT(uint16_t& address, uint8_t& value)
{
    nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF] = value;
}
uint8_t Board::ReadCHR(uint16_t& address, bool spriteFetch)
{
    return chr_banks[chr_indexes[(address >> 10) & 7]][address & 0x3FF];
}
uint8_t Board::ReadNMT(uint16_t& address)
{
    return nmt_banks[nmt_indexes[(address >> 10) & 3]][address & 0x3FF];
}
/*Clocks*/
void Board::OnCPUClock()
{
}
void Board::OnPPUClock()
{
    if (enabled_ppuA12ToggleTimer)
        ppu_cycles_timer++;
}
void Board::OnPPUScanlineTick()
{
}
void Board::OnPPUAddressUpdate(uint16_t& address)
{
    if (enabled_ppuA12ToggleTimer)
    {
        old_vram_address = new_vram_address;
        new_vram_address = address & 0x1000;
        if (ppuA12TogglesOnRaisingEdge)
        {
            if (old_vram_address < new_vram_address)
            {
                if (ppu_cycles_timer > 8)
                {
                    OnPPUA12RaisingEdge();
                }
                ppu_cycles_timer = 0;
            }
        }
        else
        {
            if (old_vram_address > new_vram_address)
            {
                if (ppu_cycles_timer > 8)
                {
                    OnPPUA12RaisingEdge();
                }
                ppu_cycles_timer = 0;
            }
        }
    }
}
void Board::OnPPUA12RaisingEdge()
{
    // Board should handle this.
}

/*Memory bank initializing*/
MemoryBank* Board::GetPRGRamBanks(bool battery_backed)
{
    if (GameDBIndex>=0)
    {
        // Take the banks from the database
        if (NesDB::GAMES[GameDBIndex].WRAM_sizes.size()>0)
        {
            int items_count =0;
            string * items;
            NesDB::Split(NesDB::GAMES[GameDBIndex].WRAM_sizes,items,items_count);

            if (items_count>0)
            {
                // Determine the sizes
                int total_count=0;
                for (int i = 0; i < items_count; i++)
                {
                    std::size_t found = items[i].find_first_of("k");
                    string bank_size_string = items[i].substr(0,found);
                    int bank_size = Convert::string_to_T<int>(bank_size_string);

                    total_count+=bank_size/8;
                }
                if (total_count>0)
                {
                    prg_ram_default_08KB_count= total_count;// Set the final count, *k each
                    MemoryBank* banks = new MemoryBank[total_count];
                    int bnk_index=0;
                    for (int i = 0; i < items_count; i++)
                    {
                        // See the size
                        std::size_t found = items[i].find_first_of("k");
                        string bank_size_string = items[i].substr(0,found);
                        int bank_size = Convert::string_to_T<int>(bank_size_string);
                        // See if battery
                        found = items[i].find_last_of(" ");

                        if (found < items[i].size())
                            bank_size_string = items[i].substr(found);

                        bool battery = bank_size_string==(std::string)"1";
                        // If something wrong happened, assume 8K
                        if (bank_size==0)
                            bank_size=8;

                        // We can add 8K banks only, so do this ...
                        for (int j=0; j<bank_size/8; j++)
                        {
                            banks[bnk_index].Initialize(0x2000);
                            banks[bnk_index].Enabled = true;
                            banks[bnk_index].IsBattery = battery;
                            banks[bnk_index].IsRam = true;
                            banks[bnk_index].IsWritable = true;
                            bnk_index++;
                        }
                    }
                    // Calculate values
                    prg_ram_size_KB = prg_ram_default_08KB_count * 8;
                    prg_08K_ram_count = prg_ram_default_08KB_count;
                    prg_16K_ram_count = prg_ram_default_08KB_count / 2;
                    prg_32K_ram_count = prg_ram_default_08KB_count / 4;
                    prg_08K_ram_mask = prg_08K_ram_count - 1;
                    prg_16K_ram_mask = prg_16K_ram_count - 1;
                    prg_32K_ram_mask = prg_32K_ram_count - 1;
                    cout << "NES: PRG RAM SIZE = " << prg_ram_size_KB << " KB\n";
                    return banks;
                }
            }
        }

    }
    // Reached here means DB didn't provide us with information. Use default settings.
    MemoryBank* banks = new MemoryBank[prg_ram_default_08KB_count];
    for (uint32_t i = 0; i < prg_ram_default_08KB_count; i++)
    {
        banks[i].Initialize(0x2000);
        banks[i].Enabled = true;
        banks[i].IsBattery = battery_backed;
        banks[i].IsRam = true;
        banks[i].IsWritable = true;
    }
    // Calculate values
    prg_ram_size_KB = prg_ram_default_08KB_count * 8;
    prg_08K_ram_count = prg_ram_default_08KB_count;
    prg_16K_ram_count = prg_ram_default_08KB_count / 2;
    prg_32K_ram_count = prg_ram_default_08KB_count / 4;
    prg_08K_ram_mask = prg_08K_ram_count - 1;
    prg_16K_ram_mask = prg_16K_ram_count - 1;
    prg_32K_ram_mask = prg_32K_ram_count - 1;
    cout << "NES: PRG RAM SIZE = " << prg_ram_size_KB << " KB\n";
    return banks;
}
MemoryBank* Board::GetCHRRamBanks()
{
    if (GameDBIndex>=0)
    {
        // Take the banks from the database
        if (NesDB::GAMES[GameDBIndex].VRAM_sizes.size()>0)
        {
            int items_count =0;
            string * items;
            NesDB::Split(NesDB::GAMES[GameDBIndex].VRAM_sizes,items,items_count);

            if (items_count>0)
            {
                // Determine the sizes
                int total_count=0;
                for (int i = 0; i < items_count; i++)
                {
                    std::cout <<"CHR RAM: "<< items[i]<<"\n";
                    std::size_t found = items[i].find_first_of("k");

                    string bank_size_string = items[i].substr(0,found);
                    int bank_size = Convert::string_to_T<int>(bank_size_string);

                    total_count+=bank_size;
                }
                if (total_count>0)
                {
                    chr_ram_1KB_default_banks_count= total_count;// Set the final count, *k each
                    MemoryBank* banks = new MemoryBank[total_count];
                    for (int i = 0; i < items_count; i++)
                    {
                        // See the size
                        std::size_t found = items[i].find_first_of("k");
                        string bank_size_string = items[i].substr(0,found);
                        int bank_size = Convert::string_to_T<int>(bank_size_string);

                        int bnk_index=0;
                        // We can add 8K banks only, so do this ...
                        for (int j=0; j<bank_size; j++)
                        {
                            banks[bnk_index].Initialize(0x0400);
                            banks[bnk_index].Enabled = true;
                            banks[bnk_index].IsBattery = true;
                            banks[bnk_index].IsRam = true;
                            banks[bnk_index].IsWritable = true;
                            bnk_index++;
                        }
                    }
                    // Calculate values
                    chr_01K_ram_count = chr_ram_1KB_default_banks_count;
                    chr_02K_ram_count = chr_ram_1KB_default_banks_count / 2;
                    chr_04K_ram_count = chr_ram_1KB_default_banks_count / 4;
                    chr_08K_ram_count = chr_ram_1KB_default_banks_count / 8;
                    chr_01K_ram_mask = chr_01K_ram_count - 1;
                    chr_02K_ram_mask = chr_02K_ram_count - 1;
                    chr_04K_ram_mask = chr_04K_ram_count - 1;
                    chr_08K_ram_mask = chr_08K_ram_count - 1;

                    cout << "NES: CHR RAM SIZE = " << chr_01K_ram_count << " KB\n";
                    return banks;
                }
            }
        }
    }
    // Set 8 for default if something wrong happened.
    if (chr_ram_1KB_default_banks_count==0)
        chr_ram_1KB_default_banks_count=8;
    // Reached here means DB didn't provide us with information. Use default settings.
    MemoryBank* banks = new MemoryBank[chr_ram_1KB_default_banks_count];
    for (uint32_t i = 0; i < chr_ram_1KB_default_banks_count; i++)
    {
        banks[i].Initialize(0x400);
        banks[i].Enabled = true;
        banks[i].IsBattery = true;
        banks[i].IsRam = true;
        banks[i].IsWritable = true;
    }
    // Calculate values
    chr_01K_ram_count = chr_ram_1KB_default_banks_count;
    chr_02K_ram_count = chr_ram_1KB_default_banks_count / 2;
    chr_04K_ram_count = chr_ram_1KB_default_banks_count / 4;
    chr_08K_ram_count = chr_ram_1KB_default_banks_count / 8;
    chr_01K_ram_mask = chr_01K_ram_count - 1;
    chr_02K_ram_mask = chr_02K_ram_count - 1;
    chr_04K_ram_mask = chr_04K_ram_count - 1;
    chr_08K_ram_mask = chr_08K_ram_count - 1;

    cout << "NES: CHR RAM SIZE = " << chr_01K_ram_count << " KB\n";
    return banks;
}

/*Helper methods*/
void Board::TogglePRGRAMEnable(bool enable)
{
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i])
            prg_enabled[i] = enable;
    }
}
void Board::TogglePRGRAMWritable(bool enable)
{
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i])
            prg_isWritable[i] = enable;
    }
}
void Board::ToggleCHRRAMWritable(bool enable)
{
    for (uint32_t i = 0; i < chr_01K_ram_count + chr_01K_rom_count; i++)
    {
        if (chr_isRam[i])
            chr_isWritable[i] = enable;
    }
}
/*Switches*/
void Board::Switch08KPRG(int index, int area, bool isRom)
{
    if (!isRom && prg_08K_ram_count == 0)
        return;
    if (area == 0x6000)
    {
        if (isRom)
            prg_indexes[0] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        else
        {
            prg_indexes[0] = index & prg_08K_ram_mask;
        }
    }
    else
    {
        if (isRom)
            prg_indexes[((area >> 13) & 0x3) + 1] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        else
            prg_indexes[((area >> 13) & 0x3) + 1] = index & prg_08K_ram_mask;
    }
}
void Board::Switch16KPRG(int index, int area, bool isRom)
{
    if (!isRom && prg_08K_ram_count == 0) return;// Nothing to do if there's no ram
    area >>= 13;
    area &= 3;
    area++;

    index <<= 1;
    if (isRom)
    {
        prg_indexes[area] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        index++;
        area++;
        prg_indexes[area] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
    }
    else
    {

        prg_indexes[area] = index & prg_08K_ram_mask;
        index++;
        area++;
        prg_indexes[area] = index & prg_08K_ram_mask;
    }
}
void Board::Switch32KPRG(int index, bool isRom)
{
    if (!isRom && prg_08K_ram_count == 0)
        return;
    index <<= 2;
    if (isRom)
    {
        prg_indexes[1] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        index++;
        prg_indexes[2] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        index++;
        prg_indexes[3] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
        index++;
        prg_indexes[4] = (index & prg_08K_rom_mask) + prg_rom_bank_offset;
    }
    else
    {
        prg_indexes[1] = index & prg_08K_ram_mask;
        index++;
        prg_indexes[2] = index & prg_08K_ram_mask;
        index++;
        prg_indexes[3] = index & prg_08K_ram_mask;
        index++;
        prg_indexes[4] = index & prg_08K_ram_mask;
    }
}
void Board::Switch01KCHR(int index, int area, bool isRom)
{
    if (!isRom && chr_01K_ram_count == 0)
        return;
    if (isRom)
        chr_indexes[(area >> 10) & 0x07] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    else
        chr_indexes[(area >> 10) & 0x07] = index & chr_01K_ram_mask;
}
void Board::Switch02KCHR(int index, int area, bool isRom)
{
    if (!isRom && chr_01K_ram_count == 0)
        return;
    area >>= 10;
    area &= 0x7;
    index <<= 1;
    if (isRom)
    {
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        area++;
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    }
    else
    {
        chr_indexes[area] = index & chr_01K_ram_mask;
        index++;
        area++;
        chr_indexes[area] = index & chr_01K_ram_mask;
    }
}
void Board::Switch04KCHR(int index, int area, bool isRom)
{
    if (!isRom && chr_01K_ram_count == 0)
        return;
    area >>= 10;
    area &= 0x7;
    index <<= 2;
    if (isRom)
    {
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        area++;
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        area++;
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        area++;
        chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    }
    else
    {
        chr_indexes[area] = index & chr_01K_ram_mask;
        index++;
        area++;
        chr_indexes[area] = index & chr_01K_ram_mask;
        index++;
        area++;
        chr_indexes[area] = index & chr_01K_ram_mask;
        index++;
        area++;
        chr_indexes[area] = index & chr_01K_ram_mask;
    }
}
void Board::Switch08KCHR(int index, bool isRom)
{
    if (!isRom && chr_01K_ram_count == 0)
        return;
    index <<= 3;

    if (isRom)
    {
        chr_indexes[0] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[1] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[2] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[3] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[4] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[5] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[6] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
        index++;
        chr_indexes[7] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    }
    else
    {
        chr_indexes[0] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[1] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[2] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[3] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[4] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[5] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[6] = index & chr_01K_ram_mask;
        index++;
        chr_indexes[7] = index & chr_01K_ram_mask;
    }
}
void Board::SwitchNMT(int value)
{
    nmt_indexes[0] = (value >> 0 & 0x03);
    nmt_indexes[1] = (value >> 2 & 0x03);
    nmt_indexes[2] = (value >> 4 & 0x03);
    nmt_indexes[3] = (value >> 6 & 0x03);
}

/*SRAM*/
bool Board::SaveSRAMRequired()
{
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i]&& prg_isBattery[i])
            return true;
    }
    return false;
}
uint32_t Board::GetSRAMBufferSize()
{
    uint32_t buf_size = 0;
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i]&& prg_isBattery[i])
            buf_size += 0x2000;
    }
    return buf_size;
}
void Board::SaveSRAMToBuffer(char* &buffer)
{
    uint32_t _index = 0;
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i]&& prg_isBattery[i])
        {
            for (int j=0; j< 0x2000; j++)
                buffer[j +_index] = prg_banks[i][j];
            _index += 0x2000;
        }
    }
}
void Board::GetSRAMFromBuffer(char* &buffer)
{
    uint32_t _index = 0;
    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i]&& prg_isBattery[i])
        {
            for (int j=0; j< 0x2000; j++)
                prg_banks[i][j]= buffer[j +_index];
            _index += 0x2000;
        }
    }
}
/*External sound channels if any*/
void Board::ClockExternalSoundChannelsSingle(bool& is_clocking_duration)
{
}
double Board::GetExternalSoundChannelsOutput()
{
    return 0;
}
void Board::VSUnisystem4016RW(uint8_t& value)
{
    if ((value & 0x4) == 0x4)
        Switch08KCHR(1, chr_01K_rom_count > 0);
    else
        Switch08KCHR(0, chr_01K_rom_count > 0);
}
void Board::SaveState()
{
    state_str->Write(prg_enabled,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Write(prg_isRam,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Write(prg_isWritable,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Write(prg_isBattery,prg_08K_ram_count + prg_08K_rom_count);

    state_str->Write(chr_enabled,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Write(chr_isRam,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Write(chr_isWritable,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Write(chr_isBattery,chr_rom_bank_offset + chr_01K_rom_count);
    for (int i=0; i<5; i++)
        state_str->Write(prg_indexes[i]);
    for (int i=0; i<8; i++)
        state_str->Write(chr_indexes[i]);
    for (int i=0; i<4; i++)
        state_str->Write(nmt_indexes[i]);

    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i])
            state_str->Write(prg_banks[i],0x2000);
    }

    for (uint32_t i = 0; i < chr_rom_bank_offset + chr_01K_rom_count; i++)
    {
        if (chr_isRam[i])
            state_str->Write(chr_banks[i],0x400);
    }
    for (int i=0; i<4; i++)
        state_str->Write(nmt_banks[i],0x400);

    state_str->Write(old_vram_address);
    state_str->Write(new_vram_address);
    state_str->Write(ppu_cycles_timer);
}
void Board::LoadState()
{
    state_str->Read(prg_enabled,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Read(prg_isRam,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Read(prg_isWritable,prg_08K_ram_count + prg_08K_rom_count);
    state_str->Read(prg_isBattery,prg_08K_ram_count + prg_08K_rom_count);

    state_str->Read(chr_enabled,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Read(chr_isRam,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Read(chr_isWritable,chr_rom_bank_offset + chr_01K_rom_count);
    state_str->Read(chr_isBattery,chr_rom_bank_offset + chr_01K_rom_count);
    for (int i=0; i<5; i++)
        state_str->Read(prg_indexes[i]);
    for (int i=0; i<8; i++)
        state_str->Read(chr_indexes[i]);
    for (int i=0; i<4; i++)
        state_str->Read(nmt_indexes[i]);

    for (uint32_t i = 0; i < prg_08K_ram_count + prg_08K_rom_count; i++)
    {
        if (prg_isRam[i])
            state_str->Read(prg_banks[i],0x2000);
    }

    for (uint32_t i = 0; i < chr_rom_bank_offset + chr_01K_rom_count; i++)
    {
        if (chr_isRam[i])
            state_str->Read(chr_banks[i],0x400);
    }
    for (int i=0; i<4; i++)
        state_str->Read(nmt_banks[i],0x400);

    state_str->Read(old_vram_address);
    state_str->Read(new_vram_address);
    state_str->Read(ppu_cycles_timer);
}
}
