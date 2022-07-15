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

namespace SDLInput
{
extern int JOYSTICKS_NUMBER;
// Execute related command-lines for sdl input
// argc: the arguments count
// args: the arguments as string arrays.
void ExecuteCommands(int argc,char* args[]);
extern void Initialize();
extern void Update();// Update status
extern bool IsKeyPressed(int key);
extern void ShutDown();
}
