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
#include "Cpu6502.h"
#include <iostream>
#include <vector>

namespace Common
{
CPU6502::CPU6502()
{
    state_buffer_length = 34;
}
CPU6502::~CPU6502()
{
    ShutDown();
}
// STATUS REGISTER
uint8_t CPU6502::p_get()
{
    return (uint8_t)(
               (flag_n ? 0x80 : 0) |
               (flag_v ? 0x40 : 0) |
               (flag_d ? 0x08 : 0) |
               (flag_i ? 0x04 : 0) |
               (flag_z ? 0x02 : 0) |
               (flag_c ? 0x01 : 0) | 0x20);
}
uint8_t CPU6502::pb_get()
{
    return (uint8_t)(
               (flag_n ? 0x80 : 0) |
               (flag_v ? 0x40 : 0) |
               (flag_d ? 0x08 : 0) |
               (flag_i ? 0x04 : 0) |
               (flag_z ? 0x02 : 0) |
               (flag_c ? 0x01 : 0) | 0x30);
}
void CPU6502::p_set(uint8_t value)
{
    flag_n = (value & 0x80) != 0;
    flag_v = (value & 0x40) != 0;
    flag_d = (value & 0x08) != 0;
    flag_i = (value & 0x04) != 0;
    flag_z = (value & 0x02) != 0;
    flag_c = (value & 0x01) != 0;
}

// METHODS
void CPU6502::Initialize(void(*writeAccess)(uint16_t address, uint8_t value), uint8_t(*readAccess)(uint16_t address))
{
    this->read = readAccess;
    this->write = writeAccess;
}
void CPU6502::HardReset()
{
    // Reset registers
    reg_a = 0x00;
    reg_x = 0x00;
    reg_y = 0x00;
    reg_sp.low = 0xFD;
    reg_sp.high = 0x01;

    p_set(0x0);// Reset all flags
    flag_i = true;
    reg_ea.value = 0;

    // Reset !!
    interrupt_suspend_nmi = true;
    interrupt_suspend_irq = true;

    int rst = 0xFFFC;
    reg_pc.low = read(rst);
    rst++;
    reg_pc.high = read(rst);

    // Reset values
    opcode = 0;
    M = 0;
    pin_irq = 0;
    nmi_detect = false;
    irq_detected = false;
    interrupt_vector = 0;
    interrupt_suspend_nmi = false;
    interrupt_suspend_irq = false;

    byte_temp = 0;
    int_temp = 0;
    int_temp1 = 0;
    dummy = 0;
}
void CPU6502::SoftReset()
{
    flag_i = true;
    reg_sp.low -= 3;

    reg_pc.low = read(0xFFFC);
    reg_pc.high = read(0xFFFD);
}
void CPU6502::ShutDown()
{
    // Reset registers
    reg_a = 0x00;
    reg_x = 0x00;
    reg_y = 0x00;
    reg_sp.low = 0x0;
    reg_sp.high = 0x0;
    p_set(0x0);// Reset all flags
    reg_ea.value = 0;
    reg_pc.low = 0;
    reg_pc.high = 0;
    // Reset values
    opcode = 0;
    M = 0;
    nmi_detect = false;
    irq_detected = false;
    interrupt_vector = 0;
    interrupt_suspend_nmi = false;
    interrupt_suspend_irq = false;
}

void CPU6502::PollInterruptStatus()
{
    if (!interrupt_suspend_nmi)
    {
        // The edge detector, see if nmi occurred.
        if (pin_nmi) // Raising edge, set nmi request
            nmi_detect = true;

        pin_nmi = false;// NMI detected or not, low both lines for this form ___|-|__
    }
    if (!interrupt_suspend_irq)
    {
        // irq level detector
        irq_detected = (!flag_i && (pin_irq != 0));
    }
    // Update interrupt vector !
    interrupt_vector = nmi_detect ? 0xFFFA : 0xFFFE;
}

// Helper methods
void CPU6502::Branch(bool condition)
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;

    if (condition)
    {
        interrupt_suspend_irq = true;
        read(reg_pc.value);
        reg_pc.low += byte_temp;
        interrupt_suspend_irq = false;
        if (byte_temp >= 0x80)
        {
            if (reg_pc.low >= byte_temp)
            {
                read(reg_pc.value);
                reg_pc.high--;
            }
        }
        else
        {
            if (reg_pc.low < byte_temp)
            {
                read(reg_pc.value);
                reg_pc.high++;
            }
        }
    }
}
void CPU6502::Push(uint8_t val)
{
    write(reg_sp.value, val);
    reg_sp.low--;
}
uint8_t CPU6502::Pull()
{
    reg_sp.low++;
    return read(reg_sp.value);
}
void CPU6502::Interrupt()
{
    Push(reg_pc.high);
    Push(reg_pc.low);

    Push(is_brk ? pb_get() : p_get());
    // the vector is detected during ?2 of previous cycle (before push about 2 ppu cycles)
    int_temp = interrupt_vector;
    // THEORY:
    // Once the vector requested, the interrupts are suspended and cleared
    // by setting the I flag and clearing the nmi detect flag. Also, the nmi
    // detection get suspended for 2 cycles while pulling PC, irq still can
    // be detected but will not be taken since I is set.
    interrupt_suspend_nmi = true;
    flag_i = true;
    nmi_detect = false;

    reg_pc.low = read(int_temp);
    int_temp++;
    reg_pc.high = read(int_temp);

    interrupt_suspend_nmi = false;
}

/*
* Addressing modes...
* _R: read access instructions, set the M value. Some addressing modes will execute 1 extra cycle when page crossed.
* _W: write access instructions, doesn't set the M value. The instruction should handle write at effective address (EF).
* _RW: read-Modify-write instructions, set the M value and the instruction should handle write at effective address (EF).
*/
void CPU6502::IndirectX_R()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    read(byte_temp);// Clock 2
    byte_temp += reg_x;

    reg_ea.low = read(byte_temp);// Clock 3
    byte_temp++;

    reg_ea.high = read(byte_temp);// Clock 4

    M = read(reg_ea.value);
}
void CPU6502::IndirectX_W()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    read(byte_temp);// Clock 2
    byte_temp += reg_x;

    reg_ea.low = read(byte_temp);// Clock 3
    byte_temp++;

    reg_ea.high = read(byte_temp);// Clock 4
}
void CPU6502::IndirectX_RW()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    read(byte_temp);// Clock 2
    byte_temp += reg_x;

    reg_ea.low = read(byte_temp);// Clock 3
    byte_temp++;

    reg_ea.high = read(byte_temp);// Clock 4

    M = read(reg_ea.value);
}
void CPU6502::IndirectY_R()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    reg_ea.low = read(byte_temp);
    byte_temp++;// Clock 2
    reg_ea.high = read(byte_temp);// Clock 2

    reg_ea.low += reg_y;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
    {
        reg_ea.high++;
        M = read(reg_ea.value);// Clock 4
    }
}
void CPU6502::IndirectY_W()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    reg_ea.low = read(byte_temp);
    byte_temp++;// Clock 2
    reg_ea.high = read(byte_temp);// Clock 2

    reg_ea.low += reg_y;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
        reg_ea.high++;
}
void CPU6502::IndirectY_RW()
{
    byte_temp = read(reg_pc.value);
    reg_pc.value++;// CLock 1
    reg_ea.low = read(byte_temp);
    byte_temp++;// Clock 2
    reg_ea.high = read(byte_temp);// Clock 2

    reg_ea.low += reg_y;

    read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
        reg_ea.high++;

    M = read(reg_ea.value);// Clock 4
}
void CPU6502::ZeroPage_R()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    M = read(reg_ea.value);// Clock 2
}
void CPU6502::ZeroPage_W()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
}
void CPU6502::ZeroPage_RW()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    M = read(reg_ea.value);// Clock 2
}
void CPU6502::ZeroPageX_R()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_x;
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::ZeroPageX_W()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_x;
}
void CPU6502::ZeroPageX_RW()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_x;
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::ZeroPageY_R()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_y;
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::ZeroPageY_W()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_y;
}
void CPU6502::ZeroPageY_RW()
{
    reg_ea.value = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    read(reg_ea.value);// Clock 2
    reg_ea.low += reg_y;
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::Immediate()
{
    M = read(reg_pc.value);
    reg_pc.value++;// Clock 1
}
void CPU6502::ImpliedAccum()
{
    dummy = read(reg_pc.value);
}
void CPU6502::Absolute_R()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::Absolute_W()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2
}
void CPU6502::Absolute_RW()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2
    M = read(reg_ea.value);// Clock 3
}
void CPU6502::AbsoluteX_R()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_x;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_x)
    {
        reg_ea.high++;
        M = read(reg_ea.value);// Clock 4
    }
}
void CPU6502::AbsoluteX_W()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_x;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_x)
        reg_ea.high++;
}
void CPU6502::AbsoluteX_RW()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_x;

    read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_x)
        reg_ea.high++;

    M = read(reg_ea.value);// Clock 4
}
void CPU6502::AbsoluteY_R()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_y;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
    {
        reg_ea.high++;
        M = read(reg_ea.value);// Clock 4
    }
}
void CPU6502::AbsoluteY_W()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_y;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
        reg_ea.high++;
}
void CPU6502::AbsoluteY_RW()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;// Clock 1
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;// Clock 2

    reg_ea.low += reg_y;

    M = read(reg_ea.value);// Clock 3
    if (reg_ea.low < reg_y)
        reg_ea.high++;

    M = read(reg_ea.value);// Clock 4
}

/*
* Instructions...
*/
void CPU6502::ADC()
{
    int_temp = (reg_a + M + (flag_c ? 1 : 0));

    flag_v = ((int_temp ^ reg_a) & (int_temp ^ M) & 0x80) != 0;
    flag_n = (int_temp & 0x80) != 0;
    flag_z = (int_temp & 0xFF) == 0;
    flag_c = (int_temp >> 0x8) != 0;

    reg_a = (uint8_t)(int_temp & 0xFF);
}
void CPU6502::AHX()
{
    byte_temp = (uint8_t)((reg_a & reg_x) & 7);
    write(reg_ea.value, byte_temp);
}
void CPU6502::ALR()
{
    reg_a &= M;

    flag_c = (reg_a & 0x01) != 0;

    reg_a >>= 1;

    flag_n = (reg_a & 0x80) != 0;
    flag_z = reg_a == 0;
}
void CPU6502::ANC()
{
    reg_a &= M;
    flag_n = (reg_a & 0x80) != 0;
    flag_z = reg_a == 0;
    flag_c = (reg_a & 0x80) != 0;
}
void CPU6502::AND()
{
    reg_a &= M;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::ARR()
{
    reg_a = (uint8_t)(((M & reg_a) >> 1) | (flag_c ? 0x80 : 0x00));

    flag_z = (reg_a & 0xFF) == 0;
    flag_n = (reg_a & 0x80) != 0;
    flag_c = (reg_a & 0x40) != 0;
    flag_v = ((reg_a << 1 ^ reg_a) & 0x40) != 0;
}
void CPU6502::AXS()
{
    int_temp = (reg_a & reg_x) - M;

    flag_n = (int_temp & 0x80) != 0;
    flag_z = (int_temp & 0xFF) == 0;
    flag_c = (~int_temp >> 8) != 0;

    reg_x = (uint8_t)(int_temp & 0xFF);
}
void CPU6502::ASL_M()
{
    flag_c = (M & 0x80) == 0x80;
    write(reg_ea.value, M);

    M = (uint8_t)((M << 1) & 0xFE);

    write(reg_ea.value, M);

    flag_n = (M & 0x80) == 0x80;
    flag_z = (M == 0);
}
void CPU6502::ASL_A()
{
    flag_c = (reg_a & 0x80) == 0x80;

    reg_a = (uint8_t)((reg_a << 1) & 0xFE);

    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::BIT()
{
    flag_n = (M & 0x80) != 0;
    flag_v = (M & 0x40) != 0;
    flag_z = (M & reg_a) == 0;
}
void CPU6502::BRK()
{
    is_brk=true;
    read(reg_pc.value);
    reg_pc.value++;
    Interrupt();
    is_brk=false;

}
void CPU6502::CMP()
{
    int_temp = reg_a - M;
    flag_n = (int_temp & 0x80) == 0x80;
    flag_c = (reg_a >= M);
    flag_z = (int_temp == 0);
}
void CPU6502::CPX()
{
    int_temp = reg_x - M;
    flag_n = (int_temp & 0x80) == 0x80;
    flag_c = (reg_x >= M);
    flag_z = (int_temp == 0);
}
void CPU6502::CPY()
{
    int_temp = reg_y - M;
    flag_n = (int_temp & 0x80) == 0x80;
    flag_c = (reg_y >= M);
    flag_z = (int_temp == 0);
}
void CPU6502::DCP()
{
    write(reg_ea.value, M);

    M--;
    write(reg_ea.value, M);

    int_temp = reg_a - M;

    flag_n = (int_temp & 0x80) != 0;
    flag_z = int_temp == 0;
    flag_c = (~int_temp >> 8) != 0;
}
void CPU6502::DEC()
{
    write(reg_ea.value, M);
    M--;
    write(reg_ea.value, M);
    flag_n = (M & 0x80) == 0x80;
    flag_z = (M == 0);
}
void CPU6502::DEY()
{
    reg_y--;
    flag_z = (reg_y == 0);
    flag_n = (reg_y & 0x80) == 0x80;
}
void CPU6502::DEX()
{
    reg_x--;
    flag_z = (reg_x == 0);
    flag_n = (reg_x & 0x80) == 0x80;
}
void CPU6502::EOR()
{
    reg_a ^= M;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::INC()
{
    write(reg_ea.value, M);
    M++;
    write(reg_ea.value, M);
    flag_n = (M & 0x80) == 0x80;
    flag_z = (M == 0);
}
void CPU6502::INX()
{
    reg_x++;
    flag_z = (reg_x == 0);
    flag_n = (reg_x & 0x80) == 0x80;
}
void CPU6502::INY()
{
    reg_y++;
    flag_n = (reg_y & 0x80) == 0x80;
    flag_z = (reg_y == 0);
}
void CPU6502::ISC()
{
    byte_temp = read(reg_ea.value);

    write(reg_ea.value, byte_temp);

    byte_temp++;

    write(reg_ea.value, byte_temp);

    int_temp = byte_temp ^ 0xFF;
    int_temp1 = (reg_a + int_temp + (flag_c ? 1 : 0));

    flag_n = (int_temp1 & 0x80) != 0;
    flag_v = ((int_temp1 ^ reg_a) & (int_temp1 ^ int_temp) & 0x80) != 0;
    flag_z = (int_temp1 & 0xFF) == 0;
    flag_c = (int_temp1 >> 0x8) != 0;
    reg_a = (uint8_t)(int_temp1 & 0xFF);
}
void CPU6502::JMP_I()
{
    // Fetch pointer
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;
    reg_ea.high = read(reg_pc.value);
    reg_pc.value++;

    reg_pc.low = read(reg_ea.value);
    reg_ea.low++; // only increment the low uint8_t, causing the "JMP ($nnnn)" bug
    reg_pc.high = read(reg_ea.value);
}
void CPU6502::JSR()
{
    reg_ea.low = read(reg_pc.value);
    reg_pc.value++;
    //reg_ea.high = read(reg_pc.value);

    read(reg_sp.value);// Dummy read !

    Push(reg_pc.high);
    Push(reg_pc.low);

    reg_ea.high = read(reg_pc.value);
    reg_pc.value = reg_ea.value;
}
void CPU6502::LAR()
{
    reg_sp.low &= M;
    reg_a = reg_sp.low;
    reg_x = reg_sp.low;

    flag_n = (reg_sp.low & 0x80) != 0;
    flag_z = (reg_sp.low & 0xFF) == 0;
}
void CPU6502::LAX()
{
    reg_x = reg_a = M;

    flag_n = (reg_x & 0x80) != 0;
    flag_z = (reg_x & 0xFF) == 0;
}
void CPU6502::LDA()
{
    reg_a = M;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::LDX()
{
    reg_x = M;
    flag_n = (reg_x & 0x80) == 0x80;
    flag_z = (reg_x == 0);
}
void CPU6502::LDY()
{
    reg_y = M;
    flag_n = (reg_y & 0x80) == 0x80;
    flag_z = (reg_y == 0);
}
void CPU6502::LSR_A()
{
    flag_c = (reg_a & 1) == 1;
    reg_a >>= 1;
    flag_z = (reg_a == 0);
    flag_n = (reg_a & 0x80) != 0;
}
void CPU6502::LSR_M()
{
    flag_c = (M & 1) == 1;
    write(reg_ea.value, M);
    M >>= 1;

    write(reg_ea.value, M);
    flag_z = (M == 0);
    flag_n = (M & 0x80) != 0;
}
void CPU6502::ORA()
{
    reg_a |= M;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::PHA()
{
    Push(reg_a);
}
void CPU6502::PHP()
{
    Push(pb_get());
}
void CPU6502::PLA()
{
    read(reg_sp.value);
    reg_a = Pull();
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::PLP()
{
    read(reg_sp.value);
    p_set(Pull());
}
void CPU6502::RLA()
{
    byte_temp = read(reg_ea.value);

    write(reg_ea.value, byte_temp);

    dummy = (uint8_t)((byte_temp << 1) | (flag_c ? 0x01 : 0x00));

    write(reg_ea.value, dummy);

    flag_n = (dummy & 0x80) != 0;
    flag_z = (dummy & 0xFF) == 0;
    flag_c = (byte_temp & 0x80) != 0;

    reg_a &= dummy;
    flag_n = (reg_a & 0x80) != 0;
    flag_z = (reg_a & 0xFF) == 0;
}
void CPU6502::ROL_A()
{
    byte_temp = (uint8_t)((reg_a << 1) | (flag_c ? 0x01 : 0x00));

    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;
    flag_c = (reg_a & 0x80) != 0;

    reg_a = byte_temp;
}
void CPU6502::ROL_M()
{
    write(reg_ea.value, M);
    byte_temp = (uint8_t)((M << 1) | (flag_c ? 0x01 : 0x00));

    write(reg_ea.value, byte_temp);
    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;
    flag_c = (M & 0x80) != 0;
}
void CPU6502::ROR_A()
{
    byte_temp = (uint8_t)((reg_a >> 1) | (flag_c ? 0x80 : 0x00));

    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;
    flag_c = (reg_a & 0x01) != 0;

    reg_a = byte_temp;
}
void CPU6502::ROR_M()
{
    write(reg_ea.value, M);

    byte_temp = (uint8_t)((M >> 1) | (flag_c ? 0x80 : 0x00));
    write(reg_ea.value, byte_temp);

    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;
    flag_c = (M & 0x01) != 0;
}
void CPU6502::RRA()
{
    byte_temp = read(reg_ea.value);

    write(reg_ea.value, byte_temp);

    dummy = (uint8_t)((byte_temp >> 1) | (flag_c ? 0x80 : 0x00));

    write(reg_ea.value, dummy);

    flag_n = (dummy & 0x80) != 0;
    flag_z = (dummy & 0xFF) == 0;
    flag_c = (byte_temp & 0x01) != 0;

    byte_temp = dummy;
    int_temp = (reg_a + byte_temp + (flag_c ? 1 : 0));

    flag_n = (int_temp & 0x80) != 0;
    flag_v = ((int_temp ^ reg_a) & (int_temp ^ byte_temp) & 0x80) != 0;
    flag_z = (int_temp & 0xFF) == 0;
    flag_c = (int_temp >> 0x8) != 0;
    reg_a = (uint8_t)(int_temp);
}
void CPU6502::RTI()
{
    read(reg_sp.value);
    p_set(Pull());

    reg_pc.low = Pull();
    reg_pc.high = Pull();
}
void CPU6502::RTS()
{
    read(reg_sp.value);
    reg_pc.low = Pull();
    reg_pc.high = Pull();

    reg_pc.value++;

    read(reg_pc.value);
}
void CPU6502::SAX()
{
    write(reg_ea.value, (uint8_t)(reg_x & reg_a));
}
void CPU6502::SBC()
{
    M ^= 0xFF;
    int_temp = (reg_a + M + (flag_c ? 1 : 0));

    flag_n = (int_temp & 0x80) != 0;
    flag_v = ((int_temp ^ reg_a) & (int_temp ^ M) & 0x80) != 0;
    flag_z = (int_temp & 0xFF) == 0;
    flag_c = (int_temp >> 0x8) != 0;
    reg_a = (uint8_t)(int_temp);
}
void CPU6502::SHX()
{
    byte_temp = (uint8_t)(reg_x & (reg_ea.high + 1));

    read(reg_ea.value);
    reg_ea.low += reg_y;

    if (reg_ea.low < reg_y)
        reg_ea.high = byte_temp;

    write(reg_ea.value, byte_temp);
}
void CPU6502::SHY()
{
    byte_temp = (uint8_t)(reg_y & (reg_ea.high + 1));

    read(reg_ea.value);
    reg_ea.low += reg_x;

    if (reg_ea.low < reg_x)
        reg_ea.high = byte_temp;
    write(reg_ea.value, byte_temp);
}
void CPU6502::SLO()
{
    byte_temp = read(reg_ea.value);

    flag_c = (byte_temp & 0x80) != 0;

    write(reg_ea.value, byte_temp);

    byte_temp <<= 1;

    write(reg_ea.value, byte_temp);

    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;

    reg_a |= byte_temp;
    flag_n = (reg_a & 0x80) != 0;
    flag_z = (reg_a & 0xFF) == 0;
}
void CPU6502::SRE()
{
    byte_temp = read(reg_ea.value);

    flag_c = (byte_temp & 0x01) != 0;

    write(reg_ea.value, byte_temp);

    byte_temp >>= 1;

    write(reg_ea.value, byte_temp);

    flag_n = (byte_temp & 0x80) != 0;
    flag_z = (byte_temp & 0xFF) == 0;

    reg_a ^= byte_temp;
    flag_n = (reg_a & 0x80) != 0;
    flag_z = (reg_a & 0xFF) == 0;
}
void CPU6502::STA()
{
    write(reg_ea.value, reg_a);
}
void CPU6502::STX()
{
    write(reg_ea.value, reg_x);
}
void CPU6502::STY()
{
    write(reg_ea.value, reg_y);
}
void CPU6502::TAX()
{
    reg_x = reg_a;
    flag_n = (reg_x & 0x80) == 0x80;
    flag_z = (reg_x == 0);
}
void CPU6502::TAY()
{
    reg_y = reg_a;
    flag_n = (reg_y & 0x80) == 0x80;
    flag_z = (reg_y == 0);
}
void CPU6502::TSX()
{
    reg_x = reg_sp.low;
    flag_n = (reg_x & 0x80) != 0;
    flag_z = reg_x == 0;
}
void CPU6502::TXA()
{
    reg_a = reg_x;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::TXS()
{
    reg_sp.low = reg_x;
}
void CPU6502::TYA()
{
    reg_a = reg_y;
    flag_n = (reg_a & 0x80) == 0x80;
    flag_z = (reg_a == 0);
}
void CPU6502::XAA()
{
    reg_a = (uint8_t)(reg_x & M);
    flag_n = (reg_a & 0x80) != 0;
    flag_z = (reg_a & 0xFF) == 0;
}
void CPU6502::XAS()
{
    reg_sp.low = (uint8_t)(reg_a & reg_x /*& ((dummyVal >> 8) + 1)*/);
    write(reg_ea.value, reg_sp.low);
}

// Clock cpu one instruction !
void CPU6502::Clock()
{
    // First clock is to fetch opcode
    opcode = read(reg_pc.value);
    reg_pc.value++;

    switch (opcode)
    {
    case 0:
    {
        BRK();
        break;
    }
    case 0x1:
    {
        IndirectX_R();
        ORA();
        break;
    }
    case 0x2:
    {
        ImpliedAccum();// ILLEGAL ! set JAM.
        break;
    }
    case 0x3:
    {
        IndirectX_W();
        SLO();
        break;
    }
    case 0x4:
    {
        ZeroPage_R(); // ILLEGAL ! set DOP
        break;
    }
    case 0x5:
    {
        ZeroPage_R();
        ORA();
        break;
    }
    case 0x6:
    {
        ZeroPage_RW();
        ASL_M();
        break;
    }
    case 0x7:
    {
        ZeroPage_W();
        SLO();
        break;
    }
    case 0x8:
    {
        ImpliedAccum();
        PHP();
        break;
    }
    case 0x9:
    {
        Immediate();
        ORA();
        break;
    }
    case 0xA:
    {
        ImpliedAccum();
        ASL_A();
        break;
    }
    case 0xB:
    {
        Immediate();
        ANC();
        break;
    }
    case 0xC:
    {
        Absolute_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0xD:
    {
        Absolute_R();
        ORA();
        break;
    }
    case 0xE:
    {
        Absolute_RW();
        ASL_M();
        break;
    }
    case 0xF:
    {
        Absolute_W();
        SLO();
        break;
    }
    case 0x10:
    {
        Branch(!flag_n);
        break;
    }
    case 0x11:
    {
        IndirectY_R();
        ORA();
        break;
    }
    case 0x12:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0x13:
    {
        IndirectY_W();
        SLO();
        break;
    }
    case 0x14:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0x15:
    {
        ZeroPageX_R();
        ORA();
        break;
    }
    case 0x16:
    {
        ZeroPageX_RW();
        ASL_M();
        break;
    }
    case 0x17:
    {
        ZeroPageX_W();
        SLO();
        break;
    }
    case 0x18:
    {
        ImpliedAccum();
        flag_c = false;
        break;
    }
    case 0x19:
    {
        AbsoluteY_R();
        ORA();
        break;
    }
    case 0x1A:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0x1B:
    {
        AbsoluteY_W();
        SLO();
        break;
    }
    case 0x1C:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0x1D:
    {
        AbsoluteX_R();
        ORA();
        break;
    }
    case 0x1E:
    {
        AbsoluteX_RW();
        ASL_M();
        break;
    }
    case 0x1F:
    {
        AbsoluteX_W();
        SLO();
        break;
    }
    case 0x20:
    {
        JSR();
        break;
    }
    case 0x21:
    {
        IndirectX_R();
        AND();
        break;
    }
    case 0x22:
    {
        ImpliedAccum();// ILLEGAL ! set JAM.
        break;
    }
    case 0x23:
    {
        IndirectX_W();
        RLA();
        break;
    }
    case 0x24:
    {
        ZeroPage_R();
        BIT();
        break;
    }
    case 0x25:
    {
        ZeroPage_R();
        AND();
        break;
    }
    case 0x26:
    {
        ZeroPage_RW();
        ROL_M();
        break;
    }
    case 0x27:
    {
        ZeroPage_W();
        RLA();
        break;
    }
    case 0x28:
    {
        ImpliedAccum();
        PLP();
        break;
    }
    case 0x29:
    {
        Immediate();
        AND();
        break;
    }
    case 0x2A:
    {
        ImpliedAccum();
        ROL_A();
        break;
    }
    case 0x2B:
    {
        Immediate();
        ANC();
        break;
    }
    case 0x2C:
    {
        Absolute_R();
        BIT();
        break;
    }
    case 0x2D:
    {
        Absolute_R();
        AND();
        break;
    }
    case 0x2E:
    {
        Absolute_RW();
        ROL_M();
        break;
    }
    case 0x2F:
    {
        Absolute_W();
        RLA();
        break;
    }
    case 0x30:
    {
        Branch(flag_n);
        break;
    }
    case 0x31:
    {
        IndirectY_R();
        AND();
        break;
    }
    case 0x32:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0x33:
    {
        IndirectY_W();
        RLA();
        break;
    }
    case 0x34:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0x35:
    {
        ZeroPageX_R();
        AND();
        break;
    }
    case 0x36:
    {
        ZeroPageX_RW();
        ROL_M();
        break;
    }
    case 0x37:
    {
        ZeroPageX_W();
        RLA();
        break;
    }
    case 0x38:
    {
        ImpliedAccum();
        flag_c = true;
        break;
    }
    case 0x39:
    {
        AbsoluteY_R();
        AND();
        break;
    }
    case 0x3A:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0x3B:
    {
        AbsoluteY_W();
        RLA();
        break;
    }
    case 0x3C:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0x3D:
    {
        AbsoluteX_R();
        AND();
        break;
    }
    case 0x3E:
    {
        AbsoluteX_RW();
        ROL_M();
        break;
    }
    case 0x3F:
    {
        AbsoluteX_W();
        RLA();
        break;
    }
    case 0x40:
    {
        ImpliedAccum();
        RTI();
        break;
    }
    case 0x41:
    {
        IndirectX_R();
        EOR();
        break;
    }
    case 0x42:
    {
        ImpliedAccum();// ILLEGAL ! set JAM.
        break;
    }
    case 0x43:
    {
        IndirectX_W();
        SRE();
        break;
    }
    case 0x44:
    {
        ZeroPage_R(); // ILLEGAL ! set DOP
        break;
    }
    case 0x45:
    {
        ZeroPage_R();
        EOR();
        break;
    }
    case 0x46:
    {
        ZeroPage_RW();
        LSR_M();
        break;
    }
    case 0x47:
    {
        ZeroPage_W();
        SRE();
        break;
    }
    case 0x48:
    {
        ImpliedAccum();
        PHA();
        break;
    }
    case 0x49:
    {
        Immediate();
        EOR();
        break;
    }
    case 0x4A:
    {
        ImpliedAccum();
        LSR_A();
        break;
    }
    case 0x4B:
    {
        Immediate();
        ALR();
        break;
    }
    case 0x4C:
    {
        Absolute_W();
        reg_pc.value = reg_ea.value;/*JMP*/
        break;
    }
    case 0x4D:
    {
        Absolute_R();
        EOR();
        break;
    }
    case 0x4E:
    {
        Absolute_RW();
        LSR_M();
        break;
    }
    case 0x4F:
    {
        Absolute_W();
        SRE();
        break;
    }
    case 0x50:
    {
        Branch(!flag_v);
        break;
    }
    case 0x51:
    {
        IndirectY_R();
        EOR();
        break;
    }
    case 0x52:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0x53:
    {
        IndirectY_W();
        SRE();
        break;
    }
    case 0x54:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0x55:
    {
        ZeroPageX_R();
        EOR();
        break;
    }
    case 0x56:
    {
        ZeroPageX_RW();
        LSR_M();
        break;
    }
    case 0x57:
    {
        ZeroPageX_W();
        SRE();
        break;
    }
    case 0x58:
    {
        ImpliedAccum();
        flag_i = false;
        break;
    }
    case 0x59:
    {
        AbsoluteY_R();
        EOR();
        break;
    }
    case 0x5A:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0x5B:
    {
        AbsoluteY_W();
        SRE();
        break;
    }
    case 0x5C:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0x5D:
    {
        AbsoluteX_R();
        EOR();
        break;
    }
    case 0x5E:
    {
        AbsoluteX_RW();
        LSR_M();
        break;
    }
    case 0x5F:
    {
        AbsoluteX_W();
        SRE();
        break;
    }
    case 0x60:
    {
        ImpliedAccum();
        RTS();
        break;
    }
    case 0x61:
    {
        IndirectX_R();
        ADC();
        break;
    }
    case 0x62:
    {
        ImpliedAccum();// ILLEGAL ! set JAM.
        break;
    }
    case 0x63:
    {
        IndirectX_W();
        RRA();
        break;
    }
    case 0x64:
    {
        ZeroPage_R(); // ILLEGAL ! set DOP
        break;
    }
    case 0x65:
    {
        ZeroPage_R();
        ADC();
        break;
    }
    case 0x66:
    {
        ZeroPage_RW();
        ROR_M();
        break;
    }
    case 0x67:
    {
        ZeroPage_W();
        RRA();
        break;
    }
    case 0x68:
    {
        ImpliedAccum();
        PLA();
        break;
    }
    case 0x69:
    {
        Immediate();
        ADC();
        break;
    }
    case 0x6A:
    {
        ImpliedAccum();
        ROR_A();
        break;
    }
    case 0x6B:
    {
        Immediate();
        ARR();
        break;
    }
    case 0x6C:
    {
        JMP_I();
        break;
    }
    case 0x6D:
    {
        Absolute_R();
        ADC();
        break;
    }
    case 0x6E:
    {
        Absolute_RW();
        ROR_M();
        break;
    }
    case 0x6F:
    {
        Absolute_W();
        RRA();
        break;
    }
    case 0x70:
    {
        Branch(flag_v);
        break;
    }
    case 0x71:
    {
        IndirectY_R();
        ADC();
        break;
    }
    case 0x72:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0x73:
    {
        IndirectY_W();
        RRA();
        break;
    }
    case 0x74:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0x75:
    {
        ZeroPageX_R();
        ADC();
        break;
    }
    case 0x76:
    {
        ZeroPageX_RW();
        ROR_M();
        break;
    }
    case 0x77:
    {
        ZeroPageX_W();
        RRA();
        break;
    }
    case 0x78:
    {
        ImpliedAccum();
        flag_i = true;
        break;
    }
    case 0x79:
    {
        AbsoluteY_R();
        ADC();
        break;
    }
    case 0x7A:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0x7B:
    {
        AbsoluteY_W();
        RRA();
        break;
    }
    case 0x7C:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0x7D:
    {
        AbsoluteX_R();
        ADC();
        break;
    }
    case 0x7E:
    {
        AbsoluteX_RW();
        ROR_M();
        break;
    }
    case 0x7F:
    {
        AbsoluteX_W();
        RRA();
        break;
    }
    case 0x80:
    {
        Immediate(); // ILLEGAL ! set DOP
        break;
    }
    case 0x81:
    {
        IndirectX_W();
        STA();
        break;
    }
    case 0x82:
    {
        Immediate();// ILLEGAL ! set DOP.
        break;
    }
    case 0x83:
    {
        IndirectX_W();
        SAX();
        break;
    }
    case 0x84:
    {
        ZeroPage_W();
        STY();
        break;
    }
    case 0x85:
    {
        ZeroPage_W();
        STA();
        break;
    }
    case 0x86:
    {
        ZeroPage_W();
        STX();
        break;
    }
    case 0x87:
    {
        ZeroPage_W();
        SAX();
        break;
    }
    case 0x88:
    {
        ImpliedAccum();
        DEY();
        break;
    }
    case 0x89:
    {
        Immediate();// ILLEGAL ! set DOP
        break;
    }
    case 0x8A:
    {
        ImpliedAccum();
        TXA();
        break;
    }
    case 0x8B:
    {
        Immediate();
        XAA();
        break;
    }
    case 0x8C:
    {
        Absolute_W();
        STY();
        break;
    }
    case 0x8D:
    {
        Absolute_W();
        STA();
        break;
    }
    case 0x8E:
    {
        Absolute_W();
        STX();
        break;
    }
    case 0x8F:
    {
        Absolute_W();
        SAX();
        break;
    }
    case 0x90:
    {
        Branch(!flag_c);
        break;
    }
    case 0x91:
    {
        IndirectY_W();
        STA();
        break;
    }
    case 0x92:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0x93:
    {
        IndirectY_W();
        AHX();
        break;
    }
    case 0x94:
    {
        ZeroPageX_W();
        STY();
        break;
    }
    case 0x95:
    {
        ZeroPageX_W();
        STA();
        break;
    }
    case 0x96:
    {
        ZeroPageY_W();
        STX();
        break;
    }
    case 0x97:
    {
        ZeroPageY_W();
        SAX();
        break;
    }
    case 0x98:
    {
        ImpliedAccum();
        TYA();
        break;
    }
    case 0x99:
    {
        AbsoluteY_W();
        STA();
        break;
    }
    case 0x9A:
    {
        ImpliedAccum();
        TXS();
        break;
    }
    case 0x9B:
    {
        AbsoluteY_W();
        XAS();
        break;
    }
    case 0x9C:
    {
        Absolute_W();
        SHY(); // ILLEGAL ! SHY fits here.
        break;
    }
    case 0x9D:
    {
        AbsoluteX_W();
        STA();
        break;
    }
    case 0x9E:
    {
        Absolute_W();
        SHX();// ILLEGAL ! SHX fits here.
        break;
    }
    case 0x9F:
    {
        AbsoluteY_W();
        AHX();
        break;
    }
    case 0xA0:
    {
        Immediate();
        LDY();
        break;
    }
    case 0xA1:
    {
        IndirectX_R();
        LDA();
        break;
    }
    case 0xA2:
    {
        Immediate();
        LDX();
        break;
    }
    case 0xA3:
    {
        IndirectX_R();
        LAX();
        break;
    }
    case 0xA4:
    {
        ZeroPage_R();
        LDY();
        break;
    }
    case 0xA5:
    {
        ZeroPage_R();
        LDA();
        break;
    }
    case 0xA6:
    {
        ZeroPage_R();
        LDX();
        break;
    }
    case 0xA7:
    {
        ZeroPage_R();
        LAX();
        break;
    }
    case 0xA8:
    {
        ImpliedAccum();
        TAY();
        break;
    }
    case 0xA9:
    {
        Immediate();
        LDA();
        break;
    }
    case 0xAA:
    {
        ImpliedAccum();
        TAX();
        break;
    }
    case 0xAB:
    {
        Immediate();
        LAX();
        break;
    }
    case 0xAC:
    {
        Absolute_R();
        LDY();
        break;
    }
    case 0xAD:
    {
        Absolute_R();
        LDA();
        break;
    }
    case 0xAE:
    {
        Absolute_R();
        LDX();
        break;
    }
    case 0xAF:
    {
        Absolute_R();
        LAX();
        break;
    }
    case 0xB0:
    {
        Branch(flag_c);
        break;
    }
    case 0xB1:
    {
        IndirectY_R();
        LDA();
        break;
    }
    case 0xB2:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0xB3:
    {
        IndirectY_R();
        LAX();
        break;
    }
    case 0xB4:
    {
        ZeroPageX_R();
        LDY();
        break;
    }
    case 0xB5:
    {
        ZeroPageX_R();
        LDA();
        break;
    }
    case 0xB6:
    {
        ZeroPageY_R();
        LDX();
        break;
    }
    case 0xB7:
    {
        ZeroPageY_R();
        LAX();
        break;
    }
    case 0xB8:
    {
        ImpliedAccum();
        flag_v = false;
        break;
    }
    case 0xB9:
    {
        AbsoluteY_R();
        LDA();
        break;
    }
    case 0xBA:
    {
        ImpliedAccum();
        TSX();
        break;
    }
    case 0xBB:
    {
        AbsoluteY_R();
        LAR();
        break;
    }
    case 0xBC:
    {
        AbsoluteX_R();
        LDY();
        break;
    }
    case 0xBD:
    {
        AbsoluteX_R();
        LDA();
        break;
    }
    case 0xBE:
    {
        AbsoluteY_R();
        LDX();
        break;
    }
    case 0xBF:
    {
        AbsoluteY_R();
        LAX();
        break;
    }
    case 0xC0:
    {
        Immediate();
        CPY();
        break;
    }
    case 0xC1:
    {
        IndirectX_R();
        CMP();
        break;
    }
    case 0xC2:
    {
        Immediate(); // ILLEGAL ! set DOP.
        break;
    }
    case 0xC3:
    {
        IndirectX_R();
        DCP();
        break;
    }
    case 0xC4:
    {
        ZeroPage_R();
        CPY();
        break;
    }
    case 0xC5:
    {
        ZeroPage_R();
        CMP();
        break;
    }
    case 0xC6:
    {
        ZeroPage_RW();
        DEC();
        break;
    }
    case 0xC7:
    {
        ZeroPage_R();
        DCP();
        break;
    }
    case 0xC8:
    {
        ImpliedAccum();
        INY();
        break;
    }
    case 0xC9:
    {
        Immediate();
        CMP();
        break;
    }
    case 0xCA:
    {
        ImpliedAccum();
        DEX();
        break;
    }
    case 0xCB:
    {
        Immediate();
        AXS();
        break;
    }
    case 0xCC:
    {
        Absolute_R();
        CPY();
        break;
    }
    case 0xCD:
    {
        Absolute_R();
        CMP();
        break;
    }
    case 0xCE:
    {
        Absolute_RW();
        DEC();
        break;
    }
    case 0xCF:
    {
        Absolute_R();
        DCP();
        break;
    }
    case 0xD0:
    {
        Branch(!flag_z);
        break;
    }
    case 0xD1:
    {
        IndirectY_R();
        CMP();
        break;
    }
    case 0xD2:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0xD3:
    {
        IndirectY_RW();
        DCP();
        break;
    }
    case 0xD4:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0xD5:
    {
        ZeroPageX_R();
        CMP();
        break;
    }
    case 0xD6:
    {
        ZeroPageX_RW();
        DEC();
        break;
    }
    case 0xD7:
    {
        ZeroPageX_RW();
        DCP();
        break;
    }
    case 0xD8:
    {
        ImpliedAccum();
        flag_d = false;
        break;
    }
    case 0xD9:
    {
        AbsoluteY_R();
        CMP();
        break;
    }
    case 0xDA:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0xDB:
    {
        AbsoluteY_RW();
        DCP();
        break;
    }
    case 0xDC:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0xDD:
    {
        AbsoluteX_R();
        CMP();
        break;
    }
    case 0xDE:
    {
        AbsoluteX_RW();
        DEC();
        break;
    }
    case 0xDF:
    {
        AbsoluteX_RW();
        DCP();
        break;
    }
    case 0xE0:
    {
        Immediate();
        CPX();
        break;
    }
    case 0xE1:
    {
        IndirectX_R();
        SBC();
        break;
    }
    case 0xE2:
    {
        Immediate(); // ILLEGAL ! set DOP.
        break;
    }
    case 0xE3:
    {
        IndirectX_W();
        ISC();
        break;
    }
    case 0xE4:
    {
        ZeroPage_R();
        CPX();
        break;
    }
    case 0xE5:
    {
        ZeroPage_R();
        SBC();
        break;
    }
    case 0xE6:
    {
        ZeroPage_RW();
        INC();
        break;
    }
    case 0xE7:
    {
        ZeroPage_W();
        ISC();
        break;
    }
    case 0xE8:
    {
        ImpliedAccum();
        INX();
        break;
    }
    case 0xE9:
    {
        Immediate();
        SBC();
        break;
    }
    case 0xEA:
    {
        ImpliedAccum();// NOP ...
        break;
    }
    case 0xEB:
    {
        Immediate();
        SBC();
        break;
    }
    case 0xEC:
    {
        Absolute_R();
        CPX();
        break;
    }
    case 0xED:
    {
        Absolute_R();
        SBC();
        break;
    }
    case 0xEE:
    {
        Absolute_RW();
        INC();
        break;
    }
    case 0xEF:
    {
        Absolute_W();
        ISC();
        break;
    }
    case 0xF0:
    {
        Branch(flag_z);
        break;
    }
    case 0xF1:
    {
        IndirectY_R();
        SBC();
        break;
    }
    case 0xF2:
    {
        // ILLEGAL ! set JAM.
        break;
    }
    case 0xF3:
    {
        IndirectY_W();
        ISC();
        break;
    }
    case 0xF4:
    {
        ZeroPageX_R();// ILLEGAL ! set DOP
        break;
    }
    case 0xF5:
    {
        ZeroPageX_R();
        SBC();
        break;
    }
    case 0xF6:
    {
        ZeroPageX_RW();
        INC();
        break;
    }
    case 0xF7:
    {
        ZeroPageX_W();
        ISC();
        break;
    }
    case 0xF8:
    {
        ImpliedAccum();
        flag_d = true;
        break;
    }
    case 0xF9:
    {
        AbsoluteY_R();
        SBC();
        break;
    }
    case 0xFA:
    {
        ImpliedAccum();// LEGAL ! set NOP. (is NOP a legal instruction ?)
        break;
    }
    case 0xFB:
    {
        AbsoluteY_W();
        ISC();
        break;
    }
    case 0xFC:
    {
        AbsoluteX_R(); // ILLEGAL ! set TOP
        break;
    }
    case 0xFD:
    {
        AbsoluteX_R();
        SBC();
        break;
    }
    case 0xFE:
    {
        AbsoluteX_RW();
        INC();
        break;
    }
    case 0xFF:
    {
        AbsoluteX_W();
        ISC();
        break;
    }
    }

    // Handle interrupts...
    if (nmi_detect || irq_detected)
    {
        read(reg_pc.value);
        read(reg_pc.value);
        Interrupt();
    }
}

void CPU6502::GetStateAsBuffer(uint8_t* &buffer)
{
// This is gonna be tough ..
    std::vector<uint8_t> data;
// Set registers
    data.push_back(reg_pc.low);
    data.push_back(reg_pc.high);
    data.push_back(reg_sp.low);
    data.push_back(reg_sp.high);
    data.push_back(reg_ea.low);
    data.push_back(reg_ea.high);
    data.push_back(reg_a);
    data.push_back(reg_x);
    data.push_back(reg_y);
    data.push_back(flag_n?1:0);
    data.push_back(flag_v?1:0);
    data.push_back(flag_d?1:0);
    data.push_back(flag_i?1:0);
    data.push_back(flag_z?1:0);
    data.push_back(flag_c?1:0);
    data.push_back(opcode);
    data.push_back(M);
    data.push_back(nmi_detect?1:0);
    data.push_back(irq_detected?1:0);
    data.push_back(interrupt_vector&0x00FF);
    data.push_back((interrupt_vector&0xFF00)>>8);
    data.push_back(interrupt_suspend_nmi?1:0);
    data.push_back(interrupt_suspend_irq?1:0);
    data.push_back(is_brk?1:0);
    data.push_back(byte_temp);
    data.push_back(dummy);
    data.push_back(int_temp & 0x000000FF);
    data.push_back((int_temp & 0x0000FF00)>>8);
    data.push_back((int_temp & 0x00FF0000)>>16);
    data.push_back((int_temp & 0xFF000000)>>24);
    data.push_back(int_temp1 & 0x000000FF);
    data.push_back((int_temp1 & 0x0000FF00)>>8);
    data.push_back((int_temp1 & 0x00FF0000)>>16);
    data.push_back((int_temp1 & 0xFF000000)>>24);

    // return (uint8_t*)&data[0];
    for(int i=0; i< (int)data.size(); i++)
    {
        buffer[i] = data[i];
    }
}
// Set state from buffer
void CPU6502::SetStateFromBufer(uint8_t* &buffer)
{
    int pos =0;
    reg_pc.low=buffer[pos];
    pos++;
    reg_pc.high=buffer[pos];
    pos++;
    reg_sp.low=buffer[pos];
    pos++;
    reg_sp.high=buffer[pos];
    pos++;
    reg_ea.low=buffer[pos];
    pos++;
    reg_ea.high=buffer[pos];
    pos++;
    reg_a=buffer[pos];
    pos++;
    reg_x=buffer[pos];
    pos++;
    reg_y=buffer[pos];
    pos++;
    flag_n=buffer[pos]==1;
    pos++;
    flag_v=buffer[pos]==1;
    pos++;
    flag_d=buffer[pos]==1;
    pos++;
    flag_i=buffer[pos]==1;
    pos++;
    flag_z=buffer[pos]==1;
    pos++;
    flag_c=buffer[pos]==1;
    pos++;
    opcode=buffer[pos];
    pos++;
    M=buffer[pos];
    pos++;
    nmi_detect=buffer[pos]==1;
    pos++;
    irq_detected=buffer[pos]==1;
    pos++;
    interrupt_vector = (interrupt_vector&0xFF00) | buffer[pos];
    pos++;
    interrupt_vector = (interrupt_vector&0x00FF) | (buffer[pos]<<8);
    pos++;
    interrupt_suspend_nmi=buffer[pos]==1;
    pos++;
    interrupt_suspend_irq=buffer[pos]==1;
    pos++;
    is_brk=buffer[pos]==1;
    pos++;
    byte_temp=buffer[pos];
    pos++;
    dummy=buffer[pos];
    pos++;
    int_temp = (int_temp & 0xFFFFFF00) | buffer[pos];
    pos++;
    int_temp = (int_temp & 0xFFFF00FF) | (buffer[pos]<<8);
    pos++;
    int_temp = (int_temp & 0xFF00FFFF) | (buffer[pos]<<16);
    pos++;
    int_temp = (int_temp & 0x00FFFFFF) | (buffer[pos]<<24);
    pos++;
    int_temp1 = (int_temp1 & 0xFFFFFF00) | buffer[pos];
    pos++;
    int_temp1 = (int_temp1 & 0xFFFF00FF) | (buffer[pos]<<8);
    pos++;
    int_temp1 = (int_temp1 & 0xFF00FFFF) | (buffer[pos]<<16);
    pos++;
    int_temp1 = (int_temp1 & 0x00FFFFFF) | (buffer[pos]<<24);
    pos++;
}
}
