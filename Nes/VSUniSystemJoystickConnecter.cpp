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
#include "IVSUniSystemDIPConnecter.h"
#include "../Providers/SDLInput.h"
#include "../Common/ConfigHandler.h"
#include <iostream>

namespace NES
{
VSUniSystemJoystickConnecter::VSUniSystemJoystickConnecter(int joystickIndex):
    IVSUniSystemConnecter()
{
    std::cout << "SDL: Opening joystick for VS UniSystem DIP, joystick index = "
              <<Convert::to_string(joystickIndex)<<"\n";
    // Get the index of current player
    joystick = SDL_JoystickOpen(joystickIndex);
    if (joystick!=NULL)
    {
        std::cout << "SDL: Joystick name = " << SDL_JoystickName(joystick)<<"\n";
        std::cout << "SDL: Joystick buttons number = " << Convert::to_string(SDL_JoystickNumButtons(joystick))<<"\n";
    }
}
void VSUniSystemJoystickConnecter::Update()
{
    data4016 = 0;

    if (SDL_JoystickGetButton(joystick,CreditServiceButton))
        data4016 |= 0x04;
    if (SDL_JoystickGetButton(joystick,DIPSwitch1))
        data4016 |= 0x08;
    if (SDL_JoystickGetButton(joystick,DIPSwitch2))
        data4016 |= 0x10;
    if (SDL_JoystickGetButton(joystick,CreditLeftCoinSlot))
        leftCoin = true;
    if (leftCoin)
        data4016 |= 0x20;
    if (SDL_JoystickGetButton(joystick,CreditRightCoinSlot))
        rightCoin = true;
    if (rightCoin)
        data4016 |= 0x40;

    data4017 = 0;
    if (SDL_JoystickGetButton(joystick,DIPSwitch3))
        data4017 |= 0x04;
    if (SDL_JoystickGetButton(joystick,DIPSwitch4))
        data4017 |= 0x08;
    if (SDL_JoystickGetButton(joystick,DIPSwitch5))
        data4017 |= 0x10;
    if (SDL_JoystickGetButton(joystick,DIPSwitch6))
        data4017 |= 0x20;
    if (SDL_JoystickGetButton(joystick,DIPSwitch7))
        data4017 |= 0x40;
    if (SDL_JoystickGetButton(joystick,DIPSwitch8))
        data4017 |= 0x80;
}
void VSUniSystemJoystickConnecter::LoadKeys()
{
    CreditServiceButton = CONFIG::GetInt32Value("nes.joystick.vs.credit_service_button",-1);
    DIPSwitch1= CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_1",-1);
    DIPSwitch2= CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_2",-1);
    DIPSwitch3= CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_3",-1);
    DIPSwitch4 = CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_4",-1);
    DIPSwitch5 = CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_5",-1);
    DIPSwitch6 = CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_6",-1);
    DIPSwitch7 = CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_7",-1);
    DIPSwitch8 = CONFIG::GetInt32Value("nes.joystick.vs.dip_switch_8",-1);
    CreditLeftCoinSlot = CONFIG::GetInt32Value("nes.joystick.vs.credit_left_coin_slot",-1);
    CreditRightCoinSlot = CONFIG::GetInt32Value("nes.joystick.vs.credit_right_coin_slot",-1);
}
void VSUniSystemJoystickConnecter::SaveKeys()
{
    CONFIG::SetValue("nes.joystick.vs.credit_service_button",CreditServiceButton);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_1",DIPSwitch1);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_2",DIPSwitch2);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_3",DIPSwitch3);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_4",DIPSwitch4);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_5",DIPSwitch5);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_6",DIPSwitch6);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_7",DIPSwitch7);
    CONFIG::SetValue("nes.joystick.vs.dip_switch_8",DIPSwitch8);
    CONFIG::SetValue("nes.joystick.vs.credit_left_coin_slot",CreditLeftCoinSlot);
    CONFIG::SetValue("nes.joystick.vs.credit_right_coin_slot",CreditRightCoinSlot);
}
}
