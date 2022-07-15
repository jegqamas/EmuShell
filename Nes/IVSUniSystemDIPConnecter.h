
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
class IVSUniSystemConnecter
{
public:
    IVSUniSystemConnecter();
    virtual void Update();
    virtual void Write4020(uint8_t& value);
    // Nes emulation engine call this at $4016 writes.
    virtual uint8_t GetData4016();
    virtual uint8_t GetData4017();

    virtual void LoadKeys();
    virtual void SaveKeys();
protected:
    int CreditServiceButton;
    int DIPSwitch1;
    int DIPSwitch2;
    int DIPSwitch3;
    int DIPSwitch4;
    int DIPSwitch5;
    int DIPSwitch6;
    int DIPSwitch7;
    int DIPSwitch8;
    int CreditLeftCoinSlot;
    bool leftCoin;
    int CreditRightCoinSlot;
    bool rightCoin;
    uint8_t data4016;
    uint8_t data4017;
};
class VSUniSystemJoystickConnecter: public IVSUniSystemConnecter
{
public:
    SDL_Joystick* joystick;
    VSUniSystemJoystickConnecter(int joystickIndex);
    // Nes emulation engine call this at frame end, can be used to update input device status.
    void Update();
    void LoadKeys();
    void SaveKeys();
};
}
