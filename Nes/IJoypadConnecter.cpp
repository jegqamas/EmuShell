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
#include "IJoypadConnecter.h"
#include "../Providers/SDLInput.h"
#include "../Common/ConfigHandler.h"

namespace NES
{
IJoypadConnecter::IJoypadConnecter(int playerIndex)
{
    this->PlayerIndex = playerIndex;
}
// Nes emulation engine call this at frame end, can be used to update input device status.
void IJoypadConnecter::Update()
{
    Turbo = !Turbo;

    DATA = 0;

    if (SDLInput::IsKeyPressed(KeyA))
        DATA |= 1;

    if (SDLInput::IsKeyPressed(KeyB))
        DATA |= 2;

    if (SDLInput::IsKeyPressed(KeyTurboA) && Turbo)
        DATA |= 1;

    if (SDLInput::IsKeyPressed(KeyTurboB) && Turbo)
        DATA |= 2;

    if (SDLInput::IsKeyPressed(KeySelect))
        DATA |= 4;

    if (SDLInput::IsKeyPressed(KeyStart))
        DATA |= 8;

    if (SDLInput::IsKeyPressed(KeyUp))
        DATA |= 0x10;

    if (SDLInput::IsKeyPressed(KeyDown))
        DATA |= 0x20;

    if (SDLInput::IsKeyPressed(KeyLeft))
        DATA |= 0x40;

    if (SDLInput::IsKeyPressed(KeyRight))
        DATA |= 0x80;
}
unsigned char IJoypadConnecter::GetData()
{
    return DATA;
}
void IJoypadConnecter::LoadKeys()
{
    switch (this->PlayerIndex)
    {
    case 0:
    {
        KeyUp =  SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.up","Up").c_str());
        KeyDown = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.down","Down").c_str());
        KeyLeft = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.left","Left").c_str());
        KeyRight = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.right","Right").c_str());
        KeyStart =SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.start","V").c_str());
        KeySelect = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.select","C").c_str());
        KeyA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.a","X").c_str());
        KeyB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.b","Z").c_str());
        KeyTurboA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.turboa","S").c_str());
        KeyTurboB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl1.tubob","A").c_str());
        break;
    }
    case 1:
    {
        KeyUp =  SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.up","R").c_str());
        KeyDown = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.down","F").c_str());
        KeyLeft = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.left","D").c_str());
        KeyRight = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.right","G").c_str());
        KeyStart =SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.start","N").c_str());
        KeySelect = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.select","B").c_str());
        KeyA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.a","K").c_str());
        KeyB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.b","L").c_str());
        KeyTurboA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.turboa","I").c_str());
        KeyTurboB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl2.tubob","O").c_str());
        break;
    }
    case 2:
    {
        KeyUp =  SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.up","Unknown").c_str());
        KeyDown = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.down","Unknown").c_str());
        KeyLeft = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.left","Unknown").c_str());
        KeyRight = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.right","Unknown").c_str());
        KeyStart =SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.start","Unknown").c_str());
        KeySelect = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.select","Unknown").c_str());
        KeyA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.a","Unknown").c_str());
        KeyB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.b","Unknown").c_str());
        KeyTurboA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.turboa","Unknown").c_str());
        KeyTurboB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl3.tubob","Unknown").c_str());
        break;
    }
    case 3:
    {
        KeyUp =  SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.up","Unknown").c_str());
        KeyDown = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.down","Unknown").c_str());
        KeyLeft = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.left","Unknown").c_str());
        KeyRight = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.right","Unknown").c_str());
        KeyStart =SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.start","Unknown").c_str());
        KeySelect = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.select","Unknown").c_str());
        KeyA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.a","Unknown").c_str());
        KeyB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.b","Unknown").c_str());
        KeyTurboA = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.turboa","Unknown").c_str());
        KeyTurboB = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.pl4.tubob","Unknown").c_str());
        break;
    }
    }
}
void IJoypadConnecter::SaveKeys()
{
    switch (this->PlayerIndex)
    {
    case 0:
    {
        CONFIG::SetValue("nes.keyboard.pl1.up",(std::string)SDL_GetKeyName(KeyUp));
        CONFIG::SetValue("nes.keyboard.pl1.down",(std::string)SDL_GetKeyName(KeyDown));
        CONFIG::SetValue("nes.keyboard.pl1.left",(std::string)SDL_GetKeyName(KeyLeft));
        CONFIG::SetValue("nes.keyboard.pl1.right",(std::string)SDL_GetKeyName(KeyRight));
        CONFIG::SetValue("nes.keyboard.pl1.start",(std::string)SDL_GetKeyName(KeyStart));
        CONFIG::SetValue("nes.keyboard.pl1.select",(std::string)SDL_GetKeyName(KeySelect));
        CONFIG::SetValue("nes.keyboard.pl1.a",(std::string)SDL_GetKeyName(KeyA));
        CONFIG::SetValue("nes.keyboard.pl1.b",(std::string)SDL_GetKeyName(KeyB));
        CONFIG::SetValue("nes.keyboard.pl1.turboa",(std::string)SDL_GetKeyName(KeyTurboA));
        CONFIG::SetValue("nes.keyboard.pl1.tubob",(std::string)SDL_GetKeyName(KeyTurboB));
        break;
    }
    case 1:
    {
        CONFIG::SetValue("nes.keyboard.pl2.up",(std::string)SDL_GetKeyName(KeyUp));
        CONFIG::SetValue("nes.keyboard.pl2.down",(std::string)SDL_GetKeyName(KeyDown));
        CONFIG::SetValue("nes.keyboard.pl2.left",(std::string)SDL_GetKeyName(KeyLeft));
        CONFIG::SetValue("nes.keyboard.pl2.right",(std::string)SDL_GetKeyName(KeyRight));
        CONFIG::SetValue("nes.keyboard.pl2.start",(std::string)SDL_GetKeyName(KeyStart));
        CONFIG::SetValue("nes.keyboard.pl2.select",(std::string)SDL_GetKeyName(KeySelect));
        CONFIG::SetValue("nes.keyboard.pl2.a",(std::string)SDL_GetKeyName(KeyA));
        CONFIG::SetValue("nes.keyboard.pl2.b",(std::string)SDL_GetKeyName(KeyB));
        CONFIG::SetValue("nes.keyboard.pl2.turboa",(std::string)SDL_GetKeyName(KeyTurboA));
        CONFIG::SetValue("nes.keyboard.pl2.tubob",(std::string)SDL_GetKeyName(KeyTurboB));
        break;
    }
    case 2:
    {
        CONFIG::SetValue("nes.keyboard.pl3.up",(std::string)SDL_GetKeyName(KeyUp));
        CONFIG::SetValue("nes.keyboard.pl3.down",(std::string)SDL_GetKeyName(KeyDown));
        CONFIG::SetValue("nes.keyboard.pl3.left",(std::string)SDL_GetKeyName(KeyLeft));
        CONFIG::SetValue("nes.keyboard.pl3.right",(std::string)SDL_GetKeyName(KeyRight));
        CONFIG::SetValue("nes.keyboard.pl3.start",(std::string)SDL_GetKeyName(KeyStart));
        CONFIG::SetValue("nes.keyboard.pl3.select",(std::string)SDL_GetKeyName(KeySelect));
        CONFIG::SetValue("nes.keyboard.pl3.a",(std::string)SDL_GetKeyName(KeyA));
        CONFIG::SetValue("nes.keyboard.pl3.b",(std::string)SDL_GetKeyName(KeyB));
        CONFIG::SetValue("nes.keyboard.pl3.turboa",(std::string)SDL_GetKeyName(KeyTurboA));
        CONFIG::SetValue("nes.keyboard.pl3.tubob",(std::string)SDL_GetKeyName(KeyTurboB));
        break;
    }
    case 3:
    {
        CONFIG::SetValue("nes.keyboard.pl4.up",(std::string)SDL_GetKeyName(KeyUp));
        CONFIG::SetValue("nes.keyboard.pl4.down",(std::string)SDL_GetKeyName(KeyDown));
        CONFIG::SetValue("nes.keyboard.pl4.left",(std::string)SDL_GetKeyName(KeyLeft));
        CONFIG::SetValue("nes.keyboard.pl4.right",(std::string)SDL_GetKeyName(KeyRight));
        CONFIG::SetValue("nes.keyboard.pl4.start",(std::string)SDL_GetKeyName(KeyStart));
        CONFIG::SetValue("nes.keyboard.pl4.select",(std::string)SDL_GetKeyName(KeySelect));
        CONFIG::SetValue("nes.keyboard.pl4.a",(std::string)SDL_GetKeyName(KeyA));
        CONFIG::SetValue("nes.keyboard.pl4.b",(std::string)SDL_GetKeyName(KeyB));
        CONFIG::SetValue("nes.keyboard.pl4.turboa",(std::string)SDL_GetKeyName(KeyTurboA));
        CONFIG::SetValue("nes.keyboard.pl4.tubob",(std::string)SDL_GetKeyName(KeyTurboB));
        break;
    }
    }
}
}
