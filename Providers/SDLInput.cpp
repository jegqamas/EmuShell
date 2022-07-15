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

#include "SDLInput.h"
#include <iostream>
#include "../Common/ConfigHandler.h"
#include <SDL2/SDL.h>

namespace SDLInput
{
const Uint8* currentKeyStates;
int JOYSTICKS_NUMBER;
void Initialize()
{
    std::cout << "SDL: Initializing joysticks ...\n";
    // Initialize joysticks
    SDL_Init(SDL_INIT_JOYSTICK);
    JOYSTICKS_NUMBER = SDL_NumJoysticks();
    std::cout << "SDL: Joysticks count = "<<Convert::to_string(JOYSTICKS_NUMBER)<<"\n";
    std::cout << "SDL: Joysticks initialized successfully.\n";
}
void Update()
{
    if (JOYSTICKS_NUMBER>0)
        SDL_JoystickUpdate();
    currentKeyStates = SDL_GetKeyboardState(NULL);
}
bool IsKeyPressed(int key)
{
    if (currentKeyStates==NULL)
        return false;
    return currentKeyStates[SDL_GetScancodeFromKey(key)];
}
void ShutDown()
{

}
void ExecuteCommands(int argc,char* args[])
{
}
}
