void main()
{
    float2 uv = gl_TexCoord[0].xy;
    float4 tex = texture2D(texture, hexUV(uv));

    uint4 index = uint4(tex.rgba * 255.0f);
    uint flags = index.b;

    bool waterTile = 0 != (flags & TEXEL_FLAG_WATER_TILE);

    float greyscale = float(index.a) / 15.0f;

    if (waterTile)
        gl_FragColor.rgba = float4(float3(0.5,0.5,2) * greyscale.xxx,1);// float4(0, 0, height.x, 1.0f);
    else
        gl_FragColor.rgba = float4(greyscale.xxx, 1);// float4(height.xxx, 1.0f);

    //gl_FragColor.rgba = float4(0, 0, 0, 1);
}