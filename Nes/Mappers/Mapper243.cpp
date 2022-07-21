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

#include "_Mappers.h"
#include "../../Definations.h"

namespace NES
{
Mapper243::Mapper243()
{
    this->Name = "Sachen Poker";
    this->MapperNumber = 243;
}
void Mapper243::HardReset()
{
    Board::HardReset();
    addr = chr_reg = 0;
}
void Mapper243::WriteEXP(uint16_t& address, uint8_t& value)
{
    if ((address < 0x5000) && (address >= 0x4020))
        switch (address & 0x4101)
        {
        case 0x4100:
            addr = value & 0x7;
            break;
        case 0x4101:
        {
            switch (addr)
            {
            case 2:
                chr_reg = ((value << 3) & 0x8) | (chr_reg & 0x7);
                Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
                break;
            case 4:
                chr_reg = (value & 1) | (chr_reg & 0xE);
                Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
                break;
            case 5:
                Switch32KPRG(value & 0x7, true);
                break;
            case 6:
                chr_reg = ((value & 0x3) << 1) | (chr_reg & 0x9);
                Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
                break;
            case 7:
            {
                switch ((value >> 1) & 3)
                {
                case 0:
                    SwitchNMT(MIRRORING_Horz);
                    break;
                case 1:
                    SwitchNMT(MIRRORING_Vert);
                    break;
                case 2:
                    SwitchNMT(0xE);
                    break;
                case 3:
                    SwitchNMT(MIRRORING_OneScB);
                    break;
                }
                break;
            }
            }
            break;
        }
        }
}
void Mapper243::SaveState()
{
    Board::SaveState();
    state_str->Write(addr);
    state_str->Write(chr_reg);
}
void Mapper243::LoadState()
{
    Board::LoadState();
    state_str->Read(addr);
    state_str->Read(chr_reg);
}
}



