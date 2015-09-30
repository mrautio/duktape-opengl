/*
 *  Duktape OpenGL wrapper 0.1.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 */

#include <duktape.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define DUK_GL_OPENGL_1_1

/*
 *  Bindings that are not enabled by default.
 */
/*#define DUK_GL_ARB*/
/*#define DUK_GL_ATI*/
/*#define DUK_GL_EXT*/
/*#define DUK_GL_HP*/
/*#define DUK_GL_IBM*/
/*#define DUK_GL_INTEL*/
/*#define DUK_GL_MESA*/
/*#define DUK_GL_NV*/

/*
 *  Enable automatically older OpenGL standard manjor versions, if higher major version is enabled
 */
#ifdef DUK_GL_OPENGL_4X
#define DUK_GL_OPENGL_3X
#endif /*DUK_GL_OPENGL_4X*/

#ifdef DUK_GL_OPENGL_3X
#define DUK_GL_OPENGL_2X
#endif /*DUK_GL_OPENGL_3X*/

#ifdef DUK_GL_OPENGL_2X
#define DUK_GL_OPENGL_1X
#endif /*DUK_GL_OPENGL_2X*/

#ifdef  DUK_GL_OPENGL_1X
#define DUK_GL_OPENGL_1_1
#define DUK_GL_OPENGL_1_2
#define DUK_GL_OPENGL_1_3
#define DUK_GL_OPENGL_1_4
#define DUK_GL_OPENGL_1_5
#endif /*DUK_GL_OPENGL_1X*/

#ifdef  DUK_GL_OPENGL_2X
#define DUK_GL_OPENGL_2_0
#define DUK_GL_OPENGL_2_1
#endif /*DUK_GL_OPENGL_2X*/

#ifdef  DUK_GL_OPENGL_3X
#define DUK_GL_OPENGL_3_0
#define DUK_GL_OPENGL_3_1
#define DUK_GL_OPENGL_3_2
#define DUK_GL_OPENGL_3_3
#endif /*DUK_GL_OPENGL_3X*/

#ifdef  DUK_GL_OPENGL_4X
#define DUK_GL_OPENGL_4_0
#define DUK_GL_OPENGL_4_1
#define DUK_GL_OPENGL_4_2
#define DUK_GL_OPENGL_4_3
#define DUK_GL_OPENGL_4_4
#define DUK_GL_OPENGL_4_5
#endif /*DUK_GL_OPENGL_4X*/

/*#define DUK_GL_SGI*/
/*#define DUK_GL_SGIX*/
/*#define DUK_GL_SUN*/
/*#define DUK_GL_WIN*/

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
 *  Macro for handling of arrays
 */
static size_t duk_gl_determine_array_length(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, size_t num)
{
	size_t array_length = sz;
	if (sz < 1)
	{
		/* use <arrayVariable>.length is array size not explicitly defined */
		duk_get_prop_string(ctx, obj_index, "length");
		array_length = (unsigned int)duk_get_uint(ctx, -1);
		duk_pop(ctx);
	}
	/* prevent buffer overflow by clamping the value */
	if (array_length > num)
	{
		array_length = num;
	}
	return array_length;
}

#define DUK_GL_ARRAY_PROCESSING_FUNCTION(argtypedef1, arg1) \
static size_t duk_gl_get_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \
{ \
	if (duk_is_array(ctx, obj_index)) \
	{ \
		duk_get_prop(ctx, obj_index); \
		size_t array_length = duk_gl_determine_array_length(ctx, obj_index, sz, num); \
		unsigned int i = 0; \
		for(i=0; i<array_length; i++) \
		{ \
			duk_get_prop_index(ctx, obj_index, (duk_uarridx_t)i); \
			array[i] = (argtypedef1)duk_get_##arg1(ctx, -1); \
			duk_pop(ctx); \
		} \
		duk_pop(ctx); \
		return array_length; \
	} \
	return 0; \
} \
static size_t duk_gl_put_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \
{ \
	if (duk_is_array(ctx, obj_index)) \
	{ \
		duk_get_prop(ctx, obj_index); \
		size_t array_length = duk_gl_determine_array_length(ctx, obj_index, sz, num); \
		unsigned int i = 0; \
		for(i=0; i<array_length; i++) \
		{ \
			duk_push_##arg1(ctx, (argtypedef1)array[i]); \
			duk_put_prop_index(ctx, obj_index, (duk_uarridx_t)i); \
		} \
		duk_pop(ctx); \
		return array_length; \
	} \
	return 0; \
}

DUK_GL_ARRAY_PROCESSING_FUNCTION(GLbyte, number)
DUK_GL_ARRAY_PROCESSING_FUNCTION(GLdouble, number)
DUK_GL_ARRAY_PROCESSING_FUNCTION(GLfloat, number)
DUK_GL_ARRAY_PROCESSING_FUNCTION(GLint, number)
DUK_GL_ARRAY_PROCESSING_FUNCTION(GLshort, number)

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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveTextureARB, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBeginQueryARB, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindBufferARB, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindProgramARB, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendEquationSeparateiARB, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationiARB, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glBlendFuncSeparateiARB, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendFunciARB, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glClampColorARB, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClientActiveTextureARB, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCurrentPaletteMatrixARB, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glDebugMessageInsertARB, GLenum, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeleteNamedStringARB, GLint, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableVertexAttribArrayARB, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glDispatchComputeGroupSizeARB, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawArraysInstancedARB, GLenum, uint, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableVertexAttribArrayARB, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEndQueryARB, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEvaluateDepthValuesARB)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFramebufferTextureARB, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTextureFaceARB, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTextureLayerARB, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetGraphicsResetStatusARB, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(glGetImageHandleARB, uint, GLuint, uint, GLint, int, GLboolean, boolean, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGetTextureHandleARB, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetTextureSamplerHandleARB, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsBufferARB, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsImageHandleResidentARB, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsNamedStringARB, boolean, GLint, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsProgramARB, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsQueryARB, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTextureHandleResidentARB, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeImageHandleNonResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMakeImageHandleResidentARB, GLuint64, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleNonResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleResidentARB, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMaxShaderCompilerThreadsARB, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMinSampleShadingARB, GLfloat, number)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameterfARB, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glPrimitiveBoundingBoxARB, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramEnvParameter4dARB, GLenum, uint, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramEnvParameter4fARB, GLenum, uint, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramLocalParameter4dARB, GLenum, uint, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramLocalParameter4fARB, GLenum, uint, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramParameteriARB, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1i64ARB, GLuint, uint, GLint, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1ui64ARB, GLuint, uint, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2i64ARB, GLuint, uint, GLint, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2ui64ARB, GLuint, uint, GLint, int, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3i64ARB, GLuint, uint, GLint, int, GLint64, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3ui64ARB, GLuint, uint, GLint, int, GLuint64, uint, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4i64ARB, GLuint, uint, GLint, int, GLint64, int, GLint64, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4ui64ARB, GLuint, uint, GLint, int, GLuint64, uint, GLuint64, uint, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniformHandleui64ARB, GLuint, uint, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleCoverageARB, GLfloat, number, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexBufferARB, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glTexPageCommitmentARB, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1fARB, GLint, int, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1i64ARB, GLint, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1iARB, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1ui64ARB, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2fARB, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2i64ARB, GLint, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2iARB, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2ui64ARB, GLint, int, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3fARB, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3i64ARB, GLint, int, GLint64, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3iARB, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3ui64ARB, GLint, int, GLuint64, uint, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4fARB, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4i64ARB, GLint, int, GLint64, int, GLint64, int, GLint64, int, GLint64, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4iARB, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4ui64ARB, GLint, int, GLuint64, uint, GLuint64, uint, GLuint64, uint, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniformHandleui64ARB, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glUnmapBufferARB, boolean, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1dARB, GLuint, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1fARB, GLuint, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1sARB, GLuint, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2dARB, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2fARB, GLuint, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2sARB, GLuint, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3dARB, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3fARB, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3sARB, GLuint, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4NubARB, GLuint, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4dARB, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4fARB, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4sARB, GLuint, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribDivisorARB, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1ui64ARB, GLuint, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glVertexBlendARB, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2dARB, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2fARB, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2iARB, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2sARB, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3dARB, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3fARB, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3iARB, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3sARB, GLshort, int, GLshort, int, GLshort, int)
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glAlphaFragmentOp1ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glAlphaFragmentOp2ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(glAlphaFragmentOp3ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glArrayObjectATI, GLenum, uint, GLint, int, GLenum, uint, GLsizei, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glBeginFragmentShaderATI)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBindFragmentShaderATI, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationSeparateATI, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClientActiveVertexStreamATI, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glColorFragmentOp1ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glColorFragmentOp2ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG13(glColorFragmentOp3ATI, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteFragmentShaderATI, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDrawElementArrayATI, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawRangeElementArrayATI, GLenum, uint, GLuint, uint, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndFragmentShaderATI)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFreeObjectBufferATI, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenFragmentShadersATI, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsObjectBufferATI, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNormalStream3bATI, GLenum, uint, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNormalStream3dATI, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNormalStream3fATI, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNormalStream3iATI, GLenum, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNormalStream3sATI, GLenum, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPNTrianglesfATI, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPNTrianglesiATI, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPassTexCoordATI, GLuint, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSampleMapATI, GLuint, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilFuncSeparateATI, GLenum, uint, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilOpSeparateATI, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glUnmapObjectBufferATI, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVariantArrayObjectATI, GLuint, uint, GLenum, uint, GLsizei, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glVertexAttribArrayObjectATI, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLsizei, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexBlendEnvfATI, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexBlendEnviATI, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexStream1dATI, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexStream1fATI, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexStream1iATI, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexStream1sATI, GLenum, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexStream2dATI, GLenum, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexStream2fATI, GLenum, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexStream2iATI, GLenum, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexStream2sATI, GLenum, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexStream3dATI, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexStream3fATI, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexStream3iATI, GLenum, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexStream3sATI, GLenum, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexStream4dATI, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexStream4fATI, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexStream4iATI, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexStream4sATI, GLenum, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_EXT
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveProgramEXT, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glActiveStencilFaceEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glApplyTextureEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glArrayElementEXT, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginTransformFeedbackEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glBeginVertexShaderEXT)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindBufferBaseEXT, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindFragDataLocationEXT, GLuint, uint, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindFramebufferEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glBindImageTextureEXT, GLuint, uint, GLuint, uint, GLint, int, GLboolean, boolean, GLint, int, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glBindLightParameterEXT, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glBindMaterialParameterEXT, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindMultiTextureEXT, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glBindParameterEXT, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindRenderbufferEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glBindTexGenParameterEXT, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTextureEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glBindTextureUnitParameterEXT, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBindVertexShaderEXT, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBinormal3bEXT, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBinormal3dEXT, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBinormal3fEXT, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBinormal3iEXT, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBinormal3sEXT, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColorEXT, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBlendEquationEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationSeparateEXT, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendFuncSeparateEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glBlitFramebufferEXT, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLbitfield, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glCheckFramebufferStatusEXT, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glCheckNamedFramebufferStatusEXT, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColorIiEXT, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColorIuiEXT, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClientAttribDefaultEXT, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glColorMaskIndexedEXT, GLuint, uint, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameterfEXT, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameteriEXT, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorSubTableEXT, GLenum, uint, GLsizei, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyConvolutionFilter1DEXT, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyConvolutionFilter2DEXT, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyMultiTexImage1DEXT, GLenum, uint, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyMultiTexImage2DEXT, GLenum, uint, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyMultiTexSubImage1DEXT, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyMultiTexSubImage2DEXT, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glCopyMultiTexSubImage3DEXT, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyTexImage1DEXT, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexImage2DEXT, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTexSubImage1DEXT, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexSubImage2DEXT, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTexSubImage3DEXT, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTextureImage1DEXT, GLuint, uint, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTextureImage2DEXT, GLuint, uint, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyTextureSubImage1DEXT, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTextureSubImage2DEXT, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glCopyTextureSubImage3DEXT, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glCreateShaderProgramEXT, uint, GLenum, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteVertexShaderEXT, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthBoundsEXT, GLclampd, number, GLclampd, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableClientStateIndexedEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableClientStateiEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableIndexedEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableVariantClientStateEXT, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableVertexArrayAttribEXT, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableVertexArrayEXT, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawArraysEXT, GLenum, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawArraysInstancedEXT, GLenum, uint, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableClientStateIndexedEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableClientStateiEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableIndexedEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableVariantClientStateEXT, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableVertexArrayAttribEXT, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableVertexArrayEXT, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndTransformFeedbackEXT)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndVertexShaderEXT)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glExtractComponentEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFogCoorddEXT, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFogCoordfEXT, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFramebufferDrawBufferEXT, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFramebufferReadBufferEXT, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFramebufferRenderbufferEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTexture1DEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTexture2DEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFramebufferTexture3DEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFramebufferTextureEXT, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTextureFaceEXT, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glFramebufferTextureLayerEXT, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG4(glGenSymbolsEXT, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenVertexShadersEXT, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGenerateMipmapEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glGenerateMultiTexMipmapEXT, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glGenerateTextureMipmapEXT, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetFragDataLocationEXT, int, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetUniformBufferSizeEXT, int, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glHistogramEXT, GLenum, uint, GLsizei, int, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glIndexFuncEXT, GLenum, uint, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glIndexMaterialEXT, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glInsertComponentEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glInsertEventMarkerEXT, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsEnabledIndexedEXT, boolean, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsFramebufferEXT, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsRenderbufferEXT, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTextureEXT, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsVariantEnabledEXT, boolean, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glLabelObjectEXT, GLenum, uint, GLuint, uint, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLockArraysEXT, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glMatrixFrustumEXT, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMatrixLoadIdentityEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glMatrixOrthoEXT, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMatrixPopEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMatrixPushEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMatrixRotatedEXT, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMatrixRotatefEXT, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMatrixScaledEXT, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMatrixScalefEXT, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMatrixTranslatedEXT, GLenum, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMatrixTranslatefEXT, GLenum, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMemoryBarrierEXT, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMinmaxEXT, GLenum, uint, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexBufferEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexEnvfEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexEnviEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexGendEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexGenfEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexGeniEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexParameterfEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexParameteriEXT, GLenum, uint, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexRenderbufferEXT, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNamedFramebufferParameteriEXT, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferRenderbufferEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTexture1DEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTexture2DEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glNamedFramebufferTexture3DEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferTextureEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTextureFaceEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTextureLayerEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glNamedProgramLocalParameter4dEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glNamedProgramLocalParameter4fEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glNamedProgramLocalParameterI4iEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glNamedProgramLocalParameterI4uiEXT, GLuint, uint, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedRenderbufferStorageEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glNamedRenderbufferStorageMultisampleCoverageEXT, GLuint, uint, GLsizei, int, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedRenderbufferStorageMultisampleEXT, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPixelTransformParameterfEXT, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPixelTransformParameteriEXT, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameterfEXT, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPolygonOffsetClampEXT, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonOffsetEXT, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopGroupMarkerEXT)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramParameteriEXT, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1dEXT, GLuint, uint, GLint, int, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1fEXT, GLuint, uint, GLint, int, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1iEXT, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1uiEXT, GLuint, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2dEXT, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2fEXT, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2iEXT, GLuint, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2uiEXT, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3dEXT, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3fEXT, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3iEXT, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3uiEXT, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4dEXT, GLuint, uint, GLint, int, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4fEXT, GLuint, uint, GLint, int, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4iEXT, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4uiEXT, GLuint, uint, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glProvokingVertexEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushClientAttribDefaultEXT, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPushGroupMarkerEXT, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterSamplesEXT, GLuint, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRenderbufferStorageEXT, GLenum, uint, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glRenderbufferStorageMultisampleEXT, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetHistogramEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetMinmaxEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleMaskEXT, GLclampf, number, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glSamplePatternEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3bEXT, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3dEXT, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3fEXT, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3iEXT, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3sEXT, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3ubEXT, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3uiEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3usEXT, GLushort, uint, GLushort, uint, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glShaderOp1EXT, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glShaderOp2EXT, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glShaderOp3EXT, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glStencilClearTagEXT, GLsizei, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glSwizzleEXT, GLuint, uint, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTangent3bEXT, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTangent3dEXT, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTangent3fEXT, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTangent3iEXT, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTangent3sEXT, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexBufferEXT, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureBufferEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTextureLightEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTextureMaterialEXT, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTextureNormalEXT, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glTexturePageCommitmentEXT, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureParameterfEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureParameteriEXT, GLuint, uint, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureRenderbufferEXT, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glTextureStorage1DEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTextureStorage2DEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTextureStorage2DMultisampleEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTextureStorage3DEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTextureStorage3DMultisampleEXT, GLuint, uint, GLenum, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1uiEXT, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2uiEXT, GLint, int, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3uiEXT, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4uiEXT, GLint, int, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniformBufferEXT, GLuint, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glUnlockArraysEXT)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glUnmapNamedBufferEXT, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUseShaderProgramEXT, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayVertexAttribBindingEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayVertexAttribDivisorEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glVertexArrayVertexAttribFormatEXT, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayVertexAttribIFormatEXT, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayVertexAttribLFormatEXT, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayVertexBindingDivisorEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribI1iEXT, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribI1uiEXT, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribI2iEXT, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribI2uiEXT, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribI3iEXT, GLuint, uint, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribI3uiEXT, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribI4iEXT, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribI4uiEXT, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1dEXT, GLuint, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribL2dEXT, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribL3dEXT, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribL4dEXT, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glVertexWeightfEXT, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glWriteMaskEXT, GLuint, uint, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
#endif /* DUK_GL_EXT */

#ifdef DUK_GL_HP
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glImageTransformParameterfHP, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glImageTransformParameteriHP, GLenum, uint, GLenum, uint, GLint, int)
#endif /* DUK_GL_HP */

#ifdef DUK_GL_IBM
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFlushStaticDataIBM, GLenum, uint)
#endif /* DUK_GL_IBM */

#ifdef DUK_GL_INTEL
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glApplyFramebufferAttachmentCMAAINTEL)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginPerfQueryINTEL, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeletePerfQueryINTEL, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEndPerfQueryINTEL, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glSyncTextureINTEL, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUnmapTexture2DINTEL, GLuint, uint, GLint, int)
#endif /* DUK_GL_INTEL */

#ifdef DUK_GL_MESA
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableTraceMESA, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableTraceMESA, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndTraceMESA)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glResizeBuffersMESA)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTraceAssertAttribMESA, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2dMESA, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2fMESA, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2iMESA, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2sMESA, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3dMESA, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3fMESA, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3iMESA, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3sMESA, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glWindowPos4dMESA, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glWindowPos4fMESA, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glWindowPos4iMESA, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glWindowPos4sMESA, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_NV
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glActiveVaryingNV, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBeginConditionalRenderNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginOcclusionQueryNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginTransformFeedbackNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginVideoCaptureNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindBufferBaseNV, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindProgramNV, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTransformFeedbackNV, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glBindVideoCaptureStreamTextureNV, GLuint, uint, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glBlendBarrierNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendParameteriNV, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCallCommandListNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepthdNV, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColorFormatNV, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCombinerInputNV, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glCombinerOutputNV, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCombinerParameterfNV, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCombinerParameteriNV, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCommandListSegmentsNV, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCompileCommandListNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glConservativeRasterParameterfNV, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(glCopyImageSubDataNV, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLuint, uint, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCopyPathNV, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCoverFillPathNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glCoverStrokePathNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCoverageModulationNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeletePathsNV, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthBoundsdNV, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRangedNV, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG11(glDrawTextureNV, GLuint, uint, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDrawTransformFeedbackNV, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEdgeFlagFormatNV, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndConditionalRenderNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndOcclusionQueryNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndTransformFeedbackNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEndVideoCaptureNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalMapsNV, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glFinalCombinerInputNV, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFinishFenceNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFlushPixelDataRangeNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFlushVertexArrayRangeNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogCoordFormatNV, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFogCoordhNV, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFragmentCoverageColorNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenPathsNV, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetCommandHeaderNV, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(glGetImageHandleNV, uint, GLuint, uint, GLint, int, GLboolean, boolean, GLint, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetPathLengthNV, number, GLuint, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGetStageIndexNV, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGetTextureHandleNV, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetTextureSamplerHandleNV, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetVaryingLocationNV, int, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glIndexFormatNV, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glInterpolatePathsNV, GLuint, uint, GLuint, uint, GLuint, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsBufferResidentNV, boolean, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsCommandListNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsFenceNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsImageHandleResidentNV, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsNamedBufferResidentNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsOcclusionQueryNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsPathNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG4(glIsPointInFillPathNV, boolean, GLuint, uint, GLuint, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glIsPointInStrokePathNV, boolean, GLuint, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsProgramNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsStateNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTextureHandleResidentNV, boolean, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTransformFeedbackNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeBufferNonResidentNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMakeBufferResidentNV, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeImageHandleNonResidentNV, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMakeImageHandleResidentNV, GLuint64, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeNamedBufferNonResidentNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMakeNamedBufferResidentNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleNonResidentNV, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMakeTextureHandleResidentNV, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1hNV, GLenum, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2hNV, GLenum, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3hNV, GLenum, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4hNV, GLenum, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNormalFormatNV, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPathCoverDepthFuncNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPathFogGenNV, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPathParameterfNV, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPathParameteriNV, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPathStencilDepthOffsetNV, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glPathStencilFuncNV, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPauseTransformFeedbackNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameteriNV, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG13(glPresentFrameDualFillNV, GLuint, uint, GLuint64EXT, uint, GLuint, uint, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG11(glPresentFrameKeyedNV, GLuint, uint, GLuint64EXT, uint, GLuint, uint, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint, GLuint, uint, GLenum, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPrimitiveRestartIndexNV, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPrimitiveRestartNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramEnvParameterI4iNV, GLenum, uint, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramEnvParameterI4uiNV, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramLocalParameterI4iNV, GLenum, uint, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramLocalParameterI4uiNV, GLenum, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramParameter4dNV, GLenum, uint, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramParameter4fNV, GLenum, uint, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1i64NV, GLuint, uint, GLint, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniform1ui64NV, GLuint, uint, GLint, int, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2i64NV, GLuint, uint, GLint, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glProgramUniform2ui64NV, GLuint, uint, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3i64NV, GLuint, uint, GLint, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glProgramUniform3ui64NV, GLuint, uint, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4i64NV, GLuint, uint, GLint, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glProgramUniform4ui64NV, GLuint, uint, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniformHandleui64NV, GLuint, uint, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glProgramUniformui64NV, GLuint, uint, GLint, int, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glProgramVertexLimitNV, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glRenderbufferStorageMultisampleCoverageNV, GLenum, uint, GLsizei, int, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glResolveDepthValuesNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glResumeTransformFeedbackNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleMaskIndexedNV, GLuint, uint, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColorFormatNV, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSetFenceNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glStateCaptureNV, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilFillPathNV, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilStrokePathNV, GLuint, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilThenCoverFillPathNV, GLuint, uint, GLenum, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glStencilThenCoverStrokePathNV, GLuint, uint, GLint, int, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSubpixelPrecisionBiasNV, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glTestFenceNV, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1hNV, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2hNV, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoordFormatNV, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTexImage2DMultisampleCoverageNV, GLenum, uint, GLsizei, int, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTexImage3DMultisampleCoverageNV, GLenum, uint, GLsizei, int, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexRenderbufferNV, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glTextureBarrierNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTextureImage2DMultisampleCoverageNV, GLuint, uint, GLenum, uint, GLsizei, int, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTextureImage2DMultisampleNV, GLuint, uint, GLenum, uint, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glTextureImage3DMultisampleCoverageNV, GLuint, uint, GLenum, uint, GLsizei, int, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTextureImage3DMultisampleNV, GLuint, uint, GLenum, uint, GLsizei, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTrackMatrixNV, GLenum, uint, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1i64NV, GLint, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniform1ui64NV, GLint, int, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2i64NV, GLint, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glUniform2ui64NV, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3i64NV, GLint, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glUniform3ui64NV, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4i64NV, GLint, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glUniform4ui64NV, GLint, int, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniformHandleui64NV, GLint, int, GLuint64, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glUniformui64NV, GLint, int, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glVDPAUFiniNV)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2hNV, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4hNV, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1dNV, GLuint, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1fNV, GLuint, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1hNV, GLuint, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttrib1sNV, GLuint, uint, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2dNV, GLuint, uint, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2fNV, GLuint, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2hNV, GLuint, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttrib2sNV, GLuint, uint, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3dNV, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3fNV, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3hNV, GLuint, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttrib3sNV, GLuint, uint, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4dNV, GLuint, uint, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4fNV, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4hNV, GLuint, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint, GLhalfNV, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4sNV, GLuint, uint, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttrib4ubNV, GLuint, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribFormatNV, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribIFormatNV, GLuint, uint, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1i64NV, GLuint, uint, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribL1ui64NV, GLuint, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribL2i64NV, GLuint, uint, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribL2ui64NV, GLuint, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribL3i64NV, GLuint, uint, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribL3ui64NV, GLuint, uint, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribL4i64NV, GLuint, uint, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int, GLint64EXT, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexAttribL4ui64NV, GLuint, uint, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint, GLuint64EXT, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribLFormatNV, GLuint, uint, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexFormatNV, GLint, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glVertexWeighthNV, GLhalfNV, uint)
#endif /* DUK_GL_NV */

#ifdef DUK_GL_OPENGL_1_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAccum, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAlphaFunc, GLenum, uint, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glArrayElement, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBegin, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTexture, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendFunc, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCallList, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClear, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearAccum, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColor, GLclampf, number, GLclampf, number, GLclampf, number, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepth, GLclampd, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearIndex, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearStencil, GLint, int)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColorMask, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glColorMaterial, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyPixels, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glCopyTexImage1D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexImage2D, GLenum, uint, GLint, int, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTexSubImage1D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTexSubImage2D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCullFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeleteLists, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthFunc, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDepthMask, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRange, GLclampd, number, GLclampd, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawArrays, GLenum, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDrawBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEdgeFlag, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnable, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEnd)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndList)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalCoord1d, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalCoord1f, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalCoord2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalCoord2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glEvalMesh1, GLenum, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glEvalMesh2, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalPoint1, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalPoint2, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFinish)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFlush)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogi, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFrontFace, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFrustum, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenLists, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetError, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glHint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexMask, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexd, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexf, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexi, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexs, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexub, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glInitNames)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsEnabled, boolean, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsList, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsTexture, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightModelf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightModeli, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glLightf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glLighti, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLineStipple, GLint, int, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLineWidth, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glListBase, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glLoadIdentity)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLoadName, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLogicOp, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMapGrid1d, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMapGrid1f, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glMapGrid2d, GLint, int, GLdouble, number, GLdouble, number, GLint, int, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glMapGrid2f, GLint, int, GLfloat, number, GLfloat, number, GLint, int, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMaterialf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMateriali, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMatrixMode, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNewList, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3b, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glOrtho, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPassThrough, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStoref, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStorei, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTransferf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTransferi, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelZoom, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPointSize, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonMode, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonOffset, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopAttrib)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopClientAttrib)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopMatrix)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopName)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushClientAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPushMatrix)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushName, GLuint, uint)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReadBuffer, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRectd, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRectf, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRecti, GLint, int, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRects, GLshort, int, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glRenderMode, int, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRotated, GLdouble, number, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRotatef, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glScaled, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glScalef, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glScissor, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glShadeModel, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilFunc, GLenum, uint, GLint, int, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glStencilMask, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glStencilOp, GLenum, uint, GLenum, uint, GLenum, uint)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexEnvf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexEnvi, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGend, GLenum, uint, GLenum, uint, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGenf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGeni, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTranslated, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTranslatef, GLfloat, number, GLfloat, number, GLfloat, number)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glViewport, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColor, GLclampf, number, GLclampf, number, GLclampf, number, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBlendEquation, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorSubTable, GLenum, uint, GLsizei, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorTable, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyConvolutionFilter1D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyConvolutionFilter2D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTexSubImage3D, GLenum, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glHistogram, GLenum, uint, GLsizei, int, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMinmax, GLenum, uint, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetHistogram, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetMinmax, GLenum, uint)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleCoverage, GLfloat, number, GLboolean, boolean)
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColor, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBlendEquation, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendFuncSeparate, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFogCoordd, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFogCoordf, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameterf, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameteri, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3b, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3s, GLshort, int, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3ub, GLubyte, uint, GLubyte, uint, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3ui, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSecondaryColor3us, GLushort, uint, GLushort, uint, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2d, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2f, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2i, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glWindowPos2s, GLshort, int, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3d, GLdouble, number, GLdouble, number, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3f, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3i, GLint, int, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glWindowPos3s, GLshort, int, GLshort, int, GLshort, int)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindAttribLocation, GLuint, uint, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationSeparate, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glCompileShader, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glCreateProgram, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glCreateShader, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteProgram, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeleteShader, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDetachShader, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableVertexAttribArray, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableVertexAttribArray, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetAttribLocation, int, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetUniformLocation, int, GLuint, uint, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBindFragDataLocation, GLuint, uint, GLuint, uint, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetFragDataLocation, int, GLuint, uint, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetUniformBlockIndex, uint, GLuint, uint, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBindFragDataLocationIndexed, GLuint, uint, GLuint, uint, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindSampler, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glColorP3ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glColorP4ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glGetFragDataIndex, int, GLuint, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsSampler, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoordP1ui, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoordP2ui, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoordP3ui, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoordP4ui, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNormalP3ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glQueryCounter, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSamplerParameterf, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glSamplerParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSecondaryColorP3ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoordP1ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoordP2ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoordP3ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoordP4ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexAttribDivisor, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP1ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP2ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP3ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertexAttribP4ui, GLuint, uint, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexP2ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexP3ui, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexP4ui, GLenum, uint, GLuint, uint)
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
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetSubroutineIndex, uint, GLuint, uint, GLenum, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetSubroutineUniformLocation, int, GLuint, uint, GLenum, uint, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glDebugMessageInsert, GLenum, uint, GLenum, uint, GLuint, uint, GLenum, uint, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDispatchCompute, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFramebufferParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetProgramResourceIndex, uint, GLuint, uint, GLenum, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetProgramResourceLocation, int, GLuint, uint, GLenum, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glGetProgramResourceLocationIndex, int, GLuint, uint, GLenum, uint, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glInvalidateBufferData, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glInvalidateTexImage, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glInvalidateTexSubImage, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glObjectLabel, GLenum, uint, GLuint, uint, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopDebugGroup)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glPushDebugGroup, GLenum, uint, GLuint, uint, GLsizei, int, const GLchar *, string)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAccumxOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glAlphaFuncxOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginConditionalRenderNVX, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBeginPerfMonitorAMD, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBindTextureUnit, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glBindVertexArrayAPPLE, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glBlendBarrierKHR)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendColorxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glBlendEquationIndexedAMD, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendEquationSeparateIndexedAMD, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBlendFuncIndexedAMD, GLuint, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glBlendFuncSeparateINGR, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glBlendFuncSeparateIndexedAMD, GLuint, uint, GLenum, uint, GLenum, uint, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(glBlitNamedFramebuffer, GLuint, uint, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLbitfield, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glBufferParameteriAPPLE, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glCheckNamedFramebufferStatus, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearAccumxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glClearColorxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepthfOES, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glClearDepthxOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glClipControl, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glColor3xOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glColor4xOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameterf, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameteri, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glConvolutionParameterxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorSubTable, GLenum, uint, GLsizei, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorTable, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyConvolutionFilter1D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyConvolutionFilter2D, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glCopyTextureSubImage1D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glCopyTextureSubImage2D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glCopyTextureSubImage3D, GLuint, uint, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glDebugMessageInsertAMD, GLenum, uint, GLenum, uint, GLuint, uint, GLsizei, int, const GLchar *, string)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRangefOES, GLclampf, number, GLclampf, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDepthRangexOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableVertexArrayAttrib, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDisableVertexAttribAPPLE, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glDrawElementArrayAPPLE, GLenum, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glDrawRangeElementArrayAPPLE, GLenum, uint, GLuint, uint, GLuint, uint, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableVertexArrayAttrib, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEnableVertexAttribAPPLE, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glEndConditionalRenderNVX)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEndPerfMonitorAMD, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEvalCoord1xOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glEvalCoord2xOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFinishFenceAPPLE, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFinishObjectAPPLE, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFinishTextureSUNX)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFogxOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFrameTerminatorGREMEDY)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFramebufferTextureMultiviewOVR, GLenum, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFrustumfOES, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glFrustumxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGenerateTextureMipmap, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetGraphicsResetStatus, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glGetMaterialxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glHintPGI, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glHistogram, GLenum, uint, GLsizei, int, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glIndexxOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsFenceAPPLE, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsNameAMD, boolean, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsVertexArrayAPPLE, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glIsVertexAttribEnabledAPPLE, boolean, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightModelxOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glLightxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLineWidthxOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glMap1xOES, GLenum, uint, GLfixed, int, GLfixed, int, GLint, int, GLint, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glMap2xOES, GLenum, uint, GLfixed, int, GLfixed, int, GLint, int, GLint, int, GLfixed, int, GLfixed, int, GLint, int, GLint, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMapGrid1xOES, GLint, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMapGrid2xOES, GLint, int, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMaterialxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glMemoryBarrierByRegion, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMinmax, GLenum, uint, GLenum, uint, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1bOES, GLenum, uint, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glMultiTexCoord1xOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2bOES, GLenum, uint, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glMultiTexCoord2xOES, GLenum, uint, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3bOES, GLenum, uint, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glMultiTexCoord3xOES, GLenum, uint, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4bOES, GLenum, uint, GLbyte, int, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glMultiTexCoord4xOES, GLenum, uint, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNamedFramebufferDrawBuffer, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNamedFramebufferParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glNamedFramebufferReadBuffer, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferRenderbuffer, GLuint, uint, GLenum, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedFramebufferTexture, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedFramebufferTextureLayer, GLuint, uint, GLenum, uint, GLuint, uint, GLint, int, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glNamedRenderbufferStorage, GLuint, uint, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glNamedRenderbufferStorageMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glNormal3xOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glObjectPurgeableAPPLE, uint, GLenum, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(glObjectUnpurgeableAPPLE, uint, GLenum, uint, GLuint, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glOrthofOES, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glOrthoxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPassThroughxOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelStorex, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTexGenParameterfSGIS, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTexGenParameteriSGIS, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelTransferxOES, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPixelZoomxOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPointParameterfSGIS, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPointSizexOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glPolygonOffsetxOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glQueryObjectParameteruiAMD, GLenum, uint, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glRasterPos2xOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glRasterPos3xOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRasterPos4xOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRectxOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetHistogram, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glResetMinmax, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glRotatexOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSampleMaskSGIS, GLclampf, number, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glSamplePatternSGIS, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glScalexOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glSetFenceAPPLE, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glStencilOpValueAMD, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTbufferMask3DFX, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTessellationFactorAMD, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTessellationModeAMD, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glTestFenceAPPLE, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(glTestObjectAPPLE, boolean, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1bOES, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glTexCoord1xOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2bOES, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glTexCoord2xOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3bOES, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexCoord3xOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4bOES, GLbyte, int, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTexCoord4xOES, GLfixed, int, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexEnvxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexGenxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTexParameterxOES, GLenum, uint, GLenum, uint, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTexStorageSparseAMD, GLenum, uint, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLsizei, int, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glTextureBarrier)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureBuffer, GLuint, uint, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureColorMaskSGIS, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureParameterf, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTextureParameteri, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glTextureStorage1D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glTextureStorage2D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTextureStorage2DMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glTextureStorage3D, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glTextureStorage3DMultisample, GLuint, uint, GLsizei, int, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLboolean, boolean)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTextureStorageSparseAMD, GLuint, uint, GLenum, uint, GLenum, uint, GLsizei, int, GLsizei, int, GLsizei, int, GLsizei, int, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTransformFeedbackBufferBase, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glTranslatexOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glUnmapNamedBuffer, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex2bOES, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glVertex2xOES, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex3bOES, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertex3xOES, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glVertex4bOES, GLbyte, int, GLbyte, int, GLbyte, int, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertex4xOES, GLfixed, int, GLfixed, int, GLfixed, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayAttribBinding, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glVertexArrayAttribFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLboolean, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayAttribIFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glVertexArrayAttribLFormat, GLuint, uint, GLuint, uint, GLint, int, GLenum, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexArrayBindingDivisor, GLuint, uint, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexArrayElementBuffer, GLuint, uint, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glVertexArrayParameteriAPPLE, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glVertexAttribParameteriAMD, GLuint, uint, GLenum, uint, GLint, int)
#endif /* DUK_GL_OPENGL_4_5 */

#ifdef DUK_GL_SGI
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glCopyColorTableSGI, GLenum, uint, GLenum, uint, GLint, int, GLint, int, GLsizei, int)
#endif /* DUK_GL_SGI */

#ifdef DUK_GL_SGIX
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glAsyncMarkerSGIX, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDeformSGIX, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeleteAsyncMarkersSGIX, GLuint, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glFlushRasterSGIX)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFragmentColorMaterialSGIX, GLenum, uint, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFragmentLightModelfSGIX, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glFragmentLightModeliSGIX, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFragmentLightfSGIX, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFragmentLightiSGIX, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFragmentMaterialfSGIX, GLenum, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glFragmentMaterialiSGIX, GLenum, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glFrameZoomSGIX, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glGenAsyncMarkersSGIX, uint, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(glGetInstrumentsSGIX, int)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(glIsAsyncMarkerSGIX, boolean, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glLightEnviSGIX, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glListParameterfSGIX, GLuint, uint, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(glListParameteriSGIX, GLuint, uint, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glLoadIdentityDeformationMapSGIX, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPixelTexGenSGIX, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReadInstrumentsSGIX, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSpriteParameterfSGIX, GLenum, uint, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glSpriteParameteriSGIX, GLenum, uint, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glStartInstrumentsSGIX)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glStopInstrumentsSGIX, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glTagSampleBufferSGIX)
#endif /* DUK_GL_SGIX */

#ifdef DUK_GL_SUN
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glColor3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(glColor4fNormal3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glColor4ubVertex2fSUN, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glColor4ubVertex3fSUN, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glDrawMeshArraysSUN, GLenum, uint, GLint, int, GLsizei, int, GLsizei, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactorbSUN, GLbyte, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactordSUN, GLdouble, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactorfSUN, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactoriSUN, GLint, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactorsSUN, GLshort, int)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactorubSUN, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactoruiSUN, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glGlobalAlphaFactorusSUN, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glNormal3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReplacementCodeubSUN, GLubyte, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glReplacementCodeuiColor3fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG11(glReplacementCodeuiColor4fNormal3fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glReplacementCodeuiColor4ubVertex3fSUN, GLuint, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(glReplacementCodeuiNormal3fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReplacementCodeuiSUN, GLuint, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG13(glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(glReplacementCodeuiTexCoord2fVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(glReplacementCodeuiVertex3fSUN, GLuint, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glReplacementCodeusSUN, GLushort, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTexCoord2fColor3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(glTexCoord2fColor4fNormal3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(glTexCoord2fColor4ubVertex3fSUN, GLfloat, number, GLfloat, number, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLubyte, uint, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTexCoord2fNormal3fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(glTexCoord2fVertex3fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(glTexCoord4fColor4fNormal3fVertex4fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(glTexCoord4fVertex4fSUN, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number, GLfloat, number)
#endif /* DUK_GL_SUN */

/*
 *  OpenGL function bindings to JavaScript
 */
void duk_gl_bind_opengl_functions(duk_context *ctx)
{
#ifdef DUK_GL_ARB
	duk_gl_bind_opengl_wrapper(ctx, glActiveTextureARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginQueryARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindBufferARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindProgramARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateiARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationiARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparateiARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunciARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glClampColorARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClientActiveTextureARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCurrentPaletteMatrixARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageInsertARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteNamedStringARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexAttribArrayARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchComputeGroupSizeARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstancedARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexAttribArrayARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEndQueryARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvaluateDepthValuesARB, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureFaceARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureLayerARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatusARB, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetImageHandleARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureHandleARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureSamplerHandleARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsBufferARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsImageHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsNamedStringARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgramARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsQueryARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleResidentARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMaxShaderCompilerThreadsARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinSampleShadingARB, 1);
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
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterfARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPrimitiveBoundingBoxARB, 8);
	duk_gl_bind_opengl_wrapper(ctx, glProgramEnvParameter4dARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramEnvParameter4fARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramLocalParameter4dARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramLocalParameter4fARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameteriARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1i64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1ui64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2i64ARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2ui64ARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3i64ARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3ui64ARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4i64ARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4ui64ARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformHandleui64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSampleCoverageARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexBufferARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexPageCommitmentARB, 9);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1fARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1i64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1iARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1ui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2fARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2i64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2iARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2ui64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3fARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3i64ARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3iARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3ui64ARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4fARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4i64ARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4iARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4ui64ARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniformHandleui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapBufferARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1dARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1fARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1sARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2dARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2fARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2sARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3dARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3fARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3sARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4NubARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4dARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4fARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4sARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribDivisorARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1ui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexBlendARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2dARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2fARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2iARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2sARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3dARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3fARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3iARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3sARB, 3);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFragmentOp1ATI, 6);
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFragmentOp2ATI, 9);
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFragmentOp3ATI, 12);
	duk_gl_bind_opengl_wrapper(ctx, glArrayObjectATI, 6);
	duk_gl_bind_opengl_wrapper(ctx, glBeginFragmentShaderATI, 0);
	duk_gl_bind_opengl_wrapper(ctx, glBindFragmentShaderATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClientActiveVertexStreamATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColorFragmentOp1ATI, 7);
	duk_gl_bind_opengl_wrapper(ctx, glColorFragmentOp2ATI, 10);
	duk_gl_bind_opengl_wrapper(ctx, glColorFragmentOp3ATI, 13);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteFragmentShaderATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDrawElementArrayATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawRangeElementArrayATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glEndFragmentShaderATI, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFreeObjectBufferATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenFragmentShadersATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsObjectBufferATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNormalStream3bATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNormalStream3dATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNormalStream3fATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNormalStream3iATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNormalStream3sATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPNTrianglesfATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPNTrianglesiATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPassTexCoordATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMapATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFuncSeparateATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOpSeparateATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapObjectBufferATI, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVariantArrayObjectATI, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribArrayObjectATI, 7);
	duk_gl_bind_opengl_wrapper(ctx, glVertexBlendEnvfATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexBlendEnviATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream1dATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream1fATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream1iATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream1sATI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream2dATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream2fATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream2iATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream2sATI, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream3dATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream3fATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream3iATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream3sATI, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream4dATI, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream4fATI, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream4iATI, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexStream4sATI, 5);
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_EXT
	duk_gl_bind_opengl_wrapper(ctx, glActiveProgramEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glActiveStencilFaceEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glApplyTextureEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glArrayElementEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginTransformFeedbackEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginVertexShaderEXT, 0);
	duk_gl_bind_opengl_wrapper(ctx, glBindBufferBaseEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindFragDataLocationEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindFramebufferEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindImageTextureEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glBindLightParameterEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindMaterialParameterEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindMultiTextureEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindParameterEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBindRenderbufferEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindTexGenParameterEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindTextureEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindTextureUnitParameterEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindVertexShaderEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBinormal3bEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBinormal3dEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBinormal3fEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBinormal3iEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBinormal3sEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendColorEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparateEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlitFramebufferEXT, 10);
	duk_gl_bind_opengl_wrapper(ctx, glCheckFramebufferStatusEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCheckNamedFramebufferStatusEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearColorIiEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearColorIuiEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClientAttribDefaultEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColorMaskIndexedEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameterfEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameteriEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorSubTableEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter1DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter2DEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyMultiTexImage1DEXT, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyMultiTexImage2DEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyMultiTexSubImage1DEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyMultiTexSubImage2DEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyMultiTexSubImage3DEXT, 10);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage1DEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage2DEXT, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage1DEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage2DEXT, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage3DEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureImage1DEXT, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureImage2DEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage1DEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage2DEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage3DEXT, 10);
	duk_gl_bind_opengl_wrapper(ctx, glCreateShaderProgramEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteVertexShaderEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthBoundsEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableClientStateIndexedEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableClientStateiEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableIndexedEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVariantClientStateEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexArrayAttribEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexArrayEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstancedEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glEnableClientStateIndexedEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableClientStateiEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableIndexedEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVariantClientStateEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexArrayAttribEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexArrayEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndTransformFeedbackEXT, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndVertexShaderEXT, 0);
	duk_gl_bind_opengl_wrapper(ctx, glExtractComponentEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoorddEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoordfEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferDrawBufferEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferReadBufferEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferRenderbufferEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture1DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture2DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture3DEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureFaceEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureLayerEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGenSymbolsEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGenVertexShadersEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateMipmapEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateMultiTexMipmapEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateTextureMipmapEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetFragDataLocationEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformBufferSizeEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glHistogramEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glIndexFuncEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIndexMaterialEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glInsertComponentEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glInsertEventMarkerEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabledIndexedEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsFramebufferEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsRenderbufferEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTextureEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsVariantEnabledEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLabelObjectEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glLockArraysEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixFrustumEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixLoadIdentityEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixOrthoEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixPopEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixPushEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixRotatedEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixRotatefEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixScaledEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixScalefEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixTranslatedEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixTranslatefEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrierEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinmaxEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexBufferEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexEnvfEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexEnviEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexGendEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexGenfEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexGeniEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexParameterfEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexParameteriEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexRenderbufferEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferParameteriEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferRenderbufferEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture1DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture2DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture3DEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureFaceEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureLayerEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedProgramLocalParameter4dEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glNamedProgramLocalParameter4fEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glNamedProgramLocalParameterI4iEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glNamedProgramLocalParameterI4uiEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageMultisampleCoverageEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageMultisampleEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransformParameterfEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransformParameteriEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterfEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffsetClampEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffsetEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPopGroupMarkerEXT, 0);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameteriEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1dEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1fEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1iEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1uiEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2dEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2fEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2iEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2uiEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3dEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3fEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3iEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3uiEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4dEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4fEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4iEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4uiEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProvokingVertexEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushClientAttribDefaultEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushGroupMarkerEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterSamplesEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorageEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorageMultisampleEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glResetHistogramEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glResetMinmaxEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMaskEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSamplePatternEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3bEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3dEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3fEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3iEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3sEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3ubEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3uiEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3usEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glShaderOp1EXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glShaderOp2EXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glShaderOp3EXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glStencilClearTagEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSwizzleEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTangent3bEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTangent3dEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTangent3fEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTangent3iEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTangent3sEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexBufferEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBufferEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureLightEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTextureMaterialEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTextureNormalEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexturePageCommitmentEXT, 9);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterfEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameteriEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureRenderbufferEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage1DEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2DEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2DMultisampleEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3DEXT, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3DMultisampleEXT, 8);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1uiEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2uiEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3uiEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4uiEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniformBufferEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUnlockArraysEXT, 0);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapNamedBufferEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glUseShaderProgramEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexAttribBindingEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexAttribDivisorEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexAttribFormatEXT, 6);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexAttribIFormatEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexAttribLFormatEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayVertexBindingDivisorEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1iEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1uiEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2iEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2uiEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3iEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3uiEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4iEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4uiEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1dEXT, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2dEXT, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3dEXT, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4dEXT, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexWeightfEXT, 1);
	duk_gl_bind_opengl_wrapper(ctx, glWriteMaskEXT, 6);
#endif /* DUK_GL_EXT */

#ifdef DUK_GL_HP
	duk_gl_bind_opengl_wrapper(ctx, glImageTransformParameterfHP, 3);
	duk_gl_bind_opengl_wrapper(ctx, glImageTransformParameteriHP, 3);
#endif /* DUK_GL_HP */

#ifdef DUK_GL_IBM
	duk_gl_bind_opengl_wrapper(ctx, glFlushStaticDataIBM, 1);
#endif /* DUK_GL_IBM */

#ifdef DUK_GL_INTEL
	duk_gl_bind_opengl_wrapper(ctx, glApplyFramebufferAttachmentCMAAINTEL, 0);
	duk_gl_bind_opengl_wrapper(ctx, glBeginPerfQueryINTEL, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeletePerfQueryINTEL, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEndPerfQueryINTEL, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSyncTextureINTEL, 1);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapTexture2DINTEL, 2);
#endif /* DUK_GL_INTEL */

#ifdef DUK_GL_MESA
	duk_gl_bind_opengl_wrapper(ctx, glDisableTraceMESA, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableTraceMESA, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEndTraceMESA, 0);
	duk_gl_bind_opengl_wrapper(ctx, glResizeBuffersMESA, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTraceAssertAttribMESA, 1);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2dMESA, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2fMESA, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2iMESA, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2sMESA, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3dMESA, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3fMESA, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3iMESA, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3sMESA, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos4dMESA, 4);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos4fMESA, 4);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos4iMESA, 4);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos4sMESA, 4);
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_NV
	duk_gl_bind_opengl_wrapper(ctx, glActiveVaryingNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBeginConditionalRenderNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBeginOcclusionQueryNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginTransformFeedbackNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginVideoCaptureNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBindBufferBaseNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindProgramNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindTransformFeedbackNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindVideoCaptureStreamTextureNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendBarrierNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glBlendParameteriNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCallCommandListNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepthdNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glColor3hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor4hNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColorFormatNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCombinerInputNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCombinerOutputNV, 10);
	duk_gl_bind_opengl_wrapper(ctx, glCombinerParameterfNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCombinerParameteriNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCommandListSegmentsNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCompileCommandListNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glConservativeRasterParameterfNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCopyImageSubDataNV, 15);
	duk_gl_bind_opengl_wrapper(ctx, glCopyPathNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCoverFillPathNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCoverStrokePathNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCoverageModulationNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeletePathsNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDepthBoundsdNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangedNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTextureNV, 11);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEdgeFlagFormatNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEndConditionalRenderNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndOcclusionQueryNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndTransformFeedbackNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndVideoCaptureNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalMapsNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFinalCombinerInputNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFinishFenceNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFlushPixelDataRangeNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFlushVertexArrayRangeNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoordFormatNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoordhNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentCoverageColorNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenPathsNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetCommandHeaderNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetImageHandleNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetPathLengthNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetStageIndexNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureHandleNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureSamplerHandleNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetVaryingLocationNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIndexFormatNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glInterpolatePathsNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glIsBufferResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsCommandListNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsFenceNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsImageHandleResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsNamedBufferResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsOcclusionQueryNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsPathNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsPointInFillPathNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glIsPointInStrokePathNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgramNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsStateNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTextureHandleResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTransformFeedbackNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeBufferNonResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeBufferResidentNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleNonResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleResidentNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeNamedBufferNonResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeNamedBufferResidentNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleNonResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleResidentNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1hNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3hNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4hNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormalFormatNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPathCoverDepthFuncNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPathFogGenNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPathParameterfNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPathParameteriNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPathStencilDepthOffsetNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPathStencilFuncNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glPauseTransformFeedbackNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameteriNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPresentFrameDualFillNV, 13);
	duk_gl_bind_opengl_wrapper(ctx, glPresentFrameKeyedNV, 11);
	duk_gl_bind_opengl_wrapper(ctx, glPrimitiveRestartIndexNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPrimitiveRestartNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glProgramEnvParameterI4iNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramEnvParameterI4uiNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramLocalParameterI4iNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramLocalParameterI4uiNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameter4dNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameter4fNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1i64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1ui64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2i64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2ui64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3i64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3ui64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4i64NV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4ui64NV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformHandleui64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformui64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramVertexLimitNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorageMultisampleCoverageNV, 6);
	duk_gl_bind_opengl_wrapper(ctx, glResolveDepthValuesNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glResumeTransformFeedbackNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMaskIndexedNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColorFormatNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSetFenceNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glStateCaptureNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFillPathNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilStrokePathNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilThenCoverFillPathNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilThenCoverStrokePathNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glSubpixelPrecisionBiasNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTestFenceNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1hNV, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2hNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4hNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoordFormatNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage2DMultisampleCoverageNV, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage3DMultisampleCoverageNV, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTexRenderbufferNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBarrierNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTextureImage2DMultisampleCoverageNV, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTextureImage2DMultisampleNV, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureImage3DMultisampleCoverageNV, 9);
	duk_gl_bind_opengl_wrapper(ctx, glTextureImage3DMultisampleNV, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTrackMatrixNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1i64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1ui64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2i64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2ui64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3i64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3ui64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4i64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4ui64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniformHandleui64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniformui64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVDPAUFiniNV, 0);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2hNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4hNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1dNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1fNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1hNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1sNV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2dNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2fNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2hNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2sNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3dNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3fNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3hNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3sNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4dNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4fNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4hNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4sNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4ubNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribFormatNV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribIFormatNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1i64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1ui64NV, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2i64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2ui64NV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3i64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3ui64NV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4i64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4ui64NV, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribLFormatNV, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexFormatNV, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexWeighthNV, 1);
#endif /* DUK_GL_NV */

#ifdef DUK_GL_OPENGL_1_1
	duk_gl_bind_opengl_wrapper(ctx, glAccum, 2);
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFunc, 2);
	duk_gl_bind_opengl_wrapper(ctx, glArrayElement, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBegin, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBindTexture, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunc, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCallList, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClear, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearAccum, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearIndex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearStencil, 1);
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
	duk_gl_bind_opengl_wrapper(ctx, glColorMask, 4);
	duk_gl_bind_opengl_wrapper(ctx, glColorMaterial, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCopyPixels, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage1D, 7);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCullFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteLists, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDepthFunc, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRange, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArrays, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEdgeFlag, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnable, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnd, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndList, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord1d, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord1f, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEvalMesh1, 3);
	duk_gl_bind_opengl_wrapper(ctx, glEvalMesh2, 5);
	duk_gl_bind_opengl_wrapper(ctx, glEvalPoint1, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalPoint2, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFinish, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFlush, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFogf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFogi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFrontFace, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFrustum, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGenLists, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetError, 0);
	duk_gl_bind_opengl_wrapper(ctx, glHint, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIndexMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexd, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexf, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexi, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexs, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIndexub, 1);
	duk_gl_bind_opengl_wrapper(ctx, glInitNames, 0);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabled, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsList, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsTexture, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLightModelf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLightModeli, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLightf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLighti, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLineStipple, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLineWidth, 1);
	duk_gl_bind_opengl_wrapper(ctx, glListBase, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLoadIdentity, 0);
	duk_gl_bind_opengl_wrapper(ctx, glLoadName, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLogicOp, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid1d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid1f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid2d, 6);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid2f, 6);
	duk_gl_bind_opengl_wrapper(ctx, glMaterialf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMateriali, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMatrixMode, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNewList, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3b, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glOrtho, 6);
	duk_gl_bind_opengl_wrapper(ctx, glPassThrough, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStoref, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStorei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransferf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransferi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelZoom, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointSize, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonMode, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffset, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPopAttrib, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPopClientAttrib, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPopMatrix, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPopName, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPushAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushClientAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushMatrix, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPushName, 1);
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
	duk_gl_bind_opengl_wrapper(ctx, glReadBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glRectd, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRectf, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRecti, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRects, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRenderMode, 1);
	duk_gl_bind_opengl_wrapper(ctx, glRotated, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRotatef, 4);
	duk_gl_bind_opengl_wrapper(ctx, glScaled, 3);
	duk_gl_bind_opengl_wrapper(ctx, glScalef, 3);
	duk_gl_bind_opengl_wrapper(ctx, glScissor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glShadeModel, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFunc, 3);
	duk_gl_bind_opengl_wrapper(ctx, glStencilMask, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOp, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glTexEnvf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexEnvi, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGend, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGenf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGeni, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTranslated, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTranslatef, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glViewport, 4);
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_bind_opengl_wrapper(ctx, glBlendColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquation, 1);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorSubTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter1D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter2D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTexSubImage3D, 9);
	duk_gl_bind_opengl_wrapper(ctx, glHistogram, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMinmax, 3);
	duk_gl_bind_opengl_wrapper(ctx, glResetHistogram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glResetMinmax, 1);
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

#ifdef DUK_GL_OPENGL_1_4
	duk_gl_bind_opengl_wrapper(ctx, glBlendColor, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquation, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoordd, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFogCoordf, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameteri, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3b, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3ub, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColor3us, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos2s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glWindowPos3s, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glBindAttribLocation, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparate, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCompileShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCreateProgram, 0);
	duk_gl_bind_opengl_wrapper(ctx, glCreateShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDetachShader, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexAttribArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexAttribArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetAttribLocation, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformLocation, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glBindFragDataLocation, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glGetFragDataLocation, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformBlockIndex, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glBindFragDataLocationIndexed, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBindSampler, 2);
	duk_gl_bind_opengl_wrapper(ctx, glColorP3ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glColorP4ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetFragDataIndex, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsSampler, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoordP1ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoordP2ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoordP3ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoordP4ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNormalP3ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glQueryCounter, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSecondaryColorP3ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoordP1ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoordP2ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoordP3ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoordP4ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribDivisor, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP1ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP2ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP4ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexP2ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexP3ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexP4ui, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glGetSubroutineIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSubroutineUniformLocation, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageInsert, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchCompute, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceLocation, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceLocationIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateBufferData, 1);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateTexImage, 2);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateTexSubImage, 8);
	duk_gl_bind_opengl_wrapper(ctx, glObjectLabel, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPopDebugGroup, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPushDebugGroup, 4);
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
	duk_gl_bind_opengl_wrapper(ctx, glAccumxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glAlphaFuncxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBeginConditionalRenderNVX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBeginPerfMonitorAMD, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBindTextureUnit, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindVertexArrayAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glBlendBarrierKHR, 0);
	duk_gl_bind_opengl_wrapper(ctx, glBlendColorxOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationIndexedAMD, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparateIndexedAMD, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncIndexedAMD, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparateINGR, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparateIndexedAMD, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlitNamedFramebuffer, 12);
	duk_gl_bind_opengl_wrapper(ctx, glBufferParameteriAPPLE, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCheckNamedFramebufferStatus, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearAccumxOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearColorxOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepthfOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepthxOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClipControl, 2);
	duk_gl_bind_opengl_wrapper(ctx, glColor3xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColor4xOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glConvolutionParameterxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorSubTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorTable, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter1D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glCopyConvolutionFilter2D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage3D, 9);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageInsertAMD, 5);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangefOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangexOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexAttribAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawElementArrayAPPLE, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawRangeElementArrayAPPLE, 5);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexAttribAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndConditionalRenderNVX, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndPerfMonitorAMD, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord1xOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glEvalCoord2xOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFinishFenceAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glFinishObjectAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFinishTextureSUNX, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFogxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFrameTerminatorGREMEDY, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureMultiviewOVR, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFrustumfOES, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFrustumxOES, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateTextureMipmap, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatus, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetMaterialxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glHintPGI, 2);
	duk_gl_bind_opengl_wrapper(ctx, glHistogram, 4);
	duk_gl_bind_opengl_wrapper(ctx, glIndexxOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsFenceAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsNameAMD, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsVertexArrayAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsVertexAttribEnabledAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLightModelxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glLightxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLineWidthxOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMap1xOES, 6);
	duk_gl_bind_opengl_wrapper(ctx, glMap2xOES, 10);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid1xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMapGrid2xOES, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMaterialxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrierByRegion, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinmax, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1bOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord1xOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2bOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord2xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3bOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord3xOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4bOES, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMultiTexCoord4xOES, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferDrawBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferReadBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferRenderbuffer, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureLayer, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorage, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageMultisample, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glObjectPurgeableAPPLE, 3);
	duk_gl_bind_opengl_wrapper(ctx, glObjectUnpurgeableAPPLE, 3);
	duk_gl_bind_opengl_wrapper(ctx, glOrthofOES, 6);
	duk_gl_bind_opengl_wrapper(ctx, glOrthoxOES, 6);
	duk_gl_bind_opengl_wrapper(ctx, glPassThroughxOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPixelStorex, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTexGenParameterfSGIS, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTexGenParameteriSGIS, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTransferxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPixelZoomxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterfSGIS, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointSizexOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPolygonOffsetxOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glQueryObjectParameteruiAMD, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos2xOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos3xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glRasterPos4xOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRectxOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glResetHistogram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glResetMinmax, 1);
	duk_gl_bind_opengl_wrapper(ctx, glRotatexOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMaskSGIS, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSamplePatternSGIS, 1);
	duk_gl_bind_opengl_wrapper(ctx, glScalexOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSetFenceAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOpValueAMD, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTbufferMask3DFX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTessellationFactorAMD, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTessellationModeAMD, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTestFenceAPPLE, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTestObjectAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1bOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord1xOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2bOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2xOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3bOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord3xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4bOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4xOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexEnvxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexGenxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterxOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorageSparseAMD, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBarrier, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureColorMaskSGIS, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage1D, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage2DMultisample, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorage3DMultisample, 7);
	duk_gl_bind_opengl_wrapper(ctx, glTextureStorageSparseAMD, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTransformFeedbackBufferBase, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTranslatexOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUnmapNamedBuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2bOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex2xOES, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3bOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertex3xOES, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4bOES, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertex4xOES, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribBinding, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribFormat, 6);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribIFormat, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayAttribLFormat, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayBindingDivisor, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayElementBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexArrayParameteriAPPLE, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribParameteriAMD, 3);
#endif /* DUK_GL_OPENGL_4_5 */

#ifdef DUK_GL_SGI
	duk_gl_bind_opengl_wrapper(ctx, glCopyColorTableSGI, 5);
#endif /* DUK_GL_SGI */

#ifdef DUK_GL_SGIX
	duk_gl_bind_opengl_wrapper(ctx, glAsyncMarkerSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeformSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteAsyncMarkersSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFlushRasterSGIX, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentColorMaterialSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentLightModelfSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentLightModeliSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentLightfSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentLightiSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentMaterialfSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFragmentMaterialiSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFrameZoomSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenAsyncMarkersSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetInstrumentsSGIX, 0);
	duk_gl_bind_opengl_wrapper(ctx, glIsAsyncMarkerSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLightEnviSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glListParameterfSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glListParameteriSGIX, 3);
	duk_gl_bind_opengl_wrapper(ctx, glLoadIdentityDeformationMapSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPixelTexGenSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glReadInstrumentsSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSpriteParameterfSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSpriteParameteriSGIX, 2);
	duk_gl_bind_opengl_wrapper(ctx, glStartInstrumentsSGIX, 0);
	duk_gl_bind_opengl_wrapper(ctx, glStopInstrumentsSGIX, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTagSampleBufferSGIX, 0);
#endif /* DUK_GL_SGIX */

#ifdef DUK_GL_SUN
	duk_gl_bind_opengl_wrapper(ctx, glColor3fVertex3fSUN, 6);
	duk_gl_bind_opengl_wrapper(ctx, glColor4fNormal3fVertex3fSUN, 10);
	duk_gl_bind_opengl_wrapper(ctx, glColor4ubVertex2fSUN, 6);
	duk_gl_bind_opengl_wrapper(ctx, glColor4ubVertex3fSUN, 7);
	duk_gl_bind_opengl_wrapper(ctx, glDrawMeshArraysSUN, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactorbSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactordSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactorfSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactoriSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactorsSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactorubSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactoruiSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGlobalAlphaFactorusSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNormal3fVertex3fSUN, 6);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeubSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiColor3fVertex3fSUN, 7);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiColor4fNormal3fVertex3fSUN, 11);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiColor4ubVertex3fSUN, 8);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiNormal3fVertex3fSUN, 7);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN, 13);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN, 9);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiTexCoord2fVertex3fSUN, 6);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeuiVertex3fSUN, 4);
	duk_gl_bind_opengl_wrapper(ctx, glReplacementCodeusSUN, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2fColor3fVertex3fSUN, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2fColor4fNormal3fVertex3fSUN, 12);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2fColor4ubVertex3fSUN, 9);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2fNormal3fVertex3fSUN, 8);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord2fVertex3fSUN, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4fColor4fNormal3fVertex4fSUN, 15);
	duk_gl_bind_opengl_wrapper(ctx, glTexCoord4fVertex4fSUN, 8);
#endif /* DUK_GL_SUN */

}

/*
 *  OpenGL constants to JavaScript
 */
void duk_gl_set_constants(duk_context *ctx)
{
#ifdef DUK_GL_ARB
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_TEXTURE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_VERTEX_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD_SIGNED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BOOL_VEC4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_ACCESS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAPPED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_POINTER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_USAGE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_FRAGMENT_COLOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_READ_COLOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_BORDER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_VERTEX_COLOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ACTIVE_TEXTURE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_INPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_SUM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPARE_R_TO_TEXTURE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPLETION_STATUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_INTENSITY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_TEXTURE_FORMATS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COORD_REPLACE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_MATRIX_INDEX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_MATRIX_STACK_DEPTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_PALETTE_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_QUERY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_VERTEX_ATTRIB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_WEIGHT_ARB);
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
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT16_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT24_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_TEXTURE_MODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGBA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER0_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER10_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER11_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER12_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER13_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER14_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER15_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER1_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER5_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER6_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER7_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER8_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER9_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_COPY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_DRAW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_READ_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FIXED_ONLY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_MAT4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_VEC4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_INPUT_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_OUTPUT_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_VERTICES_OUT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_GUILTY_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INNOCENT_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERPOLATE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_VEC4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES_ADJACENCY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP_ADJACENCY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LOSE_CONTEXT_ON_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX0_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX10_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX11_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX12_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX13_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX14_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX15_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX16_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX17_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX18_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX19_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX1_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX20_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX21_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX22_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX23_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX24_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX25_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX26_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX27_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX28_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX29_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX30_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX31_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX5_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX6_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX7_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX8_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX9_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_INDEX_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_INDEX_ARRAY_POINTER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_INDEX_ARRAY_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_INDEX_ARRAY_STRIDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_INDEX_ARRAY_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_PALETTE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_LOGGED_MESSAGES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_MESSAGE_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DRAW_BUFFERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PALETTE_MATRICES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_ATTRIBS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_MATRICES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEMPORARIES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SHADER_COMPILER_THREADS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_TEXTURE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_BUFFER_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_COORDS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_IMAGE_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_FLOATS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIBS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_VARYING_COMPONENTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_SAMPLE_SHADING_VALUE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRRORED_REPEAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW0_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW10_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW11_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW12_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW13_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW14_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW15_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW16_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW17_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW18_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW19_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW1_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW20_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW21_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW22_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW23_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW24_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW25_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW26_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW27_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW28_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW29_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW30_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW31_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW5_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW6_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW7_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW8_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW9_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_LINE_WIDTH_GRANULARITY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_LINE_WIDTH_RANGE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NAMED_STRING_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NAMED_STRING_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_MAP_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_RESET_NOTIFICATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_SPARSE_LEVELS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_VIRTUAL_PAGE_SIZES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_ACTIVE_UNIFORMS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_ATTACHED_OBJECTS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_COMPILE_STATUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_DELETE_STATUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_INFO_LOG_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_LINK_STATUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_SUBTYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_VALIDATE_STATUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PARAMETER_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PARAMETER_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_DISTANCE_ATTENUATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_FADE_THRESHOLD_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MAX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MIN_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SPRITE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PREVIOUS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMARY_COLOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVES_SUBMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_BOUNDING_BOX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAMMABLE_SAMPLE_LOCATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ADDRESS_REGISTERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ALU_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ATTRIBS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ERROR_POSITION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ERROR_STRING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_FORMAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_FORMAT_ASCII_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_LENGTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_ATTRIBS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_TEMPORARIES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_OBJECT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_PARAMETERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_POINT_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_STRING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_TEMPORARIES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_TEX_INDIRECTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_TEX_INSTRUCTIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_RECTANGLE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_COUNTER_BITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_AVAILABLE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_ONLY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_WRITE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_REFLECTION_MAP_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RESET_NOTIFICATION_STRATEGY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32F_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT_MODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_SCALE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_SHADOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_RECT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_RECT_SHADOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_SHADOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_3D_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_PASSED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_INVERT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_VALUE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_SUBPIXEL_BITS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_SHADING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_INCLUDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_OBJECT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADING_LANGUAGE_VERSION_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_ALPHA_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_RGB_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_BUFFER_PAGE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_STORAGE_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_DECODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_COPY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_DRAW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_READ_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_COPY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_DRAW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_READ_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBTRACT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_CL_EVENT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_CL_EVENT_COMPLETE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_SHADER_PATCHES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE0_ARB);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE1_ARB);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE30_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE31_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE5_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE6_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE7_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE8_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE9_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_RECTANGLE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_FORMAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_FUNC_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_MODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPRESSION_HINT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTENSITY_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LUMINANCE_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RECTANGLE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_REDUCTION_MODE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SPARSE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_COLOR_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_CURRENT_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_MODELVIEW_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_PROJECTION_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_TEXTURE_MATRIX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLES_ADJACENCY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_STRIP_ADJACENCY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNKNOWN_CONTEXT_RESET_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC2_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC3_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC4_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_NORMALIZED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_BLEND_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_TWO_SIDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTICES_SUBMITTED_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_X_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Y_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Z_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHTED_AVERAGE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_POINTER_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_SIZE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_STRIDE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_TYPE_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_SUM_UNITY_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_ONLY_ARB);
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_ATI
	duk_gl_push_opengl_constant_property(ctx, GL_2X_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_4X_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_8X_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BLEND_EQUATION_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_OBJECT_BUFFER_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_OBJECT_OFFSET_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BIAS_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_ENVMAP_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_NUM_TEX_UNITS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_ROT_MATRIX_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_ROT_MATRIX_SIZE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_TARGET_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_BUMP_TEX_UNITS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CND0_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CND_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ALPHA_PAIRING_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_COMP_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_0_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_10_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_11_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_12_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_13_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_14_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_15_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_17_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_18_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_19_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_1_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_20_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_21_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_22_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_23_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_24_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_25_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_26_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_27_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_28_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_29_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_2_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_30_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_31_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_3_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_4_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_5_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_6_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_7_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_8_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_CON_9_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DISCARD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT2_ADD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT4_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER0_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER10_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER11_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER12_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER13_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER14_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER15_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER1_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER2_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER3_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER4_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER5_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER6_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER7_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER8_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER9_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DU8DV8_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DUDV_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_EIGHTH_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_POINTER_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_TYPE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_SHADER_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_LERP_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DRAW_BUFFERS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_STREAMS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_TO_EDGE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MODULATE_ADD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MODULATE_SIGNED_ADD_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MODULATE_SUBTRACT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MOV_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_MUL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATE_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_FRAGMENT_CONSTANTS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_FRAGMENT_REGISTERS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_INSTRUCTIONS_PER_PASS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_INSTRUCTIONS_TOTAL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_LOOPBACK_COMPONENTS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_PASSES_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_BUFFER_SIZE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_BUFFER_USAGE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_NORMAL_MODE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_POINT_MODE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_PRESERVE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_QUARTER_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_0_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_10_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_11_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_12_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_13_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_14_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_15_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_17_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_18_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_19_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_1_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_20_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_21_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_22_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_23_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_24_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_25_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_26_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_27_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_28_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_29_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_2_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_30_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_31_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_3_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_4_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_5_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_6_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_7_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_8_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_REG_9_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_FREE_MEMORY_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT_MODE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_FLOAT16_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_FLOAT32_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SATURATE_BIT_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_INTERPOLATOR_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_FAIL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_FUNC_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SUB_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STQ_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STQ_DQ_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STRQ_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STRQ_DQ_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STR_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_SWIZZLE_STR_DR_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FREE_MEMORY_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXT_FRAGMENT_SHADER_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VBO_FREE_MEMORY_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SOURCE_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM0_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM1_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM2_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM3_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM4_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM5_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM6_ATI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STREAM7_ATI);
#endif /* DUK_GL_ATI */

#ifdef DUK_GL_EXT
	duk_gl_push_opengl_constant_property(ctx, GL_1PASS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_2PASS_0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_2PASS_1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_422_AVERAGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_422_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_422_REV_AVERAGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_422_REV_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ABGR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_PROGRAM_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_STENCIL_FACE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD_SIGNED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_BARRIER_BITS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_ELEMENT_LOCK_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ARRAY_ELEMENT_LOCK_FIRST_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ATOMIC_COUNTER_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTENUATION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_AVERAGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BINORMAL_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BINORMAL_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BINORMAL_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BINORMAL_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_UPDATE_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_VOLUME_CLIPPING_HINT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CMYKA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CMYK_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT10_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT11_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT13_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT14_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT15_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT6_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT7_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ATTACHMENT9_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_SUM_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMMAND_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_LATC1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RED_GREEN_RGTC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RED_RGTC1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_S3TC_DXT1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SIGNED_RED_RGTC1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SLUMINANCE_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SLUMINANCE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_S3TC_DXT1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_MODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_HEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_WIDTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CUBIC_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_VERTEX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_VERTEX_EYE_POSITION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_VERTEX_OBJECT_POSITION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_BINORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_FOG_COORDINATE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_SECONDARY_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_TANGENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_VERTEX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_VERTEX_WEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DECODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR_WRAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH24_STENCIL8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_ATTACHMENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BOUNDS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BOUNDS_TEST_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DISTANCE_ATTENUATION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGBA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_MAT4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE_VEC4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_FRAMEBUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_FRAMEBUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_EFFECTIVE_RASTER_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FIRST_VERTEX_CONVENTION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_SOURCE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_MATERIAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_NORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_COMPLETE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_SRGB_CAPABLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_SRGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_UNSUPPORTED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FULL_RANGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_ADD_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_REVERSE_SUBTRACT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_SUBTRACT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_INPUT_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_OUTPUT_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_SHADER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_VERTICES_OUT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_ALPHA_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_BLUE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_GREEN_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_LUMINANCE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_RED_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_SINK_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_WIDTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_2D_RECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_ACCESS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LAYERED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LAYER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_LEVEL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BINDING_NAME_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CUBE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CUBE_MAP_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR_WRAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_MATERIAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_MATERIAL_FACE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_MATERIAL_PARAMETER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_TEST_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_TEST_FUNC_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_TEST_REF_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLEAVED_ATTRIBS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERPOLATE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_2D_RECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_CUBE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_2D_RECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_CUBE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_FRAMEBUFFER_OPERATION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INVARIANT_DATATYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INVARIANT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_INVARIANT_VALUE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IUI_N3F_V2F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IUI_N3F_V3F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IUI_V2F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_IUI_V3F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LAST_VERTEX_CONVENTION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_COLOR_CONTROL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES_ADJACENCY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP_ADJACENCY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCAL_CONSTANT_DATATYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCAL_CONSTANT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCAL_CONSTANT_VALUE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LOCAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_ALPHA16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4_ALPHA4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE6_ALPHA2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_ALPHA8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_BINORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TANGENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_BINORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TANGENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_3D_TEXTURE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ARRAY_TEXTURE_LAYERS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_BINDABLE_UNIFORM_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_ATTACHMENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_HEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_WIDTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_INDICES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_VERTICES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_IMAGE_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_IMAGE_UNITS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXEL_OFFSET_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RASTER_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RENDERBUFFER_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_BUFFER_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_LOD_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_INVARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_LOCALS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_SHADER_VARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_VARYING_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_SINK_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXEL_OFFSET_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_TO_BORDER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRROR_CLAMP_TO_EDGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW0_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW0_STACK_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW1_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW1_STACK_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_RASTERIZATION_ALLOWED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_MVP_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_ONE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_W_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_X_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_Y_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_Z_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMALIZED_RANGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_MAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_ADD_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_CLAMP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_CROSS_PRODUCT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_DOT3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_DOT4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_EXP_BASE_2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_FLOOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_FRAC_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_INDEX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_LOG_BASE_2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MADD_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MAX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MIN_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MOV_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MULTIPLY_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_MUL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_NEGATE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_POWER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_RECIP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_RECIP_SQRT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_ROUND_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_SET_GE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_SET_LT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OP_SUB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_COLOR0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_COLOR1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_FOG_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD0_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD10_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD11_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD13_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD14_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD15_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD17_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD18_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD19_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD20_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD21_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD22_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD23_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD24_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD25_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD26_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD27_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD28_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD29_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD30_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD31_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD6_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD7_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_TEXTURE_COORD9_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_OUTPUT_VERTEX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_CMYK_HINT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_IMAGE_HEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_IMAGES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PERTURB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_BUFFER_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_CUBIC_WEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAG_FILTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MIN_FILTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TRANSFORM_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TRANSFORM_2D_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_FADE_THRESHOLD_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MAX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MIN_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_CLAMP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FACTOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PREVIOUS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMARY_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVES_GENERATED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_MATRIX_STACK_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_PIPELINE_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_POINT_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROVOKING_VERTEX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_HISTOGRAM_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_R11F_G11F_B10F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTERIZER_DISCARD_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTER_FIXED_SAMPLE_LOCATIONS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTER_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTER_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_FRAMEBUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_FRAMEBUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_REDUCE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_REFLECTION_MAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_ALPHA_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_BLUE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_DEPTH_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_GREEN_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_HEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_INTERNAL_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_RED_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_STENCIL_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_WIDTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RESCALE_NORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10_A2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB10_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5_A1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB9_E5_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA12_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA32UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8I_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8UI_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_INTEGER_MODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_SIGNED_COMPONENTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_SCALE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_1D_ARRAY_SHADOW_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_2D_ARRAY_SHADOW_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_CUBE_SHADOW_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_MASK_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_INVERT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_VALUE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_PATTERN_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALAR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALED_RESOLVE_FASTEST_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALED_RESOLVE_NICEST_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARABLE_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_ATTRIBS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_SPECULAR_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADOW_ATTENUATION_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SHARED_TEXTURE_PALETTE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SINGLE_COLOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SKIP_DECODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE8_ALPHA8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_RGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8_ALPHA8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_ALPHA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_ATTACHMENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_CLEAR_TAG_VALUE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX16_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_INDEX8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_TAG_BITS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_TEST_TWO_SIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_SYNC_X11_FENCE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_IUI_N3F_V2F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_IUI_N3F_V3F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_IUI_V2F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_IUI_V3F_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TABLE_TOO_LARGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TANGENT_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TANGENT_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TANGENT_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TANGENT_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_3D_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_APPLICATION_MODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_CUBE_MAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BUFFER_FORMAT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FETCH_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FILTER_CONTROL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INDEX_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTENSITY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LOD_BIAS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LUMINANCE_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MATERIAL_FACE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MATERIAL_PARAMETER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_ANISOTROPY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_NORMAL_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_PRIORITY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RESIDENT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SHARED_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SRGB_DECODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STENCIL_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_A_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_B_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_G_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_RGBA_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SWIZZLE_R_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_TOO_LARGE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_UPDATE_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_R_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TIME_ELAPSED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYINGS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_PROGRAM_MATRIX_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLES_ADJACENCY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_STRIP_ADJACENCY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_CMYK_HINT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_IMAGE_HEIGHT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_IMAGES_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_BYTE_3_3_2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_10F_11F_11F_REV_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_10_10_10_2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_24_8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_5_9_9_9_REV_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_8_8_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_CUBE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_1D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_3D_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_CUBE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC2_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC3_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_VEC4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_4_4_4_4_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_5_5_5_1_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_DATATYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIANT_VALUE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VECTOR_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_COUNT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_OBJECT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_BINDING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_INSTRUCTIONS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_INVARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_LOCALS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_OPTIMIZED_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER_VARIANTS_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHTING_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHT_ARRAY_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_W_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_X_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_Y_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO_EXT);
	duk_gl_push_opengl_constant_property(ctx, GL_Z_EXT);
#endif /* DUK_GL_EXT */

#ifdef DUK_GL_HP
	duk_gl_push_opengl_constant_property(ctx, GL_AVERAGE_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_BORDER_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_COLOR_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_CUBIC_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IGNORE_BORDER_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_CUBIC_WEIGHT_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_MAG_FILTER_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_MIN_FILTER_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_ROTATE_ANGLE_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_ROTATE_ORIGIN_X_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_ROTATE_ORIGIN_Y_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_SCALE_X_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_SCALE_Y_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_TRANSFORM_2D_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_TRANSLATE_X_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_IMAGE_TRANSLATE_Y_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_OCCLUSION_TEST_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_OCCLUSION_TEST_RESULT_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLICATE_BORDER_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LIGHTING_MODE_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_POST_SPECULAR_HP);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_PRE_SPECULAR_HP);
#endif /* DUK_GL_HP */

#ifdef DUK_GL_IBM
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_STATIC_DATA_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_VERTEX_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRRORED_REPEAT_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTER_POSITION_UNCLIPPED_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_VERTEX_ARRAY_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_LIST_IBM);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_LIST_STRIDE_IBM);
#endif /* DUK_GL_IBM */

#ifdef DUK_GL_INTEL
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_LAYOUT_DEFAULT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_LAYOUT_LINEAR_CPU_CACHED_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_LAYOUT_LINEAR_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PARALLEL_ARRAYS_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DATA_BOOL32_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DATA_DOUBLE_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DATA_FLOAT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DATA_UINT32_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DATA_UINT64_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DESC_LENGTH_MAX_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DURATION_NORM_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_DURATION_RAW_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_EVENT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_NAME_LENGTH_MAX_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_RAW_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_THROUGHPUT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_COUNTER_TIMESTAMP_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_DONOT_FLUSH_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_FLUSH_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_GLOBAL_CONTEXT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_GPA_EXTENDED_COUNTERS_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_QUERY_NAME_LENGTH_MAX_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_SINGLE_CONTEXT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFQUERY_WAIT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MEMORY_LAYOUT_INTEL);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL);
#endif /* DUK_GL_INTEL */

#ifdef DUK_GL_MESA
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_DATA_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_FUNC_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_CALLBACK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_POSITION_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_INVERT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ALL_BITS_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ARRAYS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_ERRORS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_MASK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_NAME_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_OPERATIONS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_PIXELS_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_PRIMITIVES_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACE_TEXTURES_BIT_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_24_8_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_24_REV_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_15_1_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_1_15_REV_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_8_8_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_8_8_REV_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_DATA_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_FUNC_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_CALLBACK_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_POSITION_MESA);
	duk_gl_push_opengl_constant_property(ctx, GL_YCBCR_MESA);
#endif /* DUK_GL_MESA */

#ifdef DUK_GL_NV
	duk_gl_push_opengl_constant_property(ctx, GL_2_BYTES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_3_BYTES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_4_BYTES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_ADJACENT_PAIRS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_VARYINGS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ACTIVE_VARYING_MAX_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ADJACENT_PAIRS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_AFFINE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_AFFINE_3D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_COMPLETED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_REF_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIBUTE_ADDRESS_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_ARRAY_POINTER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_ARRAY_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_ARRAY_STRIDE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_ARRAY_TYPE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_PRIMARY_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_SECONDARY_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BEVEL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BIAS_BY_NEGATIVE_ONE_HALF_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_ADVANCED_COHERENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_OVERLAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_PREMULTIPLIED_SRC_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BOLD_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BOUNDING_BOX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_GPU_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CIRCULAR_CCW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CIRCULAR_CW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CIRCULAR_TANGENT_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DISTANCE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CLOSE_PATH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLORBURN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLORDODGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_SUM_CLAMP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER5_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER6_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER7_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_AB_DOT_PRODUCT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_AB_OUTPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_CD_DOT_PRODUCT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_CD_OUTPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_COMPONENT_USAGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_INPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_MAPPING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_MUX_SUM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINER_SUM_OUTPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONFORMANT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONJOINT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSERVATIVE_RASTERIZATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSERVATIVE_RASTER_DILATE_GRANULARITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSERVATIVE_RASTER_DILATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSERVATIVE_RASTER_DILATE_RANGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONST_EYE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTRAST_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVEX_HULL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COORD_REPLACE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COUNT_DOWN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COUNT_UP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COVERAGE_MODULATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COVERAGE_MODULATION_TABLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_COVERAGE_MODULATION_TABLE_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FRAGMENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_MODES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_ATTRIB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_MATRIX_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_OCCLUSION_QUERY_ID_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_TIME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DARKEN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPENDENT_AR_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPENDENT_GB_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPENDENT_HILO_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPENDENT_RGB_TEXTURE_3D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH32F_STENCIL8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BUFFER_FLOAT_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLAMP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32F_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_TO_BGRA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_STENCIL_TO_RGBA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DIFFERENCE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DISCARD_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DISJOINT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_DEPTH_REPLACE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_PASS_THROUGH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_TEXTURE_1D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_TEXTURE_3D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ARRAYS_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ARRAYS_INSTANCED_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ARRAYS_STRIP_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ELEMENTS_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ELEMENTS_INSTANCED_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_ELEMENTS_STRIP_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_INDIRECT_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_INDIRECT_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_INDIRECT_UNIFIED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT8_MAG8_INTENSITY8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT8_MAG8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT_MAG_INTENSITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT_MAG_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT_MAG_VIB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DSDT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_ATOP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_IN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_OUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_OVER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DS_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DS_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DT_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DT_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DUP_FIRST_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_DUP_LAST_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ADDRESS_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_UNIFIED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EMBOSS_CONSTANT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EMBOSS_LIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EMBOSS_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_FRACTIONAL_TESSELLATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_TRIANGULAR_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB10_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB11_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB12_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB13_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB14_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB15_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB5_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB6_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB7_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_VERTEX_ATTRIB9_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EXCLUSION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EXPAND_NEGATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EXPAND_NORMAL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_LINEAR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_PLANE_ABSOLUTE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_RADIAL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_E_TIMES_F_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FAILURE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FENCE_CONDITION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FENCE_STATUS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FIELDS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FIELD_LOWER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FIELD_UPPER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FILE_NAME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FILL_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FIRST_TO_REST_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT16_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT16_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT16_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_CLEAR_COLOR_VALUE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_R16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_R32_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RG16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RG32_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGB16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGB32_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGBA16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGBA32_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGBA_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGBA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RGB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_RG_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT_R_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_DISTANCE_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_ASCENDER_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_DESCENDER_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_GLYPHS_AVAILABLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_HAS_KERNING_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_HEIGHT_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_NUM_GLYPH_INDICES_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_TARGET_UNAVAILABLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_UNAVAILABLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_UNDERLINE_POSITION_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_UNDERLINE_THICKNESS_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_UNINTELLIGIBLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_UNITS_PER_EM_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_X_MAX_BOUNDS_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_X_MIN_BOUNDS_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_Y_MAX_BOUNDS_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FONT_Y_MIN_BOUNDS_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FORCE_BLUE_TO_ONE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COVERAGE_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COVERAGE_TO_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_INPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_FACE_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GENERIC_ATTRIB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_HAS_KERNING_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_HEIGHT_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_VERTICAL_BEARING_X_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GLYPH_WIDTH_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_BIAS_NEGATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_BIAS_NORMAL_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_FLOAT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HARDLIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HARDMIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HILO16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HILO8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HILO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HI_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HI_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HORIZONTAL_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_HUE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_LUMINOSITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_SATURATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_IDENTITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT16_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT16_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT16_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT64_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT8_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT8_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT8_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLEAVED_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_RENDERBUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERSE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERSE_TRANSPOSE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERT_OVG_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERT_RGB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ITALIC_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LARGE_CCW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LARGE_CW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LAST_VIDEO_CAPTURE_STATUS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LAYER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTEN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEARBURN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEARDODGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEARLIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LO_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_LO_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAGNITUDE_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAGNITUDE_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB0_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB10_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB11_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB12_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB13_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB14_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB15_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB1_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB2_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB3_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB4_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB5_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB6_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB7_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB8_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_ATTRIB9_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB0_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB10_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB11_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB12_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB13_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB14_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB15_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB1_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB2_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB3_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB4_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB5_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB6_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB7_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB8_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_ATTRIB9_4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_ATTRIB_U_ORDER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_ATTRIB_V_ORDER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_TESSELLATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX5_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX6_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX7_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEEP_3D_TEXTURE_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEEP_3D_TEXTURE_WIDTH_HEIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GENERAL_COMBINERS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_MAP_TESSELLATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_CALL_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_GENERIC_RESULTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_IF_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_LOOP_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_LOOP_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_OUTPUT_VERTICES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_PATCH_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_RESULT_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_SUBROUTINE_NUM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_SUBROUTINE_PARAMETERS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXEL_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RATIONAL_EVAL_ORDER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_RECTANGLE_TEXTURE_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SAMPLE_MASK_WORDS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SHADER_BUFFER_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SHININESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPOT_EXPONENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SUBPIXEL_PRECISION_BIAS_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_COORDS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_IMAGE_UNITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRACK_MATRICES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MINUS_CLAMPED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MINUS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXEL_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MITER_REVERT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MITER_TRUNCATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MIXED_DEPTH_SAMPLES_SUPPORTED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MIXED_STENCIL_SAMPLES_SUPPORTED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW_PROJECTION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MOVE_TO_CONTINUES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MOVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MOVE_TO_RESETS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTIPLY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_COVERAGE_MODES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_FILTER_HINT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NEXT_VIDEO_CAPTURE_BUFFER_STATUS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NOP_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_FILL_STREAMS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_GENERAL_COMBINERS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_VIDEO_CAPTURE_STREAMS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_LINEAR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_HILO_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_2D_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_2D_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_2D_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OFFSET_TEXTURE_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND3_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND3_RGB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_OVERLAY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PARTIAL_SUCCESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PASS_THROUGH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_CLIENT_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_COMMAND_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_COMPUTED_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_COORD_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_COVER_DEPTH_FUNC_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_DASH_ARRAY_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_DASH_CAPS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_DASH_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_DASH_OFFSET_RESET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_END_CAPS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_ERROR_POSITION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FILL_BOUNDING_BOX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FILL_COVER_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FILL_MASK_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FILL_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FOG_GEN_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FORMAT_PS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_FORMAT_SVG_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_GEN_COEFF_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_GEN_COLOR_FORMAT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_GEN_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_GEN_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_INITIAL_DASH_CAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_INITIAL_END_CAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_JOIN_STYLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MAX_MODELVIEW_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MAX_PROJECTION_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MITER_LIMIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MODELVIEW_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MODELVIEW_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_MODELVIEW_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_OBJECT_BOUNDING_BOX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_PROJECTION_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_PROJECTION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_PROJECTION_STACK_DEPTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STENCIL_FUNC_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STENCIL_REF_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STENCIL_VALUE_MASK_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STROKE_BOUNDING_BOX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STROKE_COVER_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STROKE_MASK_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_STROKE_WIDTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_TERMINAL_DASH_CAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_TERMINAL_END_CAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_TRANSPOSE_MODELVIEW_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PATH_TRANSPOSE_PROJECTION_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PER_STAGE_CONSTANTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PINLIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_COUNTER_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_COUNT_AVAILABLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PLUS_CLAMPED_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PLUS_CLAMPED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PLUS_DARKER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PLUS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SPRITE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SPRITE_R_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRESENT_DURATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRESENT_TIME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PREVIOUS_TEXTURE_INPUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMARY_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVES_GENERATED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_ID_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART_INDEX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMITIVE_RESTART_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAMMABLE_SAMPLE_LOCATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ATTRIB_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ERROR_POSITION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_ERROR_STRING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_PARAMETER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_RESIDENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_RESULT_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_STRING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROGRAM_TARGET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADRATIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_NO_WAIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_WAIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_NO_WAIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_WAIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RASTERIZER_DISCARD_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXEL_DATA_RANGE_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXEL_DATA_RANGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_PIXEL_DATA_RANGE_POINTER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RECT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_REFLECTION_MAP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_REGISTER_COMBINERS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_CONIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_HORIZONTAL_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_LARGE_CCW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_LARGE_CW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_MOVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_QUADRATIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_RECT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_ROUNDED_RECT2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_ROUNDED_RECT4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_ROUNDED_RECT8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_ROUNDED_RECT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_SMALL_CCW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_SMALL_CW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RELATIVE_VERTICAL_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_COLOR_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERBUFFER_COVERAGE_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RESTART_PATH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ROUNDED_RECT2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ROUNDED_RECT4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ROUNDED_RECT8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ROUNDED_RECT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_ROUND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_RENDERBUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_LOCATION_SUBPIXEL_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_VALUE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_POSITION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALE_BY_FOUR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALE_BY_ONE_HALF_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALE_BY_TWO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SCREEN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_CONSISTENT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADER_OPERATION_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SHARED_EDGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_ALPHA8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_HILO16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_HILO8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_HILO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_IDENTITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_INTENSITY8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_INTENSITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_LUMINANCE8_ALPHA8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_LUMINANCE8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_LUMINANCE_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_LUMINANCE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_NEGATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGB8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGBA8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGBA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SIGNED_RGB_UNSIGNED_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SKIP_MISSING_GLYPH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SMALL_CCW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SMALL_CW_ARC_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_CUBIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_QUADRATIC_CURVE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SM_COUNT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SOFTLIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE3_ALPHA_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE3_RGB_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARE0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARE0_PLUS_SECONDARY_COLOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SPARE1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SQUARE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ATOP_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_IN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_OUT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_OVER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_STANDARD_FONT_FORMAT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_STANDARD_FONT_NAME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_REF_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBPIXEL_PRECISION_BIAS_X_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBPIXEL_PRECISION_BIAS_Y_BITS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SUCCESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SUPERSAMPLE_SCALE_X_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SUPERSAMPLE_SCALE_Y_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SURFACE_MAPPED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SURFACE_REGISTERED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SURFACE_STATE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_SYSTEM_FONT_NAME_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TERMINATE_SEQUENCE_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_RENDERBUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER_VALUES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COLOR_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COVERAGE_SAMPLES_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DS_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DT_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FLOAT_COMPONENTS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_HI_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LO_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAG_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RECTANGLE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RENDERBUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_SHADER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_UNSIGNED_REMAP_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACK_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRACK_MATRIX_TRANSFORM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_ATTRIBS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_MODE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_BUFFER_START_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_RECORD_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_FEEDBACK_VARYINGS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSLATE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSLATE_3D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSLATE_X_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSLATE_Y_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_AFFINE_2D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_AFFINE_3D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGULAR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNCORRELATED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_ADDRESS_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNIFORM_BUFFER_UNIFIED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_IDENTITY_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT16_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT16_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT16_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT8_VEC2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT8_VEC3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT8_VEC4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_24_8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_8_8_S8_S8_REV_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_S8_S8_8_8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INVERT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_USE_MISSING_GLYPH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UTF16_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_UTF8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_A_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_B_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_C_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_D_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_E_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_F_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VARIABLE_G_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_POINTER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_VALID_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY0_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY10_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY11_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY12_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY13_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY14_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY15_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY1_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY2_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY3_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY4_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY5_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY6_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY7_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY8_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY9_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ID_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_POINT_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_TWO_SIDE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_STATE_PROGRAM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTICAL_LINE_TO_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIBRANCE_BIAS_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIBRANCE_SCALE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_BUFFER_BINDING_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_BUFFER_INTERNAL_FORMAT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_BUFFER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_BUFFER_PITCH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_FIELD_LOWER_HEIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_FIELD_UPPER_HEIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_FRAME_HEIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_FRAME_WIDTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_SURFACE_ORIGIN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_CAPTURE_TO_422_SUPPORTED_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_COLOR_CONVERSION_MATRIX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_COLOR_CONVERSION_MAX_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_COLOR_CONVERSION_MIN_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIDEO_COLOR_CONVERSION_OFFSET_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_COMMAND_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_VIVIDLIGHT_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WARPS_PER_SM_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WARP_SIZE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_DISCARD_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_PIXEL_DATA_RANGE_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_XOR_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_YCBAYCR8A_4224_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_YCBYCR8_422_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_Z4Y12Z4CB12Z4A12Z4Y12Z4CR12Z4A12_4224_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_Z4Y12Z4CB12Z4CR12_444_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_Z4Y12Z4CB12Z4Y12Z4CR12_422_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_Z6Y10Z6CB10Z6A10Z6Y10Z6CR10Z6A10_4224_NV);
	duk_gl_push_opengl_constant_property(ctx, GL_Z6Y10Z6CB10Z6Y10Z6CR10_422_NV);
#endif /* DUK_GL_NV */

#ifdef DUK_GL_OPENGL_1_1
	duk_gl_push_opengl_constant_property(ctx, GL_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_2_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_3D_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_3D_COLOR_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_3_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_4D_COLOR_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_4_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_ALPHA_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_BLUE_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_GREEN_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ACCUM_RED_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_ATTRIB_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALL_CLIENT_ATTRIB_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA12);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_TEST_REF);
	duk_gl_push_opengl_constant_property(ctx, GL_ALWAYS);
	duk_gl_push_opengl_constant_property(ctx, GL_AMBIENT);
	duk_gl_push_opengl_constant_property(ctx, GL_AMBIENT_AND_DIFFUSE);
	duk_gl_push_opengl_constant_property(ctx, GL_AND);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_AND_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_AUTO_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX0);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX1);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX2);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX3);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_BITMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_BITMAP_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_BYTE);
	duk_gl_push_opengl_constant_property(ctx, GL_C3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4UB_V2F);
	duk_gl_push_opengl_constant_property(ctx, GL_C4UB_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_CCW);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP);
	duk_gl_push_opengl_constant_property(ctx, GL_CLEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ALL_ATTRIB_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_PIXEL_STORE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_VERTEX_ARRAY_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE0);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE1);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE2);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE3);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE4);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_PLANE5);
	duk_gl_push_opengl_constant_property(ctx, GL_COEFF);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_INDEXES);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATERIAL_PARAMETER);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPILE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPILE_AND_EXECUTE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_COPY_PIXEL_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_CULL_FACE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_DISTANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_POSITION);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_POSITION_VALID);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_TEXTURE_COORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_TEXTURE_COORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_CW);
	duk_gl_push_opengl_constant_property(ctx, GL_DECAL);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_BUFFER_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_DIFFUSE);
	duk_gl_push_opengl_constant_property(ctx, GL_DITHER);
	duk_gl_push_opengl_constant_property(ctx, GL_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_DONT_CARE);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLE);
	duk_gl_push_opengl_constant_property(ctx, GL_DOUBLEBUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_PIXEL_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_EMISSION);
	duk_gl_push_opengl_constant_property(ctx, GL_ENABLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_EQUIV);
	duk_gl_push_opengl_constant_property(ctx, GL_EVAL_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_EXP);
	duk_gl_push_opengl_constant_property(ctx, GL_EXP2);
	duk_gl_push_opengl_constant_property(ctx, GL_EXTENSIONS);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_PLANE);
	duk_gl_push_opengl_constant_property(ctx, GL_FALSE);
	duk_gl_push_opengl_constant_property(ctx, GL_FASTEST);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_FEEDBACK_BUFFER_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_FLAT);
	duk_gl_push_opengl_constant_property(ctx, GL_FLOAT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_DENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_END);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_START);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_AND_BACK);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_FACE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_FRONT_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_GEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_GREATER);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_HINT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_CLEAR_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_SHIFT);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_WRITEMASK);
	duk_gl_push_opengl_constant_property(ctx, GL_INT);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY12);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY4);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_ENUM);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_OPERATION);
	duk_gl_push_opengl_constant_property(ctx, GL_INVALID_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_KEEP);
	duk_gl_push_opengl_constant_property(ctx, GL_LEFT);
	duk_gl_push_opengl_constant_property(ctx, GL_LEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_LESS);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT0);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT1);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT2);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT3);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT4);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT5);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT6);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT7);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTING);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTING_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_AMBIENT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_LOCAL_VIEWER);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_TWO_SIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_LINES);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_LOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_RESET_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE_PATTERN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STIPPLE_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_BASE);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_LOAD);
	duk_gl_push_opengl_constant_property(ctx, GL_LOGIC_OP);
	duk_gl_push_opengl_constant_property(ctx, GL_LOGIC_OP_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA12);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE12_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_ALPHA16);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE4_ALPHA4);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE6_ALPHA2);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_COLOR_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_GRID_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_GRID_SEGMENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_1);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_2);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_TEXTURE_COORD_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP1_VERTEX_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_COLOR_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_GRID_DOMAIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_GRID_SEGMENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_INDEX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_1);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_2);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_TEXTURE_COORD_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_3);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP2_VERTEX_4);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_MAP_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_MATRIX_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIP_PLANES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_EVAL_ORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LIGHTS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_LIST_NESTING);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_MODELVIEW_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_NAME_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PIXEL_MAP_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_PROJECTION_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VIEWPORT_DIMS);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MODELVIEW_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MODULATE);
	duk_gl_push_opengl_constant_property(ctx, GL_MULT);
	duk_gl_push_opengl_constant_property(ctx, GL_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_NAME_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_NAND);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_MIPMAP_NEAREST);
	duk_gl_push_opengl_constant_property(ctx, GL_NEVER);
	duk_gl_push_opengl_constant_property(ctx, GL_NICEST);
	duk_gl_push_opengl_constant_property(ctx, GL_NONE);
	duk_gl_push_opengl_constant_property(ctx, GL_NOOP);
	duk_gl_push_opengl_constant_property(ctx, GL_NOR);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMALIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_NOTEQUAL);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_ERROR);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_LINEAR);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_PLANE);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_DST_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_OR);
	duk_gl_push_opengl_constant_property(ctx, GL_ORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_OR_REVERSE);
	duk_gl_push_opengl_constant_property(ctx, GL_OUT_OF_MEMORY);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ALIGNMENT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_LSB_FIRST);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ROW_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_PIXELS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_ROWS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SWAP_BYTES);
	duk_gl_push_opengl_constant_property(ctx, GL_PASS_THROUGH_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_PERSPECTIVE_CORRECTION_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_A_TO_A);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_A_TO_A_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_B_TO_B);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_B_TO_B_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_G_TO_G);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_G_TO_G_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_A);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_A_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_B);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_B_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_G);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_G_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_I);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_I_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_R);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_I_TO_R_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_R_TO_R);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_R_TO_R_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_S_TO_S);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MAP_S_TO_S_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_MODE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINTS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FACTOR);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_FILL);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_LINE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_POINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_OFFSET_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_SMOOTH_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_STIPPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_STIPPLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_POLYGON_TOKEN);
	duk_gl_push_opengl_constant_property(ctx, GL_POSITION);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROJECTION_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_Q);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADRATIC_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_QUADS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_STRIP);
	duk_gl_push_opengl_constant_property(ctx, GL_R);
	duk_gl_push_opengl_constant_property(ctx, GL_R3_G3_B2);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_RED);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDER);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDERER);
	duk_gl_push_opengl_constant_property(ctx, GL_RENDER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE);
	duk_gl_push_opengl_constant_property(ctx, GL_RETURN);
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
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_RIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_S);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_BOX);
	duk_gl_push_opengl_constant_property(ctx, GL_SCISSOR_TEST);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECT);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECTION_BUFFER_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_SELECTION_BUFFER_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_SET);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADE_MODEL);
	duk_gl_push_opengl_constant_property(ctx, GL_SHININESS);
	duk_gl_push_opengl_constant_property(ctx, GL_SHORT);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH);
	duk_gl_push_opengl_constant_property(ctx, GL_SPECULAR);
	duk_gl_push_opengl_constant_property(ctx, GL_SPHERE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_CUTOFF);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_DIRECTION);
	duk_gl_push_opengl_constant_property(ctx, GL_SPOT_EXPONENT);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_ALPHA_SATURATE);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_OVERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_STACK_UNDERFLOW);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BITS);
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
	duk_gl_push_opengl_constant_property(ctx, GL_T);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C4F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_C4UB_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_N3F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T2F_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_T4F_C4F_N3F_V4F);
	duk_gl_push_opengl_constant_property(ctx, GL_T4F_V4F);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BINDING_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_Q);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_R);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_S);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEN_T);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTENSITY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTERNAL_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAG_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_FILTER);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_PRIORITY);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RESIDENT);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_S);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_T);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_BIT);
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
	duk_gl_push_opengl_constant_property(ctx, GL_V2F);
	duk_gl_push_opengl_constant_property(ctx, GL_V3F);
	duk_gl_push_opengl_constant_property(ctx, GL_VENDOR);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_1);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_2);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_3);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT);
	duk_gl_push_opengl_constant_property(ctx, GL_VIEWPORT_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_XOR);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO);
	duk_gl_push_opengl_constant_property(ctx, GL_ZOOM_X);
	duk_gl_push_opengl_constant_property(ctx, GL_ZOOM_Y);
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
	duk_gl_push_opengl_constant_property(ctx, GL_ALIASED_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALIASED_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_EDGE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_INTENSITY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_REVERSE_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_COLOR_CONTROL);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_3D_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_INDICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ELEMENTS_VERTICES);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_IMAGE_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_IMAGES);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_3D);
	duk_gl_push_opengl_constant_property(ctx, GL_REDUCE);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLICATE_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_RESCALE_NORMAL);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARABLE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARATE_SPECULAR_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_SINGLE_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_LINE_WIDTH_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_GRANULARITY);
	duk_gl_push_opengl_constant_property(ctx, GL_SMOOTH_POINT_SIZE_RANGE);
	duk_gl_push_opengl_constant_property(ctx, GL_TABLE_TOO_LARGE);
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
	duk_gl_push_opengl_constant_property(ctx, GL_ADD_SIGNED);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIENT_ACTIVE_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMBINE_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_INTENSITY);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_LUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_DOT3_RGBA);
	duk_gl_push_opengl_constant_property(ctx, GL_GLEXT_VERSION);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERPOLATE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CUBE_MAP_TEXTURE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_NUM_COMPRESSED_TEXTURE_FORMATS);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND0_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_OPERAND2_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_PREVIOUS);
	duk_gl_push_opengl_constant_property(ctx, GL_PRIMARY_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_CUBE_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_REFLECTION_MAP);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_INVERT);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_COVERAGE_VALUE);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE0_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SOURCE2_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBTRACT);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_COLOR_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_MODELVIEW_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_PROJECTION_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSPOSE_TEXTURE_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_3);
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_DST_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_SRC_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_SUM);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPARE_R_TO_TEXTURE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_FOG_COORDINATE);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_SECONDARY_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_DECR_WRAP);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT16);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT24);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_TEXTURE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_SOURCE);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_ADD);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_REVERSE_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_FUNC_SUBTRACT);
	duk_gl_push_opengl_constant_property(ctx, GL_GENERATE_MIPMAP);
	duk_gl_push_opengl_constant_property(ctx, GL_GENERATE_MIPMAP_HINT);
	duk_gl_push_opengl_constant_property(ctx, GL_INCR_WRAP);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_LOD_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN);
	duk_gl_push_opengl_constant_property(ctx, GL_MIRRORED_REPEAT);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_ONE_MINUS_CONSTANT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_DISTANCE_ATTENUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_FADE_THRESHOLD_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MAX);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MIN);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_FUNC);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEPTH_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FILTER_CONTROL);
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
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_FOG_COORD);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_QUERY);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_DYNAMIC_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGE_FLAG_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_POINTER);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_STRIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_ARRAY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_COORD_SRC);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_COUNTER_BITS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_AVAILABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_ONLY);
	duk_gl_push_opengl_constant_property(ctx, GL_READ_WRITE);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_PASSED);
	duk_gl_push_opengl_constant_property(ctx, GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC0_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC0_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC1_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC1_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC2_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRC2_RGB);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_STATIC_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_COPY);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_DRAW);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_READ);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_1_5);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_WEIGHT_ARRAY_BUFFER_BINDING);
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
	duk_gl_push_opengl_constant_property(ctx, GL_COORD_REPLACE);
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
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_COORDS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VARYING_FLOATS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_ATTRIBS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SPRITE);
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
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PROGRAM_TWO_SIDE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_SHADER);
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_2_1
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SLUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SLUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_SECONDARY_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_PACK_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_UNPACK_BUFFER_BINDING);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE8);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE8_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_SLUMINANCE_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB8_ALPHA8);
	duk_gl_push_opengl_constant_property(ctx, GL_SRGB_ALPHA);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_2_1);
#endif /* DUK_GL_OPENGL_2_1 */

#ifdef DUK_GL_OPENGL_3_0
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BGRA_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BGR_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_INTEGER);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_ACCESS_FLAGS);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_LENGTH);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_MAP_OFFSET);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_FRAGMENT_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_READ_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_VERTEX_COLOR);
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
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX);
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
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_INTENSITY_TYPE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LUMINANCE_TYPE);
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
	duk_gl_push_opengl_constant_property(ctx, GL_DISPLAY_LIST);
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
	duk_gl_push_opengl_constant_property(ctx, GL_1PASS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_2PASS_0_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_2PASS_1_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_0_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_1_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_2_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_4PASS_3_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_ALLOW_DRAW_FRG_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALLOW_DRAW_MEM_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALLOW_DRAW_OBJ_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALLOW_DRAW_WIN_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_MAX_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_MIN_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_ALWAYS_FAST_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ALWAYS_SOFT_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_AUX_DEPTH_STENCIL_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_BACK_NORMALS_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_ADVANCED_COHERENT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_MAX_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLUE_MIN_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_FLUSHING_UNMAP_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_OBJECT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_BUFFER_SERIALIZED_MODIFY_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_BORDER_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_CLAMP_TO_EDGE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DEPTH_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_FAR_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_NEAR_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_ORIGIN);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR3_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR4_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLORBURN_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLORDODGE_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_FLOAT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_INTENSITY_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_FXT1_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_FXT1_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSERVE_MEMORY_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_CONSTANT_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_LOST);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_ROBUST_ACCESS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTINUOUS_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_1D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_BORDER_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FILTER_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_COUNTER_RANGE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_COUNTER_TYPE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DARKEN_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_DATA_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_API_ERROR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_APPLICATION_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_DEPRECATION_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_OTHER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_PERFORMANCE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_LOGGED_MESSAGES_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_HIGH_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_LOW_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEBUG_SEVERITY_MEDIUM_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLAMP_FAR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_CLAMP_NEAR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DETAIL_TEXTURE_2D_BINDING_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DETAIL_TEXTURE_2D_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DETAIL_TEXTURE_LEVEL_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DETAIL_TEXTURE_MODE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DIFFERENCE_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_DISCRETE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_DISTANCE_ATTENUATION_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DRAW_PIXELS_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_ALPHA12_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_ALPHA16_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_ALPHA4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_ALPHA8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_INTENSITY12_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_INTENSITY16_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_INTENSITY4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_INTENSITY8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE12_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE16_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE_ALPHA4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_LUMINANCE_ALPHA8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_DUAL_TEXTURE_SELECT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_EDGEFLAG_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_POINTER_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_ELEMENT_ARRAY_TYPE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_EXCLUSION_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_DISTANCE_TO_LINE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_DISTANCE_TO_POINT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_LINE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_EYE_POINT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_FACTOR_MAX_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_FACTOR_MIN_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_FENCE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_FILTER4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_FIXED_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_FUNC_POINTS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_FUNC_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_FORMAT_SUBSAMPLE_244_244_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_FORMAT_SUBSAMPLE_24_24_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR);
	duk_gl_push_opengl_constant_property(ctx, GL_FULL_STIPPLE_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_GENERATE_MIPMAP_HINT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_GENERATE_MIPMAP_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
	duk_gl_push_opengl_constant_property(ctx, GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_MAX_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_GREEN_MIN_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_GUILTY_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_HALF_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_HARDLIGHT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_ALPHA_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_BLUE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_GREEN_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_LUMINANCE_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_RED_SIZE);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_HISTOGRAM_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_COLOR_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_HUE_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_LUMINOSITY_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_HSL_SATURATION_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_IMPLEMENTATION_COLOR_READ_TYPE_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_INDEX_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_INNOCENT_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_INTENSITY_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLACE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLACE_READ_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLACE_READ_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_INT_SAMPLER_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_INVERTED_SCREEN_W_REND);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHTEN_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_DETAIL_ALPHA_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_DETAIL_COLOR_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_DETAIL_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_SHARPEN_ALPHA_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_SHARPEN_COLOR_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_SHARPEN_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_LOSE_CONTEXT_ON_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_ALPHA16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE16_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_ALPHA8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE8_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_ALPHA_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_LUMINANCE_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_MATERIAL_SIDE_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_AMBIENT_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_COLOR_INDEXES_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_DIFFUSE_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_EMISSION_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_SHININESS_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAT_SPECULAR_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_4D_TEXTURE_SIZE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_MATRIX_STACK_DEPTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_HEIGHT);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CONVOLUTION_WIDTH);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CULL_DISTANCES);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_LOGGED_MESSAGES_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEBUG_MESSAGE_LENGTH_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FOG_FUNC_POINTS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_3D_TEXTURE_SIZE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_SPARSE_TEXTURE_SIZE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VERTEX_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_VIEWS_OVR);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_FORMAT);
	duk_gl_push_opengl_constant_property(ctx, GL_MINMAX_SINK);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_LOD_WARNING_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MIN_SPARSE_LEVEL_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTIPLY_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_BIT_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_MULTISAMPLE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_NATIVE_GRAPHICS_END_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_NATIVE_GRAPHICS_HANDLE_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_NEGATIVE_ONE_TO_ONE);
	duk_gl_push_opengl_constant_property(ctx, GL_NORMAL_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_NO_RESET_NOTIFICATION);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_DISTANCE_TO_LINE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_DISTANCE_TO_POINT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_LINE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_OBJECT_POINT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_OCCLUSION_QUERY_EVENT_MASK_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_OVERLAY_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_IMAGE_DEPTH_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_RESAMPLE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_ROW_BYTES_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SKIP_VOLUMES_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE4_R5_G6_B5_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE4_RGB5_A1_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE4_RGB8_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE4_RGBA4_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE4_RGBA8_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE8_R5_G6_B5_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE8_RGB5_A1_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE8_RGB8_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE8_RGBA4_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PALETTE8_RGBA8_OES);
	duk_gl_push_opengl_constant_property(ctx, GL_PERCENTAGE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFMON_RESULT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFMON_RESULT_AVAILABLE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFMON_RESULT_SIZE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_PERFORMANCE_MONITOR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_GROUP_COLOR_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TEXTURE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_FADE_THRESHOLD_SIZE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MAX_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_POINT_SIZE_MIN_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_ALPHA_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_BLUE_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_GREEN_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_BIAS);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_RED_SCALE);
	duk_gl_push_opengl_constant_property(ctx, GL_PREFER_DOUBLEBUFFER_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_HISTOGRAM);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_CONVOLUTION_COLOR_TABLE);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_1D_STACK_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_2D_STACK_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_4D_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_PURGEABLE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_ALPHA4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_ALPHA8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_INTENSITY4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_INTENSITY8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_LUMINANCE4_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_LUMINANCE8_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_TEXTURE_SELECT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_ALL_EVENT_BITS_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BUFFER_BINDING_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_NO_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_BY_REGION_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_DEPTH_BOUNDS_FAIL_EVENT_BIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_DEPTH_FAIL_EVENT_BIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_DEPTH_PASS_EVENT_BIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_NO_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_OBJECT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_RESULT_NO_WAIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_STENCIL_FAIL_EVENT_BIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_TARGET);
	duk_gl_push_opengl_constant_property(ctx, GL_QUERY_WAIT_INVERTED);
	duk_gl_push_opengl_constant_property(ctx, GL_RECLAIM_MEMORY_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_REDUCE);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_MAX_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_MIN_CLAMP_INGR);
	duk_gl_push_opengl_constant_property(ctx, GL_RED_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RELEASED_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE_VALUE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLICATE_BORDER);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_AVERAGE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_DECIMATE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_REPLICATE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_ZERO_FILL_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_RESET_NOTIFICATION_STRATEGY);
	duk_gl_push_opengl_constant_property(ctx, GL_RETAINED_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB4_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA4_DXT5_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA4_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_DXT5_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGBA_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_422_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_FLOAT16_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_FLOAT32_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_RAW_422_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_S3TC);
	duk_gl_push_opengl_constant_property(ctx, GL_RGB_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_RG_SNORM);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLER_OBJECT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLES_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_MASK_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_ALPHA_TO_ONE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS_3DFX);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_BUFFERS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_INVERT_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_MASK_VALUE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SAMPLE_PATTERN_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SCREEN_COORDINATES_REND);
	duk_gl_push_opengl_constant_property(ctx, GL_SCREEN_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_SEPARABLE_2D);
	duk_gl_push_opengl_constant_property(ctx, GL_SET_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_SOFTLIGHT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_BACK_OP_VALUE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_STENCIL_OP_VALUE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_STORAGE_CACHED_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_STORAGE_CLIENT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_STORAGE_PRIVATE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_STORAGE_SHARED_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_STREAM_RASTERIZATION_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_STRICT_DEPTHFUNC_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_STRICT_LIGHTING_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_STRICT_SCISSOR_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_SUBSAMPLE_DISTANCE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_TABLE_TOO_LARGE);
	duk_gl_push_opengl_constant_property(ctx, GL_TESSELLATION_FACTOR_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_TESSELLATION_MODE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXCOORD1_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXCOORD2_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXCOORD3_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXCOORD4_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D_STACK_BINDING_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_1D_STACK_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_STACK_BINDING_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_2D_STACK_MESAX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_4DSIZE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_4D_BINDING_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_4D_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_BASE_LEVEL_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COLOR_WRITEMASK_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CONSTANT_DATA_SUNX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_FILTER4_SIZE_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LEVEL_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_LOD_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MIN_LOD_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RANGE_LENGTH_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_RANGE_POINTER_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STORAGE_HINT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_STORAGE_SPARSE_BIT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_TARGET);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_WRAP_Q_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_TRANSFORM_HINT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNDEFINED_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNKNOWN_CONTEXT_RESET);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_CLIENT_STORAGE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_CONSTANT_DATA_SUNX);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_IMAGE_DEPTH_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_RESAMPLE_OML);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_ROW_BYTES_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SKIP_VOLUMES_SGIS);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT64_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_INT_SAMPLER_BUFFER_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_8_8_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_UNSIGNED_SHORT_8_8_REV_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERSION_4_5);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX23_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX4_BIT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_BINDING_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_OBJECT_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_RANGE_POINTER_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ARRAY_STORAGE_HINT_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP1_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP2_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_CONSISTENT_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_DATA_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ELEMENT_SWIZZLE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_ID_SWIZZLE_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_X_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Y_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VIRTUAL_PAGE_SIZE_Z_AMD);
	duk_gl_push_opengl_constant_property(ctx, GL_VOLATILE_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_WIDE_LINE_HINT_PGI);
	duk_gl_push_opengl_constant_property(ctx, GL_YCBCR_422_APPLE);
	duk_gl_push_opengl_constant_property(ctx, GL_ZERO_TO_ONE);
#endif /* DUK_GL_OPENGL_4_5 */

#ifdef DUK_GL_SGI
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_MATRIX_STACK_DEPTH_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_ALPHA_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BIAS_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_BLUE_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_FORMAT_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_GREEN_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_INTENSITY_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_LUMINANCE_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_RED_SIZE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_SCALE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_COLOR_TABLE_WIDTH_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_BIAS_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_COLOR_MATRIX_RED_SCALE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_CONVOLUTION_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_PROXY_TEXTURE_COLOR_TABLE_SGI);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COLOR_TABLE_SGI);
#endif /* DUK_GL_SGI */

#ifdef DUK_GL_SGIX
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_MAX_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ALPHA_MIN_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ASYNC_DRAW_PIXELS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ASYNC_HISTOGRAM_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ASYNC_MARKER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ASYNC_READ_PIXELS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_ASYNC_TEX_IMAGE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_CALLIGRAPHIC_FRAGMENT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_CONVOLUTION_HINT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_CURRENT_RASTER_NORMAL_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_DEFORMATIONS_MASK_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT16_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT24_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_DEPTH_COMPONENT32_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_OFFSET_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_OFFSET_VALUE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_COLOR_MATERIAL_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT0_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT1_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT2_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT3_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT4_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT5_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT6_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT7_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHTING_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEZOOM_FACTOR_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_FRAMEZOOM_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_DEFORMATION_BIT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_GEOMETRY_DEFORMATION_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_INSTRUMENT_BUFFER_POINTER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_INSTRUMENT_MEASUREMENTS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_INTERLACE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_IR_INSTRUMENT1_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_LIGHT_ENV_MODE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_CLIPMAP_LINEAR_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_LINEAR_CLIPMAP_NEAREST_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_LIST_PRIORITY_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ACTIVE_LIGHTS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ASYNC_DRAW_PIXELS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ASYNC_HISTOGRAM_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ASYNC_READ_PIXELS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_ASYNC_TEX_IMAGE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIPMAP_DEPTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_DEFORMATION_ORDER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAGMENT_LIGHTS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_MAX_FRAMEZOOM_FACTOR_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_CLIPMAP_LINEAR_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_NEAREST_CLIPMAP_NEAREST_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_RESAMPLE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PACK_SUBSAMPLE_RATE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_SUBSAMPLE_2424_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_SUBSAMPLE_4242_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_SUBSAMPLE_4444_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TEX_GEN_MODE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TEX_GEN_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_CACHE_INCREMENT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_CACHE_SIZE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_GRID_DEPTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_GRID_HEIGHT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_GRID_WIDTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_HEIGHT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_PIXEL_TILE_WIDTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_TEXTURE_FILTER_BIAS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_POST_TEXTURE_FILTER_SCALE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCE_PLANE_EQUATION_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_REFERENCE_PLANE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_DECIMATE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_REPLICATE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_RESAMPLE_ZERO_FILL_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SCALEBIAS_HINT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SHADOW_AMBIENT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_AXIAL_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_AXIS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_EYE_ALIGNED_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_MODE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_OBJECT_ALIGNED_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_SPRITE_TRANSLATION_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_CENTER_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_DEPTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_FRAME_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_OFFSET_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_OPERATOR_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_COMPARE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEFORMATION_BIT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_DEFORMATION_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_ENV_BIAS_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_GEQUAL_R_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LEQUAL_R_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LOD_BIAS_R_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LOD_BIAS_S_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_LOD_BIAS_T_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_CLAMP_R_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_CLAMP_S_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MAX_CLAMP_T_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_TEXTURE_MULTI_BUFFER_HINT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_RESAMPLE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_UNPACK_SUBSAMPLE_RATE_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PRECLIP_HINT_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_VERTEX_PRECLIP_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_YCRCBA_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_YCRCB_422_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_YCRCB_444_SGIX);
	duk_gl_push_opengl_constant_property(ctx, GL_YCRCB_SGIX);
#endif /* DUK_GL_SGIX */

#ifdef DUK_GL_SUN
	duk_gl_push_opengl_constant_property(ctx, GL_GLOBAL_ALPHA_FACTOR_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_GLOBAL_ALPHA_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_QUAD_MESH_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_C3F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_C4F_N3F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_C4UB_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_N3F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_T2F_C4F_N3F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_T2F_N3F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_T2F_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_R1UI_V3F_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACEMENT_CODE_ARRAY_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACEMENT_CODE_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE_MIDDLE_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_REPLACE_OLDEST_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_RESTART_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_SLICE_ACCUM_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_LIST_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_TRIANGLE_MESH_SUN);
	duk_gl_push_opengl_constant_property(ctx, GL_WRAP_BORDER_SUN);
#endif /* DUK_GL_SUN */

#ifdef DUK_GL_WIN
	duk_gl_push_opengl_constant_property(ctx, GL_FOG_SPECULAR_TEXTURE_WIN);
	duk_gl_push_opengl_constant_property(ctx, GL_PHONG_HINT_WIN);
	duk_gl_push_opengl_constant_property(ctx, GL_PHONG_WIN);
#endif /* DUK_GL_WIN */

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

