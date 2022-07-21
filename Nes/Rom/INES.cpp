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
#include "INES.h"
#include "../Board.h"
#include <iostream>
#include <fstream>

#define MIRRORING_Horz 0x50
#define MIRRORING_Vert 0x44
#define MIRRORING_OneScA 0x00
#define MIRRORING_OneScB 0x55
#define MIRRORING_Full 0xE4

using namespace std;

namespace NES
{
namespace INES
{
char* prg_dump;// PRG banks dump
char* chr_dump;// CHR banks dump
char* trainer_dump;// Trainer dump (if presented)

bool IsValid;// Indicates if this rom is loaded and valid
bool HasBattery;// Indicates if this rom is battery-packed
bool IsPlaychoice10;// Indicates if this rom is battery-packed
bool IsVSUnisystem;// Indicates if this rom is VS-Unisystem
bool HasTrainer;// Indicates if this rom has trainer

int PRGCount;// PRG pages count
int PRGSizeInBytes;// PRG bank size in bytes
int CHRCount;// CHR pages count
int CHRSizeInBytes;// CHR bank size in bytes
int TrainerSizeInBytes; // The trainer dump size in bytes.
int MapperNumber;// INES mapper number
int Mirroring;// Default mirroring

void Load(std::string fileName)
{
    printf("NES: loading INES file ...\n");
    std::cout << "NES: reading file: " << fileName << "\n";
    ifstream fileStr(fileName.c_str(), ios::in | ios::binary);
    if (fileStr.is_open())
    {
        printf("NES: reading header ...\n");
        fileStr.seekg(0, ios::end);
        long fileSize = fileStr.tellg();
        fileStr.seekg(0, ios::beg);
        if (fileSize<16)
        {
            fileStr.close();
            IsValid = false;
            printf("NES: ERROR: This file is smaller than 16, does not contain a header !?\n");
            return;
        }
        char * header = new char[16];
        fileStr.read(header, 16);

        if (header[0] != 'N' ||
                header[1] != 'E' ||
                header[2] != 'S' ||
                header[3] != 0x1A)
        {
            fileStr.close();
            IsValid = false;
            printf("NES: ERROR: This file is not INES !\n");
            return;
        }

        PRGCount = header[4];
        CHRCount = header[5];

        switch (header[6] & 0x9)
        {
        case 0x0:
            Mirroring = MIRRORING_Horz;
            break;
        case 0x1:
            Mirroring = MIRRORING_Vert;
            break;
        case 0x8:
        case 0x9:
            Mirroring = MIRRORING_Full;
            break;
        }

        HasBattery = (header[6] & 0x2) != 0x0;
        HasTrainer = (header[6] & 0x4) != 0x0;

        if ((header[7] & 0x0F) == 0)
            MapperNumber = ((header[7] & 0xF0) | ((header[6] >> 4) & 0xF));
        else
            MapperNumber = ((header[6] >> 4) & 0xF);

        IsVSUnisystem = (header[7] & 0x01) != 0;
        IsPlaychoice10 = (header[7] & 0x02) != 0;

        delete[]header;// No longer needed ...
        // Read dumps
        fileStr.seekg(16, ios::beg);

        if (HasTrainer)
        {
            trainer_dump = new char[512];
            TrainerSizeInBytes=512;
            fileStr.read(trainer_dump, 512);
        }
        else
        {
            trainer_dump = 0;
            TrainerSizeInBytes=0;
        }

        prg_dump = new char[PRGSizeInBytes = (PRGCount * 0x4000)];
        fileStr.read(prg_dump, PRGSizeInBytes);

        if (CHRCount > 0)
        {
            chr_dump = new char[CHRSizeInBytes = (CHRCount * 0x2000)];
            fileStr.read(chr_dump, CHRSizeInBytes);
        }
        else
        {
            chr_dump = 0;
            CHRSizeInBytes = 0;
        }
        fileStr.close();

        IsValid = true;
        printf("NES: header and dumps read success !!\n");
    }
    else
    {
        IsValid = false;
        printf("NES: ERROR READING FILE !!\n");
    }
}
void Close()
{
    delete[] prg_dump;
    delete[] chr_dump;
    delete[] trainer_dump;
}
}
}
