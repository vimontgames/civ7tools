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

    const Vector2f scale = Vector2f(float(g_screenWidth) / float(width), float(g_screenHeight) / float(height));

    for (u32 h = 0; h < height; ++h)
    {
        for (u32 w = 0; w < width; ++w)
        {
            const u32 offset = w + h * width;

            const Civ7Tile & tile = civ7TerrainType.get(w, h);

            Color color = Color(0, 0, 0, 255);

            // Red is terrain type
            color.r = (u8)tile.terrainType;  

            // Green is biome
            color.g = (u8)tile.biomeType;

            terrain.image.setPixel(w, h, color);
        }
    }

    for (u32 i = 0; i < (int)MapBitmap::Count; ++i)
    {
        auto & bitmap = bitmaps[i];
        bitmap.texture.loadFromImage(bitmap.image);
        bitmap.sprite.setTexture(bitmap.texture);
        bitmap.sprite.setTextureRect(IntRect(0, 0, bitmap.texture.getSize().x, bitmap.texture.getSize().y));
        bitmap.sprite.setScale(scale);

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