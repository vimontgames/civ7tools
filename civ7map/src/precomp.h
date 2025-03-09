#pragma once

#define NOMINMAX
#include <windows.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <locale>
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

#include "magic_enum/include/magic_enum/magic_enum.hpp"

#define FMT_UNICODE 0
#include "fmt/include/fmt/core.h"
#include "fmt/include/fmt/format.h" 
#include "fmt/include/fmt/printf.h"

using i32 = int;
using i8 = signed char;
using u32 = unsigned int;
using ubyte = unsigned char;
using u8 = ubyte;
using uint = unsigned int;

using namespace std;

#include "misc/logger.h"
#include "misc/utils.h"

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

static int   Stricmp(const char * s1, const char * s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char * s1, const char * s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char * Strdup(const char * s) { size_t len = strlen(s) + 1; void * buf = malloc(len); return (char *)memcpy(buf, (const void *)s, len); }
static void  Strtrim(char * s) { char * str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

struct float2
{
    float2(float _x = 0.0f, float _y = 0.0f) :
        x(_x),
        y(_y)
    {

    }

    union
    {
        struct
        {
            float x, y;
        };
        struct
        {
            float r, g;
        };
        float value[2];
    };
};

struct float4
{
    float4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f) :
        x(_x),
        y(_y),
        z(_z),
        w(_w)
    {

    }

    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            float r, g, b, a;
        };
        float value[4];
    };
};

struct int2
{
    int2(int _x = 0, int _y = 0) :
        x(_x),
        y(_y)
    {

    }

    union
    {
        struct
        {
            int x, y;
        };
        int value[2];
    };
};