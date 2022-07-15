// This file is part of EmuShell
// A multi video game consoles emulator.
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
#include <iostream>
#include <fstream>
#include "../../Common/ConfigHandler.h"

namespace NES
{
namespace PaletteFile
{
void SavePaletteFile(const char* filepath, uint32_t* palette)
{
    std::ofstream fileStr(filepath, std::ios::out | std::ios::binary);
    for (int i = 0; i < 512; i++)
    {
        uint32_t color = palette[i];
        fileStr << (uint8_t)((color >> 16) & 0xFF);//Red
        fileStr << (uint8_t)((color >> 8) & 0xFF);//Green
        fileStr << (uint8_t)(color & 0xFF);//Blue
    }
    fileStr.close();
}
bool LoadPaletteFile(std::string filepath,uint32_t* palette_to_load_in)
{
    std::ifstream fileStr(filepath.c_str(), std::ios::in | std::ios::binary);
    fileStr.seekg(0, std::ios::end);
    long fileSize = fileStr.tellg();

    if (fileSize == 192 || fileSize == 512 * 3)
    {
        fileStr.seekg(0, std::ios::beg);

        char* buffer = new char[fileSize];
        fileStr.read(buffer,fileSize);
        fileStr.close();

        int j = 0;
        for (int i = 0; i < 512; i++)
        {
            uint8_t RedValue = (uint8_t)buffer[j];
            j++;
            if (j == fileSize)
                j = 0;
            uint8_t GreenValue = (uint8_t)buffer[j];
            j++;
            if (j == fileSize)
                j = 0;
            uint8_t BlueValue = (uint8_t)buffer[j];
            j++;
            if (j == fileSize)
                j = 0;
            palette_to_load_in[i] = (uint32_t)((0xFF << 24) | (RedValue << 16) |
                                                   (GreenValue << 8) | BlueValue);
        }
        return true;
    }
    fileStr.close();
    return false;
}
}
}

