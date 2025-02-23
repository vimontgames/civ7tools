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
enum class FeatureType : i8
{
    None                            = -1,

    SagebrushSteppe                 = 0,
    Oasis                           = 1,
    DesertFloodplainMinor           = 2,
    DesertFloodplainNavigable       = 3,
    Forest                          = 4,
    Marsh                           = 5,
    GrasslandFloodplainMinor        = 6,
    GrasslandFloodplainNavigable    = 7,
    Reef                            = 8,
    ColdReef                        = 9,
    Ice                             = 10,
    SavanaWoodland                  = 11,
    WateringHole                    = 12,
    PlainsFloodplainMinor           = 13,
    PlainsFloodplainNavigable       = 14,
    RainForest                      = 15,
    Mangrove                        = 16,
    TropicalFloodplainMinor         = 17,
    TropicalFloodplainNavigable     = 18,
    Taiga                           = 19,
    TundraBog                       = 20,
    TundraFloodplainMinor           = 21,
    TundraFloodplainNavigable       = 22,
    Volcano,
};

//--------------------------------------------------------------------------------------
enum class ResourceType
{
    None                = -1,

    Cotton              = 0,
    Dates               = 1,
    Dyes                = 2,
    Fish                = 3,
    Gold                = 4,
    GoldDistantLands    = 5,
    Gypsum              = 6,
    Incense             = 7,
    Ivory               = 8,
    Jade                = 9,
    Kaolin              = 10,
    Marble              = 11,
    Pearls              = 12,
    Silk                = 13,
    Silver              = 14,
    SilverDistantLands  = 15,
    Wine                = 16,
    Camels              = 17,
    Hides               = 18,
    Horses              = 19,
    Iron                = 20,
    Salt                = 21,
    Wool                = 22,
    LapisLazuli         = 23,
    Cocoa               = 24,
    Furs                = 25,
    Spices              = 26,
    Sugar               = 27,
    Tea                 = 28,
    Truffles            = 29,
    Niter               = 30,
    Cloves              = 31,
    Whales              = 32,
    Coffee              = 33,
    Tobacco             = 34,
    Citrus              = 35,
    Coal                = 36,
    Nickel              = 37,
    Oil                 = 38,
    Quinine             = 39,
    Rubber              = 40
};

//--------------------------------------------------------------------------------------
enum class ContinentType : i8
{
    None = -1
};

//--------------------------------------------------------------------------------------
struct Civ7Tile
{
    TerrainType  terrain   = TerrainType::Flat;
    BiomeType    biome     = BiomeType::Tundra;
    FeatureType  feature   = FeatureType::None;
    ContinentType    continent = ContinentType::None;
                           
    ResourceType resource  = ResourceType::None;
};