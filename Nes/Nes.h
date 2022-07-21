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

#include <iostream>
#include <string>
#include "Board.h"
#include "PPU.h"
#include "APU.h"
#include "DMA.h"
#include "../Common/Cpu6502.h"
#include "../Common/StateStream.h"

using namespace EmuShell;
using namespace Common;

#define NES_CPU_SPEED_NTSC 1789773
#define NES_CPU_SPEED_PALB 1662607
#define NES_CPU_SPEED_DENDY 1773448

namespace NES
{
// Execute related command-lines for nes system
// argc: the arguments count
// args: the arguments as string arrays.
void ExecuteCommands(int argc,char* args[]);
void Initialize(std::string fileName);// Initialize the nes core
void HardReset();// Hard reset the nes core
void RUN();// Run the emulation while ON is set.
void RUNFRAME();// Run the emulation until it completes a frame.
void OnFinishFrame();// Called by PPU when a frame finished.
void ClockComponents();
void SetupJoypads();

// Current loaded game title.
extern std::string GameTitle;
extern std::string STATE_FOLDER;
extern std::string SRAM_FOLDER;
extern int STATE_SLOT;// The state slot, 0 to 9
extern int TV_FORMAT;// The tv format (NTSC, PAL or Dendy)
extern int GameDBIndex;// The game index within database games collection
extern bool READY;// Indicates if a file loaded and the system is ready to go.
extern bool BoardFound;// Indicates if the board/mapper is supported, located and ready.
extern bool ON;// Indicates if the emulation is on.
extern bool PAUSED;// Indicates if the emulation is paused.
extern bool IsFourPlayers;// Indicates if 4 players is connected
extern bool ConnectZapper;// Indicates if zapper (light gun) is connected
extern bool ConnectVSUniSystem; // Indicates if VS UniSystem is connected. This switch set automatically on initialize.
extern bool BUS_RW;// R/W pin
extern uint16_t BUS_ADDRESS;// Current bus address (cpu)
extern Board* dev_board;// Nes board (i.e. mapper)
extern CPU6502* dev_cpu;// CPU 6502
extern PPU2C02* dev_ppu;// PPU 2C02
extern APU2A03* dev_apu;// APU 2A03
extern DMAUnit* dev_dma;// The DMA unit
extern StateStream* state_str;// The state stream, we gonna use it to save/load state.
extern void MemWrite(uint16_t address, uint8_t value);// Write on memory (cpu space)
extern uint8_t MemRead(uint16_t address);// Read from memory (cpu space)
extern void SetupJoypads();
extern void SetupSoundPlayback(const bool& soundEnabled, const uint32_t& frequency);
extern void ShowStateSlotDetails();
extern void ShutDown();
// Requests ...
extern void RequestHardReset();
extern void RequestSoftReset();
extern void RequestSaveState();
extern void RequestLoadState();
// Events
extern void OnMouseClick(const int &button);
extern void OnMouseUp(const int &button);
namespace NesDB
{
class NesDBGameCartInfo
{
public:
    // Game inf
    std::string Game_Name;
    std::string Game_AltName;
    std::string Game_Class;
    std::string Game_Catalog;
    std::string Game_Publisher;
    std::string Game_Developer;
    std::string Game_Region;
    std::string Game_Players;
    std::string Game_ReleaseDate;
    //cartridge info
    std::string System;
    std::string CRC;
    std::string SHA1;
    std::string Dump;
    std::string Dumper;
    std::string DateDumped;
    //board info
    std::string Board_Type;
    std::string Board_Pcb;
    std::string Board_Mapper;
    //vram
    std::string VRAM_sizes;

    //wram (s-ram)
    std::string WRAM_sizes;
    //prg
    std::string PRG_name;
    std::string PRG_size;
    std::string PRG_crc;
    std::string PRG_sha1;

    //chr
    std::string CHR_name;
    std::string CHR_size;
    std::string CHR_crc;
    std::string CHR_sha1;

    //chip, may be more than one chip
    std::string chip_types;
    //cic
    std::string CIC_type;

    //pad
    std::string PAD_h;
    std::string PAD_v;
};

// Initialize the NesDB, read database file if possible.
// dbFilePath: The complete file path
extern void Initialize(std::string dbFilePath);
// Get game index within the games collection if found. Otherwise returns -1
// gameCRC: the game CRC32
// Returns the index of the game within the games collection if found otherwise returns -1.
extern int GetGameIndex(std::string gameCRC);
// Indicates if a game contain a chip type
// gameIndex: the game index within the games collection
// chipName: the chip type name to search for
// Returns true if the chip is found otherwise returns false.
extern bool ContainChip(int gameIndex,std::string chipName);
extern bool ConatinSystem(int gameIndex,std::string systemName);
extern bool ConatinBoardType(int gameIndex,std::string boardType);
extern void Split(std::string item, std::string* &items, int &items_count);
// Array of loaded games !
extern NesDBGameCartInfo* GAMES;
extern int GamesCount;
extern std::string DBVersion;
extern std::string DBConformance;
extern std::string DBAgent ;
extern std::string DBAuthor;
extern std::string DBTimeStamp;
}
}
