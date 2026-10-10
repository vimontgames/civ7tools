//--------------------------------------------------------------------------------------
// TODO: Double hashtable TerrainType <=> Civ7TerrainString?
//--------------------------------------------------------------------------------------
string Map::getTerrainTypeAsString(TerrainType _type)
{
    switch (_type)
    {
        case TerrainType::Moutain:
            return "TERRAIN_MOUNTAIN";
        case TerrainType::Hill:
            return "TERRAIN_HILL";
        default:
            LOG_WARNING("Unknow TerrainType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case TerrainType::Flat:
            return "TERRAIN_FLAT";
        case TerrainType::Coast:
            return "TERRAIN_COAST";
        case TerrainType::Ocean:
            return "TERRAIN_OCEAN";
        case TerrainType::NavigableRiver:
            return "TERRAIN_NAVIGABLE_RIVER";
    }
}

//--------------------------------------------------------------------------------------
TerrainType Map::getTerrainTypeFromString(const string & _terrainTypeS, uint x, uint y)
{
    if (_terrainTypeS == "TERRAIN_MOUNTAIN")
        return TerrainType::Moutain;
    else if (_terrainTypeS == "TERRAIN_HILL")
        return TerrainType::Hill;
    else if (_terrainTypeS == "TERRAIN_FLAT")
        return TerrainType::Flat;
    else if (_terrainTypeS == "TERRAIN_COAST")
        return TerrainType::Coast;
    else if (_terrainTypeS == "TERRAIN_OCEAN")
        return TerrainType::Ocean;
    else if (_terrainTypeS == "TERRAIN_NAVIGABLE_RIVER")
        return TerrainType::NavigableRiver;

    LOG_WARNING("Unknow TerrainType \"%s\" at (%u,%u)", _terrainTypeS.c_str(), x, y);
    return TerrainType::Ocean;
}

//--------------------------------------------------------------------------------------
string Map::getBiomeTypeAsString(BiomeType _type)
{
    switch (_type)
    {
        case BiomeType::Tundra:
            return "BIOME_TUNDRA";
        case BiomeType::Grassland:
            return "BIOME_GRASSLAND";
        case BiomeType::Plains:
            return "BIOME_PLAINS";
        case BiomeType::Tropical:
            return "BIOME_TROPICAL";
        case BiomeType::Desert:
            return "BIOME_DESERT";
        default:
            LOG_WARNING("Unknow BiomeType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case BiomeType::Marine:
            return "BIOME_MARINE";
    }
}

//--------------------------------------------------------------------------------------
BiomeType Map::getBiomeTypeFromString(const string & _biomeTypeS, uint x, uint y)
{
    if (_biomeTypeS == "BIOME_TUNDRA")
        return BiomeType::Tundra;
    else if (_biomeTypeS == "BIOME_GRASSLAND")
        return  BiomeType::Grassland;
    else if (_biomeTypeS == "BIOME_PLAINS")
        return BiomeType::Plains;
    else if (_biomeTypeS == "BIOME_TROPICAL")
        return  BiomeType::Tropical;
    else if (_biomeTypeS == "BIOME_DESERT")
        return  BiomeType::Desert;
    else if (_biomeTypeS == "BIOME_MARINE")
        return BiomeType::Marine;

    LOG_WARNING("Unknow BiomeType \"%s\" (%u,%u)", _biomeTypeS, x, y);
    return BiomeType::Marine;
}

//--------------------------------------------------------------------------------------
string Map::getFeatureTypeAsString(FeatureType _type)
{
    switch (_type)
    {
        default:
            LOG_WARNING("Unknow FeatureType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case FeatureType::Random:
            return "-1";
        case FeatureType::None:
            return "0";
        case FeatureType::SagebrushSteppe:
            return "FEATURE_SAGEBRUSH_STEPPE";
        case FeatureType::Oasis:
            return "FEATURE_OASIS";
        case FeatureType::DesertFloodplainMinor:
            return "FEATURE_DESERT_FLOODPLAIN_MINOR";
        case FeatureType::DesertFloodplainNavigable:
            return "FEATURE_DESERT_FLOODPLAIN_NAVIGABLE";
        case FeatureType::Forest:
            return "FEATURE_FOREST";
        case FeatureType::Marsh:
            return "FEATURE_MARSH";
        case FeatureType::GrasslandFloodplainMinor:
            return "FEATURE_GRASSLAND_FLOODPLAIN_MINOR";
        case FeatureType::GrasslandFloodplainNavigable:
            return "FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE";
        case FeatureType::Reef:
            return "FEATURE_REEF";
        case FeatureType::ColdReef:
            return "FEATURE_COLD_REEF";
        case FeatureType::Ice:
            return "FEATURE_ICE";
        case FeatureType::SavannaWoodland:
            return "FEATURE_SAVANNA_WOODLAND";
        case FeatureType::WateringHole:
            return "FEATURE_WATERING_HOLE";
        case FeatureType::PlainsFloodplainMinor:
            return "FEATURE_PLAINS_FLOODPLAIN_MINOR";
        case FeatureType::PlainsFloodplainNavigable:
            return "FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE";
        case FeatureType::RainForest:
            return "FEATURE_RAINFOREST";
        case FeatureType::Mangrove:
            return "FEATURE_MANGROVE";
        case FeatureType::TropicalFloodplainMinor:
            return "FEATURE_TROPICAL_FLOODPLAIN_MINOR";
        case FeatureType::TropicalFloodplainNavigable:
            return "FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE";
        case FeatureType::Taiga:
            return "FEATURE_TAIGA";
        case FeatureType::TundraBog:
            return "FEATURE_TUNDRA_BOG";
        case FeatureType::TundraFloodplainMinor:
            return "FEATURE_TUNDRA_FLOODPLAIN_MINOR";
        case FeatureType::TundraFloodplainNavigable:
            return "FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE";
        case FeatureType::Volcano:
            return "FEATURE_VOLCANO";
    }
}

//--------------------------------------------------------------------------------------
FeatureType Map::getFeatureFromString(const string & _featureTypeS, uint x, uint y)
{
    if (_featureTypeS == "FEATURE_SAGEBRUSH_STEPPE")
        return FeatureType::SagebrushSteppe;
    else if (_featureTypeS == "FEATURE_OASIS")
        return FeatureType::Oasis;
    else if (_featureTypeS == "FEATURE_DESERT_FLOODPLAIN_MINOR")
        return FeatureType::DesertFloodplainMinor;
    else if (_featureTypeS == "FEATURE_DESERT_FLOODPLAIN_NAVIGABLE")
        return FeatureType::DesertFloodplainNavigable;
    else if (_featureTypeS == "FEATURE_FOREST")
        return FeatureType::Forest;
    else if (_featureTypeS == "FEATURE_MARSH")
        return FeatureType::Marsh;
    else if (_featureTypeS == "FEATURE_GRASSLAND_FLOODPLAIN_MINOR")
        return FeatureType::GrasslandFloodplainMinor;
    else if (_featureTypeS == "FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE")
        return FeatureType::GrasslandFloodplainNavigable;
    else if (_featureTypeS == "FEATURE_REEF")
        return FeatureType::Reef;
    else if (_featureTypeS == "FEATURE_COLD_REEF")
        return FeatureType::ColdReef;
    else if (_featureTypeS == "FEATURE_ICE")
        return FeatureType::Ice;
    else if (_featureTypeS == "FEATURE_SAVANNA_WOODLAND")
        return FeatureType::SavannaWoodland;
    else if (_featureTypeS == "FEATURE_WATERING_HOLE")
        return FeatureType::WateringHole;
    else if (_featureTypeS == "FEATURE_PLAINS_FLOODPLAIN_MINOR")
        return FeatureType::PlainsFloodplainMinor;
    else if (_featureTypeS == "FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE")
        return FeatureType::PlainsFloodplainNavigable;
    else if (_featureTypeS == "FEATURE_RAINFOREST")
        return FeatureType::RainForest;
    else if (_featureTypeS == "FEATURE_MANGROVE")
        return FeatureType::Mangrove;
    else if (_featureTypeS == "FEATURE_TROPICAL_FLOODPLAIN_MINOR")
        return FeatureType::TropicalFloodplainMinor;
    else if (_featureTypeS == "FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE")
        return FeatureType::TropicalFloodplainNavigable;
    else if (_featureTypeS == "FEATURE_TAIGA")
        return FeatureType::Taiga;
    else if (_featureTypeS == "FEATURE_TUNDRA_BOG")
        return FeatureType::TundraBog;
    else if (_featureTypeS == "FEATURE_TUNDRA_FLOODPLAIN_MINOR")
        return FeatureType::TundraFloodplainMinor;
    else if (_featureTypeS == "FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE")
        return FeatureType::TundraFloodplainNavigable;
    else if (_featureTypeS == "FEATURE_VOLCANO")
        return FeatureType::Volcano;
    else if (_featureTypeS == "FEATURE_ATOLL")
        return FeatureType::Atoll;
    else if (_featureTypeS == "FEATURE_LOTUS")
        return FeatureType::Lotus;

    LOG_WARNING("Unknow FeatureType \"%s\" (%u,%u)", _featureTypeS.c_str(), x, y);
    return FeatureType::None;
}

//--------------------------------------------------------------------------------------
string Map::getNaturalWonderTypeAsString(NaturalWonderType _type)
{
    string result = "FEATURE_";
    string name = asString(_type);

    for (char c : name)
    {
        if (isupper((char)(c)) && result.size() > strlen("FEATURE_"))
            result += '_';

        result += (char)(toupper((char)(c)));
    }

    return result;
}

//--------------------------------------------------------------------------------------
string Map::getResourceTypeAsString(ResourceType _type)
{
    switch (_type)
    {
        default:
            LOG_WARNING("Unknown ResourceType \"%d\"", static_cast<int>(_type));
        case ResourceType::Random:
            return "-1";
        case ResourceType::None:
            return "0";
        case ResourceType::Cotton:
            return "RESOURCE_COTTON";
        case ResourceType::Dates:
            return "RESOURCE_DATES";
        case ResourceType::Dyes:
            return "RESOURCE_DYES";
        case ResourceType::Fish:
            return "RESOURCE_FISH";
        case ResourceType::Gold:
            return "RESOURCE_GOLD";
        case ResourceType::GoldDistantLands:
            return "RESOURCE_GOLD_DISTANT_LANDS";
        case ResourceType::Gypsum:
            return "RESOURCE_GYPSUM";
        case ResourceType::Incense:
            return "RESOURCE_INCENSE";
        case ResourceType::Ivory:
            return "RESOURCE_IVORY";
        case ResourceType::Jade:
            return "RESOURCE_JADE";
        case ResourceType::Kaolin:
            return "RESOURCE_KAOLIN";
        case ResourceType::Marble:
            return "RESOURCE_MARBLE";
        case ResourceType::Pearls:
            return "RESOURCE_PEARLS";
        case ResourceType::Silk:
            return "RESOURCE_SILK";
        case ResourceType::Silver:
            return "RESOURCE_SILVER";
        case ResourceType::SilverDistantLands:
            return "RESOURCE_SILVER_DISTANT_LANDS";
        case ResourceType::Wine:
            return "RESOURCE_WINE";
        case ResourceType::Camels:
            return "RESOURCE_CAMELS";
        case ResourceType::Hides:
            return "RESOURCE_HIDES";
        case ResourceType::Horses:
            return "RESOURCE_HORSES";
        case ResourceType::Iron:
            return "RESOURCE_IRON";
        case ResourceType::Salt:
            return "RESOURCE_SALT";
        case ResourceType::Wool:
            return "RESOURCE_WOOL";
        case ResourceType::LapisLazuli:
            return "RESOURCE_LAPIS_LAZULI";
        case ResourceType::Cocoa:
            return "RESOURCE_COCOA";
        case ResourceType::Furs:
            return "RESOURCE_FURS";
        case ResourceType::Spices:
            return "RESOURCE_SPICES";
        case ResourceType::Sugar:
            return "RESOURCE_SUGAR";
        case ResourceType::Tea:
            return "RESOURCE_TEA";
        case ResourceType::Truffles:
            return "RESOURCE_TRUFFLES";
        case ResourceType::Niter:
            return "RESOURCE_NITER";
        case ResourceType::Cloves:
            return "RESOURCE_CLOVES";
        case ResourceType::Whales:
            return "RESOURCE_WHALES";
        case ResourceType::Coffee:
            return "RESOURCE_COFFEE";
        case ResourceType::Tobacco:
            return "RESOURCE_TOBACCO";
        case ResourceType::Citrus:
            return "RESOURCE_CITRUS";
        case ResourceType::Coal:
            return "RESOURCE_COAL";
        case ResourceType::Nickel:
            return "RESOURCE_NICKEL";
        case ResourceType::Oil:
            return "RESOURCE_OIL";
        case ResourceType::Quinine:
            return "RESOURCE_QUININE";
        case ResourceType::Rubber:
            return "RESOURCE_RUBBER";
        case ResourceType::Mangos:
            return "RESOURCE_MANGOS";
        case ResourceType::Clay:
            return "RESOURCE_CLAY";
        case ResourceType::Flax:
            return "RESOURCE_FLAX";
        case ResourceType::Rubies:
            return "RESOURCE_RUBIES";
        case ResourceType::Rice:
            return "RESOURCE_RICE";
        case ResourceType::Limestone:
            return "RESOURCE_LIMESTONE";
        case ResourceType::Tin:
            return "RESOURCE_TIN";
        case ResourceType::Llamas:
            return "RESOURCE_LLAMAS";
        case ResourceType::Hardwood:
            return "RESOURCE_HARDWOOD";
        case ResourceType::WildGame:
            return "RESOURCE_WILD_GAME";
        case ResourceType::Crabs:
            return "RESOURCE_CRABS";
        case ResourceType::Cowrie:
            return "RESOURCE_COWRIE";
        case ResourceType::Turtles:
            return "RESOURCE_TURTLES";
        case ResourceType::Pitch:
            return "RESOURCE_PITCH";
    }
}

//--------------------------------------------------------------------------------------
ResourceType Map::getResourceFromString(const string & _resourceTypeS, uint x, uint y)
{
    if (_resourceTypeS == "RESOURCE_COTTON")
        return ResourceType::Cotton;
    else if (_resourceTypeS == "RESOURCE_DATES")
        return ResourceType::Dates;
    else if (_resourceTypeS == "RESOURCE_DYES")
        return ResourceType::Dyes;
    else if (_resourceTypeS == "RESOURCE_FISH")
        return ResourceType::Fish;
    else if (_resourceTypeS == "RESOURCE_GOLD")
        return ResourceType::Gold;
    else if (_resourceTypeS == "RESOURCE_GOLD_DISTANT_LANDS")
        return ResourceType::GoldDistantLands;
    else if (_resourceTypeS == "RESOURCE_GYPSUM")
        return ResourceType::Gypsum;
    else if (_resourceTypeS == "RESOURCE_INCENSE")
        return ResourceType::Incense;
    else if (_resourceTypeS == "RESOURCE_IVORY")
        return ResourceType::Ivory;
    else if (_resourceTypeS == "RESOURCE_JADE")
        return ResourceType::Jade;
    else if (_resourceTypeS == "RESOURCE_KAOLIN")
        return ResourceType::Kaolin;
    else if (_resourceTypeS == "RESOURCE_MARBLE")
        return ResourceType::Marble;
    else if (_resourceTypeS == "RESOURCE_PEARLS")
        return ResourceType::Pearls;
    else if (_resourceTypeS == "RESOURCE_SILK")
        return ResourceType::Silk;
    else if (_resourceTypeS == "RESOURCE_SILVER")
        return ResourceType::Silver;
    else if (_resourceTypeS == "RESOURCE_SILVER_DISTANT_LANDS")
        return ResourceType::SilverDistantLands;
    else if (_resourceTypeS == "RESOURCE_WINE")
        return ResourceType::Wine;
    else if (_resourceTypeS == "RESOURCE_CAMELS")
        return ResourceType::Camels;
    else if (_resourceTypeS == "RESOURCE_HIDES")
        return ResourceType::Hides;
    else if (_resourceTypeS == "RESOURCE_HORSES")
        return ResourceType::Horses;
    else if (_resourceTypeS == "RESOURCE_IRON")
        return ResourceType::Iron;
    else if (_resourceTypeS == "RESOURCE_SALT")
        return ResourceType::Salt;
    else if (_resourceTypeS == "RESOURCE_WOOL")
        return ResourceType::Wool;
    else if (_resourceTypeS == "RESOURCE_LAPIS_LAZULI")
        return ResourceType::LapisLazuli;
    else if (_resourceTypeS == "RESOURCE_COCOA")
        return ResourceType::Cocoa;
    else if (_resourceTypeS == "RESOURCE_FURS")
        return ResourceType::Furs;
    else if (_resourceTypeS == "RESOURCE_SPICES")
        return ResourceType::Spices;
    else if (_resourceTypeS == "RESOURCE_SUGAR")
        return ResourceType::Sugar;
    else if (_resourceTypeS == "RESOURCE_TEA")
        return ResourceType::Tea;
    else if (_resourceTypeS == "RESOURCE_TRUFFLES")
        return ResourceType::Truffles;
    else if (_resourceTypeS == "RESOURCE_NITER")
        return ResourceType::Niter;
    else if (_resourceTypeS == "RESOURCE_CLOVES")
        return ResourceType::Cloves;
    else if (_resourceTypeS == "RESOURCE_WHALES")
        return ResourceType::Whales;
    else if (_resourceTypeS == "RESOURCE_COFFEE")
        return ResourceType::Coffee;
    else if (_resourceTypeS == "RESOURCE_TOBACCO")
        return ResourceType::Tobacco;
    else if (_resourceTypeS == "RESOURCE_CITRUS")
        return ResourceType::Citrus;
    else if (_resourceTypeS == "RESOURCE_COAL")
        return ResourceType::Coal;
    else if (_resourceTypeS == "RESOURCE_NICKEL")
        return ResourceType::Nickel;
    else if (_resourceTypeS == "RESOURCE_OIL")
        return ResourceType::Oil;
    else if (_resourceTypeS == "RESOURCE_QUININE")
        return ResourceType::Quinine;
    else if (_resourceTypeS == "RESOURCE_RUBBER")
        return ResourceType::Rubber;
    else if (_resourceTypeS == "RESOURCE_MANGOS")
        return ResourceType::Mangos;
    else if (_resourceTypeS == "RESOURCE_CLAY")
        return ResourceType::Clay;
    else if (_resourceTypeS == "RESOURCE_FLAX")
        return ResourceType::Flax;
    else if (_resourceTypeS == "RESOURCE_RUBIES")
        return ResourceType::Rubies;
    else if (_resourceTypeS == "RESOURCE_RICE")
        return ResourceType::Rice;
    else if (_resourceTypeS == "RESOURCE_LIMESTONE")
        return ResourceType::Limestone;
    else if (_resourceTypeS == "RESOURCE_TIN")
        return ResourceType::Tin;
    else if (_resourceTypeS == "RESOURCE_LLAMAS")
        return ResourceType::Llamas;
    else if (_resourceTypeS == "RESOURCE_HARDWOOD")
        return ResourceType::Hardwood;
    else if (_resourceTypeS == "RESOURCE_WILD_GAME")
        return ResourceType::WildGame;
    else if (_resourceTypeS == "RESOURCE_CRABS")
        return ResourceType::Crabs;
    else if (_resourceTypeS == "RESOURCE_COWRIE")
        return ResourceType::Cowrie;
    else if (_resourceTypeS == "RESOURCE_TURTLES")
        return ResourceType::Turtles;
    else if (_resourceTypeS == "RESOURCE_PITCH")
        return ResourceType::Pitch;

    LOG_WARNING("Unknown ResourceType \"%s\" at (%u, %u)", _resourceTypeS.c_str(), x, y);
    return ResourceType::None;
}
