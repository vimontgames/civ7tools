#ifdef __cplusplus
#pragma once
#endif

#define PASS_TYPE_TERRAIN       0
#define PASS_TYPE_BIOME         1
#define PASS_TYPE_FEATURE       2
#define PASS_TYPE_CONTINENT    3
#define PASS_TYPE_RESOURCE      4

#define PASS_TYPE_MASK          0xF

#define PASS_FLAG_BORDERS   0x40000000
#define PASS_FLAG_HEXES     0x80000000

#define TEXEL_FLAG_WATER_TILE       0x20
#define TEXEL_FLAG_OCEAN_TERRITORY  0x40
#define TEXEL_FLAG_VISIBLE          0x80

#ifndef __cplusplus
uniform int passFlags;
uniform float2 texSize;
uniform float2 screenSize;
uniform float2 hoveredCell;
uniform float2 selectedCell;
uniform float4 color;
uniform sampler2D texture;

float2 hexUV(float2 uv)
{
    if (0 != (PASS_FLAG_HEXES & passFlags))
    {
        if (0 != (int(uv.y * texSize.y) & 1))
            uv.x -= 0.25f / texSize.x;
        else
            uv.x += 0.25f / texSize.x;
    }
    return uv;
}
#endif

