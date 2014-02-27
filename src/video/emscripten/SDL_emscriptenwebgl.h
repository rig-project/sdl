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

#ifndef _SDL_emscriptenopengles_h
#define _SDL_emscriptenopengles_h

#include "../SDL_sysvideo.h"
#include "../SDL_egl_c.h"

typedef struct SDL_PrivateWebGLData
{
} SDL_PrivateWebGLData;

/* WebGL functions */
extern int Emscripten_WebGL_GetAttribute(_THIS, SDL_GLattr attrib, int *value);
extern void *Emscripten_WebGL_GetProcAddress(_THIS, const char *proc);
extern void Emscripten_WebGL_UnloadLibrary(_THIS);
extern int Emscripten_WebGL_SetSwapInterval(_THIS, int interval);
extern int Emscripten_WebGL_GetSwapInterval(_THIS);

extern int Emscripten_WebGL_LoadLibrary(_THIS, const char *path);
extern SDL_GLContext Emscripten_WebGL_CreateContext(_THIS, SDL_Window * window);
extern void Emscripten_WebGL_SwapWindow(_THIS, SDL_Window * window);
extern int Emscripten_WebGL_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern void Emscripten_WebGL_DeleteContext(_THIS, SDL_GLContext context);

#endif /* _SDL_emscriptenopengles_h */
