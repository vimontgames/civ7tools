
//--------------------------------------------------------------------------------------
void Map::initTerrainInfos(bool _reload)
{
    for (auto val : enumValues<TerrainType>())
    {
        int index = (int)val.first;
        if (index > 0)
        {
            TerrainInfo & info = m_terrainInfos[index];
            info.count = 0;
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::initBiomeInfos(bool _reload)
{
    for (auto val : enumValues<BiomeType>())
    {
        int index = (int)val.first;
        if (index > 0)
        {
            BiomeInfo & info = m_biomeInfos[index];
            info.count = 0;
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::initBiomeTerrainInfos(bool _reload)
{
    for (auto val : enumValues<BiomeType>())
    {
        int biomeIndex = (int)val.first;
        if (biomeIndex > 0)
        {
            for (auto val2 : enumValues<TerrainType>())
            {
                int terrainIndex = (int)val2.first;
                if (terrainIndex > 0)
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
        if (index > 0)
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
        if (index > 0)
        {
            FeatureInfo & info = m_featureInfos[index];
            info.count = 0;
        }
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

    if (m_firstRefresh)
        m_firstRefresh = true;

    // Refresh textures
    loadIcons(_reload);
    loadFlags(_reload);

    createBitmaps();

    Bitmap & terrain = m_bitmaps[(int)MapBitmap::TerrainData];

    extern u32 g_screenWidth;
    extern u32 g_screenHeight;

    Vector2f scale = Vector2f(float(g_screenWidth) / float(m_width), float(g_screenHeight) / float(m_height));
    float ar =(float(g_screenWidth) / float(m_width)) / (float(g_screenHeight) / float(m_height));
    scale.y *= ar;

    auto & resources = m_bitmaps[(int)MapBitmap::Resources];
    auto & features = m_bitmaps[(int)MapBitmap::Features];

    resources.sprites.clear();
    features.sprites.clear();

    for (u32 h = 0; h < m_height; ++h)
    {
        for (u32 w = 0; w < m_width; ++w)
        {
            const u32 offset = w + h * m_width;

            const Civ7Tile & tile = m_civ7TerrainType.get(w, h);

            Color color0 = Color(0, 0, 0, 0);

            // Red is terrain type
            color0.r = (u8)tile.terrain;

            getTerrainInfo(tile.terrain).count++;

            // Green is biome
            color0.g = (u8)tile.biome;
            getBiomeInfo(tile.biome).count++;

            // stats
            getBiomeTerrainInfo(tile.biome, tile.terrain).count++;

            // Blue is feature
            color0.b = (u8)tile.feature;
            getFeatureInfo(tile.feature).count++;

            if (m_showFeatures)
            {
                if (FeatureType::Random != tile.feature && FeatureType::None != tile.feature)
                {
                    uint featIndex = (int)tile.feature;
                    auto & icons = s_featureIcons[featIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = features.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.x = w;
                        spriteInfo.y = h;
                    }
                }
            }

            // Alpha is continent
            color0.a = (u8)tile.continent;

            terrain.image.setPixel(w, h, color0);

            Color color1 = Color(0, 0, 0, 0);

            // Red is resource
            color1.r = (u8)tile.resource;
            
            terrain.image.setPixel(w, h + m_height, color1);

            getResourceInfo(tile.resource).count++;

            if (m_showResources)
            {
                if (ResourceType::Random != tile.resource && ResourceType::None != tile.resource)
                {
                    uint resIndex = (int)tile.resource;
                    auto & icons = s_resourceIcons[resIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = resources.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.x = w;
                        spriteInfo.y = h;
                        spriteInfo.scale = 0.75f;
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
                            auto & icons = s_defaultFlag;

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
                break;
                
            case MapBitmap::Resources:
                bitmap.spriteshader = ShaderManager::add("data/shader/resources_vs.fx", "data/shader/resources_ps.fx");
                bitmap.spriteblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;

            case MapBitmap::Features:
                bitmap.spriteshader = ShaderManager::add("data/shader/resources_vs.fx", "data/shader/resources_ps.fx");
                bitmap.spriteblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;
            }
        }
    }

    m_copyRGBshader = ShaderManager::add("data/shader/default_vs.fx", "data/shader/copyRGB_ps.fx");
}