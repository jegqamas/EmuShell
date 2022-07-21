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

#include "Core.h"
#include "Definations.h"
#include "Nes/Nes.h"
#include "Providers/SDLInput.h"
#include "Providers/VideoOut.h"
#include "Common/ConfigHandler.h"
#include <stdio.h>
#include <SDL2/SDL.h>

namespace EmuShell
{
// Current loaded emu system
std::string SYSTEM;
uint32_t SystemID;
// Current loaded file
std::string FileName;
// Current loaded game title as provided by system.
std::string GameTitle;
// Current loaded file's CRC32 according to system;
// For Nes for example, the file CRC calculated without header
uint32_t GameCRC32;
// Indicates if sound playback is enabled or not
bool sound_enabled;
// The sound playback frequency
int sound_freq;
// The sound playback buffer size in bytes. Default is 1256
int sound_buffer_size;
// Shortcuts
Sint32 Key_TogglePause;
Sint32 Key_ShowInfo;
Sint32 Key_HardReset;
Sint32 Key_SoftReset;
Sint32 Key_SaveState;
Sint32 Key_LoadState;
Sint32 Key_StateSlot0;
Sint32 Key_StateSlot1;
Sint32 Key_StateSlot2;
Sint32 Key_StateSlot3;
Sint32 Key_StateSlot4;
Sint32 Key_StateSlot5;
Sint32 Key_StateSlot6;
Sint32 Key_StateSlot7;
Sint32 Key_StateSlot8;
Sint32 Key_StateSlot9;
Sint32 Key_NextStateSlot;
Sint32 Key_PreviousStateSlot;

Sint32 JoyBut_TogglePause;
Sint32 JoyBut_ShowInfo;
Sint32 JoyBut_HardReset;
Sint32 JoyBut_SoftReset;
Sint32 JoyBut_SaveState;
Sint32 JoyBut_LoadState;
Sint32 JoyBut_StateSlot0;
Sint32 JoyBut_StateSlot1;
Sint32 JoyBut_StateSlot2;
Sint32 JoyBut_StateSlot3;
Sint32 JoyBut_StateSlot4;
Sint32 JoyBut_StateSlot5;
Sint32 JoyBut_StateSlot6;
Sint32 JoyBut_StateSlot7;
Sint32 JoyBut_StateSlot8;
Sint32 JoyBut_StateSlot9;
Sint32 JoyBut_NextStateSlot;
Sint32 JoyBut_PreviousStateSlot;

void LoadSettings()
{
    sound_enabled = CONFIG::GetBoolValue("sound_enable",true);
    sound_freq= CONFIG::GetInt32Value("sound_freq",44100);
    sound_buffer_size= CONFIG::GetInt32Value("sound_buffer_size",1256);
    Key_TogglePause= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.toggle_pause","F2").c_str());
    Key_ShowInfo= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.show_info","F7").c_str());
    Key_HardReset=  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.hard_reset","F4").c_str());
    Key_SoftReset=  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.soft_reset","F3").c_str());
    Key_SaveState=  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.save_state","F6").c_str());
    Key_LoadState=  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.load_state","F9").c_str());
    Key_StateSlot0= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_0","0").c_str());
    Key_StateSlot1= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_1","1").c_str());
    Key_StateSlot2= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_2","2").c_str());
    Key_StateSlot3= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_3","3").c_str());
    Key_StateSlot4= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_4","4").c_str());
    Key_StateSlot5= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_5","5").c_str());
    Key_StateSlot6= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_6","6").c_str());
    Key_StateSlot7= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_7","7").c_str());
    Key_StateSlot8= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_8","8").c_str());
    Key_StateSlot9= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.state_slot_9","9").c_str());
    Key_NextStateSlot= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.next_state_slot","Unknown").c_str());
    Key_PreviousStateSlot= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.prev_state_slot","Unknown").c_str());

    JoyBut_TogglePause= CONFIG::GetInt32Value("shortcut.joystick.toggle_pause",-1);
    JoyBut_ShowInfo= CONFIG::GetInt32Value("shortcut.joystick.show_info",-1);
    JoyBut_HardReset=  CONFIG::GetInt32Value("shortcut.joystick.hard_reset",-1);
    JoyBut_SoftReset=  CONFIG::GetInt32Value("shortcut.joystick.soft_reset",-1);
    JoyBut_SaveState=  CONFIG::GetInt32Value("shortcut.joystick.save_state",6);
    JoyBut_LoadState=  CONFIG::GetInt32Value("shortcut.joystick.load_state",4);
    JoyBut_StateSlot0= CONFIG::GetInt32Value("shortcut.joystick.state_slot_0",-1);
    JoyBut_StateSlot1=CONFIG::GetInt32Value("shortcut.joystick.state_slot_1",-1);
    JoyBut_StateSlot2= CONFIG::GetInt32Value("shortcut.joystick.state_slot_2",-1);
    JoyBut_StateSlot3= CONFIG::GetInt32Value("shortcut.joystick.state_slot_3",-1);
    JoyBut_StateSlot4= CONFIG::GetInt32Value("shortcut.joystick.state_slot_4",-1);
    JoyBut_StateSlot5= CONFIG::GetInt32Value("shortcut.joystick.state_slot_5",-1);
    JoyBut_StateSlot6= CONFIG::GetInt32Value("shortcut.joystick.state_slot_6",-1);
    JoyBut_StateSlot7= CONFIG::GetInt32Value("shortcut.joystick.state_slot_7",-1);
    JoyBut_StateSlot8= CONFIG::GetInt32Value("shortcut.joystick.state_slot_8",-1);
    JoyBut_StateSlot9= CONFIG::GetInt32Value("shortcut.joystick.state_slot_9",-1);
    JoyBut_NextStateSlot= CONFIG::GetInt32Value("shortcut.joystick.next_state_slot",5);
    JoyBut_PreviousStateSlot= CONFIG::GetInt32Value("shortcut.joystick.prev_state_slot",7);
}
void SaveSettings()
{
    CONFIG::SetValue("sound_enable",sound_enabled);
    CONFIG::SetValue("sound_freq",sound_freq);
    CONFIG::SetValue("sound_buffer_size",sound_buffer_size);
    CONFIG::SetValue("shortcut.keyboard.toggle_pause",(std::string)SDL_GetKeyName(Key_TogglePause));
    CONFIG::SetValue("shortcut.keyboard.show_info",(std::string)SDL_GetKeyName(Key_ShowInfo));
    CONFIG::SetValue("shortcut.keyboard.hard_reset",(std::string)SDL_GetKeyName(Key_HardReset));
    CONFIG::SetValue("shortcut.keyboard.soft_reset",(std::string)SDL_GetKeyName(Key_SoftReset));
    CONFIG::SetValue("shortcut.keyboard.save_state",(std::string)SDL_GetKeyName(Key_SaveState));
    CONFIG::SetValue("shortcut.keyboard.load_state",(std::string)SDL_GetKeyName(Key_LoadState));
    CONFIG::SetValue("shortcut.keyboard.state_slot_0",(std::string)SDL_GetKeyName(Key_StateSlot0));
    CONFIG::SetValue("shortcut.keyboard.state_slot_1",(std::string)SDL_GetKeyName(Key_StateSlot1));
    CONFIG::SetValue("shortcut.keyboard.state_slot_2",(std::string)SDL_GetKeyName(Key_StateSlot2));
    CONFIG::SetValue("shortcut.keyboard.state_slot_3",(std::string)SDL_GetKeyName(Key_StateSlot3));
    CONFIG::SetValue("shortcut.keyboard.state_slot_4",(std::string)SDL_GetKeyName(Key_StateSlot4));
    CONFIG::SetValue("shortcut.keyboard.state_slot_5",(std::string)SDL_GetKeyName(Key_StateSlot5));
    CONFIG::SetValue("shortcut.keyboard.state_slot_6",(std::string)SDL_GetKeyName(Key_StateSlot6));
    CONFIG::SetValue("shortcut.keyboard.state_slot_7",(std::string)SDL_GetKeyName(Key_StateSlot7));
    CONFIG::SetValue("shortcut.keyboard.state_slot_8",(std::string)SDL_GetKeyName(Key_StateSlot8));
    CONFIG::SetValue("shortcut.keyboard.state_slot_9",(std::string)SDL_GetKeyName(Key_StateSlot9));
    CONFIG::SetValue("shortcut.keyboard.next_state_slot",(std::string)SDL_GetKeyName(Key_NextStateSlot));
    CONFIG::SetValue("shortcut.keyboard.prev_state_slot",(std::string)SDL_GetKeyName(Key_PreviousStateSlot));

    CONFIG::SetValue("shortcut.joystick.toggle_pause",JoyBut_TogglePause);
    CONFIG::SetValue("shortcut.joystick.show_info",JoyBut_ShowInfo);
    CONFIG::SetValue("shortcut.joystick.hard_reset",JoyBut_HardReset);
    CONFIG::SetValue("shortcut.joystick.soft_reset",JoyBut_SoftReset);
    CONFIG::SetValue("shortcut.joystick.save_state",JoyBut_SaveState);
    CONFIG::SetValue("shortcut.joystick.load_state",JoyBut_LoadState);
    CONFIG::SetValue("shortcut.joystick.state_slot_0",JoyBut_StateSlot0);
    CONFIG::SetValue("shortcut.joystick.state_slot_1",JoyBut_StateSlot1);
    CONFIG::SetValue("shortcut.joystick.state_slot_2",JoyBut_StateSlot2);
    CONFIG::SetValue("shortcut.joystick.state_slot_3",JoyBut_StateSlot3);
    CONFIG::SetValue("shortcut.joystick.state_slot_4",JoyBut_StateSlot4);
    CONFIG::SetValue("shortcut.joystick.state_slot_5",JoyBut_StateSlot5);
    CONFIG::SetValue("shortcut.joystick.state_slot_6",JoyBut_StateSlot6);
    CONFIG::SetValue("shortcut.joystick.state_slot_7",JoyBut_StateSlot7);
    CONFIG::SetValue("shortcut.joystick.state_slot_8",JoyBut_StateSlot8);
    CONFIG::SetValue("shortcut.joystick.state_slot_9",JoyBut_StateSlot9);
    CONFIG::SetValue("shortcut.joystick.next_state_slot",JoyBut_NextStateSlot);
    CONFIG::SetValue("shortcut.joystick.prev_state_slot",JoyBut_PreviousStateSlot);
}
void Shutdown()
{
    // Shutdown providers
    std::cout << "Shutting down video ... \n";
    VideoOut::Shutdown();
    std::cout << "Shutting down audio ... \n";
    AudioOut::Shutdown();
    std::cout << "Shutting down inputs ... \n";
    SDLInput::ShutDown();

    // Shutdown the emu engine system.
    if (SystemID==SYSTEM_ID_NES)
    {
        std::cout << "Shutting down nes emu engine ... \n";
        NES::ShutDown();
    }

    std::cout << "Saving settings ... \n";
    SaveSettings();
    // Save configuration
    CONFIG::Save();

    std::cout << "EmuShell is shutdown now. Good bye :) \n";
}
void Initialize(int argc,char* args[])
{
    CONFIG::Load ("conf.ini");
    LoadSettings();

    SDLInput::Initialize();// First thing must be initialized
    VideoOut::SetupFrameLimitter(60);

    if (argc < 2)
    {
        // Initialize system.
        SetupEmu("game.nes");
    }
    else
    {
        // Other commands will override the settings
        ExecuteCommands(argc, args);

        // We are expecting file path
        SetupEmu(args[1]);
    }

    VideoOut::Run();// Run until ON is set to off.

    Shutdown();
}
void SetupEmu(std::string filename)
{
    if (!Path::IsFileExist(filename))
    {
        std::cout << "No file to load, run standby.\n";

        // Run video with desired configuration.
        // We give it the system's render-frame method pointer that should be called each frame ...
        VideoOut::Initialize(0, 0, NULL);
        VideoOut::ON = true;// Turn renderer on and make it run on thread.
        VideoOut::EMU_ON = false;// Turn off the emu outputs handling for now.

        VideoOut::WriteNotification("STANDBY",120,0x00FF00);
        FileName="STANDBY";
        SYSTEM =SYSTEM_NULL;
        SystemID = SYSTEM_ID_NULL;
        return;
    }

    // TODO: find a better way to detect emu system.
    std::string ext = filename;

    ext.erase(0,ext.size() - 4);

    if (ext== FILE_FORMAT_INES)
    {
        SetupNES(filename);
    }
    else
    {
        std::cout << "No system detected, the file format is not supported.\n";

        // Run video with desired configuration.
        // We give it the system's render-frame method pointer that should be called each frame ...
        VideoOut::Initialize(0, 0, NULL);
        VideoOut::ON = true;// Turn renderer on and make it run on thread.
        VideoOut::EMU_ON = false;// Turn off the emu outputs handling for now.

        VideoOut::WriteNotification("SYSTEM IS NOT SUPPORTED",360,0xFF0000);

        FileName="STANDBY";
        SYSTEM =SYSTEM_NULL;
        SystemID=SYSTEM_ID_NULL;
    }
}
void SetupNES(std::string filename)
{
    printf("Initializing nes ....\n");
    FileName=filename;
    NES::Initialize(filename);
    if (NES::READY)
    {
        SYSTEM =SYSTEM_NES;
        SystemID=SYSTEM_ID_NES;
        NES::SetupSoundPlayback(sound_enabled, sound_freq);
        NES::ON = true;
        NES::PAUSED = false;
        // Run video with desired configuration.
        // We give it the system's render-frame method pointer that should be called each frame ...
        VideoOut::Initialize(NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT, NES::RUNFRAME);
        VideoOut::ON = true;// Turn renderer on and make it run on thread.
        //VideoOut::EMU_ON = true;// Turn off the emu outputs handling for now.
        VideoOut::SetEmuOn(20);// Run emu after 60 frames
        VideoOut::ShowHideCursor(NES::ConnectZapper);// If zapper is connected, show the cursor

        AudioOut::Initialize(sound_enabled, sound_freq, sound_buffer_size);

        if (!NES::BoardFound)
        {
            std::string inf = "NTSC NES | ";
            if (NES::TV_FORMAT == TVSYSTEM_NTSC)
                inf = "NTSC NES | ";
            else if (NES::TV_FORMAT == TVSYSTEM_PALB)
                inf ="PALB NES | ";
            else if (NES::TV_FORMAT == TVSYSTEM_DENDY)
                inf ="DENDY NES | ";
            inf+="Mapper is not supported ! Switched to NROM [mapper # 0]; GAME MAY NOT WORK !";
            VideoOut::WriteNotification(inf.c_str(),360, 0xFF0000);
        }
        else
        {
            if (NES::TV_FORMAT == TVSYSTEM_NTSC)
                VideoOut::WriteNotification("NTSC NES",60,0x00FF00);
            else if (NES::TV_FORMAT == TVSYSTEM_PALB)
                VideoOut::WriteNotification("PALB NES",60,0x00FF00);
            else if (NES::TV_FORMAT == TVSYSTEM_DENDY)
                VideoOut::WriteNotification("DENDY NES",60,0x00FF00);
        }

    }
    else
    {
        // Run video with desired configuration.
        // We give it the system's render-frame method pointer that should be called each frame ...
        VideoOut::Initialize(0, 0, NULL);
        VideoOut::ON = true;// Turn renderer on and make it run on thread.
        VideoOut::EMU_ON = false;// Turn off the emu outputs handling for now.

        VideoOut::WriteNotification("ERROR LOADING NES FILE !",360,0xFF0000);
        SYSTEM =SYSTEM_NULL;
        SystemID=SYSTEM_ID_NULL;
        FileName="STANDBY";
    }
}
void ShowSystemInfo()
{
    if (SystemID == SYSTEM_ID_NES)
    {
        std::string system = "NTSC";
        if (NES::TV_FORMAT == TVSYSTEM_PALB)
            system = "PALB";
        else if (NES::TV_FORMAT == TVSYSTEM_DENDY)
            system = "DENDY";
        if (NES::GameDBIndex>=0)
        {
            std::string inf = "SYSTEM: NES\nTV FORMAT: "+ system+"\nGAME NAME: "+NES::GameTitle+"\nBOARD: "+
                              NES::NesDB::GAMES[NES::GameDBIndex].Board_Type+" [Mapper #"+
                              NES::NesDB::GAMES[NES::GameDBIndex].Board_Mapper+", Chips: "+
                              NES::NesDB::GAMES[NES::GameDBIndex].chip_types+ "]";

            VideoOut::WriteStatus(inf.c_str(),180,0x00FF00,true);
        }
        else
        {
            std::string inf = "SYSTEM: NES\nTV FORMAT: "+ system+"\nGAME NAME: "+NES::GameTitle+"\nMAPPER # "+
                              Convert::T_to_string<uint32_t>( NES::dev_board->MapperNumber) + " ["+
                              NES::dev_board->Name+"]";

            VideoOut::WriteStatus(inf.c_str(),180,0x00FF00,true);
        }

    }
}
void CheckSystemShortcut(const int &shortcut)
{
    // Shortcuts
    if (shortcut == Key_TogglePause)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::PAUSED = !NES::PAUSED;
            if (NES::PAUSED)
                VideoOut::WriteStatus("PAUSED !",-1,0x00FF00,false);
            else
                VideoOut::WriteStatus("Running .. !",10,0x00FF00,false);
        }
    }
    else if (shortcut == Key_ShowInfo)
    {
        ShowSystemInfo();
    }
    else if (shortcut == Key_HardReset)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestHardReset();
    }
    else if (shortcut == Key_SoftReset)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES:: RequestSoftReset();
    }
    else if (shortcut == Key_SaveState)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestSaveState();
    }
    else if (shortcut == Key_LoadState)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestLoadState();
    }
    else if (shortcut == Key_NextStateSlot)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT ++;
            if ( NES::STATE_SLOT>=10)
                NES::STATE_SLOT=0;
            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification(("State slot set to "+Convert::T_to_string<int>(NES::STATE_SLOT)).c_str(),120,0x00FF00);
        }
    }
    else if (shortcut == Key_PreviousStateSlot)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT --;
            if ( NES::STATE_SLOT<0)
                NES::STATE_SLOT=9;
            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification(("State slot set to "+Convert::T_to_string<int>(NES::STATE_SLOT)).c_str(),120,0x00FF00);
        }
    }
    else if (shortcut == Key_StateSlot0)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 0;

            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification("State slot set to 0.",120,0x00FF00);
        }
    }
    else if (shortcut == Key_StateSlot1)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 1;
            VideoOut::WriteNotification("State slot set to 1.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot2)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 2;
            VideoOut::WriteNotification("State slot set to 2.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot3)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 3;
            VideoOut::WriteNotification("State slot set to 3.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot4)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 4;
            VideoOut::WriteNotification("State slot set to 4.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot5)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES:: STATE_SLOT = 5;
            VideoOut::WriteNotification("State slot set to 5.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot6)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 6;
            VideoOut::WriteNotification("State slot set to 6.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot7)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 7;
            VideoOut::WriteNotification("State slot set to 7.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot8)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 8;
            VideoOut::WriteNotification("State slot set to 8.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == Key_StateSlot9)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 9;
            VideoOut::WriteNotification("State slot set to 9.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
}
void CheckJoystickShortcut(const int &shortcut)
{
    // Shortcuts
    if (shortcut == JoyBut_TogglePause)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::PAUSED = !NES::PAUSED;
            if (NES::PAUSED)
                VideoOut::WriteStatus("PAUSED !",-1,0x00FF00,false);
            else
                VideoOut::WriteStatus("Running .. !",10,0x00FF00,false);
        }
    }
    else if (shortcut == JoyBut_ShowInfo)
    {
        ShowSystemInfo();
    }
    else if (shortcut == JoyBut_HardReset)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestHardReset();
    }
    else if (shortcut == JoyBut_SoftReset)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES:: RequestSoftReset();
    }
    else if (shortcut == JoyBut_SaveState)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestSaveState();
    }
    else if (shortcut == JoyBut_LoadState)
    {
        if (SystemID == SYSTEM_ID_NES)
            NES::RequestLoadState();
    }
    else if (shortcut == JoyBut_NextStateSlot)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT ++;
            if ( NES::STATE_SLOT>=10)
                NES::STATE_SLOT=0;
            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification(("State slot set to "+Convert::T_to_string<int>(NES::STATE_SLOT)).c_str(),120,0x00FF00);
        }
    }
    else if (shortcut == JoyBut_PreviousStateSlot)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT --;
            if ( NES::STATE_SLOT<0)
                NES::STATE_SLOT=9;
            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification(("State slot set to "+Convert::T_to_string<int>(NES::STATE_SLOT)).c_str(),120,0x00FF00);
        }
    }
    else if (shortcut == JoyBut_StateSlot0)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 0;

            NES::ShowStateSlotDetails();

            VideoOut::WriteNotification("State slot set to 0.",120,0x00FF00);
        }
    }
    else if (shortcut == JoyBut_StateSlot1)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 1;
            VideoOut::WriteNotification("State slot set to 1.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot2)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 2;
            VideoOut::WriteNotification("State slot set to 2.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot3)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 3;
            VideoOut::WriteNotification("State slot set to 3.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot4)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 4;
            VideoOut::WriteNotification("State slot set to 4.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot5)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES:: STATE_SLOT = 5;
            VideoOut::WriteNotification("State slot set to 5.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot6)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 6;
            VideoOut::WriteNotification("State slot set to 6.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot7)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 7;
            VideoOut::WriteNotification("State slot set to 7.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot8)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 8;
            VideoOut::WriteNotification("State slot set to 8.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
    else if (shortcut == JoyBut_StateSlot9)
    {
        if (SystemID == SYSTEM_ID_NES)
        {
            NES::STATE_SLOT = 9;
            VideoOut::WriteNotification("State slot set to 9.",120,0x00FF00);
            NES::ShowStateSlotDetails();
        }
    }
}
void CheckMouseClicks(const int &button)
{
    if (SystemID == SYSTEM_ID_NES)
    {
        NES::OnMouseClick(button);
    }
}
void CheckMouseUps(const int &button)
{
    if (SystemID == SYSTEM_ID_NES)
    {
        NES::OnMouseUp(button);
    }
}
void ExecuteCommands(int argc,char* args[])
{
    VideoOut::ExecuteCommands(argc,args);
    AudioOut::ExecuteCommands(argc,args);
    SDLInput::ExecuteCommands(argc,args);
    NES::ExecuteCommands(argc,args);
    for (int i = 2; i<argc; i++)
    {
        if ((std::string) args[i] == "-sound_enable")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("sound_enable",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-sound_freq")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("sound_freq", Convert::string_to_T<uint32_t>((std::string)args[i]));
        }
    }
}
}

