#pragma once

#include <GL/glew.h>

// assertion for msvc compiler only (debugbreak)
#define ASSERT(x) if (!(x)) __debugbreak();
// checking if application is running in debug or release mode
// if in release, the error loggin system should not work
#ifdef _DEBUG
    // #x transforms x in string
#define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);