# EmuShell

A cross platform multi-system emulator written in C++ and SDL2.

Please note that this project is out-dated, for more recent nes-emulator written in C, please visit <https://github.com/jegqamas/ANES>. For nes-emulator written in C#, please visit <https://github.com/jegqamas/My-Nes>.

## Introduction

The aim for EmuShell is to be as accurate, fast, and easy to use as possible. For Nes (Nintendo Entertainment System) system, EmuShell emulates in half-cycle level, handling all tricky events that occur in real hardware like cpu interrupts checks at the second-half of the cpu cycle.

## Systems implemented

Check the wiki pages for which systems are supported.

## How To Use

Check out the [Wiki](https://github.com/Jegqamas/emushell/wiki) for more details about how to use EmuShell.

## System Requirements
* According to [this page](http://producthelp.sdl.com/SDL%20Trados%20Studio/client_en/Introducing/System_Requirements.htm),    you'll need a 1 GHz Pentium IV or higher compatible processor and at least 1 GB RAM.
  For EmuShell emulation, it uses a lot of cpu power hence the emulation goes cycle by cycle (half cpu 6502 cycle for nes) thus you'll need more than 1 GHz, after tests EmuShell work fine with 2400 GHz cpus, so it is recommended with 2 GHz Pentium IV or higher compatible processor. 
* Memory is not that important at the time, you'll need memory only for SDL2 as mentioned above, 1 GB RAM and larger should be enough.
* According to [this page](http://producthelp.sdl.com/SDL%20Trados%20Studio/client_en/Introducing/System_Requirements.htm), you'll need a Graphic card screen resolution of 1280x1024.

## Mappers And Boards: Total of 131 mappers/boards are supported. Note that implemented mappers may change from version to version. Here it is a list of supported mappers:

| Board/Mapper Name                | Board           | iNES Mapper Number |
|:--------------------------------:|:---------------:|:------------------:|
| ----                             | NROM-128        |                  0 |
| ----                             | NROM-256        |                  0 |
| MMC1                             | SxROM           |                  1 |
| ----                             | UxROM           |                  2 |
| ----                             | CNROM           |                  3 |
| MMC3                             | TxROM           |                  4 |
| MMC6                             | HxROM           |                  4 |
| MMC5                             | ExROM           |                  5 |
| ----                             | ----            |                  6 |
| ----                             | AxROM           |                  7 |
| ----                             | ----            |                  8 |
| MMC2                             | PxROM           |                  9 |
| MMC4                             | HVC-FJROM       |                 10 |
| ----                             | AVE-NINA-07     |                 11 |
| ----                             | CPROM           |                 13 |
| ----                             | ----            |                 15 |
| BANDAI                           | BANDAI-FCG-1    |                 16 |
| ----                             | ----            |                 17 | 
| JALECO                           | JALECO-JF-27    |                 18 | 
| Namcot 106                       | NAMCOT-175      |                 19 |
| VRC4                             | KONAMI-VRC-4    |                 21 |
| VRC2                             | KONAMI-VRC-2    |                 22 |
| VRC2                             | KONAMI-VRC-2    |                 23 |
| VRC6                             | KONAMI-VRC-6    |                 24 |
| VRC4                             | KONAMI-VRC-4    |                 25 |
| VRC6                             | KONAMI-VRC-6    |                 26 |
| IREM                             | IREM-G101       |                 32 |
| TAITO                            | TAITO-TC0350XXX |                 33 |
| NINA/BXROM                       | NINA/BXROM      |                 34 |
| CALTRON6IN1                      | MLT-CALTRON6IN1 |                 41 |
| Mario Baby                       | ----            |                 42 |
| 7-in-1 MMC3 Port A001h           | ----            |                 44 |
| X-in-1 MMC3 Port 6000hx4         | ----            |                 45 |
| 15-in-1 Color Dreams             | ----            |                 46 |
| 2-in-1 MMC3 Port 6000h           | QJ              |                 47 |
| TAITO                            | TAITO-TC0190XXX |                 48 |
| 4-in-1 MMC3 Port 6xxxh           | ----            |                 49 |
| 11-in-1                          | ----            |                 50 |
| FDS-Port - Alt. Levels           | ----            |                 51 |
| 7-in-1 MMC3 Port 6800h with SRAM | ----            |                 52 |
| 6-in-1 (SuperGK)                 | ----            |                 57 |
| 68-in-1 (Game Star)              | ----            |                 58 |
| ----                             | ----            |                 60 |
| 20-in-1                          | ----            |                 61 |
| Super 700-in-1                   | ----            |                 62 |
| Tengen RAMBO-1                   | TENGEN-800032   |                 64 |
| Irem H-3001                      | IREM-H3001      |                 65 |
| GxROM                            | MHROM           |                 66 |
| Sunsoft 3                        | SUNSOFT-3       |                 67 |
| Sunsoft 4                        | SUNSOFT-4       |                 68 |
| FME-7/Sunsoft 5B                 | SUNSOFT-FME-7   |                 69 |
| Bandai                           | BANDAI          |                 70 |
| CAMERICA                         | CAMERICA-BF9093 |                 71 |
| Jaleco Early Mapper 0            | JALECO-JF-17    |                 72 |
| VRC3                             | VRC3            |                 73 |
| Pirate MMC3 variant              | ----            |                 74 |
| VRC1                             | KONAMI-VRC-1    |                 75 |
| Namco 109                        | NAMCOT-3446     |                 76 |
| Irem                             | IREM-74*161     |                 77 |
| JALECO-JF-16                     | JALECO-JF-16    |                 78 |
| AVE Nina-3                       | AVE-MB-91       |                 79 |
| Taito X-005                      | TAITO-X1-005    |                 80 |
| Taito X1-17                      | TAITO-X1-017    |                 82 |
| VRC7                             | KONAMI-VRC-7    |                 85 |
| Jaleco Early Mapper 2            | JALECO-JF-13    |                 86 |
| Jaleco/Konami                    | JALECO-JF-06    |                 87 |
| Namco 118                        | NAMCOT-3443     |                 88 |
| Sunsoft Early                    | SUNSOFT-2       |                 89 |
| Pirate MMC5-style                | ----            |                 90 |
| HK-SF3                           | ----            |                 91 |
| Jaleco Early Mapper 1            | JALECO-JF-19    |                 92 |
| 74161/32                         | SUNSOFT-2       |                 93 |
| 74161/32                         | HVC-UN1ROM      |                 94 |
| Namcot MMC3-Style                | NAMCOT-3425     |                 95 |
| 74161/32                         | BANDAI-74*161   |                 96 |
| Irem - PRG HI                    | IREM-TAM-S1     |                 97 |
| VS Unisystem Port 4016           | ----            |                 99 |
| ----                             | NES-EVENT       |                105 |
| Magic Dragon Game                | ----            |                107 |
| Asder                            | ----            |                112 |
| Sachen/Hacker/Nina               | ----            |                113 |
| MMC3 Cart Saint                  | ----            |                115 |
| MMC3 TLSROM                      | TLSROM          |                118 |
| TQROM                            | TQROM           |                119 |
| Sachen                           | ----            |                133 |
| JALECO-JF-11                     | JALECO-JF-11    |                140 |
| NAMCOT-3453                      | NAMCOT-3453     |                154 |
| Bandai                           | BANDAI-LZ93D50  |                159 |
| X-in-1                           | ----            |                160 |
| ----                             | ----            |                164 |
| ----                             | ----            |                165 |
| Nihon Bussan                     | HVC-UNROM+74HC08|                180 |
| ----                             | ----            |                182 |
| Sunsoft                          | SUNSOFT-1       |                184 |
| ----                             | HVC-CNROM+SECUR |                185 |
| MMC3 Variant                     | TXC-PT8154      |                189 |
| Pirate MMC3 variant              | ----            |                191 |
| Pirate MMC3 variant              | ----            |                192 |
| ----                             | NTDEC-TC-112    |                193 |
| Pirate MMC3 variant              | ----            |                194 |
| ----                             | ----            |                200 |
| ----                             | ----            |                201 |
| 150-in-1                         | ----            |                202 |
| ----                             | ----            |                203 |
| 64-in-1                          | ----            |                204 |
| ----                             | ----            |                205 |
| ----                             | NAMCOT-3415     |                206 |
| ----                             | TAITO-X1-005    |                207 |
| Pirate MMC5-style                | ----            |                209 |
| Namcot 106                       | Namcot 106      |                210 |
| Super HIK 300-in-1 (1994)        | ----            |                212 |
| 9999999-in-1                     | ----            |                213 |
| X-in-1                           | ----            |                214 |
| ----                             | ----            |                216 |
| ----                             | ----            |                222 |
| ----                             | ----            |                225 |
| ----                             | ----            |                226 |
| ----                             | ----            |                227 |
| ----                             | MLT-ACTION52    |                228 |
| ----                             | ----            |                229 |
| ----                             | ----            |                230 |
| ----                             | ----            |                231 |
| ----                             | CAMERICA-ALGQ   |                232 |
| ----                             | ----            |                233 |
| ----                             | ----            |                240 |
| ----                             | ----            |                242 |
| Sachen Poker                     | ----            |                243 |
| C&E                              | ----            |                245 |
| ----                             | ----            |                246 |
| ----                             | ----            |                255 |

## Sound: Implement all Nes 5 sound channels, MMC5, VRC6 and S5B external sound channels along with sound downsampling using the Band-Limited Sound Buffer "Blip_Buffer" <http://www.slack.net/~ant/>.

| Channel  | Supported |
|----------|:---------:|
| 2A03 SQ1 | yes       |
| 2A03 SQ2 | yes       |
| 2A03 TRI | yes       |
| 2A03 NOI | yes       |
| 2A03 DMC | yes       |
| MMC5 SQ1 | yes       |
| MMC5 SQ2 | yes       |
| MMC5 DMC | yes       |
| VRC6 SQ1 | yes       |
| VRC6 SQ2 | yes       |
| VRC6 SAW | yes       |
| S5B  SQ1 | yes       |
| S5B  SQ2 | yes       |
| S5B  SQ3 | yes       |
| N106     | no        |
| VRC7     | no        |
