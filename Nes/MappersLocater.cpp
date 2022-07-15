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

#include "Mappers/_Mappers.h"
#include <iostream>
#include "../Providers/VideoOut.h"
namespace NES
{
// TODO: find a better way to locate mappers.
void SetBoard(int mapperNumber)
{
    switch (mapperNumber)
    {
    default:
    {
        dev_board = new Mapper000();

        BoardFound=false;
        std::cout <<"NES: WARNING ! Mapper is not supported ! Switched to NROM [mapper # 0]; GAME MAY NOT WORK !\n";
        break;
    }
    case 0:
    {
        dev_board = new Mapper000();
        BoardFound=true;
        break;
    }
    case 1:
    {
        dev_board = new Mapper001();
        BoardFound=true;
        break;
    }
    case 2:
    {
        dev_board = new Mapper002();
        BoardFound=true;
        break;
    }
    case 3:
    {
        dev_board = new Mapper003();
        BoardFound=true;
        break;
    }
    case 4:
    {
        dev_board = new Mapper004();
        BoardFound=true;
        break;
    }
    case 5:
    {
        dev_board = new Mapper005();
        BoardFound=true;
        break;
    }
    case 6:
    {
        dev_board = new Mapper006();
        BoardFound=true;
        break;
    }
    case 7:
    {
        dev_board = new Mapper007();
        BoardFound=true;
        break;
    }
    case 8:
    {
        dev_board = new Mapper008();
        BoardFound=true;
        break;
    }
    case 9:
    {
        dev_board = new Mapper009();
        BoardFound=true;
        break;
    }
    case 10:
    {
        dev_board = new Mapper010();
        BoardFound=true;
        break;
    }
    case 11:
    {
        dev_board = new Mapper011();
        BoardFound=true;
        break;
    }
    case 13:
    {
        dev_board = new Mapper013();
        BoardFound=true;
        break;
    }
    case 15:
    {
        dev_board = new Mapper015();
        BoardFound=true;
        break;
    }
    case 16:
    {
        dev_board = new Mapper016();
        BoardFound=true;
        break;
    }
    case 17:
    {
        dev_board = new Mapper017();
        BoardFound=true;
        break;
    }
    case 18:
    {
        dev_board = new Mapper018();
        BoardFound=true;
        break;
    }
    case 19:
    {
        dev_board = new Mapper019();
        BoardFound=true;
        break;
    }
    case 21:
    {
        dev_board = new Mapper021();
        BoardFound=true;
        break;
    }
    case 22:
    {
        dev_board = new Mapper022();
        BoardFound=true;
        break;
    }
    case 23:
    {
        dev_board = new Mapper023();
        BoardFound=true;
        break;
    }
    case 24:
    {
        dev_board = new Mapper024();
        BoardFound=true;
        break;
    }
    case 25:
    {
        dev_board = new Mapper025();
        BoardFound=true;
        break;
    }
    case 26:
    {
        dev_board = new Mapper026();
        BoardFound=true;
        break;
    }
    case 32:
    {
        dev_board = new Mapper032();
        BoardFound=true;
        break;
    }
    case 33:
    {
        dev_board = new Mapper033();
        BoardFound=true;
        break;
    }
    case 34:
    {
        dev_board = new Mapper034();
        BoardFound=true;
        break;
    }
    case 41:
    {
        dev_board = new Mapper041();
        BoardFound=true;
        break;
    }
    case 42:
    {
        dev_board = new Mapper042();
        BoardFound=true;
        break;
    }
    case 44:
    {
        dev_board = new Mapper044();
        BoardFound=true;
        break;
    }
    case 45:
    {
        dev_board = new Mapper045();
        BoardFound=true;
        break;
    }
    case 46:
    {
        dev_board = new Mapper046();
        BoardFound=true;
        break;
    }
    case 47:
    {
        dev_board = new Mapper047();
        BoardFound=true;
        break;
    }
    case 48:
    {
        dev_board = new Mapper048();
        BoardFound=true;
        break;
    }
    case 49:
    {
        dev_board = new Mapper049();
        BoardFound=true;
        break;
    }
    case 50:
    {
        dev_board = new Mapper050();
        BoardFound=true;
        break;
    }
    case 51:
    {
        dev_board = new Mapper051();
        BoardFound=true;
        break;
    }
    case 52:
    {
        dev_board = new Mapper052();
        BoardFound=true;
        break;
    }
    case 57:
    {
        dev_board = new Mapper057();
        BoardFound=true;
        break;
    }
    case 58:
    {
        dev_board = new Mapper058();
        BoardFound=true;
        break;
    }
    case 60:
    {
        dev_board = new Mapper060();
        BoardFound=true;
        break;
    }
    case 61:
    {
        dev_board = new Mapper061();
        BoardFound=true;
        break;
    }
    case 62:
    {
        dev_board = new Mapper062();
        BoardFound=true;
        break;
    }
    case 64:
    {
        dev_board = new Mapper064();
        BoardFound=true;
        break;
    }
    case 65:
    {
        dev_board = new Mapper065();
        BoardFound=true;
        break;
    }
    case 66:
    {
        dev_board = new Mapper066();
        BoardFound=true;
        break;
    }
    case 67:
    {
        dev_board = new Mapper067();
        BoardFound=true;
        break;
    }
    case 68:
    {
        dev_board = new Mapper068();
        BoardFound=true;
        break;
    }
    case 69:
    {
        dev_board = new Mapper069();
        BoardFound=true;
        break;
    }
    case 70:
    {
        dev_board = new Mapper070();
        BoardFound=true;
        break;
    }
    case 71:
    {
        dev_board = new Mapper071();
        BoardFound=true;
        break;
    }
    case 72:
    {
        dev_board = new Mapper072();
        BoardFound=true;
        break;
    }
    case 73:
    {
        dev_board = new Mapper073();
        BoardFound=true;
        break;
    }
    case 74:
    {
        dev_board = new Mapper074();
        BoardFound=true;
        break;
    }
    case 75:
    {
        dev_board = new Mapper075();
        BoardFound=true;
        break;
    }
    case 76:
    {
        dev_board = new Mapper076();
        BoardFound=true;
        break;
    }
    case 77:
    {
        dev_board = new Mapper077();
        BoardFound=true;
        break;
    }
    case 78:
    {
        dev_board = new Mapper078();
        BoardFound=true;
        break;
    }
    case 79:
    {
        dev_board = new Mapper079();
        BoardFound=true;
        break;
    }
    case 80:
    {
        dev_board = new Mapper080();
        BoardFound=true;
        break;
    }
    case 82:
    {
        dev_board = new Mapper082();
        BoardFound=true;
        break;
    }
    case 85:
    {
        dev_board = new Mapper085();
        BoardFound=true;
        break;
    }
    case 86:
    {
        dev_board = new Mapper086();
        BoardFound=true;
        break;
    }
    case 87:
    {
        dev_board = new Mapper087();
        BoardFound=true;
        break;
    }
    case 88:
    {
        dev_board = new Mapper088();
        BoardFound=true;
        break;
    }
    case 89:
    {
        dev_board = new Mapper089();
        BoardFound=true;
        break;
    }
    case 90:
    {
        dev_board = new Mapper090();
        BoardFound=true;
        break;
    }
    case 91:
    {
        dev_board = new Mapper091();
        BoardFound=true;
        break;
    }
    case 92:
    {
        dev_board = new Mapper092();
        BoardFound=true;
        break;
    }
    case 93:
    {
        dev_board = new Mapper093();
        BoardFound=true;
        break;
    }
    case 94:
    {
        dev_board = new Mapper094();
        BoardFound=true;
        break;
    }
    case 95:
    {
        dev_board = new Mapper095();
        BoardFound=true;
        break;
    }
    case 96:
    {
        dev_board = new Mapper096();
        BoardFound=true;
        break;
    }
    case 97:
    {
        dev_board = new Mapper097();
        BoardFound=true;
        break;
    }
    case 99:
    {
        dev_board = new Mapper099();
        BoardFound=true;
        break;
    }
    case 105:
    {
        dev_board = new Mapper105();
        BoardFound=true;
        break;
    }
    case 107:
    {
        dev_board = new Mapper107();
        BoardFound=true;
        break;
    }
    case 112:
    {
        dev_board = new Mapper112();
        BoardFound=true;
        break;
    }
    case 113:
    {
        dev_board = new Mapper113();
        BoardFound=true;
        break;
    }
    case 115:
    {
        dev_board = new Mapper115();
        BoardFound=true;
        break;
    }
    case 118:
    {
        dev_board = new Mapper118();
        BoardFound=true;
        break;
    }
    case 119:
    {
        dev_board = new Mapper119();
        BoardFound=true;
        break;
    }
    case 133:
    {
        dev_board = new Mapper133();
        BoardFound=true;
        break;
    }
    case 140:
    {
        dev_board = new Mapper140();
        BoardFound=true;
        break;
    }
    case 154:
    {
        dev_board = new Mapper154();
        BoardFound=true;
        break;
    }
    case 159:
    {
        dev_board = new Mapper159();
        BoardFound=true;
        break;
    }
    case 160:
    {
        dev_board = new Mapper160();
        BoardFound=true;
        break;
    }
    case 164:
    {
        dev_board = new Mapper164();
        BoardFound=true;
        break;
    }
    case 165:
    {
        dev_board = new Mapper165();
        BoardFound=true;
        break;
    }
    case 180:
    {
        dev_board = new Mapper180();
        BoardFound=true;
        break;
    }
    case 182:
    {
        dev_board = new Mapper182();
        BoardFound=true;
        break;
    }
    case 184:
    {
        dev_board = new Mapper184();
        BoardFound=true;
        break;
    }
    case 185:
    {
        dev_board = new Mapper185();
        BoardFound=true;
        break;
    }
    case 189:
    {
        dev_board = new Mapper189();
        BoardFound=true;
        break;
    }
    case 191:
    {
        dev_board = new Mapper191();
        BoardFound=true;
        break;
    }
    case 192:
    {
        dev_board = new Mapper192();
        BoardFound=true;
        break;
    }
    case 193:
    {
        dev_board = new Mapper193();
        BoardFound=true;
        break;
    }
    case 194:
    {
        dev_board = new Mapper194();
        BoardFound=true;
        break;
    }
    case 200:
    {
        dev_board = new Mapper200();
        BoardFound=true;
        break;
    }
    case 201:
    {
        dev_board = new Mapper201();
        BoardFound=true;
        break;
    }
    case 202:
    {
        dev_board = new Mapper202();
        BoardFound=true;
        break;
    }
    case 203:
    {
        dev_board = new Mapper203();
        BoardFound=true;
        break;
    }
    case 204:
    {
        dev_board = new Mapper204();
        BoardFound=true;
        break;
    }
    case 205:
    {
        dev_board = new Mapper205();
        BoardFound=true;
        break;
    }
    case 206:
    {
        dev_board = new Mapper206();
        BoardFound=true;
        break;
    }
    case 207:
    {
        dev_board = new Mapper207();
        BoardFound=true;
        break;
    }
    case 209:
    {
        dev_board = new Mapper209();
        BoardFound=true;
        break;
    }
    case 210:
    {
        dev_board = new Mapper210();
        BoardFound=true;
        break;
    }
    case 212:
    {
        dev_board = new Mapper212();
        BoardFound=true;
        break;
    }
    case 213:
    {
        dev_board = new Mapper213();
        BoardFound=true;
        break;
    }
    case 214:
    {
        dev_board = new Mapper214();
        BoardFound=true;
        break;
    }
    case 216:
    {
        dev_board = new Mapper216();
        BoardFound=true;
        break;
    }
    case 222:
    {
        dev_board = new Mapper222();
        BoardFound=true;
        break;
    }
    case 225:
    {
        dev_board = new Mapper225();
        BoardFound=true;
        break;
    }
    case 226:
    {
        dev_board = new Mapper226();
        BoardFound=true;
        break;
    }
    case 227:
    {
        dev_board = new Mapper227();
        BoardFound=true;
        break;
    }
    case 228:
    {
        dev_board = new Mapper228();
        BoardFound=true;
        break;
    }
    case 229:
    {
        dev_board = new Mapper229();
        BoardFound=true;
        break;
    }
    case 230:
    {
        dev_board = new Mapper230();
        BoardFound=true;
        break;
    }
    case 231:
    {
        dev_board = new Mapper231();
        BoardFound=true;
        break;
    }
    case 232:
    {
        dev_board = new Mapper232();
        BoardFound=true;
        break;
    }
    case 233:
    {
        dev_board = new Mapper233();
        BoardFound=true;
        break;
    }
    case 240:
    {
        dev_board = new Mapper240();
        BoardFound=true;
        break;
    }
    case 242:
    {
        dev_board = new Mapper242();
        BoardFound=true;
        break;
    }
    case 243:
    {
        dev_board = new Mapper243();
        BoardFound=true;
        break;
    }
    case 245:
    {
        dev_board = new Mapper245();
        BoardFound=true;
        break;
    }
    case 246:
    {
        dev_board = new Mapper246();
        BoardFound=true;
        break;
    }
    case 255:
    {
        dev_board = new Mapper255();
        BoardFound=true;
        break;
    }
    }
}
}
