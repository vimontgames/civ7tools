#pragma once

#define NOMINMAX
#include <windows.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdlib.h> 

#include <SFML/System/Clock.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/BlendMode.hpp>

using i32 = int;
using u32 = unsigned int;
using ubyte = unsigned char;

inline void debugPrint(const char * _format, ...)
{
    va_list args;
    va_start(args, _format);

    char buffer[4096];
    vsnprintf(buffer, sizeof(buffer), _format, args);
    ::OutputDebugStringA(buffer);

    va_end(args);
}

#define SAFE_FREE(p) { if (p) {free(p); p = nullptr; }}
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; }}