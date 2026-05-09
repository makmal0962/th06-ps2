#include "GLFunc.hpp"

#include <SDL_video.h>

#ifdef __PS2__
// forward-declare ps2gl symbols without pulling in conflicting typedefs
extern "C" {
    void glAlphaFunc(unsigned int, float);
    void glBindTexture(unsigned int, unsigned int);
    void glBlendFunc(unsigned int, unsigned int);
    void glClear(unsigned int);
    void glClearColor(float, float, float, float);
    // void glClearDepth(float);
    void glColorPointer(int, unsigned int, int, const void*);
    void glDeleteTextures(int, const unsigned int*);
    void glDepthFunc(unsigned int);
    void glDepthMask(unsigned char);
    void glDisable(unsigned int);
    void glDisableClientState(unsigned int);
    void glDrawArrays(unsigned int, int, int);
    void glEnable(unsigned int);
    void glEnableClientState(unsigned int);
    void glFogf(unsigned int, float);
    void glFogfv(unsigned int, const float*);
    void glGenTextures(int, unsigned int*);
    unsigned int glGetError(void);
    void glGetFloatv(unsigned int, float*);
    void glGetIntegerv(unsigned int, int*);
    void glLoadIdentity(void);
    void glLoadMatrixf(const float*);
    void glMatrixMode(unsigned int);
    void glMultMatrixf(const float*);
    void glPopMatrix(void);
    void glPushMatrix(void);
    void glReadPixels(int, int, int, int, unsigned int, unsigned int, void*);
    void glShadeModel(unsigned int);
    void glTexCoordPointer(int, unsigned int, int, const void*);
    void glTexEnvf(unsigned int, unsigned int, float);
    void glTexEnvi(unsigned int, unsigned int, int);
    void glTexImage2D(unsigned int, int, int, int, int, int, unsigned int, unsigned int, const void*);
    void glTexParameteri(unsigned int, unsigned int, int);
    void glTexSubImage2D(unsigned int, int, int, int, int, int, unsigned int, unsigned int, const void*);
    void glVertexPointer(int, unsigned int, int, const void*);
    void glViewport(int, int, int, int);
}
#endif


GLFuncTable g_glFuncTable;

#define TRY_RESOLVE_FUNCTION(func) this->func = (decltype(this->func))SDL_GL_GetProcAddress(#func);
#define TRY_RESOLVE_FUNCTION_GLES(func) this->func##_ptr = (decltype(this->func##_ptr))SDL_GL_GetProcAddress(#func);

void GLFuncTable::ResolveFunctions(bool glesContext)
{
#ifdef __PS2__
    this->glAlphaFunc          = ::glAlphaFunc;
    this->glBindTexture        = ::glBindTexture;
    this->glBlendFunc          = ::glBlendFunc;
    this->glClear              = ::glClear;
    this->glClearColor         = ::glClearColor;
    this->glColorPointer       = ::glColorPointer;
    this->glDeleteTextures     = ::glDeleteTextures;
    this->glDepthFunc          = ::glDepthFunc;
    this->glDepthMask          = ::glDepthMask;
    this->glDisable            = ::glDisable;
    this->glDisableClientState = ::glDisableClientState;
    this->glDrawArrays         = ::glDrawArrays;
    this->glEnable             = ::glEnable;
    this->glEnableClientState  = ::glEnableClientState;
    this->glFogf               = ::glFogf;
    this->glFogfv              = ::glFogfv;
    this->glGenTextures        = ::glGenTextures;
    this->glGetError           = ::glGetError;
    this->glGetFloatv          = ::glGetFloatv;
    this->glGetIntegerv        = ::glGetIntegerv;
    this->glLoadIdentity       = ::glLoadIdentity;
    this->glLoadMatrixf        = ::glLoadMatrixf;
    this->glMatrixMode         = ::glMatrixMode;
    this->glMultMatrixf        = ::glMultMatrixf;
    this->glPopMatrix          = ::glPopMatrix;
    this->glPushMatrix         = ::glPushMatrix;
    this->glReadPixels         = ::glReadPixels;
    this->glShadeModel         = ::glShadeModel;
    this->glTexCoordPointer    = ::glTexCoordPointer;
    this->glTexEnvfv           = [](GLenum t, GLenum p, const GLfloat* v){ ::glTexEnvf(t, p, v[0]); };
    this->glTexEnvi            = ::glTexEnvi;
    this->glTexImage2D         = [](GLenum target, GLint level, GLint internalFormat,
        GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
        const GLvoid* pixels) {
        if (pixels == nullptr) return; // ps2gl crashes with NULL pixels
        ::glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
    };
    this->glTexParameteri      = ::glTexParameteri;
    this->glTexSubImage2D      = ::glTexSubImage2D;
    this->glVertexPointer      = ::glVertexPointer;
    this->glViewport           = ::glViewport;
    this->glClearDepth         = ::glClearDepth;
    this->glDepthRange         = [](GLclampd, GLclampd){};
    this->glAttachShader             = [](GLuint,GLuint){};
    this->glBindAttribLocation       = [](GLuint,GLuint,const char*){};
    this->glCompileShader            = [](GLuint){};
    this->glCreateProgram            = []()->GLuint{ return 0; };
    this->glCreateShader             = [](GLenum)->GLuint{ return 0; };
    this->glDeleteProgram            = [](GLuint){};
    this->glDeleteShader             = [](GLuint){};
    this->glDisableVertexAttribArray = [](GLuint){};
    this->glEnableVertexAttribArray  = [](GLuint){};
    this->glGetProgramInfoLog        = [](GLuint,GLsizei,GLsizei*,char*){};
    this->glGetProgramiv             = [](GLuint,GLenum,GLint*){};
    this->glGetShaderInfoLog         = [](GLuint,GLsizei,GLsizei*,char*){};
    this->glGetShaderiv              = [](GLuint,GLenum,GLint*){};
    this->glGetUniformLocation       = [](GLuint,const char*)->GLint{ return -1; };
    this->glLinkProgram              = [](GLuint){};
    this->glShaderSource             = [](GLuint,GLsizei,const char* const*,const GLint*){};
    this->glUniform1f                = [](GLint,GLfloat){};
    this->glUniform1i                = [](GLint,GLint){};
    this->glUniform4f                = [](GLint,GLfloat,GLfloat,GLfloat,GLfloat){};
    this->glUniformMatrix4fv         = [](GLint,GLsizei,GLboolean,const GLfloat*){};
    this->glUseProgram               = [](GLuint){};
    this->glVertexAttribPointer      = [](GLuint,GLint,GLenum,GLboolean,GLsizei,const void*){};
    this->isGlesContext = false;
    return;
#endif

    TRY_RESOLVE_FUNCTION(glAlphaFunc)
    TRY_RESOLVE_FUNCTION(glBindTexture)
    TRY_RESOLVE_FUNCTION(glBlendFunc)
    TRY_RESOLVE_FUNCTION(glClear)
    TRY_RESOLVE_FUNCTION(glClearColor)
    TRY_RESOLVE_FUNCTION(glColorPointer)
    TRY_RESOLVE_FUNCTION(glDeleteTextures)
    TRY_RESOLVE_FUNCTION(glDepthFunc)
    TRY_RESOLVE_FUNCTION(glDepthMask)
    TRY_RESOLVE_FUNCTION(glDisable)
    TRY_RESOLVE_FUNCTION(glDisableClientState)
    TRY_RESOLVE_FUNCTION(glDrawArrays)
    TRY_RESOLVE_FUNCTION(glEnable)
    TRY_RESOLVE_FUNCTION(glEnableClientState)
    TRY_RESOLVE_FUNCTION(glFogf)
    TRY_RESOLVE_FUNCTION(glFogfv)
    TRY_RESOLVE_FUNCTION(glGenTextures)
    TRY_RESOLVE_FUNCTION(glGetError)
    TRY_RESOLVE_FUNCTION(glGetFloatv)
    TRY_RESOLVE_FUNCTION(glGetIntegerv)
    TRY_RESOLVE_FUNCTION(glLoadIdentity)
    TRY_RESOLVE_FUNCTION(glLoadMatrixf)
    TRY_RESOLVE_FUNCTION(glMatrixMode)
    TRY_RESOLVE_FUNCTION(glMultMatrixf)
    TRY_RESOLVE_FUNCTION(glPopMatrix)
    TRY_RESOLVE_FUNCTION(glPushMatrix)
    TRY_RESOLVE_FUNCTION(glReadPixels)
    TRY_RESOLVE_FUNCTION(glShadeModel)
    TRY_RESOLVE_FUNCTION(glTexCoordPointer)
    TRY_RESOLVE_FUNCTION(glTexEnvfv)
    TRY_RESOLVE_FUNCTION(glTexEnvi)
    TRY_RESOLVE_FUNCTION(glTexImage2D)
    TRY_RESOLVE_FUNCTION(glTexParameteri)
    TRY_RESOLVE_FUNCTION(glTexSubImage2D)
    TRY_RESOLVE_FUNCTION(glVertexPointer)
    TRY_RESOLVE_FUNCTION(glViewport)

    // Ideally, we'd just check for both the regular GL and GLES version of the function and
    //   use whichever doesn't return NULL, but function resolves on GLX are actually context
    //   independent, meaning we can get a valid function pointer that then throws an error
    //   when we call it because the context doesn't actually match what's needed. So instead
    //   we need to pass a parameter to identify which function version to resolve and use.

    if (glesContext)
    {
        TRY_RESOLVE_FUNCTION_GLES(glClearDepthf)
        TRY_RESOLVE_FUNCTION_GLES(glDepthRangef)
    }
    else
    {
        TRY_RESOLVE_FUNCTION(glClearDepth)
        TRY_RESOLVE_FUNCTION(glDepthRange)
    }

    TRY_RESOLVE_FUNCTION(glAttachShader)
    TRY_RESOLVE_FUNCTION(glBindAttribLocation)
    TRY_RESOLVE_FUNCTION(glCompileShader)
    TRY_RESOLVE_FUNCTION(glCreateProgram)
    TRY_RESOLVE_FUNCTION(glCreateShader)
    TRY_RESOLVE_FUNCTION(glDeleteProgram)
    TRY_RESOLVE_FUNCTION(glDeleteShader)
    TRY_RESOLVE_FUNCTION(glDisableVertexAttribArray)
    TRY_RESOLVE_FUNCTION(glEnableVertexAttribArray)
    TRY_RESOLVE_FUNCTION(glGetProgramInfoLog)
    TRY_RESOLVE_FUNCTION(glGetProgramiv)
    TRY_RESOLVE_FUNCTION(glGetShaderInfoLog)
    TRY_RESOLVE_FUNCTION(glGetShaderiv)
    TRY_RESOLVE_FUNCTION(glGetUniformLocation)
    TRY_RESOLVE_FUNCTION(glLinkProgram)
    TRY_RESOLVE_FUNCTION(glShaderSource)
    TRY_RESOLVE_FUNCTION(glUniform1f)
    TRY_RESOLVE_FUNCTION(glUniform1i)
    TRY_RESOLVE_FUNCTION(glUniform4f)
    TRY_RESOLVE_FUNCTION(glUniformMatrix4fv)
    TRY_RESOLVE_FUNCTION(glUseProgram)
    TRY_RESOLVE_FUNCTION(glVertexAttribPointer)

    this->isGlesContext = glesContext;
}

void GLFuncTable::glClearDepthf(GLclampf depth)
{
    if (this->isGlesContext)
    {
        this->glClearDepthf_ptr(depth);
    }
    else
    {
        this->glClearDepth(depth);
    }
}

void GLFuncTable::glDepthRangef(GLclampf near_val, GLclampf far_val)
{
    if (this->isGlesContext)
    {
        this->glDepthRangef_ptr(near_val, far_val);
    }
    else
    {
        this->glDepthRange(near_val, far_val);
    }
}
