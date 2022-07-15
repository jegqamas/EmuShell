# EmuShell

A cross platform multi-system emulator written in C++ and SDL2.

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
