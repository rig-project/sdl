/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_EMSCRIPTEN

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "Emscripten" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_emscriptenvideo.h"
#include "SDL_emscriptenevents_c.h"
#include "SDL_emscriptenwindow.h"
#include "SDL_emscriptenwebgl.h"

#define EMSCRIPTENVID_DRIVER_NAME "emscripten"

/* Initialization/Query functions */
static int Emscripten_VideoInit(_THIS);
static int Emscripten_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void Emscripten_VideoQuit(_THIS);

/* Emscripten driver bootstrap functions */

static int
Emscripten_Available(void)
{
    return 1;
}

static void
Emscripten_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_free(device);
}

static SDL_VideoDevice *
Emscripten_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        SDL_free(device);
        return (0);
    }

    /* Set the function pointers */
    device->VideoInit = Emscripten_VideoInit;
    device->VideoQuit = Emscripten_VideoQuit;
    device->SetDisplayMode = Emscripten_SetDisplayMode;
    device->PumpEvents = Emscripten_PumpEvents;

    device->CreateWindow = Emscripten_CreateWindow;
    device->ShowWindow = Emscripten_ShowWindow;
    device->SetWindowFullscreen = Emscripten_SetWindowFullscreen;
    device->SetWindowSize = Emscripten_SetWindowSize;
    device->DestroyWindow = Emscripten_DestroyWindow;
    device->GetWindowWMInfo = Emscripten_GetWindowWMInfo;

    /* GL pointers */
    device->GL_LoadLibrary = Emscripten_WebGL_LoadLibrary;
    device->GL_GetProcAddress = Emscripten_WebGL_GetProcAddress;
    device->GL_UnloadLibrary = Emscripten_WebGL_UnloadLibrary;
    device->GL_CreateContext = Emscripten_WebGL_CreateContext;
    device->GL_MakeCurrent = Emscripten_WebGL_MakeCurrent;
    device->GL_SetSwapInterval = Emscripten_WebGL_SetSwapInterval;
    device->GL_GetSwapInterval = Emscripten_WebGL_GetSwapInterval;
    device->GL_SwapWindow = Emscripten_WebGL_SwapWindow;
    device->GL_DeleteContext = Emscripten_WebGL_DeleteContext;

    device->free = Emscripten_DeleteDevice;

    return device;
}

VideoBootStrap Emscripten_bootstrap = {
    EMSCRIPTENVID_DRIVER_NAME, "SDL Emscripten WebGL video driver",
    Emscripten_Available, Emscripten_CreateDevice
};


int
Emscripten_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    /* Use a fake 32-bpp desktop mode */
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.w = 1024;
    mode.h = 768;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_zero(mode);
    SDL_AddDisplayMode(&_this->displays[0], &mode);

    /* We're done! */
    return 0;
}

static int
Emscripten_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{
    return 0;
}

void
Emscripten_VideoQuit(_THIS)
{
}

#endif /* SDL_VIDEO_DRIVER_EMSCRIPTEN */

/* vi: set ts=4 sw=4 expandtab: */
