/*
 *  Duktape OpenGL 0.5 - OpenGL bindings for Duktape embeddable Javascript engine.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 *  
 *  Duktape OpenGL: https://github.com/mrautio/duktape-opengl/
 *  Duktape: http://duktape.org/
 *  OpenGL API reference: https://www.opengl.org/registry/
 */

/*
 *  BEGIN PUBLIC API
 */
#ifndef DUK_GL_API_PUBLIC_H_INCLUDED
#define DUK_GL_API_PUBLIC_H_INCLUDED

/*
 *  Avoid C++ name mangling
 */

#ifdef __cplusplus
extern "C" {
#endif

DUK_EXTERNAL_DECL void duk_gl_push_opengl_bindings(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_gl_bind_opengl_functions(duk_context *ctx);
DUK_EXTERNAL_DECL void duk_gl_set_constants(duk_context *ctx);

/*
 *  C++ name mangling
 */

#ifdef __cplusplus
/* end 'extern "C"' wrapper */
}
#endif

#endif  /* DUK_GL_API_PUBLIC_H_INCLUDED */

