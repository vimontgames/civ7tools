#version 430 compatibility

#define float2 vec2
#define float3 vec3
#define float4 vec4

#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4

#define int2 ivec2
#define int3 ivec3
#define int4 ivec4

#define lerp(a,b,f) ((1.0f-f)*a + f*b)
#define frac fract

float saturate(float x)
{
    return clamp(x, 0, 1);
}
