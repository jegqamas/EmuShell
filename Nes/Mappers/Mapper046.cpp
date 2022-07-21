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
// TODO: Test mapper 46
Mapper046::Mapper046()
{
    this->Name = "15-in-1 Color Dreams";
    this->MapperNumber = 46;
}
void Mapper046::HardReset()
{
    Board::HardReset();
    prg_reg=chr_reg =0;
}
void Mapper046::WriteSRM(uint16_t& address, uint8_t& value)
{
    prg_reg = (prg_reg & 0x01) | ((value << 1) & 0x1E);
    chr_reg = (chr_reg & 0x07) | ((value >> 1) & 0x78);
    Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
    Switch32KPRG(prg_reg, true);
}
void Mapper046::WritePRG(uint16_t& address, uint8_t& value)
{
    prg_reg = (value & 0x1) | (prg_reg & 0x1E);
    chr_reg = (value >> 4 & 0x7) | (chr_reg & 0x78);
    Switch08KCHR(chr_reg, chr_01K_rom_count > 0);
    Switch32KPRG(prg_reg, true);
}
void Mapper046::SaveState()
{
    Board::SaveState();
    state_str->Write(prg_reg);
    state_str->Write(chr_reg);
}
void Mapper046::LoadState()
{
    Board::LoadState();
    state_str->Read(prg_reg);
    state_str->Read(chr_reg);
}
}

