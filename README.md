# duktape-opengl
OpenGL bindings for Duktape embeddable Javascript engine

If you ended up here randomly and have no idea what Duktape is, then have a look at here: http://duktape.org/

## Setup
* Include duktape_opengl.c and duktape_opengl.h to your Duktape&OpenGL project.
* To initialize OpenGL bindings, call function: void duk_gl_push_opengl_bindings(duk_context *ctx)

* Currently by default only OpenGL 1.1 bindings will be initialized. If you want to include other bindings, such as ARB extensions, then add defines.
* If you're using OpenGL corearb then use duktape_opengl_corearb.c instead of duktape_opengl.c.

## Defines
* DUK_GL_OPENGL_4X  - Enable OpenGL 4.x and older major versions
* DUK_GL_OPENGL_3X  - Enable OpenGL 3.x and older major versions
* DUK_GL_OPENGL_2X  - Enable OpenGL 2.x and older major versions
* DUK_GL_OPENGL_1X  - Enable OpenGL 1.x and older major versions
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

## Prerequirements
Duktape and OpenGL headers APIs needed for these bindings to be useful.

## Performance
* Using arrays (other than text strings) has overhead as library will copy arrays back and forth in the memory using Duktape API. 

## Currently not supported functions
```C
void glCallLists ( GLsizei n, GLenum type,const GLvoid *lists );
void glVertexPointer ( GLint size, GLenum type,GLsizei stride, const GLvoid *ptr );
void glNormalPointer ( GLenum type, GLsizei stride,const GLvoid *ptr );
void glColorPointer ( GLint size, GLenum type,GLsizei stride, const GLvoid *ptr );
void glIndexPointer ( GLenum type, GLsizei stride,const GLvoid *ptr );
void glTexCoordPointer ( GLint size, GLenum type,GLsizei stride, const GLvoid *ptr );
void glEdgeFlagPointer ( GLsizei stride, const GLvoid *ptr );
void glGetPointerv ( GLenum pname, GLvoid **params );
void glDrawElements ( GLenum mode, GLsizei count,GLenum type, const GLvoid *indices );
void glInterleavedArrays ( GLenum format, GLsizei stride,const GLvoid *pointer );
void glReadPixels ( GLint x, GLint y,GLsizei width, GLsizei height,GLenum format, GLenum type,GLvoid *pixels );
void glDrawPixels ( GLsizei width, GLsizei height,GLenum format, GLenum type,const GLvoid *pixels );
void glTexImage1D ( GLenum target, GLint level,GLint internalFormat,GLsizei width, GLint border,GLenum format, GLenum type,const GLvoid *pixels );
void glTexImage2D ( GLenum target, GLint level,GLint internalFormat,GLsizei width, GLsizei height,GLint border, GLenum format, GLenum type,const GLvoid *pixels );
void glGetTexImage ( GLenum target, GLint level,GLenum format, GLenum type,GLvoid *pixels );
void glTexSubImage1D ( GLenum target, GLint level,GLint xoffset,GLsizei width, GLenum format,GLenum type, const GLvoid *pixels );
void glTexSubImage2D ( GLenum target, GLint level,GLint xoffset, GLint yoffset,GLsizei width, GLsizei height,GLenum format, GLenum type,const GLvoid *pixels );
void glDrawRangeElements ( GLenum mode, GLuint start,GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
void glTexImage3D ( GLenum target, GLint level,GLint internalFormat,GLsizei width, GLsizei height,GLsizei depth, GLint border,GLenum format, GLenum type,const GLvoid *pixels );
void glTexSubImage3D ( GLenum target, GLint level,GLint xoffset, GLint yoffset,GLint zoffset, GLsizei width,GLsizei height, GLsizei depth,GLenum format,GLenum type, const GLvoid *pixels);
void glColorTable ( GLenum target, GLenum internalformat,GLsizei width, GLenum format,GLenum type, const GLvoid *table );
void glColorSubTable ( GLenum target,GLsizei start, GLsizei count,GLenum format, GLenum type,const GLvoid *data );
void glGetColorTable ( GLenum target, GLenum format,GLenum type, GLvoid *table );
void glGetHistogram ( GLenum target, GLboolean reset,GLenum format, GLenum type,GLvoid *values );
void glGetMinmax ( GLenum target, GLboolean reset,GLenum format, GLenum types,GLvoid *values );
void glConvolutionFilter1D ( GLenum target,GLenum internalformat, GLsizei width, GLenum format, GLenum type,const GLvoid *image );
void glConvolutionFilter2D ( GLenum target,GLenum internalformat, GLsizei width, GLsizei height, GLenum format,GLenum type, const GLvoid *image );
void glGetConvolutionFilter ( GLenum target, GLenum format,GLenum type, GLvoid *image );
void glSeparableFilter2D ( GLenum target,GLenum internalformat, GLsizei width, GLsizei height, GLenum format,GLenum type, const GLvoid *row, const GLvoid *column );
void glGetSeparableFilter ( GLenum target, GLenum format,GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );
void glCompressedTexImage1D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexImage2D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexImage3D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage1D ( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage2D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
void glCompressedTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
void glGetCompressedTexImage ( GLenum target, GLint lod, GLvoid *img );
void glTracePointerMESA ( GLvoid* pointer, const GLubyte* comment );
void glTracePointerRangeMESA ( const GLvoid* first, const GLvoid* last, const GLubyte* comment );
void glProgramCallbackMESA ( GLenum target, GLprogramcallbackMESA callback, GLvoid *data);
void glDrawRangeElements ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
void glTexImage3D ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
void glTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void glCompressedTexImage3D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexImage2D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexImage1D ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTexSubImage2D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTexSubImage1D ( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
void glGetCompressedTexImage ( GLenum target, GLint level, void *img);
void glMultiDrawElements ( GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
void glFogCoordPointer ( GLenum type, GLsizei stride, const void *pointer);
void glSecondaryColorPointer ( GLint size, GLenum type, GLsizei stride, const void *pointer);
void glBufferData ( GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void glBufferSubData ( GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
void glGetBufferSubData ( GLenum target, GLintptr offset, GLsizeiptr size, void *data);
void glGetBufferPointerv ( GLenum target, GLenum pname, void **params);
void glGetVertexAttribPointerv ( GLuint index, GLenum pname, void **pointer);
void glVertexAttribPointer ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void glBindBufferRange ( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glVertexAttribIPointer ( GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void glFlushMappedBufferRange ( GLenum target, GLintptr offset, GLsizeiptr length);
void glDrawElementsInstanced ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
void glCopyBufferSubData ( GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void glDrawElementsBaseVertex ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
void glDrawRangeElementsBaseVertex ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
void glDrawElementsInstancedBaseVertex ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
void glMultiDrawElementsBaseVertex ( GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
GLsync glFenceSync ( GLenum condition, GLbitfield flags);
GLboolean glIsSync ( GLsync sync);
void glDeleteSync ( GLsync sync);
GLenum glClientWaitSync ( GLsync sync, GLbitfield flags, GLuint64 timeout);
void glWaitSync ( GLsync sync, GLbitfield flags, GLuint64 timeout);
void glGetSynciv ( GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
void glDrawArraysIndirect ( GLenum mode, const void *indirect);
void glDrawElementsIndirect ( GLenum mode, GLenum type, const void *indirect);
void glShaderBinary ( GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
void glGetProgramBinary ( GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
void glProgramBinary ( GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
void glVertexAttribLPointer ( GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void glDrawElementsInstancedBaseInstance ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
void glDrawElementsInstancedBaseVertexBaseInstance ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
void glClearBufferData ( GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
void glClearBufferSubData ( GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
void glDispatchComputeIndirect ( GLintptr indirect);
void glInvalidateBufferSubData ( GLuint buffer, GLintptr offset, GLsizeiptr length);
void glMultiDrawArraysIndirect ( GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
void glMultiDrawElementsIndirect ( GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
void glTexBufferRange ( GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glBindVertexBuffer ( GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glDebugMessageCallback ( GLDEBUGPROC callback, const void *userParam);
void glObjectPtrLabel ( const void *ptr, GLsizei length, const GLchar *label);
void glGetObjectPtrLabel ( const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
void glBufferStorage ( GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
void glClearTexImage ( GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
void glClearTexSubImage ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
void glBindBuffersRange ( GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
void glBindVertexBuffers ( GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
void glTransformFeedbackBufferRange ( GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glNamedBufferStorage ( GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glNamedBufferData ( GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
void glNamedBufferSubData ( GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void glCopyNamedBufferSubData ( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void glClearNamedBufferData ( GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
void glClearNamedBufferSubData ( GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
void glFlushMappedNamedBufferRange ( GLuint buffer, GLintptr offset, GLsizeiptr length);
void glGetNamedBufferPointerv ( GLuint buffer, GLenum pname, void **params);
void glGetNamedBufferSubData ( GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
void glTextureBufferRange ( GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glTextureSubImage1D ( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
void glTextureSubImage2D ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
void glTextureSubImage3D ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void glCompressedTextureSubImage1D ( GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage2D ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage3D ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
void glGetTextureImage ( GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void glGetCompressedTextureImage ( GLuint texture, GLint level, GLsizei bufSize, void *pixels);
void glVertexArrayVertexBuffer ( GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexArrayVertexBuffers ( GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
void glGetQueryBufferObjecti64v ( GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void glGetQueryBufferObjectiv ( GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void glGetQueryBufferObjectui64v ( GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void glGetQueryBufferObjectuiv ( GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void glGetTextureSubImage ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void glGetCompressedTextureSubImage ( GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
void glGetnCompressedTexImage ( GLenum target, GLint lod, GLsizei bufSize, void *pixels);
void glGetnTexImage ( GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void glReadnPixels ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
void glGetnColorTable ( GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table);
void glGetnConvolutionFilter ( GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image);
void glGetnSeparableFilter ( GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span);
void glGetnHistogram ( GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
void glGetnMinmax ( GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
GLsync glCreateSyncFromCLeventARB ( struct _cl_context *context, struct _cl_event *event, GLbitfield flags);
void glDebugMessageCallbackARB ( GLDEBUGPROCARB callback, const void *userParam);
void glDrawElementsInstancedARB ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);
void glProgramStringARB ( GLenum target, GLenum format, GLsizei len, const void *string);
void glGetProgramStringARB ( GLenum target, GLenum pname, void *string);
void glColorTable ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table);
void glGetColorTable ( GLenum target, GLenum format, GLenum type, void *table);
void glColorSubTable ( GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data);
void glConvolutionFilter1D ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image);
void glConvolutionFilter2D ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image);
void glGetConvolutionFilter ( GLenum target, GLenum format, GLenum type, void *image);
void glGetSeparableFilter ( GLenum target, GLenum format, GLenum type, void *row, void *column, void *span);
void glSeparableFilter2D ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column);
void glGetHistogram ( GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
void glGetMinmax ( GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
void glMultiDrawArraysIndirectCountARB ( GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void glMultiDrawElementsIndirectCountARB ( GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void glMatrixIndexPointerARB ( GLint size, GLenum type, GLsizei stride, const void *pointer);
void glGetnTexImageARB ( GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *img);
void glReadnPixelsARB ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
void glGetnCompressedTexImageARB ( GLenum target, GLint lod, GLsizei bufSize, void *img);
void glGetnColorTableARB ( GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table);
void glGetnConvolutionFilterARB ( GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image);
void glGetnSeparableFilterARB ( GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span);
void glGetnHistogramARB ( GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
void glGetnMinmaxARB ( GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
void glDeleteObjectARB ( GLhandleARB obj);
GLhandleARB glGetHandleARB ( GLenum pname);
void glDetachObjectARB ( GLhandleARB containerObj, GLhandleARB attachedObj);
GLhandleARB glCreateShaderObjectARB ( GLenum shaderType);
void glShaderSourceARB ( GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
void glCompileShaderARB ( GLhandleARB shaderObj);
GLhandleARB glCreateProgramObjectARB ( void);
void glAttachObjectARB ( GLhandleARB containerObj, GLhandleARB obj);
void glLinkProgramARB ( GLhandleARB programObj);
void glUseProgramObjectARB ( GLhandleARB programObj);
void glValidateProgramARB ( GLhandleARB programObj);
void glGetObjectParameterfvARB ( GLhandleARB obj, GLenum pname, GLfloat *params);
void glGetObjectParameterivARB ( GLhandleARB obj, GLenum pname, GLint *params);
void glGetInfoLogARB ( GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
void glGetAttachedObjectsARB ( GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
GLint glGetUniformLocationARB ( GLhandleARB programObj, const GLcharARB *name);
void glGetActiveUniformARB ( GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
void glGetUniformfvARB ( GLhandleARB programObj, GLint location, GLfloat *params);
void glGetUniformivARB ( GLhandleARB programObj, GLint location, GLint *params);
void glGetShaderSourceARB ( GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
void glBufferPageCommitmentARB ( GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
void glNamedBufferPageCommitmentEXT ( GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
void glNamedBufferPageCommitmentARB ( GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
void glCompressedTexImage3DARB ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexImage2DARB ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexImage1DARB ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexSubImage3DARB ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTexSubImage2DARB ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
void glCompressedTexSubImage1DARB ( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
void glGetCompressedTexImageARB ( GLenum target, GLint level, void *img);
void glWeightPointerARB ( GLint size, GLenum type, GLsizei stride, const void *pointer);
void glBufferDataARB ( GLenum target, GLsizeiptrARB size, const void *data, GLenum usage);
void glBufferSubDataARB ( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void *data);
void glGetBufferSubDataARB ( GLenum target, GLintptrARB offset, GLsizeiptrARB size, void *data);
void glGetBufferPointervARB ( GLenum target, GLenum pname, void **params);
void glVertexAttribPointerARB ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void glGetVertexAttribPointervARB ( GLuint index, GLenum pname, void **pointer);
void glBindAttribLocationARB ( GLhandleARB programObj, GLuint index, const GLcharARB *name);
void glGetActiveAttribARB ( GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLint glGetAttribLocationARB ( GLhandleARB programObj, const GLcharARB *name);
void glDebugMessageCallbackAMD ( GLDEBUGPROCAMD callback, void *userParam);
void glMultiDrawArraysIndirectAMD ( GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride);
void glMultiDrawElementsIndirectAMD ( GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride);
void glGetPerfMonitorCounterInfoAMD ( GLuint group, GLuint counter, GLenum pname, void *data);
void glElementPointerAPPLE ( GLenum type, const void *pointer);
void glFlushMappedBufferRangeAPPLE ( GLenum target, GLintptr offset, GLsizeiptr size);
void glTextureRangeAPPLE ( GLenum target, GLsizei length, const void *pointer);
void glGetTexParameterPointervAPPLE ( GLenum target, GLenum pname, void **params);
void glVertexArrayRangeAPPLE ( GLsizei length, void *pointer);
void glFlushVertexArrayRangeAPPLE ( GLsizei length, void *pointer);
void glElementPointerATI ( GLenum type, const void *pointer);
GLuint glNewObjectBufferATI ( GLsizei size, const void *pointer, GLenum usage);
void glUpdateObjectBufferATI ( GLuint buffer, GLuint offset, GLsizei size, const void *pointer, GLenum preserve);
GLintptr glGetUniformOffsetEXT ( GLuint program, GLint location);
void glColorSubTableEXT ( GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data);
void glConvolutionFilter1DEXT ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image);
void glConvolutionFilter2DEXT ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image);
void glGetConvolutionFilterEXT ( GLenum target, GLenum format, GLenum type, void *image);
void glGetSeparableFilterEXT ( GLenum target, GLenum format, GLenum type, void *row, void *column, void *span);
void glSeparableFilter2DEXT ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column);
void glTangentPointerEXT ( GLenum type, GLsizei stride, const void *pointer);
void glBinormalPointerEXT ( GLenum type, GLsizei stride, const void *pointer);
void glTextureImage1DEXT ( GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
void glTextureImage2DEXT ( GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void glTextureSubImage1DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
void glTextureSubImage2DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
void glGetTextureImageEXT ( GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
void glTextureImage3DEXT ( GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
void glTextureSubImage3DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void glMultiTexCoordPointerEXT ( GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer);
void glMultiTexImage1DEXT ( GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
void glMultiTexImage2DEXT ( GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void glMultiTexSubImage1DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
void glMultiTexSubImage2DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
void glGetMultiTexImageEXT ( GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
void glMultiTexImage3DEXT ( GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
void glMultiTexSubImage3DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void glGetPointerIndexedvEXT ( GLenum target, GLuint index, void **data);
void glCompressedTextureImage3DEXT ( GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits);
void glCompressedTextureImage2DEXT ( GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits);
void glCompressedTextureImage1DEXT ( GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits);
void glCompressedTextureSubImage3DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits);
void glCompressedTextureSubImage2DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits);
void glCompressedTextureSubImage1DEXT ( GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits);
void glGetCompressedTextureImageEXT ( GLuint texture, GLenum target, GLint lod, void *img);
void glCompressedMultiTexImage3DEXT ( GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits);
void glCompressedMultiTexImage2DEXT ( GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits);
void glCompressedMultiTexImage1DEXT ( GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits);
void glCompressedMultiTexSubImage3DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits);
void glCompressedMultiTexSubImage2DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits);
void glCompressedMultiTexSubImage1DEXT ( GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits);
void glGetCompressedMultiTexImageEXT ( GLenum texunit, GLenum target, GLint lod, void *img);
void glNamedBufferDataEXT ( GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
void glNamedBufferSubDataEXT ( GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void glGetNamedBufferPointervEXT ( GLuint buffer, GLenum pname, void **params);
void glGetNamedBufferSubDataEXT ( GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
void glGetPointeri_vEXT ( GLenum pname, GLuint index, void **params);
void glNamedProgramStringEXT ( GLuint program, GLenum target, GLenum format, GLsizei len, const void *string);
void glGetNamedProgramStringEXT ( GLuint program, GLenum target, GLenum pname, void *string);
void glNamedCopyBufferSubDataEXT ( GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void glVertexArrayVertexOffsetEXT ( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayColorOffsetEXT ( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayEdgeFlagOffsetEXT ( GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
void glVertexArrayIndexOffsetEXT ( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayNormalOffsetEXT ( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayTexCoordOffsetEXT ( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayMultiTexCoordOffsetEXT ( GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayFogCoordOffsetEXT ( GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArraySecondaryColorOffsetEXT ( GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glVertexArrayVertexAttribOffsetEXT ( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
void glVertexArrayVertexAttribIOffsetEXT ( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glGetVertexArrayPointervEXT ( GLuint vaobj, GLenum pname, void **param);
void glGetVertexArrayPointeri_vEXT ( GLuint vaobj, GLuint index, GLenum pname, void **param);
void glFlushMappedNamedBufferRangeEXT ( GLuint buffer, GLintptr offset, GLsizeiptr length);
void glNamedBufferStorageEXT ( GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glClearNamedBufferDataEXT ( GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
void glClearNamedBufferSubDataEXT ( GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
void glTextureBufferRangeEXT ( GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glVertexArrayBindVertexBufferEXT ( GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexArrayVertexAttribLOffsetEXT ( GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
void glDrawElementsInstancedEXT ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);
void glDrawRangeElementsEXT ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
void glFogCoordPointerEXT ( GLenum type, GLsizei stride, const void *pointer);
void glGetHistogramEXT ( GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
void glGetMinmaxEXT ( GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
void glMultiDrawElementsEXT ( GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount);
void glColorTableEXT ( GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *table);
void glGetColorTableEXT ( GLenum target, GLenum format, GLenum type, void *data);
void glSecondaryColorPointerEXT ( GLint size, GLenum type, GLsizei stride, const void *pointer);
void glTexSubImage1DEXT ( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
void glTexSubImage2DEXT ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
void glTexImage3DEXT ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
void glTexSubImage3DEXT ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void glBindBufferRangeEXT ( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glBindBufferOffsetEXT ( GLenum target, GLuint index, GLuint buffer, GLintptr offset);
void glColorPointerEXT ( GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);
void glGetPointervEXT ( GLenum pname, void **params);
void glIndexPointerEXT ( GLenum type, GLsizei stride, GLsizei count, const void *pointer);
void glNormalPointerEXT ( GLenum type, GLsizei stride, GLsizei count, const void *pointer);
void glTexCoordPointerEXT ( GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);
void glVertexPointerEXT ( GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);
void glVertexAttribLPointerEXT ( GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void glSetInvariantEXT ( GLuint id, GLenum type, const void *addr);
void glSetLocalConstantEXT ( GLuint id, GLenum type, const void *addr);
void glVariantPointerEXT ( GLuint id, GLenum type, GLuint stride, const void *addr);
void glGetVariantPointervEXT ( GLuint id, GLenum value, void **data);
void glVertexWeightPointerEXT ( GLint size, GLenum type, GLsizei stride, const void *pointer);
GLsync glImportSyncEXT ( GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);
void glStringMarkerGREMEDY ( GLsizei len, const void *string);
void glMultiModeDrawElementsIBM ( const GLenum *mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei primcount, GLint modestride);
void glColorPointerListIBM ( GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glSecondaryColorPointerListIBM ( GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glEdgeFlagPointerListIBM ( GLint stride, const GLboolean **pointer, GLint ptrstride);
void glFogCoordPointerListIBM ( GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glIndexPointerListIBM ( GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glNormalPointerListIBM ( GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glTexCoordPointerListIBM ( GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glVertexPointerListIBM ( GLint size, GLenum type, GLint stride, const void **pointer, GLint ptrstride);
void glVertexPointervINTEL ( GLint size, GLenum type, const void **pointer);
void glNormalPointervINTEL ( GLenum type, const void **pointer);
void glColorPointervINTEL ( GLint size, GLenum type, const void **pointer);
void glTexCoordPointervINTEL ( GLint size, GLenum type, const void **pointer);
void glGetPerfQueryDataINTEL ( GLuint queryHandle, GLuint flags, GLsizei dataSize, GLvoid *data, GLuint *bytesWritten);
void glMultiDrawArraysIndirectBindlessNV ( GLenum mode, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
void glMultiDrawElementsIndirectBindlessNV ( GLenum mode, GLenum type, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
void glMultiDrawArraysIndirectBindlessCountNV ( GLenum mode, const void *indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount);
void glMultiDrawElementsIndirectBindlessCountNV ( GLenum mode, GLenum type, const void *indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount);
void glDrawCommandsNV ( GLenum primitiveMode, GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, GLuint count);
void glDrawCommandsStatesNV ( GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count);
void glListDrawCommandsStatesClientNV ( GLuint list, GLuint segment, const void **indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count);
void glMapControlPointsNV ( GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void *points);
void glGetMapControlPointsNV ( GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void *points);
void glPathCommandsNV ( GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords);
void glPathCoordsNV ( GLuint path, GLsizei numCoords, GLenum coordType, const void *coords);
void glPathSubCommandsNV ( GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const void *coords);
void glPathSubCoordsNV ( GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void *coords);
void glPathStringNV ( GLuint path, GLenum format, GLsizei length, const void *pathString);
void glPathGlyphsNV ( GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const void *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
void glPathGlyphRangeNV ( GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
void glStencilFillPathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
void glStencilStrokePathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
void glCoverFillPathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
void glCoverStrokePathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
void glGetPathMetricsNV ( GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
void glGetPathSpacingNV ( GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
void glStencilThenCoverFillPathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
void glStencilThenCoverStrokePathInstancedNV ( GLsizei numPaths, GLenum pathNameType, const void *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
GLenum glPathGlyphIndexArrayNV ( GLuint firstPathName, GLenum fontTarget, const void *fontName, GLbitfield fontStyle, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
GLenum glPathMemoryGlyphIndexArrayNV ( GLuint firstPathName, GLenum fontTarget, GLsizeiptr fontSize, const void *fontData, GLsizei faceIndex, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
void glPixelDataRangeNV ( GLenum target, GLsizei length, const void *pointer);
void glBindBufferRangeNV ( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glBindBufferOffsetNV ( GLenum target, GLuint index, GLuint buffer, GLintptr offset);
void glVDPAUInitNV ( const void *vdpDevice, const void *getProcAddress);
GLvdpauSurfaceNV glVDPAURegisterVideoSurfaceNV ( const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLvdpauSurfaceNV glVDPAURegisterOutputSurfaceNV ( const void *vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
GLboolean glVDPAUIsSurfaceNV ( GLvdpauSurfaceNV surface);
void glVDPAUUnregisterSurfaceNV ( GLvdpauSurfaceNV surface);
void glVDPAUGetSurfaceivNV ( GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
void glVDPAUSurfaceAccessNV ( GLvdpauSurfaceNV surface, GLenum access);
void glVDPAUMapSurfacesNV ( GLsizei numSurfaces, const GLvdpauSurfaceNV *surfaces);
void glVDPAUUnmapSurfacesNV ( GLsizei numSurface, const GLvdpauSurfaceNV *surfaces);
void glVertexArrayRangeNV ( GLsizei length, const void *pointer);
void glBufferAddressRangeNV ( GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
void glGetVertexAttribPointervNV ( GLuint index, GLenum pname, void **pointer);
void glVertexAttribPointerNV ( GLuint index, GLint fsize, GLenum type, GLsizei stride, const void *pointer);
void glVertexAttribIPointerEXT ( GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void glBindVideoCaptureStreamBufferNV ( GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
void glTexImage4DSGIS ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const void *pixels);
void glTexSubImage4DSGIS ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const void *pixels);
void glIglooInterfaceSGIX ( GLenum pname, const void *params);
void glColorTableSGI ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table);
void glGetColorTableSGI ( GLenum target, GLenum format, GLenum type, void *table);
void glReplacementCodePointerSUN ( GLenum type, GLsizei stride, const void **pointer);
```

## Greetings
Greetings fly out to the authors of the Duktape API. Splendid work!
