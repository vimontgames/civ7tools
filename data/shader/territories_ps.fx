struct Tile
{
    float4 color0;
    float4 color1;
};

Tile getTile(float2 uv)
{
    Tile tile;
    tile.color0 = texture2D(texture, hexUV(uv * float2(1,0.5) + float2(0,0) ));
    tile.color1 = texture2D(texture, hexUV(uv * float2(1,0.5) + float2(0,0.5) ));
    return tile;
}

float4 getTileColor(Tile tile)
{    
    float4 color = float4(1.0, 0.0, 1.0, 1.0);
    
    int passIndex = passFlags & PASS_TYPE_MASK;
    switch (passIndex)
    {
        default:
            break;

        case PASS_TYPE_TERRAIN:
        {
            uint terrainType = uint(tile.color0.r * 255.0f);
            color = getTerrainColor(terrainType);
        }
        break;
        
        case PASS_TYPE_BIOME:
        {
            uint biomeType = uint(tile.color0.g * 255.0f);
            color = getBiomeColor(biomeType);
        }
        break;
        
        case PASS_TYPE_FEATURE:
        {
            uint featureType = uint(tile.color0.b * 255.0f);
            color = getFeatureColor(featureType);
        }
        break;
        
        case PASS_TYPE_CONTINENT:
        {
            uint continent = uint(tile.color0.a * 255.0f);
            color = getContinentColor(continent);
        }
        break;
        
        case PASS_TYPE_RESOURCE:
        {
            uint res = uint(tile.color1.r * 255.0f);
            color = getResourceColor(res);
        }
        break;
    }
    
    return color;
}

bool EdgeDetect(float center, float left, float right, float bottom, float up, float topLeft, float topRight, float bottomLeft, float bottomRight)
{
    return (left != center) || (right != center) || (bottom != center) || (up != center) || (topLeft != center) || (topRight != center) || (bottomLeft != center) || (bottomRight != center);       
}

void main()
{
    float2 uv = gl_TexCoord[0].xy;
    uv.y = 1 - uv.y;
    
    Tile center = getTile(uv);
    float4 color = getTileColor(center);
        
    float2 invScreenSize = 1.0f / screenSize.xy * 2.0f;
    
    //float4 left     = getTile(uv + float2(-invScreenSize.x, 0));
    //float4 right    = getTile(uv + float2(+invScreenSize.x, 0));
    //float4 bottom   = getTile(uv + float2(0, -invScreenSize.y));
    //float4 up       = getTile(uv + float2(0, +invScreenSize.y));
    //
    //float4 topLeft     = getTile(uv + float2(-invScreenSize.x, +invScreenSize.y));
    //float4 topRight    = getTile(uv + float2(+invScreenSize.x, +invScreenSize.y));
    //float4 bottomLeft  = getTile(uv + float2(-invScreenSize.x, -invScreenSize.y));
    //float4 bottomRight = getTile(uv + float2(+invScreenSize.x, -invScreenSize.y));
    
    //bool edge = false;
    //
    //int passIndex = passFlags & PASS_TYPE_MASK;
    //float edgeMul = 1.0f, edgeAdd = 0.0f;
    //switch (passIndex)
    //{
    //    case PASS_TYPE_TERRAIN:
    //        edge = EdgeDetect(center.r, left.r, right.r, bottom.r, up.r, topLeft.r, topRight.r, bottomLeft.r, bottomRight.r);
    //        edgeMul = 1.0f;
    //        edgeAdd = -0.05f;
    //        break;
    //    
    //    case PASS_TYPE_BIOME:
    //        edge = EdgeDetect(center.g, left.g, right.g, bottom.g, up.g, topLeft.g, topRight.g, bottomLeft.g, bottomRight.g);
    //        edgeMul = 1.0f;
    //        edgeAdd = -0.05f;
    //        break;
    //    
    //    case PASS_TYPE_FEATURE:
    //        edge = EdgeDetect(center.b, left.b, right.b, bottom.b, up.b, topLeft.b, topRight.b, bottomLeft.b, bottomRight.b);
    //        edgeMul = 1.0f;
    //        edgeAdd = -0.05f;
    //        break;
    //    
    //    case PASS_TYPE_CONTINENT:
    //        edge = EdgeDetect(center.a, left.a, right.a, bottom.a, up.a, topLeft.a, topRight.a, bottomLeft.a, bottomRight.a);
    //        edgeMul = 1.0f;
    //        edgeAdd = -0.25f;
    //        break;
    //}
    //
    //if (edge)
    //{
    //    color.r = color.r * edgeMul + edgeAdd;
    //    color.g = color.g * edgeMul + edgeAdd;
    //    color.b = color.b * edgeMul + edgeAdd;
    //}
        
    gl_FragColor = float4(color.rgb, 1);
    
    /*
    float4 left     = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, 0)));
    float4 right    = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, 0)));
    float4 bottom   = texture2D(texture, hexUV(uv + float2(0, -invScreenSize.y)));
    float4 up       = texture2D(texture, hexUV(uv + float2(0, +invScreenSize.y)));

    float4 topLeft     = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, +invScreenSize.y)));
    float4 topRight    = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, +invScreenSize.y)));
    float4 bottomLeft  = texture2D(texture, hexUV(uv + float2(-invScreenSize.x, -invScreenSize.y)));
    float4 bottomRight = texture2D(texture, hexUV(uv + float2(+invScreenSize.x, -invScreenSize.y)));

    
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
        gl_FragColor.a = 0;*/
}