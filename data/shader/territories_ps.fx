void main()
{
    gl_FragColor.rgba = float4(0,1,2,3);

    float2 invScreenSize = 1.0f / screenSize * 1.5f;

    float2 uv = gl_TexCoord[0].xy;

    float4 center   = texture2D(texture, hexUV(uv));

    float4 left     = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, 0)));
    float4 right    = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, 0)));
    float4 bottom   = texture2D(texture, hexUV(uv + float2(0, -invScreenSize.y)));
    float4 up       = texture2D(texture, hexUV(uv + float2(0, +invScreenSize.y)));

    float4 topLeft     = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, +invScreenSize.y)));
    float4 topRight    = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, +invScreenSize.y)));
    float4 bottomLeft  = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, -invScreenSize.y)));
    float4 bottomRight = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, -invScreenSize.y)));

    bool edge = (0 != (PASS_FLAG_BORDERS & passFlags)) && ((left.a != center.a) || (right.a != center.a) || (bottom.a != center.a) || (up.a != center.a) || (topLeft.a != center.a) || (topRight.a != center.a) || (bottomLeft.a != center.a) || (bottomRight.a != center.a));
   
    float territoryOpacity = 0.4f;
    float edgeOpacity = 0.6f;

    float3 territoryColor[6] =
    {
        float3(1,0,0),
        float3(0,1,0),
        float3(1,1,0),
        float3(1,0,1),
        float3(0,1,1),
        float3(1,1,1)
    };

    float3 biomeColors[10] =
    {
        float3(0.8,0.8,0.9),
        float3(0.6,0.6,0.4),
        float3(1.0,1.0,0.7),
        float3(0.6,0.8,0.3),
        float3(0.8,0.7,0.4),
        float3(1.0,1.0,0.3),
        float3(0.6,1.0,1.0),
        float3(0.5,0.5,0.4),
        float3(0.7,0.8,0.4),
        float3(0.6,0.7,0.6)
    };

    // Desert\0Forest\0Lake\0Mountain\0River
    float3 landmarkColors[5] =
    {
        float3(1,1,0),
        float3(0,1,0),
        float3(0,0,1),
        float3(0.5, 0.25, 0),
        float3(0.5,0.5,1.0)
    };

    uint4 index = uint4(center.rgba * 255.0f);
    
    uint tileIndex = index.g;
    uint biomeIndex = index.g;
    uint landmarkIndex = index.g;
    uint flags = index.b;
    uint territoryIndex = index.a;
    
    float3 color = float3(0,0,0);

    float3 tileColor[15] =
    {
        float3(0.0f,0.0f,0.0f), // unused
        float3(0.3f,0.7f,1.0f), // coastal
        float3(0.6f,0.7f,0.3f), // dry
        float3(0.0f,0.5f,0.0f), // forest
        float3(0.0f,0.4f,0.7f), // lake
        float3(0.5f,0.2f,0.0f), // mountain
        float3(0.9f,0.8f,0.7f), // moutain snow
        float3(0.2f,0.6f,0.9f), // ocean
        float3(0.5f,1.0f,0.0f), // prairie
        float3(0.5f,0.8f,0.3f), // rocky field
        float3(0.3f,0.6f,0.2f), // rocky forest
        float3(0.5f,0.5f,0.5f), // sterile
        float3(0.7f,0.7f,0.7f), // stone field
        float3(0.9f,0.7f,0.1f), // wasteland
        float3(0.4f,0.5f,0.0f)  // woodland
    };
  
    int passIndex = passFlags & PASS_TYPE_MASK;
    switch (passIndex)
    {
        default:
            color = center.ggg;
            break;

        case PASS_TYPE_TILE:
            color = tileColor[tileIndex % 15];
            edgeOpacity = 0.75f;
            break;

        case PASS_TYPE_TERRITORY:
            color = territoryColor[territoryIndex % 6];
            break;
    
        case PASS_TYPE_LANDMARK:
            color = landmarkColors[landmarkIndex % 5];
            break;
    
        case PASS_TYPE_BIOME:
            color = biomeColors[biomeIndex % 10];
            break;

        case PASS_TYPE_WONDER:
            color = float3(1,1,1);
            break;
    }     
            
    float3 edgeColor = float3(1, 1, 1);
    
    bool visible = 0 != (flags & TEXEL_FLAG_VISIBLE);
    bool water = 0 != (flags & TEXEL_FLAG_OCEAN_TERRITORY);
    
    if (water)
    {
        if (PASS_TYPE_TILE != passIndex)
            color = lerp(color, float3(0, 0, 1), 1);

        edgeColor = color * 1.5f;
    }
    else
        edgeColor = color * 2;

    gl_FragColor.rgba = edge ? float4(edgeColor, edgeOpacity) : float4(color, territoryOpacity);

    if (!visible && !edge)
        gl_FragColor.a = 0;
}