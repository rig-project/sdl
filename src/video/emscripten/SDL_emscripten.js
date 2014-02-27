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
    },

    SDL2_Emscripten_CreateWindow: function (width, height, x, y) {
	var canvas = document.createElement('canvas');
	canvas.width = width;
	canvas.height = height;
	canvas.style.left = x + 'px';
	canvas.style.top = y + 'px';
	document.body.appendChild(canvas);
	var id = SDL2Emscripten.getNewId (SDL2Emscripten.handles);
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
	    return 1;
	} catch (e) {
	    return 0;
	}
    },

    SDL2_Emscripten_MakeCurrent: function (context_id) {
	var ctx = SDL2Emscripten.contexts[context_id];
	GLctx = Module.ctx = ctx;
	GL.initExtensions();
	return 1;
    },

    SDL2_Emscripten_DestroyContext: function (context_id) {
	SDL2Emscripten.contexts[id] = null;
    },

    SDL2_Emscripten_GetProcAddress__deps: ['emscripten_GetProcAddress'],
    SDL2_Emscripten_GetProcAddress: function(name_) {
	return _emscripten_GetProcAddress(name_);
    },

};

autoAddDeps(LibrarySDL2Emscripten, '$SDL2Emscripten');
mergeInto(LibraryManager.library, LibrarySDL2Emscripten);
