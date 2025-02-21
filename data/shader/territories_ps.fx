
// Should match enum class TerrainType in map.h
#define TerrainType_Moutain         0
#define TerrainType_Hill            1
#define TerrainType_Flat            2
#define TerrainType_Coast           3
#define TerrainType_Ocean           4
#define TerrainType_NavigableRiver  5

// Should match enum class BiomeType in map.h
#define BiomeType_Tundra            0
#define BiomeType_Grassland         1
#define BiomeType_Plains            2
#define BiomeType_Tropical          3
#define BiomeType_Desert            4
#define BiomeType_Marine            5

void main()
{
    float2 uv = gl_TexCoord[0].xy;
    uv.y = 1 - uv.y;

    float4 value = texture2D(texture, hexUV(uv));
    
    int passIndex = passFlags & PASS_TYPE_MASK;
    
    float4 color = float4(0.5, 0.5, 0.5, 1.0);
    
    switch (passIndex)
    {
        default:
            break;

        case PASS_TYPE_TERRAIN:
        {
            uint terrainType = uint(value.r * 255.0f);
            
            switch (terrainType)
            {
                case TerrainType_Moutain:
                    color.rgb = float3(0.9f, 0.9f, 0.9f);
                    break;
                
                case TerrainType_Hill:
                    color.rgb = float3(0.6f, 0.6f, 0.6f);
                    break;
                
                case TerrainType_Flat:
                    color.rgb = float3(0.3f, 0.3f, 0.3f);
                    break;
                
                case TerrainType_Coast:
                    color.rgb = float3(0.0f, 0.5f, 1.0f);
                    break;
                
                case TerrainType_Ocean:
                    color.rgb = float3(0, 0, 1);
                    break;
                
                case TerrainType_NavigableRiver:
                    color.rgb = float3(0, 1, 1);
                    break;
            }
            
            //color = float4(0, 1, 0, 1);
        }
        break;
        
        case PASS_TYPE_BIOME:
        {
            uint biomeType = uint(value.g * 255.0f);
            
            switch (biomeType)
            {
                case BiomeType_Tundra:
                    color.rgb = float3(0.8, 1.0, 0.8);
                    break;
                case BiomeType_Grassland:
                    color.rgb = float3(0.0, 1.0, 0.0);
                    break;
                case BiomeType_Plains:
                    color.rgb = float3(0.5, 0.8, 0.0);
                    break;
                case BiomeType_Tropical:
                    color.rgb = float3(0.0, 0.3, 0.0);
                    break;
                case BiomeType_Desert:
                    color.rgb = float3(1.0, 1.0, 0.0);
                    break;
                case BiomeType_Marine:
                    color.rgb = float3(0, 0, 1);
                    break;
                }
        }
        
        case PASS_TYPE_CONTINENTS:
        //color = float4(1, 0, 0, 1);
        break;
    }
    
    //color.r = pow(color.r, 1.0/2.2);
    //color.g = pow(color.g, 1.0/2.2);
    //color.b = pow(color.b, 1.0/2.2);
    gl_FragColor = float4(color.rgb, 1);
    
    /*float2 invScreenSize = 1.0f / screenSize * 1.5f;
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
        gl_FragColor.a = 0;*/
}