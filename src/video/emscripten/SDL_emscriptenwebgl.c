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

#include "SDL_emscriptenvideo.h"
#include "SDL_emscriptenwebgl.h"
#include "SDL_emscriptenwindow.h"
#include "SDL_emscripten.h"


typedef struct _SDL_Emscripten_Context
{
    int padding;
} SDL_Emscripten_Context;


int
Emscripten_WebGL_LoadLibrary(_THIS, const char *path)
{
    return 0;
}

void
Emscripten_WebGL_UnloadLibrary(_THIS)
{

}

void *
Emscripten_WebGL_GetProcAddress(_THIS, const char *proc)
{
    return SDL2_Emscripten_GetProcAddress (proc);
}

int
Emscripten_WebGL_GetAttribute(_THIS, SDL_GLattr attrib, int *value)
{
    return 0;
}


int
Emscripten_WebGL_SetSwapInterval(_THIS, int interval)
{
    return 0;
}

int
Emscripten_WebGL_GetSwapInterval(_THIS)
{
    return 1;
}

SDL_GLContext
Emscripten_WebGL_CreateContext(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = window->driverdata;

    data->context_id = SDL2_Emscripten_CreateContext (data->canvas_id);

    if (data->context_id)
        return (SDL_GLContext) data;
    else {
        SDL_SetError("Could not create SDL context");
        return NULL;
    }
}

void
Emscripten_WebGL_SwapWindow(_THIS, SDL_Window *window)
{
    /* SwapBuffers happens implicitly with WebGL */
}


int
Emscripten_WebGL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
    SDL_WindowData *data = window->driverdata;
    SDL_WindowData *ctx = (SDL_WindowData *)context;

    if (data != ctx)
        return SDL_SetError("Unable to make WebGL context current");

    if (!SDL2_Emscripten_MakeCurrent (data->context_id))
        return SDL_SetError("Unable to make WebGL context current");
    
    return 0;
}

void 
Emscripten_WebGL_DeleteContext(_THIS, SDL_GLContext context)
{
    SDL_WindowData *data = (SDL_WindowData *)context;

    SDL2_Emscripten_DestroyContext (data->context_id);
    data->context_id = 0;
}

#endif /* SDL_VIDEO_DRIVER_EMSCRIPTEN */

/* vi: set ts=4 sw=4 expandtab: */
