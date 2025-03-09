#pragma once

//--------------------------------------------------------------------------------------
enum class TerrainType : i8
{
    Moutain = 0,
    Hill,
    Flat,
    Coast,
    Ocean,
    NavigableRiver
};

//--------------------------------------------------------------------------------------
enum class BiomeType : i8
{
    Tundra,
    Grassland,
    Plains,
    Tropical,
    Desert,
    Marine
};

//--------------------------------------------------------------------------------------
enum class FertilityType : i8
{
    Random = -1,

    Dry = 0,
    Standart,
    Fertile
};

//--------------------------------------------------------------------------------------
enum class FeatureType : i8
{
    Random                          = -1,

    None                            = 0,
    SagebrushSteppe                 = 1,
    Oasis                           = 2,
    DesertFloodplainMinor           = 3,
    DesertFloodplainNavigable       = 4,
    Forest                          = 5,
    Marsh                           = 6,
    GrasslandFloodplainMinor        = 7,
    GrasslandFloodplainNavigable    = 8,
    Reef                            = 9,
    ColdReef                        = 10,
    Ice                             = 11,
    SavanaWoodland                  = 12,
    WateringHole                    = 13,
    PlainsFloodplainMinor           = 14,
    PlainsFloodplainNavigable       = 15,
    RainForest                      = 16,
    Mangrove                        = 17,
    TropicalFloodplainMinor         = 18,
    TropicalFloodplainNavigable     = 19,
    Taiga                           = 20,
    TundraBog                       = 21,
    TundraFloodplainMinor           = 22,
    TundraFloodplainNavigable       = 23,
    Volcano                         = 24,
};

//--------------------------------------------------------------------------------------
enum class ResourceType
{
    Random              = -1,

    None                = 0,
    Cotton              = 1,
    Dates               = 2,
    Dyes                = 3,
    Fish                = 4,
    Gold                = 5,
    GoldDistantLands    = 6,
    Gypsum              = 7,
    Incense             = 8,
    Ivory               = 9,
    Jade                = 10,
    Kaolin              = 11,
    Marble              = 12,
    Pearls              = 13,
    Silk                = 14,
    Silver              = 15,
    SilverDistantLands  = 16,
    Wine                = 17,
    Camels              = 18,
    Hides               = 19,
    Horses              = 20,
    Iron                = 21,
    Salt                = 22,
    Wool                = 23,
    LapisLazuli         = 24,
    Cocoa               = 25,
    Furs                = 26,
    Spices              = 27,
    Sugar               = 28,
    Tea                 = 29,
    Truffles            = 30,
    Niter               = 31,
    Cloves              = 32,
    Whales              = 33,
    Coffee              = 34,
    Tobacco             = 35,
    Citrus              = 36,
    Coal                = 37,
    Nickel              = 38,
    Oil                 = 39,
    Quinine             = 40,
    Rubber              = 41           
};

//--------------------------------------------------------------------------------------
enum class ContinentType : i8
{
    None = -1
};

//--------------------------------------------------------------------------------------
struct Civ7Tile
{
    TerrainType   terrain   = TerrainType::Flat;
    BiomeType     biome     = BiomeType::Tundra;
    FeatureType   feature   = FeatureType::Random;
    ContinentType continent = ContinentType::None;              
    ResourceType  resource  = ResourceType::Random;

    inline bool operator==(const Civ7Tile & _other) const
    {
        return terrain == _other.terrain
            && biome == _other.biome
            && feature == _other.feature
            && continent == _other.continent
            && resource == _other.resource;
    }

    inline bool operator!=(const Civ7Tile & _other) const
    {
        return !operator==(_other);
    }
};

//--------------------------------------------------------------------------------------
struct ResourceInfo
{
    bool visible = true;
    u32 count = 0;
};