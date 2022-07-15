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
#include "IJoypadConnecter.h"
#include "../Providers/SDLInput.h"
#include "../Common/ConfigHandler.h"
#include <iostream>

namespace NES
{
JoypadJoystickConnecter::JoypadJoystickConnecter(int playerIndex, int joystickIndex):
    IJoypadConnecter(PlayerIndex)
{
    this->PlayerIndex = playerIndex;
    std::cout << "SDL: Opening joystick for player "<< Convert::to_string((playerIndex+1))<<", joystick index = "
              <<Convert::to_string(joystickIndex)<<"\n";
    // Get the index of current player
    joystick = SDL_JoystickOpen(joystickIndex);
    if (joystick!=NULL)
    {
        std::cout << "SDL: Joystick name = " << SDL_JoystickName(joystick)<<"\n";
        std::cout << "SDL: Joystick buttons number = " << Convert::to_string(SDL_JoystickNumButtons(joystick))<<"\n";
    }
}

void JoypadJoystickConnecter::Update()
{
    Turbo = !Turbo;
    DATA = 0;

    if (SDL_JoystickGetButton(joystick,KeyA))
        DATA |= 1;

    if (SDL_JoystickGetButton(joystick,KeyB))
        DATA |= 2;

    if (Turbo)
    {
        if (SDL_JoystickGetButton(joystick,KeyTurboA) )
            DATA |= 1;

        if (SDL_JoystickGetButton(joystick,KeyTurboB))
            DATA |= 2;
    }

    if (SDL_JoystickGetButton(joystick,KeySelect))
        DATA |= 4;

    if (SDL_JoystickGetButton(joystick,KeyStart))
        DATA |= 8;

    CheckDirections();
}
void JoypadJoystickConnecter::CheckDirections()
{
    if (SDL_JoystickGetAxis(joystick,1)<=-8000)
        DATA |= 0x10;// Up

    if (SDL_JoystickGetAxis(joystick,1)>=8000)
        DATA |= 0x20;// Down

    if (SDL_JoystickGetAxis(joystick,0)<=-8000)
        DATA |= 0x40;// Left

    if (SDL_JoystickGetAxis(joystick,0)>=8000)
        DATA |= 0x80;// Right
}
void JoypadJoystickConnecter::LoadKeys()
{
    switch (this->PlayerIndex)
    {
    case 0:
    {
        KeyStart = CONFIG::GetInt32Value("nes.joystick.pl1.start",9);
        KeySelect = CONFIG::GetInt32Value("nes.joystick.pl1.select",8);
        KeyA = CONFIG::GetInt32Value("nes.joystick.pl1.a",2);
        KeyB = CONFIG::GetInt32Value("nes.joystick.pl1.b",3);
        KeyTurboA = CONFIG::GetInt32Value("nes.joystick.pl1.turboa",1);
        KeyTurboB = CONFIG::GetInt32Value("nes.joystick.pl1.tubob",0);
        break;
    }
    case 1:
    {
        KeyStart = CONFIG::GetInt32Value("nes.joystick.pl2.start",-1);
        KeySelect = CONFIG::GetInt32Value("nes.joystick.pl2.select",-1);
        KeyA = CONFIG::GetInt32Value("nes.joystick.pl2.a",-1);
        KeyB = CONFIG::GetInt32Value("nes.joystick.pl2.b",-1);
        KeyTurboA = CONFIG::GetInt32Value("nes.joystick.pl2.turboa",-1);
        KeyTurboB = CONFIG::GetInt32Value("nes.joystick.pl2.tubob",-1);
        break;
    }
    case 2:
    {
        KeyStart = CONFIG::GetInt32Value("nes.joystick.pl3.start",-1);
        KeySelect = CONFIG::GetInt32Value("nes.joystick.pl3.select",-1);
        KeyA = CONFIG::GetInt32Value("nes.joystick.pl3.a",-1);
        KeyB = CONFIG::GetInt32Value("nes.joystick.pl3.b",-1);
        KeyTurboA = CONFIG::GetInt32Value("nes.joystick.pl3.turboa",-1);
        KeyTurboB = CONFIG::GetInt32Value("nes.joystick.pl3.tubob",-1);
        break;
    }
    case 3:
    {
        KeyStart = CONFIG::GetInt32Value("nes.joystick.pl4.start",-1);
        KeySelect = CONFIG::GetInt32Value("nes.joystick.pl4.select",-1);
        KeyA = CONFIG::GetInt32Value("nes.joystick.pl4.a",-1);
        KeyB = CONFIG::GetInt32Value("nes.joystick.pl4.b",-1);
        KeyTurboA = CONFIG::GetInt32Value("nes.joystick.pl4.turboa",-1);
        KeyTurboB = CONFIG::GetInt32Value("nes.joystick.pl4.tubob",-1);
        break;
    }
    }
}
void JoypadJoystickConnecter::SaveKeys()
{
    switch (this->PlayerIndex)
    {
    case 0:
    {
        CONFIG::SetValue("nes.joystick.pl1.start",KeyStart);
        CONFIG::SetValue("nes.joystick.pl1.select",KeySelect);
        CONFIG::SetValue("nes.joystick.pl1.a",KeyA);
        CONFIG::SetValue("nes.joystick.pl1.b",KeyB);
        CONFIG::SetValue("nes.joystick.pl1.turboa",KeyTurboA);
        CONFIG::SetValue("nes.joystick.pl1.tubob",KeyTurboB);
        break;
    }
    case 1:
    {
        CONFIG::SetValue("nes.joystick.pl2.start",KeyStart);
        CONFIG::SetValue("nes.joystick.pl2.select",KeySelect);
        CONFIG::SetValue("nes.joystick.pl2.a",KeyA);
        CONFIG::SetValue("nes.joystick.pl2.b",KeyB);
        CONFIG::SetValue("nes.joystick.pl2.turboa",KeyTurboA);
        CONFIG::SetValue("nes.joystick.pl2.tubob",KeyTurboB);
        break;
    }
    case 2:
    {
        CONFIG::SetValue("nes.joystick.pl3.start",KeyStart);
        CONFIG::SetValue("nes.joystick.pl3.select",KeySelect);
        CONFIG::SetValue("nes.joystick.pl3.a",KeyA);
        CONFIG::SetValue("nes.joystick.pl3.b",KeyB);
        CONFIG::SetValue("nes.joystick.pl3.turboa",KeyTurboA);
        CONFIG::SetValue("nes.joystick.pl3.tubob",KeyTurboB);
        break;
    }
    case 3:
    {
        CONFIG::SetValue("nes.joystick.pl4.start",KeyStart);
        CONFIG::SetValue("nes.joystick.pl4.select",KeySelect);
        CONFIG::SetValue("nes.joystick.pl4.a",KeyA);
        CONFIG::SetValue("nes.joystick.pl4.b",KeyB);
        CONFIG::SetValue("nes.joystick.pl4.turboa",KeyTurboA);
        CONFIG::SetValue("nes.joystick.pl4.tubob",KeyTurboB);
        break;
    }
    }

}
}
