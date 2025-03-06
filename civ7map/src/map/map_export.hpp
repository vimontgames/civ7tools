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
        case FeatureType::SavanaWoodland:
            return "FEATURE_SAVANAWOODLAND";
        case FeatureType::WateringHole:
            return "FEATURE_WATERINGHOLE";
        case FeatureType::PlainsFloodplainMinor:
            return "FEATURE_PLAINS_FLOODPLAIN_MINOR";
        case FeatureType::PlainsFloodplainNavigable:
            return "FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE";
        case FeatureType::RainForest:
            return "FEATURE_RAIN_FOREST";
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
std::string Map::getResourceTypeAsString(ResourceType _type)
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
    }
}

//--------------------------------------------------------------------------------------
// Add extra brackets for all values that are not numbers
//--------------------------------------------------------------------------------------
string exportValue(const string & _value)
{
    if (isNumber(_value))
        return _value;
    else
        return "\"" + _value + "\"";
}

//--------------------------------------------------------------------------------------
void Map::exportMap(const string & _cwd)
{
    string data = fmt::sprintf
    (
        "/*\n"
        "\n"
        "--	FILE:	 GreatestEarthMap\n"
        "--  made by djvandyke for civ5\n"
        "--  imported to civ6\n"
        "--  imported to civ7 by Gedemon (2025)\n"
        "\n"
        "*/\n"
        "\n"
        "console.log(\"loading greatest-earth-data.js\");\n\n"
        "export function GetMap() {\n"
        "\n"
        "    let MapToConvert = [];\n"
        "    for (let i = 0; i < %u; i++) {\n"
        "        MapToConvert[i] = [];\n"
        "    }\n"
        "\n"
        "    // Map Data (Civ7)\n"
        "    // MapToConvert = { civ7TerrainType, civ7BiomeType, civ7FeatureType, civ7ResourceType }\n"
        "\n"
        "\n",
        m_width
    );

    for (uint j = 0; j < m_height; ++j)
    {
        for (uint i = 0; i < m_width; ++i)
        {
            const Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            data += fmt::sprintf("    MapToConvert[%u][%u]=[%s, %s, %s, %s];\n", 
                i, j, 
                exportValue(getTerrainTypeAsString(tile.terrain)),
                exportValue(getBiomeTypeAsString(tile.biome)),
                exportValue(getFeatureTypeAsString(tile.feature)),
                exportValue(getResourceTypeAsString(tile.resource)));
        }
    }

    data +=
        "\n"
        "    return MapToConvert;\n"
        "}"
        "\n"
        "console.log(\"loaded greatest-earth-data.js\");\n"
        "//\n";

    FILE * fp = fopen(m_path.c_str(), "wb");
    
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