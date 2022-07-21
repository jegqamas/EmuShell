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
#include "../../Definations.h"
#include "../../Core.h"

namespace NES
{
const uint32_t Mapper005::sound_seq[7] = { // 240Hz
    7459, 7456, 7458, 7457, 1, 1, 7457
};
// TODO: Split Screen is not implemented on mapper 5.
Mapper005::Mapper005()
{
    this->Name = "MMC5";
    this->MapperNumber = 5;
    // We need to set these for default configuration
    this->prg_ram_default_08KB_count = 8;
    this->chr_ram_1KB_default_banks_count=16;

    InitializeSoundMixTable();
    channel_sq1 = new MMC5ChannelPulse();
    channel_sq2 = new MMC5ChannelPulse();
    channel_pcm = new MMC5ChannelPCM();
}
Mapper005::~Mapper005()
{
    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            delete [] mix_table[sq1][sq2];
        }
        delete []  mix_table[sq1];
    }
    delete [] mix_table;
    delete [] channel_sq1;
    delete [] channel_sq2;
    delete [] channel_pcm;
}
void Mapper005::HardReset()
{
    Board::HardReset();
    ram_protectA=0;
    ram_protectB=0;
    ExRAM_mode=0;
    for (int i=0; i<8; i++)
        CHROffset_spr[i]=0;
    for (int i=0; i<8; i++)
        CHROffsetEX[i]=0;
    for (int i=0; i<8; i++)
        CHROffsetSP[i]=0;
    for (int i=0; i<8; i++)
        chrRegA[i]=0;
    for (int i=0; i<4; i++)
        chrRegB[i]=0;
    for (int i=0; i<4; i++)
        prgReg[i]=0;
    useSRAMmirroring=false;
    chr_high=0;
    chr_mode=0;
    chr_setB_last=0;
    temp_val=0;
    temp_fill=0;
    lastAccessVRAM=0;
    paletteNo=0;
    shift=0;
    EXtilenumber=0;
    multiplicand=0;
    multiplier=0;
    product=0;
    split_enable=false;
    split_right=false;
    split_tile=0;
    split_yscroll=0;// The y scroll value for split.
    split_doit=false;// Set to true to make nt changes; get split happening. Used in CHR read access.
    split_watch_tile=0;// A temp tile number, for the split.
    irq_line=0;
    irq_enable=0;
    irq_pending=0;
    irq_current_counter=0;
    irq_current_inframe=0;

    useSRAMmirroring=false;

    // This is not a hack, "Uncharted Waters" title actually use 2 chips of SRAM which depends on bit 2 of
    // $5113 register instead of first 2 bits for switching.
    // There's no other way to make switching right.
    switch (EmuShell::GameCRC32)
    {
    // Uncharted Waters
    case 0xACA15643:
    // Daikoukai Jidai (J)
    case 0xFE3488D1:
        useSRAMmirroring = true;
        break;
    }

    std::cout << "MMC5: using PRG RAM mirroring = " + (std::string)(useSRAMmirroring?"yes":"no")<<"\n";
    dev_apu->UseExternalSoundChannels=true;

    prg_mode = 3;
    prgReg[3] = prg_08K_rom_mask;
    Switch08KPRG(prg_08K_rom_mask, 0x8000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xA000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xC000, true);
    Switch08KPRG(prg_08K_rom_mask, 0xE000, true);

    Switch04kCHREX(0, 0);
    Switch04kCHRSP(0, 0);
    Switch08kCHR_spr(0);

    TogglePRGRAMWritable(true);
    TogglePRGRAMEnable(true);

    channel_sq1->HardReset();
    channel_sq2->HardReset();
    channel_pcm->HardReset();

    sound_seq_curr = 0;
    sound_seq_cyc += sound_seq[sound_seq_curr];

    std::cout << "MMC5 is ready !\n";
}
void Mapper005::SoftReset()
{
    Board::SoftReset();
    channel_sq1->SoftReset();
    channel_sq2->SoftReset();
    channel_pcm->SoftReset();
}
void Mapper005::WriteEXP(uint16_t& address, uint8_t& value)
{
    if (address >= 0x5C00)
    {
        if (ExRAM_mode == 2)// Only EX2 is writable.
            nmt_banks[2][address & 0x3FF] = value;
        else if (ExRAM_mode < 2)
        {
            if (irq_current_inframe == 0x40)
                nmt_banks[2][address & 0x3FF] = value;
            else
                nmt_banks[2][address & 0x3FF] = 0;
        }
        return;
    }
    switch (address)
    {
    // Sound Channels
    case 0x5000:
        channel_sq1->Write5000(value);
        break;
    case 0x5002:
        channel_sq1->Write5002(value);
        break;
    case 0x5003:
        channel_sq1->Write5003(value);
        break;
    case 0x5004:
        channel_sq2->Write5000(value);
        break;
    case 0x5006:
        channel_sq2->Write5002(value);
        break;
    case 0x5007:
        channel_sq2->Write5003(value);
        break;
    case 0x5010:
        channel_pcm->Write5010(value);
        break;
    case 0x5011:
        channel_pcm->Write5011(value);
        break;
    case 0x5015:
    {
        channel_sq1->SetEnable((value & 0x1) != 0);
        channel_sq1->SetEnable((value & 0x2) != 0);
        break;
    }

    case 0x5100:
        prg_mode = value & 0x3;
        break;
    case 0x5101:
        chr_mode = value & 0x3;
        break;
    case 0x5102:
        ram_protectA = value & 0x3;
        UpdateRamProtect();
        break;
    case 0x5103:
        ram_protectB = value & 0x3;
        UpdateRamProtect();
        break;
    case 0x5104:
        ExRAM_mode = value & 0x3;
        break;
    case 0x5105:
        SwitchNMT(value);
        break;
    // PRG
    case 0x5113:
    {
        if (!useSRAMmirroring)
            Switch08KPRG(value & 0x7, 0x6000, false);
        else// Use chip switching (bit 2)...
            Switch08KPRG((value >> 2) & 1, 0x6000, false);
        break;
    }
    case 0x5114:
    {
        if (prg_mode == 3)
            Switch08KPRG(value & 0x7F, 0x8000, (value & 0x80) == 0x80);
        break;
    }
    case 0x5115:
    {
        switch (prg_mode)
        {
        case 1:
            Switch16KPRG((value & 0x7F) >> 1, 0x8000, (value & 0x80) == 0x80);
            break;
        case 2:
            Switch16KPRG((value & 0x7F) >> 1, 0x8000, (value & 0x80) == 0x80);
            break;
        case 3:
            Switch08KPRG(value & 0x7F, 0xA000, (value & 0x80) == 0x80);
            break;
        }
        break;
    }
    case 0x5116:
    {
        switch (prg_mode)
        {
        case 2:
        case 3:
            Switch08KPRG(value & 0x7F, 0xC000, (value & 0x80) == 0x80);
            break;
        }
        break;
    }
    case 0x5117:
    {
        switch (prg_mode)
        {
        case 0:
            Switch32KPRG((value & 0x7C) >> 2, true);
            break;
        case 1:
            Switch16KPRG((value & 0x7F) >> 1, 0xC000, true);
            break;
        case 2:
            Switch08KPRG(value & 0x7F, 0xE000, true);
            break;
        case 3:
            Switch08KPRG(value & 0x7F, 0xE000, true);
            break;
        }
        break;
    }
    // CHR
    // SPR SET
    case 0x5120:
    {
        chr_setB_last = false;
        if (chr_mode == 3)
            Switch01kCHR_spr(value | chr_high, 0);
        break;
    }
    case 0x5121:
    {
        chr_setB_last = false;
        switch (chr_mode)
        {
        case 2:
            Switch02kCHR_spr(value | chr_high, 0);
            break;
        case 3:
            Switch01kCHR_spr(value | chr_high, 0x400);
            break;
        }
        break;
    }
    case 0x5122:
    {
        chr_setB_last = false;
        if (chr_mode == 3)
            Switch01kCHR_spr(value | chr_high, 0x800);
        break;
    }
    case 0x5123:
    {
        chr_setB_last = false;
        switch (chr_mode)
        {
        case 1:
            Switch04kCHR_spr(value | chr_high, 0);
            break;
        case 2:
            Switch02kCHR_spr(value | chr_high, 0x800);
            break;
        case 3:
            Switch01kCHR_spr(value | chr_high, 0xC00);
            break;
        }
        break;
    }
    case 0x5124:
    {
        chr_setB_last = false;
        if (chr_mode == 3)
            Switch01kCHR_spr(value | chr_high, 0x1000);
        break;
    }
    case 0x5125:
    {
        chr_setB_last = false;
        switch (chr_mode)
        {
        case 2:
            Switch02kCHR_spr(value | chr_high, 0x1000);
            break;
        case 3:
            Switch01kCHR_spr(value | chr_high, 0x1400);
            break;
        }
        break;
    }
    case 0x5126:
    {
        chr_setB_last = false;
        if (chr_mode == 3)
            Switch01kCHR_spr(value | chr_high, 0x1800);
        break;
    }
    case 0x5127:
    {
        chr_setB_last = false;
        switch (chr_mode)
        {
        case 0:
            Switch08kCHR_spr(value | chr_high);
            break;
        case 1:
            Switch04kCHR_spr(value | chr_high, 0x1000);
            break;
        case 2:
            Switch02kCHR_spr(value | chr_high, 0x1800);
            break;
        case 3:
            Switch01kCHR_spr(value | chr_high, 0x1C00);
            break;
        }
        break;
    }
    // BKG SET
    case 0x5128:
    {
        chr_setB_last = true;
        if (chr_mode == 3)
        {
            Switch01KCHR(value | chr_high, 0, chr_01K_rom_count > 0);
            Switch01KCHR(value | chr_high, 0x1000, chr_01K_rom_count > 0);
        }
        break;
    }
    case 0x5129:
    {
        chr_setB_last = true;
        switch (chr_mode)
        {
        case 2:
        {
            Switch02KCHR(value | chr_high, 0, chr_01K_rom_count > 0);
            Switch02KCHR(value | chr_high, 0x1000, chr_01K_rom_count > 0);
            break;
        }
        case 3:
        {
            Switch01KCHR(value | chr_high, 0x400, chr_01K_rom_count > 0);
            Switch01KCHR(value | chr_high, 0x1400, chr_01K_rom_count > 0);
            break;
        }
        }
        break;
    }
    case 0x512A:
    {
        chr_setB_last = true;
        if (chr_mode == 3)
        {
            Switch01KCHR(value | chr_high, 0x800, chr_01K_rom_count > 0);
            Switch01KCHR(value | chr_high, 0x1800, chr_01K_rom_count > 0);
        }
        break;
    }
    case 0x512B:
    {
        chr_setB_last = true;
        switch (chr_mode)
        {
        case 0:
        {
            Switch04kCHR_bkg((value | chr_high), 0);
            Switch04kCHR_bkg((value | chr_high), 0x1000);
            break;
        }
        case 1:
        {
            Switch04KCHR(value | chr_high, 0, chr_01K_rom_count > 0);
            Switch04KCHR(value | chr_high, 0x1000, chr_01K_rom_count > 0);
            break;
        }
        case 2:
        {
            Switch02KCHR(value | chr_high, 0x800, chr_01K_rom_count > 0);
            Switch02KCHR(value | chr_high, 0x1800, chr_01K_rom_count > 0);
            break;
        }
        case 3:
        {
            Switch01KCHR(value | chr_high, 0xC00, chr_01K_rom_count > 0);
            Switch01KCHR(value | chr_high, 0x1C00, chr_01K_rom_count > 0);
            break;
        }
        }
        break;
    }
    case 0x5130:
    {
        chr_high = (value & 0x3) << 8;
        break;
    }
    //Fill-mode tile
    case 0x5106:
        for (int i = 0; i < 0x3C0; i++)
            nmt_banks[3][i] = value;
        break;
    //Fill-mode attr
    case 0x5107:
        for (int i = 0x3C0; i < (0x3C0 + 0x40); i++)
        {
            temp_fill = (uint8_t)((2 << (value & 0x03)) | (value & 0x03));
            temp_fill |= (uint8_t)((temp_fill & 0x0F) << 4);
            nmt_banks[3][i] = temp_fill;
        }
        break;
    case 0x5200:
    {
        split_tile = value & 0x1F;
        split_enable = (value & 0x80) == 0x80;
        split_right = (value & 0x40) == 0x40;
        break;
    }
    case 0x5201:
    {
        split_yscroll = value;
        break;
    }
    case 0x5202:
    {
        Switch04kCHRSP(value, address & 0x0000);
        Switch04kCHRSP(value, address & 0x1000);
        break;
    }
    case 0x5203:
        irq_line = value;
        break;
    case 0x5204:
        irq_enable = value;
        break;
    case 0x5205:
        multiplicand = value;
        product = (uint16_t)(multiplicand * multiplier);
        break;
    case 0x5206:
        multiplier = value;
        product = (uint16_t)(multiplicand * multiplier);
        break;
    }
}

uint8_t Mapper005::ReadEXP(uint16_t& address)
{
    if (address >= 0x5C00)
    {
        if (ExRAM_mode >= 2)
            return nmt_banks[2][address & 0x3FF];
    }
    switch (address)
    {
    case 0x5010:
        return channel_pcm->Read5010();
    case 0x5204:
    {
        temp_val = (uint8_t)(irq_current_inframe | irq_pending);
        irq_pending = 0;
        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
        return temp_val;
    }
    case 0x5205:
        return (uint8_t)(product & 0xFF);
    case 0x5206:
        return (uint8_t)((product & 0xFF00) >> 8);
    case 0x5015:
        return (uint8_t)((channel_sq1->IsEnabled() ? 0x1 : 0) | (channel_sq2->IsEnabled()  ? 0x2 : 0));
    }
    return 0;
}
uint8_t Mapper005::ReadCHR(uint16_t& address, bool spriteFetch)
{
    if (!spriteFetch)
    {
        if (split_enable)
        {
            if (ExRAM_mode < 2)// Split screen mode is only allowed in Ex0 or Ex1
            {
                split_watch_tile = address & 0x3FF / 16;
                if (!split_right)// Left
                    split_doit = split_watch_tile < split_tile;// Tiles 0 to T-1 are the split.
                else// Right
                    split_doit = split_watch_tile >= split_tile;// Tiles 0 to T-1 are rendered normally. Tiles T and on are the split.
                if (split_doit)
                {
                    //  return CHR[((address & 0x03FF) | CHROffsetSP[address >> 10 & 0x07]) & CHRMaxSizeInBytesMask];
                }
            }
        }
    }
    if (ExRAM_mode == 1)// Extended Attribute mode
    {
        if (!spriteFetch)
        {
            EXtilenumber = nmt_banks[2][lastAccessVRAM] & 0x3F;
            Switch04kCHREX(EXtilenumber | chr_high, address & 0x1000);
            return chr_banks[CHROffsetEX[(address >> 10) & 0x7]][address & 0x03FF];
        }
        else// Sprites not effected
        {
            return chr_banks[CHROffset_spr[(address >> 10) & 0x7]][address & 0x03FF];
        }
    }
    else
    {
        if (dev_ppu->IsSpriteSize16())
        {
            // When in 8x16 sprite mode, both sets of registers are used.
            // The 'A' set is used for sprite tiles, and the 'B' set is used for BG.
            if (!spriteFetch)
                return chr_banks[chr_indexes[(address >> 10) & 0x7]][address & 0x03FF];
            else
                return chr_banks[CHROffset_spr[(address >> 10) & 0x7]][address & 0x03FF];
        }
        else
        {
            // When in 8x8 sprite mode, only one set is used for both BG and sprites.
            // Either 'A' or 'B', depending on which set is written to last
            if (chr_setB_last)
                return chr_banks[chr_indexes[(address >> 10) & 0x7]][address & 0x03FF];
            else
                return chr_banks[CHROffset_spr[(address >> 10) & 0x7]][address & 0x03FF];
        }
    }
}
uint8_t Mapper005::ReadNMT(uint16_t& address)
{
    /*
     *  %00 = Extra Nametable mode    ("Ex0")
        %01 = Extended Attribute mode ("Ex1")
        %10 = CPU access mode         ("Ex2")
        %11 = CPU read-only mode      ("Ex3")

     * NT Values can be the following:
          %00 = NES internal NTA
          %01 = NES internal NTB
          %10 = use ExRAM as NT
          %11 = Fill Mode
     */
    if (split_doit)
    {
        // ExRAM is always used as the nametable in split screen mode.
        // return base.NMT[2][address & 0x03FF];
    }
    if (ExRAM_mode == 1)// Extended Attribute mode
    {
        if ((address & 0x03FF) <= 0x3BF)
        {
            lastAccessVRAM = address & 0x03FF;
        }
        else
        {
            paletteNo = nmt_banks[2][lastAccessVRAM] & 0xC0;
            // Fix Attribute bits
            shift = ((lastAccessVRAM >> 4 & 0x04) | (lastAccessVRAM & 0x02));
            switch (shift)
            {
            case 0:
                return (paletteNo >> 6) ;
            case 2:
                return (paletteNo >> 4) ;
            case 4:
                return (paletteNo >> 2) ;
            case 6:
                return (paletteNo >> 0) ;
            }
        }
    }
    return nmt_banks[nmt_indexes[(address >> 10) & 0x03]][address & 0x03FF];// Reached here in some cases above.
}
void Mapper005::WriteNMT(uint16_t& address, uint8_t& value)
{
    if (ExRAM_mode == 1)
    {
        if ((address & 0x03FF) <= 0x3BF)
        {
            lastAccessVRAM = address & 0x03FF;
        }
    }
    nmt_banks[nmt_indexes[(address >> 10) & 0x03]][address & 0x03FF] = value;
}
void Mapper005::OnPPUScanlineTick()
{
    // In frame signal
    irq_current_inframe = (dev_ppu->IsInRender() && dev_ppu->IsRenderingOn()) ? 0x40 : 0x00;
    if (irq_current_inframe == 0)
    {
        irq_current_inframe = 0x40;
        irq_current_counter = 0;
        irq_pending = 0;

        dev_cpu->pin_irq &= IRQ_FLAG_BOARD_OFF;
    }
    else
    {
        irq_current_counter++;
        if (irq_current_counter == irq_line)
        {
            irq_pending = 0x80;// IRQ pending flag is raised *regardless* of whether or not IRQs are enabled.
            if (irq_enable == 0x80)// Trigger an IRQ on the 6502 if both this flag *and* the IRQ enable flag is set.
                dev_cpu->pin_irq |= IRQ_FLAG_BOARD;
        }
    }
}
void Mapper005::InitializeSoundMixTable()
{
    mix_table = new double**[16];

    for (int sq1 = 0; sq1 < 16; sq1++)
    {
        mix_table[sq1] = new double*[16];

        for (int sq2 = 0; sq2 < 16; sq2++)
        {
            mix_table[sq1][sq2] = new double[256];

            for (int dmc = 0; dmc < 256; dmc++)
            {
                double sqr = (95.88 / (8128.0 / (sq1 + sq2) + 100));
                double tnd = (159.79 / (1.0 / (dmc / 22638.0) + 100));

                mix_table[sq1][sq2][dmc] = (sqr + tnd);
            }
        }
    }
}
void Mapper005::UpdateRamProtect()
{
    TogglePRGRAMWritable((ram_protectA == 0x2) && (ram_protectB == 0x1));
}
void Mapper005::Switch04kCHR_bkg(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    chr_indexes[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch01kCHR_spr(int index, int where)
{
    CHROffset_spr[(where >> 10) & 0x07] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch02kCHR_spr(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 1;

    CHROffset_spr[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[area + 1] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch04kCHR_spr(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffset_spr[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffset_spr[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffset_spr[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffset_spr[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch08kCHR_spr(int index)
{
    index <<= 3;
    CHROffset_spr[0] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[1] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[2] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[3] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[4] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[5] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[6] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    index++;
    CHROffset_spr[7] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch04kCHREX(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffsetEX[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetEX[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetEX[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetEX[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::Switch04kCHRSP(int index, int where)
{
    int area = (where >> 10) & 0x07;
    index <<= 2;

    CHROffsetSP[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetSP[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetSP[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
    area++;
    index++;
    CHROffsetSP[area] = (index & chr_01K_rom_mask) + chr_rom_bank_offset;
}
void Mapper005::ClockExternalSoundChannelsSingle(bool& is_clocking_duration)
{
    sound_seq_cyc--;
    if (sound_seq_cyc == 0)
    {
        switch (sound_seq_curr)
        {
        case 0:
        case 1:
        case 2:
        case 4:
        {
            channel_sq1->ClockEnvelope();
            channel_sq2->ClockEnvelope();
            break;
        }
        }
        sound_seq_curr++;
        sound_seq_cyc += sound_seq[sound_seq_curr];
        if (sound_seq_curr == 6)
            sound_seq_curr = 0;
    }
    channel_sq1->Clock(is_clocking_duration);
    channel_sq2->Clock(is_clocking_duration);
}
double Mapper005::GetExternalSoundChannelsOutput()
{
    return mix_table[channel_sq1->Output]
           [channel_sq2->Output]
           [channel_pcm->output];
}

void Mapper005::SaveState()
{
    Board::SaveState();
    state_str->Write(ram_protectA);
    state_str->Write(ram_protectB);
    state_str->Write(ExRAM_mode);
    for (int i = 0; i < 8; i++)
        state_str->Write(CHROffset_spr[i]);
    for (int i = 0; i < 8; i++)
        state_str->Write(CHROffsetEX[i]);
    for (int i = 0; i < 8; i++)
        state_str->Write(CHROffsetSP[i]);
    for (int i = 0; i < 8; i++)
        state_str->Write(chrRegA[i]);
    for (int i = 0; i < 4; i++)
        state_str->Write(chrRegB[i]);
    for (int i = 0; i < 4; i++)
        state_str->Write(prgReg[i]);
    state_str->Write(useSRAMmirroring);
    state_str->Write(chr_high);
    state_str->Write(chr_mode);
    state_str->Write(prg_mode);
    state_str->Write(chr_setB_last);
    state_str->Write(temp_val);
    state_str->Write(temp_fill);
    state_str->Write(lastAccessVRAM);
    state_str->Write(paletteNo);
    state_str->Write(shift);
    state_str->Write(EXtilenumber);
    state_str->Write(multiplicand);
    state_str->Write(multiplier);
    state_str->Write(product);
    state_str->Write(split_enable);
    state_str->Write(split_right);
    state_str->Write(split_tile);
    state_str->Write(split_yscroll);
    state_str->Write(split_doit);
    state_str->Write(split_watch_tile);
    state_str->Write(irq_line);
    state_str->Write(irq_enable);
    state_str->Write(irq_pending);
    state_str->Write(irq_current_counter);
    state_str->Write(irq_current_inframe);
    state_str->Write(sound_seq_curr);
    state_str->Write(sound_seq_cyc);
    channel_sq1->SaveState();
    channel_sq2->SaveState();
    channel_pcm->SaveState();
}
void Mapper005::LoadState()
{
    Board::LoadState();
    state_str->Read(ram_protectA);
    state_str->Read(ram_protectB);
    state_str->Read(ExRAM_mode);
    for (int i = 0; i < 8; i++)
        state_str->Read(CHROffset_spr[i]);
    for (int i = 0; i < 8; i++)
        state_str->Read(CHROffsetEX[i]);
    for (int i = 0; i < 8; i++)
        state_str->Read(CHROffsetSP[i]);
    for (int i = 0; i < 8; i++)
        state_str->Read(chrRegA[i]);
    for (int i = 0; i < 4; i++)
        state_str->Read(chrRegB[i]);
    for (int i = 0; i < 4; i++)
        state_str->Read(prgReg[i]);
    state_str->Read(useSRAMmirroring);
    state_str->Read(chr_high);
    state_str->Read(chr_mode);
    state_str->Read(prg_mode);
    state_str->Read(chr_setB_last);
    state_str->Read(temp_val);
    state_str->Read(temp_fill);
    state_str->Read(lastAccessVRAM);
    state_str->Read(paletteNo);
    state_str->Read(shift);
    state_str->Read(EXtilenumber);
    state_str->Read(multiplicand);
    state_str->Read(multiplier);
    state_str->Read(product);
    state_str->Read(split_enable);
    state_str->Read(split_right);
    state_str->Read(split_tile);
    state_str->Read(split_yscroll);
    state_str->Read(split_doit);
    state_str->Read(split_watch_tile);
    state_str->Read(irq_line);
    state_str->Read(irq_enable);
    state_str->Read(irq_pending);
    state_str->Read(irq_current_counter);
    state_str->Read(irq_current_inframe);
    state_str->Read(sound_seq_curr);
    state_str->Read(sound_seq_cyc);
    channel_sq1->LoadState();
    channel_sq2->LoadState();
    channel_pcm->LoadState();
}
}
