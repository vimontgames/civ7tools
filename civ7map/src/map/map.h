#pragma once

#include "shadermanager.h"
#include "tinyxml2.h"
#include "Array2D.h"

#include "tile/civ7tile.h"
#include "tile/civ6tile.h"

//--------------------------------------------------------------------------------------
enum class MapSize : int
{
    Custom = -1,

    Tiny,           // MAPSIZE_TINY
    Small,          // MAPSIZE_SMALL
    Standard,       // MAPSIZE_STANDARD
    Large,          // MAPSIZE_LARGE
    Huge,           // MAPSIZE_HUGE
    Greatest_Earth, // MAPSIZE_GREATEST_EARTH
    Massive,        // MAPSIZE_MASSIVE
    Giant,          // MAPSIZE_GIANT
    Ludicrous       // MAPSIZE_LUDICROUS
};

//--------------------------------------------------------------------------------------
static const int g_mapSizes[enumCount<MapSize>()][2] =
{
    {60,38},    // MAPSIZE_TINY
    {74,46},    // MAPSIZE_SMALL
    {84,54},    // MAPSIZE_STANDARD
    {96,60},    // MAPSIZE_LARGE
    {106,66},   // MAPSIZE_HUGE
    {104,64},   // MAPSIZE_GREATEST_EARTH
    {128,80},   // MAPSIZE_MASSIVE
    {180,94},   // MAPSIZE_GIANT
    {230,116},  // MAPSIZE_LUDICROUS
};

//--------------------------------------------------------------------------------------
enum class MapFormat
{
    Unknown = -1,
    Civ6 = 6,
    Civ7 = 7
};

//--------------------------------------------------------------------------------------
enum class MapBitmap
{
    TerrainData = 0,       // TerrainType | Biome | Feature | Continent then Resource | ? | ? | ?
    Resources
};

//--------------------------------------------------------------------------------------
enum class MapFilter
{
    All = 0,
    TerrainType,
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
struct SharedIcon
{
    bool dirty = true;
    sf::Texture texture;
};

//--------------------------------------------------------------------------------------
// Each hex tile has 6 sides
//        
//       /\   /\
//      /  \ /  \           TL: TopLeft
//     | T  |  T |          TR: TopRight
//     | L  |  R |
//    /\   / \  /\
//   /   \/   \/  \
//  |  L |    |  R |        L: Left
//  |    |    |    |        R: Right
//   \  / \  / \  /
//    \/   \/   \/
//    |  B |  B |           BL: BottomLeft
//    |  L |  R |           BR: BottomRight
//     \  / \  /
//      \/   \/
//--------------------------------------------------------------------------------------

enum HexTileSide
{
    TopLeft = 0,
    TopRight,
    Left,
    Right,
    BottomLeft,
    BottomRight
};

//--------------------------------------------------------------------------------------
enum class Era : int 
{
    Antiquity,
    Exploration,
    Modern
};

//--------------------------------------------------------------------------------------
struct TSL
{
    int2 pos = int2(-1, -1);
};

//--------------------------------------------------------------------------------------
struct Civilization
{
    Civilization(const string & _name="", Era _era = (Era)-1, const float4 _color = float4(1,1,1,1)) :
        name(_name),
        era(_era),
        color(_color)
    {

    }

    string name;
    Era era;
    float4 color;
    vector<TSL> tsl;
};

//--------------------------------------------------------------------------------------
struct Map
{
public:
    Map();

    void BeginPaint();
    void Paint(int _x, int _y);
    void EndPaint();

    bool getHexSideTile(int _x, int _y, HexTileSide _side, int2 & _result) const;

    bool setTerrain(int _x, int _y, TerrainType _value);
    bool setBiome(int _x, int _y, BiomeType _value);
    bool setFeature(int _x, int _y, FeatureType _value);
    bool setContinent(int _x, int _y, ContinentType _value);
    bool setResource(int _x, int _y, ResourceType _value);

    bool create(const string & _cwd, const string & _name, int _width, int _height);
    bool createFolders();

    // map_import.hpp
    bool importFiles(const string & _cwd);
    bool importMapSize(const string & data, int & mapWidth, int & mapHeight) const;

    // map_export.hpp
    void exportFiles(const string & _cwd, bool _useModTemplate);
    void saveBitmap(const Array2D<u32> _bitmap, tinyxml2::XMLElement * _xmlTerrainSave, const string & _field);

    // map_actions.hpp

    // map_refresh.hpp
    void refresh();

    // map_render.hpp
    void render(sf::RenderWindow & _window);

    // misc
    void createBitmaps();
    void initResources();
    static void loadIcons();
    static void loadFlags();

    string getShortName() const;

    void resetCameraPan();
    void resetCameraZoom();
    void resetCamera();

    void translate(const sf::Vector2i & _offset);

    string getContinentName(ContinentType continent) const;
    string getContinentShortName(ContinentType continent) const;
    uint getContinentCount() const;

    static string getTerrainTypeAsString(TerrainType _type);
    static string getBiomeTypeAsString(BiomeType _type);
    static string getFeatureTypeAsString(FeatureType _type);
    static string getResourceTypeAsString(ResourceType _type);

    static string GetMapDataPathFromMapPath(const string & _mapPath);

    string getBaseName() const;
    string getPrettyName() const;
    string getLocMapName() const;
    string getLocMapDescription() const;

    string getExportMapSize(MapSize _mapSize);
    static MapSize getMapSize(uint _width, uint _height);

    static Era getEra(Civilization _civ);

    static const string s_noContinentName;
    bool fixHemispheres();

private:
    template <typename T> void loadBitmap(Array2D<T> & _array, tinyxml2::XMLElement * _xmlTerrainSave, const string & _name, u32 _width, u32 _height);
    u32 * loadTexture(tinyxml2::XMLElement * _xmlTerrainSave, const string & _name);
    
    bool importYnAMP(const string & data);
    bool importPrettyName(const string & data);
    bool importHemispheres(const string & data);
    bool importTSL();

    void exportModInfo();
    void exportSQLTables();
    void exportMap();
    void exportMapData();  
    void exportConfig();
    void exportMapText();
    void exportModuleText();
    void exportTSL();

    string getModID() const;

    Civ7Tile ConvertCiv6TileToCiv7(const Civ6Tile & _civ6Tile, u32 i, u32 j);
        
public:
    // shared
    static SharedIcon   s_resourceIcons[enumCount<ResourceType>()];
    static SharedIcon   s_defaultFlag;

    // file(s)
    string              m_modFolder;
    string              m_prettyName;
    string              m_mapPath;
    string              m_mapDataPath;

    // map data
    string              m_author = "Civ7MapUser";
    u32                 m_width = 0;
    u32                 m_height = 0;
    u32                 m_westStart = -1;
    u32                 m_westEnd = -1;
    u32                 m_eastStart = -1;
    u32                 m_eastEnd = -1;
    MapSize             m_mapSize = (MapSize)-1;
    Array2D<Civ7Tile>   m_civ7TerrainType;
    Bitmap              m_bitmaps[enumCount<MapBitmap>()];
    ResourceInfo        m_resources[enumCount<ResourceType>()];
    vector<string>      m_continents;
    vector<Civilization> m_civilizations; 

    //map view
    bool                m_isLoaded = false;
    bool                m_isOpen = true;
    MapFilter           m_mapFilter = MapFilter::All;
    GridType            m_gridType = GridType::Hexagon;
    bool                m_showBorders = true;
    bool                m_showResources = true;
    bool                m_showTSL = true;
    bool                m_showHemispheres = true;
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