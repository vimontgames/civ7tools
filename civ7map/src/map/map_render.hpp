extern Vector2i g_hoveredCell;
extern Vector2i g_selectedCell;

void Map::render(RenderWindow & _window)
{
    extern u32 g_screenWidth;
    extern u32 g_screenHeight;

    sf::ContextSettings contextSettings;
                        contextSettings.sRgbCapable = true;

    m_renderTexture.create(g_screenWidth, g_screenHeight, contextSettings);
    m_renderTexture.clear(Color(255, 0, 255,255)); // should not be visible

    sf::RenderTexture tempRenderTexture;
    tempRenderTexture.create(g_screenWidth, g_screenHeight, contextSettings);
    tempRenderTexture.clear(Color(0, 0, 0, 255));

    sf::View view;
             view.setCenter(sf::Vector2f(float(g_screenWidth)*0.5f, (float)(g_screenHeight)*0.5f));
             view.setSize(sf::Vector2f(float(g_screenWidth), (float)g_screenHeight));
             view.zoom(m_cameraZoom); // zeng
             view.move(m_cameraOffset);

    tempRenderTexture.setView(view);

    for (u32 i = 0; i < enumCount<MapBitmap>(); ++i)
    {
        auto & bitmap = m_bitmaps[i];
        const auto mapBitmap = (MapBitmap)i;
        switch (mapBitmap)
        {
            case MapBitmap::TerrainData:
                bitmap.drawQuad = true;
                bitmap.drawSprites = false;
                break;

            case MapBitmap::Resources:
                bitmap.drawQuad = false;
                bitmap.drawSprites = true;
                break;
        }

        if (bitmap.visible)
        {
            if (bitmap.drawQuad)
            {
                auto & texture = bitmap.texture;
                texture.setRepeated(true);

                auto & sprite = bitmap.sprite;

                sf::Shader * shader = ShaderManager::get(bitmap.quadshader);

                RenderStates rs;
                rs.shader = shader;
                rs.blendMode = bitmap.quadblend;

                if (shader)
                {
                    shader->setUniform("texSize", (Vector2f)texture.getSize());
                    shader->setUniform("hoveredCell", Vector2f((float)g_hoveredCell.x, (float)g_hoveredCell.y));
                    shader->setUniform("selectedCell", Vector2f((float)g_selectedCell.x, (float)g_selectedCell.y));

                    int passFlags = 0;

                    switch (m_mapFilter)
                    {
                        default:
                            LOG_ERROR("Missing case \"%s\" (%i)", asString(m_mapFilter).c_str(), (int)m_mapFilter);
                            break;

                        case MapFilter::TerrainType:
                            passFlags = PASS_TYPE_TERRAIN;
                            break;

                        case MapFilter::Biome:
                            passFlags = PASS_TYPE_BIOME;
                            break;

                        case MapFilter::Feature:
                            passFlags = PASS_TYPE_FEATURE;
                            break;

                        case MapFilter::Continent:
                            passFlags = PASS_TYPE_CONTINENT;
                            break;

                        case MapFilter::Resource:
                            passFlags = PASS_TYPE_RESOURCE;
                            break;
                    }

                    switch (m_gridType)
                    {
                        case GridType::Hexagon:
                            passFlags |= PASS_FLAG_HEXAGON;
                            break;

                        case GridType::Offset:
                            passFlags |= PASS_FLAG_OFFSET;
                            break;
                    }

                    if (m_showBorders)
                        passFlags |= PASS_FLAG_BORDERS;

                    shader->setUniform("passFlags", passFlags);
                }

                tempRenderTexture.draw(sprite, rs);
            }

            if (bitmap.drawSprites)
            {
                sf::Shader * shader = ShaderManager::get(bitmap.spriteshader);

                RenderStates rs;
                rs.shader = shader;
                rs.blendMode = bitmap.spriteblend;

                for (u32 j = 0; j < bitmap.sprites.size(); ++j)
                {
                    SpriteInfo & spriteInfo = bitmap.sprites[j];

                    if (shader)
                    {
                        shader->setUniform("texSize", (Vector2f)spriteInfo.sprite.getTexture()->getSize());

                        auto color = Glsl::Vec4(spriteInfo.sprite.getColor());
                        shader->setUniform("color", color);
                    }

                    float scaleX, scaleY;
                    switch (m_gridType)
                    {
                        default:
                            scaleX = 2.0f * (float)m_renderTexture.getSize().x / (float)(m_width);
                            scaleY = 2.0f * (float)m_renderTexture.getSize().x / (float)(m_width);
                            break;

                        case GridType::Offset:
                            scaleX = 2.0f * (float)m_renderTexture.getSize().x / (float)(m_width+1);
                            scaleY = 2.0f * (float)m_renderTexture.getSize().x / (float)(m_width);
                            break;

                        case GridType::Hexagon:
                            scaleX = 2.0f * (float)m_renderTexture.getSize().x / (float)(m_width + 1);
                            scaleY = 2.0f * (float)m_renderTexture.getSize().x / ( float(m_width)+ 1 - 1.0f/6.0f );
                            break;
                    }

                    float texScaleX = scaleX / (float)spriteInfo.sprite.getTexture()->getSize().x;
                    float texScaleY = scaleY / (float)spriteInfo.sprite.getTexture()->getSize().y;

                    spriteInfo.sprite.setOrigin(0, 0);

                    const float fGridX = (float)(spriteInfo.x) * scaleX;
                    const float fGridY = (float)(m_height - spriteInfo.y - 1) * scaleY;

                    switch (m_gridType)
                    {
                        default:
                            spriteInfo.sprite.setPosition(fGridX, fGridY);
                            break;

                        case GridType::Offset:
                            if (spriteInfo.y & 1)
                                spriteInfo.sprite.setPosition(fGridX + scaleX * 0.5f, fGridY);
                            else
                                spriteInfo.sprite.setPosition(fGridX, fGridY);
                            break;

                        case GridType::Hexagon:
                            if (spriteInfo.y & 1)
                                spriteInfo.sprite.setPosition(fGridX + scaleX * 0.5f, fGridY + scaleY * 0.25f);
                            else
                                spriteInfo.sprite.setPosition(fGridX, fGridY + scaleY * 0.25f);
                            break;
                    }

                    spriteInfo.sprite.setScale(texScaleX, texScaleY);

                    tempRenderTexture.draw(spriteInfo.sprite, rs);
                }
            }
        }
    }

    tempRenderTexture.display();

    // clear alpha to one
    RenderStates rs;
    rs.shader = ShaderManager::get(m_copyRGBshader);
    rs.blendMode = sf::BlendMode(BlendMode::Factor::One, sf::BlendMode::Factor::Zero, BlendMode::Equation::Add);
    Sprite quad;
    quad.setColor(Color(1, 1, 1, 1));
    quad.setPosition(Vector2f(0, 0));
    quad.setTexture(tempRenderTexture.getTexture());
    quad.setTextureRect(IntRect(0, 0, m_renderTexture.getSize().x, m_renderTexture.getSize().y));
    m_renderTexture.draw(quad, rs);

    m_renderTexture.display();
}