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
#include "../Common/ConfigHandler.h"
#include "VideoOut.h"
#include "SDLInput.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <math.h>
#include <chrono>
#include <thread>
#include "Timer/Timer.h"
#include "../Core.h"
#include "../Definations.h"

namespace VideoOut
{
bool ON;// Indicates that the SDL video is on/off
bool EMU_ON;// If set, this provider will consider that emu is on and accept buffer submits.
bool initialized;// Indicates that the SDL video is initialized.
bool isRendering;// Indicates that the SDL video is rendering.
bool SHOW_FPS;// Indicates if the fps should be shown
bool LockPixels;// Lock the pixels, some system is rendering !
bool FULLSCREEN;// Indicates if the video is on fullscreen mode.
bool keep_aspect_ratio_on_fullscreen;
// Render stuff
SDL_Window* window = NULL;
SDL_Renderer * renderer;
int SCREEN_WIDTH = 640;// Visible screen to user width
int SCREEN_HEIGHT = 480;// Visible screen to user height
int RENDER_WIDTH = 640;// The emu rendering screen width
int RENDER_HEIGHT = 480;// The emu rendering screen height
// As loaded from settings ...
int custom_screen_width;
int custom_screen_height;
int screen_mulitply;
bool use_custom_screen_size;
bool enable_vsync;

SDL_Rect* targetPixelsRect;// The render screen pixels
SDL_Rect* renderPixelsRect;// The render screen pixels original (256 x 224 for example)
void CalculateTargetRect();
Uint32* SCREEN_BUFFER;// temp screen buffer.

// Emu out pixels stuff
SDL_Texture * pixels_texture;
Uint32* buffer_pixels;// Screen pixels buffer.
int buffer_size;// Screen buffer size.
std::string SnapshotFolder;
std::string SnapshotFormat;

void(*runEmuFrame)();

// Notification and status
SDL_Surface* statusSurface;
TTF_Font* statusFont;
SDL_Texture* statusTexture;
SDL_Rect statusRect;
bool status_show;
int status_appearance;
bool status_blended;

SDL_Surface* notSurface;
TTF_Font* notFont;
SDL_Texture* notTexture;
SDL_Rect notRect;
bool not_show;
int not_appearance;

SDL_Surface* fpsSurface;
TTF_Font* fpsFont;
SDL_Color fpsColor;
SDL_Texture* fpsTexture;
SDL_Rect fpsRect;

SDL_Surface* imgSurface;
SDL_Texture* imgTexture;
SDL_Rect imgRect;
SDL_Rect imgRectBack;
int img_appearance;
const int img_size = 128;
bool img_show;

// Events
SDL_Event event;

// FPS
// Indicates if the frame limiter is enabled
bool FrameLimitterEnabled;
// The frame limitter target, 60 FPS for example.
uint32_t FrameLimitterTarget;
bool FrameSkipEnabled;
// The frame skipping interval if enabled
uint32_t FrameSkipInterval;
uint32_t FrameSkipCounter;
uint32_t start_time;
uint32_t last_time;
uint32_t fps;
Timer high_res_timer;
double CurrentFrameTime;
double ImmediateFrameTime;
double DeadTime;
double LastFrameTime;
double FramePeriod;// Frame period in ticks

// Shortcuts
Sint32 Key_ToggleFullscreen;
Sint32 Key_ToggleFrameSkip;
Sint32 Key_TakeSnapshot;
Sint32 Key_ToggleTurbo;
Sint32 Key_ToggleFPSShow;

SDL_Joystick* shortucts_joystick;
bool shortcuts_use_joystick;
int shortcuts_joystick_index;
Sint32 JoyButton_ToggleFullscreen;
Sint32 JoyButton_ToggleFrameSkip;
Sint32 JoyButton_TakeSnapshot;
Sint32 JoyButton_ToggleTurbo;
Sint32 JoyButton_ToggleFPSShow;
// Misc
int delay_before_run_emu;
bool delay_before_run_emu_enable;
bool use_linear;
std::string driver_to_use;
std::string current_window_title;

void WriteFps(const char* status, int color);

void LoadSettings()
{
    // For windows, set directx otherwise use opengl (for default of course)
    // After tests, opengl is faster than direct3d whith fullscreen mode.
    // Still user can change driver anytime via config file.
//#ifdef __MINGW32__
//    driver_to_use =CONFIG::GetStringValue("video_driver","direct3d");
//#else
    driver_to_use =CONFIG::GetStringValue("video_driver","opengl");
//#endif
    // VSYNC is not recommended for customized systems (for pal it is 50 fps not 60 nor 75 ...)
    enable_vsync =CONFIG::GetBoolValue("enable_vsync", false);
    use_linear =CONFIG::GetBoolValue("video_linear", false);
    FrameLimitterEnabled = CONFIG::GetBoolValue("frame_limit", true);
    SHOW_FPS = CONFIG::GetBoolValue("show_fps", false);
    FULLSCREEN= CONFIG::GetBoolValue("fullscreen", false);
    FrameSkipEnabled = CONFIG::GetBoolValue("frame_skip_enable", false);
    FrameSkipInterval = CONFIG::GetInt32Value("frame_skip_int", 1);
    custom_screen_width= CONFIG::GetInt32Value("screen_w", 800);
    custom_screen_height= CONFIG::GetInt32Value("screen_h", 600);
    screen_mulitply= CONFIG::GetInt32Value("screen_multi", 2);
    use_custom_screen_size= CONFIG::GetBoolValue("screen_use_custom",false);
    SnapshotFolder = CONFIG::GetStringValue("snaps_folder","snaps");
    SnapshotFormat= CONFIG::GetStringValue("snap_format",".png");
    keep_aspect_ratio_on_fullscreen= CONFIG::GetBoolValue("keep_aspect",true);

    SetupFrameSkip(FrameSkipEnabled,FrameSkipInterval);
    Key_ToggleFullscreen = SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.toggle_fullscreen","F12").c_str());
    Key_ToggleFrameSkip = SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.toggle_frameskip","F11").c_str());
    Key_TakeSnapshot =  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.take_snapshot","F5").c_str());
    Key_ToggleTurbo=  SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.toggle_turbo","F10").c_str());
    Key_ToggleFPSShow= SDL_GetKeyFromName(CONFIG::GetStringValue("shortcut.keyboard.toggle_show_fps","F8").c_str());
    shortcuts_use_joystick=CONFIG::GetBoolValue("shortcut.use_joystick",false);
    shortcuts_joystick_index=CONFIG::GetBoolValue("shortcut.joystick_index",0);
    JoyButton_ToggleFullscreen=CONFIG::GetInt32Value("shortcut.joystick.toggle_fullscreen",10);
    JoyButton_ToggleFrameSkip=CONFIG::GetInt32Value("shortcut.joystick.toggle_frameskip",-1);
    JoyButton_TakeSnapshot=CONFIG::GetInt32Value("shortcut.joystick.take_snapshot",11);
    JoyButton_ToggleTurbo=CONFIG::GetInt32Value("shortcut.joystick.toggle_turbo",-1);
    JoyButton_ToggleFPSShow=CONFIG::GetInt32Value("shortcut.joystick.toggle_show_fps",-1);
}
void SaveSettings()
{
    CONFIG::SetValue("video_linear", use_linear);
    CONFIG::SetValue("enable_vsync", enable_vsync);
    CONFIG::SetValue("video_driver",driver_to_use);
    CONFIG::SetValue("show_fps", SHOW_FPS);
    CONFIG::SetValue("frame_limit", FrameLimitterEnabled);
    CONFIG::SetValue("fullscreen", FULLSCREEN);
    CONFIG::SetValue("frame_skip_enable", FrameSkipEnabled);
    CONFIG::SetValue("frame_skip_int", FrameSkipInterval);
    CONFIG::SetValue("snaps_folder",SnapshotFolder);
    CONFIG::SetValue("snap_format",SnapshotFormat);
    CONFIG::SetValue("keep_aspect",keep_aspect_ratio_on_fullscreen);
    CONFIG::SetValue("screen_w",custom_screen_width);
    CONFIG::SetValue("screen_h", custom_screen_height);
    CONFIG::SetValue("screen_multi", screen_mulitply);
    CONFIG::SetValue("screen_use_custom",use_custom_screen_size);
    CONFIG::SetValue("shortcut.keyboard.toggle_fullscreen",(std::string)SDL_GetKeyName(Key_ToggleFullscreen));
    CONFIG::SetValue("shortcut.keyboard.toggle_frameskip",(std::string)SDL_GetKeyName(Key_ToggleFrameSkip));
    CONFIG::SetValue("shortcut.keyboard.take_snapshot",(std::string)SDL_GetKeyName(Key_TakeSnapshot));
    CONFIG::SetValue("shortcut.keyboard.toggle_turbo",(std::string)SDL_GetKeyName(Key_ToggleTurbo));
    CONFIG::SetValue("shortcut.keyboard.toggle_show_fps",(std::string)SDL_GetKeyName(Key_ToggleFPSShow));
    CONFIG::SetValue("shortcut.use_joystick",shortcuts_use_joystick);
    CONFIG::SetValue("shortcut.joystick_index",shortcuts_joystick_index);
    CONFIG::SetValue("shortcut.joystick.toggle_fullscreen",JoyButton_ToggleFullscreen);
    CONFIG::SetValue("shortcut.joystick.toggle_frameskip",JoyButton_ToggleFrameSkip);
    CONFIG::SetValue("shortcut.joystick.take_snapshot",JoyButton_TakeSnapshot);
    CONFIG::SetValue("shortcut.joystick.toggle_turbo",JoyButton_ToggleTurbo);
    CONFIG::SetValue("shortcut.joystick.toggle_show_fps",JoyButton_ToggleFPSShow);
}
void Initialize(int render_width, int render_height, void(*EmuFrameFunction)())
{
    initialized = false;
    LoadSettings();
    runEmuFrame = EmuFrameFunction;
    std::cout <<"SDL: Initializing video ...\n";
    if (render_width>0 && render_height>0)
    {
        RENDER_WIDTH = render_width;
        RENDER_HEIGHT = render_height;

        if (use_custom_screen_size)
        {
            SCREEN_WIDTH = custom_screen_width;
            SCREEN_HEIGHT = custom_screen_height;
        }
        else
        {
            SCREEN_WIDTH = RENDER_WIDTH*screen_mulitply;
            SCREEN_HEIGHT = RENDER_HEIGHT*screen_mulitply;
        }
    }
    else
    {
        // No render, standby moe maybe.
        // Use the custom size instead
        RENDER_WIDTH = custom_screen_width;
        RENDER_HEIGHT = custom_screen_height;
        SCREEN_WIDTH  = custom_screen_width;
        SCREEN_HEIGHT = custom_screen_height;
    }

    std::cout <<"SDL: Screen W = "<< Convert::to_string(SCREEN_WIDTH)<<"\n";
    std::cout <<"SDL: Screen H = "<< Convert::to_string(SCREEN_HEIGHT)<<"\n";
    std::cout <<"SDL: Render W = "<< Convert::to_string(RENDER_WIDTH)<<"\n";
    std::cout <<"SDL: Render H = "<< Convert::to_string(RENDER_HEIGHT)<<"\n";
    SDL_Init(SDL_INIT_VIDEO);
    // Set video driver and configuration
    SDL_SetHint( SDL_HINT_RENDER_DRIVER, driver_to_use.c_str() );
    std::cout << "SDL: video driver set to "<<driver_to_use<<"\n";
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, use_linear?"1":"0" );
    std::cout << "SDL: use linear ? "<<use_linear<<"\n";

    std::cout << "SDL: creating window...\n";
    // Create the window.
    if (current_window_title.size()==0)
        current_window_title="EmuShell";

    window = SDL_CreateWindow(current_window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FOREIGN);
    std::cout << "SDL: window created successfully.\n";
    std::cout << "SDL: toggling fullscreen/windowed mode...\n";
    // Set fullscreen
    ToggleFullscreen(FULLSCREEN);
    std::cout << "SDL: done !\n";
    std::cout << "SDL: creating renderer for video ...\n";
    // Create the renderer that will render the texture.
    if (enable_vsync)
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE |SDL_RENDERER_ACCELERATED|
                                      SDL_RENDERER_PRESENTVSYNC);
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE|SDL_RENDERER_ACCELERATED);
    }

    std::cout << "SDL: renderer is ready !\n";
    std::cout << "SDL: creating textures and buffers...\n";
    // Create the texture
    pixels_texture = SDL_CreateTexture(renderer,
                                       SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STATIC,
                                       RENDER_WIDTH,
                                       RENDER_HEIGHT);

    // Buffers.
    SCREEN_BUFFER = new Uint32[buffer_size = (RENDER_WIDTH * RENDER_HEIGHT)];
    buffer_pixels = new Uint32[buffer_size];
    // Cleat buffers.
    //memset(SCREEN_BUFFER, 0, buffer_size * sizeof(Uint32));
    //memset(buffer_pixels, 0, buffer_size * sizeof(Uint32));
    std::cout << "SDL: done !!\n";
    start_time = SDL_GetTicks();

    //Initialize SDL_ttf
    std::cout << "SDL: Initializing TTF ...\n";
    TTF_Init();
    std::cout << "SDL: ->Initializing fonts ...\n";
    statusFont = TTF_OpenFont("FreeSans.ttf", 24);
    if (statusFont==NULL)
        std::cout << "SDL: ->ERORR !! STATUS FONT DOESN'T LOADED !\n";
    fpsFont = TTF_OpenFont("FreeSans.ttf", 24);
    notFont = TTF_OpenFont("FreeSans.ttf", 24);
    std::cout << "SDL: ->Fonts initialized successfully.\n";
    std::cout << "SDL: TTF Initialized successfully \n";

    // Start the limit timer
    high_res_timer.start();

    std::cout << "SDL: Initializing IMG ...\n";
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        std::cout << "SDL: ->IMG could not initialize!\n";
    }
    else
        std::cout << "SDL: IMG initialized successfully.\n";

    // Just in case
    WriteStatus("NO SIGNAL", -1, 0x00FF00,false);
    CalculateTargetRect();
    initialized = true;
    std::cout << "SDL: video initialized successfully.\n";

    // Get the index of current player
    if (shortcuts_use_joystick)
    {
        std::cout << "SDL: Opening joystick for shortcuts, joystick index = "
                  <<Convert::to_string(shortcuts_joystick_index)<<"\n";
        shortucts_joystick = SDL_JoystickOpen(shortcuts_joystick_index);
        if (shortucts_joystick!=NULL)
        {
            std::cout << "SDL: Shortcuts Joystick name = " << SDL_JoystickName(shortucts_joystick)<<"\n";
            std::cout << "SDL: Shortcuts Joystick buttons number = " << Convert::to_string(SDL_JoystickNumButtons(shortucts_joystick))<<"\n";
        }
    }
}
void SetRenderRectangle(int x, int y, int w, int h)
{
    renderPixelsRect=new SDL_Rect();
    renderPixelsRect->x= x;
    renderPixelsRect->y= y;
    renderPixelsRect->h= h;
    renderPixelsRect->w= w;
}
void GetRenderRectangle(int* x, int* y, int* w, int* h)
{
    if (renderPixelsRect==NULL)
        return;
    *x= renderPixelsRect->x;
    *y= renderPixelsRect->y;
    *h= renderPixelsRect->h;
    *w= renderPixelsRect->w;
}
void Shutdown()
{
    // Save settings
    SaveSettings();
    // Sutdown video objects.
    delete[] targetPixelsRect;
    delete[] renderPixelsRect;
    delete[] SCREEN_BUFFER;
    delete[] buffer_pixels;
    SDL_DestroyTexture(pixels_texture);
    SDL_DestroyTexture( imgTexture);
    SDL_DestroyTexture(statusTexture);
    SDL_DestroyTexture(notTexture);
    SDL_DestroyTexture( fpsTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void CheckEvents()
{
    while (SDL_PollEvent(&event) != 0)
    {
        //User requests quit
        if (event.type == SDL_QUIT)
        {
            ON = false;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                ON = false;
            }
            else if (event.key.keysym.sym==Key_ToggleTurbo)
            {
                FrameLimitterEnabled = !FrameLimitterEnabled;
                std::string note = "TURBO " + (std::string)(FrameLimitterEnabled ? "OFF":"ON");
                WriteNotification(note.c_str(),120,0x00FF00);
            }
            else if (event.key.keysym.sym==Key_ToggleFullscreen)
            {
                ToggleFullscreen(!FULLSCREEN);
            }
            else if (event.key.keysym.sym==Key_ToggleFrameSkip)
            {
                FrameSkipEnabled = !FrameSkipEnabled;
                if (FrameSkipInterval <=0)
                    FrameSkipInterval=1;

                if (FrameSkipEnabled)
                    WriteNotification("Frame skip ON", 120, 0x00FF00);
                else
                    WriteNotification("Frame skip OFF", 120,0x00FF00);
            }
            else if (event.key.keysym.sym==Key_TakeSnapshot)
            {
                TakeSnapshot();
            }
            else if (event.key.keysym.sym==Key_ToggleFPSShow)
            {
                SHOW_FPS = !SHOW_FPS;
            }
            else
            {
                // System shortcuts
                EmuShell::CheckSystemShortcut(event.key.keysym.sym);
            }
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            CalculateTargetRect();
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            EmuShell::CheckMouseClicks(event.button.button);
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            EmuShell::CheckMouseUps(event.button.button);
        }
        else  if (shortcuts_use_joystick)
        {
            if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (event.jbutton.button == JoyButton_ToggleTurbo)
                {
                    FrameLimitterEnabled = !FrameLimitterEnabled;
                    std::string note = "TURBO " + (std::string)(FrameLimitterEnabled ? "OFF":"ON");
                    WriteNotification(note.c_str(),120,0x00FF00);
                }
                else if (event.jbutton.button==JoyButton_ToggleFullscreen)
                {
                    ToggleFullscreen(!FULLSCREEN);
                }
                else if (event.jbutton.button==JoyButton_ToggleFrameSkip)
                {
                    FrameSkipEnabled = !FrameSkipEnabled;
                    if (FrameSkipInterval <=0)
                        FrameSkipInterval=1;

                    if (FrameSkipEnabled)
                        WriteNotification("Frame skip ON", 120, 0x00FF00);
                    else
                        WriteNotification("Frame skip OFF", 120,0x00FF00);
                }
                else if (event.jbutton.button==JoyButton_TakeSnapshot)
                {
                    TakeSnapshot();
                }
                else if (event.jbutton.button==JoyButton_ToggleFPSShow)
                {
                    SHOW_FPS = !SHOW_FPS;
                }
                else
                {
                    // System shortcuts
                    EmuShell::CheckJoystickShortcut(event.jbutton.button);
                }
            }
        }
    }
}
void Run()
{
    while (ON)
    {
        if (!initialized)
            continue;
        SDLInput::Update();
        if (delay_before_run_emu_enable)
        {
            if (delay_before_run_emu>0)
                delay_before_run_emu--;
            else
            {
                delay_before_run_emu_enable=false;
                EMU_ON=true;
            }

        }

        if (!EMU_ON)// Emu is off, draw snow.
        {
            for (int i = 0; i < buffer_size; i++)
            {
                uint8_t val = (rand() % 255);

                buffer_pixels[i] = (Uint32)((val << 16) | (val << 8) | val);
            }
        }
        else
        {
            // Run emulator
            runEmuFrame();
        }

        isRendering=true;
        SDL_RenderClear(renderer);

        if (FrameSkipEnabled)
        {
            if (FrameSkipCounter>0)
            {
                FrameSkipCounter--;
                // Frame get skipped here, pixels are not copied
            }
            else
            {
                FrameSkipCounter = FrameSkipInterval;
                // Render
                if (!LockPixels)
                {
                    SDL_UpdateTexture(pixels_texture, NULL, buffer_pixels, RENDER_WIDTH * sizeof(Uint32));
                    // Render the pixels
                    SDL_RenderCopy(renderer, pixels_texture, renderPixelsRect, targetPixelsRect);
                }
            }
        }
        else
        {
            // Render normally without skipping
            if (!LockPixels)
            {
                SDL_UpdateTexture(pixels_texture, NULL, buffer_pixels, RENDER_WIDTH * sizeof(Uint32));
                // Render the pixels
                SDL_RenderCopy(renderer, pixels_texture, renderPixelsRect, targetPixelsRect);
            }
        }

        //********* Render the status
        if (status_show)
        {
            if (status_appearance > 0)
            {
                status_appearance--;
                if (status_blended)
                {
                    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
                    SDL_RenderFillRect( renderer, &statusRect );
                }
                SDL_RenderCopy(renderer, statusTexture, NULL, &statusRect);
            }
            else
            {
                status_show = false;
            }

        }
        else if (status_appearance == -1)
        {
            if (status_blended)
            {
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
                SDL_RenderFillRect( renderer, &statusRect );
            }
            SDL_RenderCopy(renderer, statusTexture, NULL, &statusRect);
        }
        //***********************************************************

        //********* Render the notification
        if (not_show)
        {
            if (not_appearance > 0)
            {
                not_appearance--;
                SDL_RenderCopy(renderer, notTexture, NULL, &notRect);
            }
            else
            {
                not_show = false;
            }

        }
        else if (not_appearance == -1)
        {
            SDL_RenderCopy(renderer, notTexture, NULL, &notRect);
        }
        //***********************************************************

        //********* Render the image notification
        if (img_show)
        {
            if (img_appearance > 0)
            {
                img_appearance--;
                // Set render color to black ( rect will be rendered in this color )
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
                SDL_RenderFillRect( renderer, &imgRectBack );

                SDL_RenderCopy(renderer, imgTexture, NULL, &imgRect);
            }
            else
            {
                img_show = false;
            }

        }
        else if (img_appearance == -1)
        {
            SDL_RenderCopy(renderer, imgTexture, NULL, &imgRect);
        }
        //***********************************************************

        //********* Show FPS
        if (SHOW_FPS)
        {
            if (ImmediateFrameTime>0)
            {
                WriteFps(("FPS: " + Convert::to_string<double>(1.0/ ImmediateFrameTime)).c_str(), 0x00FF00);

                SDL_RenderCopy(renderer, fpsTexture, NULL, &fpsRect);
            }
        }
        //***********************************************************

        SDL_RenderPresent(renderer);

        fps++;

        isRendering=false;

        // EVENTS ******************************
        CheckEvents();

        //********* Speed limitter **********************************
        ImmediateFrameTime = CurrentFrameTime = high_res_timer.getElapsedTimeInSec() - LastFrameTime;
        DeadTime = FramePeriod - CurrentFrameTime;
        if (FrameLimitterEnabled && ImmediateFrameTime > 0)// ImmediateFrameTime==0 means the timer doesn't work
        {
            if (DeadTime > 0)
            {

                //std::this_thread::sleep_for(std::chrono::milliseconds(floor(DeadTime * 1000)));
                SDL_Delay(floor(DeadTime * 1000));
            }



            //while (ImmediateFrameTime < FramePeriod)
            //{
            //    ImmediateFrameTime = high_res_timer.getElapsedTimeInSec() - LastFrameTime;
            //}
        }
        LastFrameTime = high_res_timer.getElapsedTimeInSec();
        //***********************************************************

    }
}
void GetRatioStretchRectangle(int orgWidth, int orgHeight,int maxWidth, int maxHeight,
                              int &out_x, int &out_y, int &out_w, int &out_h)
{
    float hRatio = orgHeight / maxHeight;
    float wRatio = orgWidth / maxWidth;
    bool touchTargetFromOutside = false;
    if ((wRatio > hRatio) ^ touchTargetFromOutside)
    {
        out_w= maxWidth;
        out_h = (orgHeight * maxWidth) / orgWidth;
    }
    else
    {
        out_h= maxHeight;
        out_w= (orgWidth * maxHeight) / orgHeight;
    }

    out_x = (maxWidth - out_w) / 2;
    out_y = (maxHeight - out_h) / 2;
}
void CalculateTargetRect()
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(window,&window_w,&window_h);
    targetPixelsRect=new SDL_Rect();
    if (keep_aspect_ratio_on_fullscreen && renderPixelsRect != NULL)
    {
// Calculate aspect ratio
        GetRatioStretchRectangle(renderPixelsRect->w, renderPixelsRect->h, window_w, window_h,
                                 targetPixelsRect->x,
                                 targetPixelsRect->y,
                                 targetPixelsRect->w,
                                 targetPixelsRect->h);
    }
    else
    {
        // As it is !
        targetPixelsRect->x= 0;
        targetPixelsRect->y= 0;
        targetPixelsRect->h= window_h;
        targetPixelsRect->w= window_w;
    }
    custom_screen_width=window_w;
    custom_screen_height=window_h;
}
void WriteStatus(const char* status, int appearance, int color, bool blended)
{
    if (!initialized)
        return;
    if (isRendering)
        return;
    status_appearance = appearance;
    status_blended = blended;
    if (appearance != -1)
        status_show = true;
    // Create the surface for the text
    SDL_Color staColor = { Uint8(color >> 16), Uint8(color >> 8), Uint8(color) };
    if (blended)
    {
        statusSurface = TTF_RenderUTF8_Blended_Wrapped(statusFont, status, staColor,targetPixelsRect->w-10);
    }
    else
    {
        SDL_Color bkColor = {0x0,0x0,0x0};
        statusSurface = TTF_RenderText_Shaded(statusFont, status, staColor, bkColor);
    }

    //Create texture from surface pixels
    statusTexture = SDL_CreateTextureFromSurface(renderer, statusSurface);

    statusRect.x = 10;
    statusRect.y = 10;
    statusRect.h = statusSurface->h;
    statusRect.w = statusSurface->w;

    //Get rid of old surface
    SDL_FreeSurface(statusSurface);
}
void WriteNotification(const char* notification, int appearance, int color)
{
    if (!initialized)
        return;
    if (isRendering)
        return;
    not_appearance = appearance;
    if (appearance != -1)
        not_show = true;
    // Create the surface for the text
    SDL_Color staColor = { Uint8(color >> 16), Uint8(color >> 8), Uint8(color) };
    SDL_Color bkColor = {0x0,0x0,0x0};
    notSurface = TTF_RenderText_Shaded(notFont, notification, staColor, bkColor);

    //Create texture from surface pixels
    notTexture = SDL_CreateTextureFromSurface(renderer, notSurface);

    int window_w;
    int window_h;
    SDL_GetWindowSize(window,&window_w,&window_h);

    notRect.x = 10;
    notRect.y = window_h - (notSurface->h + 10);
    notRect.h = notSurface->h;
    notRect.w = notSurface->w;

    //Get rid of old surface
    SDL_FreeSurface(notSurface);
}
void WriteFps(const char* status, int color)
{
    if (!initialized)
        return;
    // Create the surface for the text
    SDL_Color staColor = { Uint8(color >> 16), Uint8(color >> 8), Uint8(color) };
    SDL_Color bkColor = {0x0,0x0,0x0};
    fpsSurface = TTF_RenderText_Shaded(fpsFont, status, staColor, bkColor);

    //Create texture from surface pixels
    fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);

    int window_w;
    int window_h;
    SDL_GetWindowSize(window,&window_w,&window_h);

    fpsRect.x = window_w - (fpsSurface->w + 10);
    fpsRect.y = 10;
    fpsRect.h = fpsSurface->h;
    fpsRect.w = fpsSurface->w;

    //Get rid of old surface
    SDL_FreeSurface(fpsSurface);
}
void DrawImageNotification(const char * filepath, int appearance)
{
    if (!initialized)
        return;
    if (isRendering)
        return;
    img_appearance = appearance;
    if (appearance != -1)
        img_show = true;

    std::cout << "SDL: Loading image ...\n";
    std::string ext = Path::GetExtnsion(filepath);
    std::cout << "SDL: Format = "<<ext<<"\n";
    if (ext==".png")
        imgSurface = IMG_Load(filepath);
    else if (ext==".bmp")
        imgSurface = SDL_LoadBMP(filepath);

    std::cout << "SDL: Image loaded successfully.\n";
    if (imgSurface!=NULL)
    {
        imgTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);

        imgRect.h = img_size;
        imgRect.w = img_size;
        imgRect.x = 10;
        int window_w;
        int window_h;
        SDL_GetWindowSize(window,&window_w,&window_h);

        if (notRect.h>0)
            imgRect.y = window_h - (img_size + notRect.h + 15);
        else
            imgRect.y = window_h - (img_size + 10);

        imgRectBack.h = img_size+2;
        imgRectBack.w= img_size+2;
        imgRectBack.x = 9;
        imgRectBack.y =  imgRect.y-1;
    }
    else
    {
        std::cout <<"SDL: the image surface is NULL \n";
    }
    //Get rid of old surface
    SDL_FreeSurface(imgSurface);
}
void HideImageNotification()
{
    img_show = false;
    img_appearance=0;
}
void SetupFrameLimitter(double target)
{
    FrameLimitterTarget = target;

    FramePeriod = (1.0 / target);

    std::string fpsString = "SDL: Frame limitter target set to " +
                            Convert::to_string<double>(target) + " fps\n";

    std::cout << fpsString.c_str();
}
void ToggleFullscreen(bool fullscreen)
{
    FULLSCREEN = fullscreen;
    if (FULLSCREEN)
    {
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(0);
    }
    else
    {
        SDL_SetWindowFullscreen(window,SDL_WINDOW_RESIZABLE | SDL_WINDOW_FOREIGN);
        SDL_ShowCursor(1);
    }
    CalculateTargetRect();
}
bool ShowHideCursor(bool show)
{
    SDL_ShowCursor(show?1:0);
}
void TakeSnapshot()
{

// Make the system folder
    std::string folder = Path::Combine(SnapshotFolder,(std::string)EmuShell::SYSTEM);
    std::string gamename = EmuShell::FileName;
// Erase the folder ...
    std::size_t found = gamename.find_last_of("/\\");
    gamename = gamename.substr(found+1);

// Erase the extension
    gamename.erase(gamename.size() - 4, 4);

    std::string snapfile = Path::Combine(folder,gamename);

// Add the snapshot
    std::string snap =snapfile+SnapshotFormat;

// While exist ...etc
    int i =1 ;
    while (Path::IsFileExist(snap.c_str()))
    {
        snap =snapfile+"_"+Convert::T_to_string(i)+SnapshotFormat;
        i++;
    }
    TakeSnapshot(snap.c_str(),SnapshotFormat.c_str());

    WriteNotification(("Snap saved: "+Path::GetFileName(snap)).c_str(),120,0x00FF00);
}
void TakeSnapshot(const char * filepath, const char * snapformat)
{
    // Do it !
    LockPixels=true;
    // Make surface from the pixels texture
    SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(buffer_pixels, RENDER_WIDTH, RENDER_HEIGHT, 32, RENDER_WIDTH * 4, 0,0,0,0);

    LockPixels=false;
    // Save it !
    if ((std::string)snapformat == ".png")
    {
        IMG_SavePNG(surf, filepath);
    }
    else if ((std::string)snapformat == ".bmp")
    {
        SDL_SaveBMP(surf, filepath);
    }

    std::cout << "SDL: snapshot taken, file saved at\n";
    std::cout << filepath <<"\n";
}
void SetupFrameSkip(bool enabled, uint32_t interval)
{
    if (interval>0)
    {
        FrameSkipEnabled=enabled;
        FrameSkipInterval = interval;
    }
    else
    {
        FrameSkipEnabled=false;
        FrameSkipInterval = 1;
    }
}
void SetEmuOn(int delay)
{
    delay_before_run_emu=delay;
    delay_before_run_emu_enable=true;
    WriteStatus("WAITING FOR SIGNAL", 40, 0x00FF00,false);
}
void ExecuteCommands(int argc,char* args[])
{
    for (int i = 2; i<argc; i++)
    {
        if ((std::string) args[i] == "-show_fps")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("show_fps",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-frame_limit")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("frame_limit",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-fullscreen")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("fullscreen",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-frame_skip_enable")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("frame_skip_enable",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-frame_skip_int")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("frame_skip_int", Convert::string_to_T<uint32_t>((std::string)args[i]));
        }
        else if ((std::string) args[i]== "-snaps_folder")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("snaps_folder", (std::string)args[i]);
        }
        else if ((std::string) args[i]== "-snap_format")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("snap_format", (std::string)args[i]);
        }
        else if ((std::string) args[i]== "-keep_aspect")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("keep_aspect",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-screen_use_custom")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("screen_use_custom",(bool)((std::string) args[i] == "1"));
        }
        else if ((std::string) args[i]== "-screen_w")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("screen_w", Convert::string_to_T<int>((std::string)args[i]));
        }
        else if ((std::string) args[i]== "-screen_h")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("screen_h", Convert::string_to_T<int>((std::string)args[i]));
        }
        else if ((std::string) args[i]== "-screen_multi")
        {
            i++;
            if (i < argc)
                CONFIG::SetValue("screen_multi", Convert::string_to_T<int>((std::string)args[i]));
        }
    }
}
void SetWindowTitle(const char * newTitle)
{
    current_window_title=(std::string)newTitle;
    SDL_SetWindowTitle(window, newTitle);
}
void GetWindowSize(int* width, int* height)
{
    SDL_GetWindowSize(window, width, height);
}
void GetPixel(int x, int y, int* color)
{
    *color= buffer_pixels[(y * RENDER_WIDTH) + x];
}
}

