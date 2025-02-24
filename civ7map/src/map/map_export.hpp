//--------------------------------------------------------------------------------------
// TODO: Double hashtable TerrainType <=> Civ7TerrainString?
//--------------------------------------------------------------------------------------
string getTerrainTypeAsString(TerrainType _type)
{
    switch (_type)
    {
        case TerrainType::Moutain:
            return "\"TERRAIN_MOUNTAIN\"";
        case TerrainType::Hill:
            return "\"TERRAIN_HILL\"";
        default:
            LOG_WARNING("Unknow TerrainType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case TerrainType::Flat:
            return "\"TERRAIN_FLAT\"";
        case TerrainType::Coast:
            return "\"TERRAIN_COAST\"";
        case TerrainType::Ocean:
            return "\"TERRAIN_OCEAN\"";
        case TerrainType::NavigableRiver:
            return "\"TERRAIN_NAVIGABLE_RIVER\"";
    }
}

//--------------------------------------------------------------------------------------
string getBiomeTypeAsString(BiomeType _type)
{
    switch (_type)
    {
        case BiomeType::Tundra:
            return "\"BIOME_TUNDRA\"";
        case BiomeType::Grassland:
            return "\"BIOME_GRASSLAND\"";
        case BiomeType::Plains:
            return "\"BIOME_PLAINS\"";
        case BiomeType::Tropical:
            return "\"BIOME_TROPICAL\"";
        case BiomeType::Desert:
            return "\"BIOME_DESERT\"";
        default:
            LOG_WARNING("Unknow BiomeType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case BiomeType::Marine:
            return "\"BIOME_MARINE\"";
    }
}

//--------------------------------------------------------------------------------------
string getFeatureTypeAsString(FeatureType _type)
{
    switch (_type)
    {
        default:
            LOG_WARNING("Unknow FeatureType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case FeatureType::None:
            return "-1";

        case FeatureType::SagebrushSteppe:
            return "\"FEATURE_SAGEBRUSH_STEPPE\"";
        case FeatureType::Oasis:
            return "\"FEATURE_OASIS\"";
        case FeatureType::DesertFloodplainMinor:
            return "\"FEATURE_DESERT_FLOODPLAIN_MINOR\"";
        case FeatureType::DesertFloodplainNavigable:
            return "\"FEATURE_DESERT_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Forest:
            return "\"FEATURE_FOREST\"";
        case FeatureType::Marsh:
            return "\"FEATURE_MARSH\"";
        case FeatureType::GrasslandFloodplainMinor:
            return "\"FEATURE_GRASSLAND_FLOODPLAIN_MINOR\"";
        case FeatureType::GrasslandFloodplainNavigable:
            return "\"FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Reef:
            return "\"FEATURE_REEF\"";
        case FeatureType::ColdReef:
            return "\"FEATURE_COLD_REEF\"";
        case FeatureType::Ice:
            return "\"FEATURE_ICE\"";
        case FeatureType::SavanaWoodland:
            return "\"FEATURE_SAVANAWOODLAND\"";
        case FeatureType::WateringHole:
            return "\"FEATURE_WATERINGHOLE\"";
        case FeatureType::PlainsFloodplainMinor:
            return "\"FEATURE_PLAINS_FLOODPLAIN_MINOR\"";
        case FeatureType::PlainsFloodplainNavigable:
            return "\"FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::RainForest:
            return "\"FEATURE_RAIN_FOREST\"";
        case FeatureType::Mangrove:
            return "\"FEATURE_MANGROVE\"";
        case FeatureType::TropicalFloodplainMinor:
            return "\"FEATURE_TROPICAL_FLOODPLAIN_MINOR\"";
        case FeatureType::TropicalFloodplainNavigable:
            return "\"FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Taiga:
            return "\"FEATURE_TAIGA\"";
        case FeatureType::TundraBog:
            return "\"FEATURE_TUNDRA_BOG\"";
        case FeatureType::TundraFloodplainMinor:
            return "\"FEATURE_TUNDRA_FLOODPLAIN_MINOR\"";
        case FeatureType::TundraFloodplainNavigable:
            return "\"FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Volcano:
            return "\"FEATURE_VOLCANO\"";
    }
}


//--------------------------------------------------------------------------------------
void Map::exportCiv7Map(string & _map, const string & _cwd)
{
    string data = fmt::sprintf
    (
        "export function GetMap2() {\n"
        "\n"
        "    let MapToConvert2 = [];\n"
        "    for (let i = 0; i < %u; i++) {\n"
        "        MapToConvert2[i] = [];\n"
        "    }\n"
        "\n"
        "    // Map Data (Civ7)\n"
        "    // MapToConvert2 = { civ7TerrainType, civ7BiomeType, civ7FeatureType }\n"
        "\n"
        "\n",
        m_width
    );

    for (uint j = 0; j < m_height; ++j)
    {
        for (uint i = 0; i < m_width; ++i)
        {
            const Civ7Tile & tile = civ7TerrainType.get(i, j);

            data += fmt::sprintf("MapToConvert2[%u][%u]=[%s, %s, %s]\n", i, j, getTerrainTypeAsString(tile.terrain), getBiomeTypeAsString(tile.biome), getFeatureTypeAsString(tile.feature));
        }
    }

    data +=
        "\n"
        "    return MapToConvert2;\n"
        "}"
        "\n"
        "//# sourceMappingURL=file:///base-standard/maps/continents-plus.js.map\n";

    FILE * fp = fopen(_map.c_str(), "wb");
    
    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
}

//--------------------------------------------------------------------------------------
void Map::saveBitmap(const Array2D<u32> _bitmap, XMLElement * _xmlTerrainSave, const string & _field)
{
    XMLElement * xmlBytes = _xmlTerrainSave->FirstChildElement((_field + ".Bytes").c_str());

    if (xmlBytes)
    {
        int len = 0;
        ubyte * png = stbi_write_png_to_mem((const ubyte*)_bitmap.data(), m_width * sizeof(u32), m_width, m_height, 4, &len);
        string pngBase64 = base64::encode((char*)png, (size_t)len);

        xmlBytes->SetAttribute("Length", len);
        xmlBytes->FirstChild()->SetValue(pngBase64.c_str());
    }
}