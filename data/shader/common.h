#ifdef __cplusplus
#pragma once
#endif

#define PASS_TYPE_ALL           0
#define PASS_TYPE_TERRAIN       1
#define PASS_TYPE_BIOME         2
#define PASS_TYPE_FEATURE       3
#define PASS_TYPE_CONTINENT     4
#define PASS_TYPE_RESOURCE      5

#define PASS_TYPE_MASK          0xF

#define PASS_FLAG_SWAPY         0x10000000
#define PASS_FLAG_HEXAGON       0x20000000
#define PASS_FLAG_OFFSET        0x40000000
#define PASS_FLAG_BORDERS       0x80000000

#define TEXEL_FLAG_WATER_TILE       0x20
#define TEXEL_FLAG_OCEAN_TERRITORY  0x40
#define TEXEL_FLAG_VISIBLE          0x80

#ifndef __cplusplus
uniform int passFlags;
uniform float2 texSize;
uniform float2 mapSize;
uniform float2 screenSize;
uniform float3 hoveredCell;
uniform float2 selectedCell;
uniform float2 selectedRectBegin;
uniform float2 selectedRectEnd;
uniform float2 west;
uniform float2 east;
uniform float4 color;
uniform sampler2D texture;
uniform sampler2D overlay;
uniform float overlayOpacity;
#define inline
#endif

inline float2 getTileUV_Offset(float2 uv, float2 size)
{
    if (0 != (int(uv.y) & 1))
        uv.x -= 0.25f;
    else
        uv.x += 0.25f;
    return uv;
}

inline float2 getTileUV_Hexagon(float2 uv, float2 size)
{
    if (0 != (int(uv.y) & 1))
        uv.x -= 0.25f;
    else
        uv.x += 0.25f;

    int test = 1;

    if (uv.y * size.y <= 0.125)
    {
        test = 0;
        uv.x -= 0.5;
    }

    if (test == (int(uv.y) & 1))
    {
        if (frac(uv.x) <= 0.5 && frac(uv.y) > 0.5f)
        {
            if (frac(uv.x) <= frac(0.5f + uv.y))
            {
                uv.y += 0.5f;
            }
        }
        else if (frac(uv.x) > 0.5f && frac(uv.y) > 0.5f)
        {
            if (frac(uv.x) >= 1.0f - frac(0.5f + uv.y))
            {
                uv.x += 0.5f;
                uv.y += 0.5f;
            }
        }
    }
    else
    {
        if (frac(uv.x) < 0.5f && frac(uv.y) > 0.5f)
        {
            if (frac(uv.x) <= frac(0.5f + uv.y))
            {
                uv.x -= 0.5f;
                uv.y += 0.5f;
            }
        }
        else if (frac(uv.x) > 0.5f && frac(uv.y) > 0.5f)
        {
            if (frac(uv.x) >= 1.0f - frac(0.5f + uv.y))
            {
                uv.y += 0.5;
            }
        }
    }

    return uv;
}

inline float2 getTileUV(float2 uv, float2 size, int flags)
{
    if (0 != (PASS_FLAG_OFFSET & flags))
    {
        uv.x *= size.x + 1.0f;
        uv.y *= size.y;
        uv.x -= 0.25f;
        uv = getTileUV_Offset(uv, size);
    }
    else if (0 != (PASS_FLAG_HEXAGON & flags))
    {
        uv.x *= size.x + 1.0f;
        uv.y *= size.y + 1.0f;
        uv.x -= 0.25f;
        uv.y -= 0.5f;

        uv = getTileUV_Hexagon(uv, size);
    }
    else
    {
        uv.x *= size.x;
        uv.y *= size.y;
    }

    uv.x /= size.x;
    uv.y /= size.y;
    return uv;
}

inline float2 getCellPos(int2 cell)
{
    if (0 != (int(cell.y) & 1))
        return float2(float(cell.x) + 0.5f + 0.25f, float(cell.y) + 0.5f);
    else
        return float2(float(cell.x) + 0.5f - 0.25f, float(cell.y) + 0.5f);
}

inline float cellDist(int2 a, int2 b)
{
    float2 cell = getCellPos(a);
    float2 cursor = getCellPos(b);

    float d = length(float2(cell.x - cursor.x, cell.y - cursor.y));
    return round(d);
}

