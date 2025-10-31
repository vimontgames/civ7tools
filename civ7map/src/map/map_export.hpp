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
void Map::exportFiles(const string & _cwd, bool _useModTemplate)
{
    if (_useModTemplate)
    {
        exportModInfo();
        exportSQLTables();
        exportConfig();
        exportMap();
        exportMapData();
        exportMapText();
        exportModuleText();
        exportTSL();
    }
    else
    {
        exportMapData();
        exportTSL();
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
    data += fmt::sprintf("<ActionGroup id=\"base-game-init-%s\" scope=\"game\" criteria=\"always\">\n", getBaseName());
	data += "		<Properties>                                                          \n";
	data += "			<LoadOrder>-999</LoadOrder>                                       \n";
	data += "		</Properties>                                                         \n";
	data += "		<Actions>                                                             \n";
	data += "			<UpdateDatabase>                                                  \n";
	data += "				<Item>data/tables.sql</Item>                                  \n";
	data += "			</UpdateDatabase>                                                 \n";
	data += "		</Actions>                                                            \n";
	data += "	</ActionGroup>                                                            \n";
    data += fmt::sprintf("        <ActionGroup id=\"base-game-main-%s\" scope=\"game\" criteria=\"always\">\n", getBaseName());
    data += "            <Actions>\n";
    data += "                <UpdateDatabase>\n";
    data += fmt::sprintf("                   <Item>maps/%s.xml</Item>\n", getBaseName());
    data += "                </UpdateDatabase>\n";
    data += "                <UpdateText>\n";
    data += "                    <Item>text/en_us/MapText.xml</Item>\n";
    data += "                </UpdateText>\n";
    data += "		         <UpdateIcons>\n";
	data += "		             <Item>icons/icons.xml</Item>\n";
	data += "	             </UpdateIcons>\n";
    data += "            </Actions>\n";
    data += "        </ActionGroup>\n";
    data += fmt::sprintf("        <ActionGroup id=\"shell-%s\" scope=\"shell\" criteria=\"always\">\n", getBaseName());
    data += "            <Actions>\n";
    data += "                <UpdateDatabase>\n";
    data += "                    <Item>config/config.xml</Item>\n";
    data += fmt::sprintf("                    <Item>maps/%s.xml</Item>\n", getBaseName());
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
string Map::getExportMapSize(MapSize _mapSize)
{
    return fmt::sprintf("MAPSIZE_%s", ToUpperLabel(asString(_mapSize)));
}

//--------------------------------------------------------------------------------------
// e.g. LOC_MAPNAME_NAME
//--------------------------------------------------------------------------------------
string Map::getLocMapName() const
{
    string locMapName = fmt::sprintf("LOC_%s_NAME", ToUpperLabel(getBaseName()));
    return locMapName;
}

//--------------------------------------------------------------------------------------
// e.g. LOC_MAPNAME_DESCRIPTION
//--------------------------------------------------------------------------------------
string Map::getLocMapDescription() const
{
    string locMapDescription = fmt::sprintf("LOC_%s_DESCRIPTION", ToUpperLabel(getBaseName()));
    return locMapDescription;
}

//--------------------------------------------------------------------------------------
void Map::exportConfig()
{
    std::string data;

    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += "<Database>\n";
    data += "    <Maps>\n";
    data += fmt::sprintf("        <Row File=\"{%s}maps/%s\" Name=\"%s\" Description=\"%s\" SortIndex=\"100\"/>\n", getModID(), GetFilename(m_mapPath), getLocMapName(), getLocMapDescription());
    data += "    </Maps>\n";
    data += "    <SupportedValuesByMap>\n";
    data += fmt::sprintf("        <Row Map=\"{%s}maps/%s\" Domain=\"StandardMapSizes\" Value=\"%s\"/>\n", getModID(), GetFilename(m_mapPath), getExportMapSize(m_mapSize));
    data += fmt::sprintf("        <Row Map=\"{%s}maps/%s\" Domain=\"DistantLandsMapSizes\" Value=\"%s\"/>\n", getModID(), GetFilename(m_mapPath), getExportMapSize(m_mapSize));
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
        LOG_WARNING("Config file \"%s\" updated", configPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write config file \"%s\"", configPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportMapText()
{
    string data;

    data += "    <?xml version=\"1.0\" encoding=\"utf-8\"?>   \n";
    data += "<Database>                                      \n";
    data += "	<EnglishText>                                \n";
    data += fmt::sprintf("		<Replace Tag=\"%s\">       \n", getLocMapName());
    data += fmt::sprintf("			<Text>%s[n](Civ7Map)</Text>\n", getBaseName());
    data += "		</Replace>                               \n";
    data += fmt::sprintf("		<Replace Tag=\"%s\">\n", getLocMapDescription());
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
        LOG_WARNING("Map text file \"%s\" updated", mapTextPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write map text file \"%s\"", mapTextPath.c_str());
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
        LOG_WARNING("Module text file \"%s\" updated", moduleTextPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write module text file \"%s\"", moduleTextPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportTSL()
{
    string data;
                                                           
    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>     \n";
    data += "<Database>                                     \n";
    data += "                                               \n";
    data += "	<!-- +++++++++++++++++++++++++++++++++++ -->\n";
    data += fmt::sprintf("	<!-- \"%s\" start position -->\n", getBaseName());
    data += "	<!-- +++++++++++++++++++++++++++++++++++ -->\n";
    data += "	                                            \n";
    data += "	<!-- Major Civilizations -->                \n";
    data += "	<StartPosition>                             \n";

    for (uint c = 0; c < m_civilizations.size(); ++c)
    {
        const auto & civ = m_civilizations[c];
        for (uint t = 0; t < civ.tsl.size(); ++t)
        {
            const auto & tsl = civ.tsl[t];
            data += fmt::sprintf("		<Replace MapName=\"%s\" Civilization=\"%s\" X=\"%i\" Y=\"%i\" />\n", getPrettyName(), civ.civilizationName, tsl.pos.x, tsl.pos.y);
        }
    }
  
    data += "	</StartPosition>                            \n";
    data += "</Database>                                    \n";

    string tslPath = fmt::sprintf("%s\\%s.xml", GetFolder(m_mapPath), getBaseName());
    FILE * fp = fopen(tslPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("TSL file \"%s\" updated", tslPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write TSL file \"%s\"", tslPath.c_str());
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
string Map::getPrettyName() const
{
    return m_prettyName;
}

//--------------------------------------------------------------------------------------
string Map::getModID() const
{
    return fmt::sprintf("%s-%s", m_author, getBaseName());
}

//--------------------------------------------------------------------------------------
void Map::exportMap()
{
    createFolders();

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
    data += fmt::sprintf("    initParams.width = %i;\n", m_width);
    data += fmt::sprintf("    initParams.height = %i;\n", m_height);
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
    data += fmt::sprintf("    const mapName = '%s';\n", getPrettyName());
    data += "    let genParameters = {\n";
    data += fmt::sprintf("        westStart: %i,\n", m_westStart);
    data += fmt::sprintf("        westEnd: %i,\n", m_westEnd);
    data += fmt::sprintf("        eastStart: %i,\n", m_eastStart);
    data += fmt::sprintf("        eastEnd: %i\n", m_eastEnd);
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
        LOG_WARNING("Map file \"%s\" updated", m_mapPath.c_str());
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

    data += fmt::sprintf(
        "\n"
        "    return MapToConvert;\n"
        "}"
        "\n"
        "console.log(\"loaded %s\");\n"
        "//\n",
        GetFilename(m_mapDataPath)
    );

    FILE * fp = fopen(m_mapDataPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("Map data file \"%s\" updated", m_mapDataPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write map data file \"%s\"", m_mapDataPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
// This file is necessary for TSL to work (Loading order issue with YnAMP?)
//--------------------------------------------------------------------------------------
void Map::exportSQLTables()
{
    string data;

    data += "/*                                                                                                                                                                                                                      \n";
    data += "YnAMP                                                                                                                                                                                                                   \n";
    data += "by Gedemon (2016-2025)                                                                                                                                                                                                  \n";
    data += "	                                                                                                                                                                                                                     \n";
    data += "*/                                                                                                                                                                                                                      \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-----------------------------------------------                                                                                                                                                                         \n";
    data += "-- Create Tables                                                                                                                                                                                                        \n";
    data += "-----------------------------------------------                                                                                                                                                                         \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- City names by Era                                                                                                                                                                                                    \n";
    data += "CREATE TABLE IF NOT EXISTS CityNameByEra                                                                                                                                                                                \n";
    data += "	(	CityLocaleName TEXT,                                                                                                                                                                                             \n";
    data += "		Era TEXT,                                                                                                                                                                                                        \n";
    data += "		CityEraName TEXT);                                                                                                                                                                                               \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Resources : Exclusion zones for resources                                                                                                                                                                            \n";
    data += "CREATE TABLE IF NOT EXISTS ResourceRegionExclude                                                                                                                                                                        \n";
    data += "	(	Region TEXT,                                                                                                                                                                                                     \n";
    data += "		Resource TEXT);                                                                                                                                                                                                  \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Resources : Exclusive zones for resources                                                                                                                                                                            \n";
    data += "CREATE TABLE IF NOT EXISTS ResourceRegionExclusive                                                                                                                                                                      \n";
    data += "	(	Region TEXT,                                                                                                                                                                                                     \n";
    data += "		Resource TEXT);	                                                                                                                                                                                                 \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Resources : Regions of Major Deposits                                                                                                                                                                                \n";
    data += "CREATE TABLE IF NOT EXISTS ResourceRegionDeposit                                                                                                                                                                        \n";
    data += "	(	Region TEXT,                                                                                                                                                                                                     \n";
    data += "		Resource TEXT,                                                                                                                                                                                                   \n";
    data += "		Deposit TEXT,                                                                                                                                                                                                    \n";
    data += "		MinYield INT default 1,                                                                                                                                                                                          \n";
    data += "		MaxYield INT default 1);                                                                                                                                                                                         \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Resources : Requested for each Civilization                                                                                                                                                                          \n";
    data += "CREATE TABLE IF NOT EXISTS CivilizationRequestedResource                                                                                                                                                                \n";
    data += "	(	Civilization TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		Resource TEXT,                                                                                                                                                                                                   \n";
    data += "		Quantity INT default 1);                                                                                                                                                                                         \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Optional Extra Placement                                                                                                                                                                                             \n";
    data += "CREATE TABLE IF NOT EXISTS ExtraPlacement                                                                                                                                                                               \n";
    data += "	(	MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		MapScript TEXT,                                                                                                                                                                                                  \n";
    data += "		X INT default 0,                                                                                                                                                                                                 \n";
    data += "		Y INT default 0,                                                                                                                                                                                                 \n";
    data += "		ConfigurationId TEXT,                                                                                                                                                                                            \n";
    data += "		ConfigurationValue TEXT,                                                                                                                                                                                         \n";
    data += "		RuleSet TEXT,                                                                                                                                                                                                    \n";
    data += "		Civilization TEXT,                                                                                                                                                                                               \n";
    data += "		TerrainType TEXT,                                                                                                                                                                                                \n";
    data += "		FeatureType TEXT,                                                                                                                                                                                                \n";
    data += "		ResourceType TEXT,                                                                                                                                                                                               \n";
    data += "		DisabledByFeature TEXT,                                                                                                                                                                                          \n";
    data += "		Elevation INT,                                                                                                                                                                                                   \n";
    data += "		Quantity INT default 0);                                                                                                                                                                                         \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Natural Wonder Positions                                                                                                                                                                                             \n";
    data += "CREATE TABLE IF NOT EXISTS NaturalWonderPosition                                                                                                                                                                        \n";
    data += "	(	MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		MapScript TEXT,                                                                                                                                                                                                  \n";
    data += "		FeatureType TEXT NOT NULL,                                                                                                                                                                                       \n";
    data += "		TerrainType TEXT,                                                                                                                                                                                                \n";
    data += "		X INT default 0,                                                                                                                                                                                                 \n";
    data += "		Y INT default 0);                                                                                                                                                                                                \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Start Positions                                                                                                                                                                                                      \n";
    data += "CREATE TABLE IF NOT EXISTS StartPosition                                                                                                                                                                                \n";
    data += "	(	MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		MapScript TEXT, 	-- to override MapName reference for specific entries in relation to MapScript (like in region of the Largest Earth Map that were heavily modified and can't use the GiantEarth reference)   \n";
    data += "		Civilization TEXT,                                                                                                                                                                                               \n";
    data += "		Leader TEXT,                                                                                                                                                                                                     \n";
    data += "		DisabledByCivilization TEXT,                                                                                                                                                                                     \n";
    data += "		DisabledByLeader TEXT,                                                                                                                                                                                           \n";
    data += "		AlternateStart INT default 0,                                                                                                                                                                                    \n";
    data += "		X INT default 0,                                                                                                                                                                                                 \n";
    data += "		Y INT default 0);                                                                                                                                                                                                \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Replaced civilization (Civilization type still exist, but is used to represent another Civilization, for example CIVILIZATION_JAKARTA is now representing Bandar Brunei)                                             \n";
    data += "CREATE TABLE IF NOT EXISTS ReplacedCivilization                                                                                                                                                                         \n";
    data += "	(	Civilization TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		ReplacedBy TEXT NOT NULL,                                                                                                                                                                                        \n";
    data += "		WhenNameTag TEXT NOT NULL);                                                                                                                                                                                      \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Regions positions                                                                                                                                                                                                    \n";
    data += "CREATE TABLE IF NOT EXISTS RegionPosition                                                                                                                                                                               \n";
    data += "	(	MapName TEXT NOT NULL,                                                                                                                                                                                           \n";
    data += "		Region TEXT NOT NULL,                                                                                                                                                                                            \n";
    data += "		X INT default 0,                                                                                                                                                                                                 \n";
    data += "		Y INT default 0,                                                                                                                                                                                                 \n";
    data += "		Width INT default 0,                                                                                                                                                                                             \n";
    data += "		Height INT default 0);                                                                                                                                                                                           \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- City Map		                                                                                                                                                                                                     \n";
    data += "CREATE TABLE IF NOT EXISTS CityMap                                                                                                                                                                                      \n";
    data += "	(	MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		MapScript TEXT, 	-- to override MapName reference for specific entries in relation to MapScript (like in region of the Largest Earth Map that were heavily modified and can't use the GiantEarth reference)   \n";
    data += "		Civilization TEXT,                                                                                                                                                                                               \n";
    data += "		CityLocaleName TEXT NOT NULL,                                                                                                                                                                                    \n";
    data += "		X INT default 0,                                                                                                                                                                                                 \n";
    data += "		Y INT default 0,                                                                                                                                                                                                 \n";
    data += "		Area INT NOT NULL default 1);                                                                                                                                                                                    \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Scenario Civilization Replacements                                                                                                                                                                                   \n";
    data += "-- To code:                                                                                                                                                                                                             \n";
    data += "-- Replace scenario's <CivilizationType> by the (last) <PreferedType> available                                                                                                                                         \n";
    data += "-- Use (last available) <BackupType> when the scenario's <CivilizationType> is not available                                                                                                                            \n";
    data += "-- If the scenario use a <PreferedType> and it's not available, try to use the first available <CivilizationType> referencing it.                                                                                       \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioCivilizationsReplacement                                                                                                                                                             \n";
    data += "	(	ScenarioName TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		CivilizationType TEXT NOT NULL,                                                                                                                                                                                  \n";
    data += "		BackupType TEXT,                                                                                                                                                                                                 \n";
    data += "		PreferedType TEXT);                                                                                                                                                                                              \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Civilizations                                                                                                                                                                                               \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioCivilizations                                                                                                                                                                        \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		MapScript TEXT,                                                                                                                                                                                                  \n";
    data += "		SpecificEra TEXT,                                                                                                                                                                                                \n";
    data += "		CivilizationType TEXT, 		-- can be NULL to set default values for all Civilization, in that case ScenarioName must not be NULL                                                                                \n";
    data += "		ExploreAll BOOLEAN,                                                                                                                                                                                              \n";
    data += "		MeetAll BOOLEAN,                                                                                                                                                                                                 \n";
    data += "		Priority INT default 0, 	-- higher means first selected for placement in loops                                                                                                                                \n";
    data += "		CityPlacement TEXT,                                                                                                                                                                                              \n";
    data += "		MaxDistanceFromCapital INT, -- if OnlySameLandMass is true, then this is the land path distance, else it's the air distance                                                                                      \n";
    data += "		MinCitySeparation INT,                                                                                                                                                                                           \n";
    data += "		SouthernLatitude INT,		-- from -90 to 90, 0 being equator                                                                                                                                                   \n";
    data += "		NorthernLatitude INT,                                                                                                                                                                                            \n";
    data += "		BorderPlacement TEXT,                                                                                                                                                                                            \n";
    data += "		BorderMaxDistance INT,                                                                                                                                                                                           \n";
    data += "		OnlySameLandMass BOOLEAN,                                                                                                                                                                                        \n";
    data += "		NumberOfCity INT,                                                                                                                                                                                                \n";
    data += "		NumberOfMinorCity INT, 		-- Default scenario setting only, it has no effect when CivilizationType exists                                                                                                      \n";
    data += "		CapitalSize INT,                                                                                                                                                                                                 \n";
    data += "		OtherCitySize INT,                                                                                                                                                                                               \n";
    data += "		DecreaseOtherCitySize BOOLEAN,	-- Default scenario setting only, it has no effect when CivilizationType exists                                                                                                  \n";
    data += "		CitySizeDecrement INT,                                                                                                                                                                                           \n";
    data += "		NumCityPerSizeDecrement INT,                                                                                                                                                                                     \n";
    data += "		Infrastructure TEXT,                                                                                                                                                                                             \n";
    data += "		RoadPlacement TEXT,                                                                                                                                                                                              \n";
    data += "		RoadMaxDistance INT,                                                                                                                                                                                             \n";
    data += "		MaxRoadPerCity INT,                                                                                                                                                                                              \n";
    data += "		InternationalRoads BOOLEAN,                                                                                                                                                                                      \n";
    data += "		InternationalRoadMaxDistance INT,                                                                                                                                                                                \n";
    data += "		NationalRailPlacement TEXT,                                                                                                                                                                                      \n";
    data += "		InternationalRails TEXT,                                                                                                                                                                                         \n";
    data += "		RailsMaxDistance INT,                                                                                                                                                                                            \n";
    data += "		Improvements TEXT,                                                                                                                                                                                               \n";
    data += "		MaxNumImprovements INT,                                                                                                                                                                                          \n";
    data += "		ImprovementsPerSizeRatio INT,                                                                                                                                                                                    \n";
    data += "		MaxImprovementsDistance INT,                                                                                                                                                                                     \n";
    data += "		Districts TEXT,                                                                                                                                                                                                  \n";
    data += "		MaxNumDistricts INT,                                                                                                                                                                                             \n";
    data += "		DistrictsPerSize TEXT,                                                                                                                                                                                           \n";
    data += "		MaxDistrictsDistance INT,                                                                                                                                                                                        \n";
    data += "		Buildings TEXT,                                                                                                                                                                                                  \n";
    data += "		BuildingsPerSize TEXT,                                                                                                                                                                                           \n";
    data += "		MaxNumBuildings INT);                                                                                                                                                                                            \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Cities                                                                                                                                                                                                      \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioCities                                                                                                                                                                               \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		CivilizationType TEXT,			-- to code : if NULL it will search a possible CivilizationType using the GameInfo.CityNames table (CityName must be set in that case)                                           \n";
    data += "		CityName TEXT,					-- if not NULL it will override the civilization city list name                                                                                                                  \n";
    data += "		CitySize INT default 1,                                                                                                                                                                                          \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT,                                                                                                                                                                                                           \n";
    data += "		Y INT);                                                                                                                                                                                                          \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Territory                                                                                                                                                                                                   \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioTerritory                                                                                                                                                                            \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		CivilizationType TEXT NOT NULL,                                                                                                                                                                                  \n";
    data += "		CityName TEXT,					-- if NULL the plot will be owned by the nearest city                                                                                                                            \n";
    data += "		CityX INT,                                                                                                                                                                                                       \n";
    data += "		CityY INT,                                                                                                                                                                                                       \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Districts (placed after Territory)                                                                                                                                                                          \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioDistricts                                                                                                                                                                            \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		DistrictType TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		CityName TEXT,					-- if NULL the district will be owned by the nearest city in that case                                                                                                           \n";
    data += "		CityX INT,                                                                                                                                                                                                       \n";
    data += "		CityY INT,                                                                                                                                                                                                       \n";
    data += "		InnerHealth INT,                                                                                                                                                                                                 \n";
    data += "		OutterHealth INT,                                                                                                                                                                                                \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Buildings (placed after Districts)                                                                                                                                                                          \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioBuildings                                                                                                                                                                            \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		BuildingType TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Occupied Territory (placed last - can be used only if there is a Get/SetOriginalOwner method for plots)                                                                                                     \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioOccupiedTerritory                                                                                                                                                                    \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		CivilizationType TEXT NOT NULL,                                                                                                                                                                                  \n";
    data += "		CityName TEXT,					-- if NULL the plot will be owned by the nearest city in that case                                                                                                               \n";
    data += "		CityX INT,                                                                                                                                                                                                       \n";
    data += "		CityY INT,                                                                                                                                                                                                       \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-- Scenario Improvements                                                                                                                                                                                                \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioInfrastructure                                                                                                                                                                       \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		ImprovementType TEXT,                                                                                                                                                                                            \n";
    data += "		RouteType TEXT,                                                                                                                                                                                                  \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Scenario Units Replacements                                                                                                                                                                                          \n";
    data += "-- Use <BackupType> if the scenario's <UnitType> is not available                                                                                                                                                       \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioUnitsReplacement                                                                                                                                                                     \n";
    data += "	(	ScenarioName TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		UnitType TEXT NOT NULL,                                                                                                                                                                                          \n";
    data += "		BackupType TEXT NOT NULL);                                                                                                                                                                                       \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Scenario Units                                                                                                                                                                                                       \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioUnits                                                                                                                                                                                \n";
    data += "	(	ScenarioName TEXT,                                                                                                                                                                                               \n";
    data += "		MapName TEXT,                                                                                                                                                                                                    \n";
    data += "		CivilizationType TEXT NOT NULL,                                                                                                                                                                                  \n";
    data += "		UnitType TEXT NOT NULL,                                                                                                                                                                                          \n";
    data += "		Name TEXT,                                                                                                                                                                                                       \n";
    data += "		Damage INT default 0,                                                                                                                                                                                            \n";
    data += "		PromotionList TEXT,                                                                                                                                                                                              \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		X INT NOT NULL,                                                                                                                                                                                                  \n";
    data += "		Y INT NOT NULL);                                                                                                                                                                                                 \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Scenario Technologies                                                                                                                                                                                                \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioTechs                                                                                                                                                                                \n";
    data += "	(	ScenarioName TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		CivilizationType TEXT,			-- If NULL give the tech (or the era's techs depending on which is set) to all civilizations                                                                                     \n";
    data += "		EraType TEXT,                                                                                                                                                                                                    \n";
    data += "		OnlyAI BOOLEAN NOT NULL CHECK (OnlyAI IN (0,1)) DEFAULT 0,                                                                                                                                                       \n";
    data += "		OnlyHuman BOOLEAN NOT NULL CHECK (OnlyHuman IN (0,1)) DEFAULT 0,                                                                                                                                                 \n";
    data += "		TechnologyType TEXT);                                                                                                                                                                                            \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "-- Scenario Diplomacy                                                                                                                                                                                                   \n";
    data += "CREATE TABLE IF NOT EXISTS ScenarioDiplomacy                                                                                                                                                                            \n";
    data += "	(	ScenarioName TEXT NOT NULL,                                                                                                                                                                                      \n";
    data += "		CivilizationType TEXT NOT NULL,                                                                                                                                                                                  \n";
    data += "		DiplomaticAction TEXT,                                                                                                                                                                                           \n";
    data += "		OtherCivilization TEXT,                                                                                                                                                                                          \n";
    data += "		ActionValue INT);                                                                                                                                                                                                \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "-----------------------------------------------                                                                                                                                                                         \n";
    data += "-- Temporary Tables for initialization                                                                                                                                                                                  \n";
    data += "-----------------------------------------------                                                                                                                                                                         \n";
    data += "                                                                                                                                                                                                                        \n";
    data += "DROP TABLE IF EXISTS CityStatesConfiguration;                                                                                                                                                                           \n";
    data += "		                                                                                                                                                                                                                 \n";
    data += "CREATE TABLE CityStatesConfiguration                                                                                                                                                                                    \n";
    data += "	(	Name TEXT,                                                                                                                                                                                                       \n";
    data += "		Category TEXT,                                                                                                                                                                                                   \n";
    data += "		Ethnicity TEXT		                                                                                                                                                                                             \n";
    data += "	);                                                                                                                                                                                                                   \n";

    string tablePath = fmt::sprintf("%s\\data\\tables.sql", m_modFolder);
    FILE * fp = fopen(tablePath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("SQL tables file \"%s\" updated", tablePath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write SQL tables file \"%s\"", tablePath.c_str());
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