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
#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>
namespace EmuShell
{
// Class for saving/reading state. This class hold state data as buffer
// that can be used any time to save/load state. Then later, it can be
// saved into file.
class StateStream
{
public:
    ~StateStream();
    long position;
    const static uint8_t header_chars[4];
    int header_chars_count ;
    uint32_t game_crc;

    // Create new state stream
    // crc: game crc
    StateStream(const uint32_t &crc);
    // Write the stream data into a file.
    // fileName: the complete file path
    void WriteToFile(std::string fileName);
    // Read the stream data from file.
    // fileName: the complete file path
    bool ReadFromFile(std::string fileName);
    // Clear the stream and start as new.
    // WARNING: unsaved changes will be lost.
    void Clear();

    void Write(const uint8_t &value);
    void Write(const uint8_t* value, int num);

    void Write(const uint16_t &value);
    void Write(uint16_t* &value, int num);

    void Write(const uint32_t &value);
    void Write(uint32_t* &value, int num);

    void Write(const bool &value);
    void Write(bool* &value, int num);

    uint8_t ReadUint08();
    uint16_t ReadUint16();
    uint32_t ReadUint32();
    bool ReadBoolean();

    void Read(uint8_t &value);
    void Read(uint8_t* &value, int num);

    void Read(uint16_t &value);
    void Read(uint16_t* &value, int num);

    void Read(uint32_t &value);
    void Read(uint32_t* &value, int num);

    void Read(bool &value);
    void Read(bool* &value, int num);
private :
    std::vector<uint8_t> data_buffer;// The data buffer to store things
};
}
