
//--------------------------------------------------------------------------
float3 color_saturation(float3 _color, float _saturation)
{
	float grey = dot(_color.rgb, vec3(0.3, 0.59, 0.11));
	return lerp(grey, _color.rgb, _saturation).rgb;
}

//--------------------------------------------------------------------------
float4 texture_blur(sampler2D _texture, float2 _uv, float2 _size, int _samples, float _scale)
{
	float4 pixel = float4(0, 0, 0, 0);
	float2 invSize = float2(1.0 / _size.x, 1.0 /_size.y);

	for (int j = 0; j < _samples; ++j)
	{
		for (int i = 0; i < _samples; ++i)
		{
			pixel += texture2D(_texture, _uv + float2(i - _samples / 2, j - _samples / 2) * _scale * invSize);
		}
	}
	return pixel / (_samples*_samples);
}

//--------------------------------------------------------------------------
float4 sampleAs3DTexture(sampler2D _texture, float3 _uv, float _width, int _lut, int _count) 
{
	float innerWidth = _width - 1.0;
	float sliceSize = 1.0 / _width; // space of 1 slice
	float slicePixelSize = sliceSize / _width; // space of 1 pixel
	float sliceInnerSize = slicePixelSize * innerWidth; // space of width pixels
	float zSlice0 = min(floor(_uv.z * innerWidth), innerWidth);
	float zSlice1 = min(zSlice0 + 1.0, innerWidth);
	float xOffset = slicePixelSize * 0.5 + _uv.x * sliceInnerSize;
	float s0 = xOffset + (zSlice0 * sliceSize);
	float s1 = xOffset + (zSlice1 * sliceSize);
	float yPixelSize = sliceSize;
	float yOffset = yPixelSize * 0.5 + _uv.y * (1.0 - yPixelSize);
		  yOffset /= _count;
		  yOffset += float(_lut) / float(_count);

	float4 slice0Color = texture2D(_texture, float2(s0, yOffset), 0.0);
	float4 slice1Color = texture2D(_texture, float2(s1, yOffset), 0.0);
	float zOffset = mod(_uv.z * innerWidth, 1.0);
	float4 result = lerp(slice0Color, slice1Color, zOffset);
	return result;
}
