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

// CRC32 CALCULATION (rc_crc32 method)
// TAKEN FROM <http://rosettacode.org/wiki/CRC-32>

#include "Crc32.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

namespace CRC32
{
uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len)
{
    static uint32_t table[256];
    static int have_table = 0;
    uint32_t rem;
    uint8_t octet;
    int i, j;
    const char *p, *q;

    /* This check is not thread safe; there is no mutex. */
    if (have_table == 0)
    {
        /* Calculate CRC table. */
        for (i = 0; i < 256; i++)
        {
            rem = i;  /* remainder from polynomial division */
            for (j = 0; j < 8; j++)
            {
                if (rem & 1)
                {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                    rem >>= 1;
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = buf + len;
    for (p = buf; p < q; p++)
    {
        octet = *p;  /* Cast to unsigned octet. */
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}

uint32_t CalculateCRC32(std::string filepath)
{
    return CalculateCRC32(filepath,0);
}
uint32_t CalculateCRC32(std::string filepath, int bytesToSkip)
{
    std::ifstream fs(filepath.c_str(), std::ios::in | std::ios::binary);
    if (!fs.good())
    {
        printf("ERROR CALCULATING CRC32:  unable o read file to calculate CRC32.");
        fs.close();
        return 0;
    }

    uint32_t crc = 0;
    fs.seekg(0,std::ios::end);

    long bufsize =fs.tellg();
    if (bufsize <= bytesToSkip)
    {
        printf("ERROR CALCULATING CRC32: this file is smaller than the requested header size.");
        fs.close();
        return 0;
    }
    fs.seekg(bytesToSkip, std::ios::beg);

    char* buffer = new char[bufsize - bytesToSkip];
    fs.read(buffer, bufsize - bytesToSkip);
    fs.close();

    crc = rc_crc32(crc, buffer, bufsize - bytesToSkip);
    printf("FILE CRC: 0x%X\n",crc);
    delete [] buffer;

    return crc;
}

}

