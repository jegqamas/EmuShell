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
#include <stdint.h>
#include <SDL2/SDL.h>

namespace NES
{
class IJoypadConnecter
{
public:
    int PlayerIndex;

    IJoypadConnecter(int playerIndex);
    // Nes emulation engine call this at frame end, can be used to update input device status.
    virtual void Update();
    // Nes emulation engine call this at $4016 writes.
    virtual uint8_t GetData();

    virtual void LoadKeys();
    virtual void SaveKeys();
    int KeyUp;
    int KeyDown;
    int KeyLeft;
    int KeyRight;
    int KeyStart;
    int KeySelect;
    int KeyA;
    int KeyB;
    int KeyTurboA;
    int KeyTurboB;

protected:
    uint8_t DATA;
    bool Turbo;
};

class JoypadJoystickConnecter: public IJoypadConnecter
{
public:
    SDL_Joystick* joystick;
    JoypadJoystickConnecter(int playerIndex, int joystickIndex);
    // Nes emulation engine call this at frame end, can be used to update input device status.
    void Update();
    void LoadKeys();
    void SaveKeys();
private :
    void CheckDirections();
};
}
