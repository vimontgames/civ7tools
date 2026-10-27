#include "shader/colors.h"

//--------------------------------------------------------------------------------------
void Map::initTerrainInfos(bool _reload)
{
    for (auto val : enumValues<TerrainType>())
    {
        int index = (int)val.first;
        ASSERT(index >= 0);
        TerrainInfo & info = m_terrainInfos[index];
        info.count = 0;
    }
}

//--------------------------------------------------------------------------------------
void Map::initBiomeInfos(bool _reload)
{
    for (auto val : enumValues<BiomeType>())
    {
        int index = (int)val.first;
        ASSERT(index >= 0);
        BiomeInfo & info = m_biomeInfos[index];
        info.count = 0;
    }
}

//--------------------------------------------------------------------------------------
void Map::initBiomeTerrainInfos(bool _reload)
{
    for (auto val : enumValues<BiomeType>())
    {
        int biomeIndex = (int)val.first;
        if (biomeIndex >= 0)
        {
            for (auto val2 : enumValues<TerrainType>())
            {
                int terrainIndex = (int)val2.first;
                if (terrainIndex >= 0)
                {
                    BiomeTerrainInfo & info = m_biomeTerrainInfos[biomeIndex][terrainIndex];
                    info.count = 0;
                }
            }
        }
    }
}



//--------------------------------------------------------------------------------------
void Map::initResourceInfos(bool _reload)
{
    for (auto val : enumValues<ResourceType>())
    {
        int index = (int)val.first;
        if (index >= 0)
        {
            ResourceInfo & info = m_resourceInfos[index];
            info.count = 0;
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::initFeatureInfos(bool _reload)
{
    for (auto val : enumValues<FeatureType>())
    {
        int index = (int)val.first;
        if (index >= 0)
        {
            FeatureInfo & info = m_featureInfos[index];
            info.count = 0;
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::initContinentInfos(bool _reload)
{
    for (uint i = 0; i < m_continents.size(); ++i)
    {
        m_continents[i].count = 0;
    }
}

//--------------------------------------------------------------------------------------
void Map::initLandmassInfos(bool _reload)
{
    for (uint i = 0; i < m_landmasses.size(); ++i)
    {
        m_landmasses[i].count = 0;
    }
}

//--------------------------------------------------------------------------------------
void Map::initNaturalWonderInfos(bool _reload)
{
    for (uint i = 0; i < enumCount<NaturalWonderType>(); ++i)
    {
        m_naturalWonderInfos[i].count = 0;
    }
}

//--------------------------------------------------------------------------------------
void Map::refresh(bool _reload)
{
    if (!m_isLoaded)
        return;

    initTerrainInfos(m_firstRefresh);
    initBiomeInfos(m_firstRefresh);
    initBiomeTerrainInfos(m_firstRefresh);
    initResourceInfos(m_firstRefresh);
    initFeatureInfos(m_firstRefresh);
    initContinentInfos(m_firstRefresh);
    initLandmassInfos(m_firstRefresh);
    initNaturalWonderInfos(m_firstRefresh);

    if (m_showRivers)
        UpdateRivers(false);

    if (m_firstRefresh)
        m_firstRefresh = true;

    // Refresh textures
    if (_reload)
    {
        loadIcons(_reload);
    }

    createBitmaps();

    Bitmap & terrain = m_bitmaps[(int)MapBitmap::TerrainData];

    extern u32 g_screenWidth;
    extern u32 g_screenHeight;

    Vector2f scale = Vector2f(float(g_screenWidth) / float(m_width), float(g_screenHeight) / float(m_height));
    float ar =(float(g_screenWidth) / float(m_width)) / (float(g_screenHeight) / float(m_height));
    scale.y *= ar;

    auto & resources = m_bitmaps[(int)MapBitmap::Resources];
    auto & features = m_bitmaps[(int)MapBitmap::Features];
    auto & naturalWonders = m_bitmaps[(int)MapBitmap::NaturalWonder];
    auto & rivers = m_bitmaps[(int)MapBitmap::Rivers];

    for (auto & bitmap : m_bitmaps)
        bitmap.sprites.clear();

    vector<int2> riverErrors;

    float resourceAlpha = 1.0f;
    float featureAlpha = 1.0f;
    float riverAlpha = 1.0f;

    switch (m_mapFilter)
    {
        case MapFilter::NaturalWonder:
            resourceAlpha = 0.25f;
            featureAlpha = 0.25f;
            riverAlpha = 0.25f;
            break;
    }

    for (u32 h = 0; h < m_height; ++h)
    {
        for (u32 w = 0; w < m_width; ++w)
        {
            const u32 offset = w + h * m_width;

            const Civ7Tile & tile = m_civ7TerrainType.get(w, h);

            Color color0 = Color(0, 0, 0, 0);

            // Color0.r is terrain type
            color0.r = (u8)tile.terrain;

            getTerrainInfo(tile.terrain).count++;

            // Color0.g is biome
            color0.g = (u8)tile.biome;
            getBiomeInfo(tile.biome).count++;

            // stats
            getBiomeTerrainInfo(tile.biome, tile.terrain).count++;

            // Color0.b is feature
            color0.b = (u8)tile.feature;
            getFeatureInfo(tile.feature).count++;

            // Count continents and landmasses
            if (tile.continent != ContinentType::None && (int)tile.continent < m_continents.size())
                getContinentInfo(tile.continent).count++;

            if ((int)tile.landmass < m_landmasses.size())
                getLandmassInfo(tile.landmass).count++;

            if (m_showFeatures)
            {
                if (FeatureType::None != tile.feature)
                {
                    uint featIndex = (int)tile.feature;
                    auto & icons = s_featureIcons[featIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = features.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.sprite.setColor(Color(255, 255, 255, (uint)(featureAlpha * 255.0f)));
                        spriteInfo.x = w;
                        spriteInfo.y = h;
                    }
                }
            }

            if (m_showRivers)
            {
                for (uint i = 0; i < MAX_RIVER_PER_TILE; ++i)
                {
                    const RiverTileInfo & riverTile = tile.rivers[i];

                    if (riverTile.nameIndex != RiverName::None)
                    {
                        uint riverIndex = (int)riverTile.type * enumCount<DirectionType>() + (int)riverTile.direction;

                        bool isDisconnectedRiverTile = false;
                        const RiverInfo & riverInfo = m_rivers[(int)riverTile.nameIndex];
                        for (uint d = 0; d < riverInfo.disconnectedTiles.size(); ++d)
                        {
                            if (riverInfo.disconnectedTiles[d] == int2(w, h))
                            {
                                isDisconnectedRiverTile = true;
                                riverErrors.push_back(int2(w, h));
                                break;
                            }
                        }

                        auto & icons = s_riverIcons[riverIndex];

                        if (icons.texture.getSize().x > 0)
                        {
                            SpriteInfo & spriteInfo = rivers.sprites.emplace_back();
                            spriteInfo.sprite.setTexture(icons.texture);
                            spriteInfo.x = w;
                            spriteInfo.y = h;
                            
                            float4 color = getRiverNameColor(riverTile.nameIndex);
                            float alpha = isDisconnectedRiverTile ? riverAlpha * 0.25f : riverAlpha;
                            spriteInfo.sprite.setColor(Color((uint)(color.r * 255.0f), (uint)(color.g * 255.0f), (uint)(color.b * 255.0f), (uint)(alpha * color.a * 255.0f)));
                        }

                        
                    }
                }
            }

            // Color0.a is continent
            color0.a = (u8)tile.continent;

            terrain.image.setPixel(w, h, color0);

            Color color1 = Color(0, 0, 0, 0);

            // Color1.r is resource
            color1.r = (u8)tile.resource;

            // Color1.g is landmass
            color1.g = (u8)tile.landmass;

            // Color1.b is elevation
            color1.b = (u8)((uint)tile.elevation * 255.0f / float(maxElevation)); // 0..maxElevation values

            // Color1.a is snow amount
            if (m_showSnow || m_mapFilter == MapFilter::Snow)
            {
                float snowAmount = 0.0f;

                if (tile.biome != BiomeType::Marine)
                {
                    if (m_randomSnow)
                    {
                        if (m_useAdvancedSnow)
                        {
                            if ((int)h < m_bottomSnowRows)
                            {
                                snowAmount = (float)(m_bottomSnowRows-h) / (float)m_bottomSnowRows;
                            }
                            else if (h > (m_height - 1 - m_topSnowRows))
                            {
                                snowAmount = (m_topSnowRows + h - m_height + 1) / (float)m_topSnowRows;
                            }

                            snowAmount *= ((float)m_maxSnowWeight / float(maxSnowAmount));
                        }
                    }

                    if (m_snowBorderX > 0)
                    {
                        if ((int)w < m_snowBorderX || (int)w >(m_width - 1 - m_snowBorderX))
                            snowAmount = 1.0f;
                    }

                    if ((int)tile.snow > 0)
                        snowAmount = (float)tile.snow / float(maxSnowAmount);
                }

                color1.a = (u8)((uint)(snowAmount* maxSnowAmount) * 255.0f / float(maxSnowAmount));
            }

            terrain.image.setPixel(w, h + m_height, color1);

            getResourceInfo(tile.resource).count++;

            if (m_showResources)
            {
                if (ResourceType::None != tile.resource)
                {
                    uint resIndex = (int)tile.resource;
                    auto & icons = s_resourceIcons[resIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = resources.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.sprite.setColor(Color(255, 255, 255, (uint)(resourceAlpha * 255.0f)));
                        spriteInfo.x = w;
                        spriteInfo.y = h;
                        //spriteInfo.scale = 0.75f;
                    }
                }
            }

            if (NaturalWonderType::None != tile.naturalWonder)
            {
                getNaturalWonderInfo(tile.naturalWonder).count++;
                if (m_showNaturalWonders)
                {
                    uint resIndex = (int)tile.naturalWonder;
                    auto & icons = s_naturalWonderIcons[resIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = naturalWonders.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.x = w;
                        spriteInfo.y = h;
                        //spriteInfo.scale = 0.75f;
                    }
                }
            }

            if (m_showTSL)
            {
                for (int c = 0; c < m_civilizations.size(); ++c)
                {
                    const auto & civ = m_civilizations[c];
                    for (int t = 0; t < civ.tsl.size(); ++t)
                    {
                        const auto & tsl = civ.tsl[t];
                        if (tsl.pos.x == w && tsl.pos.y == h)
                        {
                            auto & icons = s_miscIcons[(int)MiscIcons::TSL];

                            if (icons.texture.getSize().x > 0)
                            {
                                SpriteInfo & spriteInfo = resources.sprites.emplace_back();
                                spriteInfo.sprite.setTexture(icons.texture);
                                const Color color = Color((u8)(pow(civ.color.r, 1.0f / 2.2f) * 255.0f), (u8)(pow(civ.color.g, 1.0f / 2.2f) * 255.0f), (u8)(pow(civ.color.b, 1.0f / 2.2f) * 255.0f), 255);
                                spriteInfo.sprite.setColor(color);
                                spriteInfo.x = w;
                                spriteInfo.y = h;
                            }
                        }
                    }
                }
            }
        }
    }

    if (m_showRivers)
    {
        //auto & icons = s_miscIcons[(int)MiscIcons::RiverError];
        //if (icons.texture.getSize().x > 0)
        //{
        //    for (uint i = 0; i < riverErrors.size(); ++i)
        //    {
        //        SpriteInfo & spriteInfo = rivers.sprites.emplace_back();
        //        spriteInfo.sprite.setTexture(icons.texture);
        //        spriteInfo.scale = 0.25f;
        //        spriteInfo.x = riverErrors[i].x;
        //        spriteInfo.y = riverErrors[i].y;
        //    }
        //}
    }

    for (u32 i = 0; i < enumCount<MapBitmap>(); ++i)
    {
        auto & bitmap = m_bitmaps[i];

        //if (bitmap.quadshader == invalidShaderID)
        {
            switch ((MapBitmap)i)
            {
            default:
                break;

            case MapBitmap::TerrainData:
                bitmap.texture.loadFromImage(bitmap.image);
                bitmap.sprite.setTexture(bitmap.texture);
                bitmap.sprite.setTextureRect(IntRect(0, 0, bitmap.texture.getSize().x, bitmap.texture.getSize().y));
                bitmap.sprite.setScale(Vector2f(scale.x * 2.0f, scale.y));
                bitmap.quadshader = ShaderManager::add("data/shader/terrain_vs.fx", "data/shader/terrain_ps.fx");
                bitmap.quadblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;
            }
        }

        //if (bitmap.spriteshader == invalidShaderID)
        {
            switch ((MapBitmap)i)
            {
                default:
                    ASSERT(false);
                    break;

                case MapBitmap::TerrainData:
                    break;
                
                case MapBitmap::Resources:
                case MapBitmap::Features:
                case MapBitmap::NaturalWonder:
                case MapBitmap::Rivers:
                    bitmap.spriteshader = ShaderManager::add("data/shader/resources_vs.fx", "data/shader/resources_ps.fx");
                    bitmap.spriteblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                    break;
            }
        }
    }

    m_copyRGBshader = ShaderManager::add("data/shader/default_vs.fx", "data/shader/copyRGB_ps.fx");
}