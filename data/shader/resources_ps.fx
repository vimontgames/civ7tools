void main()
{
    float2 invTexSize = 1.0f / texSize;

    float2 uv = gl_TexCoord[0].xy;

    uv.xy += invTexSize * float2(-1,0);

    float4 center = texture2D(texture, uv);
    
    gl_FragColor.rgba = center * color;
}