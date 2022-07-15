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
//TODO: Mapper 180's Crazy Climber needs special controller which not implemented yet.
Mapper185::Mapper185()
{
    this->Name = "Unknown";
    this->MapperNumber = 185;
}
void Mapper185::HardReset()
{
    Board::HardReset();
    lockchr = false;
}
uint8_t Mapper185::ReadCHR(uint16_t& address, bool spriteFetch)
{
    if (!lockchr)
        return Board::ReadCHR(address, spriteFetch);
    else
        return 0xFF;
}
void Mapper185::WritePRG(uint16_t& address, uint8_t& value)
{
    lockchr = ((value & 0xF) == 0) || (value == 0x13);
}
void Mapper185::SaveState()
{
    Board::SaveState();
    state_str->Write(lockchr);
}
void Mapper185::LoadState()
{
    Board::LoadState();
    state_str->Read(lockchr);
}
}



