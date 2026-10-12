#pragma once

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

//--------------------------------------------------------------------------------------
enum class TerrainType : u8
{
    Moutain = 0,
    Hill,
    Flat,
    Coast,
    Ocean,
    NavigableRiver
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
enum class Elevation : u16
{

};

//--------------------------------------------------------------------------------------
struct Civ7Tile
{
    ContinentType     continent     = ContinentType::None;
    LandmassType      landmass      = (LandmassType)0;
    TerrainType       terrain       = TerrainType::Flat;
    BiomeType         biome         = BiomeType::Tundra;
    FeatureType       feature       = FeatureType::None;
    ResourceType      resource      = ResourceType::None;
    NaturalWonderType naturalWonder = NaturalWonderType::None;
    Elevation         elevation     = (Elevation)0;
    bool              impassable    = false;

    inline bool operator==(const Civ7Tile & _other) const
    {
        return continent     == _other.continent
            && landmass      == _other.landmass
            && terrain       == _other.terrain
            && biome         == _other.biome
            && feature       == _other.feature
            && resource      == _other.resource
            && naturalWonder == _other.naturalWonder
            && elevation     == _other.elevation
            && impassable    == _other.impassable;
    }

    inline bool operator!=(const Civ7Tile & _other) const
    {
        return !operator==(_other);
    }
};
