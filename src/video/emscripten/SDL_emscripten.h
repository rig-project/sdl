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

#ifndef _SDL_emscripten_h
#define _SDL_emscripten_h

int SDL2_Emscripten_CreateContext (int canvas_id);
int SDL2_Emscripten_MakeCurrent (int context_id);
void SDL2_Emscripten_DestroyContext (int canvas_id);

int SDL2_Emscripten_CreateWindow (int width, int height, int x, int y);
void SDL2_Emscripten_ShowWindow (int canvas_id);
void SDL2_Emscripten_DestroyWindow (int canvas_id);

void SDL2_Emscripten_RequestFullScreen (int canvas_id);
void SDL2_Emscripten_CancelFullScreen (int canvas_id);
int SDL2_Emscripten_SetWindowSize (int canvas_id, int width, int height);

void *SDL2_Emscripten_GetProcAddress (const char *name);

void int SDL2_Emscripten_AddInputListener (void);

#endif
