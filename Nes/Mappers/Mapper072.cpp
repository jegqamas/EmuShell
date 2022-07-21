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

namespace NES
{
Mapper072::Mapper072()
{
    this->Name = "Jaleco Early Mapper 0";
    this->MapperNumber = 72;
}
void Mapper072::HardReset()
{
    Board::HardReset();
    Switch16KPRG(prg_16K_rom_mask, 0xC000, true);
    writeData = chr_reg = prg_reg = 0;
}
void Mapper072::WritePRG(uint16_t& address, uint8_t& value)
{
    writeData = value & ReadPRG(address);// Bus conflicts!!
    switch ((value >> 6) & 0x3)
    {
    case 0:// Do switches
    {
        Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
        Switch16KPRG(prg_reg, 0x8000, true);
        break;
    }
    case 1:
        chr_reg = value & 0x0F;
        break;// Set chr reg
    case 2:
        prg_reg = value & 0x0F;
        break;// Set prg reg
    }
}
void Mapper072::SaveState()
{
    Board::SaveState();
    state_str->Write(writeData);
    state_str->Write(chr_reg);
    state_str->Write(prg_reg);
}
void Mapper072::LoadState()
{
    Board::LoadState();
    state_str->Read(writeData);
    state_str->Read(chr_reg);
    state_str->Read(prg_reg);
}
}


