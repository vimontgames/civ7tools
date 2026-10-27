#define SQL(func) { rc = func; if (rc != SQLITE_OK && rc != SQLITE_DONE  && rc != SQLITE_ROW) { LOG_ERROR("Query \"%s\" failed : %s", sql.c_str(), sqlite3_errmsg(db)); } }

#include "winsqlite/winsqlite3.h"
#include "map/map.h"

//--------------------------------------------------------------------------------------
DirectionType Map::GetOppositeDirection(DirectionType direction)
{
    switch (direction)
    {
        case DirectionType::East:
            return DirectionType::West;

        case DirectionType::West:
            return DirectionType::East;

        case DirectionType::NorthEast:
            return DirectionType::SouthWest;

        case DirectionType::NorthWest:
            return DirectionType::SouthEast;

        case DirectionType::SouthEast:
            return DirectionType::NorthWest;

        case DirectionType::SouthWest:
            return DirectionType::NorthEast;
    }

    ASSERT("Invalid direction \"%s\" (%i)", asString(direction).c_str(), (int)direction);
    return (DirectionType)0;
}

//--------------------------------------------------------------------------------------
bool Map::AreOpposedDirections(DirectionType dir1, DirectionType dir2)
{
    switch (dir1)
    {
        case DirectionType::East:
        {
            switch (dir2)
            {
                case DirectionType::East:
                case DirectionType::NorthEast:
                case DirectionType::SouthEast:
                    return false;

                case DirectionType::West:
                case DirectionType::NorthWest:
                case DirectionType::SouthWest:
                    return true;
            }
        }
        break;

        case DirectionType::West:
        {
            switch (dir2)
            {
                case DirectionType::West:
                case DirectionType::NorthWest:
                case DirectionType::SouthWest:
                    return false;

                case DirectionType::East:
                case DirectionType::NorthEast:
                case DirectionType::SouthEast:
                    return true;
            }
        }
        break;

        case DirectionType::NorthEast:
        {
            switch (dir2)
            {
                case DirectionType::NorthEast:
                case DirectionType::East:
                case DirectionType::NorthWest:
                    return false;

                case DirectionType::SouthWest:
                case DirectionType::West:
                case DirectionType::SouthEast:
                    return true;
            }
        }
        break;

        case DirectionType::NorthWest:
        {
            switch (dir2)
            {
                case DirectionType::NorthWest:
                case DirectionType::West:
                case DirectionType::NorthEast:
                    return false;

                case DirectionType::SouthEast:
                case DirectionType::East:
                case DirectionType::SouthWest:
                    return true;
            }
        }
        break;

        case DirectionType::SouthEast:
        {
            switch (dir2)
            {
                case DirectionType::SouthEast:
                case DirectionType::East:
                case DirectionType::SouthWest:
                    return false;

                case DirectionType::NorthWest:
                case DirectionType::West:
                case DirectionType::NorthEast:
                    return true;
            }
        }
        break;

        case DirectionType::SouthWest:
        {
            switch (dir2)
            {
                case DirectionType::SouthWest:
                case DirectionType::West:
                case DirectionType::SouthEast:
                    return false;

                case DirectionType::NorthEast:
                case DirectionType::East:
                case DirectionType::NorthWest:
                    return true;
            }
        }
        break;

        default:
            break;
    }

    ASSERT("Invalid direction \"%s\" (%i) and \"%s\" (%i)", asString(dir1).c_str(), (int)dir1, asString(dir2).c_str(), (int)dir2);
    return false;
}

//--------------------------------------------------------------------------------------
int2 Map::GetTileCoordsInDirection(int x, int y, DirectionType direction) const
{
    switch (direction)
    {
        case DirectionType::East:
            return int2((x + 1) % m_width, y);

        case DirectionType::West:
            return int2((x + m_width - 1) % m_width, y);

        case DirectionType::NorthEast:
            return (y & 1) ? int2((x + 1) % m_width, y + 1) : int2(x, y + 1);

        case DirectionType::NorthWest:
            return (y & 1) ? int2(x, y + 1) : int2((x + m_width - 1) % m_width, y + 1);

        case DirectionType::SouthEast:
            return (y & 1) ? int2((x + 1) % m_width, y - 1) : int2(x, y - 1);

        case DirectionType::SouthWest:
            return (y & 1) ? int2(x, y - 1) : int2((x + m_width - 1) % m_width, y - 1);

        default:
            ASSERT("Invalid direction \"%s\" (%i)", asString(direction).c_str(), (int)direction);
            return int2(x, y);
    }
}


//--------------------------------------------------------------------------------------
bool Map::UpdateRivers(bool verbose)
{
    struct RiverNode
    {
        int2 xy;
        int nextIndex = -1;
        int incomingCount = 0;
    };

    // River analysis
    for (int i = 0; i < (int)m_rivers.size(); ++i)
    {
        RiverInfo & riverInfo = m_rivers[i];
        riverInfo.riverPaths.clear();
        riverInfo.disconnectedTiles.clear();
        riverInfo.tileCount = 0;

        vector<RiverNode> nodes;
        vector<int> nodeAt(m_width * m_height, -1);

        // #1 Collect all tiles belonging to this river.
        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                const Civ7Tile & tile = m_civ7TerrainType.get(x, y);

                for (uint r = 0; r < MAX_RIVER_PER_TILE; ++r)
                {
                    const RiverTileInfo & riverTile = tile.rivers[r];

                    if (riverTile.nameIndex == (RiverName)i)
                    {
                        const int nodeIndex = (int)nodes.size();
                        nodes.push_back({ int2(x, y) });
                        nodeAt[y * m_width + x] = nodeIndex;
                        break;
                    }
                }
            }
        }

        riverInfo.tileCount = (int)nodes.size();

        if (nodes.empty())
        {
            if (verbose)
                LOG_INFO("River \"%s\" (%d) has no river tiles", riverInfo.prettyName.c_str(), i);

            continue;
        }

        // #2 Build the directed graph.
        for (int n = 0; n < (int)nodes.size(); ++n)
        {
            const int2 xy = nodes[n].xy;
            const Civ7Tile & tile = m_civ7TerrainType.get(xy.x, xy.y);

            for (uint r = 0; r < MAX_RIVER_PER_TILE; ++r)
            {
                const RiverTileInfo & riverTile = tile.rivers[r];

                if (riverTile.nameIndex != (RiverName)i)
                    continue;

                const int2 nextXY = GetTileCoordsInDirection(xy.x, xy.y, riverTile.direction);

                if (nextXY.x >= 0 && nextXY.y >= 0 && nextXY.x < (int)m_width && nextXY.y < (int)m_height)
                {
                    const int nextIndex = nodeAt[nextXY.y * m_width + nextXY.x];

                    if (nextIndex >= 0)
                    {
                        nodes[n].nextIndex = nextIndex;
                        nodes[nextIndex].incomingCount++;

                        if (verbose)
                            LOG_INFO("River \"%s\" tile (%d,%d) points to (%d,%d)", riverInfo.prettyName.c_str(), xy.x, xy.y, nextXY.x, nextXY.y);
                    }
                }

                break;
            }
        }

        // #3 Find all starts (tiles with no incoming connections).
        vector<int> starts;

        for (int n = 0; n < (int)nodes.size(); ++n)
        {
            if (nodes[n].incomingCount == 0)
            {
                starts.push_back(n);

                if (verbose)
                    LOG_INFO("River \"%s\" tile (%d,%d) is a start", riverInfo.prettyName.c_str(), nodes[n].xy.x, nodes[n].xy.y);
            }

            if (nodes[n].nextIndex < 0)
            {
                if (verbose)
                    LOG_INFO("River \"%s\" tile (%d,%d) is an end", riverInfo.prettyName.c_str(), nodes[n].xy.x, nodes[n].xy.y);
            }
        }

        // #4 Trace a path from a given start.
        // Shared downstream tiles are allowed in multiple paths.
        vector<bool> covered(nodes.size(), false);

        auto tracePath = [&](int startIndex)
        {
            RiverPath path;
            vector<bool> visited(nodes.size(), false);
            int currentIndex = startIndex;

            while (currentIndex >= 0 && !visited[currentIndex])
            {
                visited[currentIndex] = true;
                covered[currentIndex] = true;
                path.tiles.push_back(nodes[currentIndex].xy);
                currentIndex = nodes[currentIndex].nextIndex;
            }

            if (!path.tiles.empty())
                riverInfo.riverPaths.push_back(path);
        };

        for (int startIndex : starts)
            tracePath(startIndex);

        // #5 Mark tiles belonging to paths that don't reach the main river end.
        int2 mainEnd(-1, -1);
        size_t longestPathLength = 0;

        for (const RiverPath & path : riverInfo.riverPaths)
        {
            if (path.tiles.size() > longestPathLength)
            {
                longestPathLength = path.tiles.size();
                mainEnd = path.tiles.back();
            }
        }

        for (const RiverPath & path : riverInfo.riverPaths)
        {
            if (path.tiles.empty() || path.tiles.back() == mainEnd)
                continue;

            for (const int2 & xy : path.tiles)
            {
                if (std::find(riverInfo.disconnectedTiles.begin(), riverInfo.disconnectedTiles.end(), xy) == riverInfo.disconnectedTiles.end())
                    riverInfo.disconnectedTiles.push_back(xy);
            }
        }

        // Also detect tiles that were not reached from any start, such as cycles.
        for (int n = 0; n < (int)nodes.size(); ++n)
        {
            if (!covered[n] && std::find(riverInfo.disconnectedTiles.begin(), riverInfo.disconnectedTiles.end(), nodes[n].xy) == riverInfo.disconnectedTiles.end())
                riverInfo.disconnectedTiles.push_back(nodes[n].xy);
        }

        if (verbose && !riverInfo.disconnectedTiles.empty())
        {
            string msg = fmt::sprintf("River \"%s\" (%d) has %u disconnected tiles:", riverInfo.prettyName.c_str(), i, (uint)riverInfo.disconnectedTiles.size());

            for (const int2 & xy : riverInfo.disconnectedTiles)
                msg += fmt::sprintf(" (%d,%d)", xy.x, xy.y);

            LOG_ERROR("%s", msg.c_str());
        }

        // #6 Log the resulting paths.
        if (verbose)
        {
            LOG_INFO("River \"%s\" (%d): %d tiles, %d paths", riverInfo.prettyName.c_str(), i, riverInfo.tileCount, (int)riverInfo.riverPaths.size());

            for (int p = 0; p < (int)riverInfo.riverPaths.size(); ++p)
            {
                string msg = fmt::sprintf("River \"%s\" path %d:", riverInfo.prettyName.c_str(), p);

                for (const int2 & xy : riverInfo.riverPaths[p].tiles)
                    msg += fmt::sprintf(" (%d,%d)", xy.x, xy.y);

                LOG_INFO("%s", msg.c_str());
            }
        }
    }

    return true;
}


//--------------------------------------------------------------------------------------
void Map::exportFilesCiv7Map(const string & _cwd, bool _useModTemplate)
{
    UpdateRivers(true);

    exportModInfoCiv7Map();
    exportTextCiv7Map();
    exportConfigCiv7Map();
    exportMapsCiv7Map();
    exportSQLiteMap();
    exportMapDataCiv7Map();
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
    //data += "              <ImportFiles>\n";
	//data += "				    <Item>assets/icon.png</Item>\n";
	//data += "			   </ImportFiles>\n";
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
    string mapSize = getExportMapSize();

    data += fmt::sprintf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    data += fmt::sprintf("<Database>\n");
    data += fmt::sprintf("	<Maps>\n");
    data += fmt::sprintf("		<Row File=\"{%s}maps/%s.Civ7Map\" Name=\"%s\" Description=\"%s\" SortIndex=\"67\"/>\n", getModID(), getBaseName(), getPrettyName(), getDescription());
    data += fmt::sprintf("	</Maps>\n");
    data += fmt::sprintf("	<UnsupportedValuesByAge>\n");
    data += fmt::sprintf("		<Row AgeType=\"AGE_EXPLORATION\" Domain=\"StandardMaps\" Value=\"{%s}maps/%s.Civ7Map\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("		<Row AgeType=\"AGE_MODERN\" Domain=\"StandardMaps\" Value=\"{%s}maps/%s.Civ7Map\"/>\n", getModID(), getBaseName());
    data += fmt::sprintf("	</UnsupportedValuesByAge>\n");

    if (m_mapSize == MapSize::Custom)
    {
        string mapSizePrettyName = getExportMapSizePrettyName();
        string mapSizePrettyDescription = getExportMapSizePrettyDescription();

        data += fmt::sprintf("	<MapSizes>\n");
        data += fmt::sprintf("		<Row Domain=\"StandardMapSizes\" MapSizeType=\"%s\" Name=\"%s\" Description=\"%s\" MinPlayers=\"%u\" MaxPlayers=\"%u\" MaxHumans=\"%u\" DefaultPlayers=\"%u\" SortIndex=\"61\"/>\n", mapSize, mapSizePrettyName, mapSizePrettyDescription, m_minPlayers, m_maxPlayers, m_maxHumanPlayers, m_defaultPlayers);
        data += fmt::sprintf("		<Row Domain=\"DistantLandsMapSizes\" MapSizeType=\"%s\" Name=\"%s\" Description=\"%s\" MinPlayers=\"%u\" MaxPlayers=\"%u\" MaxHumans=\"%u\" DefaultPlayers=\"%u\" SortIndex=\"61\"/>\n", mapSize, mapSizePrettyName, mapSizePrettyDescription, m_minPlayers, m_maxPlayers, m_maxHumanPlayers, m_defaultPlayers);
        data += fmt::sprintf("	</MapSizes>\n");
    }

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

    if (m_mapSize == MapSize::Custom)
    {
        string mapSizeName = getExportMapSize();
        string mapSizePrettyName = getExportMapSizePrettyName();
        string mapSizePrettyDescription = getExportMapSizePrettyDescription();

        data += fmt::sprintf("	<Types>\n");
        data += fmt::sprintf("		<Replace Type=\"%s\" Kind=\"KIND_MAPSIZE\"/>\n", mapSizeName);
        data += fmt::sprintf("	</Types>\n");
        data += fmt::sprintf("	<Maps>\n");
        data += fmt::sprintf("		<Row MapSizeType=\"%s\" Name=\"%s\" Description=\"%s\" DefaultPlayers=\"8\" PlayersLandmass1=\"6\" PlayersLandmass2=\"2\" GridWidth=\"%u\" GridHeight=\"%u\" NumNaturalWonders=\"7\" OceanWidth=\"8\" LakeSizeCutoff=\"10\" LakeGenerationFrequency=\"25\" Continents=\"6\" StartSectorRows=\"4\" StartSectorCols=\"3\"/>\n", mapSizeName, mapSizePrettyName, mapSizePrettyDescription, m_width, m_height);
        data += fmt::sprintf("	</Maps>\n");
    }

    if (m_exportPaintedRivers && m_rivers.size() > 0)
    {
        data += fmt::sprintf("  <NamedRivers>\n");
        for (uint i = 0; i < m_rivers.size(); ++i)
        {
            const RiverInfo & riverInfo = m_rivers[i];
            data += fmt::sprintf("      <Row NamedRiverType=\"%s\" Name=\"%s\"/>\n", riverInfo.name, riverInfo.prettyName);
        }
        data += fmt::sprintf("  </NamedRivers>\n");
    }

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
    data += fmt::sprintf("import { assignStartPositionsFromHexMap } from '/base-standard/maps/assign-starting-plots.js';\n");
    data += fmt::sprintf("import { HexMap } from '/base-standard/scripts/hex-map.js';\n");
    data += fmt::sprintf("import { profileScope } from '/base-standard/scripts/profiling.js';\n");
    data += fmt::sprintf("\n");
    data += fmt::sprintf("console.log(\"Generatingmap from Civ7Map script %s.\");\n", GetFilename(m_mapDataPath));
    data += fmt::sprintf("function requestMapData(initParams) {\n");
    data += fmt::sprintf("  console.log(\"Begin requestMapData()\");\n");
    data += fmt::sprintf("  console.log(initParams.width);\n");
    data += fmt::sprintf("  console.log(initParams.height);\n");
    data += fmt::sprintf("  console.log(initParams.topLatitude);\n");
    data += fmt::sprintf("  console.log(initParams.bottomLatitude);\n");
    data += fmt::sprintf("  console.log(initParams.wrapX);\n");
    data += fmt::sprintf("  console.log(initParams.wrapY);\n");
    data += fmt::sprintf("  console.log(initParams.mapSize);\n");
    data += fmt::sprintf("  engine.call(\"SetMapInitData\", initParams);\n");
    data += fmt::sprintf("  console.log(\"End requestMapData()\");\n");
    data += fmt::sprintf("}\n");
    data += fmt::sprintf("async function generateMap() {\n");
    data += fmt::sprintf("  console.log(\"Begin generateMap()\");\n");
    data += fmt::sprintf("  console.log(`Age - ${GameInfo.Ages.lookup(Game.age).AgeType}`);\n");
    data += fmt::sprintf("  const civ7MapScope = new profileScope(\"%s Generation\");\n", GetFilename(m_mapDataPath));
    data += fmt::sprintf("  const iWidth = GameplayMap.getGridWidth();\n");
    data += fmt::sprintf("  const iHeight = GameplayMap.getGridHeight();\n");
    data += fmt::sprintf("  const uiMapSize = GameplayMap.getMapSize();\n");
    data += fmt::sprintf("  const mapInfo = GameInfo.Maps.lookup(uiMapSize);\n");
    data += fmt::sprintf("  if (mapInfo == null) return;\n");
    data += fmt::sprintf("  const hexMap = new HexMap();\n");
    data += fmt::sprintf("  hexMap.initFromTerrainBuilder();\n");
    data += fmt::sprintf("  paintElevation();\n");
    data += fmt::sprintf("  paintNaturalWonders(iWidth, iHeight);\n");
    data += fmt::sprintf("  //paintEarthHugeResourcesAntiquity();\n");
    data += fmt::sprintf("  const topSnowRows = %u;\n", m_topSnowRows);
    data += fmt::sprintf("  const bottomSnowRows = %u;\n", m_bottomSnowRows);
    data += fmt::sprintf("  const maxSnowWeight = %u;\n", m_maxSnowWeight);
    data += fmt::sprintf("  const snowRandomization = %u;\n", m_snowRandomization);
    data += fmt::sprintf("  const paintedSnowRandomization = %u;\n", m_paintedSnowRandomization);
    data += fmt::sprintf("  paintSnow(iWidth, iHeight, topSnowRows, bottomSnowRows, maxSnowWeight, snowRandomization, paintedSnowRandomization);\n");

    data += fmt::sprintf("  const genCtx = new GenerationContext();\n");

    //data += fmt::sprintf("  genCtx.phases = GenerationPhases.Rainfall | GenerationPhases.FloodPlains | GenerationPhases.Resources | GenerationPhases.Features | GenerationPhases.Elevation | GenerationPhases.Rivers | GenerationPhases.NaturalWonders | GenerationPhases.Snow | GenerationPhases.WriteToTerrainBuilder;\n");

    string genFlags = "GenerationPhases.WriteToTerrainBuilder"; 
        
    if (m_randomLakes)
        genFlags += "| GenerationPhases.Lakes";

    if (m_randomContinents)
        genFlags += "| GenerationPhases.Continents";

    if (m_randomElevation)
        genFlags += "| GenerationPhases.Elevation";

    if (m_randomHills)
        genFlags += "| GenerationPhases.Hills";

    if (m_randomRainfall)
        genFlags += "| GenerationPhases.Rainfall";

    if (m_randomRivers)
        genFlags += "| GenerationPhases.Rivers";

    if (m_randomBiomes)
        genFlags += "| GenerationPhases.Biomes";

    if (m_randomNaturalWonders)
        genFlags += "| GenerationPhases.NaturalWonders";

    if (m_randomFloodPlains)
        genFlags += "| GenerationPhases.RandomFloodPlains";

    if (m_randomFeatures)
        genFlags += "| GenerationPhases.Features";

    if (m_randomSnow && !m_useAdvancedSnow)
        genFlags += "| GenerationPhases.Snow";

    if (m_randomResources)
        genFlags += "| GenerationPhases.Resources";

    data += fmt::sprintf("  genCtx.phases = %s;\n", genFlags);
    data += fmt::sprintf("  genCtx.bRunAestheticRiverValidation = false;\n");
    data += fmt::sprintf("  generateMapFeatures(hexMap, genCtx);\n");
    data += fmt::sprintf("  paintRivers();\n");
    data += fmt::sprintf("  nameRivers();\n");
    data += fmt::sprintf("  //nameVolcanoes();\n");

    if (m_noElevationOnNavigableRivers)
    {
        data += fmt::sprintf("  fixElevation();\n");
    }

    // Fix navigable rivers
    data += fmt::sprintf("  genCtx.phases = 0; //GenerationPhases.WriteToTerrainBuilder;\n");
    data += fmt::sprintf("  genCtx.bRunAestheticRiverValidation = false;\n");
    data += fmt::sprintf("  generateMapFeatures(hexMap, genCtx);\n");

    if (m_snowBorderX > 0)
    {
        data += fmt::sprintf("  fakeWrapX();\n");
    }

    uint tslCount = 0;
    if (m_useTSL)
    {
        for (uint i = 0; i < m_civilizations.size(); ++i)
        {
            const Civilization & civ = m_civilizations[i];
            if (civ.tsl.size() > 0)
            {
                tslCount++;
            }
        }
    }

    if (tslCount > 0)
    {
        data += fmt::sprintf("  const startPositions = assignStartPositions();\n");
    }
    else
    {
        data += fmt::sprintf("  const startPositions = assignStartPositionsFromHexMap(hexMap);");
    }

    data += fmt::sprintf("  generateDiscoveries(iWidth, iHeight, startPositions, g_PolarWaterRows);\n");
    data += fmt::sprintf("  civ7MapScope.end();\n");
    data += fmt::sprintf("  console.log(\"End generateMap()\");\n");
    data += fmt::sprintf("}\n");

    data += fmt::sprintf("function paintElevation() {\n");
    data += "    let elevationArray = [";
    for (uint y = 0; y < m_height; ++y)
    {
        for (uint x = 0; x < m_width; ++x)
        {
            Civ7Tile tile = m_civ7TerrainType.get(x, y);
            if (x == 0 && y == 0)
                data += fmt::sprintf("%i", (int)tile.elevation);
            else
                data += fmt::sprintf(", %i", (int)tile.elevation);
        }
    }
    data += "];\n";
    data += fmt::sprintf("    TerrainBuilder.setElevation(elevationArray);\n");
    data += fmt::sprintf("    TerrainBuilder.generateCliffsFromElevation();\n");
    data += fmt::sprintf("}\n\n");

    data += "function fixElevation() {\n";
    data += "    console.log(\"Begin fixElevation()\");\n";
    data += "    const mapWidth = GameplayMap.getGridWidth();\n";
    data += "    const mapHeight = GameplayMap.getGridHeight();\n";
    data += "    let elevationArray = [];\n\n";

    data += "    for (let y = 0; y < mapHeight; y++) {\n";
    data += "        for (let x = 0; x < mapWidth; x++) {\n";
    data += "            let index = (y * mapWidth) + x;\n";
    data += "            elevationArray[index] = GameplayMap.getElevation(x, y);\n";
    data += "        }\n";
    data += "    }\n\n";

    // Zero elevation on navigable river tiles
    for (uint y = 0; y < m_height; ++y)
    {
        for (uint x = 0; x < m_width; ++x)
        {
            Civ7Tile tile = m_civ7TerrainType.get(x, y);
            for (uint r = 0; r < MAX_RIVER_PER_TILE; ++r)
            {
                if (m_noElevationOnNavigableRivers)
                {
                    if (tile.rivers[r].nameIndex != RiverName::None && tile.rivers[r].type == RiverType::Navigable)
                    {
                        data += fmt::sprintf("    elevationArray[(%i * mapWidth) + %i] = 0;\n", y, x);
                    }
                }
            }
        }
    }

    data += "    TerrainBuilder.setElevation(elevationArray);\n";
    data += "    TerrainBuilder.generateCliffsFromElevation();\n";
    data += "    console.log(\"end fixElevation()\");\n";
    data += "}\n\n";

    // Rivers plots
    data += "function paintRivers()\n";
    data += "{\n";

    if (m_exportPaintedRivers)
    {
        data += "  console.log(\"Begin paintRivers()\");\n";
        data += "  const emptyFeatureParam = \n";
        data += "  {\n";
        data += "    Feature: -1,\n";
        data += "    Direction: -1,\n";
        data += "    Elevation: 0\n";
        data += "  }; \n";

        for (uint i = 0; i < m_rivers.size(); ++i)
        {
            const RiverInfo & riverInfo = m_rivers[i];
            if (!riverInfo.riverPaths.empty())
            {
                data += fmt::sprintf("  // %s\n", riverInfo.name.c_str());

                for (uint p = 0; p < riverInfo.riverPaths.size(); ++p)
                {
                    const RiverPath & riverPath = riverInfo.riverPaths[p];

                    for (uint j = 0; j < riverPath.tiles.size(); ++j)
                    {
                        const int2 & tileXY = riverPath.tiles[j];
                        const Civ7Tile & tile = m_civ7TerrainType.get(tileXY.x, tileXY.y);

                        for (uint r = 0; r < MAX_RIVER_PER_TILE; ++r)
                        {
                            const RiverTileInfo & riverTile = tile.rivers[r];
                            if (riverTile.nameIndex == (RiverName)i)
                            {
                                if (riverTile.type == RiverType::Navigable)
                                {
                                    if (m_noFeatureOnNavigableRivers)
                                    {
                                        data += fmt::sprintf("  TerrainBuilder.setFeatureType(%u, %u, emptyFeatureParam);\n", tileXY.x, tileXY.y);
                                    }

                                    if (m_noResourceOnNavigableRivers)
                                    {
                                        data += fmt::sprintf("  ResourceBuilder.setResourceType(%u,%u, -1);", tileXY.x, tileXY.y);
                                    }
                                }
                                data += fmt::sprintf("  TerrainBuilder.setRiverInfo(%u, %u, DirectionTypes.%s, RiverTypes.%s);\n", tileXY.x, tileXY.y, getDirectionTypeAsString(riverTile.direction), getRiverTypeAsString(riverTile.type).c_str());
                            }
                        }
                    }
                }
            }
        }
        data += "  console.log(\"End paintRivers()\");\n";
    }
    else
    {
        data += "   console.log(\"Painted rivers are disabled\");\n";
    }

    
    data += "}\n\n";

    data += "function nameRivers()\n";
    data += "{\n";

    if (m_exportPaintedRivers)
    {
        data += "   console.log(\"Begin nameRivers()\");\n";
        for (uint i = 0; i < m_rivers.size(); ++i)
        {
            const RiverInfo & riverInfo = m_rivers[i];
            if (!riverInfo.riverPaths.empty() && !riverInfo.riverPaths[0].tiles.empty())
            {
                const int2 & startXY = riverInfo.riverPaths[0].tiles[0];
                data += fmt::sprintf("   TerrainBuilder.setCustomRiverName(%u, %u, \"%s\");\n", startXY.x, startXY.y, riverInfo.prettyName.c_str());
            }
        }
        data += "   console.log(\"End nameRivers()\");\n";
    }
    else
    {
        data += "   console.log(\"Painted rivers are disabled\");\n";
    }
    
    data += "}\n\n";

    // TSL
    if (m_useTSL && tslCount > 0)
    {
        data += "function getDistanceToClosestOtherStart(iX, iY, startPositions, skipIndex) { \n";
        data += "  let minDistance = 32768;\n";
        data += "  for (let iStart = 0; iStart < startPositions.length; iStart++) {\n";
        data += "    const startPlotIndex = startPositions[iStart];\n";
        data += "    if (startPlotIndex && iStart != skipIndex) {\n";
        data += "      const iStartX = startPlotIndex % GameplayMap.getGridWidth();\n";
        data += "      const iStartY = startPlotIndex / GameplayMap.getGridWidth();\n";
        data += "      const distance = GameplayMap.getPlotDistance(iX, iY, iStartX, iStartY);\n";
        data += "      if (distance < minDistance) {\n";
        data += "        minDistance = distance;\n";
        data += "      }\n";
        data += "    }\n";
        data += "  }\n";
        data += "  return minDistance;\n";
        data += "}\n";

        data += fmt::sprintf("function getRandomCivStartLocation() {\n");
        data += fmt::sprintf("  const randomCiv = TerrainBuilder.getRandomNumber(%u, \"CIV TSL RANDOMIZATION\");\n", tslCount);
        data += fmt::sprintf("  console.log(randomCiv + \" \");\n");
        data += fmt::sprintf("  let plotIndex = -1;\n");
        data += fmt::sprintf("  switch (randomCiv) {\n");

        //data += "    case 0:\n";
        //data += "      plotIndex = GameplayMap.getIndexFromXY(61, 28);\n";
        //data += "      break;\n";

        uint tslIndex = 0;
        for (uint i = 0; i < m_civilizations.size(); ++i)
        {
            const Civilization & civ = m_civilizations[i];
            if (civ.tsl.size() > 0)
            {
                const TSL & tsl = civ.tsl[0];

                data += fmt::sprintf("    case %u:\n", tslIndex);
                data += fmt::sprintf("      plotIndex = GameplayMap.getIndexFromXY(%u, %u);\n", tsl.pos.x, tsl.pos.y);
                data += fmt::sprintf("      break;\n");

                tslIndex++;
            }
        }

        data += "  }\n";
        data += "  return plotIndex;\n";
        data += "}\n";

        data += fmt::sprintf("function assignStartPositions() {\n");

        data += fmt::sprintf("  const aliveMajorIds = Players.getAliveMajorIds();\n");
        data += fmt::sprintf("  const startPositions = new Array(aliveMajorIds.length);\n");
        data += fmt::sprintf("  let plotIndex = -1;\n");
        data += fmt::sprintf("  for (const majorId of aliveMajorIds) {\n");
        data += fmt::sprintf("    const player = Players.get(majorId);\n");
        data += fmt::sprintf("    if (player != null) {\n");
        data += fmt::sprintf("      switch (player.civilizationName) {\n");

        //data += fmt::sprintf("        case \"LOC_CIVILIZATION_AKSUM_NAME\":\n");
        //data += fmt::sprintf("          plotIndex = GameplayMap.getIndexFromXY(61, 28);\n");
        //data += fmt::sprintf("          break;\n");

        for (uint i = 0; i < m_civilizations.size(); ++i)
        {
            const Civilization & civ = m_civilizations[i];
            if (civ.tsl.size() > 0)
            {
                const TSL & tsl = civ.tsl[0];

                if (civ.civilizationName.length() > 0)
                {
                    data += fmt::sprintf("        case \"LOC_%s_NAME\":\n", civ.civilizationName);
                    data += fmt::sprintf("          plotIndex = GameplayMap.getIndexFromXY(%u, %u);\n", tsl.pos.x, tsl.pos.y);
                    data += fmt::sprintf("          break;\n");
                }
            }
        }

        data += fmt::sprintf("      }\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("    if (plotIndex >= 0) {\n");
        data += fmt::sprintf("      startPositions[majorId] = plotIndex;\n");
        data += fmt::sprintf("      const location2 = GameplayMap.getLocationFromIndex(plotIndex);\n");
        data += fmt::sprintf("      console.log(\"CHOICE FOR PLAYER: \" + majorId + \" (\" + location2.x + \", \" + location2.y + \")\");\n");
        data += fmt::sprintf("    } else {\n");
        data += fmt::sprintf("      console.log(\"FAILED TO PICK LOCATION FOR: \" + majorId);\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("    plotIndex = -1;\n");
        data += fmt::sprintf("  }\n");
        data += fmt::sprintf("  const validatedStartPositions = startPositions;\n");
        data += fmt::sprintf("  console.log(startPositions.length + \"startlength\");\n");
        data += fmt::sprintf("  validatedStartPositions[0] = startPositions[0];\n");
        data += fmt::sprintf("  const location = GameplayMap.getLocationFromIndex(validatedStartPositions[0]);\n");
        data += fmt::sprintf("  console.log(\"CHOICE FOR PLAYER: 0 (\" + location.x + \", \" + location.y + \")\");\n");
        data += fmt::sprintf("  StartPositioner.setStartPosition(validatedStartPositions[0], 0);\n");

        if (m_noResourceOnTSL)
        {
            data += fmt::sprintf("  ResourceBuilder.setResourceType(location.x,location.y, -1);");
        }

        data += fmt::sprintf("  for (let index = startPositions.length - 1; index > 0; index--) {\n");
        data += fmt::sprintf("    let invalidPosition = false;\n");
        data += fmt::sprintf("    let position = startPositions[index];\n");
        data += fmt::sprintf("    for (let indexCheck = 0; indexCheck < index; indexCheck++) {\n");
        data += fmt::sprintf("      console.log(\n");
        data += fmt::sprintf("        getDistanceToClosestOtherStart(\n");
        data += fmt::sprintf("          GameplayMap.getLocationFromIndex(position).x,\n");
        data += fmt::sprintf("          GameplayMap.getLocationFromIndex(position).y,\n");
        data += fmt::sprintf("          startPositions,\n");
        data += fmt::sprintf("          index\n");
        data += fmt::sprintf("        )\n");
        data += fmt::sprintf("      );\n");
        data += fmt::sprintf("      if (getDistanceToClosestOtherStart(\n");
        data += fmt::sprintf("        GameplayMap.getLocationFromIndex(position).x,\n");
        data += fmt::sprintf("        GameplayMap.getLocationFromIndex(position).y,\n");
        data += fmt::sprintf("        startPositions,\n");
        data += fmt::sprintf("        index\n");
        data += fmt::sprintf("      ) < 3) {\n");
        data += fmt::sprintf("        invalidPosition = true;\n");
        data += fmt::sprintf("        console.log(\"check1\");\n");
        data += fmt::sprintf("      } else {\n");
        data += fmt::sprintf("        console.log(index + \" fine \" + indexCheck);\n");
        data += fmt::sprintf("      }\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("    while (invalidPosition) {\n");
        data += fmt::sprintf("      position = getRandomCivStartLocation();\n");
        data += fmt::sprintf("      invalidPosition = false;\n");
        data += fmt::sprintf("      for (let indexCheck = 0; indexCheck < startPositions.length; indexCheck++) {\n");
        data += fmt::sprintf("        if (index != indexCheck) {\n");
        data += fmt::sprintf("          if (getDistanceToClosestOtherStart(\n");
        data += fmt::sprintf("            GameplayMap.getLocationFromIndex(position).x,\n");
        data += fmt::sprintf("            GameplayMap.getLocationFromIndex(position).y,\n");
        data += fmt::sprintf("            startPositions,\n");
        data += fmt::sprintf("            index\n");
        data += fmt::sprintf("          ) < 4) {\n");
        data += fmt::sprintf("            invalidPosition = true;\n");
        data += fmt::sprintf("            console.log(\"check2\");\n");
        data += fmt::sprintf("          }\n");
        data += fmt::sprintf("        }\n");
        data += fmt::sprintf("      }\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("    if (position >= 0) {\n");
        data += fmt::sprintf("      validatedStartPositions[index] = position;\n");
        data += fmt::sprintf("      const location2 = GameplayMap.getLocationFromIndex(position);\n");
        data += fmt::sprintf("      console.log(\"CHOICE FOR PLAYER: \" + index + \" (\" + location2.x + \", \" + location2.y + \")\");\n");
        data += fmt::sprintf("      StartPositioner.setStartPosition(position, index);\n");

        if (m_noResourceOnTSL)
        {
            data += fmt::sprintf("      ResourceBuilder.setResourceType(location2.x,location2.y, -1);");
        }

        data += fmt::sprintf("    } else {\n");
        data += fmt::sprintf("      console.log(\"FAILED TO PICK LOCATION FOR: \" + index);\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("  }\n");
        data += fmt::sprintf("  return validatedStartPositions;\n");
        data += fmt::sprintf("}\n");
    }

    if (m_snowBorderX > 0)
    {
        data += fmt::sprintf("function fakeWrapX()\n");
        data += fmt::sprintf("{ \n");    
        data += fmt::sprintf("    const effects = MapPlotEffects.getPlotEffectTypesContainingTags([\"SNOW\", \"HEAVY\", \"PERMANENT\"]); \n");
        data += fmt::sprintf("    const effect = effects[0]; const width = GameplayMap.getGridWidth(); \n");
        data += fmt::sprintf("    const height = GameplayMap.getGridHeight(); \n");
        data += fmt::sprintf("    for (let x = 0; x < %u; ++x) \n", m_snowBorderX);
        data += fmt::sprintf("    {\n");
        data += fmt::sprintf("        for (let y = 0; y < height; ++y) \n");
        data += fmt::sprintf("        {\n");
        data += fmt::sprintf("            if (!GameplayMap.isWater(x, y)) \n");
        data += fmt::sprintf("            { \n");
        data += fmt::sprintf("                const index = GameplayMap.getIndexFromXY(x, y); \n");
        data += fmt::sprintf("                MapPlotEffects.addPlotEffect(index, effect); \n");
        data += fmt::sprintf("            } \n");
        data += fmt::sprintf("            if (!GameplayMap.isWater(x + width - %u - 1, y)) \n", m_snowBorderX);
        data += fmt::sprintf("            { \n");
        data += fmt::sprintf("                const index = GameplayMap.getIndexFromXY(x + width - %u - 1, y); \n", m_snowBorderX);
        data += fmt::sprintf("                MapPlotEffects.addPlotEffect(index, effect); \n");
        data += fmt::sprintf("            } \n");
        data += fmt::sprintf("        } \n");
        data += fmt::sprintf("    } \n");
        data += fmt::sprintf("}\n\n");
    }
   

    data += fmt::sprintf("function paintSnow(width, height, topRows, bottomRows, maxWeight, randomization, paintRandomization)\n");
    data += fmt::sprintf("{\n");
    data += fmt::sprintf("    console.log(\"Begin paintSnow\");\n\n");

    if (m_randomSnow && m_useAdvancedSnow)
    {
        data += fmt::sprintf("    console.log(\"Generating permanent snow\");\n");
        data += fmt::sprintf("    const aLightSnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags([\"SNOW\", \"LIGHT\", \"PERMANENT\"]);\n");
        data += fmt::sprintf("    const aMediumSnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags([\"SNOW\", \"MEDIUM\", \"PERMANENT\"]);\n");
        data += fmt::sprintf("    const aHeavySnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags([\"SNOW\", \"HEAVY\", \"PERMANENT\"]);\n");
        data += fmt::sprintf("    const aWeightEffect = [-1, -1, -1];\n");
        data += fmt::sprintf("    aWeightEffect[0] = aLightSnowEffects ? aLightSnowEffects[0] : -1;\n");
        data += fmt::sprintf("    aWeightEffect[1] = aMediumSnowEffects ? aMediumSnowEffects[0] : -1;\n");
        data += fmt::sprintf("    aWeightEffect[2] = aHeavySnowEffects ? aHeavySnowEffects[0] : -1;\n");
        data += fmt::sprintf("    const aWeightChance = [10, 30, 60];\n");
        data += fmt::sprintf("    const weightAdjustment = maxWeight / 100;\n");
        data += fmt::sprintf("    const placeSnow = (x, y, percentChance, randomScale) => {\n");
        data += fmt::sprintf("        percentChance *= weightAdjustment;\n");
        data += fmt::sprintf("        if (!GameplayMap.isWater(x, y)) {\n");
        data += fmt::sprintf("            const snowRandomization = TerrainBuilder.getRandomNumber(randomScale * 2, \"Snow weight randomization\");\n");
        data += fmt::sprintf("            const snowWeight = percentChance + snowRandomization - randomScale;\n");
        data += fmt::sprintf("            if (snowWeight > 0) {\n");
        data += fmt::sprintf("                for (let weight = aWeightChance.length - 1; weight >= 0; --weight) {\n");
        data += fmt::sprintf("                    if (snowWeight > aWeightChance[weight]) {\n");
        data += fmt::sprintf("                        MapPlotEffects.addPlotEffect(GameplayMap.getIndexFromXY(x, y), aWeightEffect[weight]);\n");
        data += fmt::sprintf("                        break;\n");
        data += fmt::sprintf("                    }\n");
        data += fmt::sprintf("                }\n");
        data += fmt::sprintf("            }\n");
        data += fmt::sprintf("        }\n");
        data += fmt::sprintf("    };\n");
        data += fmt::sprintf("    if (topRows > 0) {\n");
        data += fmt::sprintf("        const endTopRow = height - topRows;\n");
        data += fmt::sprintf("        for (let row = height; row > endTopRow; --row) {\n");
        data += fmt::sprintf("            const snowPercent = (1 - (height - row) / topRows) * 100;\n");
        data += fmt::sprintf("            for (let col = 0; col < width; ++col) {\n");
        data += fmt::sprintf("                placeSnow(col, row, snowPercent, randomization);\n");
        data += fmt::sprintf("            }\n");
        data += fmt::sprintf("        }\n");
        data += fmt::sprintf("    }\n");
        data += fmt::sprintf("    if (bottomRows > 0) {\n");
        data += fmt::sprintf("        for (let row = 0; row < bottomRows; ++row) {\n");
        data += fmt::sprintf("            const snowPercent = (1 - row / bottomRows) * 100;\n");
        data += fmt::sprintf("            for (let col = 0; col < width; ++col) {\n");
        data += fmt::sprintf("                placeSnow(col, row, snowPercent, randomization);\n");
        data += fmt::sprintf("            }\n");
        data += fmt::sprintf("        }\n");
        data += fmt::sprintf("  }\n");
    }

    // Manually painted snow
    if (m_usePaintedSnow)
    {
        data += fmt::sprintf("    console.log(\"Begin Snow tiles\");\n");
        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                const Civ7Tile tile = m_civ7TerrainType.get(x, y);
                if ((int)tile.snow > 0 && tile.biome != BiomeType::Marine)
                {
                    data += fmt::sprintf("  placeSnow(%u, %u, %u, paintRandomization);\n", x, y, (int)tile.snow);
                }
            }
        }
        data += fmt::sprintf("  console.log(\"End Snow tiles\");\n");
    }

    data += fmt::sprintf("    console.log(\"End paintSnow\");\n");
    data += fmt::sprintf("}\n\n");

    // Natural wonders should appear only once
    int2 naturalWonderCoords[enumCount<NaturalWonderType>()];
    int naturalWonderCounter[enumCount<NaturalWonderType>()];

    for (uint i = 0; i < enumCount<NaturalWonderType>(); ++i)
    {
        naturalWonderCoords[i] = int2(-1, -1);
        naturalWonderCounter[i] = 0;
    }

    for (uint y = 0; y < m_height; ++y)
    {
        for (uint x = 0; x < m_width; ++x)
        {
            Civ7Tile tile = m_civ7TerrainType.get(x, y);
            if ((int)tile.naturalWonder > 0)
            {
                if (naturalWonderCounter[(int)tile.naturalWonder] == 0)
                {
                    naturalWonderCoords[(int)tile.naturalWonder] = int2(x, y);
                }
                else
                {
                    LOG_WARNING("Natural wonder \"%s\" at coords (%i,%i)is already present at coords (%i,%i)", asString(tile.naturalWonder).c_str(), x, y, naturalWonderCoords[(int)tile.naturalWonder].x, naturalWonderCoords[(int)tile.naturalWonder].y);
                }
                naturalWonderCounter[(int)tile.naturalWonder]++;
            }
        }
    }

    // Inspired by legacy wonder placement code
    data += fmt::sprintf("async function paintNaturalWonders(width, height) {\n");
    data += fmt::sprintf("  console.log(\"Begin paintNaturalWonders\");\n");
    data += fmt::sprintf("  let aPossibleWonders = [];\n");
    data += fmt::sprintf("  let iElevation\n");
    data += fmt::sprintf("  let featureParam;\n");
    data += fmt::sprintf("  let placementX ;\n");
    data += fmt::sprintf("  let placementY;\n");
    data += fmt::sprintf("  for (const nwDef of GameInfo.Feature_NaturalWonders) \n");
    data += fmt::sprintf("  {\n");
    data += fmt::sprintf("      aPossibleWonders.push(nwDef.$hash);\n");
    data += fmt::sprintf("  }\n");
    data += fmt::sprintf("  for (let iI = 0; iI < aPossibleWonders.length; iI++)\n");
    data += fmt::sprintf("  {\n");
    data += fmt::sprintf("      let eFeature = aPossibleWonders[iI];\n");
    data += fmt::sprintf("      let nwDef = GameInfo.Feature_NaturalWonders.lookup(eFeature);\n");
    data += fmt::sprintf("      if (nwDef != null)\n");
    data += fmt::sprintf("      {\n");
    data += fmt::sprintf("          console.log(\"Placing natural wonder \" + nwDef.FeatureType + \" ...\");\n");
    data += fmt::sprintf("          let possibleLocations = \"\"\n");
    data += fmt::sprintf("          let possibleLocationCount = 0;\n");
    data += fmt::sprintf("          for (let iY = height - 1; iY >= 0; iY--)\n");
    data += fmt::sprintf("          {\n");
    data += fmt::sprintf("              for (let iX = 0; iX < width; iX++)\n");
    data += fmt::sprintf("              {\n");
    data += fmt::sprintf("                  iElevation = GameplayMap.getElevation(iX, iY);\n");
    data += fmt::sprintf("                  featureParam = \n");
    data += fmt::sprintf("                  {\n");
    data += fmt::sprintf("                      Feature: nwDef.$hash,\n");
    data += fmt::sprintf("                      Direction : nwDef.Direction,\n");
    data += fmt::sprintf("                      Elevation : iElevation\n");
    data += fmt::sprintf("                  };\n");
    data += fmt::sprintf("                  if (TerrainBuilder.canHaveFeatureParam(iX, iY, featureParam))\n");
    data += fmt::sprintf("                  {\n");
    data += fmt::sprintf("                      if (possibleLocations.length > 0)\n");
    data += fmt::sprintf("                      {\n");
    data += fmt::sprintf("                          possibleLocations += \", \";\n");
    data += fmt::sprintf("                      }\n");
    data += fmt::sprintf("                      possibleLocations += \"(\" + String(iX).padStart(2, '0') + \", \" + String(iY).padStart(2, '0') + \")\";\n");
    data += fmt::sprintf("                      possibleLocationCount++;\n");
    data += fmt::sprintf("                  }\n");
    data += fmt::sprintf("              }\n");
    data += fmt::sprintf("              if (possibleLocations.length > 0)\n");
    data += fmt::sprintf("              {\n");
    data += fmt::sprintf("                  console.log(possibleLocations);\n");
    data += fmt::sprintf("                  possibleLocations = \"\";\n");
    data += fmt::sprintf("              }\n");
    data += fmt::sprintf("          }\n");
    data += fmt::sprintf("          if (possibleLocations.length > 0)\n");
    data += fmt::sprintf("          {\n");
    data += fmt::sprintf("              console.log(possibleLocations);\n");
    data += fmt::sprintf("          }\n");
    data += fmt::sprintf("          console.log(possibleLocationCount + \" possible location(s) found.\");\n");
    data += fmt::sprintf("          switch (nwDef.FeatureType)\n");
    data += fmt::sprintf("          {\n");

    for (uint i = 0; i < enumCount<NaturalWonderType>(); ++i)
    {
        if (naturalWonderCounter[i] > 0)
        {
            data += fmt::sprintf("              case \"%s\":\n", getNaturalWonderTypeAsString((NaturalWonderType)i));
            data += fmt::sprintf("              {\n");
            data += fmt::sprintf("                  placementX = %i;\n", naturalWonderCoords[i].x);
            data += fmt::sprintf("                  placementY = %i;\n", naturalWonderCoords[i].y);
            data += fmt::sprintf("                  iElevation = GameplayMap.getElevation(placementX, placementY);\n");
            data += fmt::sprintf("                  featureParam =\n");
            data += fmt::sprintf("                  {\n");
            data += fmt::sprintf("                      Feature: eFeature,\n");
            data += fmt::sprintf("                      Direction : nwDef.Direction,\n");
            data += fmt::sprintf("                      Elevation : iElevation\n");
            data += fmt::sprintf("                  };\n");
            data += fmt::sprintf("                  if (TerrainBuilder.canHaveFeatureParam(placementX, placementY, featureParam))\n");
            data += fmt::sprintf("                  {\n");
            data += fmt::sprintf("                      TerrainBuilder.setFeatureType(placementX, placementY, featureParam);\n");
            data += fmt::sprintf("                      console.log(nwDef.FeatureType + \" has been successfully placed at (\" + String(placementX).padStart(2, '0') + \", \" + String(placementY).padStart(2, '0') + \").\");\n");
            data += fmt::sprintf("                  }\n");
            data += fmt::sprintf("                  else\n");
            data += fmt::sprintf("                  {\n");
            data += fmt::sprintf("                      console.log(nwDef.FeatureType + \" could not be placed at (\" + String(placementX).padStart(2, '0') + \", \" + String(placementY).padStart(2, '0') + \").\");\n");
            data += fmt::sprintf("                  }\n");
            data += fmt::sprintf("              }\n");
            data += fmt::sprintf("              break;\n\n");
        }
    }
    data += fmt::sprintf("                      default:\n");
    data += fmt::sprintf("                          console.log(nwDef.FeatureType + \" had no custom location defined.\"); \n");
    data += fmt::sprintf("                      break;\n");
    data += fmt::sprintf("          }\n");
    data += fmt::sprintf("      }\n");
    data += fmt::sprintf("  }\n");
    data += fmt::sprintf("  console.log(\"End paintNaturalWonders\");\n");
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
bool DeleteMetadata(sqlite3 * db, const string & name)
{
    const char * sql = "DELETE FROM MetaData WHERE Name = ?;";
    sqlite3_stmt * stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        LOG_WARNING("Failed to prepare statement: %s", sqlite3_errmsg(db));
        return false; // <-- CRUCIAL: On ne ferme PAS 'db' ici !
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) 
    {
        return true;
    }
    else
    {
        LOG_WARNING("Delete metadata '%s' failed : %s", name.c_str(), sqlite3_errmsg(db));
        return false; 
    }
}

//--------------------------------------------------------------------------------------
bool ExportMetaData(sqlite3 * db, const string & name, const string & value)
{
    sqlite3_stmt * stmt;
    string sql = fmt::sprintf("INSERT OR REPLACE INTO MetaData (Name, Value) VALUES ('%s', ?);", name);
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        SQL(sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC));
        SQL(sqlite3_step(stmt));
        SQL(sqlite3_finalize(stmt));
        return true;
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql.c_str(), sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }
}

//--------------------------------------------------------------------------------------
bool ExportMetaData(sqlite3 * db, const string & name, bool value)
{
    sqlite3_stmt * stmt;
    string sql = fmt::sprintf("INSERT OR REPLACE INTO MetaData (Name, Value) VALUES ('%s', ?);", name);
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        SQL(sqlite3_bind_text(stmt, 1, value ? "1" : "0", -1, SQLITE_STATIC));
        SQL(sqlite3_step(stmt));
        SQL(sqlite3_finalize(stmt));
        return true;
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql.c_str(), sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }
}

//--------------------------------------------------------------------------------------
bool ExportMetaData(sqlite3 * db, const string & name, int value)
{
    sqlite3_stmt * stmt;
    string sql = fmt::sprintf("INSERT OR REPLACE INTO MetaData (Name, Value) VALUES ('%s', ?);", name);
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        string s = fmt::sprintf("%i", value);
        SQL(sqlite3_bind_text(stmt, 1, s.c_str(), -1, SQLITE_STATIC));
        SQL(sqlite3_step(stmt));
        SQL(sqlite3_finalize(stmt));
        return true;
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql.c_str(), sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }
}

//--------------------------------------------------------------------------------------
void Map::exportSQLiteMap()
{
    // If the file does not exist, copy from the empty template to make sure we've all the tables correct
    if (!FileExists(m_mapPath))
    {
        CopyFile("data/Civ7Map.sql", m_mapPath);
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

    sqlite3_busy_timeout(db, 2000);

    const char * sql = nullptr;

    // Map
    sql = "INSERT OR REPLACE INTO Map (ID, Width, Height, TopLatitude, BottomLatitude, WrapX, WrapY, MapSizeType) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt * stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        string mapSize = getExportMapSize().c_str();

        sqlite3_bind_text(stmt, 1, "Default", -1, SQLITE_STATIC);   // ID
        sqlite3_bind_int(stmt, 2, m_width);
        sqlite3_bind_int(stmt, 3, m_height);
        sqlite3_bind_int(stmt, 4, m_topLattitude);                  // TopLatitude
        sqlite3_bind_int(stmt, 5, m_bottomLattitude);               // BottomLatitude
        sqlite3_bind_int(stmt, 6, m_wrapX? 1 : 0);                  // Maps without WrapX 1 will not render minimap correctly :(
        sqlite3_bind_int(stmt, 7, m_wrapY? 1 : 0);                  // WrapY
        sqlite3_bind_text(stmt, 8, mapSize.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Delete deprecated metadata
    DeleteMetadata(db, "SnowBorderX2");
    DeleteMetadata(db, "TopLattitude2");
    DeleteMetadata(db, "BottomLattitude2");

    // Metadata
    ExportMetaData(db, "DisplayName", m_prettyName);  
    ExportMetaData(db, "Author", m_author);
    ExportMetaData(db, "Description", m_description);
    ExportMetaData(db, "RandomLakes", m_randomLakes);
    ExportMetaData(db, "RandomFeatures", m_randomFeatures);
    ExportMetaData(db, "RandomContinents", m_randomContinents);
    ExportMetaData(db, "RandomElevation", m_randomElevation);
    ExportMetaData(db, "RandomHills", m_randomHills);
    ExportMetaData(db, "RandomRainfall", m_randomRainfall);
    ExportMetaData(db, "RandomRivers", m_randomRivers);
    ExportMetaData(db, "RandomBiomes", m_randomBiomes);
    ExportMetaData(db, "RandomNaturalWonders", m_randomNaturalWonders);
    ExportMetaData(db, "RandomFloodPlains", m_randomFloodPlains);
    ExportMetaData(db, "RandomSnow", m_randomSnow);
    ExportMetaData(db, "RandomFloodPlains", m_randomResources);
    ExportMetaData(db, "TopLattitude", m_topLattitude);
    ExportMetaData(db, "BottomLattitude", m_bottomLattitude);
    ExportMetaData(db, "ForceCustomMapSize", m_forceCustomMapSize);
    ExportMetaData(db, "WrapX", m_wrapX);
    ExportMetaData(db, "WrapY", m_wrapY);
    ExportMetaData(db, "UseAdvancedSnow", m_useAdvancedSnow);
    ExportMetaData(db, "SnowBorderX", m_snowBorderX);
    ExportMetaData(db, "TopSnowRows", m_topSnowRows);
    ExportMetaData(db, "BottomSnowRows", m_bottomSnowRows);
    ExportMetaData(db, "MaxSnowWeight", m_maxSnowWeight);
    ExportMetaData(db, "SnowRandomization", m_snowRandomization);
    ExportMetaData(db, "UsePaintedSnow", m_usePaintedSnow);
    ExportMetaData(db, "PaintedSnowRandomization", m_paintedSnowRandomization);   
    ExportMetaData(db, "UseTSL", m_useTSL);
    ExportMetaData(db, "NoResourceOnTSL", m_noResourceOnTSL);
    ExportMetaData(db, "NoResourceOnNavigableRivers", m_noResourceOnNavigableRivers);
    ExportMetaData(db, "NoFeatureOnNavigableRivers", m_noFeatureOnNavigableRivers);
    ExportMetaData(db, "NoElevationOnNavigableRivers", m_noElevationOnNavigableRivers);
    ExportMetaData(db, "ExportPaintedRivers", m_exportPaintedRivers);
    ExportMetaData(db, "MinPlayers", m_minPlayers);  
    ExportMetaData(db, "MaxPlayers", m_maxPlayers);
    ExportMetaData(db, "DefaultPlayers", m_defaultPlayers);    
    
    // Plot data
    sql = "INSERT OR REPLACE INTO Plots (ID, TerrainType, BiomeType, ContinentType, Elevation, IsImpassable, Tag, LandmassRegionId) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
        sqlite3_exec(db, "DELETE FROM Plots;", NULL, 0, NULL);

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
                sqlite3_bind_int(stmt, 5, (int)tile.elevation);                         // Elevation
                sqlite3_bind_int(stmt, 6, tile.impassable ? 1 : 0);                     // IsImpassable 
                sqlite3_bind_text(stmt, 7, "", -1, SQLITE_STATIC);                      // Tag (unused?)
                sqlite3_bind_int(stmt, 8, (int)tile.landmass);                          // LandmassRegionId

                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }
        }

        // Commit
        sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotFeatures  
    sql = "INSERT OR REPLACE INTO PlotFeatures (ID, FeatureType) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
        sqlite3_exec(db, "DELETE FROM PlotFeatures;", NULL, 0, NULL);

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

        // Commit 
        sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // PlotResources  
    sql = "INSERT OR REPLACE INTO PlotResources (ID, ResourceType, ResourceCount) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
        sqlite3_exec(db, "DELETE FROM PlotResources;", NULL, 0, NULL);

        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                Civ7Tile tile = m_civ7TerrainType.get(x, y);

                if ((int)tile.resource > 0)
                {
                    string resourceTypeNameS = getResourceTypeAsString(tile.resource);

                    sqlite3_bind_int(stmt, 1, x + y * m_width);                                 // ID
                    sqlite3_bind_text(stmt, 2, resourceTypeNameS.c_str(), -1, SQLITE_STATIC);   // ResourceType
                    sqlite3_bind_int(stmt, 3, 1);                                               // ResourceCount

                    sqlite3_step(stmt);
                    sqlite3_reset(stmt);
                }
            }
        }

        // Commit 
        sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // StartPositions
    sql = "INSERT OR REPLACE INTO StartPositions (Plot, Type, Value) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK)
    {
        // Add transaction for massive speed improvement
        rc = sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
        rc = sqlite3_exec(db, "DELETE FROM StartPositions;", NULL, 0, NULL);

        for (uint c = 0; c < m_civilizations.size(); ++c)
        {
            const Civilization & civ = m_civilizations[c];
            for (uint t = 0; t < civ.tsl.size(); ++t)
            {
                const TSL & tsl = civ.tsl[t];
                int plotIndex = getPlotIndex(tsl.pos);

                rc = sqlite3_bind_int(stmt, 1, plotIndex);                                          // Plot
                rc = sqlite3_bind_text(stmt, 2, "", -1, SQLITE_STATIC);                             // Type
                rc = sqlite3_bind_text(stmt, 3, civ.civilizationName.c_str(), -1, SQLITE_STATIC);   // Value

                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }
        }

        // Commit 
        rc = sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
        rc = sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Natural wonders
    {
        const char * createTableSQL =
            "CREATE TABLE IF NOT EXISTS PlotNaturalWonders ("
            "ID INTEGER PRIMARY KEY,"
            "Value TEXT"
            ");";

        rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)
        {
            LOG_ERROR("Failed to create natural wonders table: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sql = "INSERT OR REPLACE INTO PlotNaturalWonders (ID, Value) VALUES (?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc == SQLITE_OK)
        {
            // Add transaction for massive speed improvement
            rc = sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
            rc = sqlite3_exec(db, "DELETE FROM PlotNaturalWonders;", NULL, 0, NULL);

            for (uint y = 0; y < m_height; ++y)
            {
                for (uint x = 0; x < m_width; ++x)
                {
                    Civ7Tile tile = m_civ7TerrainType.get(x, y);

                    if ((int)tile.naturalWonder > 0)
                    {
                        string naturalWonderTypeNameS = getNaturalWonderTypeAsString(tile.naturalWonder);

                        sqlite3_bind_int(stmt, 1, x + y * m_width);                                     // ID
                        sqlite3_bind_text(stmt, 2, naturalWonderTypeNameS.c_str(), -1, SQLITE_STATIC);  // Value

                        sqlite3_step(stmt);
                        sqlite3_reset(stmt);
                    }
                }
            }

            // Commit 
            rc = sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
            rc = sqlite3_finalize(stmt);
        }
        else
        {
            LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
    }

    // custom rivers
    {
        const char * createTableSQL =
            "CREATE TABLE IF NOT EXISTS Rivers ("
            "ID INTEGER PRIMARY KEY,"
            "Name TEXT,"
            "PrettyName TEXT"
            ");";

        rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)
        {
            LOG_ERROR("Failed to create custom Rivers table: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sql = "INSERT OR REPLACE INTO Rivers (ID, Name, PrettyName) VALUES (?, ?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc == SQLITE_OK)
        {
            // Add transaction for massive speed improvement
            rc = sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
            rc = sqlite3_exec(db, "DELETE FROM Rivers;", NULL, 0, NULL);

            for (uint i = 0; i < m_rivers.size(); ++i)
            {
                const RiverInfo & river = m_rivers[i];
                string riverName = river.name;
                string riverPrettyName = river.prettyName;

                sqlite3_bind_int(stmt, 1, i);                                           // ID
                sqlite3_bind_text(stmt, 2, riverName.c_str(), -1, SQLITE_STATIC);       // Name
                sqlite3_bind_text(stmt, 3, riverPrettyName.c_str(), -1, SQLITE_STATIC); // PrettyName

                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }

            // Commit 
            rc = sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
            rc = sqlite3_finalize(stmt);
        }
        else
        {
            LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
    }

    // River tiles
    {
        const char * createTableSQL =
            "CREATE TABLE IF NOT EXISTS RiverTiles ("
            "ID INTEGER KEY,"
            "NameIndex INTEGER,"
            "Type TEXT,"
            "Direction TEXT"
            ");";

        rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)
        {
            LOG_ERROR("Failed to create RiverTiles table: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sql = "INSERT OR REPLACE INTO RiverTiles (ID, NameIndex, Type, Direction) VALUES (?, ?, ?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc == SQLITE_OK)
        {
            // Add transaction for massive speed improvement
            rc = sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
            rc = sqlite3_exec(db, "DELETE FROM RiverTiles;", NULL, 0, NULL);

            for (uint y = 0; y < m_height; ++y)
            {
                for (uint x = 0; x < m_width; ++x)
                {
                    Civ7Tile tile = m_civ7TerrainType.get(x, y);

                    for (uint i = 0; i < MAX_RIVER_PER_TILE; ++i)
                    {
                        RiverTileInfo & riverTile = tile.rivers[i];

                        if (riverTile.nameIndex != RiverName::None)
                        {
                            string riverTypeString = getRiverTypeAsString(riverTile.type);
                            string riverDirString = getDirectionTypeAsString(riverTile.direction);

                            sqlite3_bind_int(stmt, 1, x + y * m_width);                 // ID
                            sqlite3_bind_int(stmt, 2, (int)riverTile.nameIndex);
                            sqlite3_bind_text(stmt, 3, riverTypeString.c_str(), -1, SQLITE_STATIC); 
                            sqlite3_bind_text(stmt, 4, riverDirString.c_str(), -1, SQLITE_STATIC); 
                            sqlite3_step(stmt);
                            sqlite3_reset(stmt);
                        }
                    }
                }
            }

            // Commit 
            rc = sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
            rc = sqlite3_finalize(stmt);
        }
        else
        {
            LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
    }

    // SnowAmount
    {
        const char * createTableSQL =
            "CREATE TABLE IF NOT EXISTS PlotSnowAmount ("
            "ID INTEGER PRIMARY KEY,"
            "Value INTEGER"
            ");";

        rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK)
        {
            LOG_ERROR("Failed to create PlotSnowAmount table: %s", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sql = "INSERT OR REPLACE INTO PlotSnowAmount (ID, Value) VALUES (?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc == SQLITE_OK)
        {
            // Add transaction for massive speed improvement
            rc = sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, 0, NULL);
            rc = sqlite3_exec(db, "DELETE FROM PlotSnowAmount;", NULL, 0, NULL);

            for (uint y = 0; y < m_height; ++y)
            {
                for (uint x = 0; x < m_width; ++x)
                {
                    Civ7Tile tile = m_civ7TerrainType.get(x, y);

                    if ((int)tile.snow > 0)
                    {
                        string naturalWonderTypeNameS = getNaturalWonderTypeAsString(tile.naturalWonder);

                        sqlite3_bind_int(stmt, 1, x + y * m_width); // ID
                        sqlite3_bind_int(stmt, 2, (int)tile.snow);  // SnowAmount

                        sqlite3_step(stmt);
                        sqlite3_reset(stmt);
                    }
                }
            }

            // Commit 
            rc = sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
            rc = sqlite3_finalize(stmt);
        }
        else
        {
            LOG_ERROR("Query \"%s\" failed : %s", sql, sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
    }

    // Close the database connection
    sqlite3_close(db);
}