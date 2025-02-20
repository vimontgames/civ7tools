//--------------------------------------------------------------------------------------
void Map::refresh()
{
    if (!loaded)
        return;

    loadIcons();

    Bitmap & heigthfield = bitmaps[Heightfield];
    Bitmap & territories = bitmaps[Territories];
    Bitmap & resources = bitmaps[Resources];

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
            //const u32 offset = w + h * width;
            //
            //// Elevation
            //const u32 & elevation = elevationTexture.get(w,h);
            //
            //// Elevation::Heightfield
            //const ubyte height = elevation & 0x0F;
            //Color heightfieldColor = Color(0, 0, 0, 0);
            //heightfieldColor.a = height;
            //
            //// Elevation::Tile
            //const ubyte tileIndex = (elevation >> 8) & 0x0F;
            //
            //// Zones
            //const ubyte territoryIndex = zonesTexture.get(w, h);
            //const Territory territory = territoriesInfo[territoryIndex];
            //Color territoryColor = Color(0, 0, 0, 255);
            //
            //// Landmarks
            //const ubyte landmarkIndex = landmarksTexture.get(w, h);
            //
            //// Natural Wonders
            //const ubyte naturalWonderIndex = naturalWonderTexture.get(w, h);
            //
            //switch (territoryBackground)
            //{
            //    case TerritoryBackground::None:
            //        break;
            //
            //    case TerritoryBackground::Tile:
            //    {
            //        territoryColor.r = 0;
            //        territoryColor.g = tileIndex;
            //        territoryColor.b = TEXEL_FLAG_VISIBLE;
            //        territoryColor.a = territoryIndex;
            //    }
            //    break;
            //
            //    case TerritoryBackground::Territory:
            //    {
            //        territoryColor.r = 0;
            //        territoryColor.g = territoryIndex;
            //        territoryColor.b = TEXEL_FLAG_VISIBLE;
            //        territoryColor.a = territoryIndex;
            //    }
            //    break;
            //
            //    case TerritoryBackground::Biome:
            //    {
            //        u32 biomeIndex = territory.biome;
            //
            //        territoryColor.r = 0;
            //        territoryColor.g = biomeIndex;
            //        territoryColor.b = TEXEL_FLAG_VISIBLE;
            //        territoryColor.a = territoryIndex;
            //    }
            //    break;
            //
            //    case TerritoryBackground::Landmarks:
            //    {
            //        if (0xFF != landmarkIndex)
            //        {
            //            // There's a bug in the map editor that makes not all landmarks are actually saved
            //            if (landmarkIndex < landmarkInfo.size())
            //            {
            //                const Landmark landmark = landmarkInfo[landmarkIndex];
            //
            //                territoryColor.r = 0;
            //                territoryColor.g = landmark.definitonIndex;
            //                territoryColor.b = TEXEL_FLAG_VISIBLE;
            //                territoryColor.a = landmarkIndex;
            //            }
            //        }
            //    }
            //    break;
            //
            //    case TerritoryBackground::Wonders:
            //    {
            //        if (0xFF != (naturalWonderIndex & 0xFF))
            //        {
            //            territoryColor.r = 0;
            //            territoryColor.g = naturalWonderIndex;
            //            territoryColor.b = TEXEL_FLAG_VISIBLE;
            //            territoryColor.a = landmarkIndex;
            //        }
            //    }
            //    break;
            //}
            //
            //if (territory.ocean)
            //    territoryColor.b |= TEXEL_FLAG_OCEAN_TERRITORY;
            //
            //switch ((TileType)tileIndex)
            //{
            //    case TileType::Coastal:
            //    case TileType::Ocean:
            //    case TileType::Lake:
            //        territoryColor.b |= TEXEL_FLAG_WATER_TILE;
            //        heightfieldColor.b |= TEXEL_FLAG_WATER_TILE;
            //        break;
            //}
            //
            //heigthfield.image.setPixel(w, h, heightfieldColor);
            //territories.image.setPixel(w, h, territoryColor);
            //
            //// Resource
            //const u32 poiData = poiTexture.get(w,h);
            //Color resColor = Color(0, 0, 0, 0);
            //
            //const u32 resourceIndex = poiData & 0xFF;
            //const u32 wonderIndex = naturalWonderTexture.get(w, h) & 0xFF;
            //
            //float cellWidth = float(g_screenWidth) / float(width);
            //Vector2f spriteOffset = Vector2f(0.0f, 0.0f);
            //
            //if (useHexUVs)
            //    spriteOffset.x = (h & 1) ? -0.25f * cellWidth : +0.25f * cellWidth;
            //
            //if (showWonders)
            //{
            //    if (0xFF != wonderIndex)
            //    {
            //        ResourceInfo & wonder = naturalWonderResources[wonderIndex];
            //
            //        if (naturalWonderResources[wonderIndex].visible)
            //        {
            //            resColor = wonder.color.toColor();
            //
            //            wonder.texture.setSmooth(true);
            //            wonder.texture.setRepeated(false);
            //            Sprite resSprite;
            //            resSprite.setTexture(wonder.texture);
            //            resSprite.setColor(resColor);
            //            resSprite.setOrigin(Vector2f(wonder.texture.getSize().x*0.5f, wonder.texture.getSize().y*0.5f));
            //            resSprite.setPosition(Vector2f((float(w) + 0.5f)*scale.x, (float(h) + 0.5f)*scale.y));
            //            resSprite.move(spriteOffset);
            //            resources.sprites.push_back(resSprite);
            //        }
            //
            //        wonder.count++;
            //    }
            //}
            //
            //if (showLuxuryResources)
            //{
            //    if (resourceIndex >= (u32)LuxuryResource::First && resourceIndex <= (u32)LuxuryResource::Last)
            //    {
            //        const u32 luxuryIndex = resourceIndex - (u32)LuxuryResource::First;
            //        ResourceInfo & luxury = luxuryResources[luxuryIndex];
            //
            //        if (luxury.visible)
            //        {
            //            resColor = luxury.color.toColor();
            //
            //            luxury.texture.setSmooth(true);
            //            luxury.texture.setRepeated(false);
            //            Sprite resSprite;
            //            resSprite.setTexture(luxury.texture);
            //            resSprite.setColor(resColor);
            //            resSprite.setOrigin(Vector2f(luxury.texture.getSize().x*0.5f, luxury.texture.getSize().y*0.5f));
            //            resSprite.setPosition(Vector2f((float(w) + 0.5f)*scale.x, (float(h) + 0.5f)*scale.y));
            //            resSprite.move(spriteOffset);
            //            resources.sprites.push_back(resSprite);
            //        }
            //
            //        luxury.count++;
            //    }
            //}
            //
            //if (showStrategicResources)
            //{
            //    if (resourceIndex >= (u32)StrategicResource::First && resourceIndex <= (u32)StrategicResource::Last)
            //    {
            //        const u32 strategicIndex = resourceIndex - (u32)StrategicResource::First;
            //        ResourceInfo & strategic = strategicResources[strategicIndex];
            //
            //        if (strategic.visible)
            //        {
            //            resColor = strategic.color.toColor();
            //
            //            strategic.texture.setSmooth(true);
            //            strategic.texture.setRepeated(false);
            //            Sprite resSprite;
            //            resSprite.setTexture(strategic.texture);
            //            resSprite.setColor(resColor);
            //            resSprite.setOrigin(Vector2f(strategic.texture.getSize().x*0.5f, strategic.texture.getSize().y*0.5f));
            //            resSprite.setPosition(Vector2f((float(w) + 0.5f)*scale.x, (float(h) + 0.5f)*scale.y));
            //            resSprite.move(spriteOffset);
            //            resources.sprites.push_back(resSprite);
            //        }
            //
            //        strategic.count++;
            //    }
            //}
            //
            //if (showSpawnPoints)
            //{
            //    for (u32 i = 0; i < _countof(spawnInfo); ++i)
            //    {
            //        if (spawnPlayerCountDisplayed != i + 1)
            //            continue;
            //
            //        for (u32 j = 0; j < allSpawnsPoints.size(); ++j)
            //        {
            //            auto & spawn = allSpawnsPoints[j];
            //
            //            if (spawn.flags & (1 << i) && spawn.pos.x == w && spawn.pos.y == h)
            //            {
            //                auto & texture = spawnInfo[spawn.index[i]].texture;
            //
            //                texture.setSmooth(true);
            //                texture.setRepeated(false);
            //                Sprite resSprite;
            //                resSprite.setTexture(texture);
            //                resSprite.setOrigin(Vector2f(texture.getSize().x*0.5f, texture.getSize().y*0.5f));
            //                resSprite.setPosition(Vector2f((float(w) + 0.5f)*scale.x, (float(h) + 0.5f)*scale.y));
            //                resSprite.move(spriteOffset);
            //                resources.sprites.push_back(resSprite);
            //            }
            //        }
            //    }
            //}

            resources.image.setPixel(w, h, Color(0, 0, 0, 96));
            resources.drawQuad = true;
            resources.drawSprites = true;
        }
    }

    for (u32 i = 0; i < MapBitmap::Count; ++i)
    {
        auto & bitmap = bitmaps[i];
        bitmap.texture.loadFromImage(bitmap.image);
        bitmap.sprite.setTexture(bitmap.texture);
        bitmap.sprite.setTextureRect(IntRect(0, 0, bitmap.texture.getSize().x, bitmap.texture.getSize().y));
        bitmap.sprite.setScale(scale);

        if (bitmap.quadshader == invalidShaderID)
        {
            switch ((MapBitmap)i)
            {
            default:
                break;

            case MapBitmap::Territories:
                bitmap.quadshader = ShaderManager::add("data/shader/territories_vs.fx", "data/shader/territories_ps.fx");
                bitmap.quadblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;

            case MapBitmap::Resources:
                bitmap.quadblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;
            }
        }

        if (bitmap.spriteshader == invalidShaderID)
        {
            switch ((MapBitmap)i)
            {
            default:
                break;

            case MapBitmap::Heightfield:
                bitmap.quadshader = ShaderManager::add("data/shader/heightfield_vs.fx", "data/shader/heightfield_ps.fx");
                bitmap.quadblend = sf::BlendMode(BlendMode::Factor::One, sf::BlendMode::Factor::Zero, BlendMode::Equation::Add);
                break;

            case MapBitmap::Resources:
                bitmap.spriteshader = ShaderManager::add("data/shader/resources_vs.fx", "data/shader/resources_ps.fx");
                bitmap.spriteblend = sf::BlendMode(BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add);
                break;
            }
        }
    }

    copyRGBshader = ShaderManager::add("data/shader/default_vs.fx", "data/shader/copyRGB_ps.fx");
}