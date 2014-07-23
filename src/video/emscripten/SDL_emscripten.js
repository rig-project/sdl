//"use strict";

var LibrarySDL2Emscripten = {
    $SDL2Emscripten__deps: ['$Browser', '$GL'],
    $SDL2Emscripten: {

        counter: 1, //To use as unique handles to pass to native code

        handles: {},

	windows: {},
	contexts: {},

        // XXX: This trick for keeping the table dense was nabbed from
        // library_gl.js:
        //
        // Get a new ID for images, while keeping the table dense and
        // fast. Creation is farely rare so it is worth optimizing
        // lookups later.
        //
        // I'm not a js expert but to me this looks like because it
        // never scans for null entries before incrementing counter
        // then this table will just continuously grow as images are
        // created and destroyed?
        getNewId: function (table) {
            var ret = SDL2Emscripten.counter++;
            for (var i = table.length; i < ret; i++) {
                table[i] = null;
            }
            return ret;
        },

        touchX: 0, touchY: 0,
        savedKeydown: null,

	receiveEvent: function(event) {
          //XXX: This is copied from library_sdl.js
          switch(event.type) {
            case 'touchstart':
              event.preventDefault();
              var touch = event.touches[0];
              touchX = touch.pageX;
              touchY = touch.pageY;
              var event = {
                type: 'mousedown',
                button: 0,
                pageX: touchX,
                pageY: touchY
              };
              SDL.DOMButtons[0] = 1;
              SDL2Emscripten.events.push(event);
              break;
            case 'touchmove':
              event.preventDefault();
              var touch = event.touches[0];
              touchX = touch.pageX;
              touchY = touch.pageY;
              event = {
                type: 'mousemove',
                button: 0,
                pageX: touchX,
                pageY: touchY
              };
              SDL2Emscripten.events.push(event);
              break;
            case 'touchend':
              event.preventDefault();
              event = {
                type: 'mouseup',
                button: 0,
                pageX: touchX,
                pageY: touchY
              };
              SDL.DOMButtons[0] = 0;
              SDL2Emscripten.events.push(event);
              break;
            case 'mousemove':
              if (Browser.pointerLock) {
                // workaround for firefox bug 750111
                if ('mozMovementX' in event) {
                  event['movementX'] = event['mozMovementX'];
                  event['movementY'] = event['mozMovementY'];
                }
                // workaround for Firefox bug 782777
                if (event['movementX'] == 0 && event['movementY'] == 0) {
                  // ignore a mousemove event if it doesn't contain any movement info
                  // (without pointer lock, we infer movement from pageX/pageY, so this check is unnecessary)
                  event.preventDefault();
                  return;
                }
              }
              // fall through
            case 'keydown': case 'keyup': case 'keypress': case 'mousedown': case 'mouseup': case 'DOMMouseScroll': case 'mousewheel':
              // If we preventDefault on keydown events, the subsequent keypress events
              // won't fire. However, it's fine (and in some cases necessary) to
              // preventDefault for keys that don't generate a character. Otherwise,
              // preventDefault is the right thing to do in general.
              if (event.type !== 'keydown' || (!SDL.unicode && !SDL.textInput) || (event.keyCode === 8 /* backspace */ || event.keyCode === 9 /* tab */)) {
                event.preventDefault();
              }

              if (event.type == 'DOMMouseScroll' || event.type == 'mousewheel') {
                var button = Browser.getMouseWheelDelta(event) > 0 ? 4 : 3;
                var event2 = {
                  type: 'mousedown',
                  button: button,
                  pageX: event.pageX,
                  pageY: event.pageY
                };
                SDL2Emscripten.events.push(event2);
                event = {
                  type: 'mouseup',
                  button: button,
                  pageX: event.pageX,
                  pageY: event.pageY
                };
              } else if (event.type == 'mousedown') {
                SDL.DOMButtons[event.button] = 1;
              } else if (event.type == 'mouseup') {
                // ignore extra ups, can happen if we leave the canvas while pressing down, then return,
                // since we add a mouseup in that case
                if (!SDL.DOMButtons[event.button]) {
                  return;
                }

                SDL.DOMButtons[event.button] = 0;
              }

              // We can only request fullscreen as the result of user input.
              // Due to this limitation, we toggle a boolean on keydown which
              // SDL_WM_ToggleFullScreen will check and subsequently set another
              // flag indicating for us to request fullscreen on the following
              // keyup. This isn't perfect, but it enables SDL_WM_ToggleFullScreen
              // to work as the result of a keypress (which is an extremely
              // common use case).
              if (event.type === 'keydown') {
                SDL.canRequestFullscreen = true;
              } else if (event.type === 'keyup') {
                if (SDL.isRequestingFullscreen) {
                  Module['requestFullScreen'](true, true);
                  SDL.isRequestingFullscreen = false;
                }
                SDL.canRequestFullscreen = false;
              }

              // SDL expects a unicode character to be passed to its keydown events.
              // Unfortunately, the browser APIs only provide a charCode property on
              // keypress events, so we must backfill in keydown events with their
              // subsequent keypress event's charCode.
              if (event.type === 'keypress' && SDL.savedKeydown) {
                // charCode is read-only
                SDL.savedKeydown.keypressCharCode = event.charCode;
                SDL.savedKeydown = null;
              } else if (event.type === 'keydown') {
                SDL.savedKeydown = event;
              }

              // Don't push keypress events unless SDL_StartTextInput has been called.
              if (event.type !== 'keypress' || SDL.textInput) {
                SDL2Emscripten.events.push(event);
              }
              break;
            case 'mouseout':
              // Un-press all pressed mouse buttons, because we might miss the release outside of the canvas
              for (var i = 0; i < 3; i++) {
                if (SDL.DOMButtons[i]) {
                  SDL2Emscripten.events.push({
                    type: 'mouseup',
                    button: i,
                    pageX: event.pageX,
                    pageY: event.pageY
                  });
                  SDL.DOMButtons[i] = 0;
                }
              }
              event.preventDefault();
              break;
            case 'blur':
            case 'visibilitychange': {
              // Un-press all pressed keys: TODO
              for (var code in SDL.keyboardMap) {
                SDL2Emscripten.events.push({
                  type: 'keyup',
                  keyCode: SDL.keyboardMap[code]
                });
              }
              event.preventDefault();
              break;
            }
            case 'unload':
              if (Browser.mainLoop.runner) {
                SDL2Emscripten.events.push(event);
                // Force-run a main event loop, since otherwise this event will never be caught!
                Browser.mainLoop.runner();
              }
              return;
            case 'resize':
              SDL2Emscripten.events.push(event);
              // manually triggered resize event doesn't have a preventDefault member
              if (event.preventDefault) {
                event.preventDefault();
              }
              break;
          }
          if (SDL2Emscripten.events.length >= 10000) {
            Module.printErr('SDL event queue full, dropping events');
            SDL2Emscripten.events = SDL2Emscripten.events.slice(0, 10000);
          }

          Browser.mainLoop.resume();
	},
    },

    SDL2_Emscripten_CreateWindow: function (width, height, x, y) {
      var canvas = document.createElement('canvas');
      canvas.width = width;
      canvas.height = height;
      canvas.style.left = x + 'px';
      canvas.style.top = y + 'px';
      document.body.appendChild(canvas);
      var id = SDL2Emscripten.getNewId (SDL2Emscripten.handles);
      canvas.setAttribute("id", "sdl_window_" + id);
      canvas.setAttribute("class", "sdl_window");
      SDL2Emscripten.windows[id] = canvas;
      return id;
    },

    SDL2_Emscripten_ShowWindow: function (canvas_id) {
      var canvas = SDL2Emscripten.windows[canvas_id];
      canvas.style.visibility = "visible";
    },

    SDL2_Emscripten_RequestFullScreen: function (canvas_id) {
      var canvas = SDL2Emscripten.windows[canvas_id];
      if(canvas.requestFullScreen)
        canvas.requestFullScreen();
      else if(canvas.webkitRequestFullScreen)
        canvas.webkitRequestFullScreen();
      else if(canvas.mozRequestFullScreen)
        canvas.mozRequestFullScreen();
    },
    SDL2_Emscripten_CancelFullScreen: function (canvas_id) {
      var canvas = SDL2Emscripten.windows[canvas_id];
      if(canvas.cancelFullscren)
        canvas.cancelFullscren();
      else if(canvas.webkitCancelFullScreen)
        canvas.webkitCancelFullScreen();
      else if(canvas.mozCancelFullScreen)
        canvas.mozCancelFullScreen();
    },

    SDL2_Emscripten_SetWindowSize: function (canvas_id, width, height) {
      var canvas = SDL2Emscripten.windows[canvas_id];
      canvas.width = width;
      canvas.height = height;
    },

    SDL2_Emscripten_DestroyWindow: function (canvas_id) {
      SDL2Emscripten.windows[canvas_id] = null;
    },

    SDL2_Emscripten_CreateContext__deps: ['emscripten_init_gl_extensions'],
    SDL2_Emscripten_CreateContext: function (canvas_id) {
      try {
        var canvas = SDL2Emscripten.windows[canvas_id];
        var ctx = canvas.getContext('webgl');
        var id = SDL2Emscripten.getNewId (SDL2Emscripten.handles);
        SDL2Emscripten.contexts[id] = ctx;
        GLctx = Module.ctx = ctx;
        _emscripten_init_gl_extensions();
        return id;
      } catch (e) {
        return 0;
      }
    },

    SDL2_Emscripten_MakeCurrent: function (context_id) {
      var ctx = SDL2Emscripten.contexts[context_id];
      GLctx = Module.ctx = ctx;
      return 1;
    },

    SDL2_Emscripten_DestroyContext: function (context_id) {
      SDL2Emscripten.contexts[id] = null;
    },

    SDL2_Emscripten_GetProcAddress__deps: ['emscripten_GetProcAddress'],
    SDL2_Emscripten_GetProcAddress: function(name_) {
      return _emscripten_GetProcAddress(name_);
    },

    SDL2_Emscripten_AddInputListener: function (canvas_id) {
      var canvas = SDL2Emscripten.windows[canvas_id];
      ['mousedown', 'mouseup', 'mousemove', 'DOMMouseScroll',
        'mousewheel', 'mouseout'].forEach(function(event) {
          canvas.addEventListener(event, SDL2Emscripten.receiveEvent, true);
        });
    },
};

autoAddDeps(LibrarySDL2Emscripten, '$SDL2Emscripten');
mergeInto(LibraryManager.library, LibrarySDL2Emscripten);
