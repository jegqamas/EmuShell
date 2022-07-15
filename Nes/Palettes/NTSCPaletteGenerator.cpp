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

#include "NTSCPaletteGenerator.h"
#include "../../Common/ConfigHandler.h"
#include <math.h>

namespace NES
{
namespace NTSCPaletteGenerator
{
// Voltage levels, relative to synch voltage
float black;
float white ;
float attenuation ;

float saturation;
float hue_tweak ;
float contrast ;
float brightness ;
float gamma;

const float levels[8] =  {
    0.350F, 0.518F, 0.962F, 1.550F, // Signal low
    1.094F, 1.506F, 1.962F, 1.962F  // Signal high
};

int wave(int p, int color)
{
    return ((color + p + 8) % 12 < 6) ? 1 : 0;
}

float gammafix(float f)
{
    return f <= 0.f ? 0.f : pow(f, 2.2f / gamma);
};
int clamp(float v)
{
    return v>255 ? 255 : v;
};
void Initialize()
{
    black = 0.518F;
    white = 1.962F;
    attenuation = 0.746F;

    saturation = CONFIG::GetFloatValue("nes.palette.ntsc.saturation", 1.5F);
    hue_tweak = CONFIG::GetFloatValue("nes.palette.ntsc.hue", 0.0F);
    contrast = CONFIG::GetFloatValue("nes.palette.ntsc.contrast", 1.2F);
    brightness = CONFIG::GetFloatValue("nes.palette.ntsc.brightness", 1.07F);
    gamma = CONFIG::GetFloatValue("nes.palette.ntsc.gamma", 1.8F);
}
void SaveSettings()
{
    CONFIG::SetValue("nes.palette.ntsc.saturation", saturation);
    CONFIG::SetValue("nes.palette.ntsc.hue",hue_tweak);
    CONFIG::SetValue("nes.palette.ntsc.contrast", contrast);
    CONFIG::SetValue("nes.palette.ntsc.brightness", brightness);
    CONFIG::SetValue("nes.palette.ntsc.gamma", gamma);
}
uint32_t MakeRGBcolor(int pixel)
{
    // The input value is a NES color index (with de-emphasis bits).
    // We need RGB values. Convert the index into RGB.
    // For most part, this process is described at:
    //    http://wiki.nesdev.com/w/index.php/NTSC_video

    // Decode the color index
    int color = (pixel & 0x0F);
    int level = color < 0xE ? (pixel >> 4) & 3 : 1;

    float lo_and_hi[2] =
    {
        levels[level + ((color == 0x0) ? 4 : 0)],
        levels[level + ((color <= 0xC) ? 4 : 0)]
    };

    // Calculate the luma and chroma by emulating the relevant circuits:
    float y = 0.0f, i = 0.0f, q = 0.0f;

    for (int p = 0; p < 12; p++) // 12 clock cycles per pixel.
    {
        // NES NTSC modulator (square wave between two voltage levels):
        float spot = lo_and_hi[wave(p, color)];

        // De-emphasis bits attenuate a part of the signal:
        if (((pixel & 0x040) != 0 && wave(p, 0xC) == 1) ||
            ((pixel & 0x080) != 0 && wave(p, 0x4) == 1) ||
            ((pixel & 0x100) != 0 && wave(p, 0x8) == 1))
            spot *= attenuation;

        // Normalize:
        float v = (spot - black) / (white - black);

        // Ideal TV NTSC demodulator:
        // Apply contrast/brightness
        v = (v - 0.5F) * contrast + 0.5F;
        v *= brightness / 12.0F;

        y +=  v;
        i += (v * cos((M_PI / 6.0) * (p + hue_tweak)));
        q += (v * sin((M_PI / 6.0) * (p + hue_tweak)));
    }

    i *= saturation;
    q *= saturation;


    // Convert YIQ into RGB according to FCC-sanctioned conversion matrix.
    unsigned rgb =
        0x10000*clamp(255.95 * gammafix(y +  0.946882f*i +  0.623557f*q))
        + 0x00100*clamp(255.95 * gammafix(y + -0.274788f*i + -0.635691f*q))
        + 0x00001*clamp(255.95 * gammafix(y + -1.108545f*i +  1.709007f*q));

    return rgb;
}

void GeneratePalette512(uint32_t* palette)
{
    for (int i = 0; i < 512; i++)
        palette[i] = MakeRGBcolor(i) | (0xFF << 24);
}
}
}
