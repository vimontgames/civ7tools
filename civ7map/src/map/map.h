#pragma once

#include "shadermanager.h"
#include "tinyxml2.h"
#include "Array2D.h"

#include "tile/civ7tile.h"
#include "tile/civ6tile.h"

#define MAX_PLAYER_SPAWN 10

//--------------------------------------------------------------------------------------
enum class MapBitmap
{
    TerrainData = 0,       // TerrainType | Biome | Feature | Continent then Resource | ? | ? | ?
    Resources
};

//--------------------------------------------------------------------------------------
enum class MapFilter
{
    TerrainType = 0,
    Biome,
    Feature,
    Continent,

    Resource
};

//--------------------------------------------------------------------------------------
enum class GridType
{
    Regular = 0,
    Offset,
    Hexagon
};

//--------------------------------------------------------------------------------------
struct SpriteInfo
{
    sf::Sprite sprite;
    int x, y;
};

//--------------------------------------------------------------------------------------
struct Bitmap
{
    bool visible = true;

    sf::Sprite sprite;
    sf::Image image;
    sf::Texture texture;

    bool drawQuad = false;
    ShaderID quadshader = invalidShaderID;
    sf::BlendMode quadblend = { sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero, sf::BlendMode::Add };

    bool drawSprites = false;
    vector<SpriteInfo> sprites;
    ShaderID spriteshader = invalidShaderID;
    sf::BlendMode spriteblend = { sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero, sf::BlendMode::Add };

    float alpha = 1.0f;
};

//--------------------------------------------------------------------------------------
struct Map
{
public:
    Map();

    // map_import.hpp
    bool importMap(const string & _cwd);
    bool importMapSize(const string & data, int & mapWidth, int & mapHeight) const;

    // map_export.hpp
    void exportMap(const string & _cwd);
    void saveBitmap(const Array2D<u32> _bitmap, tinyxml2::XMLElement * _xmlTerrainSave, const string & _field);

    // map_actions.hpp

    // map_refresh.hpp
    void refresh();

    // map_render.hpp
    void render(sf::RenderWindow & _window);

    // misc
    void createBitmaps();
    void loadIcons();

    string getShortName() const;

    void resetCameraPan();
    void resetCameraZoom();
    void resetCamera();

    void translate(const sf::Vector2i & _offset);

    string getContinentName(ContinentType continent) const;
    string getContinentShortName(ContinentType continent) const;
    uint getContinentCount() const;

private:
    template <typename T> void loadBitmap(Array2D<T> & _array, tinyxml2::XMLElement * _xmlTerrainSave, const string & _name, u32 _width, u32 _height);
    u32 * loadTexture(tinyxml2::XMLElement * _xmlTerrainSave, const string & _name);
    bool ImportYnAMP(const string & data);

    Civ7Tile ConvertCiv6TileToCiv7(const Civ6Tile & _civ6Tile, u32 i, u32 j);
        
public:
    // file(s)
    string              m_path;
    string              m_mapDataPath;

    // map data
    string              m_author;
    u32                 m_width = 0;
    u32                 m_height = 0;
    Array2D<Civ7Tile>   m_civ7TerrainType;
    Bitmap              m_bitmaps[enumCount<MapBitmap>()];
    ResourceInfo        m_resources[enumCount<ResourceType>()];
    vector<string>      m_continents;

    //map view
    bool                m_isLoaded = false;
    bool                m_isOpen = true;
    MapFilter           m_mapFilter = MapFilter::TerrainType;
    GridType            m_gridType = GridType::Hexagon;
    bool                m_showBorders = true;
    sf::RenderTexture   m_renderTexture;
    bool                m_isHovered = false;
    bool                m_isDocked = false;
    bool                m_cameraPan = false;
    sf::Vector2f        m_cameraPanOrigin;
    sf::Vector2f        m_cameraOffset = sf::Vector2f(0, 0);
    sf::Vector2f        m_cameraPreviousOffset = sf::Vector2f(0, 0);
    float               m_cameraZoom = 1.0f;
    float               m_mouseWheelDelta = 0;
    ShaderID            m_copyRGBshader = invalidShaderID;
    sf::Vector2i        m_mapOffset[2] = { sf::Vector2i(0,0), sf::Vector2i(0,0) };
};