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
#include "Registers.h"
#include <stdint.h>

namespace Common
{
// Class emulates the 6502 cpu.
class CPU6502
{
public:
    CPU6502();
    ~CPU6502();
    // Initialize the cpu
    void Initialize(void(*writeAccess)(uint16_t address, uint8_t value), uint8_t(*readAccess)(uint16_t address));
    // Clock cpu one instruction
    void Clock();
    // Hard reset the cpu
    void HardReset();
    // Soft reset the cpu
    void SoftReset();
    // Shutdown the cpu
    void ShutDown();
    // Get a buffer contain current state
    void GetStateAsBuffer(uint8_t* &buffer);
    // Set state from buffer
    void SetStateFromBufer(uint8_t* &buffer);
    // The state buffer length
    int state_buffer_length;
    // Force the cpu to check interrupt status and flags.
    void PollInterruptStatus();
    // The NMI pin.
    bool pin_nmi;
    // The IRQ pins (each bit represent an IRQ pin or flag).
    uint8_t pin_irq;

private:
    // Represents an unsigned 16-bit register.
    Register16 reg_pc, reg_sp, reg_ea;// PROGRAM COUNTER, STACK POINTER,
    uint8_t reg_a, reg_x, reg_y;// ACCUMULATOR, X INDEX REGISTER, Y INDEX REGISTER
    bool flag_n, flag_v, flag_d, flag_i, flag_z, flag_c; // FLAGS

    // Helper values
    uint8_t opcode;// Current opcode
    uint8_t M;// Value read with some addressing modes.
    bool nmi_detect;// Indicates that NMI is detected and should be handled.
    bool irq_detected;// Indicates that IRQ is detected and should be handled.
    uint16_t interrupt_vector;// This is the interrupt vector to jump in the last 2 cycles of BRK/IRQ/NMI
    bool interrupt_suspend_nmi;// If set, NMI can't be detected if asserted.
    bool interrupt_suspend_irq;// If set, IRQ can't be detected if asserted.

    // These temps may increase performance by avoiding memory allocation and copying.
    uint8_t byte_temp;
    int int_temp;
    int int_temp1;
    uint8_t dummy;
    bool is_brk;

    // Pointers for memory access
    uint8_t(*read)(uint16_t address);
    void(*write)(uint16_t address, uint8_t value);

    // Methods
    inline uint8_t p_get();
    inline uint8_t pb_get();
    inline void p_set(uint8_t value);

    inline void Branch(bool condition);
    inline void Push(uint8_t val);
    inline uint8_t Pull();
    inline void Interrupt();

    /*
    * Addressing modes...
    * _R: read access instructions, set the M value. Some addressing modes will execute 1 extra cycle when page crossed.
    * _W: write access instructions, doesn't set the M value. The instruction should handle write at effective address (EF).
    * _RW: NES::MemRead-Modify-NES::MemWrite instructions, set the M value and the instruction should handle write at effective address (EF).
    */
    inline void IndirectX_R();
    inline void IndirectX_W();
    inline void IndirectX_RW();
    inline void IndirectY_R();
    inline void IndirectY_W();
    inline void IndirectY_RW();
    inline void ZeroPage_R();
    inline void ZeroPage_W();
    inline void ZeroPage_RW();
    inline void ZeroPageX_R();
    inline void ZeroPageX_W();
    inline void ZeroPageX_RW();
    inline void ZeroPageY_R();
    inline void ZeroPageY_W();
    inline void ZeroPageY_RW();
    inline void Immediate();
    inline void ImpliedAccum();
    inline void Absolute_R();
    inline void Absolute_W();
    inline void Absolute_RW();
    inline void AbsoluteX_R();
    inline void AbsoluteX_W();
    inline void AbsoluteX_RW();
    inline void AbsoluteY_R();
    inline void AbsoluteY_W();
    inline void AbsoluteY_RW();

    /*
    * Instructions...
    */
    inline void ADC();
    inline void AHX();
    inline void ALR();
    inline void ANC();
    inline void AND();
    inline void ARR();
    inline void AXS();
    inline void ASL_M();
    inline void ASL_A();
    inline void BIT();
    inline void BRK();
    inline void CMP();
    inline void CPX();
    inline void CPY();
    inline void DCP();
    inline void DEC();
    inline void DEY();
    inline void DEX();
    inline void EOR();
    inline void INC();
    inline void INX();
    inline void INY();
    inline void ISC();
    inline void JMP_I();
    inline void JSR();
    inline void LAR();
    inline void LAX();
    inline void LDA();
    inline void LDX();
    inline void LDY();
    inline void LSR_A();
    inline void LSR_M();
    inline void ORA();
    inline void PHA();
    inline void PHP();
    inline void PLA();
    inline void PLP();
    inline void RLA();
    inline void ROL_A();
    inline void ROL_M();
    inline void ROR_A();
    inline void ROR_M();
    inline void RRA();
    inline void RTI();
    inline void RTS();
    inline void SAX();
    inline void SBC();
    inline void SHX();
    inline void SHY();
    inline void SLO();
    inline void SRE();
    inline void STA();
    inline void STX();
    inline void STY();
    inline void TAX();
    inline void TAY();
    inline void TSX();
    inline void TXA();
    inline void TXS();
    inline void TYA();
    inline void XAA();
    inline void XAS();
};
}
