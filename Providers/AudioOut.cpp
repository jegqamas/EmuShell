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
#include <string>
#include <iostream>
#include "VideoOut.h"
#include <SDL2/SDL.h>

namespace AudioOut
{
bool IsPlaying;
Uint8* audio_samples;
long samples_count;
int buffer_size;
int w_pos;
int r_pos;
short temp_value;
bool ready;
bool enabled;
SDL_AudioSpec* wav_spec_desigred;
SDL_AudioSpec* specs1;

int audio_selected_device_index;
int audio_opened_device_index;

void my_audio_callback(void *userdata, Uint8 * stream, int len);

void Initialize(const bool pl_enabled, const uint32_t frequency,const int buffer_size_bytes)
{
    ready = false;
    enabled= pl_enabled;
    buffer_size=buffer_size_bytes;

    std::cout << "SDL: Initializing audio ...\n";
    SDL_Init(SDL_INIT_AUDIO);

    // List audio drivers
    int c = SDL_GetNumAudioDrivers();
    for (int i = 0; i < c; i++)
    {
        printf("SDL: ->Audio driver found: %s \n", SDL_GetAudioDeviceName(i, 0));
    }
    if (audio_selected_device_index<0)
        audio_selected_device_index=0;

    printf("SDL: Opening audio device ... \n");
    printf("SDL: ->Audio device to open: %s \n",SDL_GetAudioDeviceName(audio_selected_device_index, 0));

    wav_spec_desigred = new SDL_AudioSpec();
    wav_spec_desigred->channels = 1;
    wav_spec_desigred->format = AUDIO_S16SYS;
    wav_spec_desigred->freq = frequency;
    wav_spec_desigred->samples = buffer_size;
    wav_spec_desigred->callback = my_audio_callback;
    wav_spec_desigred->userdata = NULL;

    // SDL_OpenAudio(wav_spec_desigred, NULL);

    audio_opened_device_index = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(audio_selected_device_index, 0), 0, wav_spec_desigred, specs1, NULL);

    if (audio_opened_device_index == 0)
    {
        printf("SDL: ERROR while opening audio device: %s \n",SDL_GetError());
    }
    else
    {
        printf("SDL: Audio device opened successfully.\n");
    }

    samples_count = buffer_size * 20;
    audio_samples = new Uint8[samples_count];

    for (int i = 0; i < samples_count; i++)
        audio_samples[i] = rand() % 100;
    w_pos = 0;
    r_pos = w_pos + buffer_size;

    IsPlaying = false;

    SDL_PauseAudio(enabled ?1:0);

    ready = true;

    std::cout <<"SDL: Audio initialized success.\n";
}
void Play()
{
    if (!IsPlaying)
    {
        IsPlaying = true;
        SDL_PauseAudio(0);
    }
}
void Pause()
{
    if (IsPlaying)
    {
        IsPlaying = false;
        SDL_PauseAudio(1);
    }
}
void PutSample(const short& sample)
{
    if (!enabled)
        return;
    if (!ready)
        return;

    audio_samples[w_pos] = sample & 0xFF;
    w_pos++;
    if (w_pos >= samples_count)
        w_pos -= samples_count;

    audio_samples[w_pos] = sample >> 8;
    w_pos++;
    if (w_pos >= samples_count)
        w_pos -= samples_count;
}
void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
    if (!enabled)
        return;
    if (!ready)
        return;

    for (int i = 0; i < len; i++)
    {
        if (r_pos == w_pos)
        {
            // The buffer is full, avoid conflicts by shifting the read pointer
            if (r_pos % 2 !=0)
                r_pos--;
            else
                r_pos -=2;
            if (r_pos < 0)
                r_pos += samples_count;
        }
        stream[i] = audio_samples[r_pos];

        r_pos++;
        if (r_pos >= samples_count)
            r_pos -= samples_count;
    }
}
void Shutdown()
{
    delete [] audio_samples;
    delete [] wav_spec_desigred;
}
void ExecuteCommands(int argc,char* args[])
{
    //for (int i = 2; i<argc; i++)
    //{
    //}
}
}
