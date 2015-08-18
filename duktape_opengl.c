/*
 *  Duktape OpenGL wrapper 0.1.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 */

#include <duktape.h>
#include <GL/gl.h>

#define DUK_GL_OPENGL_BASIC

/*
 *  Bindings that are not enabled by default.
 */
/*#define DUK_GL_ARB*/
/*#define DUK_GL_ATI*/
/*#define DUK_GL_MESA*/
/*#define DUK_GL_OPENGL_1_2*/
/*#define DUK_GL_OPENGL_1_3*/

/*
 *  Macro for binding OpenGL wrapper C function as Duktape JavaScript function
 */
#define duk_gl_bind_opengl_wrapper(ctx, c_function_name, argument_count) \
	duk_push_c_function((ctx), duk_gl_##c_function_name, (argument_count)); \
	duk_put_prop_string((ctx), -2, #c_function_name)

/*
 *  Macro for setting OpenGL constants
 */
#define duk_gl_push_opengl_constant_property(ctx, opengl_constant) \
	duk_push_uint((ctx), (opengl_constant)); \
	duk_put_prop_string((ctx), -2, #opengl_constant)

/*
 *  Wrapper macros for OpenGL C functions.
 *  Macro name defines the amount of return types and function arguments it supports.
 *  Macros take arguments in following manner (<Function name>, <OpenGL variable type 1>, <Duktape push type 1>, ...N)
 *  - Function name = OpenGL C function name
 *  - OpenGL variable type N = OpenGL's C variable type definition, for example, "GLenum"
 *  - Duktape push type 1 = Duktape API's duk_to_... function's type name, for example, duk_to_number
 */
#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(c_function_name) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name(); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(c_function_name, rettypedef1) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name()); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(c_function_name, argtypedef1, arg1) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(c_function_name, rettypedef1, argtypedef1, arg1) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(c_function_name, argtypedef1, arg1, argtypedef2, arg2) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG4(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG6(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG7(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6),  \
		(argtypedef8)duk_to_##arg8(ctx, 7) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG8(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6),  \
		(argtypedef8)duk_to_##arg8(ctx, 7) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6),  \
		(argtypedef8)duk_to_##arg8(ctx, 7),  \
		(argtypedef9)duk_to_##arg9(ctx, 8) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG9(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2),  \
		(argtypedef4)duk_to_##arg4(ctx, 3),  \
		(argtypedef5)duk_to_##arg5(ctx, 4),  \
		(argtypedef6)duk_to_##arg6(ctx, 5),  \
		(argtypedef7)duk_to_##arg7(ctx, 6),  \
		(argtypedef8)duk_to_##arg8(ctx, 7),  \
		(argtypedef9)duk_to_##arg9(ctx, 8) \
	)); \
	return 1; \
}

/*
 *  OpenGL wrapper function definitions
 */
#ifdef DUK_GL_ARB
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveTextureARB, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClientActiveTextureARB, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1dARB, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1fARB, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1iARB, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1sARB, GLenum, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2dARB, GLenum, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2fARB, GLenum, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2iARB, GLenum, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2sARB, GLenum, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3dARB, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3fARB, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3iARB, GLenum, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3sARB, GLenum, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4dARB, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4fARB, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4iARB, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4sARB, GLenum, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationSeparateATI, GLenum, uint, GLenum, uint)
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_MESA
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableTraceMESA, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableTraceMESA, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndTraceMESA)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTraceAssertAttribMESA, GLbitfield, uint)
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_OPENGL_1_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTexSubImage3D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorSubTable, GLenum, uint, GLsizei, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorTable, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBlendEquation, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColor, GLclampf, number, GLclampf, number, GLclampf, number, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glHistogram, GLenum, uint, GLsizei, int, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetHistogram, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMinmax, GLenum, uint, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetMinmax, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyConvolutionFilter1D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyConvolutionFilter2D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveTexture, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClientActiveTexture, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1d, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1f, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1i, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1s, GLenum, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2d, GLenum, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2f, GLenum, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2i, GLenum, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2s, GLenum, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3d, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3f, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3i, GLenum, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3s, GLenum, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4d, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4f, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4i, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4s, GLenum, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleCoverage, GLclampf, number, GLboolean, boolean)
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_BASIC
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearIndex, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColor, GLclampf, number, GLclampf, number, GLclampf, number, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClear, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexMask, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColorMask, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAlphaFunc, GLenum, uint, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendFunc, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLogicOp, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCullFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFrontFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPointSize, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLineWidth, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLineStipple, GLint, int, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonMode, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonOffset, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEdgeFlag, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glScissor, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDrawBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReadBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsEnabled, boolean, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopAttrib)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushClientAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopClientAttrib)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glRenderMode, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetError, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFinish)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFlush)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glHint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepth, GLclampd, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthFunc, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthMask, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRange, GLclampd, number, GLclampd, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearAccum, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAccum, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMatrixMode, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glOrtho, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFrustum, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glViewport, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPushMatrix)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopMatrix)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glLoadIdentity)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRotated, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRotatef, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glScaled, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glScalef, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTranslated, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTranslatef, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsList, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeleteLists, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenLists, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNewList, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndList)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCallList, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glListBase, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBegin, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEnd)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2i, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2s, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4d, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4f, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4i, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4s, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3b, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexd, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexf, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexi, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexs, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexub, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3b, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3ub, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3ui, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3us, GLushort, uint, GLushort, uint, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4b, GLbyte, int, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4d, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4f, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4i, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4s, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4ub, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4ui, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4us, GLushort, uint, GLushort, uint, GLushort, uint, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1d, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1f, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1i, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1s, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2i, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2s, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4d, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4f, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4i, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4s, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterPos2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterPos2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterPos2i, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterPos2s, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glRasterPos3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glRasterPos3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glRasterPos3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glRasterPos3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRasterPos4d, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRasterPos4f, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRasterPos4i, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRasterPos4s, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRectd, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRectf, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRecti, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRects, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glArrayElement, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawArrays, GLenum, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glShadeModel, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glLightf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glLighti, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightModelf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightModeli, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMaterialf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMateriali, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glColorMaterial, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelZoom, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStoref, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStorei, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTransferf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTransferi, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyPixels, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilFunc, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glStencilMask, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilOp, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearStencil, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGend, GLenum, uint, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGenf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGeni, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexEnvf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexEnvi, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTexture, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTexture, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyTexImage1D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexImage2D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTexSubImage1D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexSubImage2D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalCoord1d, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalCoord1f, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalCoord2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalCoord2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMapGrid1d, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMapGrid1f, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glMapGrid2d, GLint, int, GLdouble, number, GLdouble, number, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glMapGrid2f, GLint, int, GLfloat, number, GLfloat, number, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalPoint1, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalPoint2, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glEvalMesh1, GLenum, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glEvalMesh2, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogi, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPassThrough, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glInitNames)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLoadName, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushName, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopName)
#endif /* DUK_GL_OPENGL_BASIC */


/*
 *  OpenGL function bindings to JavaScript
 */
void duk_gl_bind_opengl_functions(duk_context *ctx)
{
	duk_push_global_object(ctx);
#ifdef DUK_GL_ARB
	duk_gl_bind_opengl_wrapper(ctx, glActiveTextureARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClientActiveTextureARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1dARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1fARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1iARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1sARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2dARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2fARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2iARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2sARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3dARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3fARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3iARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3sARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4dARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4fARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4iARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4sARB, 5);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateATI, 2);
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_MESA
	duk_gl_bind_opengl_wrapper(ctx, glEnableTraceMESA, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisableTraceMESA, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEndTraceMESA, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTraceAssertAttribMESA, 1);
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage3D, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorSubTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquation, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glHistogram, 4);
	duk_gl_bind_opengl_wrapper(ctx, glResetHistogram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinmax, 3);
	duk_gl_bind_opengl_wrapper(ctx, glResetMinmax, 1);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter1D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter2D, 6);
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
	duk_gl_bind_opengl_wrapper(ctx, glActiveTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClientActiveTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4s, 5);
	duk_gl_bind_opengl_wrapper(ctx, glSampleCoverage, 2);
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_BASIC
	duk_gl_bind_opengl_wrapper(ctx, glClearIndex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClear, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColorMask, 4);
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFunc, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunc, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLogicOp, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCullFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFrontFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPointSize, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLineWidth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLineStipple, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonMode, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffset, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEdgeFlag, 1);
	duk_gl_bind_opengl_wrapper(ctx, glScissor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glDrawBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glReadBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabled, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPopAttrib, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPushClientAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPopClientAttrib, 0);
	duk_gl_bind_opengl_wrapper(ctx, glRenderMode, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetError, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFinish, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFlush, 0);
	duk_gl_bind_opengl_wrapper(ctx, glHint, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthFunc, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRange, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearAccum, 4);
	duk_gl_bind_opengl_wrapper(ctx, glAccum, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixMode, 1);
	duk_gl_bind_opengl_wrapper(ctx, glOrtho, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFrustum, 6);
	duk_gl_bind_opengl_wrapper(ctx, glViewport, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPushMatrix, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPopMatrix, 0);
	duk_gl_bind_opengl_wrapper(ctx, glLoadIdentity, 0);
	duk_gl_bind_opengl_wrapper(ctx, glRotated, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRotatef, 4);
	duk_gl_bind_opengl_wrapper(ctx, glScaled, 3);
	duk_gl_bind_opengl_wrapper(ctx, glScalef, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTranslated, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTranslatef, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsList, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteLists, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenLists, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNewList, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndList, 0);
	duk_gl_bind_opengl_wrapper(ctx, glCallList, 1);
	duk_gl_bind_opengl_wrapper(ctx, glListBase, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBegin, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnd, 0);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3b, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIndexd, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexf, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexi, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexs, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexub, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColor3b, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3ub, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor3us, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor4b, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4ub, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColor4us, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1d, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1f, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1i, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1s, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos2i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos2s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos4d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos4f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos4i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos4s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRectd, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRectf, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRecti, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRects, 4);
	duk_gl_bind_opengl_wrapper(ctx, glArrayElement, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArrays, 3);
	duk_gl_bind_opengl_wrapper(ctx, glShadeModel, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLightf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLighti, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLightModelf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLightModeli, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMaterialf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMateriali, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColorMaterial, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelZoom, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStoref, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStorei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransferf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransferi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCopyPixels, 5);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFunc, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOp, 3);
	duk_gl_bind_opengl_wrapper(ctx, glClearStencil, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexGend, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGenf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGeni, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexEnvf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexEnvi, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindTexture, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage1D, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord1d, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord1f, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid1d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid1f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid2d, 6);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid2f, 6);
	duk_gl_bind_opengl_wrapper(ctx, glEvalPoint1, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalPoint2, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEvalMesh1, 3);
	duk_gl_bind_opengl_wrapper(ctx, glEvalMesh2, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFogf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFogi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPassThrough, 1);
	duk_gl_bind_opengl_wrapper(ctx, glInitNames, 0);
	duk_gl_bind_opengl_wrapper(ctx, glLoadName, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushName, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPopName, 0);
#endif /* DUK_GL_OPENGL_BASIC */

	duk_pop(ctx);
}

/*
 *  OpenGL constants to JavaScript
 */
void duk_gl_set_constants(duk_context *ctx)
{
	duk_push_global_object(ctx);
#ifdef DUK_GL_ARB
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE0_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE1_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE5_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE6_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE7_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE8_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE9_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE10_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE11_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE12_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE13_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE14_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE15_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE16_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE17_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE18_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE19_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE20_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE21_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE22_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE23_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE24_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE25_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE26_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE27_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE28_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE29_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE30_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE31_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_TEXTURE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ACTIVE_TEXTURE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_UNITS_ARB);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BLEND_EQUATION_ATI);
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_MESA
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ALL_BITS_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_OPERATIONS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_PRIMITIVES_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ARRAYS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_TEXTURES_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_PIXELS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ERRORS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_MASK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_NAME_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_24_8_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_24_REV_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_15_1_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_1_15_REV_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_POSITION_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_FUNC_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_DATA_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_POSITION_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_FUNC_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_DATA_MESA);
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_push_opengl_constant_property(ctx, GL_RESCALE_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_EDGE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_INDICES);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE_3_3_2);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE_2_3_3_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_6_5);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_6_5_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_4_4_4_4);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_4_4_4_4_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_5_5_1);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_1_5_5_5_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_8_8);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_8_8_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_10_10_10_2);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_2_10_10_10_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_COLOR_CONTROL);
	duk_gl_push_opengl_constant_property(ctx, GL_SINGLE_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_SPECULAR_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_LOD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LOD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BASE_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_ALIASED_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALIASED_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_IMAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_IMAGE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_IMAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_IMAGE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_R);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_3D_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_INTENSITY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARABLE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_REDUCE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLICATE_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_TABLE_TOO_LARGE);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_REVERSE_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR);
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE0);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE1);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE2);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE3);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE4);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE5);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE6);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE7);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE8);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE9);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE10);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE11);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE12);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE13);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE14);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE15);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE16);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE17);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE18);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE19);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE20);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE21);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE22);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE23);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE24);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE25);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE26);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE27);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE28);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE29);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE30);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE31);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ACTIVE_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_REFLECTION_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CUBE_MAP_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_INTENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSION_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_MODELVIEW_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_PROJECTION_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_TEXTURE_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_COLOR_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD_SIGNED);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERPOLATE);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMARY_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_PREVIOUS);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_BORDER);
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_BASIC
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_1);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_2);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_3);
	duk_gl_push_opengl_constant_property(ctx, GL_FALSE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRUE);
	duk_gl_push_opengl_constant_property(ctx, GL_BYTE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE);
	duk_gl_push_opengl_constant_property(ctx, GL_SHORT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_2_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_3_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_4_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINTS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_LOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLES);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_FAN);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_V2F);
	duk_gl_push_opengl_constant_property(ctx, GL_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4UB_V2F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4UB_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_C3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T4F_V4F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C4UB_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C4F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T4F_C4F_N3F_V4F);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE_PATTERN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_CW);
	duk_gl_push_opengl_constant_property(ctx, GL_CCW);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_STIPPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FACTOR);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPILE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPILE_AND_EXECUTE);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_BASE);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_NEVER);
	duk_gl_push_opengl_constant_property(ctx, GL_LESS);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_LEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_GREATER);
	duk_gl_push_opengl_constant_property(ctx, GL_NOTEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_GEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_ALWAYS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTING);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT0);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT1);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT2);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT3);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT4);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT5);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT6);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT7);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_EXPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_CUTOFF);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADRATIC_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_AMBIENT);
	duk_gl_push_opengl_constant_property(ctx, GL_DIFFUSE);
	duk_gl_push_opengl_constant_property(ctx, GL_SPECULAR);
	duk_gl_push_opengl_constant_property(ctx, GL_SHININESS);
	duk_gl_push_opengl_constant_property(ctx, GL_EMISSION);
	duk_gl_push_opengl_constant_property(ctx, GL_POSITION);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_DIRECTION);
	duk_gl_push_opengl_constant_property(ctx, GL_AMBIENT_AND_DIFFUSE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEXES);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_TWO_SIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_LOCAL_VIEWER);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_AMBIENT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_AND_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADE_MODEL);
	duk_gl_push_opengl_constant_property(ctx, GL_FLAT);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL_PARAMETER);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMALIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE0);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE1);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE2);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE3);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE4);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE5);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_RED_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_GREEN_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_BLUE_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_ALPHA_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_LOAD);
	duk_gl_push_opengl_constant_property(ctx, GL_MULT);
	duk_gl_push_opengl_constant_property(ctx, GL_RETURN);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST_REF);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA_SATURATE);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDER);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECT);
	duk_gl_push_opengl_constant_property(ctx, GL_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_3D_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_3D_COLOR_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_4D_COLOR_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_RESET_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_BITMAP_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_PIXEL_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_PIXEL_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_PASS_THROUGH_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECTION_BUFFER_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECTION_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_DENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_START);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_END);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_EXP);
	duk_gl_push_opengl_constant_property(ctx, GL_EXP2);
	duk_gl_push_opengl_constant_property(ctx, GL_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_LOGIC_OP_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_SET);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_NOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_AND);
	duk_gl_push_opengl_constant_property(ctx, GL_NAND);
	duk_gl_push_opengl_constant_property(ctx, GL_OR);
	duk_gl_push_opengl_constant_property(ctx, GL_NOR);
	duk_gl_push_opengl_constant_property(ctx, GL_XOR);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUIV);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_VALUE_MASK);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_REF);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_PASS_DEPTH_PASS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_PASS_DEPTH_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_KEEP);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR);
	duk_gl_push_opengl_constant_property(ctx, GL_NONE);
	duk_gl_push_opengl_constant_property(ctx, GL_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX0);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX1);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX2);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX3);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_RED);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBPIXEL_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_STEREO);
	duk_gl_push_opengl_constant_property(ctx, GL_BITMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_DITHER);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LIST_NESTING);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_MODELVIEW_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_NAME_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROJECTION_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_EVAL_ORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LIGHTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIP_PLANES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PIXEL_MAP_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VIEWPORT_DIMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_DISTANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_POSITION);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_TEXTURE_COORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_POSITION_VALID);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_TEXTURE_COORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_NAME_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT);
	duk_gl_push_opengl_constant_property(ctx, GL_AUTO_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_COLOR_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_1);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_2);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_COLOR_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_1);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_2);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_GRID_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_GRID_SEGMENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_GRID_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_GRID_SEGMENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_COEFF);
	duk_gl_push_opengl_constant_property(ctx, GL_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_ORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_PERSPECTIVE_CORRECTION_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_DONT_CARE);
	duk_gl_push_opengl_constant_property(ctx, GL_FASTEST);
	duk_gl_push_opengl_constant_property(ctx, GL_NICEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_BOX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_SHIFT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_S_TO_S_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_I_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_R_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_G_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_B_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_A_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_R_TO_R_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_G_TO_G_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_B_TO_B_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_A_TO_A_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_S_TO_S);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_I);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_R);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_G);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_B);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_A);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_R_TO_R);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_G_TO_G);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_B_TO_B);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_A_TO_A);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_LSB_FIRST);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ROW_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_ROWS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SWAP_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_LSB_FIRST);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_ROW_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_ROWS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SWAP_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_ZOOM_X);
	duk_gl_push_opengl_constant_property(ctx, GL_ZOOM_Y);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_S);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_T);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAG_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_S);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_T);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTENSITY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_PLANE);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_PLANE);
	duk_gl_push_opengl_constant_property(ctx, GL_SPHERE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_DECAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MODULATE);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP);
	duk_gl_push_opengl_constant_property(ctx, GL_S);
	duk_gl_push_opengl_constant_property(ctx, GL_T);
	duk_gl_push_opengl_constant_property(ctx, GL_R);
	duk_gl_push_opengl_constant_property(ctx, GL_Q);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_R);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_Q);
	duk_gl_push_opengl_constant_property(ctx, GL_VENDOR);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_EXTENSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_ERROR);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_ENUM);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_OPERATION);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_OVERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_UNDERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_OUT_OF_MEMORY);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_STIPPLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MODE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTING_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ENABLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_HINT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_ATTRIB_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_PRIORITY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RESIDENT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTERNAL_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA12);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE6_ALPHA2);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA12);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_ALPHA16);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY4);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY12);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16);
	duk_gl_push_opengl_constant_property(ctx, GL_R3_G3_B2);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB4);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB12);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA2);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA4);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5_A1);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10_A2);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA12);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_PIXEL_STORE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_VERTEX_ARRAY_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_CLIENT_ATTRIB_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ALL_ATTRIB_BITS);
#endif /* DUK_GL_OPENGL_BASIC */

	duk_pop(ctx);
}

