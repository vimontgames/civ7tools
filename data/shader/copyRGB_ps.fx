void main()
{
    float2 uv = gl_TexCoord[0].xy;
    float4 tex = texture2D(texture, uv);
    float gamma = 1.0f; // 2.2f;
    gl_FragColor = float4(pow(tex.rgb, float3(gamma, gamma, gamma)), 1);
}