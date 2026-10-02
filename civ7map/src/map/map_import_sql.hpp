//--------------------------------------------------------------------------------------
bool Map::importSQLiteMap(const string & _cwd)
{
    // Try to open the SQLite database
    sqlite3 * db;
    int rc = sqlite3_open(m_mapPath.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        LOG_ERROR("Cannot open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Get map size from the database
    const char * sql = "SELECT width, height FROM Map LIMIT 1;";
    sqlite3_stmt * stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            uint mapWidth = sqlite3_column_int(stmt, 0);
            uint mapHeight = sqlite3_column_int(stmt, 1);

            // Set up the tile array
            m_width = mapWidth;
            m_height = mapHeight;
            m_mapSize = getMapSize(mapWidth, mapHeight);
            m_editMapSize[0] = m_width;
            m_editMapSize[1] = m_height;

            LOG_INFO("Map size is %ux%u", mapWidth, mapHeight);
            m_civ7TerrainType.SetSize(m_width, m_height);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Failed to prepare SQL statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Get map name from database
    //sql = "SELECT Value FROM MetaData WHERE Name = 'DisplayName' LIMIT 1;";
    //rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    //if (rc == SQLITE_OK)
    //{
    //    rc = sqlite3_step(stmt);
    //    if (rc == SQLITE_ROW)
    //    {
    //        m_prettyName = std::string((const char *)sqlite3_column_text(stmt, 0));
    //        LOG_INFO("Map name is \"%s\"", m_prettyName.c_str());
    //    }
    //    sqlite3_finalize(stmt);
    //}
    m_prettyName = getBaseName();

    // Read tile data 
    sql = "SELECT ID, TerrainType, BiomeType, ContinentType, Elevation, IsImpassable, Tag, LandmassRegionId FROM Plots";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int ID = sqlite3_column_int(stmt, 0);

            int x = ID % m_width;
            int y = ID / m_width;

            Civ7Tile tile;

            string terrainTypeS = (const char *)sqlite3_column_text(stmt, 1);
            tile.terrain = getTerrainTypeFromString(terrainTypeS, x, y);

            string biomeTypeS = (const char *)sqlite3_column_text(stmt, 2);
            tile.biome = getBiomeTypeFromString(biomeTypeS, x, y);

            string continentS = (const char *)sqlite3_column_text(stmt, 3);
            tile.continent = getOrCreateContinentType(continentS);

            // We can read/write elevation from SQL but the engine seems to only use elevation set from script
            // c.f.   paintEarthHugeElevation();
            tile.elevation = sqlite3_column_int(stmt, 4);

            tile.landmass = getOrCreateLandmassType(sqlite3_column_int(stmt, 7));

            m_civ7TerrainType.set(x, y, tile);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Failed to prepare SQL statement for tiles: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Read plot features
    sql = "SELECT ID, FeatureType FROM PlotFeatures";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int ID = sqlite3_column_int(stmt, 0);

            int x = ID % m_width;
            int y = ID / m_width;

            Civ7Tile tile = m_civ7TerrainType.get(x, y);

            string plotFeaturesS = (const char *)sqlite3_column_text(stmt, 1);
            tile.feature = getFeatureFromString(plotFeaturesS, x, y);

            m_civ7TerrainType.set(x, y, tile);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Failed to prepare SQL statement for tiles: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Read plot resources
    sql = "SELECT ID, ResourceType, ResourceCount FROM PlotResources";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK)
    {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int ID = sqlite3_column_int(stmt, 0);

            int x = ID % m_width;
            int y = ID / m_width;

            Civ7Tile tile = m_civ7TerrainType.get(x, y);

            string plotResourceS = (const char *)sqlite3_column_text(stmt, 1);
            tile.resource = getResourceFromString(plotResourceS, x, y);

            m_civ7TerrainType.set(x, y, tile);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        LOG_ERROR("Failed to prepare SQL statement for tiles: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Close the database connection
    sqlite3_close(db);

    m_isLoaded = true;
    return true;
}