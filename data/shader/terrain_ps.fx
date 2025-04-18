struct Tile
{
    float4 color0;
    float4 color1;
};

bool isBorder(float2 uv, out bool isWestBorder, out bool isEastBorder, out bool isRectBorder)
{
    isWestBorder = false;
    isEastBorder = false;
    isRectBorder = false;
    
    uv.y = uv.y * 0.5;
    
    float2 center = getTileUV(uv, texSize, passFlags);
    
    float2 invScreenSize = 1.5f / (texSize.xy*16.0f) * float2(0.5f, 0.5f);
    
    float2 left   = getTileUV(uv + float2(-invScreenSize.x, 0), texSize, passFlags);
    float2 right  = getTileUV(uv + float2(+invScreenSize.x, 0), texSize, passFlags);
    float2 bottom = getTileUV(uv + float2(0, -invScreenSize.y), texSize, passFlags);
    float2 up     = getTileUV(uv + float2(0, +invScreenSize.y), texSize, passFlags);
    
    float2 centerCell = float2(center.x * texSize.x, center.y * texSize.y);
    float2 leftCell   = float2(  left.x * texSize.x,   left.y * texSize.y);
    float2 rightCell  = float2( right.x * texSize.x,  right.y * texSize.y);
    float2 bottomCell = float2(bottom.x * texSize.x, bottom.y * texSize.y);
    float2 upCell     = float2    (up.x * texSize.x,     up.y * texSize.y);
    
    if (leftCell.x < west.x && rightCell.x >= west.x || leftCell.x < (west.y + 1) && rightCell.x >= (west.y + 1))
        isWestBorder = true;
    
    if (leftCell.x < east.x && rightCell.x >= east.x || leftCell.x < (east.y + 1) && rightCell.x >= (east.y + 1))
        isEastBorder = true;
        
    ////if ((centerCell.y >= selectedRectBegin.y && centerCell.y <= selectedRectEnd.y + 1))
    //{
    //    if ((leftCell.x < selectedRectBegin.x && rightCell.x > selectedRectBegin.x) || (leftCell.x < selectedRectEnd.x + 1 && rightCell.x > selectedRectEnd.x + 1))
    //        isRectBorder = true;
    //}
    //
    ////if ((centerCell.x >= selectedRectBegin.x && centerCell.x <= selectedRectEnd.x + 1))
    //{
    //    if ((bottomCell.y < selectedRectBegin.y && upCell.y > selectedRectBegin.y) || (bottomCell.y < selectedRectEnd.y + 1 && upCell.y > selectedRectEnd.y + 1))
    //        isRectBorder = true;
    //}
    
    if ((centerCell.y >= selectedRectBegin.y && centerCell.y <= selectedRectEnd.y + 1) && (centerCell.x >= selectedRectBegin.x && centerCell.x <= selectedRectEnd.x + 1))
        isRectBorder = true;
            
    if (floor(left.x * texSize.x)   != floor(center.x * texSize.x) || floor(left.y * texSize.y)   != floor(center.y * texSize.y)
     || floor(right.x * texSize.x)  != floor(center.x * texSize.x) || floor(right.y * texSize.y)  != floor(center.y * texSize.y)
     || floor(bottom.x * texSize.x) != floor(center.x * texSize.x) || floor(bottom.y * texSize.y) != floor(center.y * texSize.y)
     || floor(up.x * texSize.x)     != floor(center.x * texSize.x) || floor(up.y * texSize.y)     != floor(center.y * texSize.y))
        return true;
        
    return false;
}

Tile getTile(float2 uv)
{    
    float offsetY = 0.0f;
    if (0 != (PASS_FLAG_HEXAGON & passFlags))
        offsetY = 0.499f;
    
    float2 uv0 = getTileUV(uv * float2(1.0f, 0.5f) + float2(0.0f, 0.0f), texSize, passFlags);
    float2 uv1 = getTileUV(uv * float2(1.0f, 0.5f) + float2(0.0f, 0.5f - offsetY / texSize.y), texSize, passFlags);
        
    Tile tile;
    tile.color0 = texture2D(texture, uv0);
    tile.color1 = texture2D(texture, uv1);
    return tile;
}

float4 getTileColor(Tile tile)
{    
    float4 color = float4(1.0, 0.0, 1.0, 1.0);
    
    uint terrainType = uint(tile.color0.r * 255.0f);
    bool isWater = false;
    if (terrainType == TerrainType_Ocean || terrainType == TerrainType_Coast)
        isWater = true;
    
    int passIndex = passFlags & PASS_TYPE_MASK;
    switch (passIndex)
    {
        default:
            break;
        
        case PASS_TYPE_ALL:
        {
            uint biomeType = uint(tile.color0.g * 255.0f);
            uint terrainType = uint(tile.color0.r * 255.0f);
            
            color = getBiomeTerrainColor(biomeType, terrainType);
        }
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
            
            if (isWater)
                color.rgb *= float3(0.9f, 0.9f, 1.0f);
        }
        break;
        
        case PASS_TYPE_FEATURE:
        {
            uint featureType = uint(tile.color0.b * 255.0f);
            color = getFeatureColor(featureType);
            
            if (isWater)
                color.rgb *= float3(0.9f, 0.9f, 1.0f);
        }
        break;
        
        case PASS_TYPE_CONTINENT:
        {
            uint continent = uint(tile.color0.a * 255.0f);
            color = getContinentColor(continent);
            
            if (isWater)
                 color.rgb *= float3(0.9f, 0.9f, 1.0f);
        }
        break;
        
        case PASS_TYPE_RESOURCE:
        {
            uint res = uint(tile.color1.r * 255.0f);
            color = getResourceColor(res);
            
            if (isWater)
                 color.rgb *= float3(0.9f, 0.9f, 1.0f);
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
    
    //gl_FragColor = float4(uv.xy, 0, 1);
    //return;
        
    Tile center = getTile(uv);
    float4 color = getTileColor(center);
        
    float2 invScreenSize = 1.5f / (texSize.xy * 16.0f) * float2(1.0f, 0.5f);
        
    Tile left     = getTile(uv + float2(-invScreenSize.x, 0));
    Tile right    = getTile(uv + float2(+invScreenSize.x, 0));
    Tile bottom   = getTile(uv + float2(0, -invScreenSize.y));
    Tile up       = getTile(uv + float2(0, +invScreenSize.y));
    
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
    
    bool selected = false, hovered = false;
    
    float2 tileUV = getTileUV(uv.xy * float2(1, 0.5), texSize, passFlags) * texSize;
    float2 cell = floor(tileUV);
    
    if (cell.x < 0 || cell.x >= texSize.x || cell.y < 0 || cell.y > (texSize.y / 2 - 1))
    {
        discard;
        gl_FragColor = float4(1, 0, 1, 1);
        return;
    }
    
    float4 borderColor = color.rgba * float4(0.9f, 0.9f, 0.9f, 1.0f);
    
    if (cell.x == selectedCell.x && cell.y == selectedCell.y)
    {
        borderColor.rgba = float4(0,1,0, 0.95f); // (color.rgb * 0.2f) + (1.0f - color.rgb) * 0.8f;
        selected = true;
    }
    
    //else if (cell.x == hoveredCell.x && cell.y == hoveredCell.y)
    else if (cellDist(int2(cell.xy), int2(hoveredCell.xy)) < hoveredCell.z && hoveredCell.x != -1 && hoveredCell.y != -1)
    {
        hovered = true;
        borderColor.rgba =  float4(0,10,0, 0.05f); //(color.rgb * 0.8f) + (1.0f - color.rgb) * 0.2f;
    }
    
    //color = lerp(color, float4(uv, 0, 1), 1);
    //color = float4(int(tileUV / texSize.xy) & 1 ? 1 :0, 0, 0, 1);
    
    bool showBorders = (0 != (PASS_FLAG_BORDERS & passFlags));
    bool isWestBorder = false;
    bool isEastBorder = false;
    bool isRectBorder = false;
    
    bool isBorder = isBorder(uv, isWestBorder, isEastBorder, isRectBorder);
    
    if (isBorder)
    {
        if (showBorders || hovered || selected)
        {
            color.rgb = lerp(color.rgb, borderColor.rgb, borderColor.a);
        }
        
        if (isWestBorder && isEastBorder)
        {
            color.rgb = lerp(color.rgb, float3(1, 0.5, 1), 0.75);
        }
        else if (isWestBorder)
        {
            color.rgb = lerp(color.rgb, float3(0, 1, 1), 0.75);
        }
        else if (isEastBorder)
        {
            color.rgb = lerp(color.rgb, float3(1, 0, 0), 0.75);
        }
        
        // select rect (for copy-pasta)
        if (isRectBorder)
            color.rgb = float3(1, 1, 1);
    }
    
    float4 over = texture2D( overlay, float2(uv.x, 1-uv.y));
    //color = float4(uv, 0, 1); //    over;
    color = lerp(color, over, overlayOpacity);
            
    gl_FragColor = float4(color.rgb, 1);   
}