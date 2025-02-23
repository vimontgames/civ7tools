//--------------------------------------------------------------------------------------
void Map::refresh()
{
    if (!loaded)
        return;

    loadIcons();

    Bitmap & terrain = bitmaps[(int)MapBitmap::TerrainData];

    static sf::Color zoneColors[] =
    {
        Color::Red,
        Color::Green,
        Color::Yellow,
        Color::Magenta,
        Color::Cyan
    };

    extern u32 g_screenWidth;
    extern u32 g_screenHeight;

    const Vector2f scale = Vector2f(float(g_screenWidth) / float(m_width), float(g_screenHeight) / float(m_height));

    for (u32 h = 0; h < m_height; ++h)
    {
        for (u32 w = 0; w < m_width; ++w)
        {
            const u32 offset = w + h * m_width;

            const Civ7Tile & tile = civ7TerrainType.get(w, h);

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
        }
    }

    for (u32 i = 0; i < (int)MapBitmap::Count; ++i)
    {
        auto & bitmap = bitmaps[i];
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
                bitmap.quadshader = ShaderManager::add("data/shader/territories_vs.fx", "data/shader/territories_ps.fx");
                bitmap.quadblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;
            }
        }

        //if (bitmap.spriteshader == invalidShaderID)
        //{
        //    switch ((MapBitmap)i)
        //    {
        //    default:
        //        break;
        //
        //    case MapBitmap::Heightfield:
        //        bitmap.quadshader = ShaderManager::add("data/shader/heightfield_vs.fx", "data/shader/heightfield_ps.fx");
        //        bitmap.quadblend = sf::BlendMode(BlendMode::Factor::One, sf::BlendMode::Factor::Zero, BlendMode::Equation::Add);
        //        break;
        //
        //    case MapBitmap::Resources:
        //        bitmap.spriteshader = ShaderManager::add("data/shader/resources_vs.fx", "data/shader/resources_ps.fx");
        //        bitmap.spriteblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
        //        break;
        //    }
        //}
    }

    copyRGBshader = ShaderManager::add("data/shader/default_vs.fx", "data/shader/copyRGB_ps.fx");
}