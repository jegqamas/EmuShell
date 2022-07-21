// This file is part of EmuShell
// A multi video game consoles emulator.
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

namespace NES
{
namespace INES
{
// Load rom file
// fileName: the complete rom file path
void Load(std::string fileName);
// Close and free resources
void Close();
extern char* prg_dump;// PRG banks dump
extern char* chr_dump;// CHR banks dump
extern char* trainer_dump;// Trainer dump (if presented)

extern bool IsValid;// Indicates if this rom is loaded and valid
extern bool HasBattery;// Indicates if this rom is battery-packed
extern bool IsPlaychoice10;// Indicates if this rom is battery-packed
extern bool IsVSUnisystem;// Indicates if this rom is VS-Unisystem
extern bool HasTrainer;// Indicates if this rom has trainer

extern int PRGCount;// PRG pages count
extern int PRGSizeInBytes;// PRG bank size in bytes
extern int CHRCount;// CHR pages count
extern int CHRSizeInBytes;// CHR bank size in bytes
extern int TrainerSizeInBytes; // The trainer dump size in bytes.
extern int MapperNumber;// INES mapper number
extern int Mirroring;// Default mirroring
};
}
