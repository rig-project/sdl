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

#ifndef _SDL_emscriptenwindow_h
#define _SDL_emscriptenwindow_h

#include "../SDL_sysvideo.h"
#include "SDL_syswm.h"

#include "SDL_emscriptenvideo.h"

typedef struct {
    int canvas_id;
    int context_id;
} SDL_WindowData;

extern void Emscripten_ShowWindow(_THIS, SDL_Window *window);
extern void Emscripten_SetWindowFullscreen(_THIS, SDL_Window * window,
                                        SDL_VideoDisplay * _display,
                                        SDL_bool fullscreen);
extern int Emscripten_CreateWindow(_THIS, SDL_Window *window);
extern void Emscripten_SetWindowSize(_THIS, SDL_Window * window);
extern void Emscripten_DestroyWindow(_THIS, SDL_Window *window);

extern SDL_bool
Emscripten_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info);

#endif /* _SDL_emscriptenwindow_h */

/* vi: set ts=4 sw=4 expandtab: */
