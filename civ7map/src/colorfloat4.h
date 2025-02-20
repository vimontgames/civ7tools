#pragma once

#include "maths.h"

//--------------------------------------------------------------------------------------
struct ColorFloat4
{
    inline ColorFloat4(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f, float _a = 1.0f) :
        r(_r), g(_g), b(_b), a(_a)
    {

    }

    union
    {
        struct { float r, g, b, a; };
        float rgba[4];
    };

    inline sf::Color toColor() const
    {
        return sf::Color
        (
            ubyte(saturate(r) * 255.0f + 0.5f),
            ubyte(saturate(g) * 255.0f + 0.5f),
            ubyte(saturate(b) * 255.0f + 0.5f),
            ubyte(saturate(a) * 255.0f + 0.5f)
        );
    }
};