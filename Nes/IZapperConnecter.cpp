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
#include "Nes.h"
#include "IZapperConnecter.h"
#include "../Providers/VideoOut.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "../Core.h"

namespace NES
{
IZapperConnecter::IZapperConnecter()
{
    // By default, set the timer to 3
    // For Wild Gunman, we need to set the timer to a larger value
    // TODO: Nes Zapper issue with Wild Gunman Cang Mode
    if (EmuShell::GameCRC32 == 0x5112DC21 || EmuShell::GameCRC32 == 0xB8B9ACA3)
    {
        std::cout <<"WILD GUNMAN FIX !!\n";
        timer_reload=7;
    }
    else
        timer_reload=3;
    oldTrigger=false;
}
void IZapperConnecter::Update()
{
    if (timer > 0)
    {
        timer--;
        if (State)
            return;// State already detected

// Check a square of 30x30 pixels !
        for (int x = -15; x < 15; x++)
        {
            for (int y = -15; y < 15; y++)
            {
                if (scaled_x + x < 256 && scaled_y + x >= 0 && scaled_y + y < render_h && scaled_y + y >= 0)
                {
                    VideoOut::GetPixel(scaled_x + x, scaled_y + y, &color);
                    // analyze the color
                    uint8_t r = (color >> 0x10); // R
                    uint8_t g = (color >> 0x08); // G
                    uint8_t b = (color >> 0x00);  // B
                    State = (r > 85 && g > 85 && b > 85);//bright color ?
                }
                if (State)
                    break;
            }
            if (State)
                break;
        }
    }
    else
        State = false;
}
void IZapperConnecter::OnmouseButtonDownUp(bool click)
{
    // Calling this method means the mouse is clicked ...
    oldTrigger = Trigger;
    Trigger = click;

    if (click)
    {
        // Get SDL window bounds
        VideoOut::GetWindowSize(&window_w, &window_h);

        VideoOut::GetRenderRectangle(&render_x, &render_y, &render_w, &render_h);

// Get mouse position
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // Calculate the scaled positions

        scaled_x = (mouse_x * (render_w-render_y))/window_w;
        scaled_y = (mouse_y * (render_h-render_x))/window_h;

        // Make a quick check ..
        // Check a square of 30x30 pixels !
        for (int x = -15; x < 15; x++)
        {
            for (int y = -15; y < 15; y++)
            {
                if (scaled_x + x < 256 && scaled_y + x >= 0 && scaled_y + y < render_h && scaled_y + y >= 0)
                {
                    VideoOut::GetPixel(scaled_x + x, scaled_y + y, &color);
                    // analyze the color
                    uint8_t r = (color >> 0x10); // R
                    uint8_t g = (color >> 0x08); // G
                    uint8_t b = (color >> 0x00);  // B
                    State = (r > 85 && g > 85 && b > 85);//bright color ?
                }
                if (State)
                    break;
            }
            if (State)
                break;
        }

    }
    if (!Trigger && oldTrigger)
        timer = timer_reload;
}
uint8_t IZapperConnecter::GetData()
{
    return (uint8_t)((Trigger ? 0x10 : 0x0) | (State ? 0x8 : 0x0));
}

}
