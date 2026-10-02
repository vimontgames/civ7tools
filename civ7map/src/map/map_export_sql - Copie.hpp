
//--------------------------------------------------------------------------------------
void Map::exportFilesCiv7Map(const string & _cwd, bool _useModTemplate)
{
    exportModInfoCiv7Map();
    exportTextCiv7Map();
    exportConfigCiv7Map();
    exportMapsCiv7Map();
    exportSQLiteMap();
    exportMapDataCiv7Map();

    //exportModInfo();
    //exportSQLTables();
    //exportConfig();
    //exportMap();
    //exportMapData();
    //exportMapText();
    //exportModuleText();
    //exportTSL();
}

//--------------------------------------------------------------------------------------
void Map::exportModInfoCiv7Map()
{
    std::string data;

    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += fmt::sprintf("<Mod id=\"%s\" version=\"1\" xmlns=\"ModInfo\">\n", getModID());
    data += "  <Properties>\n";
    data += fmt::sprintf("      <Name>%s</Name>\n", getPrettyName());
    data += fmt::sprintf("      <Description>%s</Description>\n", getDescription());
    data += fmt::sprintf("      <Authors>%s</Authors>\n", getAuthor());
    data += "      <Package>Mod</Package>\n";
    data += "      <AffectsSavedGames>1</AffectsSavedGames>\n";
    data += "  </Properties>\n";
    data += "  <Dependencies>\n";
    data += "      <Mod id=\"base-standard\" title=\"LOC_MODULE_BASE_STANDARD_NAME\"/>\n";
    data += "  </Dependencies>\n";
    data += "  <ActionCriteria>\n";
    data += "      <Criteria id=\"always\">\n";
    data += "          <AlwaysMet></AlwaysMet>\n";
    data += "      </Criteria>\n";
    data += "  </ActionCriteria>\n";
    data += "  <ActionGroups>\n";
    data += fmt::sprintf("      <ActionGroup id=\"%s-shell\" scope=\"shell\" criteria=\"always\">\n", getModID());
    data += "          <Actions>\n";
    data += "              <UpdateDatabase>\n";
    data += "                  <Item>config/config.xml</Item>\n";
    data += "              </UpdateDatabase>\n";
    data += "              <UpdateText>\n";
    data += fmt::sprintf("                  <Item>text/en_us/%s_Text.xml</Item>\n", getBaseName());
    data += "              </UpdateText>\n";
    data += "          </Actions>\n";
    data += "      </ActionGroup>\n";
    data += fmt::sprintf("      <ActionGroup id=\"%s-game\" scope=\"game\" criteria=\"always\">\n", getModID());
    data += "          <Actions>\n";
    data += "              <UpdateDatabase>\n";
    data += "                  <Item>data/maps.xml</Item>\n";
    data += "              </UpdateDatabase>\n";
    data += "              <UpdateText>\n";
    data += fmt::sprintf("                  <Item>text/en_us/%s_Text.xml</Item>\n", getBaseName());
    data += "              </UpdateText>\n";
    data += "          </Actions>\n";
    data += "      </ActionGroup>\n";
    data += "  </ActionGroups>\n";
    data += "  <LocalizedText>\n";
    data += fmt::sprintf("      <File>text/en_us/%s_Text.xml</File>\n", getBaseName());
    data += "  </LocalizedText>\n";
    data += "</Mod>\n";

    string modInfoPath = fmt::sprintf("%s\\%s.modinfo", m_modFolder, GetFilenameWithoutExtension(m_mapPath));
    FILE * fp = fopen(modInfoPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("Maps file \"%s\" updated (Civ7Map)", modInfoPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write maps file \"%s\" (Civ7Map)", modInfoPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportTextCiv7Map()
{
    std::string data;

    data += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += "<Database>\n";
    data += "    <EnglishText>\n";
    data += "    </EnglishText>\n";
    data += "    <LocalizedText>\n";
    data += "    </LocalizedText>\n";
    data += "</Database>\n";

    string textPath = fmt::sprintf("%s\\text\\en_us\\%s_Text.xml", m_modFolder, getBaseName());
    FILE * fp = fopen(textPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("Text file \"%s\" updated (Civ7Map)", textPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write text file \"%s\" (Civ7Map)", textPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportConfigCiv7Map()
{
    std::string data;

    // TODO: custom map size
    string mapSize = getExportMapSize(m_mapSize);

    data += fmt::sprintf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    data += fmt::sprintf("<Database>\n");
    data += fmt::sprintf("	<Maps>\n");
    data += fmt::sprintf("		<Row File=\"{%s}maps/%s.Civ7Map\" Name=\"%s\" Description=\"%s\" SortIndex=\"67\"/>\n", getModID(), getBaseName(), getPrettyName(), getDescription());
    data += fmt::sprintf("	</Maps>\n");
    data += fmt::sprintf("	<UnsupportedValuesByAge>\n");
    data += fmt::sprintf("		<Row AgeType=\"AGE_EXPLORATION\" Domain=\"StandardMaps\" Value=\"{%s}maps/%s.Civ7Map\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("		<Row AgeType=\"AGE_MODERN\" Domain=\"StandardMaps\" Value=\"{%s}maps/%s.Civ7Map\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("	</UnsupportedValuesByAge>\n");
    //data += fmt::sprintf("	<MapSizes>\n");
    //data += fmt::sprintf("		<Row Domain=\"StandardMapSizes\" MapSizeType=\"MAPSIZE_HUGE_24\" Name=\"LOC_MAPSIZE_HUGE24_NAME\" Description=\"LOC_MAPSIZE_HUGE24_DESCRIPTION\" MinPlayers=\"2\" MaxPlayers=\"24\" MaxHumans=\"24\" DefaultPlayers=\"24\" SortIndex=\"61\"/>\n");
    //data += fmt::sprintf("		<Row Domain=\"DistantLandsMapSizes\" MapSizeType=\"MAPSIZE_HUGE_24\" Name=\"LOC_MAPSIZE_HUGE24_NAME\" Description=\"LOC_MAPSIZE_HUGE24_DESCRIPTION\" MinPlayers=\"2\" MaxPlayers=\"24\" MaxHumans=\"24\" DefaultPlayers=\"24\" SortIndex=\"61\"/>\n");
    //data += fmt::sprintf("	</MapSizes>\n");
    data += fmt::sprintf("	<SupportedValuesByMap>\n");
    //data += fmt::sprintf("		<Row Map=\"{%s}maps/%s.Civ7Map\" Domain=\"StandardMapStartPositions\" Value=\"START_POSITION_TSL_HUGE\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("		<Row Map=\"{%s}maps/%s.Civ7Map\" Domain=\"StandardMapSizes\" Value=\"%s\"/>\n", getModID(), getBaseName(), mapSize);
    data += fmt::sprintf("		<Row Map=\"{%s}maps/%s.Civ7Map\" Domain=\"DistantLandsMapSizes\" Value=\"%s\"/>\n", getModID(), getBaseName(), mapSize);
    data += fmt::sprintf("	</SupportedValuesByMap>\n");
    data += fmt::sprintf("	<ParameterDependencies>\n");
    data += fmt::sprintf("		<Row ParameterID=\"MapSeaLevel\" ConfigurationGroup=\"Map\" ConfigurationKey=\"MapScript\" Operator=\"NotEquals\" ConfigurationValue=\"{%s}maps/%s.Civ7Map\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("	</ParameterDependencies>\n");
    data += fmt::sprintf("</Database>\n");

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
        LOG_WARNING("Config file \"%s\" updated (Civ7Map)", configPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write config file \"%s\" (Civ7Map)", configPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportMapsCiv7Map()
{
    std::string data;

    data += fmt::sprintf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    data += fmt::sprintf("<Database>\n");
    //data += fmt::sprintf("	<Types>\n");
    //data += fmt::sprintf("		<Replace Type=\"MAPSIZE_HUGE_24\" Kind=\"KIND_MAPSIZE\"/>\n");
    //data += fmt::sprintf("	</Types>\n");
    //data += fmt::sprintf("	<Maps>\n");
    //data += fmt::sprintf("		<Row MapSizeType=\"MAPSIZE_HUGE_24\" Name=\"LOC_MAPSIZE_HUGE24_NAME\" Description=\"LOC_MAPSIZE_HUGE24_DESCRIPTION\" DefaultPlayers=\"24\" PlayersLandmass1=\"6\" PlayersLandmass2=\"6\" GridWidth=\"106\" GridHeight=\"66\" NumNaturalWonders=\"7\" OceanWidth=\"8\" LakeSizeCutoff=\"10\" LakeGenerationFrequency=\"25\" Continents=\"6\" StartSectorRows=\"4\" StartSectorCols=\"3\"/>\n");
    //data += fmt::sprintf("	</Maps>\n");
    data += fmt::sprintf("</Database>\n");

    string dataFolder = fmt::sprintf("%s\\data", m_modFolder);
    bool created = CreateFolder(dataFolder);
    if (created)
        LOG_WARNING("Folder \"%s\" created", dataFolder.c_str());

    string mapsPath = fmt::sprintf("%s\\maps.xml", dataFolder);
    FILE * fp = fopen(mapsPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("Maps file \"%s\" updated (Civ7Map)", mapsPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write maps file \"%s\" (Civ7Map)", mapsPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportMapDataCiv7Map()
{
    std::string data;

    data += fmt::sprintf("import { generateDiscoveries } from '/base-standard/maps/discovery-generator.js';\n");
    data += fmt::sprintf("import { g_PolarWaterRows } from '/base-standard/maps/map-globals.js';\n");
    data += fmt::sprintf("import { shuffle } from '/base-standard/maps/map-utilities.js';\n");
    data += fmt::sprintf("import { GenerationContext, GenerationPhases, generateMapFeatures } from '/base-standard/scripts/common-generation.js';\n");
    data += fmt::sprintf("import { HexMap } from '/base-standard/scripts/hex-map.js';\n");
    data += fmt::sprintf("import { profileScope } from '/base-standard/scripts/profiling.js';\n");
    data += fmt::sprintf("\n");
    data += fmt::sprintf("console.log(\"Generating Earth_Huge map from Civ7Map and script helper.\");\n");
    data += fmt::sprintf("function requestMapData(initParams) {\n");
    data += fmt::sprintf("  console.log(initParams.width);\n");
    data += fmt::sprintf("  console.log(initParams.height);\n");
    data += fmt::sprintf("  console.log(initParams.topLatitude);\n");
    data += fmt::sprintf("  console.log(initParams.bottomLatitude);\n");
    data += fmt::sprintf("  console.log(initParams.wrapX);\n");
    data += fmt::sprintf("  console.log(initParams.wrapY);\n");
    data += fmt::sprintf("  console.log(initParams.mapSize);\n");
    data += fmt::sprintf("  engine.call(\"SetMapInitData\", initParams);\n");
    data += fmt::sprintf("}\n");
    data += fmt::sprintf("async function generateMap() {\n");
    data += fmt::sprintf("  console.log(\"Generating a map!\");\n");
    data += fmt::sprintf("  console.log(`Age - ${GameInfo.Ages.lookup(Game.age).AgeType}`);\n");
    data += fmt::sprintf("  const earthHugeScope = new profileScope(\"Earth_Huge Generation\");\n");
    data += fmt::sprintf("  const iWidth = GameplayMap.getGridWidth();\n");
    data += fmt::sprintf("  const iHeight = GameplayMap.getGridHeight();\n");
    data += fmt::sprintf("  const uiMapSize = GameplayMap.getMapSize();\n");
    data += fmt::sprintf("  const mapInfo = GameInfo.Maps.lookup(uiMapSize);\n");
    data += fmt::sprintf("  if (mapInfo == null) return;\n");
    data += fmt::sprintf("  const hexMap = new HexMap();\n");
    data += fmt::sprintf("  hexMap.initFromTerrainBuilder();\n");
    data += fmt::sprintf("  //paintEarthHugeElevation();\n");
    data += fmt::sprintf("  //paintEarthHugeRivers();\n");
    data += fmt::sprintf("  //paintEarthHugeNaturalWonders();\n");
    data += fmt::sprintf("  //paintEarthHugeResourcesAntiquity();\n");
    data += fmt::sprintf("  //const topSnowRows = 11;\n");
    data += fmt::sprintf("  //const bottomSnowRows = 0;\n");
    data += fmt::sprintf("  //const maxSnowWeight = 60;\n");
    data += fmt::sprintf("  //const snowRandomization = 20;\n");
    data += fmt::sprintf("  //paintEarthHugeSnow(iWidth, iHeight, topSnowRows, bottomSnowRows, maxSnowWeight, snowRandomization);\n");
    data += fmt::sprintf("  const genCtx = new GenerationContext();\n");
    data += fmt::sprintf("  genCtx.phases = GenerationPhases.Rainfall | GenerationPhases.FloodPlains;\n");
    data += fmt::sprintf("  genCtx.bRunAestheticRiverValidation = false;\n");
    data += fmt::sprintf("  //generateMapFeatures(hexMap, genCtx);\n");
    data += fmt::sprintf("  //nameRivers();\n");
    data += fmt::sprintf("  //nameVolcanoes();\n");
    data += fmt::sprintf("  //const startPositions = assignStartPositionsEarthHuge();\n");
    data += fmt::sprintf("  //generateDiscoveries(iWidth, iHeight, startPositions, g_PolarWaterRows);\n");
    data += fmt::sprintf("  earthHugeScope.end();\n");
    data += fmt::sprintf("}\n");
    data += fmt::sprintf("engine.on(\"RequestMapInitData\", requestMapData);\n");
    data += fmt::sprintf("engine.on(\"GenerateMap\", generateMap);\n");
    data += fmt::sprintf("//# sourceMappingURL=Earth_Huge.js.map\n");

    FILE * fp = fopen(m_mapDataPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
        LOG_WARNING("mapData file \"%s\" updated (Civ7Map)", m_mapDataPath.c_str());
    }
    else
    {
        LOG_ERROR("Could not write mapData file \"%s\" ((Civ7Map))", m_mapDataPath.c_str());
    }
}

//--------------------------------------------------------------------------------------
void Map::exportSQLiteMap()
{
    static bool clear = true;
    if (clear)
    {
        if (FileExists(m_mapPath))
        {
            DeleteFile(m_mapPath);
        }
    }

    // Try to open the SQLite database for writing (create or overwrite)
    sqlite3 * db;
    int rc = sqlite3_open(m_mapPath.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Cannot open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Create tables if they don't exist
    
    // Create Map table

    // Map table
    const char * sql =
        "CREATE TABLE IF NOT EXISTS Map ("
        "ID TEXT PRIMARY KEY, "
        "Width INTEGER, "
        "Height INTEGER, "
        "TopLatitude INTEGER, "
        "BottomLatitude INTEGER, "
        "WrapX BOOLEAN, "
        "WrapY BOOLEAN, "
        "MapSizeType TEXT);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create Map table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Insert map size
    sql = "INSERT OR REPLACE INTO Map (ID, Width, Height, TopLatitude, BottomLatitude, WrapX, WrapY, MapSizeType) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt * stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        string mapSize = getExportMapSize(m_mapSize).c_str();

        sqlite3_bind_text(stmt, 1, "Default", -1, SQLITE_STATIC);     // ID
        sqlite3_bind_int(stmt, 2, m_width);
        sqlite3_bind_int(stmt, 3, m_height);
        sqlite3_bind_int(stmt, 4, 90);                          // TopLatitude
        sqlite3_bind_int(stmt, 5, -90);                         // BottomLatitude
        sqlite3_bind_int(stmt, 6, 0);                           // WrapX
        sqlite3_bind_int(stmt, 7, 0);                           // WrapY
        sqlite3_bind_text(stmt, 8, mapSize.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create MetaData table
    sql = 
        "CREATE TABLE IF NOT EXISTS MetaData ("
        "Name TEXT PRIMARY KEY, "
        "Value TEXT);";
    
    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create MetaData table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Insert map name
    sql = "INSERT OR REPLACE INTO MetaData (Name, Value) VALUES ('DisplayName', ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, m_prettyName.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Players table
    sql =
        "CREATE TABLE IF NOT EXISTS Players ("
        "ID INTEGER NOT NULL, "
        "TeamID INTEGER NOT NULL, "
        "CivilizationType TEXT, "
        "LeaderType TEXT, "
        "CivilizationLevelType TEXT, "
        "AgendaType TEXT, "
        "Status TEXT, "
        "Handicap TEXT, "
        "StartingPosition TEXT, "
        "Color TEXT, "
        "Initialized BOOLEAN, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create Players table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // StartPositions table
    sql =
        "CREATE TABLE IF NOT EXISTS StartPositions ("
        "Plot INTEGER NOT NULL, "
        "Type STRING NOT NULL, "
        "Value STRING NOT NULL);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create StartPositions table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Cities table
    sql =
        "CREATE TABLE IF NOT EXISTS Cities ("
        "Owner INTEGER NOT NULL, "
        "Plot INTEGER NOT NULL, "
        "OriginalName TEXT NOT NULL, "
        "Name TEXT NOT NULL, "
        "Civilization INTEGER NOT NULL, "
        "OriginalOwner INTEGER NOT NULL, "
        "IsCapital BOOLEAN NOT NULL, "
        "IsOriginalCapital BOOLEAN NOT NULL, "
        "PRIMARY KEY(Plot));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create Cities table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // MapAttributes table
    sql =
        "CREATE TABLE IF NOT EXISTS MapAttributes ("
        "Name TEXT PRIMARY KEY, "
        "Value TEXT);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create MapAttributes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // GameAttributes table
    sql =
        "CREATE TABLE IF NOT EXISTS GameAttributes ("
        "Type TEXT NOT NULL, "
        "Name TEXT NOT NULL, "
        "Value TEXT, "
        "PRIMARY KEY(Type, Name));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create GameAttributes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerAttributePoints table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerAttributePoints ("
        "PlayerID INTEGER NOT NULL, "
        "AttributeType TEXT NOT NULL, "
        "Available INTEGER NOT NULL, "
        "Spent INTEGER NOT NULL, "
        "PRIMARY KEY(PlayerID, AttributeType));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerAttributePoints table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sql =
        "CREATE TABLE IF NOT EXISTS PlayerAttributes ("
        "ID INTEGER NOT NULL, "
        "Type TEXT NOT NULL, "
        "Name TEXT NOT NULL, "
        "Value TEXT, "
        "PRIMARY KEY(ID, Type, Name));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerAttributes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Districts table
    sql =
        "CREATE TABLE IF NOT EXISTS Districts ("
        "DistrictType TEXT NOT NULL, "
        "Owner INTEGER NOT NULL, "
        "CityID INTEGER NOT NULL, "
        "Plot INTEGER NOT NULL, "
        "PRIMARY KEY(Plot));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create Districts table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // ProgressionTreeNodes table
    sql =
        "CREATE TABLE IF NOT EXISTS ProgressionTreeNodes ("
        "ID INTEGER PRIMARY KEY, "
        "NodeType TEXT, "
        "Depth INTEGER, "
        "RepeatedDepth INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create ProgressionTreeNodes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerNarrativeTagPoints table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerNarrativeTagPoints ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "NarrativeTagType TEXT, "
        "Value INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerNarrativeTagPoints table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerUnlockedTraditions table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerUnlockedTraditions ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "TraditionType TEXT);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerUnlockedTraditions table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerWorkers table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerWorkers ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "PlotID INTEGER, "
        "Placed INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerWorkers table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerUniqueGreatWorks table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerUniqueGreatWorks ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "GreatWorkType TEXT, "
        "Age INTEGER, "
        "TurnCreated INTEGER);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerUniqueGreatWorks table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerVictoryPoints table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerVictoryPoints ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "VictoryType TEXT, "
        "PointID INTEGER, "
        "Name TEXT, "
        "Points REAL, "
        "Turn INTEGER, "
        "Age INTEGER, "
        "TrackerType TEXT, "
        "Data TEXT);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerVictoryPoints table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlayerPastAgeVictoryHistory table
    sql =
        "CREATE TABLE IF NOT EXISTS PlayerPastAgeVictoryHistory ("
        "ID INTEGER PRIMARY KEY, "
        "PlayerID INTEGER, "
        "VictoryType TEXT, "
        "Age INTEGER, "
        "Turn INTEGER, "
        "Points REAL);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlayerPastAgeVictoryHistory table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // HistoricMoments table
    sql =
        "CREATE TABLE IF NOT EXISTS HistoricMoments ("
        "RowId INTEGER PRIMARY KEY AUTOINCREMENT, "
        "MomentType INTEGER NOT NULL, "
        "Age INTEGER NOT NULL, "
        "Turn INTEGER NOT NULL, "
        "PlotIndex INTEGER NOT NULL, "
        "ActingPlayer INTEGER NOT NULL, "
        "TargetPlayer INTEGER, "
        "UnitType INTEGER, "
        "ConstructibleType INTEGER, "
        "ConstructibleType2 INTEGER, "
        "NamedRiverType INTEGER, "
        "IndependentType INTEGER, "
        "NamedVolcanoType INTEGER, "
        "SiteAge INTEGER NOT NULL);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create HistoricMoments table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // RevealedPlots table
    sql =
        "CREATE TABLE IF NOT EXISTS RevealedPlots ("
        "ID INTEGER NOT NULL, "
        "Player INTEGER, "
        "PRIMARY KEY(ID, Player));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create RevealedPlots table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotEffects table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotEffects ("
        "ID INTEGER NOT NULL, "
        "EffectType INTEGER NOT NULL, "
        "PRIMARY KEY(ID, EffectType));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotEffects table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotOwners table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotOwners ("
        "ID INTEGER NOT NULL, "
        "Owner INTEGER, "
        "CityOwner INTEGER, "
        "CityWorking INTEGER, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotOwners table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotRoutes table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotRoutes ("
        "ID INTEGER NOT NULL, "
        "RouteType TEXT, "
        "RouteAge TEXT, "
        "IsRoutePillaged BOOLEAN, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotRoutes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotImprovements table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotImprovements ("
        "ID INTEGER NOT NULL, "
        "ImprovementType TEXT, "
        "ImprovementOwner INTEGER, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotImprovements table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Create PlotFeatures table  
    sql =
        "CREATE TABLE IF NOT EXISTS PlotFeatures ("
        "ID INTEGER PRIMARY KEY, "
        "FeatureType TEXT);";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotFeatures table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Insert plot features data
    sql = "INSERT OR REPLACE INTO PlotFeatures (ID, FeatureType) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);

        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                Civ7Tile tile = m_civ7TerrainType.get(x, y);

                if ((int)tile.feature > 0)
                {
                    string featureTypeNameS = getFeatureTypeAsString(tile.feature);

                    sqlite3_bind_int(stmt, 1, x + y * m_width);                                     // ID
                    sqlite3_bind_text(stmt, 2, featureTypeNameS.c_str(), -1, SQLITE_STATIC);        // FeatureType

                    sqlite3_step(stmt);
                    sqlite3_reset(stmt);
                }
            }
        }

        // Commit the transaction
        sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);

        sqlite3_finalize(stmt);
    }

    // PlotResources table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotResources ("
        "ID INTEGER NOT NULL, "
        "ResourceType TEXT, "
        "ResourceCount INTEGER, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotResources table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotCliffs table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotCliffs ("
        "ID INTEGER NOT NULL, "
        "IsNEOfCliff BOOLEAN, "
        "IsWOfCliff BOOLEAN, "
        "IsNWOfCliff BOOLEAN, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotCliffs table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotRivers table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotRivers ("
        "ID INTEGER NOT NULL, "
        "Size INTEGER, "
        "Outflow INTEGER, "
        "NEInflow BOOLEAN, "
        "EInflow BOOLEAN, "
        "SEInflow BOOLEAN, "
        "SWInflow BOOLEAN, "
        "WInflow BOOLEAN, "
        "NWInflow BOOLEAN, "
        "PRIMARY KEY(ID));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotRivers table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotAttributes table
    sql =
        "CREATE TABLE IF NOT EXISTS PlotAttributes ("
        "ID INTEGER NOT NULL, "
        "Type TEXT NOT NULL, "
        "Name TEXT NOT NULL, "
        "Value TEXT, "
        "PRIMARY KEY(ID, Type, Name));";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create PlotAttributes table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Create Plots table
    sql = 
        "CREATE TABLE IF NOT EXISTS Plots ("
        "ID INTEGER PRIMARY KEY, "
        "TerrainType TEXT, "
        "BiomeType TEXT, "
        "ContinentType TEXT, "
        "Elevation INTEGER, "
        "IsImpassable INTEGER, "
        "Tag TEXT, "
        "LandmassRegionId INTEGER);";
    
    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Failed to create Plots table: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Insert plot data
    sql = "INSERT OR REPLACE INTO Plots (ID, TerrainType, BiomeType, ContinentType, Elevation, IsImpassable, Tag, LandmassRegionId) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);

        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                Civ7Tile tile = m_civ7TerrainType.get(x, y);

                string terrainTypeS = getTerrainTypeAsString(tile.terrain);
                string biomeTypeS = getBiomeTypeAsString(tile.biome);
                string continentTypeS = getContinentName(tile.continent);

                sqlite3_bind_int(stmt, 1, x + y * m_width);                             // ID
                sqlite3_bind_text(stmt, 2, terrainTypeS.c_str(), -1, SQLITE_STATIC);    // TerrainType
                sqlite3_bind_text(stmt, 3, biomeTypeS.c_str(), -1, SQLITE_STATIC);      // BiomeType
                sqlite3_bind_text(stmt, 4, continentTypeS.c_str(), -1, SQLITE_STATIC);  // ContinentType
                sqlite3_bind_int(stmt, 5, tile.elevation);                              // Elevation
                sqlite3_bind_int(stmt, 6, 0);                                           // IsImpassable (commented out)
                sqlite3_bind_text(stmt, 7, "", -1, SQLITE_STATIC);                                     // Tag (commented out)
                sqlite3_bind_int(stmt, 8, tile.landmass);                               // LandmassRegionId

                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }
        }

        // Commit the transaction
        sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);

        sqlite3_finalize(stmt);
    }

    // Close the database connection
    sqlite3_close(db);
}
