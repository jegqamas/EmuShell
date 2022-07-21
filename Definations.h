// This file is part of EmuShell
// A multi video game consoles emulator.
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

/* Emu systems */
#define SYSTEM_NULL "Standby"
#define SYSTEM_NES "Nintendo Entertinement System (Nes)"

/*System IDs (Very important !!)*/
#define SYSTEM_ID_NULL 0
#define SYSTEM_ID_NES 1

/* File formats ...*/
#define FILE_FORMAT_INES ".nes"

/* TV formats*/
#define TVSYSTEM_NTSC 0
#define TVSYSTEM_PALB 1
#define TVSYSTEM_DENDY 2

/* Nes mirroring */
#define MIRRORING_Horz 0x50
#define MIRRORING_Vert 0x44
#define MIRRORING_OneScA 0x00
#define MIRRORING_OneScB 0x55
#define MIRRORING_Full 0xE4

/* Nes irq flags */
#define IRQ_FLAG_APU 0x1
#define IRQ_FLAG_DMC 0x2
#define IRQ_FLAG_BOARD 0x4

#define IRQ_FLAG_APU_OFF 0xFE
#define IRQ_FLAG_DMC_OFF 0xFD
#define IRQ_FLAG_BOARD_OFF 0xFB

#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240
