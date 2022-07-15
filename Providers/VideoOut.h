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
#include <stdint.h>
namespace VideoOut
{
// Execute related command-lines for video out
// argc: the arguments count
// args: the arguments as string arrays.
extern void ExecuteCommands(int argc,char* args[]);
// Initialize the SDL video renderer.
// render_width: the system rendering width (256 for nes for example)
// render_height: the system rendering height (240 for nes for example)
// runEmuFrame: a pointer of the emulator run frame function.
extern void Initialize(int render_width, int render_height, void(*EmuFrameFunction)());
// Draw status and show it in the status area, top-left corner of screen
// status: the status text to draw
// appearance: how many frames should the status remains.
// color: the status text color in R8G8B8 format.
// blended: if true, it will separate and wrap lines but will not draw shadow for the texts.
extern void WriteStatus(const char* status, int appearance, int color, bool blended);
// Set the renderer rectangle, for emu pixels. For nes for example,
// you can set it to 256 x 224 instead of 256 x 240. The emu will
// still render full rendering size, only this rectangle will be displayed
// to user.
// x: the x coordinates
// y: the y coordinates
// w: the render width. Must be smaller than the original render width (256 for nes for example)
// h: the render height. Must be smaller than the original render height (240 for nes for example)
extern void SetRenderRectangle(int x, int y, int w, int h);
// Get current renderer rectangle
// x: the x coordinates
// y: the y coordinates
// w: the render width.
// h: the render height.
extern void GetRenderRectangle(int* x, int* y, int* w, int* h);
// Draw notification and show it in the notification area, bottom-left corner of screen
// notification: the notification text to draw
// appearance: how many frames should the notification remains.
// color: the notification text color in R8G8B8 format.
extern void WriteNotification(const char* notification, int appearance, int color);
// Draw an image as notification at the bottom above notification, it is recommended to to use WriteNotification as well
// filepath: the complete file path of the image file.
extern void DrawImageNotification(const char * filepath, int appearance);
// Get pixel color from the RENDERING BUFFER
extern void GetPixel(int x, int y, int* color);
extern bool ShowHideCursor(bool show);
// Run the sdl thread.
extern void Run();
// Shutdown the sdl renderer.
extern void Shutdown();
// Indicates if the sdl renderer is on.
extern bool ON;
// Indicates if this provider should consider that emu is on and accept buffer submits.
extern bool EMU_ON;
// Indicates if the fps should be shown
extern bool SHOW_FPS;
// Indicates if the video is on fullscreen mode.
extern bool FULLSCREEN;
// Indicates if the frame limiter is enabled
extern bool FrameLimitterEnabled;
// The frame limitter target, 60 FPS for example.
extern uint32_t FrameLimitterTarget;
// Indicates if the frame skip is enabled
extern bool FrameSkipEnabled;
// The frame skipping interval if enabled
extern uint32_t FrameSkipInterval;
// Setup frame limiter
// target: the target frame rate, 60, 50 ... fps for example
extern void SetupFrameLimitter(double target);
// Setup fram skip
// enabled: indicates if frame skip is enabled or not
// interval: how many frames to skip oddly, 1 means frame:on - frame:off - frame:on - frame:off ...etc
extern void SetupFrameSkip(bool enabled, uint32_t interval);
// Take a snap shot of current screen
extern void TakeSnapshot();
// Take a snap shot of current screen
// filepath: the complete path of snap file with snap format.
// snapformat: the snapshot format to use, can be '.png' or '.bmp'
extern void TakeSnapshot(const char * filepath, const char * snapformat);
// Hides the image notification if it is on screen
extern void HideImageNotification();
// Run emu frames after some frames
// delay: the number of frames to delay
extern void SetEmuOn(int delay);
// Set the window title
// newTitle: the new title of the window to use.
extern void SetWindowTitle(const char * newTitle);
// Get current window size
extern void GetWindowSize(int* width, int* height);
// Toggle fullscreen
// fullscreen: indicates if fullscreen or not.
extern void ToggleFullscreen(bool fullscreen);
extern bool LockPixels;// Lock the pixels, some system is rendering !
extern uint32_t* buffer_pixels;// Screen pixels buffer.
}
