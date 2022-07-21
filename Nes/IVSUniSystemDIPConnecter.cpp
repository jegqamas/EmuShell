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
#include "IVSUniSystemDIPConnecter.h"
#include "../Providers/SDLInput.h"
#include "../Common/ConfigHandler.h"

namespace NES
{
IVSUniSystemConnecter::IVSUniSystemConnecter()
{
}
void  IVSUniSystemConnecter::Update()
{
    data4016 = 0;

    if (SDLInput::IsKeyPressed(CreditServiceButton))
        data4016 |= 0x04;
    if (SDLInput::IsKeyPressed(DIPSwitch1))
        data4016 |= 0x08;
    if (SDLInput::IsKeyPressed(DIPSwitch2))
        data4016 |= 0x10;
    if (SDLInput::IsKeyPressed(CreditLeftCoinSlot))
        leftCoin = true;
    if (leftCoin)
        data4016 |= 0x20;
    if (SDLInput::IsKeyPressed(CreditRightCoinSlot))
        rightCoin = true;
    if (rightCoin)
        data4016 |= 0x40;

    data4017 = 0;
    if (SDLInput::IsKeyPressed(DIPSwitch3))
        data4017 |= 0x04;
    if (SDLInput::IsKeyPressed(DIPSwitch4))
        data4017 |= 0x08;
    if (SDLInput::IsKeyPressed(DIPSwitch5))
        data4017 |= 0x10;
    if (SDLInput::IsKeyPressed(DIPSwitch6))
        data4017 |= 0x20;
    if (SDLInput::IsKeyPressed(DIPSwitch7))
        data4017 |= 0x40;
    if (SDLInput::IsKeyPressed(DIPSwitch8))
        data4017 |= 0x80;
}
void  IVSUniSystemConnecter::Write4020(uint8_t& value)
{
    if ((value & 0x1) == 0x1)
    {
        leftCoin = false;
        rightCoin = false;
    }
}
// Nes emulation engine call this at $4016 writes.
uint8_t IVSUniSystemConnecter::GetData4016()
{
    return data4016;
}
uint8_t IVSUniSystemConnecter::GetData4017()
{
    return data4017;
}
void IVSUniSystemConnecter::LoadKeys()
{
    CreditServiceButton = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.credit_service_button","End").c_str());
    DIPSwitch1= SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_1","NumberPad1").c_str());
    DIPSwitch2= SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_2","NumberPad2").c_str());
    DIPSwitch3= SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_3","NumberPad3").c_str());
    DIPSwitch4 = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_4","NumberPad4").c_str());
    DIPSwitch5 = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_5","NumberPad5").c_str());
    DIPSwitch6 = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_6","NumberPad6").c_str());
    DIPSwitch7 = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_7","NumberPad7").c_str());
    DIPSwitch8 = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.dip_switch_8","NumberPad8").c_str());
    CreditLeftCoinSlot = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.credit_left_coin_slot","Insert").c_str());
    CreditRightCoinSlot = SDL_GetKeyFromName(CONFIG::GetStringValue("nes.keyboard.vs.credit_right_coin_slot","Home").c_str());
}
void IVSUniSystemConnecter::SaveKeys()
{
    CONFIG::SetValue("nes.keyboard.vs.credit_service_button",(std::string)SDL_GetKeyName(CreditServiceButton));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_1",(std::string)SDL_GetKeyName(DIPSwitch1));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_2",(std::string)SDL_GetKeyName(DIPSwitch2));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_3",(std::string)SDL_GetKeyName(DIPSwitch3));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_4",(std::string)SDL_GetKeyName(DIPSwitch4));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_5",(std::string)SDL_GetKeyName(DIPSwitch5));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_6",(std::string)SDL_GetKeyName(DIPSwitch6));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_7",(std::string)SDL_GetKeyName(DIPSwitch7));
    CONFIG::SetValue("nes.keyboard.vs.dip_switch_8",(std::string)SDL_GetKeyName(DIPSwitch8));
    CONFIG::SetValue("nes.keyboard.vs.credit_left_coin_slot",(std::string)SDL_GetKeyName(CreditLeftCoinSlot));
    CONFIG::SetValue("nes.keyboard.vs.credit_right_coin_slot",(std::string)SDL_GetKeyName(CreditRightCoinSlot));
}
}
