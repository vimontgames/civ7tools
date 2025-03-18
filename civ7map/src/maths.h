#pragma once

//--------------------------------------------------------------------------------------
inline float clamp(float _val, float _min, float _max)
{
    return _val < _min ? _min : (_val > _max ? _max : _val);
}

//--------------------------------------------------------------------------------------
inline int clamp(int _val, int _min, int _max)
{
    return _val < _min ? _min : (_val > _max ? _max : _val);
}

//--------------------------------------------------------------------------------------
inline float saturate(float _val)
{
    return clamp(_val, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------------
inline float frac(float _val)
{
    return _val - floor(_val);
}

//--------------------------------------------------------------------------------------
inline float length(float2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

//--------------------------------------------------------------------------------------
inline float length(float3 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z*v.z);
}