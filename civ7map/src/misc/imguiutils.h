#pragma once

enum class ContinentType : i8;
enum class TerrainType : u8;
enum class BiomeType : u8;
enum class FeatureType : u8;
enum class ResourceType : u8;
enum class NaturalWonderType : u8;
enum class LandmassType : u8;
enum class Elevation : u16;
enum class RiverType : u8;
enum class DirectionType : i8;
enum class RiverName : i8;

class Map;

string GetContinentShortName(const Map * map, ContinentType continent);
bool EditContinent(const Map * map, ContinentType & continent, bool * pBool = nullptr);
bool EditLandmass(const Map * map, LandmassType & landmass, bool * pBool = nullptr);
bool EditTerrain(const Map * map, TerrainType & terrain, bool * pBool = nullptr);
bool EditBiome(const Map * map, BiomeType & biome, bool * pBool = nullptr);
bool EditFeature(const Map * map, FeatureType & feature, bool * pBool = nullptr);
bool EditResource(const Map * map, ResourceType & resource, bool * pBool = nullptr);
bool EditNaturalWonder(const Map * map, NaturalWonderType & naturalWonder, bool * pBool = nullptr);
bool EditElevation(const Map * map, Elevation & elevation, bool * pBool = nullptr);
bool EditRiverName(const Map * map, RiverName & riverIndex, bool * pBool = nullptr);
bool EditRiverType(const Map * map, RiverType & river, bool * pBool = nullptr);
bool EditDirectionType(const Map * map, DirectionType & direction, bool * pBool = nullptr);

void DrawColoredSquare(const float4 & _color);
void DrawSmallColoredSquare(const float4 & _color);
bool DrawColoredCheckbox(const float4 & _color, bool * _checked);

void PushDisabled(bool _disabled);
void PopDisabled();

void ApplyDisabledStyle(bool _disabled);
