#pragma once

//--------------------------------------------------------------------------------------
enum class TerrainType
{
    Moutain = 0,
    Hill,
    Flat,
    Coast,
    Ocean,
    NavigableRiver
};

//--------------------------------------------------------------------------------------
enum class BiomeType
{
    Tundra,
    Grassland,
    Plains,
    Tropical,
    Desert,
    Marine
};

//--------------------------------------------------------------------------------------
struct Civ7Tile
{
    TerrainType terrainType = TerrainType::Flat;
    BiomeType biomeType = BiomeType::Tundra;
    u32 featureType = (u32)-1;
    u32 continentIndex = (u32)-1;
};