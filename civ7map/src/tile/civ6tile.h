#pragma once

//--------------------------------------------------------------------------------------
enum class Civ6Terrain : i8
{
    Grass           = 0,
    Grass_Hills     = 1,
    Grass_Moutain   = 2,
    Plains          = 3,
    Plains_Hills    = 4,
    Plains_Moutain  = 5,
    Desert          = 6,    
    Desert_Hills    = 7,
    Desert_Moutain  = 8,
    Tundra          = 9,
    Tundra_Hills    = 10,
    Tundra_Moutain  = 11,
    Snow            = 12,
    Snow_Hills      = 13,
    Snow_Moutain    = 14,
    Coast           = 15,
    Ocean           = 16
};

//--------------------------------------------------------------------------------------
enum class Civ6Feature : i8
{
    None        = -1,

    FloodPlains = 0,
    Ice         = 1,
    Jungle      = 2,
    Forest      = 3,
    Oasis       = 4,
    Marsh       = 5,
    BarrierReef = 6
};

//--------------------------------------------------------------------------------------
enum class Civ6Resource : i8
{
    None            = -1,

    Bananas         = 0,
    Cattle          = 1,
    Copper          = 2,
    Crabs           = 3,
    Deer            = 4,
    Fish            = 5,
    Rice            = 6,
    Sheep           = 7,
    Stone           = 8,
    Wheat           = 9,
    Citrus          = 10,
    Cocoa           = 11,
    Coffee          = 12,
    Cotton          = 13,
    Diamonds        = 14,
    Dyes            = 15,
    Furs            = 16,
    Gypsum          = 17,
    Incense         = 18,
    Ivory           = 19,
    Jade            = 20,
    Marble          = 21,
    Mercury         = 22,
    Pearls          = 23,
    Salt            = 24,
    Silk            = 25,
    Silver          = 26,
    Spices          = 27,
    Sugar           = 28,
    Tea             = 29,
    Tobacco         = 30,
    Truffles        = 31,
    Whales          = 32,
    Wine            = 33,
    Aluminum        = 40,
    Coal            = 41,
    Horses          = 42,
    Iron            = 43,
    Niter           = 44,
    Oil             = 45,
    Uranium         = 46
};

//--------------------------------------------------------------------------------------
struct Civ6Tile
{
    Civ6Terrain     terrain = Civ6Terrain::Tundra;
    Civ6Feature     feature = Civ6Feature::None;
    Civ6Resource    resource = Civ6Resource::None;
    ContinentType   continent = ContinentType::None;
};