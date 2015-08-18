# duktape-opengl
OpenGL bindings for Duktape embeddable Javascript engine

If you ended up here randomly and have no idea what Duktape is, then have a look at here: http://duktape.org/

Setup
=====
* Include duktape_opengl.c and duktape_opengl.h to your Duktape&OpenGL project.
* To initialize OpenGL bindings, call function: void duk_gl_push_opengl_bindings(duk_context *ctx)

* Currently by default only OpenGL 1.1 bindings will be initialized. If you want to include other bindings, such as ARB extensions, then add defines.
* If you're using OpenGL corearb then use duktape_opengl_corearb.c instead of duktape_opengl.c.

Defines
=======
* DUK_GL_OPENGL_4X  - Enable OpenGL 4.x
* DUK_GL_OPENGL_3X  - Enable OpenGL 3.x
* DUK_GL_OPENGL_2X  - Enable OpenGL 2.x
* DUK_GL_OPENGL_1X  - Enable OpenGL 1.x
* DUK_GL_OPENGL_N_X - Enable OpenGL N.X (for example DUK_GL_OPENGL_1_2)
* DUK_GL_ARB        – Enable extensions officially approved by the OpenGL Architecture Review Board
* DUK_GL_ATI        – Enable ATI Technologies extensions
* DUK_GL_EXT        – Enable extensions agreed upon by multiple OpenGL vendors
* DUK_GL_HP         – Enable Hewlett-Packard extensions
* DUK_GL_IBM        – Enable International Business Machines extensions
* DUK_GL_KTX        – Enable Kinetix extensions
* DUK_GL_INTEL      – Enable Intel extensions
* DUK_GL_NV         – Enable NVIDIA Corporation extensions
* DUK_GL_MESA       – Enable MESA implementation extensions
* DUK_GL_SGI        – Enable Silicon Graphics extensions
* DUK_GL_SGIX       – Enable Silicon Graphics (experimental) extensions
* DUK_GL_SUN        – Enable Sun Microsystems extensions
* DUK_GL_WIN        – Enable Microsoft extensions

Prerequirements
===============
Duktape and OpenGL headers APIs needed for these bindings to be useful.

Greetings
=========
Greetings fly out to the authors of the Duktape API. Splendid work!
