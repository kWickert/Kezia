#pragma once

#ifdef _WIN32
	#include <Gl/glew.h>
	#include <gl/GL.h>
#endif

#ifdef __ANDROID__
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif
