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

#include <string>

namespace EmuShell
{
// Current loaded emu system
extern std::string SYSTEM;
// Current loaded system id
extern unsigned int SystemID;
// Current loaded file
extern std::string FileName;

// Current loaded file's CRC32 according to system;
// For Nes for example, the file CRC calculated without header
extern unsigned int GameCRC32;

extern void SetupEmu(std::string filename);
extern void SetupNES(std::string filename);
extern void ExecuteCommands(int argc, char *args[]);
extern void Initialize(int argc, char *args[]);
extern void CheckSystemShortcut(const int &shortcut);
extern void CheckJoystickShortcut(const int &shortcut);
extern void CheckMouseClicks(const int &button);
extern void CheckMouseUps(const int &button);
}
