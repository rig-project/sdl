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

#include "../SDL_sysvideo.h"
#include "SDL_emscriptenwindow.h"
#include "SDL_emscriptenvideo.h"
#include "SDL_emscripten.h"

SDL_bool
Emscripten_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info)
{
    info->subsystem = SDL_SYSWM_EMSCRIPTEN;

    return SDL_TRUE;
}

void Emscripten_ShowWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = window->driverdata;

    SDL2_Emscripten_ShowWindow (data->canvas_id);

    if (window->flags & SDL_WINDOW_FULLSCREEN)
        SDL2_Emscripten_RequestFullScreen (data->canvas_id);
}

void
Emscripten_SetWindowFullscreen(_THIS, SDL_Window * window,
        SDL_VideoDisplay * _display, SDL_bool fullscreen)
{
    SDL_WindowData *data = window->driverdata;

    if (fullscreen) {
        SDL2_Emscripten_RequestFullScreen(data->canvas_id);
    } else {
        SDL2_Emscripten_CancelFullScreen(data->canvas_id);
    }
}

int Emscripten_CreateWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data;

    data = calloc(1, sizeof *data);
    if (data == NULL) {
        SDL_SetError("failed to allocate window data");
        return -1;
    }

    window->driverdata = data;

    if (!(window->flags & SDL_WINDOW_OPENGL)) {
        SDL_GL_LoadLibrary(NULL);
        window->flags |= SDL_WINDOW_OPENGL;
    }

    if (window->x == SDL_WINDOWPOS_UNDEFINED) {
        window->x = 0;
    }
    if (window->y == SDL_WINDOWPOS_UNDEFINED) {
        window->y = 0;
    }

    data->canvas_id = 
        SDL2_Emscripten_CreateWindow(window->w, window->h, window->x, window->y);

    return 0;
}

void Emscripten_SetWindowSize(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = window->driverdata;

    SDL2_Emscripten_SetWindowSize (data->canvas_id, window->w, window->h);
}

void Emscripten_DestroyWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = window->driverdata;

    SDL2_Emscripten_DestroyWindow (data->canvas_id);

    window->driverdata = NULL;

    if (data) {
        SDL_free (data);
    }
}

/* vi: set ts=4 sw=4 expandtab: */
