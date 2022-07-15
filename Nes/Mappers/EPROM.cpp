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
#include "EPROM.h"
#include "../Nes.h"

namespace NES
{
Eprom::Eprom(uint32_t memorySize)
{
    mem_size = memorySize;
    data = new uint8_t[memorySize];
    device = memorySize == 256 ? EMPROM_DEVICE_X24C02 : EMPROM_DEVICE_X24C01;
    mode = EMPROM_MODE_Data;
    nextmode = EMPROM_MODE_Data;
    output = 0;
    cbit = 0;
    caddress = 0;
    cdata = 0;
}
Eprom::~Eprom()
{
    delete [] data;
}
void Eprom::HardReset()
{
    pscl = false;
    psda = false;
    mode = EMPROM_MODE_Idle;
    nextmode = EMPROM_MODE_Idle;
    cbit = 0;
    caddress = 0;
    cdata = 0;
    isRead = false;
    output = 0x10;
}
void Eprom::Write(uint16_t& address, uint8_t& value)
{
    cSCL = (value & 0x20) == 0x20;
    cSDA = (value & 0x40) == 0x40;
    if (pscl && !cSDA & value)
    {
        Start();
    }
    else if (pscl && cSDA & !psda)
    {
        Stop();
    }
    else if (cSCL & !pscl)
    {
        switch (device)
        {
        case EMPROM_DEVICE_X24C01:
            RiseX24C01((value >> 6) & 1);
            break;
        case EMPROM_DEVICE_X24C02:
            RiseX24C02((value >> 6) & 1);
            break;
        }
    }
    else if (!cSCL & pscl)
    {
        switch (device)
        {
        case EMPROM_DEVICE_X24C01:
            FallX24C01();
            break;
        case EMPROM_DEVICE_X24C02:
            FallX24C02();
            break;
        }
    }

    pscl = cSCL;
    psda = cSDA;
}
uint8_t Eprom::Read(int address)
{
    return (uint8_t)output;
}

void Eprom::Start()
{
    switch (device)
    {
    case EMPROM_DEVICE_X24C01:
        mode = EMPROM_MODE_Addressing;
        cbit = 0;
        caddress = 0;
        output = 0x10;
        break;
    case EMPROM_DEVICE_X24C02:
        mode = EMPROM_MODE_Data;
        cbit = 0;
        output = 0x10;
        break;
    }
}
void Eprom::Stop()
{
    mode = EMPROM_MODE_Idle;
    output = 0x10;
}
void Eprom::RiseX24C01(int bit)
{
    switch (mode)
    {
    case EMPROM_MODE_Addressing:

        if (cbit < 7)
        {
            caddress &= ~(1 << cbit);
            caddress |= bit << cbit++;
        }
        else if (cbit < 8)
        {
            cbit = 8;

            if (bit != 0)
            {
                nextmode = EMPROM_MODE_Read;
                cdata = data[caddress];
            }
            else
            {
                nextmode = EMPROM_MODE_Write;
            }
        }
        break;

    case EMPROM_MODE_Ack:

        output = 0x00;
        break;

    case EMPROM_MODE_Read:

        if (cbit < 8)
            output = (cdata & 1 << cbit++) != 0 ? 0x10 : 0x00;

        break;

    case EMPROM_MODE_Write:

        if (cbit < 8)
        {
            cdata &= ~(1 << cbit);
            cdata |= bit << cbit++;
        }
        break;

    case EMPROM_MODE_AckWait:

        if (bit == 0)
            nextmode = EMPROM_MODE_Idle;

        break;
    }
}
void Eprom::RiseX24C02(int bit)
{
    switch (mode)
    {
    case EMPROM_MODE_Data:
        if (cbit < 8)
        {
            cdata &= ~(1 << (7 - cbit));
            cdata |= bit << (7 - cbit++);
        }
        break;
    case EMPROM_MODE_Addressing:
        if (cbit < 8)
        {
            caddress &= ~(1 << (7 - cbit));
            caddress |= bit << (7 - cbit++);
        }
        break;
    case EMPROM_MODE_Read:
        if (cbit < 8)
            output = (cdata & 1 << (7 - cbit++)) != 0 ? 0x10 : 0x00;
        break;
    case EMPROM_MODE_Write:
        if (cbit < 8)
        {
            cdata &= ~(1 << (7 - cbit));
            cdata |= bit << (7 - cbit++);
        }
        break;
    case EMPROM_MODE_NotAck:
        output = 0x10;
        break;
    case EMPROM_MODE_Ack:
        output = 0x00;
        break;
    case EMPROM_MODE_AckWait:
        if (bit == 0)
        {
            nextmode = EMPROM_MODE_Read;
            cdata = data[caddress];
        }
        break;
    }
}
void Eprom::FallX24C01()
{
    switch (mode)
    {
    case EMPROM_MODE_Addressing:

        if (cbit == 8)
        {
            mode = EMPROM_MODE_Ack;
            output = 0x10;
        }
        break;

    case EMPROM_MODE_Ack:

        mode = nextmode;
        cbit = 0;
        output = 0x10;
        break;

    case EMPROM_MODE_Read:

        if (cbit == 8)
        {
            mode = EMPROM_MODE_AckWait;
            caddress = (caddress + 1) & 0x7F;
        }
        break;

    case EMPROM_MODE_Write:

        if (cbit == 8)
        {
            mode = EMPROM_MODE_Ack;
            nextmode = EMPROM_MODE_Idle;
            data[caddress] = (uint8_t)cdata;
            caddress = (caddress + 1) & 0x7F;
        }
        break;
    }
}
void Eprom::FallX24C02()
{
    switch (mode)
    {
    case EMPROM_MODE_Data:

        if (cbit == 8)
        {
            if ((cdata & 0xA0) == 0xA0)
            {
                cbit = 0;
                mode = EMPROM_MODE_Ack;
                isRead = (cdata & 0x01) == 1;
                output = 0x10;

                if (isRead)
                {
                    nextmode = EMPROM_MODE_Read;
                    cdata = data[caddress];
                }
                else
                {
                    nextmode = EMPROM_MODE_Addressing;
                }
            }
            else
            {
                mode = EMPROM_MODE_NotAck;
                nextmode = EMPROM_MODE_Idle;
                output = 0x10;
            }
        }
        break;

    case EMPROM_MODE_Addressing:

        if (cbit == 8)
        {
            cbit = 0;
            mode = EMPROM_MODE_Ack;
            nextmode = (isRead ? EMPROM_MODE_Idle : EMPROM_MODE_Write);
            output = 0x10;
        }
        break;

    case EMPROM_MODE_Read:

        if (cbit == 8)
        {
            mode = EMPROM_MODE_AckWait;
            caddress = (caddress + 1) & 0xFF;
        }
        break;

    case EMPROM_MODE_Write:

        if (cbit == 8)
        {
            cbit = 0;
            mode = EMPROM_MODE_Ack;
            nextmode = EMPROM_MODE_Write;
            data[caddress] = (unsigned char)cdata;
            caddress = (caddress + 1) & 0xFF;
        }
        break;

    case EMPROM_MODE_NotAck:

        mode = EMPROM_MODE_Idle;
        cbit = 0;
        output = 0x10;
        break;

    case EMPROM_MODE_Ack:
    case EMPROM_MODE_AckWait:

        mode = nextmode;
        cbit = 0;
        output = 0x10;
        break;
    }
}

void Eprom::SaveState()
{
    for (uint32_t i = 0; i < mem_size; i++)
        state_str->Write(data[i]);

    state_str->Write( mode);
    state_str->Write(nextmode);
    state_str->Write(device) ;
    state_str->Write(psda);
    state_str->Write(pscl);
    state_str->Write(output) ;
    state_str->Write(cbit) ;
    state_str->Write( caddress);
    state_str->Write(cdata) ;
    state_str->Write(isRead);
    state_str->Write(cSCL);
    state_str->Write( cSDA);
}
void Eprom::LoadState()
{
    for (uint32_t i = 0; i < mem_size; i++)
        state_str->Read(data[i]);

    state_str->Read( mode);
    state_str->Read(nextmode);
    state_str->Read(device) ;
    state_str->Read(psda);
    state_str->Read(pscl);
    state_str->Read(output) ;
    state_str->Read(cbit) ;
    state_str->Read( caddress);
    state_str->Read(cdata) ;
    state_str->Read(isRead);
    state_str->Read(cSCL);
    state_str->Read( cSDA);
}
}

