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
void Map::exportFiles(const string & _cwd, bool _mapDataOnly)
{
    if (_mapDataOnly)
    {
        exportMapData();
    }
    else
    {
        exportModInfo();
        exportConfig();
        exportMap();
        exportMapData();
        exportMapText();
        exportModuleText();
    }
}

//--------------------------------------------------------------------------------------
void Map::exportModInfo()
{
    std::string data;

    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += fmt::sprintf( "<Mod id=\"%s\" version=\"1\"\n", getModID());
    data += "    xmlns=\"ModInfo\">\n";
    data += "    <Properties>\n";
    data += "        <Name>LOC_MODULE_MAPNAME_NAME</Name>\n";
    data += "        <Description>LOC_MODULE_MAPNAME_DESCRIPTION</Description>\n";
    data += fmt::sprintf("        <Authors>%s</Authors>\n", m_author);
    data += "        <Package>Mod</Package>\n";
    data += "    </Properties>\n";
    data += "    <Dependencies>\n";
    data += "        <Mod id=\"base-standard\" title=\"LOC_MODULE_BASE_STANDARD_NAME\"/>\n";
    data += "    </Dependencies>\n";
    data += "    <ActionCriteria>\n";
    data += "        <Criteria id=\"always\">\n";
    data += "            <AlwaysMet></AlwaysMet>\n";
    data += "        </Criteria>\n";
    data += "    </ActionCriteria>\n";
    data += "    <ActionGroups>\n";
    data += "        <ActionGroup id=\"base-game-main-mapname\" scope=\"game\" criteria=\"always\">\n";
    data += "            <Actions>\n";
    data += "                <UpdateText>\n";
    data += "                    <Item>text/en_us/MapText.xml</Item>\n";
    data += "                </UpdateText>\n";
    data += "            </Actions>\n";
    data += "        </ActionGroup>\n";
    data += "        <ActionGroup id=\"shell-mapname\" scope=\"shell\" criteria=\"always\">\n";
    data += "            <Actions>\n";
    data += "                <UpdateDatabase>\n";
    data += "                    <Item>config/config.xml</Item>\n";
    data += "                </UpdateDatabase>\n";
    data += "                <UpdateText>\n";
    data += "                    <Item>text/en_us/MapText.xml</Item>\n";
    data += "                </UpdateText>\n";
    data += "            </Actions>\n";
    data += "        </ActionGroup>\n";
    data += "    </ActionGroups>\n";
    data += "    <LocalizedText>\n";
    data += "        <File>text/en_us/ModuleText.xml</File>\n";
    data += "    </LocalizedText>\n";
    data += "</Mod>\n";

    string modInfoPath = fmt::sprintf("%s\\%s.modinfo", m_modFolder, GetFilenameWithoutExtension(m_mapPath));
    FILE * fp = fopen(modInfoPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
}

//--------------------------------------------------------------------------------------
void Map::exportConfig()
{
    std::string data;

    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += "<Database>\n";
    data += "    <Maps>\n";
    data += fmt::sprintf("        <Row File=\"{%s}maps/%s\" Name=\"LOC_MAPNAME_NAME\" Description=\"LOC_MAPNAME_DESCRIPTION\" SortIndex=\"100\"/>\n", getModID(), GetFilename(m_mapPath));
    data += "    </Maps>\n";
    data += "    <SupportedValuesByMap>\n";
    data += fmt::sprintf("        <Row Map=\"{%s}maps/%s\" Domain=\"StandardMapSizes\" Value=\"MAPSIZE_GREATEST_EARTH\"/>\n", getModID(), GetFilename(m_mapPath));
    data += fmt::sprintf("        <Row Map=\"{%s}maps/%s\" Domain=\"DistantLandsMapSizes\" Value=\"MAPSIZE_GREATEST_EARTH\"/>\n", getModID(), GetFilename(m_mapPath));
    data += "    </SupportedValuesByMap>\n";
    data += "</Database>\n";

    string configFolder = fmt::sprintf("%s\\config", m_modFolder);
    bool created = CreateFolder(configFolder);
    if (created)
        LOG_WARNING("Folder \"%s\" created", configFolder.c_str());

    string configPath = fmt::sprintf("%s\\config.xml", configFolder);
    FILE * fp = fopen(configPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
}

//--------------------------------------------------------------------------------------
void Map::exportMapText()
{
    string data;

    data += "    <?xml version=\"1.0\" encoding=\"utf-8\"?>   \n";
    data += "<Database>                                      \n";
    data += "	<EnglishText>                                \n";
    data += "		<Replace Tag=\"LOC_MAPNAME_NAME\">       \n";
    data += fmt::sprintf("			<Text>%s[n](Civ7Map)</Text>\n", getBaseName());
    data += "		</Replace>                               \n";
    data += "		<Replace Tag=\"LOC_MAPNAME_DESCRIPTION\">\n";
    data += "			<Text>TODO: Map description</Text>         \n";
    data += "		</Replace>                               \n";
    data += "	</EnglishText>                               \n";
    data += "</Database>                                     \n";

    const string mapTextPath = fmt::sprintf("%s\\text\\en_us\\MapText.xml", m_modFolder);
    FILE * fp = fopen(mapTextPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
    else
    {
        LOG_ERROR("Could not write MapText file \"%s\"", mapTextPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportModuleText()
{
    string data;

    data += "    <?xml version=\"1.0\" encoding=\"utf-8\"?>           \n";
    data += "<Database>                                              \n";
    data += "	<EnglishText>                                        \n";
    data += "		<Replace Tag=\"LOC_MODULE_MAPNAME_NAME\">        \n";
    data += fmt::sprintf("			<Text>%s</Text>               \n", getBaseName());
    data += "		</Replace>                                       \n";
    data += "		<Replace Tag=\"LOC_MODULE_MAPNAME_DESCRIPTION\"> \n";
    data += "			<Text>TODO: Map module description</Text> \n";
    data += "		</Replace>                                       \n";
    data += "	</EnglishText>                                       \n";
    data += "</Database>                                             \n";

    const string moduleTextPath = fmt::sprintf("%s\\text\\en_us\\ModuleText.xml", m_modFolder);
    FILE * fp = fopen(moduleTextPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
    else
    {
        LOG_ERROR("Could not write MapText file \"%s\"", moduleTextPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
string Map::getBaseName() const
{
    string name = GetFilename(m_mapPath);
    name = name.substr(0, name.length() - 7); // remove "-map.js"
    return name;
}

//--------------------------------------------------------------------------------------
string Map::getModID() const
{
    return fmt::sprintf("%s-%s", m_author, getBaseName());
}

//--------------------------------------------------------------------------------------
void Map::exportMap()
{
    string baseName = getBaseName();

    std::string data;

    data += "// " + GetFilename(m_mapPath) + "\n";
    data += "/**\n";
    data += "* " + baseName + "\n";
    data += "* \n";
    data += "*/\n";
    data += fmt::sprintf("console.log(\"loading script %s\");\n", GetFilename(m_mapPath));
    data += "import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';\n";
    data += fmt::sprintf("import { GetMap } from '/%s/maps/%s';\n", getModID(), GetFilename(m_mapDataPath));
    data += "\n";
    data += "function requestMapData(initParams) {\n";
    data += fmt::sprintf("    initParams.width = 104;\n", m_width);
    data += fmt::sprintf("    initParams.height = 64;\n", m_height);
    data += "    console.log(initParams.width);\n";
    data += "    console.log(initParams.height);\n";
    data += "    console.log(initParams.topLatitude);\n";
    data += "    console.log(initParams.bottomLatitude);\n";
    data += "    console.log(initParams.wrapX);\n";
    data += "    console.log(initParams.wrapY);\n";
    data += "    console.log(initParams.mapSize);\n";
    data += "    engine.call(\"SetMapInitData\", initParams);\n";
    data += "}\n";
    data += "function generateMap() {\n";
    data += fmt::sprintf("    const mapName = '%s';\n", getBaseName());
    data += "    let genParameters = {\n";
    data += "        westStart: 1,\n";
    data += "        westEnd: 31,\n";
    data += "        eastStart: 32,\n";
    data += "        eastEnd: 102\n";
    data += "    };\n";
    data += "    let importedMap = GetMap();\n";
    data += "    generateYnAMP(mapName, importedMap, genParameters);\n";
    data += "}\n";
    data += "\n";
    data += "// Register listeners.\n";
    data += "engine.on('RequestMapInitData', requestMapData);\n";
    data += "engine.on('GenerateMap', generateMap);\n";
    data += "\n";
    data += fmt::sprintf("console.log(\"Loaded %s\");\n", GetFilename(m_mapPath));

    FILE * fp = fopen(m_mapPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
    else
    {
        LOG_ERROR("Could not write map file \"%s\"", m_mapPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportMapData()
{
    string data;

    data += "/*\n";
    data += "\n";
    data += fmt::sprintf("--	FILE:	 %s\n", getBaseName());
    data += fmt::sprintf("--  made by %s\n", m_author);
    data += "\n";
    data += "*/\n";
    data += "\n";
    data += fmt::sprintf("console.log(\"loading %s\");\n\n", GetFilename(m_mapPath));
    data += "export function GetMap() {\n";
    data += "\n";
    data += "    let MapToConvert = [];\n";
    data += fmt::sprintf("    for (let i = 0; i < %u; i++) {\n", m_width);
    data += "        MapToConvert[i] = [];\n";
    data += "    }\n";
    data += "\n";
    data += "    // Map Data (Civ7)\n";
    data += "    // MapToConvert = { civ7TerrainType, civ7BiomeType, civ7FeatureType, civ7ResourceType }\n";
    data += "\n\n";

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

    FILE * fp = fopen(m_mapDataPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
    else
    {
        LOG_ERROR("Could not write map data file \"%s\"", m_mapPath.c_str());
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