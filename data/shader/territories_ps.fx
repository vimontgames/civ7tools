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
    
    Tile left     = getTile(uv + float2(-invScreenSize.x, 0));
    Tile right    = getTile(uv + float2(+invScreenSize.x, 0));
    Tile bottom   = getTile(uv + float2(0, -invScreenSize.y));
    Tile up = getTile(uv + float2(0, +invScreenSize.y));
    
    Tile topLeft     = getTile(uv + float2(-invScreenSize.x, +invScreenSize.y));
    Tile topRight    = getTile(uv + float2(+invScreenSize.x, +invScreenSize.y));
    Tile bottomLeft  = getTile(uv + float2(-invScreenSize.x, -invScreenSize.y));
    Tile bottomRight = getTile(uv + float2(+invScreenSize.x, -invScreenSize.y));
    
    bool edge = false;
    
    int passIndex = passFlags & PASS_TYPE_MASK;
    float edgeMul = 1.0f, edgeAdd = 0.0f;
    switch (passIndex)
    {
        //case PASS_TYPE_TERRAIN:
        //    edge = EdgeDetect(center.r, left.r, right.r, bottom.r, up.r, topLeft.r, topRight.r, bottomLeft.r, bottomRight.r);
        //    edgeMul = 1.0f;
        //    edgeAdd = -0.05f;
        //    break;
        //
        //case PASS_TYPE_BIOME:
        //    edge = EdgeDetect(center.g, left.g, right.g, bottom.g, up.g, topLeft.g, topRight.g, bottomLeft.g, bottomRight.g);
        //    edgeMul = 1.0f;
        //    edgeAdd = -0.05f;
        //    break;
        //
        //case PASS_TYPE_FEATURE:
        //    edge = EdgeDetect(center.b, left.b, right.b, bottom.b, up.b, topLeft.b, topRight.b, bottomLeft.b, bottomRight.b);
        //    edgeMul = 1.0f;
        //    edgeAdd = -0.05f;
        //    break;
        
        case PASS_TYPE_CONTINENT:
            edge = EdgeDetect(center.color0.a, left.color0.a, right.color0.a, bottom.color0.a, up.color0.a, topLeft.color0.a, topRight.color0.a, bottomLeft.color0.a, bottomRight.color0.a);
            edgeMul = 1.0f;
            edgeAdd = -0.25f;
            break;
    }
    
    if (edge)
    {
        color.r = color.r * edgeMul + edgeAdd;
        color.g = color.g * edgeMul + edgeAdd;
        color.b = color.b * edgeMul + edgeAdd;
    }
        
    float2 tileUV = (hexUV(uv.xy * float2(1, 0.5)) * texSize);
    
    // wrap poulet
    if (tileUV.x > texSize.x)
        tileUV.x -= (texSize.x);
    else if (tileUV.x < 0)
        tileUV.x += (texSize.x);
    
    float borderSize = 0.1f;
    float4 borderColor = color.rgba;// * float4(0.99f, 0.99f, 0.99f, 1.0f);
    
    float2 cell = floor(tileUV);
    
    if (cell.x == selectedCell.x && cell.y == selectedCell.y)
        borderColor.rgb = (color.rgb * 0.2f) + (1.0f - color.rgb) * 0.8f;
    else if (cell.x == hoveredCell.x && cell.y == hoveredCell.y)
        borderColor.rgb = (color.rgb * 0.8f) + (1.0f - color.rgb) * 0.2f;  
    
    float2 tileUVFract = fract(tileUV);
    if (((abs(tileUVFract.x) < borderSize) || (abs(tileUVFract.x) > (1.0 - borderSize))) || ((abs(tileUVFract.y) < borderSize) || (abs(tileUVFract.y) > (1.0 - borderSize))))
        color.rgb = borderColor.rgb;
        
    gl_FragColor = float4(color.rgb, 1);
}