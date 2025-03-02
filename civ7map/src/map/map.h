#pragma once

#include "shadermanager.h"
#include "resourceinfo.h"
#include "tinyxml2.h"
#include "Array2D.h"

#include "tile/civ7tile.h"
#include "tile/civ6tile.h"

#define MAX_PLAYER_SPAWN 10

//--------------------------------------------------------------------------------------
enum class MapBitmap
{
    TerrainData = 0,        // TerrainType | Biome | Feature | Continent then Resource | ? | ? | ?

    First = TerrainData,
    Last = TerrainData,
    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
enum class MapFilter
{
    TerrainType = 0,
    Biome,
    Feature,
    Continent,

    Resource,

    First = TerrainType,
    Last = Resource,
    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
enum class GridType
{
    Regular = 0,
    Offset,
    Hexagon
};

//--------------------------------------------------------------------------------------
struct Bitmap
{
    bool visible = true;

    sf::Sprite sprite;
    sf::Image image;
    sf::Texture texture;

    bool drawQuad = true;
    ShaderID quadshader = invalidShaderID;
    sf::BlendMode quadblend = { sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero, sf::BlendMode::Add };

    bool drawSprites = false;

    vector<sf::Sprite> sprites;
    ShaderID spriteshader = invalidShaderID;
    sf::BlendMode spriteblend = { sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero, sf::BlendMode::Add };

    float alpha = 1.0f;
};

//--------------------------------------------------------------------------------------
struct NaturalWonder
{
    string name;
    ColorFloat4 color = ColorFloat4(1,0,0,1);
    bool visible = true;
};

//--------------------------------------------------------------------------------------
struct SpawnPoint
{
    sf::Vector2u pos;
    u32 flags = (u32)-1;
    u32 index[MAX_PLAYER_SPAWN] = { (u32)0 };
};

//--------------------------------------------------------------------------------------
struct SpawnInfo
{
    bool visible = false;
    sf::Texture texture;
};

//--------------------------------------------------------------------------------------
struct Map
{
public:
    Map();

    // map_import.hpp
    bool importCiv7Map(const string & _map, const string & _cwd);

    // map_export.hpp
    void exportCiv7Map(string & _map, const string & _cwd);
    void saveBitmap(const Array2D<u32> _bitmap, tinyxml2::XMLElement * _xmlTerrainSave, const string & _field);

    // map_actions.hpp
    void randomizeSpawnOrder(); 
    void addSpawn();
    void computeSpawnOrder();
    void mergeSpawns();
    void removeSpawn(u32 _index);
    void clearTerritories();
    void clearLandmarks();

    // map_refresh.hpp
    void refresh();

    // map_render.hpp
    void render(sf::RenderWindow & _window);

    // misc
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
    bool GetCiv7TerrainFromCiv6(const string & data);
    Civ7Tile ConvertCiv6TileToCiv7(const Civ6Tile & _civ6Tile, u32 i, u32 j);
        
public:
    string m_path;
    string m_GetMapPath;
    string m_author;

    u32 m_width = 0;
    u32 m_height = 0;

    Array2D<Civ7Tile> civ7TerrainType;

    Bitmap bitmaps[(int)MapBitmap::Count];

    bool loaded = false;
    bool m_isOpen = true;
    
    MapFilter m_mapFilter = MapFilter::TerrainType;
    GridType m_gridType = GridType::Hexagon;
    bool m_showBorders = true;

    bool showStrategicResources = false;
    bool showLuxuryResources = false;
    bool showWonders = false;
    bool showSpawnPoints = false;

    bool useMapCycling = false;
    bool useProceduralMountainChains = false;

    tinyxml2::XMLDocument xmlDocDescriptor;
    tinyxml2::XMLDocument xmlDocSave;

    u32 spawnPlayerCountDisplayed = 0;
    int empireCount = 0;

    SpawnInfo spawnInfo[MAX_PLAYER_SPAWN];
    vector<SpawnPoint> allSpawnsPoints;

    sf::RenderTexture renderTexture;

    // camera
    bool hovered = false;
    bool docked = false;
    bool cameraPan = false;
    sf::Vector2f cameraPanOrigin;
    sf::Vector2f cameraOffset = sf::Vector2f(0, 0);
    sf::Vector2f cameraPreviousOffset = sf::Vector2f(0, 0);
    float cameraZoom = 1.0f;
    float mouseWheelDelta = 0;

    ShaderID copyRGBshader = invalidShaderID;

    sf::Vector2i mapOffset[2] = { sf::Vector2i(0,0), sf::Vector2i(0,0) };

    vector<string> m_continents;
};