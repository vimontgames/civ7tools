#pragma once

//--------------------------------------------------------------------------------------
inline float clamp(float _val, float _min, float _max)
{
    return _val < 0.0f ? 0.0f : (_val > _max ? _max : _val);
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