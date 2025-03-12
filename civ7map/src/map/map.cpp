#include "map.h"
#include "maths.h"
#include "stringconvert.h"
#include "tinyxml2.h"
#include "zip_file.hpp"
#include "base64.hpp"
#include "undoredo/UndoRedoTile.h"
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

SharedIcon Map::s_resourceIcons[enumCount<ResourceType>()];
SharedIcon Map::s_defaultFlag;
const string Map::s_noContinentName = "No continent";

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
    // init with default civ list (TODO: parse mod files for custom civs?)

    // Antiquity Age Civilizations
    m_civilizations.push_back(Civilization("Aksuma", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));        // Amanitore's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Egypt", Era::Antiquity, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));      // Hatshepsut's color (YELLOW_LT)
    m_civilizations.push_back(Civilization("Greece", Era::Antiquity, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));     // Pericles's color (YELLOW_LT)
    m_civilizations.push_back(Civilization("Han", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));           // Ibn Battuta's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Khmer", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));         // Jayavarman VII's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Maurya", Era::Antiquity, float4(44.0f / 255.0f, 48.0f / 255.0f, 44.0f / 255.0f, 1.0f)));        // Ashoka's color (PURPLE_DK)
    m_civilizations.push_back(Civilization("Maya", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));          // Pacal's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Mississippian", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f))); // Pocatello's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Persia", Era::Antiquity, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f)));     // Xerxes's color (WHITE_LT)
    m_civilizations.push_back(Civilization("Rome", Era::Antiquity, float4(44.0f / 255.0f, 48.0f / 255.0f, 44.0f / 255.0f, 1.0f)));          // Augustus's color (PURPLE_DK)
    m_civilizations.push_back(Civilization("Carthage", Era::Antiquity, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f))); // Assumed color (ORANGE_MD)

    // Exploration Age Civilizations
    m_civilizations.push_back(Civilization("Abbasid", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));     // Harun al-Rashid's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Chola", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));       // Rajendra Chola I's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Hawaii", Era::Exploration, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));   // Kamehameha's color (YELLOW_LT)
    m_civilizations.push_back(Civilization("Inca", Era::Exploration, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));      // Pachacuti's color (YELLOW_MD)
    m_civilizations.push_back(Civilization("Majapahit", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));   // Hayam Wuruk's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Ming", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));        // Zhu Yuanzhang's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Mongolia", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));    // Genghis Khan's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Norman", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));      // William the Conqueror's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Songhai", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));     // Askia Muhammad I's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Spain", Era::Exploration, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f)));     // Isabella's color (ORANGE_MD)
    m_civilizations.push_back(Civilization("Buganda", Era::Exploration, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f))); // Assumed color (ORANGE_MD)
    m_civilizations.push_back(Civilization("Shawnee", Era::Exploration, float4(95.0f / 255.0f, 86.0f / 255.0f, 10.0f / 255.0f, 1.0f))); // Tecumseh's color (ORANGE_DK)

    // Modern Age Civilizations
    m_civilizations.push_back(Civilization("America", Era::Modern, float4(54.0f / 255.0f, 50.0f / 255.0f, 19.0f / 255.0f, 1.0f)));          // Benjamin Franklin's color (RED_DK)
    m_civilizations.push_back(Civilization("French Empire", Era::Modern, float4(0.0f / 255.0f, 44.0f / 255.0f, 140.0f / 255.0f, 1.0f)));    // Napoleon's color (BLUE_DK)
    m_civilizations.push_back(Civilization("Mexico", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));           // Benito Juarez's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Mughal", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));           // Akbar's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Prussia", Era::Modern, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f)));       // Friedrich's color (WHITE_LT)
    m_civilizations.push_back(Civilization("Qing", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));             // Kangxi Emperor's color (GREEN_DK)
    m_civilizations.push_back(Civilization("Russia", Era::Modern, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));         // Catherine's color (YELLOW_MD)
    m_civilizations.push_back(Civilization("Siam", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));             // Ramkhamhaeng's color (GREEN_DK)
    m_civilizations.push_back(Civilization("America", Era::Modern, float4(54.0f / 255.0f, 50.0f / 255.0f, 19.0f / 255.0f, 1.0f)));          // Benjamin Franklin's color (RED_DK)
    m_civilizations.push_back(Civilization("Great Britain", Era::Modern, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f))); // Assumed color (WHITE_LT)
    m_civilizations.push_back(Civilization("Meiji", Era::Modern, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));          // Assumed color (YELLOW_MD)
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
            SharedIcon & resIcon = s_resourceIcons[index];
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
void Map::loadFlags()
{
    SharedIcon & defaultFlagIcon = s_defaultFlag;
    if (defaultFlagIcon.dirty)
    {
        bool dirty = false;
        string path = "data/img/tsl/white.png";
        if (defaultFlagIcon.texture.loadFromFile(path))
        {
            LOG_INFO("Texture \"%s\" loaded", GetFilename(path).c_str());
            defaultFlagIcon.dirty = dirty;
            defaultFlagIcon.texture.generateMipmap();
        }
        else
            LOG_ERROR("Texture \"%s\" could not be loaded", GetFilename(path).c_str());
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

    LOG_INFO("Translate map by {%i,%i}", offsetX, offsetY);

    // move spawn points also
    for (uint c = 0; c < m_civilizations.size(); ++c)
    {
        auto & civ = m_civilizations[c];
        for (uint t = 0; t < civ.tsl.size(); ++t)
        {
            auto & tsl = civ.tsl[t];

            tsl.pos.x = (tsl.pos.x - _offset.x) % m_width;
            tsl.pos.y = (tsl.pos.y - _offset.y) % m_height;
        }
    }

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
        return s_noContinentName;
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

//--------------------------------------------------------------------------------------
bool Map::setTerrain(int _x, int _y, TerrainType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.terrain != _value)
    {
        tile.terrain = _value;
        auto event = new UndoRedoTile(this);
        event->add(_x, _y, m_civ7TerrainType.get(_x, _y), tile);
        UndoRedoStack::add(event);
        m_civ7TerrainType.get(_x, _y) = tile;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setBiome(int _x, int _y, BiomeType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.biome != _value)
    {
        tile.biome = _value;
        auto event = new UndoRedoTile(this);
        event->add(_x, _y, m_civ7TerrainType.get(_x, _y), tile);
        UndoRedoStack::add(event);
        m_civ7TerrainType.get(_x, _y) = tile;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setFeature(int _x, int _y, FeatureType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.feature != _value)
    {
        tile.feature = _value;
        auto event = new UndoRedoTile(this);
        event->add(_x, _y, m_civ7TerrainType.get(_x, _y), tile);
        UndoRedoStack::add(event);
        m_civ7TerrainType.get(_x, _y) = tile;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setResource(int _x, int _y, ResourceType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.resource != _value)
    {
        tile.resource = _value;
        auto event = new UndoRedoTile(this);
        event->add(_x, _y, m_civ7TerrainType.get(_x, _y), tile);
        UndoRedoStack::add(event);
        m_civ7TerrainType.get(_x, _y) = tile;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setContinent(int _x, int _y, ContinentType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.continent != _value)
    {
        tile.continent = _value;
        auto event = new UndoRedoTile(this);
        event->add(_x, _y, m_civ7TerrainType.get(_x, _y), tile);
        UndoRedoStack::add(event);
        m_civ7TerrainType.get(_x, _y) = tile;
        return true;
    }
    return false;
}
