void main()
{
    float2 invTexSize = 1.0f / texSize;

    float2 uv = gl_TexCoord[0].xy;

    uv.xy += invTexSize * float2(-1,0);

    float4 center = texture2D(texture, uv);

    float4 left     = texture2D(texture, uv + float2(-invTexSize.x, 0));
    float4 right     = texture2D(texture, uv + float2(+invTexSize.x, 0));
    float4 bottom    = texture2D(texture, uv + float2(0, -invTexSize.y));
    float4 up        = texture2D(texture, uv + float2(0, +invTexSize.y));

    float4 topLeft     = texture2D(texture, uv + float2(-invTexSize.x, +invTexSize.y));
    float4 topRight    = texture2D(texture, uv + float2(+invTexSize.x, +invTexSize.y));
    float4 bottomLeft  = texture2D(texture, uv + float2(-invTexSize.x, -invTexSize.y));
    float4 bottomRight = texture2D(texture, uv + float2(+invTexSize.x, -invTexSize.y));

    bool edge = (left.r > 0.5 != center.r > 0.5) || (right.r > 0.5 != center.r > 0.5) || (bottom.r > 0.5 != center.r > 0.5) || (up.r > 0.5 != center.r > 0.5);

    gl_FragColor.rgba = float4(1, 1, 1, right.r) * color;
}