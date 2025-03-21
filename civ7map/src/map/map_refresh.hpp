//--------------------------------------------------------------------------------------
void Map::refresh()
{
    if (!m_isLoaded)
        return;

    // Refresh textures
    loadIcons();
    loadFlags();

    createBitmaps();
    initResources();

    Bitmap & terrain = m_bitmaps[(int)MapBitmap::TerrainData];

    extern u32 g_screenWidth;
    extern u32 g_screenHeight;

    Vector2f scale = Vector2f(float(g_screenWidth) / float(m_width), float(g_screenHeight) / float(m_height));
    float ar =(float(g_screenWidth) / float(m_width)) / (float(g_screenHeight) / float(m_height));
    scale.y *= ar;

    auto & resources = m_bitmaps[(int)MapBitmap::Resources];

    for (u32 h = 0; h < m_height; ++h)
    {
        for (u32 w = 0; w < m_width; ++w)
        {
            const u32 offset = w + h * m_width;

            const Civ7Tile & tile = m_civ7TerrainType.get(w, h);

            Color color0 = Color(0, 0, 0, 0);

            // Red is terrain type
            color0.r = (u8)tile.terrain;

            // Green is biome
            color0.g = (u8)tile.biome;

            // Blue is feature
            color0.b = (u8)tile.feature;

            // Alpha is continent
            color0.a = (u8)tile.continent;

            terrain.image.setPixel(w, h, color0);

            Color color1 = Color(0, 0, 0, 0);

            // Red is resource
            color1.r = (u8)tile.resource;
            
            terrain.image.setPixel(w, h + m_height, color1);

            if (m_showResources)
            {
                if (ResourceType::Random != tile.resource && ResourceType::None != tile.resource)
                {
                    uint resIndex = (int)/*ResourceType::Gold*/ tile.resource;
                    auto & info = m_resources[resIndex];
                    auto & icons = s_resourceIcons[resIndex];

                    if (icons.texture.getSize().x > 0)
                    {
                        SpriteInfo & spriteInfo = resources.sprites.emplace_back();
                        spriteInfo.sprite.setTexture(icons.texture);
                        spriteInfo.x = w;
                        spriteInfo.y = h;
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
        bitmap.texture.loadFromImage(bitmap.image);
        bitmap.sprite.setTexture(bitmap.texture);
        bitmap.sprite.setTextureRect(IntRect(0, 0, bitmap.texture.getSize().x, bitmap.texture.getSize().y));
        bitmap.sprite.setScale(Vector2f(scale.x * 2.0f, scale.y));

        //if (bitmap.quadshader == invalidShaderID)
        {
            switch ((MapBitmap)i)
            {
            default:
                break;

            case MapBitmap::TerrainData:
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
            }
        }
    }

    m_copyRGBshader = ShaderManager::add("data/shader/default_vs.fx", "data/shader/copyRGB_ps.fx");
}