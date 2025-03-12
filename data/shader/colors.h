//--------------------------------------------------------------------------------------
// TerrainType
//--------------------------------------------------------------------------------------
 
// Should match enum class TerrainType in map.h
#define TerrainType_Moutain         0
#define TerrainType_Hill            1
#define TerrainType_Flat            2
#define TerrainType_Coast           3
#define TerrainType_Ocean           4
#define TerrainType_NavigableRiver  5

float4 getTerrainColor(uint terrain)
{
    switch (terrain)
    {
        default:                         return float4(1.0f, 0.0f, 1.0f, (1.0f));

        case TerrainType_Moutain:        return float4(0.9f, 0.9f, 0.9f, (1.0f));
        case TerrainType_Hill:           return float4(0.6f, 0.6f, 0.6f, (1.0f));
        case TerrainType_Flat:           return float4(0.4f, 0.4f, 0.4f, (1.0f));
        case TerrainType_Coast:          return float4(0.0f, 0.5f, 1.0f, (1.0f));
        case TerrainType_Ocean:          return float4(0.0f, 0.0f, 1.0f, (1.0f));
        case TerrainType_NavigableRiver: return float4(0.0f, 1.0f, 1.0f, (1.0f));
    }
}

#ifdef __cplusplus
float4 getTerrainColor(TerrainType terrain)
{
    return getTerrainColor((uint)terrain);
}
#endif

//--------------------------------------------------------------------------------------
// BiomeType
//--------------------------------------------------------------------------------------

// Should match enum class BiomeType in map.h
#define BiomeType_Tundra            0
#define BiomeType_Grassland         1
#define BiomeType_Plains            2
#define BiomeType_Tropical          3
#define BiomeType_Desert            4
#define BiomeType_Marine            5

float4 getBiomeColor(uint biome)
{
    switch (biome)
    {
        default:                    return float4(1.0f, 0.0f, 1.0f, (1.0f));

        case BiomeType_Tundra:      return float4(0.8f, 1.0f, 0.8f, (1.0f));
        case BiomeType_Grassland:   return float4(0.2f, 0.9f, 0.0f, (1.0f));
        case BiomeType_Plains:      return float4(0.5f, 0.8f, 0.0f, (1.0f));
        case BiomeType_Tropical:    return float4(0.0f, 0.5f, 0.0f, (1.0f));
        case BiomeType_Desert:      return float4(1.0f, 1.0f, 0.0f, (1.0f));
        case BiomeType_Marine:      return float4(0.0f, 0.0f, 1.0f, (1.0f));
    }
}

#ifdef __cplusplus
float4 getBiomeColor(BiomeType biome)
{
    return getBiomeColor((uint)biome);
}
#endif

//--------------------------------------------------------------------------------------
// Feature
//--------------------------------------------------------------------------------------

// Should match enum class FeatureType in map.h
#define FeatureType_Random                          255
#define FeatureType_None                            0
#define FeatureType_SagebrushSteppe                 1
#define FeatureType_Oasis                           2
#define FeatureType_DesertFloodplainMinor           3
#define FeatureType_DesertFloodplainNavigable       4
#define FeatureType_Forest                          5
#define FeatureType_Marsh                           6
#define FeatureType_GrasslandFloodplainMinor        7
#define FeatureType_GrasslandFloodplainNavigable    8
#define FeatureType_Reef                            9
#define FeatureType_ColdReef                        10
#define FeatureType_Ice                             11
#define FeatureType_SavanaWoodland                  12
#define FeatureType_WateringHole                    13
#define FeatureType_PlainsFloodplainMinor           14
#define FeatureType_PlainsFloodplainNavigable       15
#define FeatureType_RainForest                      16
#define FeatureType_Mangrove                        17
#define FeatureType_TropicalFloodplainMinor         18
#define FeatureType_TropicalFloodplainNavigable     19
#define FeatureType_Taiga                           20
#define FeatureType_TundraBog                       21
#define FeatureType_TundraFloodplainMinor           22
#define FeatureType_TundraFloodplainNavigable       23
#define FeatureType_Volcano                         24 

float4 getFeatureColor(uint feature)
{
    switch (feature & 0xFF)
    {
        default:                                        return float4(1.0f, 0.0f, 1.0f, (0.5f));
        case FeatureType_None:                          return float4(0.1f, 0.1f, 0.1f, (1.0f));
        case FeatureType_Random:                        return float4(0.3f, 0.3f, 0.3f, (1.0f));
        case FeatureType_SagebrushSteppe:               return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_Oasis:                         return float4(1.0f, 0.5f, 0.0f, (1.0f));
        case FeatureType_DesertFloodplainMinor:         return float4(1.0f, 1.0f, 0.5f, (1.0f));
        case FeatureType_DesertFloodplainNavigable:     return float4(1.0f, 1.0f, 0.7f, (1.0f));
        case FeatureType_Forest:                        return float4(0.0f, 0.5f, 0.0f, (1.0f));
        case FeatureType_Marsh:                         return float4(0.5f, 0.5f, 0.0f, (1.0f));
        case FeatureType_GrasslandFloodplainMinor:      return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_GrasslandFloodplainNavigable:  return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_Reef:                          return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_ColdReef:                      return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_Ice:                           return float4(0.9f, 0.9f, 1.0f, (1.0f));
        case FeatureType_SavanaWoodland:                return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_WateringHole:                  return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_PlainsFloodplainMinor:         return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_PlainsFloodplainNavigable:     return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_RainForest:                    return float4(0.0f, 0.5f, 0.4f,(1.0f));
        case FeatureType_Mangrove:                      return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_TropicalFloodplainMinor:       return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_TropicalFloodplainNavigable:   return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_Taiga:                         return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_TundraBog:                     return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_TundraFloodplainMinor:         return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_TundraFloodplainNavigable:     return float4(0.5f, 0.5f, 0.5f, (1.0f));
        case FeatureType_Volcano:                       return float4(1.0f, 0.0f, 0.0f, (1.0f));
    }
}

#ifdef __cplusplus
float4 getFeatureColor(FeatureType feature)
{
    return getFeatureColor((uint)feature);
}
#endif

//--------------------------------------------------------------------------------------
// Continent
//--------------------------------------------------------------------------------------
float4 getContinentColor(uint index)
{
    index &= 0xFF;

    if (index == 0xFF)
        return float4(0.1f, 0.1f, 0.1f, (1.0f));
    
    const float4 continentColors[6] =
    {
        float4(1,0,0,(1)),
        float4(0,1,0,(1)),
        float4(0,0,1,(1)),
        float4(1,1,0,(1)),
        float4(0,1,1,(1)),
        float4(1,0,1,(1)),
    };

    float4 color = continentColors[index % 6];
    float greyscale = 1.0f - (float(index / 6)) / 16.0f; // max 16 * 6 = 96 continents seems reasonable?
    color.r *= greyscale;
    color.g *= greyscale;
    color.b *= greyscale;
    return color;
}

#ifdef __cplusplus
float4 getContinentColor(ContinentType continent)
{
    return getContinentColor((uint)continent);
}
#endif

//--------------------------------------------------------------------------------------
// Resource
//--------------------------------------------------------------------------------------

#define ResourceType_Random              255
#define ResourceType_None                0
#define ResourceType_Cotton              1
#define ResourceType_Dates               2
#define ResourceType_Dyes                3
#define ResourceType_Fish                4
#define ResourceType_Gold                5
#define ResourceType_GoldDistantLands    6
#define ResourceType_Gypsum              7
#define ResourceType_Incense             8
#define ResourceType_Ivory               9
#define ResourceType_Jade                10
#define ResourceType_Kaolin              11
#define ResourceType_Marble              12
#define ResourceType_Pearls              13
#define ResourceType_Silk                14
#define ResourceType_Silver              15
#define ResourceType_SilverDistantLands  16
#define ResourceType_Wine                17
#define ResourceType_Camels              18
#define ResourceType_Hides               19
#define ResourceType_Horses              20
#define ResourceType_Iron                21
#define ResourceType_Salt                22
#define ResourceType_Wool                23
#define ResourceType_LapisLazuli         24
#define ResourceType_Cocoa               25
#define ResourceType_Furs                26
#define ResourceType_Spices              27
#define ResourceType_Sugar               28
#define ResourceType_Tea                 29
#define ResourceType_Truffles            30
#define ResourceType_Niter               31
#define ResourceType_Cloves              32
#define ResourceType_Whales              33
#define ResourceType_Coffee              34
#define ResourceType_Tobacco             35
#define ResourceType_Citrus              36
#define ResourceType_Coal                37
#define ResourceType_Nickel              38
#define ResourceType_Oil                 39
#define ResourceType_Quinine             40
#define ResourceType_Rubber              41 

float4 getResourceColor(uint index)
{
    switch (index & 0xFF)
    {
        default: 
            return float4(1.0f, 0.0f, 1.0f, (0.5f));

        case ResourceType_None:
            return float4(0.0f, 0.0f, 0.0f, 1.0f);

        case ResourceType_Random:
            return float4(0.2f, 0.2f, 0.2f, 1.0f); 

        case ResourceType_Cotton:
        case ResourceType_Wool:
            return float4(0.9f, 0.9f, 0.9f, 1.0f); // White/Greyish for fibers

        case ResourceType_Dates:
        case ResourceType_Cocoa:
        case ResourceType_Sugar:
            return float4(0.6f, 0.3f, 0.1f, 1.0f); // Brown for sweet products

        case ResourceType_Dyes:
            return float4(0.5f, 0.0f, 0.5f, 1.0f); // Purple for dye

        case ResourceType_Fish:
        case ResourceType_Whales:
            return float4(0.0f, 0.3f, 0.8f, 1.0f); // Blue for marine resources

        case ResourceType_Gold:
        case ResourceType_GoldDistantLands:
            return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow for gold

        case ResourceType_Silver:
        case ResourceType_SilverDistantLands:
            return float4(0.75f, 0.75f, 0.75f, 1.0f); // Silver/Grey

        case ResourceType_Gypsum:
        case ResourceType_Marble:
            return float4(0.8f, 0.8f, 0.8f, 1.0f); // Light grey for stones

        case ResourceType_Incense:
        case ResourceType_Spices:
        case ResourceType_Cloves:
        case ResourceType_Tea:
            return float4(0.7f, 0.4f, 0.2f, 1.0f); // Warm brown for aromatic goods

        case ResourceType_Ivory:
            return float4(1.0f, 1.0f, 0.9f, 1.0f); // Off-white for ivory

        case ResourceType_Jade:
            return float4(0.0f, 0.6f, 0.2f, 1.0f); // Green for jade

        case ResourceType_Kaolin:
            return float4(1.0f, 0.9f, 0.8f, 1.0f); // Light beige for clay

        case ResourceType_Pearls:
            return float4(0.9f, 0.9f, 1.0f, 1.0f); // Soft pearl color

        case ResourceType_Silk:
            return float4(1.0f, 0.8f, 0.6f, 1.0f); // Light golden for silk

        case ResourceType_Wine:
            return float4(0.5f, 0.0f, 0.0f, 1.0f); // Deep red for wine

        case ResourceType_Camels:
        case ResourceType_Horses:
            return float4(0.8f, 0.6f, 0.3f, 1.0f); // Sandy brown for animals

        case ResourceType_Hides:
        case ResourceType_Furs:
            return float4(0.6f, 0.4f, 0.2f, 1.0f); // Dark brown for leather/fur

        case ResourceType_Iron:
        case ResourceType_Nickel:
            return float4(0.5f, 0.5f, 0.5f, 1.0f); // Grey for metals

        case ResourceType_Salt:
            return float4(1.0f, 1.0f, 1.0f, 1.0f); // White for salt

        case ResourceType_LapisLazuli:
            return float4(0.0f, 0.2f, 0.8f, 1.0f); // Deep blue for lapis lazuli

        case ResourceType_Tobacco:
        case ResourceType_Coffee:
            return float4(0.4f, 0.2f, 0.1f, 1.0f); // Dark brown for tobacco/coffee

        case ResourceType_Citrus:
            return float4(1.0f, 0.7f, 0.0f, 1.0f); // Orange for citrus

        case ResourceType_Coal:
            return float4(0.2f, 0.2f, 0.2f, 1.0f); // Black for coal

        case ResourceType_Oil:
            return float4(0.1f, 0.1f, 0.1f, 1.0f); // Black for oil

        case ResourceType_Quinine:
            return float4(0.8f, 0.9f, 0.7f, 1.0f); // Pale greenish-yellow

        case ResourceType_Rubber:
            return float4(0.3f, 0.3f, 0.1f, 1.0f); // Dark rubber color

        case ResourceType_Niter:
            return float4(1.0f, 0.9f, 0.6f, 1.0f); // Yellowish-white for nitrate

        case ResourceType_Truffles:
            return float4(0.4f, 0.3f, 0.2f, 1.0f); // Dark earthy brown for truffles
    }
}

#ifdef __cplusplus
float4 getResourceColor(ResourceType res)
{
    return getResourceColor((uint)res);
}
#endif
