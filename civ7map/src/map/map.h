#pragma once

#include "shadermanager.h"
#include "resourceinfo.h"
#include "tinyxml2.h"
#include "Array2D.h"

#define MAX_PLAYER_SPAWN 10

//--------------------------------------------------------------------------------------
enum MapBitmap
{
    Heightfield = 0,
    Territories,
    Resources,

    First = Heightfield,
    Last = Resources,
    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
struct Territory
{
    u32 continent = 0;
    u32 biome = 0;
    bool ocean = true;
};

//--------------------------------------------------------------------------------------
struct Landmark
{
    std::string name;
    u32 definitonIndex = 0;
    std::vector<sf::Vector2u> positions;
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

    std::vector<sf::Sprite> sprites;
    ShaderID spriteshader = invalidShaderID;
    sf::BlendMode spriteblend = { sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero, sf::BlendMode::Add };

    float alpha = 1.0f;
};

//--------------------------------------------------------------------------------------
enum class TerritoryBackground : u32
{
    None = 0,
    Tile,
    Territory,
    Biome,
    Landmarks,
    Wonders,

    First = None,
    Last = Wonders,
    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
struct NaturalWonder
{
    std::string name;
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
    bool importCiv7Map(const std::string & _map, const std::string & _cwd);

    // map_export.hpp
    void exportCiv7Map(std::string & _map, const std::string & _cwd);
    void saveBitmap(const Array2D<u32> _bitmap, tinyxml2::XMLElement * _xmlTerrainSave, const std::string & _field);

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

    std::string getShortName() const;

    void resetCameraPan();
    void resetCameraZoom();
    void resetCamera();

    void translate(const sf::Vector2i & _offset);

private:
    template <typename T> void loadBitmap(Array2D<T> & _array, tinyxml2::XMLElement * _xmlTerrainSave, const std::string & _name, u32 _width, u32 _height);
    u32 * loadTexture(tinyxml2::XMLElement * _xmlTerrainSave, const std::string & _name);
        
public:
    std::string path;
    std::string author;

    u32 width = 0;
    u32 height = 0;

    Array2D<u32>    elevationTexture;
    Array2D<u32>    zonesTexture;
    Array2D<u32>    poiTexture;
    Array2D<u32>    visibilityTexture;
    Array2D<u32>    roadTexture;
    Array2D<u32>    riverTexture;
    Array2D<u32>    matchingSeedTexture;
    Array2D<u32>    naturalWonderTexture;
    Array2D<u32>    landmarksTexture;

    std::vector<Territory> territoriesInfo;
    std::vector<Landmark> landmarkInfo;
    //std::vector<NaturalWonder> naturalWondersInfo;

    Bitmap bitmaps[MapBitmap::Count];

    bool loaded = false;
    bool m_isOpen = true;
    
    TerritoryBackground territoryBackground = TerritoryBackground::Territory;
    bool showTerritoriesBorders = true;
    bool useHexUVs = true;

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
    std::vector<SpawnPoint> allSpawnsPoints;

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

    
};