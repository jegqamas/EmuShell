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

#include <stdint.h>

#define EMPROM_DEVICE_X24C01 0
#define EMPROM_DEVICE_X24C02 1

#define EMPROM_MODE_Data 0
#define EMPROM_MODE_Addressing 1
#define EMPROM_MODE_Idle 2
#define EMPROM_MODE_Read 3
#define EMPROM_MODE_Write 4
#define EMPROM_MODE_Ack 5
#define EMPROM_MODE_NotAck 6
#define EMPROM_MODE_AckWait 7

namespace NES
{
class Eprom
{
public:
    Eprom(uint32_t memorySize);
    ~Eprom();
    void HardReset();
    void Write(uint16_t& address, uint8_t& value);
    uint8_t Read(int address);
    void SaveState();
    void LoadState();
private:
    uint32_t mem_size ;
    uint8_t* data;
    uint8_t mode;
    uint8_t nextmode;
    uint8_t device ;
    bool psda;
    bool pscl;
    uint32_t output ;
    uint32_t cbit ;
    uint32_t caddress;
    uint32_t cdata ;
    bool isRead;
    bool cSCL;
    bool cSDA;

    void Start();
    void Stop();
    void RiseX24C01(int bit);
    void RiseX24C02(int bit);
    void FallX24C01();
    void FallX24C02();
};
}
