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
Mapper096::Mapper096()
{
    this->Name = "74161/32";
    this->MapperNumber = 96;
    this->prg_ram_default_08KB_count = 1;
    this->chr_ram_1KB_default_banks_count=32;
}
void Mapper096::HardReset()
{
    Board::HardReset();
    flag_c = 0;
    Switch04KCHR(3, 0x1000, false);
}
void Mapper096::WritePRG(uint16_t& address, uint8_t& value)
{
    Switch32KPRG(value & 0x3, true);
    flag_c = (value & 0x4) == 0x4 ? 0x01 : 0;
    Switch04KCHR(3, 0x1000, false);
}
void  Mapper096::OnPPUAddressUpdate(uint16_t& address)
{
    if ((address & 0x03FF) < 0x03C0 && (address & 0x1000) == 0x0000)
        Switch04KCHR(((address & 0x0300) >> 8) | flag_c, 0x0000, false);
}
void Mapper096::SaveState()
{
    Board::SaveState();
    state_str->Write(flag_c);
}
void  Mapper096::LoadState()
{
    Board::LoadState();
    state_str->Read(flag_c);
}
}

