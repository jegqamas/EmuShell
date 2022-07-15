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

#include "_Mappers.h"

namespace NES
{
Mapper207::Mapper207()
{
    this->Name = "Unknown";
    this->MapperNumber = 207;
}
void Mapper207::HardReset()
{
    Board::HardReset();
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);
}
void Mapper207::WriteSRM(uint16_t& address, uint8_t& value)
{
    switch (address)
    {
    case 0x7EF0:
    {
        Switch02KCHR(value & 0x7F >> 1, 0x0000, chr_01K_rom_count > 0);
        mirroring0 = value >> 7 & 1;
        break;
    }
    case 0x7EF1:
    {
        Switch02KCHR(value & 0x7F >> 1, 0x0800, chr_01K_rom_count > 0);
        mirroring1 = value >> 7 & 1;
        break;
    }
    case 0x7EF2:
        Switch01KCHR(value, 0x1000, chr_01K_rom_count > 0);
        break;
    case 0x7EF3:
        Switch01KCHR(value, 0x1400, chr_01K_rom_count > 0);
        break;
    case 0x7EF4:
        Switch01KCHR(value, 0x1800, chr_01K_rom_count > 0);
        break;
    case 0x7EF5:
        Switch01KCHR(value, 0x1C00, chr_01K_rom_count > 0);
        break;
    case 0x7EFA:
    case 0x7EFB:
        Switch08KPRG(value, 0x8000, true);
        break;
    case 0x7EFC:
    case 0x7EFD:
        Switch08KPRG(value, 0xA000, true);
        break;
    case 0x7EFE:
    case 0x7EFF:
        Switch08KPRG(value, 0xC000, true);
        break;
    }
}
void Mapper207::WriteNMT(uint16_t& address, uint8_t& value)
{
    switch ((address >> 10) & 3)
    {
    case 0:
    case 1:
        nmt_banks[mirroring0][address & 0x3FF] = value;
        break;
    case 2:
    case 3:
        nmt_banks[mirroring1][address & 0x3FF] = value;
        break;
    }
}
uint8_t Mapper207::ReadNMT(uint16_t& address)
{
    switch ((address >> 10) & 3)
    {
    case 0:
    case 1:
        return nmt_banks[mirroring0][address & 0x3FF];
    case 2:
    case 3:
        return nmt_banks[mirroring1][address & 0x3FF];
    }
    return 0;// Make compiler happy.
}
void Mapper207::SaveState()
{
    Board::SaveState();
    state_str->Write(mirroring0);
    state_str->Write(mirroring1);
}
void Mapper207::LoadState()
{
    Board::LoadState();
    state_str->Read(mirroring0);
    state_str->Read(mirroring1);
}
}

