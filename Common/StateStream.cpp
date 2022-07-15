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

#include "StateStream.h"
#include "ConfigHandler.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

namespace EmuShell
{
StateStream::~StateStream()
{
}
const uint8_t StateStream::header_chars[4] = { 'e', 's', 's', 's' };
StateStream::StateStream(const uint32_t &crc)
{
    header_chars_count = 4;
    game_crc=crc;
    this->Clear();
}
void StateStream::WriteToFile(std::string fileName)
{
    std::cout << "STATE: writing file: " << fileName << "\n";
    ofstream fileStr(fileName.c_str(), ios::out | ios::binary);
    if (fileStr.is_open())
    {
        fileStr.write((const char*)&data_buffer[0], data_buffer.size());

        fileStr.flush();
        fileStr.close();

        std::cout << "STATE: state file write completed successfully.\n";
    }
    else
    {
        std::cout << "STATE: error; can't save to file (file could not open)\n";
    }
}
bool StateStream::ReadFromFile(std::string fileName)
{
    if (!Path::IsFileExist(fileName.c_str()))
    {
        std::cout << "STATE: error; the file is not exist.\n";
        return false;
    }
// 1 create the stream
    std::cout << "STATE: reading file: " << fileName << "\n";
    ifstream fileStr(fileName.c_str(), ios::binary | ios::ate);
    if (fileStr.is_open())
    {
        std::cout << "STATE: file opened, checking header ...\n";
        position=0;
        fileStr.seekg (0, ios::end);
        long file_size = fileStr.tellg();
        std::cout <<"STATE: File size = "<< Convert::to_string(file_size)<< "\n";
        if (file_size < header_chars_count)
        {
            std::cout << "STATE: error; this file is not a state file (size is too small).\n";
            return false;
        }
        // Seek into the beginning
        fileStr.seekg (0, ios::beg);

        // Read the header
        char header[header_chars_count];
        fileStr.read(header, header_chars_count);
        if (header[0]!='e' ||
                header[1]!='s' ||
                header[2]!='s' ||
                header[3]!='s' )
        {
            std::cout << "STATE: error; this file is not a state file (header check error)\n";
            return false;
        }
        position+=header_chars_count;

        // Read the system name
        // Read the size
        uint32_t read_crc = 0;
        char sys_size_buff [4];
        fileStr.read(sys_size_buff, 4);
        position+=4;

        read_crc = (read_crc & 0xFFFFFF00) | sys_size_buff[0];
        read_crc = (read_crc & 0xFFFF00FF) | (sys_size_buff[1]<<8);
        read_crc = (read_crc & 0xFF00FFFF) | (sys_size_buff[2]<<16);
        read_crc = (read_crc & 0x00FFFFFF) | (sys_size_buff[3]<<24);
        printf("STATE: STATE FILE CRC =: 0x%X\n",read_crc);
        if (read_crc != game_crc)
        {
            std::cout << "STATE: error; this file is not for this game (crc check error)\n";
            printf("STATE: ->STATE FILE CRC =: 0x%X\n",read_crc);
            printf("STATE: ->GAME FILE CRC =: 0x%X\n",game_crc);
            return false;
        }

// Read out the 4 zeros
        fileStr.read(sys_size_buff, 4);
        position+=4;

        // Read all the file and put it into the buffer
        char dump_buff[file_size - position];
        fileStr.read(dump_buff, file_size - position);
        fileStr.close();

        // Start a new, add header and names
        Clear();
        // Put state data into the buffer
        for (int i=0; i<file_size - position; i++)
            this->data_buffer.push_back(dump_buff[i]);


        std::cout << "STATE: state file read completed successfully.\n";
        return true;
    }
    else
    {
        std::cout << "STATE: error; this file is not a state file (file could not open)\n";
        return false;
    }
}
void StateStream::Clear()
{
// Clear the buffer
    this->data_buffer.clear();
    position=0;

// Write the identifier
    Write(header_chars,header_chars_count);

    // Write crc
    Write(game_crc);

    // Ready !!, Write 4 zeros
    Write((uint32_t)0);
}
void StateStream::Write(const uint8_t &value)
{
    this->data_buffer.push_back(value);
    position ++;
}
void StateStream::Write(const uint8_t* value, int num)
{
    for (int i = 0; i<num ; i++)
    {
        Write(value[i]);
    }
}

void StateStream::Write(const uint16_t &value)
{
    this->data_buffer.push_back(value & 0x00FF);
    this->data_buffer.push_back((value & 0xFF00)>>8);
    position +=2;
}
void StateStream::Write(uint16_t* &value, int num)
{
    for (int i = 0; i<num ; i++)
        Write(value[i]);
}

void StateStream::Write(const uint32_t &value)
{
    this->data_buffer.push_back(value & 0x000000FF);
    this->data_buffer.push_back((value & 0x0000FF00)>>8);
    this->data_buffer.push_back((value & 0x00FF0000)>>16);
    this->data_buffer.push_back((value & 0xFF000000)>>24);
    position +=4;
}
void StateStream::Write(uint32_t* &value, int num)
{
    for (int i = 0; i<num ; i++)
        Write(value[i]);
}

void StateStream::Write(const bool &value)
{
    this->data_buffer.push_back(value ?1:0);
    position ++;
}
void StateStream::Write(bool* &value, int num)
{
    for (int i = 0; i<num ; i++)
        Write(value[i]);
}

uint8_t StateStream::ReadUint08()
{
    uint8_t val = data_buffer[position];
    position ++;
    return val;
}
uint16_t StateStream::ReadUint16()
{
    uint16_t val =0;

    val = (val & 0xFF00) | data_buffer[position];
    position++;

    val = (val & 0x00FF) | (data_buffer[position]<<8);
    position++;

    return val;
}

uint32_t StateStream::ReadUint32()
{
    uint32_t val =0;

    val = (val & 0xFFFFFF00) | data_buffer[position];
    position++;

    val = (val & 0xFFFF00FF) | (data_buffer[position]<<8);
    position++;

    val = (val & 0xFF00FFFF) | (data_buffer[position]<<16);
    position++;

    val = (val & 0x00FFFFFF) | (data_buffer[position]<<24);
    position++;

    return val;
}
bool StateStream::ReadBoolean()
{
    uint8_t val =data_buffer[position];
    position++;

    return val == 1;
}
void StateStream::Read(uint8_t &value)
{
    value = ReadUint08();
}
void StateStream::Read(uint8_t* &value, int num)
{
    for (int i = 0; i<num ; i++)
        value[i] = ReadUint08();
}
void StateStream::Read(uint16_t &value)
{
    value = ReadUint16();
}
void StateStream::Read(uint16_t* &value, int num)
{
    for (int i = 0; i<num ; i++)
        value[i] = ReadUint16();
}
void StateStream::Read(uint32_t &value)
{
    value = ReadUint32();
}
void StateStream::Read(uint32_t* &value, int num)
{
    for (int i = 0; i<num ; i++)
        value[i] = ReadUint32();
}

void StateStream::Read(bool &value)
{
    value = ReadBoolean();
}
void StateStream::Read(bool* &value, int num)
{
    for (int i = 0; i<num ; i++)
        value[i] = ReadBoolean();
}
}
