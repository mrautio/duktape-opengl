/*
 *  Duktape OpenGL wrapper 0.1.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 */

#include <duktape.h>
#include <GL/glcorearb.h>

#define DUK_GL_OPENGL_1_1

/*
 *  Bindings that are not enabled by default.
 */
/*#define DUK_GL_ARB*/
#ifdef DUK_GL_OPENGL_4X
#ifdef DUK_GL_OPENGL_3X
#ifdef DUK_GL_OPENGL_2X
#ifdef DUK_GL_OPENGL_1X
#define DUK_GL_OPENGL_1_1
#define DUK_GL_OPENGL_1_2
#define DUK_GL_OPENGL_1_3
#define DUK_GL_OPENGL_1_4
#define DUK_GL_OPENGL_1_5
#endif /*DUK_GL_OPENGL_1X*/
#define DUK_GL_OPENGL_2_0
#define DUK_GL_OPENGL_2_1
#endif /*DUK_GL_OPENGL_2X*/
#define DUK_GL_OPENGL_3_0
#define DUK_GL_OPENGL_3_1
#define DUK_GL_OPENGL_3_2
#define DUK_GL_OPENGL_3_3
#endif /*DUK_GL_OPENGL_3X*/
#define DUK_GL_OPENGL_4_0
#define DUK_GL_OPENGL_4_1
#define DUK_GL_OPENGL_4_2
#define DUK_GL_OPENGL_4_3
#define DUK_GL_OPENGL_4_4
#define DUK_GL_OPENGL_4_5
#endif /*DUK_GL_OPENGL_4X*/

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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name(); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(c_function_name, rettypedef1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name()); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(c_function_name, argtypedef1, arg1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(c_function_name, rettypedef1, argtypedef1, arg1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(c_function_name, argtypedef1, arg1, argtypedef2, arg2) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0),  \
		(argtypedef2)duk_to_##arg2(ctx, 1),  \
		(argtypedef3)duk_to_##arg3(ctx, 2) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG10(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG11(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG11(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG12(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG13(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG13(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG14(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12),  \
		(argtypedef14)duk_to_##arg14(ctx, 13) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG14(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12),  \
		(argtypedef14)duk_to_##arg14(ctx, 13) \
	)); \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14, argtypedef15, arg15) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12),  \
		(argtypedef14)duk_to_##arg14(ctx, 13),  \
		(argtypedef15)duk_to_##arg15(ctx, 14) \
	); \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG15(c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14, argtypedef15, arg15) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
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
		(argtypedef9)duk_to_##arg9(ctx, 8),  \
		(argtypedef10)duk_to_##arg10(ctx, 9),  \
		(argtypedef11)duk_to_##arg11(ctx, 10),  \
		(argtypedef12)duk_to_##arg12(ctx, 11),  \
		(argtypedef13)duk_to_##arg13(ctx, 12),  \
		(argtypedef14)duk_to_##arg14(ctx, 13),  \
		(argtypedef15)duk_to_##arg15(ctx, 14) \
	)); \
	return 1; \
}

/*
 *  OpenGL wrapper function definitions
 */
#ifdef DUK_GL_ARB
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendEquationSeparateiARB, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationiARB, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glBlendFuncSeparateiARB, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendFunciARB, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glDispatchComputeGroupSizeARB, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetGraphicsResetStatusARB, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(glGetImageHandleARB, uint, GLuint, uint, GLint, int, GLboolean, boolean, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGetTextureHandleARB, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetTextureSamplerHandleARB, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsImageHandleResidentARB, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTextureHandleResidentARB, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeImageHandleNonResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMakeImageHandleResidentARB, GLuint64, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleNonResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMinSampleShadingARB, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniformHandleui64ARB, GLuint, uint, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glTexPageCommitmentARB, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniformHandleui64ARB, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1ui64ARB, GLuint, uint, GLuint64EXT, uint)
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_OPENGL_1_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTexture, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendFunc, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClear, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColor, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepth, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearStencil, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColorMask, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyTexImage1D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexImage2D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTexSubImage1D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexSubImage2D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCullFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthFunc, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthMask, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRange, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawArrays, GLenum, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDrawBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFinish)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFlush)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFrontFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetError, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glHint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsEnabled, boolean, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTexture, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLineWidth, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLogicOp, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStoref, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStorei, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPointSize, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonMode, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonOffset, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReadBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glScissor, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilFunc, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glStencilMask, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilOp, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glViewport, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTexSubImage3D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveTexture, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleCoverage, GLfloat, number, GLboolean, boolean)
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColor, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBlendEquation, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendFuncSeparate, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameterf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameteri, GLenum, uint, GLint, int)
#endif /* DUK_GL_OPENGL_1_4 */

#ifdef DUK_GL_OPENGL_1_5
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBeginQuery, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindBuffer, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEndQuery, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsBuffer, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsQuery, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glUnmapBuffer, boolean, GLenum, uint)
#endif /* DUK_GL_OPENGL_1_5 */

#ifdef DUK_GL_OPENGL_2_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAttachShader, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationSeparate, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCompileShader, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glCreateProgram, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glCreateShader, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteProgram, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteShader, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDetachShader, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableVertexAttribArray, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableVertexAttribArray, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsProgram, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsShader, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLinkProgram, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilFuncSeparate, GLenum, uint, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glStencilMaskSeparate, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilOpSeparate, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1f, GLint, int, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1i, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2f, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3f, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3i, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4f, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4i, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glUseProgram, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glValidateProgram, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1d, GLuint, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1f, GLuint, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1s, GLuint, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2d, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2f, GLuint, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2s, GLuint, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3d, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3f, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3s, GLuint, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4Nub, GLuint, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4d, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4f, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4s, GLuint, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_3_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBeginConditionalRender, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginTransformFeedback, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindBufferBase, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindFramebuffer, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindRenderbuffer, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBindVertexArray, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glBlitFramebuffer, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLbitfield, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glCheckFramebufferStatus, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glClampColor, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearBufferfi, GLenum, uint, GLint, int, GLfloat, number, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glColorMaski, GLuint, uint, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisablei, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnablei, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndConditionalRender)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFramebufferRenderbuffer, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTexture1D, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTexture2D, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFramebufferTexture3D, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTextureLayer, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGenerateMipmap, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsEnabledi, boolean, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsFramebuffer, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsRenderbuffer, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsVertexArray, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRenderbufferStorage, GLenum, uint, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glRenderbufferStorageMultisample, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1ui, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2ui, GLint, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3ui, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4ui, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribI1i, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribI1ui, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribI2i, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribI2ui, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribI3i, GLuint, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribI3ui, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribI4i, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribI4ui, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
#endif /* DUK_GL_OPENGL_3_0 */

#ifdef DUK_GL_OPENGL_3_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawArraysInstanced, GLenum, uint, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPrimitiveRestartIndex, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexBuffer, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniformBlockBinding, GLuint, uint, GLuint, uint, GLuint, uint)
#endif /* DUK_GL_OPENGL_3_1 */

#ifdef DUK_GL_OPENGL_3_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFramebufferTexture, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glProvokingVertex, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleMaski, GLuint, uint, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTexImage2DMultisample, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTexImage3DMultisample, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
#endif /* DUK_GL_OPENGL_3_2 */

#ifdef DUK_GL_OPENGL_3_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindSampler, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsSampler, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glQueryCounter, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSamplerParameterf, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSamplerParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribDivisor, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP1ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP2ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP3ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP4ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
#endif /* DUK_GL_OPENGL_3_3 */

#ifdef DUK_GL_OPENGL_4_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBeginQueryIndexed, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTransformFeedback, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendEquationSeparatei, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationi, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glBlendFuncSeparatei, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendFunci, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDrawTransformFeedback, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawTransformFeedbackStream, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEndQueryIndexed, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTransformFeedback, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMinSampleShading, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPatchParameteri, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPauseTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glResumeTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1d, GLint, int, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2d, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3d, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4d, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
#endif /* DUK_GL_OPENGL_4_0 */

#ifdef DUK_GL_OPENGL_4_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glActiveShaderProgram, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBindProgramPipeline, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepthf, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDepthRangeIndexed, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRangef, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsProgramPipeline, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1d, GLuint, uint, GLint, int, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1f, GLuint, uint, GLint, int, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1i, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1ui, GLuint, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2d, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2f, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2i, GLuint, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2ui, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3d, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3f, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3i, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3ui, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4d, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4f, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4i, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4ui, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glReleaseShaderCompiler)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glScissorIndexed, GLuint, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUseProgramStages, GLuint, uint, GLbitfield, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glValidateProgramPipeline, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1d, GLuint, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribL2d, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribL3d, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribL4d, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glViewportIndexedf, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
#endif /* DUK_GL_OPENGL_4_1 */

#ifdef DUK_GL_OPENGL_4_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glBindImageTexture, GLuint, uint, GLuint, uint, GLint, int, GLboolean, boolean, GLint, int, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glDrawArraysInstancedBaseInstance, GLenum, uint, GLint, int, GLsizei, int, GLsizei, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawTransformFeedbackInstanced, GLenum, uint, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawTransformFeedbackStreamInstanced, GLenum, uint, GLuint, uint, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMemoryBarrier, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexStorage1D, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glTexStorage2D, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTexStorage3D, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int)
#endif /* DUK_GL_OPENGL_4_2 */

#ifdef DUK_GL_OPENGL_4_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(glCopyImageSubData, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDispatchCompute, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFramebufferParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glInvalidateBufferData, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glInvalidateTexImage, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glInvalidateTexSubImage, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopDebugGroup)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glShaderStorageBlockBinding, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTexStorage2DMultisample, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTexStorage3DMultisample, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTextureView, GLuint, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribBinding, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribFormat, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribIFormat, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribLFormat, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexBindingDivisor, GLuint, uint, GLuint, uint)
#endif /* DUK_GL_OPENGL_4_3 */

#ifdef DUK_GL_OPENGL_4_5
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTextureUnit, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(glBlitNamedFramebuffer, GLuint, uint, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLbitfield, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glCheckNamedFramebufferStatus, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glClipControl, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTextureSubImage1D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTextureSubImage2D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTextureSubImage3D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableVertexArrayAttrib, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableVertexArrayAttrib, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGenerateTextureMipmap, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetGraphicsResetStatus, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMemoryBarrierByRegion, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNamedFramebufferDrawBuffer, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNamedFramebufferParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNamedFramebufferReadBuffer, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferRenderbuffer, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferTexture, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTextureLayer, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedRenderbufferStorage, GLuint, uint, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedRenderbufferStorageMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glTextureBarrier)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureBuffer, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureParameterf, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureStorage1D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glTextureStorage2D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTextureStorage2DMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTextureStorage3D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTextureStorage3DMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTransformFeedbackBufferBase, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glUnmapNamedBuffer, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayAttribBinding, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glVertexArrayAttribFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayAttribIFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayAttribLFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayBindingDivisor, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexArrayElementBuffer, GLuint, uint, GLuint, uint)
#endif /* DUK_GL_OPENGL_4_5 */

/*
 *  OpenGL function bindings to JavaScript
 */
void duk_gl_bind_opengl_functions(duk_context *ctx)
{
#ifdef DUK_GL_ARB
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateiARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationiARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparateiARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunciARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchComputeGroupSizeARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatusARB, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetImageHandleARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureHandleARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureSamplerHandleARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsImageHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleResidentARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinSampleShadingARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformHandleui64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexPageCommitmentARB, 9);
	duk_gl_bind_opengl_wrapper(ctx, glUniformHandleui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1ui64ARB, 2);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_OPENGL_1_1
	duk_gl_bind_opengl_wrapper(ctx, glBindTexture, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunc, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClear, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearStencil, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColorMask, 4);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage1D, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCullFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthFunc, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRange, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArrays, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFinish, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFlush, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFrontFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetError, 0);
	duk_gl_bind_opengl_wrapper(ctx, glHint, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabled, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLineWidth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLogicOp, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStoref, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStorei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointSize, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonMode, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffset, 2);
	duk_gl_bind_opengl_wrapper(ctx, glReadBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glScissor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFunc, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOp, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glViewport, 4);
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage3D, 9);
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
	duk_gl_bind_opengl_wrapper(ctx, glActiveTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSampleCoverage, 2);
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
	duk_gl_bind_opengl_wrapper(ctx, glBlendColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquation, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameteri, 2);
#endif /* DUK_GL_OPENGL_1_4 */

#ifdef DUK_GL_OPENGL_1_5
	duk_gl_bind_opengl_wrapper(ctx, glBeginQuery, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndQuery, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsQuery, 1);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapBuffer, 1);
#endif /* DUK_GL_OPENGL_1_5 */

#ifdef DUK_GL_OPENGL_2_0
	duk_gl_bind_opengl_wrapper(ctx, glAttachShader, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparate, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCompileShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCreateProgram, 0);
	duk_gl_bind_opengl_wrapper(ctx, glCreateShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDetachShader, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexAttribArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexAttribArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLinkProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFuncSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilMaskSeparate, 2);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOpSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUseProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glValidateProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nub, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4s, 5);
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_3_0
	duk_gl_bind_opengl_wrapper(ctx, glBeginConditionalRender, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBeginTransformFeedback, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBindBufferBase, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindFramebuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindRenderbuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindVertexArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlitFramebuffer, 10);
	duk_gl_bind_opengl_wrapper(ctx, glCheckFramebufferStatus, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClampColor, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearBufferfi, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColorMaski, 5);
	duk_gl_bind_opengl_wrapper(ctx, glDisablei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnablei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndConditionalRender, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferRenderbuffer, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture1D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture3D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureLayer, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateMipmap, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabledi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsFramebuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsRenderbuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsVertexArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorage, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorageMultisample, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4ui, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4ui, 5);
#endif /* DUK_GL_OPENGL_3_0 */

#ifdef DUK_GL_OPENGL_3_1
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstanced, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPrimitiveRestartIndex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexBuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniformBlockBinding, 3);
#endif /* DUK_GL_OPENGL_3_1 */

#ifdef DUK_GL_OPENGL_3_2
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProvokingVertex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMaski, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage2DMultisample, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage3DMultisample, 7);
#endif /* DUK_GL_OPENGL_3_2 */

#ifdef DUK_GL_OPENGL_3_3
	duk_gl_bind_opengl_wrapper(ctx, glBindSampler, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsSampler, 1);
	duk_gl_bind_opengl_wrapper(ctx, glQueryCounter, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribDivisor, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP1ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP2ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP4ui, 4);
#endif /* DUK_GL_OPENGL_3_3 */

#ifdef DUK_GL_OPENGL_4_0
	duk_gl_bind_opengl_wrapper(ctx, glBeginQueryIndexed, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindTransformFeedback, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparatei, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparatei, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunci, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedback, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackStream, 3);
	duk_gl_bind_opengl_wrapper(ctx, glEndQueryIndexed, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsTransformFeedback, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinSampleShading, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPatchParameteri, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPauseTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glResumeTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4d, 5);
#endif /* DUK_GL_OPENGL_4_0 */

#ifdef DUK_GL_OPENGL_4_1
	duk_gl_bind_opengl_wrapper(ctx, glActiveShaderProgram, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepthf, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangeIndexed, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangef, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3ui, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4d, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4f, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4i, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4ui, 6);
	duk_gl_bind_opengl_wrapper(ctx, glReleaseShaderCompiler, 0);
	duk_gl_bind_opengl_wrapper(ctx, glScissorIndexed, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUseProgramStages, 3);
	duk_gl_bind_opengl_wrapper(ctx, glValidateProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glViewportIndexedf, 5);
#endif /* DUK_GL_OPENGL_4_1 */

#ifdef DUK_GL_OPENGL_4_2
	duk_gl_bind_opengl_wrapper(ctx, glBindImageTexture, 7);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstancedBaseInstance, 5);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackInstanced, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackStreamInstanced, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrier, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage1D, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage3D, 6);
#endif /* DUK_GL_OPENGL_4_2 */

#ifdef DUK_GL_OPENGL_4_3
	duk_gl_bind_opengl_wrapper(ctx, glCopyImageSubData, 15);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchCompute, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateBufferData, 1);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateTexImage, 2);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateTexSubImage, 8);
	duk_gl_bind_opengl_wrapper(ctx, glPopDebugGroup, 0);
	duk_gl_bind_opengl_wrapper(ctx, glShaderStorageBlockBinding, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage2DMultisample, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage3DMultisample, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureView, 8);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribBinding, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribFormat, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribIFormat, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribLFormat, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexBindingDivisor, 2);
#endif /* DUK_GL_OPENGL_4_3 */

#ifdef DUK_GL_OPENGL_4_5
	duk_gl_bind_opengl_wrapper(ctx, glBindTextureUnit, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlitNamedFramebuffer, 12);
	duk_gl_bind_opengl_wrapper(ctx, glCheckNamedFramebufferStatus, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClipControl, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage3D, 9);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateTextureMipmap, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatus, 0);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrierByRegion, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferDrawBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferReadBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferRenderbuffer, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureLayer, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorage, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageMultisample, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBarrier, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage1D, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2DMultisample, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3DMultisample, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTransformFeedbackBufferBase, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapNamedBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribBinding, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribFormat, 6);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribIFormat, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribLFormat, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayBindingDivisor, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayElementBuffer, 2);
#endif /* DUK_GL_OPENGL_4_5 */

}

/*
 *  OpenGL constants to JavaScript
 */
void duk_gl_set_constants(duk_context *ctx)
{
#ifdef DUK_GL_ARB
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_INPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CALLBACK_FUNCTION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CALLBACK_USER_PARAM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_LOGGED_MESSAGES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_HIGH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_LOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_MEDIUM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_API_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_APPLICATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_OTHER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_SHADER_COMPILER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_THIRD_PARTY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_ERROR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_OTHER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_PERFORMANCE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_PORTABILITY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GUILTY_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INNOCENT_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LOSE_CONTEXT_ON_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_LOGGED_MESSAGES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_MESSAGE_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_SAMPLE_SHADING_VALUE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NAMED_STRING_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NAMED_STRING_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_RESET_NOTIFICATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_SPARSE_LEVELS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_VIRTUAL_PAGE_SIZES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PARAMETER_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PARAMETER_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVES_SUBMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RESET_NOTIFICATION_STRATEGY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_SHADING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_INCLUDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_BUFFER_PAGE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_STORAGE_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_DECODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_CL_EVENT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_CL_EVENT_COMPLETE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SHADER_PATCHES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SPARSE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNKNOWN_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTICES_SUBMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_X_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Y_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Z_ARB);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_OPENGL_1_1
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ALWAYS);
	duk_gl_push_opengl_constant_property(ctx, GL_AND);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE);
	duk_gl_push_opengl_constant_property(ctx, GL_BYTE);
	duk_gl_push_opengl_constant_property(ctx, GL_CCW);
	duk_gl_push_opengl_constant_property(ctx, GL_CLEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CW);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_DITHER);
	duk_gl_push_opengl_constant_property(ctx, GL_DONT_CARE);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUIV);
	duk_gl_push_opengl_constant_property(ctx, GL_EXTENSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_FALSE);
	duk_gl_push_opengl_constant_property(ctx, GL_FASTEST);
	duk_gl_push_opengl_constant_property(ctx, GL_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_AND_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_GREATER);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR);
	duk_gl_push_opengl_constant_property(ctx, GL_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_ENUM);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_OPERATION);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_KEEP);
	duk_gl_push_opengl_constant_property(ctx, GL_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_LEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_LESS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_LOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_LOGIC_OP_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VIEWPORT_DIMS);
	duk_gl_push_opengl_constant_property(ctx, GL_NAND);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_NEVER);
	duk_gl_push_opengl_constant_property(ctx, GL_NICEST);
	duk_gl_push_opengl_constant_property(ctx, GL_NONE);
	duk_gl_push_opengl_constant_property(ctx, GL_NOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_NOR);
	duk_gl_push_opengl_constant_property(ctx, GL_NOTEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_ERROR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_OR);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_OUT_OF_MEMORY);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_LSB_FIRST);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ROW_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_ROWS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SWAP_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINTS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FACTOR);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADS);
	duk_gl_push_opengl_constant_property(ctx, GL_R3_G3_B2);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_RED);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERER);
	duk_gl_push_opengl_constant_property(ctx, GL_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10_A2);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB12);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB4);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5_A1);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA12);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA2);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA4);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8);
	duk_gl_push_opengl_constant_property(ctx, GL_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_BOX);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SET);
	duk_gl_push_opengl_constant_property(ctx, GL_SHORT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA_SATURATE);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_OVERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_UNDERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_PASS_DEPTH_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_PASS_DEPTH_PASS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_REF);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_VALUE_MASK);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_STEREO);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBPIXEL_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTERNAL_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAG_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_S);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_T);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLES);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_FAN);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_TRUE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_LSB_FIRST);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_ROW_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_ROWS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SWAP_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT);
	duk_gl_push_opengl_constant_property(ctx, GL_VENDOR);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_0);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_1);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT);
	duk_gl_push_opengl_constant_property(ctx, GL_XOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO);
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_push_opengl_constant_property(ctx, GL_ALIASED_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_EDGE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_3D_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_INDICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_IMAGE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_IMAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BASE_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LOD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_LOD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_R);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_IMAGE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_IMAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE_2_3_3_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE_3_3_2);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_10_10_10_2);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_2_10_10_10_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_8_8);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_8_8_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_1_5_5_5_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_4_4_4_4);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_4_4_4_4_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_5_5_1);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_6_5);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_6_5_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_2);
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CUBE_MAP_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE0);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE1);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE2);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE3);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE30);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE31);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE4);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE5);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE6);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE7);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE8);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE9);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSION_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_3);
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR_WRAP);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT16);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT24);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_REVERSE_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR_WRAP);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_LOD_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRRORED_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_FADE_THRESHOLD_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LOD_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_4);
#endif /* DUK_GL_OPENGL_1_4 */

#ifdef DUK_GL_OPENGL_1_5
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_ACCESS);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAPPED);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_USAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_QUERY);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_COUNTER_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_AVAILABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_ONLY);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_WRITE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_PASSED);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_5);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_ONLY);
#endif /* DUK_GL_OPENGL_1_5 */

#ifdef DUK_GL_OPENGL_2_0
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_ATTRIBUTES);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_UNIFORM_MAX_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTACHED_SHADERS);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC2);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC3);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC4);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPILE_STATUS);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_PROGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_VERTEX_ATTRIB);
	duk_gl_push_opengl_constant_property(ctx, GL_DELETE_STATUS);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER0);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER1);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER10);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER11);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER12);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER13);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER14);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER15);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER2);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER3);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER4);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER5);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER6);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER7);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER8);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER9);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT2);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT3);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT4);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC2);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC3);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC4);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_DERIVATIVE_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_INFO_LOG_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC2);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC3);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC4);
	duk_gl_push_opengl_constant_property(ctx, GL_LINK_STATUS);
	duk_gl_push_opengl_constant_property(ctx, GL_LOWER_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DRAW_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_FLOATS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIBS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SPRITE_COORD_ORIGIN);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_SOURCE_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADING_LANGUAGE_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_PASS_DEPTH_FAIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_PASS_DEPTH_PASS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_REF);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_VALUE_MASK);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_UPPER_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_VALIDATE_STATUS);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_2_0);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_ENABLED);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_POINT_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER);
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_2_1
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_2_1);
#endif /* DUK_GL_OPENGL_2_1 */

#ifdef DUK_GL_OPENGL_3_0
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_ACCESS_FLAGS);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_READ_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE0);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE1);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE2);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE3);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE4);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE5);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE6);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE7);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT0);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT1);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT10);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT11);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT12);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT13);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT14);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT15);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT16);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT17);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT18);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT19);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT2);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT20);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT21);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT22);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT23);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT24);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT25);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT26);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT27);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT28);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT29);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT3);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT30);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT31);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT4);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT5);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT6);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT7);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT8);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT9);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPARE_REF_TO_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RED);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RED_RGTC1);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RG);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RG_RGTC2);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_RED_RGTC1);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_RG_RGTC2);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAGS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH24_STENCIL8);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH32F_STENCIL8);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_ATTACHMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32F);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_ATTACHMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_FRAMEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_FRAMEBUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FIXED_ONLY);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_COMPLETE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_SRGB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_UNDEFINED);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_UNSUPPORTED);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLEAVED_ATTRIBS);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_FRAMEBUFFER_OPERATION);
	duk_gl_push_opengl_constant_property(ctx, GL_MAJOR_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_FLUSH_EXPLICIT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_INVALIDATE_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_INVALIDATE_RANGE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_READ_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_UNSYNCHRONIZED_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_WRITE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ARRAY_TEXTURE_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIP_DISTANCES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_ATTACHMENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXEL_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RENDERBUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MINOR_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXEL_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_EXTENSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVES_GENERATED);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_NO_WAIT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_WAIT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_NO_WAIT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_WAIT);
	duk_gl_push_opengl_constant_property(ctx, GL_R11F_G11F_B10F);
	duk_gl_push_opengl_constant_property(ctx, GL_R16);
	duk_gl_push_opengl_constant_property(ctx, GL_R16F);
	duk_gl_push_opengl_constant_property(ctx, GL_R16I);
	duk_gl_push_opengl_constant_property(ctx, GL_R16UI);
	duk_gl_push_opengl_constant_property(ctx, GL_R32F);
	duk_gl_push_opengl_constant_property(ctx, GL_R32I);
	duk_gl_push_opengl_constant_property(ctx, GL_R32UI);
	duk_gl_push_opengl_constant_property(ctx, GL_R8);
	duk_gl_push_opengl_constant_property(ctx, GL_R8I);
	duk_gl_push_opengl_constant_property(ctx, GL_R8UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTERIZER_DISCARD);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_FRAMEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_FRAMEBUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_DEPTH_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_INTERNAL_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_STENCIL_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_RG);
	duk_gl_push_opengl_constant_property(ctx, GL_RG16);
	duk_gl_push_opengl_constant_property(ctx, GL_RG16F);
	duk_gl_push_opengl_constant_property(ctx, GL_RG16I);
	duk_gl_push_opengl_constant_property(ctx, GL_RG16UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RG32F);
	duk_gl_push_opengl_constant_property(ctx, GL_RG32I);
	duk_gl_push_opengl_constant_property(ctx, GL_RG32UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RG8);
	duk_gl_push_opengl_constant_property(ctx, GL_RG8I);
	duk_gl_push_opengl_constant_property(ctx, GL_RG8UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16F);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32F);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB9_E5);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16F);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32F);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8I);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8UI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_RG_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_ARRAY_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_ARRAY_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_ATTRIBS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_ATTACHMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX1);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX16);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX4);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX8);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SHARED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STENCIL_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_START);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYINGS);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_10F_11F_11F_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_24_8);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_5_9_9_9_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC2);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC3);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC4);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_NORMALIZED);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_3_0);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_INTEGER);
#endif /* DUK_GL_OPENGL_3_0 */

#ifdef DUK_GL_OPENGL_3_1
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_READ_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_WRITE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RECTANGLE_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_UNIFORM_BLOCK_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_UNIFORM_BUFFER_BINDINGS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_RECTANGLE);
	duk_gl_push_opengl_constant_property(ctx, GL_R16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_R8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RG16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RG8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_RECT_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_NORMALIZED);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_RECTANGLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_DATA_STORE_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RECTANGLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_DATA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_NAME_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_START);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_IS_ROW_MAJOR);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_MATRIX_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_NAME_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_3_1);
#endif /* DUK_GL_OPENGL_3_1 */

#ifdef DUK_GL_OPENGL_3_2
	duk_gl_push_opengl_constant_property(ctx, GL_ALREADY_SIGNALED);
	duk_gl_push_opengl_constant_property(ctx, GL_CONDITION_SATISFIED);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_COMPATIBILITY_PROFILE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_CORE_PROFILE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_PROFILE_MASK);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLAMP);
	duk_gl_push_opengl_constant_property(ctx, GL_FIRST_VERTEX_CONVENTION);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_LAYERED);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_INPUT_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_OUTPUT_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_VERTICES_OUT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_LAST_VERTEX_CONVENTION);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES_ADJACENCY);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP_ADJACENCY);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_TEXTURE_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEPTH_TEXTURE_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_INPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_INPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_OUTPUT_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_INTEGER_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SAMPLE_MASK_WORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SERVER_WAIT_TIMEOUT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_POINT_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROVOKING_VERTEX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_POSITION);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNALED);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_CONDITION);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_FENCE);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_FLAGS);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_FLUSH_COMMANDS_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_GPU_COMMANDS_COMPLETE);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_STATUS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_SEAMLESS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FIXED_SAMPLE_LOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_TIMEOUT_EXPIRED);
	duk_gl_push_opengl_constant_property(ctx, GL_TIMEOUT_IGNORED);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLES_ADJACENCY);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_STRIP_ADJACENCY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNALED);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_3_2);
	duk_gl_push_opengl_constant_property(ctx, GL_WAIT_FAILED);
#endif /* DUK_GL_OPENGL_3_2 */

#ifdef DUK_GL_OPENGL_3_3
	duk_gl_push_opengl_constant_property(ctx, GL_ANY_SAMPLES_PASSED);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_2_10_10_10_REV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DUAL_SOURCE_DRAW_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC1_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10_A2UI);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC1_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_A);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_B);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_G);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_R);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_TIMESTAMP);
	duk_gl_push_opengl_constant_property(ctx, GL_TIME_ELAPSED);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_3_3);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_DIVISOR);
#endif /* DUK_GL_OPENGL_3_3 */

#ifdef DUK_GL_OPENGL_4_0
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_SUBROUTINES);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_SUBROUTINE_MAX_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_SUBROUTINE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPATIBLE_SUBROUTINES);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT2);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT3);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT4);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC2);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC3);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC4);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_INDIRECT_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_INDIRECT_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FRACTIONAL_EVEN);
	duk_gl_push_opengl_constant_property(ctx, GL_FRACTIONAL_ODD);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_INTERPOLATION_OFFSET_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_INVOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_ISOLINES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_INTERPOLATION_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_SHADER_INVOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PATCH_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SUBROUTINES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_INPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_GEN_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_PATCH_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_STREAMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_FRAGMENT_INTERPOLATION_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_SAMPLE_SHADING_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_COMPATIBLE_SUBROUTINES);
	duk_gl_push_opengl_constant_property(ctx, GL_PATCHES);
	duk_gl_push_opengl_constant_property(ctx, GL_PATCH_DEFAULT_INNER_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PATCH_DEFAULT_OUTER_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PATCH_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_SHADING);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_OUTPUT_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_GEN_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_GEN_POINT_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_GEN_SPACING);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_GEN_VERTEX_ORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_0);
#endif /* DUK_GL_OPENGL_4_0 */

#ifdef DUK_GL_OPENGL_4_1
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_PROGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_SHADER_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_FIXED);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_HIGH_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_HIGH_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMPLEMENTATION_COLOR_READ_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMPLEMENTATION_COLOR_READ_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_LAYER_PROVOKING_VERTEX);
	duk_gl_push_opengl_constant_property(ctx, GL_LOW_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOW_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_UNIFORM_VECTORS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_VECTORS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNIFORM_VECTORS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VIEWPORTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MEDIUM_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_MEDIUM_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_PROGRAM_BINARY_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_SHADER_BINARY_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_BINARY_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_BINARY_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_BINARY_RETRIEVABLE_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_PIPELINE_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_SEPARABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB565);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_BINARY_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_COMPILER);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNDEFINED_VERTEX);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_1);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_BOUNDS_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_INDEX_PROVOKING_VERTEX);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_SUBPIXEL_BITS);
#endif /* DUK_GL_OPENGL_4_1 */

#ifdef DUK_GL_OPENGL_4_2
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_BARRIER_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_START);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_UPDATE_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMMAND_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_BPTC_UNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_READ_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_WRITE_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_ACCESS);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LAYER);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LAYERED);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_NAME);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_FORMAT_COMPATIBILITY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_IMAGE_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_MAP_BUFFER_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_SAMPLE_COUNTS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_COMPRESSED_BLOCK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_COMPRESSED_BLOCK_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_COMPRESSED_BLOCK_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_COMPRESSED_BLOCK_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_BUFFER_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FETCH_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_IMMUTABLE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_UPDATE_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_ACTIVE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_PAUSED);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_COMPRESSED_BLOCK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_COMPRESSED_BLOCK_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_COMPRESSED_BLOCK_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_COMPRESSED_BLOCK_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_ATOMIC_COUNTER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_1D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_RECT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_CUBE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_2);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
#endif /* DUK_GL_OPENGL_4_2 */

#ifdef DUK_GL_OPENGL_4_3
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_RESOURCES);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_VARIABLES);
	duk_gl_push_opengl_constant_property(ctx, GL_ANY_SAMPLES_PASSED_CONSERVATIVE);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_AUTO_GENERATE_MIPMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_BLOCK_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_DATA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_VARIABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_CAVEAT_SUPPORT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLEAR_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ENCODING);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_RENDERABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_R11_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RG11_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB8_ETC2);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA8_ETC2_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_R11_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_RG11_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB8_ETC2);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SHADER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_WORK_GROUP_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_DEBUG_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CALLBACK_FUNCTION);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CALLBACK_USER_PARAM);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_GROUP_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_LOGGED_MESSAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_OUTPUT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_OUTPUT_SYNCHRONOUS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_HIGH);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_LOW);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_MEDIUM);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_NOTIFICATION);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_API);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_APPLICATION);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_OTHER);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_SHADER_COMPILER);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_THIRD_PARTY);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SOURCE_WINDOW_SYSTEM);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_ERROR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_MARKER);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_OTHER);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_PERFORMANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_POP_GROUP);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_PORTABILITY);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_PUSH_GROUP);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_RENDERABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_TEXTURE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_DISPATCH_INDIRECT_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DISPATCH_INDIRECT_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_BLEND);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_DEFAULT_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_RENDERABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_RENDERABLE_LAYERED);
	duk_gl_push_opengl_constant_property(ctx, GL_FULL_SUPPORT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_GET_TEXTURE_IMAGE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_GET_TEXTURE_IMAGE_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_10_10_10_2);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_11_11_10);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_1_X_16);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_1_X_32);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_1_X_8);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_2_X_16);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_2_X_32);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_2_X_8);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_4_X_16);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_4_X_32);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CLASS_4_X_8);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_COMPATIBILITY_CLASS);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_PIXEL_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_PIXEL_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_TEXEL_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_ALPHA_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_BLUE_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_DEPTH_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_DEPTH_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_GREEN_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_PREFERRED);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_RED_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_SHARED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_STENCIL_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_STENCIL_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERNALFORMAT_SUPPORTED);
	duk_gl_push_opengl_constant_property(ctx, GL_IS_PER_PATCH);
	duk_gl_push_opengl_constant_property(ctx, GL_IS_ROW_MAJOR);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCATION);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCATION_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MANUAL_GENERATE_MIPMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_DIMENSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_ATOMIC_COUNTERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_IMAGE_UNIFORMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_SHARED_MEMORY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_UNIFORM_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_WORK_GROUP_COUNT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_WORK_GROUP_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_GROUP_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_LOGGED_MESSAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_MESSAGE_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENT_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAMEBUFFER_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAMEBUFFER_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAMEBUFFER_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAMEBUFFER_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LABEL_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_NAME_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_NUM_ACTIVE_VARIABLES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_NUM_COMPATIBLE_SUBROUTINES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SHADER_STORAGE_BLOCK_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_UNIFORM_LOCATIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIB_BINDINGS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MIPMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_NAME_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_ACTIVE_VARIABLES);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_SHADING_LANGUAGE_VERSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART_FIXED_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_INPUT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_OUTPUT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_PIPELINE);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXELS_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXELS_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_COMPUTE_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_FRAGMENT_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_GEOMETRY_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_TESS_CONTROL_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_TESS_EVALUATION_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCED_BY_VERTEX_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_IMAGE_ATOMIC);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_IMAGE_LOAD);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_IMAGE_STORE);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BLOCK);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_STORAGE_BUFFER_START);
	duk_gl_push_opengl_constant_property(ctx, GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE);
	duk_gl_push_opengl_constant_property(ctx, GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_WRITE);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_RENDERABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_BLOCK_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_BLOCK_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GATHER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GATHER_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_IMAGE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_IMAGE_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_IMMUTABLE_LEVELS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SHADOW);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_VIEW);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_VIEW_MIN_LAYER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_VIEW_MIN_LEVEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_VIEW_NUM_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_VIEW_NUM_LEVELS);
	duk_gl_push_opengl_constant_property(ctx, GL_TOP_LEVEL_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TOP_LEVEL_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYING);
	duk_gl_push_opengl_constant_property(ctx, GL_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_3);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_LONG);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_RELATIVE_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_BINDING_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_BINDING_DIVISOR);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_BINDING_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_BINDING_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SUBROUTINE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SUBROUTINE_UNIFORM);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_128_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_16_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_24_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_32_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_48_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_64_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_8_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_96_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_BPTC_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_BPTC_UNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_RGTC1_RED);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_RGTC2_RG);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_S3TC_DXT1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_S3TC_DXT1_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_S3TC_DXT3_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_CLASS_S3TC_DXT5_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEW_COMPATIBILITY_CLASS);
#endif /* DUK_GL_OPENGL_4_3 */

#ifdef DUK_GL_OPENGL_4_4
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_IMMUTABLE_STORAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_STORAGE_FLAGS);
	duk_gl_push_opengl_constant_property(ctx, GL_CLEAR_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_STORAGE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_STORAGE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCATION_COMPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_COHERENT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_PERSISTENT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIB_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_TO_EDGE);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BUFFER_BARRIER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_NO_WAIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_4);
#endif /* DUK_GL_OPENGL_4_4 */

#ifdef DUK_GL_OPENGL_4_5
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DEPTH_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_ORIGIN);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_LOST);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_ROBUST_ACCESS);
	duk_gl_push_opengl_constant_property(ctx, GL_GUILTY_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_INNOCENT_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_LOSE_CONTEXT_ON_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CULL_DISTANCES);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_ONE_TO_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_RESET_NOTIFICATION);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_NO_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_NO_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_TARGET);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_RESET_NOTIFICATION_STRATEGY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_TARGET);
	duk_gl_push_opengl_constant_property(ctx, GL_UNKNOWN_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_5);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO_TO_ONE);
#endif /* DUK_GL_OPENGL_4_5 */

}

/*
 *  Push OpenGL bindings to JavaScript global object
 */
void duk_gl_push_opengl_bindings(duk_context *ctx)
{
	duk_push_global_object(ctx);
	duk_gl_bind_opengl_functions(ctx);
	duk_gl_set_constants(ctx);
	duk_pop(ctx);
}

