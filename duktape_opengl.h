/*
 *  Duktape OpenGL wrapper 0.1.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
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

DUK_EXTERNAL_DECL void duk_gl_bind_opengl_functions(duk_context *ctx);

/*
 *  C++ name mangling
 */

#ifdef __cplusplus
/* end 'extern "C"' wrapper */
}
#endif

#endif  /* DUK_GL_API_PUBLIC_H_INCLUDED */

