#pragma once

//--------------------------------------------------------------------------------------
enum class Civ6Terrain : u8
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
    Ocean           = 16,

    Default         = Tundra
};

//--------------------------------------------------------------------------------------
enum class Civ6Feature : u8
{
    FloodPlains = 0,
    Ice         = 1,
    Jungle      = 2,
    Forest      = 3,
    Oasis       = 4,
    Marsh       = 5,
    BarrierReef = 6,

    None        = 0xFF,
    Default     = None,
};

//--------------------------------------------------------------------------------------
struct Civ6Tile
{
    Civ6Terrain terrain;
    Civ6Feature feature;
};