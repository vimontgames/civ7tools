#include "map.h"
#include "maths.h"
#include "stringconvert.h"
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
#include "map_create.hpp"
#include "map_actions.hpp"
#include "map_render.hpp"

ResourceIcon Map::s_resourceIcons[enumCount<ResourceType>()];

//--------------------------------------------------------------------------------------
string Map::GetMapDataPathFromMapPath(const string & _mapPath)
{
    string mapDataPath = GetFilenameWithoutExtension(_mapPath);
    mapDataPath = mapDataPath.substr(0, mapDataPath.length() - 4); // remove "-map"
    mapDataPath = fmt::sprintf("%s\\%s-data.js", GetFolder(_mapPath), mapDataPath);
    return mapDataPath;
}

//--------------------------------------------------------------------------------------
Map::Map()
{
    
}

//--------------------------------------------------------------------------------------
void Map::createBitmaps()
{
    for (u32 i = 0; i < enumCount<MapBitmap>(); ++i)
    {
        auto & bitmap = m_bitmaps[i];
        bitmap.image.create(m_width, m_height * 2);
        bitmap.sprites.clear();
    }
}

//--------------------------------------------------------------------------------------
void Map::initResources()
{
    for (auto val : enumValues<ResourceType>())
    {
        int index = (int)val.first;
        if (index > 0)
        {
            ResourceInfo & info = m_resources[index];
            info.count = 0;
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadIcons()
{
    for (auto val : enumValues<ResourceType>())
    {
        int index = (int)val.first;
        if (index != -1)
        {
            ResourceIcon & resIcon = s_resourceIcons[index];
            if (resIcon.dirty)
            {
                bool dirty = false;

                auto path = fmt::sprintf("data/img/resources/%s.png", asString((ResourceType)index));
                if (!FileExists(path))
                {
                    LOG_WARNING("Texture \"%s\" not found", GetFilename(path).c_str());
                    path = fmt::sprintf("data/img/resources/%s.png", "Default");
                    dirty = true;
                }

                if (resIcon.texture.loadFromFile(path))
                {
                    LOG_INFO("Texture \"%s\" loaded", GetFilename(path).c_str());
                    resIcon.dirty = dirty;
                    resIcon.texture.generateMipmap();
                }
                else
                    LOG_ERROR("Texture \"%s\" could not be loaded", GetFilename(path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
string Map::getShortName() const
{
    return GetFilenameWithoutExtension(m_mapPath);
}

//--------------------------------------------------------------------------------------
void Map::resetCameraPan()
{
    m_cameraOffset = Vector2f(0, 0);
    m_cameraPreviousOffset = m_cameraOffset;
}

//--------------------------------------------------------------------------------------
void Map::resetCameraZoom()
{
    m_cameraZoom = 1.0f;
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

    //for (u32 i = 0; i < allSpawnsPoints.size(); ++i)
    //{
    //    SpawnPoint & spawn = allSpawnsPoints[i];
    //    spawn.pos.x = ((int)spawn.pos.x + (int)m_width - offsetX) % (int)m_width;
    //    spawn.pos.y = ((int)spawn.pos.y + (int)m_height - offsetY) % (int)m_height;
    //}

    m_civ7TerrainType.translate(offsetX, offsetY);
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

//--------------------------------------------------------------------------------------
MapSize Map::getMapSize(uint _width, uint _height)
{
    for (auto val : enumValues<MapSize>())
    {
        const int index = (int)val.first;
        if (index > 0)
        {
            if (g_mapSizes[index][0] == _width && g_mapSizes[index][1] == _height)
                return val.first;
        }
    }
    return MapSize::Custom;
}