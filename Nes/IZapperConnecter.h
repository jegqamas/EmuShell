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
#include <stdint.h>

namespace NES
{
class IZapperConnecter
{
public:
    IZapperConnecter();
    // Nes emulation engine call this at frame end, can be used to update input device status.
    virtual void OnmouseButtonDownUp(bool click);
    virtual void Update();
    // Nes emulation engine call this at $4016 writes.
    virtual uint8_t GetData();

protected:
    int timer_reload;
    bool Trigger;
    bool oldTrigger;
    bool State;
    int timer;
    int window_h;
    int window_w;
    int render_x;
    int render_y;
    int render_w;
    int render_h;
    int mouse_x;
    int mouse_y;
    int scaled_x;
    int scaled_y;
    int color;
};
}
