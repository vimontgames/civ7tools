#include "map.h"
#include "resourceinfo.h"
#include "maths.h"
#include "tinyxml2.h"
#include "zip_file.hpp"
#include "base64.hpp"

#include "shader/common.h"

#define STBI_ONLY_PNG

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace sf;
using namespace tinyxml2;

#include "map_import.hpp"
#include "map_export.hpp"
#include "map_refresh.hpp"
#include "map_actions.hpp"
#include "map_render.hpp"

//--------------------------------------------------------------------------------------
Map::Map()
{
    
}

//--------------------------------------------------------------------------------------
void Map::loadIcons()
{
    // load resource textures if needed
    for (u32 i = 0; i < (u32)StrategicResource::Count; ++i)
    {
        ResourceInfo & shared = strategicResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/" + strategicResources[i].name + ".png");
        shared.count = 0;
    }

    for (u32 i = 0; i < (u32)LuxuryResource::Count; ++i)
    {
        ResourceInfo & shared = luxuryResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/" + luxuryResources[i].name + ".png");
        shared.count = 0;
    }

    for (u32 i = 0; i < (u32)NaturalWonderResource::Count; ++i)
    {
        ResourceInfo & shared = naturalWonderResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/wonder.png");
        shared.count = 0;
    }

    for (u32 i = 0; i < (int)MapBitmap::Count; ++i)
    {
        auto & bitmap = bitmaps[i];
        bitmap.image.create(m_width, m_height * 2);
        bitmap.sprites.clear();
    }

    for (u32 i = 0; i < _countof(spawnInfo); ++i)
    {
        auto & info = spawnInfo[i];
        if (Vector2u(0, 0) == info.texture.getSize())
            info.texture.loadFromFile("data/img/" + to_string(i + 1) + ".png");  
    }
}

//--------------------------------------------------------------------------------------
string Map::getShortName() const
{
    return GetFilenameWithoutExtension(m_path);
}

//--------------------------------------------------------------------------------------
void Map::resetCameraPan()
{
    cameraOffset = Vector2f(0, 0);
    cameraPreviousOffset = cameraOffset;
}

//--------------------------------------------------------------------------------------
void Map::resetCameraZoom()
{
    cameraZoom = 1.0f;
}

//--------------------------------------------------------------------------------------
void Map::resetCamera()
{
    resetCameraPan();
    resetCameraZoom();
}

//--------------------------------------------------------------------------------------
void Map::translate(const sf::Vector2i & _offset)
{
    int offsetX = _offset.x;
    int offsetY = _offset.y;

    while (offsetX < 0)
        offsetX += m_width;
    while (offsetY < 0)
        offsetY += m_height;

    LOG_INFO("Translate map by {%i,%i}\n", offsetX, offsetY);

    for (u32 i = 0; i < allSpawnsPoints.size(); ++i)
    {
        SpawnPoint & spawn = allSpawnsPoints[i];
        spawn.pos.x = ((int)spawn.pos.x + (int)m_width - offsetX) % (int)m_width;
        spawn.pos.y = ((int)spawn.pos.y + (int)m_height - offsetY) % (int)m_height;
    }

    civ7TerrainType.translate(offsetX, offsetY);
}

//--------------------------------------------------------------------------------------
string Map::getContinentName(ContinentType continent) const
{
    if ((int)continent < m_continents.size())
        return m_continents[(int)continent];
    else
        return "";
}

//--------------------------------------------------------------------------------------
string Map::getContinentShortName(ContinentType continent) const
{
    if (-1 != (int)continent)
    {
        string name = getContinentName(continent);
        const string continentPrefix = "CONTINENT_";
        size_t trim = name.find(continentPrefix);
        if (-1 != trim)
            name = name.substr(trim + continentPrefix.length());

        name = CapitalizeWords(name);
        return name;
    }
    else
    {
        return "No continent";
    }
}

//--------------------------------------------------------------------------------------
uint Map::getContinentCount() const
{
    return (uint)m_continents.size();
}