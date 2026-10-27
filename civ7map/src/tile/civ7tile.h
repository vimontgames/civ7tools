#pragma once

//--------------------------------------------------------------------------------------
enum class TerrainType : u8
{
    Mountain = 0,
    Hill,
    Flat,
    Coast,
    Ocean,
    NavigableRiver
};

//--------------------------------------------------------------------------------------
enum class TerrainBit : u8
{
    Mountain       = (1UL << (uint)TerrainType::Mountain),
    Hill           = (1UL << (uint)TerrainType::Hill),
    Flat           = (1UL << (uint)TerrainType::Flat),
    Coast          = (1UL << (uint)TerrainType::Coast),
    Ocean          = (1UL << (uint)TerrainType::Ocean),
    NavigableRiver = (1UL << (uint)TerrainType::NavigableRiver)
};

//--------------------------------------------------------------------------------------
struct TerrainInfo
{
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
enum class BiomeType : u8
{
    Tundra = 0,
    Grassland,
    Plains,
    Tropical,
    Desert,
    Marine
};

//--------------------------------------------------------------------------------------
enum class BiomeBit : u8
{
    Tundra     = (1UL << (uint)BiomeType::Tundra),
    Grassland  = (1UL << (uint)BiomeType::Grassland),
    Plains     = (1UL << (uint)BiomeType::Plains),
    Tropical   = (1UL << (uint)BiomeType::Tropical),
    Desert     = (1UL << (uint)BiomeType::Desert),
    Marine     = (1UL << (uint)BiomeType::Marine),
};

//--------------------------------------------------------------------------------------
struct BiomeInfo
{
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
struct BiomeTerrainInfo
{
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
enum class FertilityType : u8
{
    Dry = 0,
    Standart,
    Fertile
};

//--------------------------------------------------------------------------------------
enum class FeatureType : u8
{
    None = 0,

    SagebrushSteppe,
    Oasis,
    DesertFloodplainMinor,
    DesertFloodplainNavigable,
    Forest,
    Marsh,
    GrasslandFloodplainMinor,
    GrasslandFloodplainNavigable,
    Reef,
    ColdReef,
    Ice,
    SavannaWoodland,
    WateringHole,
    PlainsFloodplainMinor,
    PlainsFloodplainNavigable,
    RainForest,
    Mangrove,
    TropicalFloodplainMinor,
    TropicalFloodplainNavigable,
    Taiga,
    TundraBog,
    TundraFloodplainMinor,
    TundraFloodplainNavigable,
    Volcano,
    Atoll,
    Lotus                           
};

//--------------------------------------------------------------------------------------
struct FeatureInfo
{
    u32 count = 0;
    u8 compatibleBiomes = -1;
    u8 compatibleTerrains = -1;
};

//--------------------------------------------------------------------------------------
struct ContinentInfo
{
    string name;
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
struct LandmassInfo
{
    u8 index;
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
enum class ResourceType : u8
{
    None = 0,
    Cotton,
    Dates,
    Dyes,
    Fish,
    Gold,
    GoldDistantLands,
    Gypsum,
    Incense,
    Ivory,
    Jade,
    Kaolin,
    Marble,
    Pearls,
    Silk,
    Silver,
    SilverDistantLands,
    Wine,
    Camels,
    Hides,
    Horses,
    Iron,
    Salt,
    Wool,
    LapisLazuli,
    Cocoa,
    Furs,
    Spices,
    Sugar,
    Tea,
    Truffles,
    Niter,
    Cloves,
    Whales,
    Coffee,
    Tobacco,
    Citrus,
    Coal,
    Nickel,
    Oil,
    Quinine,
    Rubber,
    Mangos,
    Clay,
    Flax,
    Rubies,
    Rice,
    Limestone,
    Tin,
    Llamas,
    Hardwood,
    WildGame,
    Crabs,
    Cowrie,
    Turtles,
    Pitch               
};

//--------------------------------------------------------------------------------------
struct ResourceInfo
{
    u32 count = 0;
    u8 compatibleBiomes = -1;
    u8 compatibleTerrains = -1;
};

//--------------------------------------------------------------------------------------
enum class ContinentType : i8
{
    None = -1
};

//--------------------------------------------------------------------------------------
enum class LandmassType : u8
{

};

//--------------------------------------------------------------------------------------
enum class NaturalWonderType : u8
{
    None = 0,

    ValleyOfFlowers,
    BarrierReef,
    RedwoodForest,
    GrandCanyon,
    Gullfoss,
    Hoerikwaggo,
    IguazuFalls,
    Kilimanjaro,
    Zhangjiajie,
    Thera,
    TorresDelPaine,
    Uluru,
    MountEverest,
    BermudaTriangle,
    Machapuchare,
    MountFuji,
    Vihren,
    Vinicunca,
    GreatBlueHole,
    MapuAVaeaBlowholes,
    NachiFalls,
    SeongsanIlchulbong,
};

struct NaturalWonderInfo
{
    u32 count = 0;
}; 

//--------------------------------------------------------------------------------------
struct NaturalWonderPlacementInfo
{
    BiomeBit biomes;
    TerrainBit terrains;
    const char * description;
};

//--------------------------------------------------------------------------------------
constexpr NaturalWonderPlacementInfo g_NaturalWonderPlacement[] =
{
    { (BiomeBit)0x00,      (TerrainBit)0x00,     ""},
    { BiomeBit::Plains,    TerrainBit::Flat,     "2 tiles; passable." },                                            // ValleyOfFlowers
    { BiomeBit::Marine,    TerrainBit::Coast,    "4 tiles (chain); passable; Coastal Reef." },                      // BarrierReef
    { BiomeBit::Grassland, TerrainBit::Flat,     "3 tiles (triangle); passable; Vegetated terrain." },              // RedwoodForest
    { BiomeBit::Desert,    TerrainBit::Hill,     "4 tiles (diamond); impassable; Rough Desert." },                  // GrandCanyon
    { BiomeBit::Tundra,    TerrainBit::Flat,     "1 tile; impassable; Wet Tundra; provides fresh water." },         // Gullfoss
    { BiomeBit::Grassland, TerrainBit::Mountain, "4 tiles (Y shape); impassable; Mountainous Grassland." },         // Hoerikwaggo
    { BiomeBit::Tropical,  TerrainBit::Flat,     "1 tile; impassable; Wet Tropical; provides fresh water." },       // IguazuFalls
    { BiomeBit::Plains,    TerrainBit::Mountain, "3 tiles (triangle); impassable; Mountainous Volcano." },          // Kilimanjaro
    { BiomeBit::Tropical,  TerrainBit::Mountain, "2 tiles; impassable; Mountainous Tropical." },                    // Zhangjiajie
    { BiomeBit::Marine,    TerrainBit::Coast,    "4 tiles (diamond); impassable; Coastal Volcano." },               // Thera
    { BiomeBit::Tundra,    TerrainBit::Mountain, "3 tiles (triangle); impassable; Mountainous Tundra." },           // TorresDelPaine
    { BiomeBit::Desert,    TerrainBit::Hill,     "1 tile; impassable; Rough Desert." },                             // Uluru
    { BiomeBit::Tropical,  TerrainBit::Mountain, "4 tiles (diamond); impassable; Mountainous Tropical." },          // MountEverest
    { BiomeBit::Marine,    TerrainBit::Ocean,    "3 tiles (triangle); passable; Open Ocean." },                     // BermudaTriangle
    { BiomeBit::Tropical,  TerrainBit::Mountain, "3 tiles (triangle); impassable; Mountainous Tropical." },         // Machapuchare
    { BiomeBit::Plains,    TerrainBit::Mountain, "3 tiles (triangle); impassable; Mountainous Volcano." },          // MountFuji
    { BiomeBit::Plains,    TerrainBit::Mountain, "3 tiles (triangle); impassable; Mountainous Plains." },           // Vihren
    { BiomeBit::Desert,    TerrainBit::Mountain, "4 tiles (diamond); impassable; Mountainous Desert." },            // Vinicunca
    { BiomeBit::Marine,    TerrainBit::Coast,    "1 tile; passable; placed on Coast adjacent to 6 Coast tiles." },  // GreatBlueHole
    { BiomeBit::Marine,    TerrainBit::Coast,    "2 tiles; impassable; placed on Coast adjacent to Cliffs." },      // MapuAVaeaBlowholes
    { BiomeBit::Tropical,  TerrainBit::Flat,     "4 tiles (diamond); impassable; Flat Tropical." },                 // NachiFalls
    { BiomeBit::Marine,    TerrainBit::Coast,    "2 tiles; passable; Coastal." }                                    // SeongsanIlchulbong
};

//--------------------------------------------------------------------------------------
enum class Elevation : u16
{

};

//--------------------------------------------------------------------------------------
// ]10..30] = LIGHT
// ]30..60] = MEDIUM
// ]60..100] = HEAVY
//--------------------------------------------------------------------------------------
enum class SnowAmount : u8
{
    None = 0,
    Light = 20,
    Medium = 45,
    Heavy = 80
};

//--------------------------------------------------------------------------------------
enum class RiverType : u8
{
    Minor = 0,  // RIVER_MINOR,
    Navigable   // RIVER_NAVIGABLE
};

//--------------------------------------------------------------------------------------
enum class DirectionType : i8
{
    NorthEast,      // DIRECTION_NORTHEAST,
    East,           // DIRECTION_EAST,
    SouthEast,      // DIRECTION_SOUTHEAST,
    SouthWest,      // DIRECTION_SOUTHWEST
    West,           // DIRECTION_WEST,
    NorthWest       // DIRECTION_NORTHWEST,
};

//--------------------------------------------------------------------------------------
struct RiverPath
{
    vector<int2> tiles;
};

//--------------------------------------------------------------------------------------
struct RiverInfo
{
    RiverInfo(const string & _name, bool _vanilla = true);

    string name;
    string prettyName;
    bool vanilla;

    vector<RiverPath> riverPaths;
    vector<int2> disconnectedTiles;
    int tileCount = 0;
};

//--------------------------------------------------------------------------------------
enum class RiverName : i8
{
    None = -1
};

//--------------------------------------------------------------------------------------
struct RiverTileInfo
{
    RiverName nameIndex     = RiverName::None;
    RiverType type          = RiverType::Minor;
    DirectionType direction = DirectionType::West;

    inline bool operator!=(const RiverTileInfo & _other) const
    {
        return nameIndex != _other.nameIndex || type != _other.type || direction != _other.direction;
    }
};

#define MAX_RIVER_PER_TILE 2

//--------------------------------------------------------------------------------------
struct Civ7Tile
{
    ContinentType     continent      = ContinentType::None;
    LandmassType      landmass       = (LandmassType)0;
    TerrainType       terrain        = TerrainType::Flat;
    BiomeType         biome          = BiomeType::Tundra;
    FeatureType       feature        = FeatureType::None;
    ResourceType      resource       = ResourceType::None;
    NaturalWonderType naturalWonder  = NaturalWonderType::None;
    Elevation         elevation      = (Elevation)0;
    SnowAmount        snow           = (SnowAmount)0;
    RiverTileInfo     rivers[MAX_RIVER_PER_TILE];
    bool              impassable     = false;

    inline bool operator==(const Civ7Tile & _other) const
    {
        for (uint i = 0; i < MAX_RIVER_PER_TILE; ++i)
        {
            if (rivers[i].nameIndex != _other.rivers[i].nameIndex || rivers[i].type != _other.rivers[i].type || rivers[i].direction != _other.rivers[i].direction)
            {
                return false;
            }
        }

        return continent      == _other.continent
            && landmass       == _other.landmass
            && terrain        == _other.terrain
            && biome          == _other.biome
            && feature        == _other.feature
            && resource       == _other.resource
            && naturalWonder  == _other.naturalWonder
            && elevation      == _other.elevation
            && impassable     == _other.impassable
            && snow           == _other.snow;
    }

    inline bool operator!=(const Civ7Tile & _other) const
    {
        return !operator==(_other);
    }
};
