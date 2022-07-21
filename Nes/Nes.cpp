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
#include <fstream>
#include <exception>
#include <SDL2/SDL.h>
#include "Nes.h"
#include "IJoypadConnecter.h"
#include "IZapperConnecter.h"
#include "IVSUniSystemDIPConnecter.h"
#include "Rom/INES.h"
#include "../Common/Crc32.h"
#include "../Common/ConfigHandler.h"
#include "../Providers/SDLInput.h"
#include "../Providers/VideoOut.h"
#include "../Definations.h"
#include "../Core.h"

using namespace Common;
using namespace EmuShell;
using namespace std;

#define NES_SYSTEM_MODE_AUTO 0
#define NES_SYSTEM_MODE_NTSC 1
#define NES_SYSTEM_MODE_PALB 2
#define NES_SYSTEM_MODE_DENDY 3

namespace NES
{
Board* dev_board;// Nes board (i.e. mapper)
CPU6502* dev_cpu;// CPU 6502
PPU2C02* dev_ppu;// PPU 2C02
APU2A03* dev_apu;// APU 2A03
DMAUnit* dev_dma;// The DMA unit
StateStream* state_str;// The state stream, we gonna use it to save/load state.

// Current loaded game title.
std::string GameTitle;

/*Emulation controls*/
bool READY;// Indicates if the system is ready to clock.
bool ON;// Indicates if the emulation is on.
bool PAUSED;// Indicates if the emulation is paused.
int TV_FORMAT;// The tv format (NTSC, PAL or Dendy)
/*Some useful things*/
bool BUS_RW;// R/W pin
uint16_t BUS_ADDRESS;// Current bus address (cpu)
uint8_t* WRAM;// Work ram, used only on memory access 0-0x2000.
int GameDBIndex;// The game index within database games collection
bool BoardFound;// Indicates if the board/mapper is supported, located and ready.

/*Input*/
bool IsFourPlayers;
IJoypadConnecter* joy1;
bool pl1_use_joystick;
int pl1_joystick_index;
IJoypadConnecter* joy2;
bool pl2_use_joystick;
int pl2_joystick_index;
IJoypadConnecter* joy3;
bool pl3_use_joystick;
int pl3_joystick_index;
IJoypadConnecter* joy4;
bool pl4_use_joystick;
int pl4_joystick_index;

IZapperConnecter* zapp;
bool ConnectZapper;

IVSUniSystemConnecter * vsuni_dip;
bool ConnectVSUniSystem;
bool vs_use_joystick;
int vs_joystick_index;

uint32_t PORT0;
uint32_t PORT1;
int temp_4016;
int temp_4017;
uint32_t inputStrobe;
// Tv format mode, specify how the system choses tv format at start up.
uint32_t tv_system_mode;
bool DoPalAdditionalClock;
uint8_t palCyc;
int PaletteUsageMode;
std::string PaletteFile;
void CheckRequests();
/*STATE*/
int STATE_SLOT;// The state slot, 0 to 9
std::string STATE_FOLDER;
std::string SRAM_FOLDER;
std::string SRAM_FILE;
bool request_state_save;
bool request_state_load;
bool request_state_load_after_intialize;
void save_state();
void load_state();
void SetupVSUniSystem(bool setByHeader, int mapper);

void LoadSettings()
{
    STATE_FOLDER =CONFIG::GetStringValue("nes.states_folder",
                                         (std::string)("states"+Path::Separator()+"nes"+Path::Separator()));
    SRAM_FOLDER =CONFIG::GetStringValue("nes.sram_folder",
                                        (std::string)("srams"+Path::Separator()+"nes"+Path::Separator()));

    tv_system_mode = CONFIG::GetInt32Value("nes.tv_format_mode",NES_SYSTEM_MODE_AUTO);
    IsFourPlayers=CONFIG::GetBoolValue("nes.use_4_players",false);
    ConnectZapper=CONFIG::GetBoolValue("nes.connect_zapper",false);
    PaletteUsageMode=CONFIG::GetInt32Value("nes.palette_mode",PALETTE_USAGE_AUTO);
    PaletteFile=CONFIG::GetStringValue("nes.palette_file","palettes"+Path::Separator()+"nes"+Path::Separator()+"default.pal");

    pl1_use_joystick=CONFIG::GetBoolValue("nes.pl1.use_joystick",false);
    pl1_joystick_index = CONFIG::GetInt32Value("nes.pl1.joystick_index",0);
    pl2_use_joystick=CONFIG::GetBoolValue("nes.pl2.use_joystick",false);
    pl2_joystick_index = CONFIG::GetInt32Value("nes.pl2.joystick_index",0);
    pl3_use_joystick=CONFIG::GetBoolValue("nes.pl3.use_joystick",false);
    pl3_joystick_index = CONFIG::GetInt32Value("nes.pl3.joystick_index",0);
    pl4_use_joystick=CONFIG::GetBoolValue("nes.pl4.use_joystick",false);
    pl4_joystick_index = CONFIG::GetInt32Value("nes.pl4.joystick_index",0);
    vs_use_joystick=CONFIG::GetBoolValue("nes.vs.use_joystick",false);
    vs_joystick_index= CONFIG::GetInt32Value("nes.vs.joystick_index",0);
}
void SaveSettings()
{
    CONFIG::SetValue("nes.states_folder",STATE_FOLDER);
    CONFIG::SetValue("nes.sram_folder",SRAM_FOLDER);
    CONFIG::SetValue("nes.tv_format_mode",tv_system_mode);
    CONFIG::SetValue("nes.use_4_players",IsFourPlayers);
    CONFIG::SetValue("nes.connect_zapper",ConnectZapper);
    if (READY)
    {
        CONFIG::SetValue("nes.palette_mode",dev_ppu->PaletteUsageMode);
        CONFIG::SetValue("nes.palette_file",dev_ppu->PaletteFile);

        joy1->SaveKeys();
        joy2->SaveKeys();
        joy3->SaveKeys();
        joy4->SaveKeys();

        if (ConnectVSUniSystem)
            vsuni_dip->SaveKeys();

        CONFIG::SetValue("nes.pl1.use_joystick",pl1_use_joystick);
        CONFIG::SetValue("nes.pl1.joystick_index",pl1_joystick_index);
        CONFIG::SetValue("nes.pl2.use_joystick",pl2_use_joystick);
        CONFIG::SetValue("nes.pl2.joystick_index",pl2_joystick_index);
        CONFIG::SetValue("nes.pl3.use_joystick",pl3_use_joystick);
        CONFIG::SetValue("nes.pl3.joystick_index",pl3_joystick_index);
        CONFIG::SetValue("nes.pl4.use_joystick",pl4_use_joystick);
        CONFIG::SetValue("nes.pl4.joystick_index",pl4_joystick_index);
        CONFIG::SetValue("nes.vs.use_joystick",vs_use_joystick);
        CONFIG::SetValue("nes.vs.joystick_index",vs_joystick_index);
    }
}
void Initialize(std::string fileName)
{
    LoadSettings();
    READY = false;
    // TODO: support more nes file types
    // Load the file
    INES::Load(fileName);

    if (INES::IsValid)
    {
        // WARNING: the crc that stored here will not include the header to be compatible with
        // databases.
        EmuShell::GameCRC32=CRC32::CalculateCRC32(fileName,16);

        GameTitle = Path::GetFileNameWithoutExtnsion((string)EmuShell::FileName) + " - EmuShell";

        cout << ("NES: loading NesCart database....\n");

        string db_path = Path::Combine("databases","nes.xml");
        if (Path::IsFileExist(db_path))
        {
            NesDB::Initialize(db_path);
            cout << ("NES: looking the game in the database ...\n");

            // Get the crc as string and make it upper-case
            string crcString =Convert::to_string_hexa(EmuShell::GameCRC32);
            int crcStringSize = (int)crcString.size();
            for (int i=0; i < crcStringSize; i++)
                crcString[i] = std::toupper(crcString[i]);

            // Look up the game in the database
            GameDBIndex = NesDB::GetGameIndex(crcString.c_str());
            // Print the game name to see the result
            if (GameDBIndex >=0)
            {
                std::cout << "NES: Game name = "<<NesDB::GAMES[GameDBIndex].Game_Name <<"\n";
                std::cout << "NES: Game alt name = "<<NesDB::GAMES[GameDBIndex].Game_AltName <<"\n";
                std::cout << "NES: Game mapper = "<<NesDB::GAMES[GameDBIndex].Board_Mapper <<"\n";
                // Override the rom settings
                INES::MapperNumber = Convert::string_to_T<int>(NesDB::GAMES[GameDBIndex].Board_Mapper);

                // Set game title
                if (NesDB::GAMES[GameDBIndex].Game_AltName.size()>0)
                {
                    GameTitle = NesDB::GAMES[GameDBIndex].Game_Name +" ("+ NesDB::GAMES[GameDBIndex].Game_AltName+ ")";
                }
                else
                {
                    GameTitle = NesDB::GAMES[GameDBIndex].Game_Name;
                }
            }
            else
            {
                GameTitle = Path::GetFileNameWithoutExtnsion((string)EmuShell::FileName);
                std::cout << "NES: ->Game cannot be found in the database.\n";
            }
        }
        else
        {
            GameDBIndex=-1;
            cout << ("NES: NesCart database file is not found.\n");
        }
        VideoOut::SetWindowTitle((GameTitle+" - EmuShell").c_str());
        // Make sram file
        string game_name = Path::GetFileNameWithoutExtnsion((string)EmuShell::FileName) +".sr";

        SRAM_FILE =Path::Combine(SRAM_FOLDER,game_name);


        cout << ("NES: Initializing components ....\n");

        cout << ("NES: Initializing cpu ....\n");
        dev_cpu = new CPU6502();
        dev_cpu->Initialize(&MemWrite, &MemRead);
        cout << ("NES: CPU initialized success !!\n");

        cout << ("NES: Initializing ppu ....\n");
        dev_ppu = new PPU2C02();
        dev_ppu->PaletteFile = PaletteFile;
        dev_ppu->PaletteUsageMode = PaletteUsageMode;
        // Detect tv format
        switch(tv_system_mode)
        {
        case NES_SYSTEM_MODE_AUTO:
        {
            // TV format will be specified using DB or header
            if (GameDBIndex >=0)
            {
                if (NesDB::ConatinSystem (GameDBIndex,(std::string)"NTSC")||NesDB::GAMES[GameDBIndex].System== ((std::string)"Famicom"))
                {
                    TV_FORMAT  = TVSYSTEM_NTSC;
                    cout << ("NES: TV format set to NTSC; auto detected from DB.\n");
                }
                else if (NesDB::ConatinSystem (GameDBIndex,(std::string)"PAL"))
                {
                    TV_FORMAT  = TVSYSTEM_PALB;
                    cout << ("NES: TV format set to PALB; auto detected from DB.\n");
                }
            }
            // Using file name ...
            else if (((std::string)fileName).find("(U)") != std::string::npos)
            {
                TV_FORMAT  = TVSYSTEM_NTSC;
                cout << ("NES: TV format set to NTSC; auto detected from file name.\n");
            }
            else if (((std::string)fileName).find("(E)") != std::string::npos)
            {
                TV_FORMAT  = TVSYSTEM_PALB;
                cout << ("NES: TV format set to PALB; auto detected from file name.\n");
            }
            else
            {
                // Just set NTSC
                TV_FORMAT  = TVSYSTEM_NTSC;
                cout << ("NES: TV format set to NTSC; auto detected failed.\n");
            }
            break;
        }
        case NES_SYSTEM_MODE_NTSC:// Force NTSC
        {
            TV_FORMAT  = TVSYSTEM_NTSC;
            cout << ("NES: TV format set to NTSC; forced.\n");
            break;
        }
        case NES_SYSTEM_MODE_PALB:// Force PALB
        {
            TV_FORMAT  = TVSYSTEM_PALB;
            cout << ("NES: TV format set to PALB; forced.\n");
            break;
        }
        case NES_SYSTEM_MODE_DENDY:// Force DENDY
        {
            TV_FORMAT  = TVSYSTEM_DENDY;
            cout << ("NES: TV format set to DENDY; forced.\n");
            break;
        }
        }
        palCyc=0;
        DoPalAdditionalClock = (TV_FORMAT != TVSYSTEM_NTSC) ;
        dev_ppu->Initialize(TV_FORMAT);

        cout << ("NES: PPU initialized success !!\n");

        cout << ("NES: Initializing apu ....\n");
        dev_apu = new APU2A03();
        dev_apu->Initialize(TV_FORMAT);
        cout << ("NES: APU initialized success !!\n");

        cout << ("NES: Initializing board ....\n");
        cout << "NES: Requesting mapper " << " #" << INES::MapperNumber << "\n";
        NES::SetBoard(INES::MapperNumber);
        cout << "NES: -> MAPPER [Board: " << dev_board->Name << "] #" << dev_board->MapperNumber << " initialized successfully.\n";
        dev_board->Initialize(INES::prg_dump, INES::PRGSizeInBytes,
                              INES::chr_dump, INES::CHRSizeInBytes, INES::trainer_dump,INES::TrainerSizeInBytes,
                              INES::Mirroring, INES::HasBattery);

        cout << ("NES: Board initialized success !!\n");
        cout << ("NES: Checking if SRAM file is required ...\n");
        if (dev_board->SaveSRAMRequired())
        {
            cout << ("NES: ->SRAM is required, checking file ...\n");
            if (Path::IsFileExist(SRAM_FILE))
            {
                cout << ("NES: ->Reading SRAM file ...\n");
                uint32_t sram_size = dev_board->GetSRAMBufferSize();

                ifstream readStr(SRAM_FILE.c_str(), ios::in|ios::binary);
                if (readStr.good())
                {
                    readStr.seekg(0,ios::end);
                    uint32_t file_length = readStr.tellg();
                    readStr.seekg(0,ios::beg);
                    if (file_length != sram_size)
                    {
                        cout << ("NES: ->ERROR ! this file doesn't match the required sram size.\n");
                        readStr.close();
                    }
                    else
                    {
                        char * sram_buffer = new char [sram_size];
                        readStr.read(sram_buffer,sram_size);
                        readStr.close();
                        cout << ("NES: ->SRAM read successfully.\n");
                        // Load it to board !!
                        dev_board->GetSRAMFromBuffer(sram_buffer);
                        // Clear ..
                        delete [] sram_buffer;
                        cout << ("NES: SRAM applyed successfully.\n");
                    }
                }
                else
                {
                    cout << ("NES: ->ERROR ! could not open the sram file.\n");
                    readStr.close();
                }
            }
            else
            {
                cout << ("NES: ->ERROR ! SRAM file is not exist.\n");
            }
        }
        else
        {
            cout << ("NES: ->SRAM is not required.\n");
        }


        SetupVSUniSystem(INES::IsVSUnisystem,INES::MapperNumber);
// Setup the state
        state_str = new StateStream(EmuShell::GameCRC32);

        SetupJoypads();

        if (ConnectZapper)
        {
            zapp=new IZapperConnecter();
        }

        WRAM = new uint8_t[0x800];
        HardReset();

        if (request_state_load_after_intialize)
        {
            request_state_load_after_intialize=false;
            load_state();
        }

        // Finally !
        INES::Close();
        READY = true;
    }
    else
    {
        cout << ("NES: FILE IS NOT VALID !!\n");
    }
}
void HardReset()
{
    READY = false;
    // memory
    for (int i=0; i<0x800; i++)
        WRAM[i]=0;

    WRAM[0x0008] = 0xF7;
    WRAM[0x0008] = 0xF7;
    WRAM[0x0009] = 0xEF;
    WRAM[0x000A] = 0xDF;
    WRAM[0x000F] = 0xBF;
    BUS_ADDRESS = 0;
    BUS_RW = false;
    // Hard reset !
    cout << ("NES: HARD RESET ...\n");

    cout << ("NES: hard reset board...\n");
    dev_board->HardReset();
    cout << ("NES: hard reset board done.\n");

    cout << ("NES: hard reset cpu...\n");
    dev_cpu->HardReset();
    cout << ("NES: hard reset cpu done.\n");

    cout << ("NES: hard reset dma...\n");
    dev_dma = new DMAUnit();
    dev_dma->HardReset();
    cout << ("NES: hard reset dma done.\n");

    cout << ("NES: hard reset ppu...\n");
    dev_ppu->HardReset();
    cout << ("NES: hard reset ppu done.\n");

    cout << ("NES: hard reset apu...\n");
    dev_apu->HardReset();
    cout << ("NES: hard reset apu done.\n");

    cout << ("NES: HARD RESET DONE !!\n");

    READY = true;
}
void SoftReset()
{
    cout << ("NES: SOFT RESET ...\n");

    cout << ("NES: soft reset board...\n");
    dev_board->SoftReset();
    cout << ("NES: soft reset board done.\n");

    cout << ("NES: soft reset cpu...\n");
    dev_cpu->SoftReset();
    cout << ("NES: soft reset cpu done.\n");

    cout << ("NES: soft reset dma...\n");
    dev_dma->SoftReset();
    cout << ("NES: soft reset dma done.\n");

    cout << ("NES: soft reset ppu...\n");
    dev_ppu->SoftReset();
    cout << ("NES: soft reset ppu done.\n");

    cout << ("NES: soft reset apu...\n");
    dev_apu->SoftReset();
    cout << ("NES: soft reset apu done.\n");

    cout << ("NES: HARD RESET DONE !!\n");
}
/*********************CLOCKS**************************************/
void RUN()
{
    while (ON)
    {
        if (!PAUSED)
            dev_cpu->Clock();
        else
        {
            CheckRequests();
        }
    }
}
void RUNFRAME()
{
    dev_ppu->FrameFinished = false;
    if (ON)
    {
        if (!PAUSED)
        {
            while (!dev_ppu->FrameFinished)
            {
                dev_cpu->Clock();
            }
        }
        else
        {
            // Check for shortcuts
            SDLInput::Update();
            CheckRequests();
        }
    }
    dev_ppu->FrameFinished = false;
}
void ClockComponents()
{
    if (!READY)
        return;

    // In this point, phi1 is starting ...
    dev_ppu->Clock();
    dev_cpu->PollInterruptStatus();

    // Some point here, phi2 is starting, data bus should be full by now.
    dev_ppu->Clock();
    dev_ppu->Clock();

    if (DoPalAdditionalClock)// In pal/dendy system ..
    {
        palCyc++;
        if (palCyc == 5)
        {
            dev_ppu->Clock();
            palCyc = 0;
        }
    }

    // These clocks happens at the same time with the 3rd ppu clock
    // We can't do this with single-threaded emu :(
    dev_apu->Clock();
    dev_dma->Clock();
    dev_board->OnCPUClock();
}
void OnFinishFrame()
{
    joy1->Update();
    joy2->Update();
    if (IsFourPlayers)
    {
        joy3->Update();
        joy4->Update();
    }
    if (ConnectZapper)
        zapp->Update();
    if (ConnectVSUniSystem)
        vsuni_dip->Update();
    dev_apu->FinishFrame();
}
void ShutDown()
{
    if (READY)
    {
        if (dev_board->SaveSRAMRequired())
        {
            cout << ("NES: ->SRAM is battery loaded, saving file ...\n");

            Path::DeleteFile(SRAM_FILE);

            cout << ("NES: ->Saving SRAM file ...\n");
            uint32_t sram_size = dev_board->GetSRAMBufferSize();
            if (sram_size==0)
            {
                cout << ("NES: ->ERROR ! sram size is 0 !?\n");
            }
            else
            {
                char * sram_buffer = new char [sram_size];
                dev_board->SaveSRAMToBuffer(sram_buffer);

                ofstream saveStr(SRAM_FILE.c_str(), ios::out|ios::binary);
                if (saveStr.good())
                {
                    saveStr.write(sram_buffer,sram_size);
                    saveStr.close();
                    cout << ("NES: SRAM saved successfully.\n");
                }
                else
                {
                    saveStr.close();
                    cout << ("NES: ->ERROR ! could not create the file.\n");
                }
                // Clear ..
                delete [] sram_buffer;
            }
        }
        else
        {
            cout << ("NES: ->SRAM is not required.\n");
        }
        cout << ("NES: Shutting down cpu ...\n");
        dev_cpu->ShutDown();
        cout << ("NES: Shutting down board ...\n");
        dev_board->ShutDown();
        cout << ("NES: Shutting down apu ...\n");
        dev_apu->ShutDown();
        cout << ("NES: Shutting down ppu ...\n");
        dev_ppu->Shutdown();

        // Free pointers ...
        cout << ("NES: Freeing memory ...\n");
        delete [] WRAM;

        cout << ("NES: Shutdown successed.\n");
    }
    SaveSettings();
}
void CheckRequests()
{
    if (request_state_save)
    {
        request_state_save=false;
        save_state();
    }
    else if (request_state_load)
    {
        request_state_load=false;
        load_state();
    }
}
/*********************MEMORY ACCESS*******************************/
void MemWrite(uint16_t address, uint8_t value)
{
    BUS_RW = false;
    BUS_ADDRESS = address;
    ClockComponents();// Clock components one cycle
    if (address < 0x2000)
    {
        // Internal 2K Work RAM (mirrored to 800h-1FFFh)
        WRAM[address & 0x7FF] = value;
    }
    else if (address < 0x4000)
    {
        // Internal PPU Registers (mirrored to 2008h-3FFFh)
        switch (address & 0x7)
        {
        case 0:
        {
            dev_ppu->Write2000(value);
            break;
        }
        case 1:
        {
            dev_ppu->Write2001(value);
            break;
        }
        case 3:
        {
            dev_ppu->Write2003(value);
            break;
        }
        case 4:
        {
            dev_ppu->Write2004(value);
            break;
        }
        case 5:
        {
            dev_ppu->Write2005(value);
            break;
        }
        case 6:
        {
            dev_ppu->Write2006(value);
            break;
        }
        case 7:
        {
            dev_ppu->Write2007(value);
            break;
        }
        }
    }
    else if (address < 0x4020)
    {
        // Internal APU Registers
        switch (address)
        {
        case 0x4000:
        {
            dev_apu->Write4000(value);
            break;
        }
        case 0x4001:
        {
            dev_apu->Write4001(value);
            break;
        }
        case 0x4002:
        {
            dev_apu->Write4002(value);
            break;
        }
        case 0x4003:
        {
            dev_apu->Write4003(value);
            break;
        }
        case 0x4004:
        {
            dev_apu->Write4004(value);
            break;
        }
        case 0x4005:
        {
            dev_apu->Write4005(value);
            break;
        }
        case 0x4006:
        {
            dev_apu->Write4006(value);
            break;
        }
        case 0x4007:
        {
            dev_apu->Write4007(value);
            break;
        }
        case 0x4008:
        {
            dev_apu->Write4008(value);
            break;
        }
        case 0x400A:
        {
            dev_apu->Write400A(value);
            break;
        }
        case 0x400B:
        {
            dev_apu->Write400B(value);
            break;
        }
        case 0x400C:
        {
            dev_apu->Write400C(value);
            break;
        }
        case 0x400E:
        {
            dev_apu->Write400E(value);
            break;
        }
        case 0x400F:
        {
            dev_apu->Write400F(value);
            break;
        }
        case 0x4010:
        {
            dev_apu->Write4010(value);
            break;
        }
        case 0x4011:
        {
            dev_apu->Write4011(value);
            break;
        }
        case 0x4012:
        {
            dev_apu->Write4012(value);
            break;
        }
        case 0x4013:
        {
            dev_apu->Write4013(value);
            break;
        }
        case 0x4014:
        {
            dev_dma->AssertOAMDMA(value << 8);
            break;
        }
        case 0x4015:
        {
            dev_apu->Write4015(value);
            break;
        }
        case 0x4016:
        {
            if (inputStrobe > (value & 0x01))
            {
                if (IsFourPlayers)
                {
                    PORT0 = joy3->GetData() << 8 | joy1->GetData() | 0x01010000;
                    PORT1 = joy4->GetData() << 8 | joy2->GetData() | 0x02020000;
                }
                else
                {
                    PORT0 = joy1->GetData() | 0x01010100;// What is this ? see *
                    PORT1 = joy2->GetData() | 0x02020200;
                }
            }
            if (ConnectVSUniSystem)
                dev_board->VSUnisystem4016RW(value);
            inputStrobe = value & 0x01;
            break;
            // * The data port is 24 bits length
            // Each 8 bits indicates device, if that device is connected, then device data set on it normally...
            // So we have 4 block of data on each register ([] indicate uint8_t block here, let's call these blocks a SEQ)
            // SEQ:
            // [block 3] [block 2] [block 1] [block 0]
            // 0000 0000 0000 0000 0000 0000 0000 0000
            // ^ bit 23                              ^ bit 0
            // Let's say we connect joypad 1 and joypad2, then:
            // In $4016: the data could be like this [00h][00h][00h][joy1]
            // In $4017: the data could be like this [00h][00h][00h][joy2]
            // Instead of having 00h value on other blocks, the read returns a bit set on each unused block
            // to indicate that there's no device (i.e. joypad) is connected :
            // In $4016 the first bit (i.e. bit 0) is set if no device connected on that block
            // Example: [01h][01h][01h][joy1] (we only have joypad 1 connected so other blocks are blocked)
            // In $4017 work the same but with second bit (i.e. bit 1) is set if no device connected on other blocks
            // Example: [02h][02h][02h][joy2] (when we have joypad 2 connected so other blocks are blocked)
            // If we connect 4 joypads then:
            // $4016 : [01h][01h][joy3][joy1]
            // $4017 : [02h][02h][joy4][joy2]
        }
        case 0x4017:
        {
            dev_apu->Write4017(value);
            break;
        }
        }
    }
    else if (address < 0x6000)
    {
        if (ConnectVSUniSystem && address == 0x4020)
            vsuni_dip->Write4020(value);
        // Cartridge Expansion Area almost 8K
        dev_board->WriteEXP(address, value);
    }
    else if (address < 0x8000)
    {
        // Cartridge SRAM Area 8K
        dev_board->WriteSRM(address, value);
    }
    else if (address <= 0xFFFF)
    {
        // Cartridge PRG-ROM Area 32K
        dev_board->WritePRG(address, value);
    }
}
uint8_t MemRead(uint16_t address)
{
    BUS_RW = true;
    BUS_ADDRESS = address;
    ClockComponents();// Clock components one cycle
    if (address < 0x2000)
    {
        // Internal 2K Work RAM (mirrored to 800h-1FFFh)
        return WRAM[address & 0x7FF];
    }
    else if (address < 0x4000)
    {
        // Internal PPU Registers (mirrored to 2008h-3FFFh)
        switch (address & 7)
        {
        case 2:
        {
            return dev_ppu->Read2002();
        }
        case 4:
        {
            return dev_ppu->Read2004();
        }
        case 7:
        {
            return dev_ppu->Read2007();
        }
        }
    }
    else if (address < 0x4020)
    {
        // Internal APU Registers
        switch (address)
        {
        case 0x4015:
        {
            return dev_apu->Read4015();
        }
        case 0x4016:
        {
            temp_4016 = PORT0 & 1;

            PORT0 >>= 1;

            if (ConnectZapper)
                temp_4016 |= zapp->GetData();
            if (ConnectVSUniSystem)
                temp_4016 |= vsuni_dip->GetData4016();
            return temp_4016;
        }
        case 0x4017:
        {
            temp_4017 = PORT1 & 1;

            PORT1 >>= 1;

            if (ConnectZapper)
                temp_4017 |= zapp->GetData();
            if (ConnectVSUniSystem)
                temp_4017 |= vsuni_dip->GetData4017();
            return temp_4017;
        }
        }
    }
    else if (address < 0x6000)
    {
        // Cartridge Expansion Area almost 8K
        return	dev_board->ReadEXP(address);
    }
    else if (address < 0x8000)
    {
        // Cartridge SRAM Area 8K
        return dev_board->ReadSRM(address);
    }
    else if (address <= 0xFFFF)
    {
        // Cartridge PRG-ROM Area 32K
        return dev_board->ReadPRG(address);
    }
    return 0;// Should not reach here !
}

/*********************SETUP***************************************/
void SetupJoypads()
{
    cout << ("NES: Initializing inputs ....\n");
    // Player 1
    if (pl1_use_joystick && SDLInput::JOYSTICKS_NUMBER>0)
        joy1 = new JoypadJoystickConnecter(0,pl1_joystick_index);
    else
        joy1 = new IJoypadConnecter(0);

    // Player 2
    if (pl2_use_joystick && SDLInput::JOYSTICKS_NUMBER>0)
        joy2 = new JoypadJoystickConnecter(1,pl2_joystick_index);
    else
        joy2 = new IJoypadConnecter(1);

    // Player 3
    if (pl3_use_joystick && SDLInput::JOYSTICKS_NUMBER>0)
        joy3 = new JoypadJoystickConnecter(2,pl3_joystick_index);
    else
        joy3 = new IJoypadConnecter(2);

    // Player 4
    if (pl4_use_joystick && SDLInput::JOYSTICKS_NUMBER>0)
        joy4 = new JoypadJoystickConnecter(3,pl4_joystick_index);
    else
        joy4 = new IJoypadConnecter(3);

    if (ConnectVSUniSystem)
    {
        if (vs_use_joystick && SDLInput::JOYSTICKS_NUMBER>0)
            vsuni_dip=new VSUniSystemJoystickConnecter(vs_joystick_index);
        else
            vsuni_dip=new IVSUniSystemConnecter();

        vsuni_dip->LoadKeys();
    }

    joy1->LoadKeys();
    joy2->LoadKeys();
    joy3->LoadKeys();
    joy4->LoadKeys();

    cout << ("NES: Inputs initialized success !!\n");
}
void SetupSoundPlayback(const bool& soundEnabled, const uint32_t& frequency)
{
    dev_apu->SetupSoundPlayback(soundEnabled, frequency);
}
void SetupVSUniSystem(bool setByHeader, int mapper)
{
    // First thing we set VS by the header ... not trustful choice
    ConnectVSUniSystem = setByHeader;
// Check by mapper
    if (!ConnectVSUniSystem)
    {
        if (mapper == 99)
            ConnectVSUniSystem =true;
    }
// Check by name !
    if (!ConnectVSUniSystem)
    {
        if (EmuShell::FileName.find("(VS)") != std::string::npos)
        {
            ConnectVSUniSystem =true;
        }
    }
// By CRC !
// So sad that there is no way to setup VS UniSystem games on the header nor database.
// TODO: complete the VS UniSustem palette indexes setup
    switch(EmuShell::GameCRC32)
    {
    case 0xd59f31e8:// Super Skater
    case 0xcba9144d:// Super Skater
    case 0x0fa322c2:// Clu Clu Land (VS)
    case 0x98e3c75a:// VS Super Mario Bros
    {
        uint32_t* VS1 =new uint32_t[512]
        {
            0x18,0x3f,0x1c,0x3f,0x3f,0x3f,0x0b,0x17,0x10,0x3f,0x14,0x3f,0x36,0x37,0x1a,0x3f,
            0x25,0x3f,0x12,0x3f,0x0f,0x3f,0x3f,0x3f,0x3f,0x3f,0x22,0x19,0x3f,0x3f,0x3a,0x21,
            0x3f,0x3f,0x07,0x3f,0x3f,0x3f,0x00,0x15,0x0c,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,
            0x3f,0x3f,0x07,0x16,0x3f,0x3f,0x30,0x3c,0x3f,0x27,0x3f,0x3f,0x29,0x3f,0x1b,0x09,
        };
        dev_ppu->SetupPaletteIndexes(VS1);

        delete [] VS1;
        break;
    }
    case 0x21a653c7:// Super Sky Kid (VS)
    case 0x01357944:// Battle City (VS)
    case 0x832cf592:// Freedom Force (VS)
    case 0x9768e5e0:// Golf
    case 0x63abf889:// Ladies Golf
    case 0xe8b20197:// Mach Rider (VS)
    case 0x9ae2baa0:// VS Slalom (VS)
    {
        uint32_t* VS2 =new uint32_t[512]
        {
            0x0f,0x27,0x18,0x3f,0x3f,0x25,0x3f,0x34,0x16,0x13,0x3f,0x3f,0x20,0x23,0x3f,0x0b,
            0x3f,0x3f,0x06,0x3f,0x1b,0x3f,0x3f,0x22,0x3f,0x24,0x3f,0x3f,0x32,0x3f,0x3f,0x03,
            0x3f,0x37,0x26,0x33,0x11,0x3f,0x10,0x3f,0x14,0x3f,0x00,0x09,0x12,0x0f,0x3f,0x30,
            0x3f,0x3f,0x2a,0x17,0x0c,0x01,0x15,0x19,0x3f,0x3f,0x07,0x37,0x3f,0x05,0x3f,0x3f,
        };
        dev_ppu->SetupPaletteIndexes(VS2);

        delete [] VS2;
        break;
    }

    case 0xffbef374:// VS Castlevania (VS)
    {
        uint32_t* VS3 =new uint32_t[512]
        {
            0x3f,0x3f,0x3f,0x3f,0x1a,0x30,0x3c,0x09,0x0f,0x0f,0x3f,0x0f,0x3f,0x3f,0x3f,0x30,
            0x32,0x1c,0x3f,0x12,0x3f,0x18,0x17,0x3f,0x0c,0x3f,0x3f,0x02,0x16,0x3f,0x3f,0x3f,
            0x3f,0x3f,0x0f,0x37,0x3f,0x28,0x27,0x3f,0x29,0x3f,0x21,0x3f,0x11,0x3f,0x0f,0x3f,
            0x31,0x3f,0x3f,0x3f,0x0f,0x2a,0x28,0x3f,0x3f,0x3f,0x3f,0x3f,0x13,0x3f,0x3f,0x3f,
        };
        dev_ppu->SetupPaletteIndexes(VS3);

        delete [] VS3;
        break;
    }
    case 0x766c2cac:// Soccer (VS)
    {
        uint32_t* VS4 =new uint32_t[512]
        {
            0x0f,0x3f,0x3f,0x10,0x3f,0x30,0x31,0x3f,0x01,0x0f,0x36,0x3f,0x3f,0x3f,0x3f,0x3c,
            0x3f,0x3f,0x3f,0x12,0x19,0x3f,0x17,0x3f,0x00,0x3f,0x3f,0x02,0x16,0x3f,0x3f,0x3f,
            0x3f,0x3f,0x3f,0x37,0x3f,0x27,0x26,0x20,0x3f,0x04,0x22,0x3f,0x11,0x3f,0x3f,0x3f,
            0x2c,0x3f,0x3f,0x3f,0x07,0x2a,0x3f,0x3f,0x3f,0x3f,0x3f,0x38,0x13,0x3f,0x3f,0x0c,
        };
        dev_ppu->SetupPaletteIndexes(VS4);

        delete [] VS4;
        break;
    }
    case 0xef7af338:// Ice Climber (VS)
    {
        uint32_t* VS5 =new uint32_t[512]
        {
            0x18,0x3f,0x1c,0x3f,0x3f,0x3f,0x01,0x17,0x10,0x3f,0x2a,0x3f,0x36,0x37,0x1a,0x39,
            0x25,0x3f,0x12,0x3f,0x0f,0x3f,0x3f,0x26,0x3f,0x3f,0x22,0x19,0x3f,0x0f,0x3a,0x21,
            0x3f,0x0a,0x07,0x06,0x13,0x3f,0x00,0x15,0x0c,0x3f,0x11,0x3f,0x3f,0x38,0x3f,0x3f,
            0x3f,0x3f,0x07,0x16,0x3f,0x3f,0x30,0x3c,0x0f,0x27,0x3f,0x31,0x29,0x3f,0x11,0x09,
        };
        dev_ppu->SetupPaletteIndexes(VS5);

        delete [] VS5;
        break;
    }
    case 0xca3e9b1a:// Pinball (VS)
    {
        uint32_t* VS6 =new uint32_t[512]
        {
            0x35,0x3f,0x16,0x3f,0x1c,0x3f,0x3f,0x15,0x3f,0x3f,0x27,0x05,0x04,0x3f,0x3f,0x30,
            0x21,0x3f,0x3f,0x3f,0x3f,0x3f,0x36,0x12,0x3f,0x2b,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,
            0x3f,0x31,0x3f,0x2a,0x2c,0x0c,0x3f,0x3f,0x3f,0x07,0x34,0x06,0x3f,0x25,0x26,0x0f,
            0x3f,0x19,0x10,0x3f,0x3f,0x3f,0x3f,0x17,0x3f,0x11,0x3f,0x3f,0x3f,0x3f,0x18,0x3f,
        };
        dev_ppu->SetupPaletteIndexes(VS6);

        delete [] VS6;
        break;
    }
    case 0xe9a6f17d:// Tetris
    {
        uint32_t* VS7 =new uint32_t[512]
        {
            0x35,0x3f,0x16,0x22,0x1c,0x3f,0x3f,0x15,0x3f,0x00,0x27,0x05,0x04,0x3f,0x3f,0x30,
            0x21,0x3f,0x3f,0x29,0x3c,0x3f,0x36,0x12,0x3f,0x2b,0x3f,0x3f,0x3f,0x3f,0x3f,0x01,
            0x3f,0x31,0x3f,0x2a,0x2c,0x0c,0x3f,0x3f,0x3f,0x07,0x34,0x06,0x3f,0x25,0x26,0x0f,
            0x3f,0x19,0x10,0x3f,0x3f,0x3f,0x3f,0x17,0x3f,0x11,0x3f,0x3f,0x3f,0x25,0x18,0x3f,
        };
        dev_ppu->SetupPaletteIndexes(VS7);

        delete [] VS7;
        break;
    }
    }
}
/*********************REQUESTS*************************************/
void RequestHardReset()
{
    // Since this can be called only at frame finish, we can hard reset directly.
    HardReset();
}
void RequestSoftReset()
{
    // Since this can be called only at frame finish, we can hard reset directly.
    SoftReset();
}
void RequestSaveState()
{
    PAUSED=true;
    request_state_save=true;
}
void RequestLoadState()
{
    PAUSED=true;
    request_state_load=true;
}
/*********************STATE***************************************/
void save_state()
{
    PAUSED = true;//Prevent further cpu clocks
    // Make the file

    string game_name = Path::GetFileNameWithoutExtnsion((string)EmuShell::FileName) +"_"+
                       Convert::T_to_string<int>( STATE_SLOT);

    string state_file =Path::Combine(STATE_FOLDER,game_name+".est");
    string snap_file = Path::Combine(STATE_FOLDER,game_name+".png");
// Delete the old save if already exist
    Path::DeleteFile(state_file);

// Start saving !!
    VideoOut::WriteStatus("Saving state ...",-1,0x00FF00,false);

    state_str->Clear();// Start a new
    // Write cpu
    uint8_t * cpu_state = new uint8_t[dev_cpu->state_buffer_length];
    dev_cpu->GetStateAsBuffer(cpu_state);
    state_str->Write(cpu_state,dev_cpu->state_buffer_length);
    delete [] cpu_state;
    // Other components ...
    dev_apu->SaveState();
    dev_ppu->SaveState();
    dev_dma->SaveState();
    dev_board->SaveState();

    // Write nes mother stuff
    state_str->Write(WRAM,0x800);
    state_str->Write(PORT0);
    state_str->Write(PORT1);
    state_str->Write(inputStrobe);

// Write into file !
    state_str->WriteToFile(state_file);

// Save snapshot ...
    VideoOut::TakeSnapshot(snap_file.c_str(),".png");

    VideoOut::WriteStatus("Saving state ...",1,0x00FF00,false);
    VideoOut::WriteNotification(("State saved at slot # "+Convert::to_string<int>(STATE_SLOT)).c_str(),120,0x00FF00);
    PAUSED = false;
}
void load_state()
{
    std::cout << "NES: Loading state ...\n";
    PAUSED = true;//Prevent further cpu clocks
    // Make the file

    string game_name = Path::GetFileNameWithoutExtnsion((string)EmuShell::FileName) +"_"+
                       Convert::T_to_string<int>( STATE_SLOT)+".est";

    string state_file =Path::Combine(STATE_FOLDER,game_name);

    cout << "NES: State file path = "<<state_file<<"\n";
// Start saving !!
    VideoOut::WriteStatus("Loading state ...",-1,0x00FF00,false);

    // Write into file !
    if ( state_str->ReadFromFile(state_file))
    {
        std::cout << "NES: reading cpu state ..\n";
        uint8_t* cpu_state =new uint8_t[dev_cpu->state_buffer_length];
        state_str->Read(cpu_state,dev_cpu->state_buffer_length);
        dev_cpu->SetStateFromBufer(cpu_state);
        delete [] cpu_state;
        std::cout << "NES: reading apu state ...\n";
        dev_apu->LoadState();
        std::cout << "NES: reading ppu state ...\n";
        dev_ppu->LoadState();
        std::cout << "NES: reading dma state ...\n";
        dev_dma->LoadState();
        std::cout << "NES: reading board state ...\n";
        dev_board->LoadState();
        std::cout << "NES: reading nes motherboard state ...\n";
        // Write nes mother stuff
        state_str->Read(WRAM,0x800);
        state_str->Read(PORT0);
        state_str->Read(PORT1);
        state_str->Read(inputStrobe);

        std::cout << "NES: read state done successfully.\n";

        VideoOut::WriteStatus("Loading state ....",10,0x00FF00,false);
        VideoOut::WriteNotification(("State loaded from slot # "+Convert::to_string<int>(STATE_SLOT)).c_str(),120,0x00FF00);
    }
    else
    {
        VideoOut::WriteStatus("Loading state ....",10,0x00FF00,false);
        VideoOut::WriteNotification(("ERROR: could not load state from slot # "+Convert::to_string<int>(STATE_SLOT)).c_str(),
                                    120,0x0000FF);
    }
    PAUSED = false;
}
/*********************HANDLERS************************************/
void OnMouseClick(const int &button)
{
    if (button == SDL_BUTTON_LEFT)
        if (ConnectZapper)
            zapp->OnmouseButtonDownUp(true);
}
void OnMouseUp(const int &button)
{
    if (button == SDL_BUTTON_LEFT)
        if (ConnectZapper)
            zapp->OnmouseButtonDownUp(false);
}
/*********************MISC****************************************/
void ShowStateSlotDetails()
{
    // Get the state image file if exist
    VideoOut::HideImageNotification();
    string game_name = Path::GetFileNameWithoutExtnsion((std::string)EmuShell::FileName) +"_"+Convert::T_to_string<int>( STATE_SLOT);

    std::string snap_file = Path::Combine(STATE_FOLDER,game_name+".png");

    if (Path::IsFileExist(snap_file))
        VideoOut::DrawImageNotification(snap_file.c_str(),120);
}
void ExecuteCommands(int argc,char* args[])
{
    for (int i = 2; i<argc; i++)
    {
        if ((std::string) args[i] == "-nes.use_4_players")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("nes.use_4_players", (bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i] == "-nes.tv_format_mode")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("nes.tv_format_mode",Convert::string_to_T<uint32_t>((std::string)args[i]));
        }
        else  if ((std::string) args[i] == "-nes.connect_zapper")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("nes.connect_zapper", (bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-load_state")
        {
            std::cout << "]-load_state\n";
            // Make load state
            request_state_load_after_intialize=true;
        }
        else if ((std::string) args[i]== "-state_slot_0")
        {
            std::cout << "]-state_slot_0\n";
            STATE_SLOT = 0;
        }
        else if ((std::string) args[i]== "-state_slot1")
        {
            std::cout << "]-state_slot_1\n";
            STATE_SLOT = 1;
        }
        else if ((std::string) args[i]== "-state_slot_2")
        {
            std::cout << "]-state_slot_2\n";
            STATE_SLOT = 2;
        }
        else if ((std::string) args[i]== "-state_slot_3")
        {
            std::cout << "]-state_slot_3\n";
            STATE_SLOT = 3;
        }
        else if ((std::string) args[i]== "-state_slot_4")
        {
            std::cout << "]-state_slot_4\n";
            STATE_SLOT = 4;
        }
        else if ((std::string) args[i]== "-state_slot_5")
        {
            std::cout << "]-state_slot_5\n";
            STATE_SLOT = 5;
        }
        else if ((std::string) args[i]== "-state_slot_6")
        {
            std::cout << "]-state_slot_6\n";
            STATE_SLOT = 6;
        }
        else if ((std::string) args[i]== "-state_slot_7")
        {
            std::cout << "]-state_slot_7\n";
            STATE_SLOT = 7;
        }
        else if ((std::string) args[i]== "-state_slot_8")
        {
            std::cout << "]-state_slot_8\n";
            STATE_SLOT = 8;
        }
        else if ((std::string) args[i]== "-state_slot_9")
        {
            std::cout << "]-state_slot_9\n";
            STATE_SLOT = 9;
        }
    }
}
}
