#ifndef GL_EXTS_H
#define GL_EXTS_H

#include "glad/glad.h"
#include "gl/glext.h"
#include "gl/glcorearb.h"

#define GL_TEXTURE_MAX_ANISOTROPY              0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY          0x84FF

// GL_ARB_debug_output
extern PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;
extern PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB;
extern PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
extern PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB;

#endif // !GL_EXTS_H
