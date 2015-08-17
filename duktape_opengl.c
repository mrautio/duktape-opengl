/*
 *  Duktape OpenGL wrapper 0.1.
 *  See AUTHORS.rst and LICENSE.txt for copyright and licensing information.
 */

#include <duktape.h>
#include <GL/gl.h>

/*
 *  Macro for binding OpenGL wrapper C function as Duktape JavaScript function
 */
#define duk_gl_bind_opengl_wrapper(ctx, c_function_name, argument_count) \
	duk_push_c_function((ctx), duk_gl_##c_function_name, (argument_count)); \
	duk_put_prop_string((ctx), -2, #c_function_name)

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

#define DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(c_function_name, argtypedef1, arg1) \
static duk_ret_t duk_gl_##c_function_name(duk_context *ctx) \
{ \
	c_function_name( \
		(argtypedef1)duk_to_##arg1(ctx, 0) \
	); \
	return 0; \
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

/*
 *  OpenGL wrapper function definitions
 */
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glEnableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glDisableClientState, GLenum, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopAttrib)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG1(glPushClientAttrib, GLbitfield, uint)
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG0(glPopClientAttrib)
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
DUK_GL_C_WRAPPER_FUNCTION_RET0_ARG2(glDeleteLists, GLuint, uint, GLsizei, int)
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

/*
 *  OpenGL function bindings to JavaScript
 */
void duk_gl_bind_opengl_functions(duk_context *ctx)
{
	duk_push_global_object(ctx);
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
	duk_gl_bind_opengl_wrapper(ctx, glEnableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glDisableClientState, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPushAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPopAttrib, 0);
	duk_gl_bind_opengl_wrapper(ctx, glPushClientAttrib, 1);
	duk_gl_bind_opengl_wrapper(ctx, glPopClientAttrib, 0);
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
	duk_gl_bind_opengl_wrapper(ctx, glDeleteLists, 2);
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
	duk_pop(ctx);
}
