/*
 *  Duktape OpenGL 0.5 - OpenGL bindings for Duktape embeddable Javascript engine.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 *  
 *  Duktape OpenGL: https://github.com/mrautio/duktape-opengl/
 *  Duktape: http://duktape.org/
 *  OpenGL API reference: https://www.opengl.org/registry/
 */

#include <duktape.h>

#ifdef __APPLE__
#include <OpenGL/glcorearb.h>
#else
#include <GL/glcorearb.h>
#endif

#define DUK_GL_OPENGL_1_1

/*
 *  Bindings that are not enabled by default.
 */
/*#define DUK_GL_ARB*/

/*
 *  Enable automatically older OpenGL standard major versions, if higher major version is enabled
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
DUK_LOCAL size_t duk_gl_determine_array_length(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, size_t num)
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

#define DUK_GL_ARRAY_GET_FUNCTION(argtypedef1, arg1) \
DUK_LOCAL argtypedef1 *duk_gl_get_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \
{ \
	if (duk_is_array(ctx, obj_index)) \
	{ \
		size_t array_length = duk_gl_determine_array_length(ctx, obj_index, sz, num); \
		unsigned int i = 0; \
		for(i=0; i<array_length; i++) \
		{ \
			duk_get_prop_index(ctx, obj_index, (duk_uarridx_t)i); \
			array[i] = (argtypedef1)duk_get_##arg1(ctx, -1); \
			duk_pop(ctx); \
		} \
		return array; \
	} \
	return NULL; \
}

#define DUK_GL_ARRAY_PUT_FUNCTION(argtypedef1, arg1) \
DUK_LOCAL duk_bool_t duk_gl_put_##argtypedef1##_array(duk_context *ctx, duk_idx_t obj_index, duk_size_t sz, argtypedef1 *array, size_t num) \
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
		return 1; \
	} \
	return 0; \
}

DUK_GL_ARRAY_GET_FUNCTION(GLboolean, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLboolean, number)
DUK_GL_ARRAY_GET_FUNCTION(GLbyte, number)
DUK_GL_ARRAY_GET_FUNCTION(GLubyte, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLubyte, number)
DUK_GL_ARRAY_GET_FUNCTION(GLdouble, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLdouble, number)
DUK_GL_ARRAY_GET_FUNCTION(GLfloat, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLfloat, number)
DUK_GL_ARRAY_GET_FUNCTION(GLclampf, number)
DUK_GL_ARRAY_GET_FUNCTION(GLint, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLint, number)
DUK_GL_ARRAY_GET_FUNCTION(GLuint, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLuint, number)
DUK_GL_ARRAY_GET_FUNCTION(GLshort, number)
DUK_GL_ARRAY_GET_FUNCTION(GLushort, number)
DUK_GL_ARRAY_PUT_FUNCTION(GLushort, number)

/*
 *  Wrapper macros for OpenGL C functions.
 *  Macro name defines the amount of return types and function arguments it supports.
 *  Macros take arguments in following manner (<Function name>, <OpenGL variable type 1>, <Duktape push type 1>, ...N)
 *  - Function name = OpenGL C function name
 *  - OpenGL variable type N = OpenGL's C variable type definition, for example, "GLenum"
 *  - Duktape push type 1 = Duktape API's duk_get_... function's type name, for example, duk_get_number
 */
#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(PREPROCESS, POSTPROCESS, c_function_name) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name(); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name()); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG4(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG6(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG7(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG8(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG9(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG10(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG11(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG11(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG12(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG13(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG13(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG14(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13,  \
		(argtypedef14)arg14 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG14(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13,  \
		(argtypedef14)arg14 \
	)); \
	POSTPROCESS; \
	return 1; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(PREPROCESS, POSTPROCESS, c_function_name, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14, argtypedef15, arg15) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13,  \
		(argtypedef14)arg14,  \
		(argtypedef15)arg15 \
	); \
	POSTPROCESS; \
	return 0; \
}

#define DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG15(PREPROCESS, POSTPROCESS, c_function_name, rettypedef1, argtypedef1, arg1, argtypedef2, arg2, argtypedef3, arg3, argtypedef4, arg4, argtypedef5, arg5, argtypedef6, arg6, argtypedef7, arg7, argtypedef8, arg8, argtypedef9, arg9, argtypedef10, arg10, argtypedef11, arg11, argtypedef12, arg12, argtypedef13, arg13, argtypedef14, arg14, argtypedef15, arg15) \
DUK_LOCAL duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	PREPROCESS; \
	duk_push_##rettypedef1(ctx, c_function_name( \
		(argtypedef1)arg1,  \
		(argtypedef2)arg2,  \
		(argtypedef3)arg3,  \
		(argtypedef4)arg4,  \
		(argtypedef5)arg5,  \
		(argtypedef6)arg6,  \
		(argtypedef7)arg7,  \
		(argtypedef8)arg8,  \
		(argtypedef9)arg9,  \
		(argtypedef10)arg10,  \
		(argtypedef11)arg11,  \
		(argtypedef12)arg12,  \
		(argtypedef13)arg13,  \
		(argtypedef14)arg14,  \
		(argtypedef15)arg15 \
	)); \
	POSTPROCESS; \
	return 1; \
}

/*
 *  OpenGL wrapper function definitions
 */
#ifdef DUK_GL_ARB
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBlendEquationSeparateiARB, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBlendEquationiARB, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glBlendFuncSeparateiARB, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLenum, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBlendFunciARB, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLint var3 [0]; ,/*EMPTY*/,glCompileShaderIncludeARB, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLchar * const *, duk_get_string(ctx, 2), const GLint *, duk_gl_get_GLint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var3 = duk_get_int(ctx, 3);  GLuint var4 [0]; ,/*EMPTY*/,glDebugMessageControlARB, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLsizei, var3, const GLuint *, duk_gl_get_GLuint_array(ctx, 4, 0, var4, 0), GLboolean, duk_get_boolean(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glDebugMessageInsertARB, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLsizei, var4, const GLchar *, duk_get_string(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDeleteNamedStringARB, GLint, duk_get_int(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glDispatchComputeGroupSizeARB, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4), GLuint, duk_get_uint(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG8(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var6 = duk_get_DETERMINE(ctx, 6); GLenum var2 [0]; GLenum var3 [0]; GLuint var4 [0]; GLenum var5 [0]; GLsizei var6 [0]; GLchar var7 [0]; ,duk_gl_put_GLenum_array(ctx, 2, 0, var2, 0); duk_gl_put_GLenum_array(ctx, 3, 0, var3, 0); duk_gl_put_GLuint_array(ctx, 4, 0, var4, 0); duk_gl_put_GLenum_array(ctx, 5, 0, var5, 0); duk_gl_put_GLsizei_array(ctx, 6, 0, var6, 0); duk_gl_put_GLchar_array(ctx, 7, 0, var7, 0); ,glGetDebugMessageLogARB, uint, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 3, 0, var3, 0), GLuint *, duk_gl_get_GLuint_array(ctx, 4, 0, var4, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 5, 0, var5, 0), GLsizei *, duk_gl_get_GLsizei_array(ctx, 6, 0, var6, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 7, 0, var7, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(/*EMPTY*/,/*EMPTY*/,glGetGraphicsResetStatusARB, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG5(/*EMPTY*/,/*EMPTY*/,glGetImageHandleARB, uint, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLint, duk_get_int(ctx, 3), GLenum, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2); GLint var3 [0]; GLchar var4 [0]; ,duk_gl_put_GLint_array(ctx, 3, 0, var3, 0); duk_gl_put_GLchar_array(ctx, 4, 0, var4, 0); ,glGetNamedStringARB, GLint, duk_get_int(ctx, 0), const GLchar *, duk_get_string(ctx, 1), GLsizei, var2, GLint *, duk_gl_get_GLint_array(ctx, 3, 0, var3, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 4, 0, var4, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetNamedStringivARB, GLint, duk_get_int(ctx, 0), const GLchar *, duk_get_string(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glGetTextureHandleARB, uint, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetTextureSamplerHandleARB, uint, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint64EXT var2 [0]; ,duk_gl_put_GLuint64EXT_array(ctx, 2, 0, var2, 0); ,glGetVertexAttribLui64vARB, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint64EXT *, duk_gl_get_GLuint64EXT_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLdouble var3 [1]; ,duk_gl_put_GLdouble_array(ctx, 3, 1, var3, 1); ,glGetnUniformdvARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLfloat var3 [1]; ,duk_gl_put_GLfloat_array(ctx, 3, 1, var3, 1); ,glGetnUniformfvARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetnUniformivARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLuint var3 [1]; ,duk_gl_put_GLuint_array(ctx, 3, 1, var3, 1); ,glGetnUniformuivARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLuint *, duk_gl_get_GLuint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsImageHandleResidentARB, boolean, GLuint64, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glIsNamedStringARB, boolean, GLint, duk_get_int(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsTextureHandleResidentARB, boolean, GLuint64, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMakeImageHandleNonResidentARB, GLuint64, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glMakeImageHandleResidentARB, GLuint64, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMakeTextureHandleNonResidentARB, GLuint64, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMakeTextureHandleResidentARB, GLuint64, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMinSampleShadingARB, GLfloat, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glNamedStringARB, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), const GLchar *, duk_get_string(ctx, 2), GLint, duk_get_int(ctx, 3), const GLchar *, duk_get_string(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramUniformHandleui64ARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint64, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint64 var3 [0]; ,/*EMPTY*/,glProgramUniformHandleui64vARB, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLuint64 *, duk_gl_get_GLuint64_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_int(ctx, 6); GLsizei var7 = duk_get_int(ctx, 7); ,/*EMPTY*/,glTexPageCommitmentARB, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLsizei, var6, GLsizei, var7, GLboolean, duk_get_boolean(ctx, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glUniformHandleui64ARB, GLint, duk_get_int(ctx, 0), GLuint64, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint64 var2 [0]; ,/*EMPTY*/,glUniformHandleui64vARB, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLuint64 *, duk_gl_get_GLuint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribL1ui64ARB, GLuint, duk_get_uint(ctx, 0), GLuint64EXT, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint64EXT var1 [0]; ,/*EMPTY*/,glVertexAttribL1ui64vARB, GLuint, duk_get_uint(ctx, 0), const GLuint64EXT *, duk_gl_get_GLuint64EXT_array(ctx, 1, 0, var1, 0))
#endif /* DUK_GL_ARB */

#ifdef DUK_GL_OPENGL_1_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindTexture, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBlendFunc, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glClear, GLbitfield, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glClearColor, GLfloat, duk_get_number(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glClearDepth, GLdouble, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glClearStencil, GLint, duk_get_int(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glColorMask, GLboolean, duk_get_boolean(ctx, 0), GLboolean, duk_get_boolean(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLboolean, duk_get_boolean(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glCopyTexImage1D, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLint, duk_get_int(ctx, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_int(ctx, 6); ,/*EMPTY*/,glCopyTexImage2D, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLsizei, var6, GLint, duk_get_int(ctx, 7))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glCopyTexSubImage1D, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(GLsizei var6 = duk_get_int(ctx, 6); GLsizei var7 = duk_get_int(ctx, 7); ,/*EMPTY*/,glCopyTexSubImage2D, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLsizei, var6, GLsizei, var7)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glCullFace, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteTextures, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDepthFunc, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDepthMask, GLboolean, duk_get_boolean(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDepthRange, GLdouble, duk_get_number(ctx, 0), GLdouble, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDisable, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var2 = duk_get_int(ctx, 2); ,/*EMPTY*/,glDrawArrays, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDrawBuffer, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glEnable, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glFinish)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glFlush)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glFrontFace, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenTextures, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLboolean var1 [0]; ,duk_gl_put_GLboolean_array(ctx, 1, 0, var1, 0); ,glGetBooleanv, GLenum, duk_get_uint(ctx, 0), GLboolean *, duk_gl_get_GLboolean_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLdouble var1 [0]; ,duk_gl_put_GLdouble_array(ctx, 1, 0, var1, 0); ,glGetDoublev, GLenum, duk_get_uint(ctx, 0), GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(/*EMPTY*/,/*EMPTY*/,glGetError, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLfloat var1 [0]; ,duk_gl_put_GLfloat_array(ctx, 1, 0, var1, 0); ,glGetFloatv, GLenum, duk_get_uint(ctx, 0), GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLint var1 [0]; ,duk_gl_put_GLint_array(ctx, 1, 0, var1, 0); ,glGetIntegerv, GLenum, duk_get_uint(ctx, 0), GLint *, duk_gl_get_GLint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLfloat var3 [1]; ,duk_gl_put_GLfloat_array(ctx, 3, 1, var3, 1); ,glGetTexLevelParameterfv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetTexLevelParameteriv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetTexParameterfv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetTexParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glHint, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsEnabled, boolean, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsTexture, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glLineWidth, GLfloat, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glLogicOp, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPixelStoref, GLenum, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPixelStorei, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glPointSize, GLfloat, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPolygonMode, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPolygonOffset, GLfloat, duk_get_number(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glReadBuffer, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glScissor, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glStencilFunc, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glStencilMask, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glStencilOp, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTexParameterf, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLfloat var2 [1]; ,/*EMPTY*/,glTexParameterfv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTexParameteri, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glTexParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glViewport, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLsizei, var3)
#endif /* DUK_GL_OPENGL_1_1 */

#ifdef DUK_GL_OPENGL_1_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(GLsizei var7 = duk_get_int(ctx, 7); GLsizei var8 = duk_get_int(ctx, 8); ,/*EMPTY*/,glCopyTexSubImage3D, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLint, duk_get_int(ctx, 6), GLsizei, var7, GLsizei, var8)
#endif /* DUK_GL_OPENGL_1_2 */

#ifdef DUK_GL_OPENGL_1_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glActiveTexture, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glSampleCoverage, GLfloat, duk_get_number(ctx, 0), GLboolean, duk_get_boolean(ctx, 1))
#endif /* DUK_GL_OPENGL_1_3 */

#ifdef DUK_GL_OPENGL_1_4
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glBlendColor, GLfloat, duk_get_number(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glBlendEquation, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glBlendFuncSeparate, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3);  GLint var1 [0];  GLsizei var2 [0]; ,/*EMPTY*/,glMultiDrawArrays, GLenum, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 0, var1, 0), const GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPointParameterf, GLenum, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [1]; ,/*EMPTY*/,glPointParameterfv, GLenum, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPointParameteri, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [1]; ,/*EMPTY*/,glPointParameteriv, GLenum, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 1, var1, 1))
#endif /* DUK_GL_OPENGL_1_4 */

#ifdef DUK_GL_OPENGL_1_5
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBeginQuery, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindBuffer, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteBuffers, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteQueries, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glEndQuery, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenBuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenQueries, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetBufferParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetQueryObjectiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetQueryObjectuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetQueryiv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsBuffer, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsQuery, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glUnmapBuffer, boolean, GLenum, duk_get_uint(ctx, 0))
#endif /* DUK_GL_OPENGL_1_5 */

#ifdef DUK_GL_OPENGL_2_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glAttachShader, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBindAttribLocation, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBlendEquationSeparate, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glCompileShader, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(/*EMPTY*/,/*EMPTY*/,glCreateProgram, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glCreateShader, uint, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDeleteProgram, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDeleteShader, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDetachShader, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glDisableVertexAttribArray, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLenum var1 [0]; ,/*EMPTY*/,glDrawBuffers, GLsizei, var0, const GLenum *, duk_gl_get_GLenum_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glEnableVertexAttribArray, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var3 [0]; GLint var4 [0]; GLenum var5 [0]; GLchar var6 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLint_array(ctx, 4, 0, var4, 0); duk_gl_put_GLenum_array(ctx, 5, 0, var5, 0); duk_gl_put_GLchar_array(ctx, 6, 0, var6, 0); ,glGetActiveAttrib, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLint *, duk_gl_get_GLint_array(ctx, 4, 0, var4, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 5, 0, var5, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 6, 0, var6, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var3 [0]; GLint var4 [0]; GLenum var5 [0]; GLchar var6 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLint_array(ctx, 4, 0, var4, 0); duk_gl_put_GLenum_array(ctx, 5, 0, var5, 0); duk_gl_put_GLchar_array(ctx, 6, 0, var6, 0); ,glGetActiveUniform, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLint *, duk_gl_get_GLint_array(ctx, 4, 0, var4, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 5, 0, var5, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 6, 0, var6, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var2 [0]; GLuint var3 [0]; ,duk_gl_put_GLsizei_array(ctx, 2, 0, var2, 0); duk_gl_put_GLuint_array(ctx, 3, 0, var3, 0); ,glGetAttachedShaders, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLuint *, duk_gl_get_GLuint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetAttribLocation, int, GLuint, duk_get_uint(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var2 [0]; GLchar var3 [0]; ,duk_gl_put_GLsizei_array(ctx, 2, 0, var2, 0); duk_gl_put_GLchar_array(ctx, 3, 0, var3, 0); ,glGetProgramInfoLog, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetProgramiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var2 [0]; GLchar var3 [0]; ,duk_gl_put_GLsizei_array(ctx, 2, 0, var2, 0); duk_gl_put_GLchar_array(ctx, 3, 0, var3, 0); ,glGetShaderInfoLog, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var2 [0]; GLchar var3 [0]; ,duk_gl_put_GLsizei_array(ctx, 2, 0, var2, 0); duk_gl_put_GLchar_array(ctx, 3, 0, var3, 0); ,glGetShaderSource, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetShaderiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetUniformLocation, int, GLuint, duk_get_uint(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetUniformfv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetUniformiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLdouble var2 [1]; ,duk_gl_put_GLdouble_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribdv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsProgram, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsShader, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glLinkProgram, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLint var3 [0]; ,/*EMPTY*/,glShaderSource, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLchar * const *, duk_get_string(ctx, 2), const GLint *, duk_gl_get_GLint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glStencilFuncSeparate, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glStencilMaskSeparate, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glStencilOpSeparate, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glUniform1f, GLint, duk_get_int(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var2 [1]; ,/*EMPTY*/,glUniform1fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glUniform1i, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLint var2 [1]; ,/*EMPTY*/,glUniform1iv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUniform2f, GLint, duk_get_int(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var2 [2]; ,/*EMPTY*/,glUniform2fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 2, var2, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUniform2i, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLint var2 [2]; ,/*EMPTY*/,glUniform2iv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLint *, duk_gl_get_GLint_array(ctx, 2, 2, var2, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glUniform3f, GLint, duk_get_int(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var2 [3]; ,/*EMPTY*/,glUniform3fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 3, var2, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glUniform3i, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLint var2 [3]; ,/*EMPTY*/,glUniform3iv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLint *, duk_gl_get_GLint_array(ctx, 2, 3, var2, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glUniform4f, GLint, duk_get_int(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3), GLfloat, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var2 [4]; ,/*EMPTY*/,glUniform4fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 4, var2, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glUniform4i, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLint var2 [4]; ,/*EMPTY*/,glUniform4iv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLint *, duk_gl_get_GLint_array(ctx, 2, 4, var2, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [4]; ,/*EMPTY*/,glUniformMatrix2fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [9]; ,/*EMPTY*/,glUniformMatrix3fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 9, var3, 9))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [16]; ,/*EMPTY*/,glUniformMatrix4fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 16, var3, 16))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glUseProgram, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glValidateProgram, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttrib1d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [1]; ,/*EMPTY*/,glVertexAttrib1dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttrib1f, GLuint, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [1]; ,/*EMPTY*/,glVertexAttrib1fv, GLuint, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttrib1s, GLuint, duk_get_uint(ctx, 0), GLshort, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [1]; ,/*EMPTY*/,glVertexAttrib1sv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttrib2d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [2]; ,/*EMPTY*/,glVertexAttrib2dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttrib2f, GLuint, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [2]; ,/*EMPTY*/,glVertexAttrib2fv, GLuint, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttrib2s, GLuint, duk_get_uint(ctx, 0), GLshort, duk_get_int(ctx, 1), GLshort, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [2]; ,/*EMPTY*/,glVertexAttrib2sv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttrib3d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [3]; ,/*EMPTY*/,glVertexAttrib3dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttrib3f, GLuint, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [3]; ,/*EMPTY*/,glVertexAttrib3fv, GLuint, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttrib3s, GLuint, duk_get_uint(ctx, 0), GLshort, duk_get_int(ctx, 1), GLshort, duk_get_int(ctx, 2), GLshort, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [3]; ,/*EMPTY*/,glVertexAttrib3sv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLbyte var1 [0]; ,/*EMPTY*/,glVertexAttrib4Nbv, GLuint, duk_get_uint(ctx, 0), const GLbyte *, duk_gl_get_GLbyte_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [1]; ,/*EMPTY*/,glVertexAttrib4Niv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [1]; ,/*EMPTY*/,glVertexAttrib4Nsv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttrib4Nub, GLuint, duk_get_uint(ctx, 0), GLubyte, duk_get_uint(ctx, 1), GLubyte, duk_get_uint(ctx, 2), GLubyte, duk_get_uint(ctx, 3), GLubyte, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLubyte var1 [1]; ,/*EMPTY*/,glVertexAttrib4Nubv, GLuint, duk_get_uint(ctx, 0), const GLubyte *, duk_gl_get_GLubyte_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [1]; ,/*EMPTY*/,glVertexAttrib4Nuiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLushort var1 [1]; ,/*EMPTY*/,glVertexAttrib4Nusv, GLuint, duk_get_uint(ctx, 0), const GLushort *, duk_gl_get_GLushort_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLbyte var1 [0]; ,/*EMPTY*/,glVertexAttrib4bv, GLuint, duk_get_uint(ctx, 0), const GLbyte *, duk_gl_get_GLbyte_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttrib4d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3), GLdouble, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [4]; ,/*EMPTY*/,glVertexAttrib4dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttrib4f, GLuint, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3), GLfloat, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [4]; ,/*EMPTY*/,glVertexAttrib4fv, GLuint, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [4]; ,/*EMPTY*/,glVertexAttrib4iv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttrib4s, GLuint, duk_get_uint(ctx, 0), GLshort, duk_get_int(ctx, 1), GLshort, duk_get_int(ctx, 2), GLshort, duk_get_int(ctx, 3), GLshort, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [4]; ,/*EMPTY*/,glVertexAttrib4sv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLubyte var1 [4]; ,/*EMPTY*/,glVertexAttrib4ubv, GLuint, duk_get_uint(ctx, 0), const GLubyte *, duk_gl_get_GLubyte_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [4]; ,/*EMPTY*/,glVertexAttrib4uiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLushort var1 [4]; ,/*EMPTY*/,glVertexAttrib4usv, GLuint, duk_get_uint(ctx, 0), const GLushort *, duk_gl_get_GLushort_array(ctx, 1, 4, var1, 4))
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_2_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [6]; ,/*EMPTY*/,glUniformMatrix2x3fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 6, var3, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [8]; ,/*EMPTY*/,glUniformMatrix2x4fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 8, var3, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [6]; ,/*EMPTY*/,glUniformMatrix3x2fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 6, var3, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [12]; ,/*EMPTY*/,glUniformMatrix3x4fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 12, var3, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [8]; ,/*EMPTY*/,glUniformMatrix4x2fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 8, var3, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var3 [12]; ,/*EMPTY*/,glUniformMatrix4x3fv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 12, var3, 12))
#endif /* DUK_GL_OPENGL_2_1 */

#ifdef DUK_GL_OPENGL_3_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBeginConditionalRender, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glBeginTransformFeedback, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBindBufferBase, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBindFragDataLocation, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindFramebuffer, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindRenderbuffer, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glBindVertexArray, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG10(/*EMPTY*/,/*EMPTY*/,glBlitFramebuffer, GLint, duk_get_int(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLint, duk_get_int(ctx, 6), GLint, duk_get_int(ctx, 7), GLbitfield, duk_get_uint(ctx, 8), GLenum, duk_get_uint(ctx, 9))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glCheckFramebufferStatus, uint, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glClampColor, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glClearBufferfi, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLfloat var2 [1]; ,/*EMPTY*/,glClearBufferfv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glClearBufferiv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLuint var2 [1]; ,/*EMPTY*/,glClearBufferuiv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glColorMaski, GLuint, duk_get_uint(ctx, 0), GLboolean, duk_get_boolean(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLboolean, duk_get_boolean(ctx, 3), GLboolean, duk_get_boolean(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteFramebuffers, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteRenderbuffers, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteVertexArrays, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDisablei, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glEnablei, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glEndConditionalRender)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glEndTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glFramebufferRenderbuffer, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glFramebufferTexture1D, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glFramebufferTexture2D, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glFramebufferTexture3D, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glFramebufferTextureLayer, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenFramebuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenRenderbuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenVertexArrays, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glGenerateMipmap, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLboolean var2 [0]; ,duk_gl_put_GLboolean_array(ctx, 2, 0, var2, 0); ,glGetBooleani_v, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLboolean *, duk_gl_get_GLboolean_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetFragDataLocation, int, GLuint, duk_get_uint(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetFramebufferAttachmentParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [0]; ,duk_gl_put_GLint_array(ctx, 2, 0, var2, 0); ,glGetIntegeri_v, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetRenderbufferParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetTexParameterIiv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetTexParameterIuiv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var4 = duk_get_DETERMINE(ctx, 4); GLsizei var3 [0]; GLsizei var4 [0]; GLenum var5 [0]; GLchar var6 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLsizei_array(ctx, 4, 0, var4, 0); duk_gl_put_GLenum_array(ctx, 5, 0, var5, 0); duk_gl_put_GLchar_array(ctx, 6, 0, var6, 0); ,glGetTransformFeedbackVarying, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLsizei *, duk_gl_get_GLsizei_array(ctx, 4, 0, var4, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 5, 0, var5, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 6, 0, var6, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetUniformuiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribIiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribIuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glIsEnabledi, boolean, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsFramebuffer, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsRenderbuffer, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsVertexArray, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glRenderbufferStorage, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glRenderbufferStorageMultisample, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glTexParameterIiv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLuint var2 [1]; ,/*EMPTY*/,glTexParameterIuiv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); ,/*EMPTY*/,glTransformFeedbackVaryings, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLchar * const *, duk_get_string(ctx, 2), GLenum, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glUniform1ui, GLint, duk_get_int(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [1]; ,/*EMPTY*/,glUniform1uiv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUniform2ui, GLint, duk_get_int(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [2]; ,/*EMPTY*/,glUniform2uiv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 2, var2, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glUniform3ui, GLint, duk_get_int(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [3]; ,/*EMPTY*/,glUniform3uiv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 3, var2, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glUniform4ui, GLint, duk_get_int(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [4]; ,/*EMPTY*/,glUniform4uiv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 4, var2, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribI1i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [1]; ,/*EMPTY*/,glVertexAttribI1iv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribI1ui, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [1]; ,/*EMPTY*/,glVertexAttribI1uiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttribI2i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [2]; ,/*EMPTY*/,glVertexAttribI2iv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttribI2ui, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [2]; ,/*EMPTY*/,glVertexAttribI2uiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribI3i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [3]; ,/*EMPTY*/,glVertexAttribI3iv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribI3ui, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [3]; ,/*EMPTY*/,glVertexAttribI3uiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLbyte var1 [0]; ,/*EMPTY*/,glVertexAttribI4bv, GLuint, duk_get_uint(ctx, 0), const GLbyte *, duk_gl_get_GLbyte_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttribI4i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [4]; ,/*EMPTY*/,glVertexAttribI4iv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLshort var1 [4]; ,/*EMPTY*/,glVertexAttribI4sv, GLuint, duk_get_uint(ctx, 0), const GLshort *, duk_gl_get_GLshort_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLubyte var1 [4]; ,/*EMPTY*/,glVertexAttribI4ubv, GLuint, duk_get_uint(ctx, 0), const GLubyte *, duk_gl_get_GLubyte_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttribI4ui, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLuint var1 [4]; ,/*EMPTY*/,glVertexAttribI4uiv, GLuint, duk_get_uint(ctx, 0), const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLushort var1 [4]; ,/*EMPTY*/,glVertexAttribI4usv, GLuint, duk_get_uint(ctx, 0), const GLushort *, duk_gl_get_GLushort_array(ctx, 1, 4, var1, 4))
#endif /* DUK_GL_OPENGL_3_0 */

#ifdef DUK_GL_OPENGL_3_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glDrawArraysInstanced, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var3 [0]; GLchar var4 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLchar_array(ctx, 4, 0, var4, 0); ,glGetActiveUniformBlockName, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 4, 0, var4, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetActiveUniformBlockiv, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var3 [0]; GLchar var4 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLchar_array(ctx, 4, 0, var4, 0); ,glGetActiveUniformName, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 4, 0, var4, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [1]; GLint var4 [1]; ,duk_gl_put_GLint_array(ctx, 4, 1, var4, 1); ,glGetActiveUniformsiv, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1), GLenum, duk_get_uint(ctx, 3), GLint *, duk_gl_get_GLint_array(ctx, 4, 1, var4, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetUniformBlockIndex, uint, GLuint, duk_get_uint(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLuint var3 [0]; ,duk_gl_put_GLuint_array(ctx, 3, 0, var3, 0); ,glGetUniformIndices, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLchar * const *, duk_get_string(ctx, 2), GLuint *, duk_gl_get_GLuint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glPrimitiveRestartIndex, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTexBuffer, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUniformBlockBinding, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
#endif /* DUK_GL_OPENGL_3_1 */

#ifdef DUK_GL_OPENGL_3_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glFramebufferTexture, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint64 var2 [0]; ,duk_gl_put_GLint64_array(ctx, 2, 0, var2, 0); ,glGetBufferParameteri64v, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint64 *, duk_gl_get_GLint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint64 var2 [0]; ,duk_gl_put_GLint64_array(ctx, 2, 0, var2, 0); ,glGetInteger64i_v, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint64 *, duk_gl_get_GLint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLint64 var1 [0]; ,duk_gl_put_GLint64_array(ctx, 1, 0, var1, 0); ,glGetInteger64v, GLenum, duk_get_uint(ctx, 0), GLint64 *, duk_gl_get_GLint64_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetMultisamplefv, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glProvokingVertex, GLenum, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glSampleMaski, GLuint, duk_get_uint(ctx, 0), GLbitfield, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glTexImage2DMultisample, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLboolean, duk_get_boolean(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glTexImage3DMultisample, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLsizei, var5, GLboolean, duk_get_boolean(ctx, 6))
#endif /* DUK_GL_OPENGL_3_2 */

#ifdef DUK_GL_OPENGL_3_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glBindFragDataLocationIndexed, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), const GLchar *, duk_get_string(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindSampler, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteSamplers, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenSamplers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glGetFragDataIndex, int, GLuint, duk_get_uint(ctx, 0), const GLchar *, duk_get_string(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint64 var2 [0]; ,duk_gl_put_GLint64_array(ctx, 2, 0, var2, 0); ,glGetQueryObjecti64v, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint64 *, duk_gl_get_GLint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint64 var2 [0]; ,duk_gl_put_GLuint64_array(ctx, 2, 0, var2, 0); ,glGetQueryObjectui64v, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint64 *, duk_gl_get_GLuint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetSamplerParameterIiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetSamplerParameterIuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetSamplerParameterfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetSamplerParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsSampler, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glQueryCounter, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glSamplerParameterIiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLuint var2 [1]; ,/*EMPTY*/,glSamplerParameterIuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glSamplerParameterf, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLfloat var2 [1]; ,/*EMPTY*/,glSamplerParameterfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glSamplerParameteri, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glSamplerParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribDivisor, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribP1ui, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLuint var3 [1]; ,/*EMPTY*/,glVertexAttribP1uiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribP2ui, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLuint var3 [2]; ,/*EMPTY*/,glVertexAttribP2uiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 2, var3, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribP3ui, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLuint var3 [3]; ,/*EMPTY*/,glVertexAttribP3uiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 3, var3, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribP4ui, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLuint var3 [4]; ,/*EMPTY*/,glVertexAttribP4uiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLboolean, duk_get_boolean(ctx, 2), const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 4, var3, 4))
#endif /* DUK_GL_OPENGL_3_3 */

#ifdef DUK_GL_OPENGL_4_0
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBeginQueryIndexed, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindTransformFeedback, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBlendEquationSeparatei, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBlendEquationi, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glBlendFuncSeparatei, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLenum, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glBlendFunci, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteTransformFeedbacks, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDrawTransformFeedback, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glDrawTransformFeedbackStream, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glEndQueryIndexed, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenTransformFeedbacks, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_DETERMINE(ctx, 4); GLsizei var4 [0]; GLchar var5 [0]; ,duk_gl_put_GLsizei_array(ctx, 4, 0, var4, 0); duk_gl_put_GLchar_array(ctx, 5, 0, var5, 0); ,glGetActiveSubroutineName, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei *, duk_gl_get_GLsizei_array(ctx, 4, 0, var4, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 5, 0, var5, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_DETERMINE(ctx, 4); GLsizei var4 [0]; GLchar var5 [0]; ,duk_gl_put_GLsizei_array(ctx, 4, 0, var4, 0); duk_gl_put_GLchar_array(ctx, 5, 0, var5, 0); ,glGetActiveSubroutineUniformName, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei *, duk_gl_get_GLsizei_array(ctx, 4, 0, var4, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 5, 0, var5, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLint var4 [1]; ,duk_gl_put_GLint_array(ctx, 4, 1, var4, 1); ,glGetActiveSubroutineUniformiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLint *, duk_gl_get_GLint_array(ctx, 4, 1, var4, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetProgramStageiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetQueryIndexediv, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(/*EMPTY*/,/*EMPTY*/,glGetSubroutineIndex, uint, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(/*EMPTY*/,/*EMPTY*/,glGetSubroutineUniformLocation, int, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetUniformSubroutineuiv, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLdouble var2 [1]; ,duk_gl_put_GLdouble_array(ctx, 2, 1, var2, 1); ,glGetUniformdv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsTransformFeedback, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMinSampleShading, GLfloat, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [1]; ,/*EMPTY*/,glPatchParameterfv, GLenum, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glPatchParameteri, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glPauseTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glResumeTransformFeedback)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glUniform1d, GLint, duk_get_int(ctx, 0), GLdouble, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var2 [1]; ,/*EMPTY*/,glUniform1dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUniform2d, GLint, duk_get_int(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var2 [2]; ,/*EMPTY*/,glUniform2dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 2, var2, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glUniform3d, GLint, duk_get_int(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var2 [3]; ,/*EMPTY*/,glUniform3dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 3, var2, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glUniform4d, GLint, duk_get_int(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3), GLdouble, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var2 [4]; ,/*EMPTY*/,glUniform4dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 4, var2, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [4]; ,/*EMPTY*/,glUniformMatrix2dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [6]; ,/*EMPTY*/,glUniformMatrix2x3dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 6, var3, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [8]; ,/*EMPTY*/,glUniformMatrix2x4dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 8, var3, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [9]; ,/*EMPTY*/,glUniformMatrix3dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 9, var3, 9))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [6]; ,/*EMPTY*/,glUniformMatrix3x2dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 6, var3, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [12]; ,/*EMPTY*/,glUniformMatrix3x4dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 12, var3, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [16]; ,/*EMPTY*/,glUniformMatrix4dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 16, var3, 16))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [8]; ,/*EMPTY*/,glUniformMatrix4x2dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 8, var3, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var3 [12]; ,/*EMPTY*/,glUniformMatrix4x3dv, GLint, duk_get_int(ctx, 0), GLsizei, var1, GLboolean, duk_get_boolean(ctx, 2), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 12, var3, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [1]; ,/*EMPTY*/,glUniformSubroutinesuiv, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
#endif /* DUK_GL_OPENGL_4_0 */

#ifdef DUK_GL_OPENGL_4_1
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glActiveShaderProgram, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glBindProgramPipeline, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glClearDepthf, GLfloat, duk_get_number(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(GLsizei var1 = duk_get_int(ctx, 1); ,/*EMPTY*/,glCreateShaderProgramv, uint, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, const GLchar * const *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0);  GLuint var1 [0]; ,/*EMPTY*/,glDeleteProgramPipelines, GLsizei, var0, const GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLdouble var2 [0]; ,/*EMPTY*/,glDepthRangeArrayv, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glDepthRangeIndexed, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDepthRangef, GLfloat, duk_get_number(ctx, 0), GLfloat, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glGenProgramPipelines, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLdouble var2 [0]; ,duk_gl_put_GLdouble_array(ctx, 2, 0, var2, 0); ,glGetDoublei_v, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [0]; ,duk_gl_put_GLfloat_array(ctx, 2, 0, var2, 0); ,glGetFloati_v, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var2 = duk_get_DETERMINE(ctx, 2); GLsizei var2 [0]; GLchar var3 [0]; ,duk_gl_put_GLsizei_array(ctx, 2, 0, var2, 0); duk_gl_put_GLchar_array(ctx, 3, 0, var3, 0); ,glGetProgramPipelineInfoLog, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLsizei *, duk_gl_get_GLsizei_array(ctx, 2, 0, var2, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetProgramPipelineiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var2 [0]; GLint var3 [0]; ,duk_gl_put_GLint_array(ctx, 2, 0, var2, 0); duk_gl_put_GLint_array(ctx, 3, 0, var3, 0); ,glGetShaderPrecisionFormat, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 0, var2, 0), GLint *, duk_gl_get_GLint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLdouble var2 [1]; ,duk_gl_put_GLdouble_array(ctx, 2, 1, var2, 1); ,glGetVertexAttribLdv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLdouble *, duk_gl_get_GLdouble_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glIsProgramPipeline, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramParameteri, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramUniform1d, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLdouble, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var3 [1]; ,/*EMPTY*/,glProgramUniform1dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramUniform1f, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var3 [1]; ,/*EMPTY*/,glProgramUniform1fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramUniform1i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLint var3 [1]; ,/*EMPTY*/,glProgramUniform1iv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glProgramUniform1ui, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint var3 [1]; ,/*EMPTY*/,glProgramUniform1uiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glProgramUniform2d, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var3 [2]; ,/*EMPTY*/,glProgramUniform2dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 2, var3, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glProgramUniform2f, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var3 [2]; ,/*EMPTY*/,glProgramUniform2fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 2, var3, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glProgramUniform2i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLint var3 [2]; ,/*EMPTY*/,glProgramUniform2iv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLint *, duk_gl_get_GLint_array(ctx, 3, 2, var3, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glProgramUniform2ui, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint var3 [2]; ,/*EMPTY*/,glProgramUniform2uiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 2, var3, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glProgramUniform3d, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3), GLdouble, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var3 [3]; ,/*EMPTY*/,glProgramUniform3dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 3, var3, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glProgramUniform3f, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3), GLfloat, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var3 [3]; ,/*EMPTY*/,glProgramUniform3fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 3, var3, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glProgramUniform3i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLint var3 [3]; ,/*EMPTY*/,glProgramUniform3iv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLint *, duk_gl_get_GLint_array(ctx, 3, 3, var3, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glProgramUniform3ui, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint var3 [3]; ,/*EMPTY*/,glProgramUniform3uiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 3, var3, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glProgramUniform4d, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3), GLdouble, duk_get_number(ctx, 4), GLdouble, duk_get_number(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var3 [4]; ,/*EMPTY*/,glProgramUniform4dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glProgramUniform4f, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3), GLfloat, duk_get_number(ctx, 4), GLfloat, duk_get_number(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var3 [4]; ,/*EMPTY*/,glProgramUniform4fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glProgramUniform4i, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLint var3 [4]; ,/*EMPTY*/,glProgramUniform4iv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLint *, duk_gl_get_GLint_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glProgramUniform4ui, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4), GLuint, duk_get_uint(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint var3 [4]; ,/*EMPTY*/,glProgramUniform4uiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 4, var3, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [4]; ,/*EMPTY*/,glProgramUniformMatrix2dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 4, var4, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [4]; ,/*EMPTY*/,glProgramUniformMatrix2fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 4, var4, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [6]; ,/*EMPTY*/,glProgramUniformMatrix2x3dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 6, var4, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [6]; ,/*EMPTY*/,glProgramUniformMatrix2x3fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 6, var4, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [8]; ,/*EMPTY*/,glProgramUniformMatrix2x4dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 8, var4, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [8]; ,/*EMPTY*/,glProgramUniformMatrix2x4fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 8, var4, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [9]; ,/*EMPTY*/,glProgramUniformMatrix3dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 9, var4, 9))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [9]; ,/*EMPTY*/,glProgramUniformMatrix3fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 9, var4, 9))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [6]; ,/*EMPTY*/,glProgramUniformMatrix3x2dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 6, var4, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [6]; ,/*EMPTY*/,glProgramUniformMatrix3x2fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 6, var4, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [12]; ,/*EMPTY*/,glProgramUniformMatrix3x4dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 12, var4, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [12]; ,/*EMPTY*/,glProgramUniformMatrix3x4fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 12, var4, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [16]; ,/*EMPTY*/,glProgramUniformMatrix4dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 16, var4, 16))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [16]; ,/*EMPTY*/,glProgramUniformMatrix4fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 16, var4, 16))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [8]; ,/*EMPTY*/,glProgramUniformMatrix4x2dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 8, var4, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [8]; ,/*EMPTY*/,glProgramUniformMatrix4x2fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 8, var4, 8))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLdouble var4 [12]; ,/*EMPTY*/,glProgramUniformMatrix4x3dv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 4, 12, var4, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2);  GLfloat var4 [12]; ,/*EMPTY*/,glProgramUniformMatrix4x3fv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLboolean, duk_get_boolean(ctx, 3), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 4, 12, var4, 12))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glReleaseShaderCompiler)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLint var2 [0]; ,/*EMPTY*/,glScissorArrayv, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLint *, duk_gl_get_GLint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glScissorIndexed, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLsizei, var3, GLsizei, var4)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLint var1 [1]; ,/*EMPTY*/,glScissorIndexedv, GLuint, duk_get_uint(ctx, 0), const GLint *, duk_gl_get_GLint_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glUseProgramStages, GLuint, duk_get_uint(ctx, 0), GLbitfield, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glValidateProgramPipeline, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribL1d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [1]; ,/*EMPTY*/,glVertexAttribL1dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 1, var1, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexAttribL2d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [2]; ,/*EMPTY*/,glVertexAttribL2dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 2, var1, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribL3d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [3]; ,/*EMPTY*/,glVertexAttribL3dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 3, var1, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttribL4d, GLuint, duk_get_uint(ctx, 0), GLdouble, duk_get_number(ctx, 1), GLdouble, duk_get_number(ctx, 2), GLdouble, duk_get_number(ctx, 3), GLdouble, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLdouble var1 [4]; ,/*EMPTY*/,glVertexAttribL4dv, GLuint, duk_get_uint(ctx, 0), const GLdouble *, duk_gl_get_GLdouble_array(ctx, 1, 4, var1, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLfloat var2 [0]; ,/*EMPTY*/,glViewportArrayv, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glViewportIndexedf, GLuint, duk_get_uint(ctx, 0), GLfloat, duk_get_number(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLfloat, duk_get_number(ctx, 3), GLfloat, duk_get_number(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2( GLfloat var1 [1]; ,/*EMPTY*/,glViewportIndexedfv, GLuint, duk_get_uint(ctx, 0), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 1, 1, var1, 1))
#endif /* DUK_GL_OPENGL_4_1 */

#ifdef DUK_GL_OPENGL_4_2
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(/*EMPTY*/,/*EMPTY*/,glBindImageTexture, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLboolean, duk_get_boolean(ctx, 3), GLint, duk_get_int(ctx, 4), GLenum, duk_get_uint(ctx, 5), GLenum, duk_get_uint(ctx, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glDrawArraysInstancedBaseInstance, GLenum, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLsizei, var3, GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var2 = duk_get_int(ctx, 2); ,/*EMPTY*/,glDrawTransformFeedbackInstanced, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glDrawTransformFeedbackStreamInstanced, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetActiveAtomicCounterBufferiv, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var3 = duk_get_int(ctx, 3); GLint var4 [1]; ,duk_gl_put_GLint_array(ctx, 4, 1, var4, 1); ,glGetInternalformativ, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLint *, duk_gl_get_GLint_array(ctx, 4, 1, var4, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMemoryBarrier, GLbitfield, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glTexStorage1D, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glTexStorage2D, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glTexStorage3D, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLsizei, var5)
#endif /* DUK_GL_OPENGL_4_2 */

#ifdef DUK_GL_OPENGL_4_3
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG15(GLsizei var12 = duk_get_int(ctx, 12); GLsizei var13 = duk_get_int(ctx, 13); GLsizei var14 = duk_get_int(ctx, 14); ,/*EMPTY*/,glCopyImageSubData, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLuint, duk_get_uint(ctx, 6), GLenum, duk_get_uint(ctx, 7), GLint, duk_get_int(ctx, 8), GLint, duk_get_int(ctx, 9), GLint, duk_get_int(ctx, 10), GLint, duk_get_int(ctx, 11), GLsizei, var12, GLsizei, var13, GLsizei, var14)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var3 = duk_get_int(ctx, 3);  GLuint var4 [0]; ,/*EMPTY*/,glDebugMessageControl, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLsizei, var3, const GLuint *, duk_gl_get_GLuint_array(ctx, 4, 0, var4, 0), GLboolean, duk_get_boolean(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glDebugMessageInsert, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLsizei, var4, const GLchar *, duk_get_string(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glDispatchCompute, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glFramebufferParameteri, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG8(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var6 = duk_get_DETERMINE(ctx, 6); GLenum var2 [0]; GLenum var3 [0]; GLuint var4 [0]; GLenum var5 [0]; GLsizei var6 [0]; GLchar var7 [0]; ,duk_gl_put_GLenum_array(ctx, 2, 0, var2, 0); duk_gl_put_GLenum_array(ctx, 3, 0, var3, 0); duk_gl_put_GLuint_array(ctx, 4, 0, var4, 0); duk_gl_put_GLenum_array(ctx, 5, 0, var5, 0); duk_gl_put_GLsizei_array(ctx, 6, 0, var6, 0); duk_gl_put_GLchar_array(ctx, 7, 0, var7, 0); ,glGetDebugMessageLog, uint, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 3, 0, var3, 0), GLuint *, duk_gl_get_GLuint_array(ctx, 4, 0, var4, 0), GLenum *, duk_gl_get_GLenum_array(ctx, 5, 0, var5, 0), GLsizei *, duk_gl_get_GLsizei_array(ctx, 6, 0, var6, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 7, 0, var7, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetFramebufferParameteriv, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var3 = duk_get_int(ctx, 3); GLint64 var4 [0]; ,duk_gl_put_GLint64_array(ctx, 4, 0, var4, 0); ,glGetInternalformati64v, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLint64 *, duk_gl_get_GLint64_array(ctx, 4, 0, var4, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_DETERMINE(ctx, 3); GLsizei var3 [0]; GLchar var4 [0]; ,duk_gl_put_GLsizei_array(ctx, 3, 0, var3, 0); duk_gl_put_GLchar_array(ctx, 4, 0, var4, 0); ,glGetObjectLabel, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei *, duk_gl_get_GLsizei_array(ctx, 3, 0, var3, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 4, 0, var4, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetProgramInterfaceiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(/*EMPTY*/,/*EMPTY*/,glGetProgramResourceIndex, uint, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(/*EMPTY*/,/*EMPTY*/,glGetProgramResourceLocation, int, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG3(/*EMPTY*/,/*EMPTY*/,glGetProgramResourceLocationIndex, int, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLchar *, duk_get_string(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_DETERMINE(ctx, 4); GLsizei var4 [0]; GLchar var5 [0]; ,duk_gl_put_GLsizei_array(ctx, 4, 0, var4, 0); duk_gl_put_GLchar_array(ctx, 5, 0, var5, 0); ,glGetProgramResourceName, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei *, duk_gl_get_GLsizei_array(ctx, 4, 0, var4, 0), GLchar *, duk_gl_get_GLchar_array(ctx, 5, 0, var5, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(GLsizei var3 = duk_get_int(ctx, 3); GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_DETERMINE(ctx, 6);  GLenum var4 [1]; GLsizei var6 [1]; GLint var7 [1]; ,duk_gl_put_GLsizei_array(ctx, 6, 1, var6, 1); duk_gl_put_GLint_array(ctx, 7, 1, var7, 1); ,glGetProgramResourceiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLsizei, var3, const GLenum *, duk_gl_get_GLenum_array(ctx, 4, 1, var4, 1), GLsizei, var5, GLsizei *, duk_gl_get_GLsizei_array(ctx, 6, 1, var6, 1), GLint *, duk_gl_get_GLint_array(ctx, 7, 1, var7, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glInvalidateBufferData, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLenum var2 [0]; ,/*EMPTY*/,glInvalidateFramebuffer, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, const GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_int(ctx, 6);  GLenum var2 [0]; ,/*EMPTY*/,glInvalidateSubFramebuffer, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, const GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLsizei, var6)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glInvalidateTexImage, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_int(ctx, 6); GLsizei var7 = duk_get_int(ctx, 7); ,/*EMPTY*/,glInvalidateTexSubImage, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLsizei, var6, GLsizei, var7)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); ,/*EMPTY*/,glObjectLabel, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, const GLchar *, duk_get_string(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glPopDebugGroup)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); ,/*EMPTY*/,glPushDebugGroup, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, const GLchar *, duk_get_string(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glShaderStorageBlockBinding, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glTexStorage2DMultisample, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLboolean, duk_get_boolean(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glTexStorage3DMultisample, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLsizei, var5, GLboolean, duk_get_boolean(ctx, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(/*EMPTY*/,/*EMPTY*/,glTextureView, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4), GLuint, duk_get_uint(ctx, 5), GLuint, duk_get_uint(ctx, 6), GLuint, duk_get_uint(ctx, 7))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexAttribBinding, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexAttribFormat, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLboolean, duk_get_boolean(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribIFormat, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glVertexAttribLFormat, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexBindingDivisor, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
#endif /* DUK_GL_OPENGL_4_3 */

#ifdef DUK_GL_OPENGL_4_4
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2);  GLuint var3 [0]; ,/*EMPTY*/,glBindBuffersBase, GLenum, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLsizei, var2, const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [0]; ,/*EMPTY*/,glBindImageTextures, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [0]; ,/*EMPTY*/,glBindSamplers, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLuint var2 [0]; ,/*EMPTY*/,glBindTextures, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 0, var2, 0))
#endif /* DUK_GL_OPENGL_4_4 */

#ifdef DUK_GL_OPENGL_4_5
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glBindTextureUnit, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG12(/*EMPTY*/,/*EMPTY*/,glBlitNamedFramebuffer, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLint, duk_get_int(ctx, 6), GLint, duk_get_int(ctx, 7), GLint, duk_get_int(ctx, 8), GLint, duk_get_int(ctx, 9), GLbitfield, duk_get_uint(ctx, 10), GLenum, duk_get_uint(ctx, 11))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG2(/*EMPTY*/,/*EMPTY*/,glCheckNamedFramebufferStatus, uint, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glClearNamedFramebufferfi, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat, duk_get_number(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLfloat var3 [1]; ,/*EMPTY*/,glClearNamedFramebufferfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLint var3 [1]; ,/*EMPTY*/,glClearNamedFramebufferiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), const GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4( GLuint var3 [1]; ,/*EMPTY*/,glClearNamedFramebufferuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), const GLuint *, duk_gl_get_GLuint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glClipControl, GLenum, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glCopyTextureSubImage1D, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG8(GLsizei var6 = duk_get_int(ctx, 6); GLsizei var7 = duk_get_int(ctx, 7); ,/*EMPTY*/,glCopyTextureSubImage2D, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLsizei, var6, GLsizei, var7)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG9(GLsizei var7 = duk_get_int(ctx, 7); GLsizei var8 = duk_get_int(ctx, 8); ,/*EMPTY*/,glCopyTextureSubImage3D, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLint, duk_get_int(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLint, duk_get_int(ctx, 5), GLint, duk_get_int(ctx, 6), GLsizei, var7, GLsizei, var8)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateBuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateFramebuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateProgramPipelines, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1); GLuint var2 [0]; ,duk_gl_put_GLuint_array(ctx, 2, 0, var2, 0); ,glCreateQueries, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLuint *, duk_gl_get_GLuint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateRenderbuffers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateSamplers, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1); GLuint var2 [0]; ,duk_gl_put_GLuint_array(ctx, 2, 0, var2, 0); ,glCreateTextures, GLenum, duk_get_uint(ctx, 0), GLsizei, var1, GLuint *, duk_gl_get_GLuint_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateTransformFeedbacks, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(GLsizei var0 = duk_get_int(ctx, 0); GLuint var1 [0]; ,duk_gl_put_GLuint_array(ctx, 1, 0, var1, 0); ,glCreateVertexArrays, GLsizei, var0, GLuint *, duk_gl_get_GLuint_array(ctx, 1, 0, var1, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glDisableVertexArrayAttrib, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glEnableVertexArrayAttrib, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glGenerateTextureMipmap, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG0(/*EMPTY*/,/*EMPTY*/,glGetGraphicsResetStatus, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint64 var2 [0]; ,duk_gl_put_GLint64_array(ctx, 2, 0, var2, 0); ,glGetNamedBufferParameteri64v, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint64 *, duk_gl_get_GLint64_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetNamedBufferParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetNamedFramebufferAttachmentParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetNamedFramebufferParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetNamedRenderbufferParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLfloat var3 [1]; ,duk_gl_put_GLfloat_array(ctx, 3, 1, var3, 1); ,glGetTextureLevelParameterfv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetTextureLevelParameteriv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetTextureParameterIiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLuint var2 [1]; ,duk_gl_put_GLuint_array(ctx, 2, 1, var2, 1); ,glGetTextureParameterIuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLfloat var2 [1]; ,duk_gl_put_GLfloat_array(ctx, 2, 1, var2, 1); ,glGetTextureParameterfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetTextureParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint64 var3 [0]; ,duk_gl_put_GLint64_array(ctx, 3, 0, var3, 0); ,glGetTransformFeedbacki64_v, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint64 *, duk_gl_get_GLint64_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [0]; ,duk_gl_put_GLint_array(ctx, 3, 0, var3, 0); ,glGetTransformFeedbacki_v, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 0, var3, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetTransformFeedbackiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint64 var3 [1]; ,duk_gl_put_GLint64_array(ctx, 3, 1, var3, 1); ,glGetVertexArrayIndexed64iv, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint64 *, duk_gl_get_GLint64_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetVertexArrayIndexediv, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLint var2 [1]; ,duk_gl_put_GLint_array(ctx, 2, 1, var2, 1); ,glGetVertexArrayiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLdouble var3 [1]; ,duk_gl_put_GLdouble_array(ctx, 3, 1, var3, 1); ,glGetnUniformdv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLdouble *, duk_gl_get_GLdouble_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLfloat var3 [1]; ,duk_gl_put_GLfloat_array(ctx, 3, 1, var3, 1); ,glGetnUniformfv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLfloat *, duk_gl_get_GLfloat_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLint var3 [1]; ,duk_gl_put_GLint_array(ctx, 3, 1, var3, 1); ,glGetnUniformiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLint *, duk_gl_get_GLint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLuint var3 [1]; ,duk_gl_put_GLuint_array(ctx, 3, 1, var3, 1); ,glGetnUniformuiv, GLuint, duk_get_uint(ctx, 0), GLint, duk_get_int(ctx, 1), GLsizei, var2, GLuint *, duk_gl_get_GLuint_array(ctx, 3, 1, var3, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLenum var2 [0]; ,/*EMPTY*/,glInvalidateNamedFramebufferData, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var5 = duk_get_int(ctx, 5); GLsizei var6 = duk_get_int(ctx, 6);  GLenum var2 [0]; ,/*EMPTY*/,glInvalidateNamedFramebufferSubData, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4), GLsizei, var5, GLsizei, var6)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(/*EMPTY*/,/*EMPTY*/,glMemoryBarrierByRegion, GLbitfield, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferDrawBuffer, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(GLsizei var1 = duk_get_int(ctx, 1);  GLenum var2 [0]; ,/*EMPTY*/,glNamedFramebufferDrawBuffers, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, const GLenum *, duk_gl_get_GLenum_array(ctx, 2, 0, var2, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferParameteri, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferReadBuffer, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferRenderbuffer, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLenum, duk_get_uint(ctx, 2), GLuint, duk_get_uint(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferTexture, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glNamedFramebufferTextureLayer, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2), GLint, duk_get_int(ctx, 3), GLint, duk_get_int(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var2 = duk_get_int(ctx, 2); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glNamedRenderbufferStorage, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLsizei, var2, GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glNamedRenderbufferStorageMultisample, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(/*EMPTY*/,/*EMPTY*/,glTextureBarrier)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTextureBuffer, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glTextureParameterIiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLuint var2 [1]; ,/*EMPTY*/,glTextureParameterIuiv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLuint *, duk_gl_get_GLuint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTextureParameterf, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLfloat, duk_get_number(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLfloat var2 [1]; ,/*EMPTY*/,glTextureParameterfv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLfloat *, duk_gl_get_GLfloat_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTextureParameteri, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3( GLint var2 [1]; ,/*EMPTY*/,glTextureParameteriv, GLuint, duk_get_uint(ctx, 0), GLenum, duk_get_uint(ctx, 1), const GLint *, duk_gl_get_GLint_array(ctx, 2, 1, var2, 1))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG4(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); ,/*EMPTY*/,glTextureStorage1D, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glTextureStorage2D, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); ,/*EMPTY*/,glTextureStorage2DMultisample, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLboolean, duk_get_boolean(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glTextureStorage3D, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLsizei, var5)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG7(GLsizei var1 = duk_get_int(ctx, 1); GLsizei var3 = duk_get_int(ctx, 3); GLsizei var4 = duk_get_int(ctx, 4); GLsizei var5 = duk_get_int(ctx, 5); ,/*EMPTY*/,glTextureStorage3DMultisample, GLuint, duk_get_uint(ctx, 0), GLsizei, var1, GLenum, duk_get_uint(ctx, 2), GLsizei, var3, GLsizei, var4, GLsizei, var5, GLboolean, duk_get_boolean(ctx, 6))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glTransformFeedbackBufferBase, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET1_ARG1(/*EMPTY*/,/*EMPTY*/,glUnmapNamedBuffer, boolean, GLuint, duk_get_uint(ctx, 0))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexArrayAttribBinding, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG6(/*EMPTY*/,/*EMPTY*/,glVertexArrayAttribFormat, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLboolean, duk_get_boolean(ctx, 4), GLuint, duk_get_uint(ctx, 5))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexArrayAttribIFormat, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG5(/*EMPTY*/,/*EMPTY*/,glVertexArrayAttribLFormat, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLint, duk_get_int(ctx, 2), GLenum, duk_get_uint(ctx, 3), GLuint, duk_get_uint(ctx, 4))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG3(/*EMPTY*/,/*EMPTY*/,glVertexArrayBindingDivisor, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1), GLuint, duk_get_uint(ctx, 2))
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(/*EMPTY*/,/*EMPTY*/,glVertexArrayElementBuffer, GLuint, duk_get_uint(ctx, 0), GLuint, duk_get_uint(ctx, 1))
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
	duk_gl_bind_opengl_wrapper(ctx, glCompileShaderIncludeARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageControlARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageInsertARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteNamedStringARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchComputeGroupSizeARB, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGetDebugMessageLogARB, 8);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatusARB, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetImageHandleARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedStringARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedStringivARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureHandleARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureSamplerHandleARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribLui64vARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformdvARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformfvARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformivARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformuivARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glIsImageHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsNamedStringARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeImageHandleResidentARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleNonResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMakeTextureHandleResidentARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinSampleShadingARB, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNamedStringARB, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformHandleui64ARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformHandleui64vARB, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexPageCommitmentARB, 9);
	duk_gl_bind_opengl_wrapper(ctx, glUniformHandleui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniformHandleui64vARB, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1ui64ARB, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1ui64vARB, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glDeleteTextures, 2);
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
	duk_gl_bind_opengl_wrapper(ctx, glGenTextures, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetBooleanv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetDoublev, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetError, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetFloatv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetIntegerv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexLevelParameterfv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexLevelParameteriv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexParameteriv, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameteriv, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glMultiDrawArrays, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterf, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameterfv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameteri, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPointParameteriv, 2);
#endif /* DUK_GL_OPENGL_1_4 */

#ifdef DUK_GL_OPENGL_1_5
	duk_gl_bind_opengl_wrapper(ctx, glBeginQuery, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteBuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteQueries, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndQuery, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGenBuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenQueries, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetBufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryObjectiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryObjectuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryiv, 3);
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
	duk_gl_bind_opengl_wrapper(ctx, glDrawBuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexAttribArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveAttrib, 7);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveUniform, 7);
	duk_gl_bind_opengl_wrapper(ctx, glGetAttachedShaders, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetAttribLocation, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramInfoLog, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetShaderInfoLog, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetShaderSource, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetShaderiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformLocation, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribdv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsShader, 1);
	duk_gl_bind_opengl_wrapper(ctx, glLinkProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glShaderSource, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilFuncSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glStencilMaskSeparate, 2);
	duk_gl_bind_opengl_wrapper(ctx, glStencilOpSeparate, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1fv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1iv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2fv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2iv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3fv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3iv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4fv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4iv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUseProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glValidateProgram, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1f, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1fv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1s, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib1sv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2fv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2s, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib2sv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3fv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3s, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib3sv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nbv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Niv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nsv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nub, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nubv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nuiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4Nusv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4bv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4fv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4iv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4s, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4sv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4ubv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4uiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttrib4usv, 2);
#endif /* DUK_GL_OPENGL_2_0 */

#ifdef DUK_GL_OPENGL_2_1
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2x3fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2x4fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3x2fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3x4fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4x2fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4x3fv, 4);
#endif /* DUK_GL_OPENGL_2_1 */

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
	duk_gl_bind_opengl_wrapper(ctx, glClearBufferfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glClearBufferiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glClearBufferuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glColorMaski, 5);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteFramebuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteRenderbuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteVertexArrays, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisablei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnablei, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEndConditionalRender, 0);
	duk_gl_bind_opengl_wrapper(ctx, glEndTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferRenderbuffer, 4);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture1D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture3D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTextureLayer, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGenFramebuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenRenderbuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenVertexArrays, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateMipmap, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetBooleani_v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetFragDataLocation, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetFramebufferAttachmentParameteriv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetIntegeri_v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetRenderbufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTexParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTransformFeedbackVarying, 7);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsEnabledi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glIsFramebuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsRenderbuffer, 1);
	duk_gl_bind_opengl_wrapper(ctx, glIsVertexArray, 1);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorage, 4);
	duk_gl_bind_opengl_wrapper(ctx, glRenderbufferStorageMultisample, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTexParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTransformFeedbackVaryings, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1uiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2uiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3uiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4ui, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4uiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1i, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1iv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1ui, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI1uiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2iv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI2uiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3iv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI3uiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4bv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4iv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4sv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4ubv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4ui, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4uiv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribI4usv, 2);
#endif /* DUK_GL_OPENGL_3_0 */

#ifdef DUK_GL_OPENGL_3_1
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstanced, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveUniformBlockName, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveUniformBlockiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveUniformName, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveUniformsiv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformBlockIndex, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformIndices, 4);
	duk_gl_bind_opengl_wrapper(ctx, glPrimitiveRestartIndex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexBuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniformBlockBinding, 3);
#endif /* DUK_GL_OPENGL_3_1 */

#ifdef DUK_GL_OPENGL_3_2
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferTexture, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetBufferParameteri64v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetInteger64i_v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetInteger64v, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetMultisamplefv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProvokingVertex, 1);
	duk_gl_bind_opengl_wrapper(ctx, glSampleMaski, 2);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage2DMultisample, 6);
	duk_gl_bind_opengl_wrapper(ctx, glTexImage3DMultisample, 7);
#endif /* DUK_GL_OPENGL_3_2 */

#ifdef DUK_GL_OPENGL_3_3
	duk_gl_bind_opengl_wrapper(ctx, glBindFragDataLocationIndexed, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBindSampler, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteSamplers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenSamplers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetFragDataIndex, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryObjecti64v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryObjectui64v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSamplerParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSamplerParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSamplerParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSamplerParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsSampler, 1);
	duk_gl_bind_opengl_wrapper(ctx, glQueryCounter, 2);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glSamplerParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribDivisor, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP1ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP1uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP2ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP2uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP3ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP3uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP4ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribP4uiv, 4);
#endif /* DUK_GL_OPENGL_3_3 */

#ifdef DUK_GL_OPENGL_4_0
	duk_gl_bind_opengl_wrapper(ctx, glBeginQueryIndexed, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindTransformFeedback, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationSeparatei, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBlendEquationi, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFuncSeparatei, 5);
	duk_gl_bind_opengl_wrapper(ctx, glBlendFunci, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteTransformFeedbacks, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedback, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackStream, 3);
	duk_gl_bind_opengl_wrapper(ctx, glEndQueryIndexed, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenTransformFeedbacks, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveSubroutineName, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveSubroutineUniformName, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveSubroutineUniformiv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramStageiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetQueryIndexediv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetSubroutineIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetSubroutineUniformLocation, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformSubroutineuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetUniformdv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsTransformFeedback, 1);
	duk_gl_bind_opengl_wrapper(ctx, glMinSampleShading, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPatchParameterfv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPatchParameteri, 2);
	duk_gl_bind_opengl_wrapper(ctx, glPauseTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glResumeTransformFeedback, 0);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUniform1dv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform2dv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniform3dv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glUniform4dv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2x3dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix2x4dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3x2dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix3x4dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4x2dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformMatrix4x3dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glUniformSubroutinesuiv, 3);
#endif /* DUK_GL_OPENGL_4_0 */

#ifdef DUK_GL_OPENGL_4_1
	duk_gl_bind_opengl_wrapper(ctx, glActiveShaderProgram, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBindProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glClearDepthf, 1);
	duk_gl_bind_opengl_wrapper(ctx, glCreateShaderProgramv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDeleteProgramPipelines, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangeArrayv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangeIndexed, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDepthRangef, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenProgramPipelines, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGetDoublei_v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetFloati_v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramPipelineInfoLog, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramPipelineiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetShaderPrecisionFormat, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexAttribLdv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glIsProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glProgramParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1f, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1i, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1iv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1ui, 3);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform1uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2f, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2i, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2iv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2ui, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform2uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3f, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3i, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3iv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3ui, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform3uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4d, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4dv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4f, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4fv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4i, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4iv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4ui, 6);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniform4uiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2x3dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2x3fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2x4dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix2x4fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3x2dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3x2fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3x4dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix3x4fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4x2dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4x2fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4x3dv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glProgramUniformMatrix4x3fv, 5);
	duk_gl_bind_opengl_wrapper(ctx, glReleaseShaderCompiler, 0);
	duk_gl_bind_opengl_wrapper(ctx, glScissorArrayv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glScissorIndexed, 5);
	duk_gl_bind_opengl_wrapper(ctx, glScissorIndexedv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glUseProgramStages, 3);
	duk_gl_bind_opengl_wrapper(ctx, glValidateProgramPipeline, 1);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1d, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL1dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2d, 3);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL2dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3d, 4);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL3dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4d, 5);
	duk_gl_bind_opengl_wrapper(ctx, glVertexAttribL4dv, 2);
	duk_gl_bind_opengl_wrapper(ctx, glViewportArrayv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glViewportIndexedf, 5);
	duk_gl_bind_opengl_wrapper(ctx, glViewportIndexedfv, 2);
#endif /* DUK_GL_OPENGL_4_1 */

#ifdef DUK_GL_OPENGL_4_2
	duk_gl_bind_opengl_wrapper(ctx, glBindImageTexture, 7);
	duk_gl_bind_opengl_wrapper(ctx, glDrawArraysInstancedBaseInstance, 5);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackInstanced, 3);
	duk_gl_bind_opengl_wrapper(ctx, glDrawTransformFeedbackStreamInstanced, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetActiveAtomicCounterBufferiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetInternalformativ, 5);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrier, 1);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage1D, 4);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage2D, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTexStorage3D, 6);
#endif /* DUK_GL_OPENGL_4_2 */

#ifdef DUK_GL_OPENGL_4_3
	duk_gl_bind_opengl_wrapper(ctx, glCopyImageSubData, 15);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageControl, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDebugMessageInsert, 6);
	duk_gl_bind_opengl_wrapper(ctx, glDispatchCompute, 3);
	duk_gl_bind_opengl_wrapper(ctx, glFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetDebugMessageLog, 8);
	duk_gl_bind_opengl_wrapper(ctx, glGetFramebufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetInternalformati64v, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetObjectLabel, 5);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramInterfaceiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceLocation, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceLocationIndex, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceName, 6);
	duk_gl_bind_opengl_wrapper(ctx, glGetProgramResourceiv, 8);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateBufferData, 1);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateFramebuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateSubFramebuffer, 7);
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

#ifdef DUK_GL_OPENGL_4_4
	duk_gl_bind_opengl_wrapper(ctx, glBindBuffersBase, 4);
	duk_gl_bind_opengl_wrapper(ctx, glBindImageTextures, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindSamplers, 3);
	duk_gl_bind_opengl_wrapper(ctx, glBindTextures, 3);
#endif /* DUK_GL_OPENGL_4_4 */

#ifdef DUK_GL_OPENGL_4_5
	duk_gl_bind_opengl_wrapper(ctx, glBindTextureUnit, 2);
	duk_gl_bind_opengl_wrapper(ctx, glBlitNamedFramebuffer, 12);
	duk_gl_bind_opengl_wrapper(ctx, glCheckNamedFramebufferStatus, 2);
	duk_gl_bind_opengl_wrapper(ctx, glClearNamedFramebufferfi, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearNamedFramebufferfv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearNamedFramebufferiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClearNamedFramebufferuiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glClipControl, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage1D, 6);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage2D, 8);
	duk_gl_bind_opengl_wrapper(ctx, glCopyTextureSubImage3D, 9);
	duk_gl_bind_opengl_wrapper(ctx, glCreateBuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateFramebuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateProgramPipelines, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateQueries, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCreateRenderbuffers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateSamplers, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateTextures, 3);
	duk_gl_bind_opengl_wrapper(ctx, glCreateTransformFeedbacks, 2);
	duk_gl_bind_opengl_wrapper(ctx, glCreateVertexArrays, 2);
	duk_gl_bind_opengl_wrapper(ctx, glDisableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glEnableVertexArrayAttrib, 2);
	duk_gl_bind_opengl_wrapper(ctx, glGenerateTextureMipmap, 1);
	duk_gl_bind_opengl_wrapper(ctx, glGetGraphicsResetStatus, 0);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedBufferParameteri64v, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedBufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedFramebufferAttachmentParameteriv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedFramebufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetNamedRenderbufferParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureLevelParameterfv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureLevelParameteriv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTextureParameteriv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetTransformFeedbacki64_v, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTransformFeedbacki_v, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetTransformFeedbackiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexArrayIndexed64iv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexArrayIndexediv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetVertexArrayiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformdv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformfv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glGetnUniformuiv, 4);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateNamedFramebufferData, 3);
	duk_gl_bind_opengl_wrapper(ctx, glInvalidateNamedFramebufferSubData, 7);
	duk_gl_bind_opengl_wrapper(ctx, glMemoryBarrierByRegion, 1);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferDrawBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferDrawBuffers, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferReadBuffer, 2);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferRenderbuffer, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTexture, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedFramebufferTextureLayer, 5);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorage, 4);
	duk_gl_bind_opengl_wrapper(ctx, glNamedRenderbufferStorageMultisample, 5);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBarrier, 0);
	duk_gl_bind_opengl_wrapper(ctx, glTextureBuffer, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterIiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterIuiv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterf, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameterfv, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameteri, 3);
	duk_gl_bind_opengl_wrapper(ctx, glTextureParameteriv, 3);
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
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_COLOR);
	duk_gl_push_opengl_constant_property(ctx, GL_BLEND_EQUATION);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_INPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGBA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_COMPUTE_SHADER_INVOCATIONS_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_ROBUST_ACCESS);
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
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_DEPTH_MODE);
	duk_gl_push_opengl_constant_property(ctx, GL_CLIP_ORIGIN);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_LOST);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR);
	duk_gl_push_opengl_constant_property(ctx, GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH);
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

