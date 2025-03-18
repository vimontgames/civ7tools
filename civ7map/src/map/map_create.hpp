//--------------------------------------------------------------------------------------
bool Map::createFolders()
{
    extern string g_currentWorkingDirectory;
    string _cwd = g_currentWorkingDirectory;

    bool created = false;

    string modsFolder = fmt::sprintf("%s\\mods", _cwd);
    created |= CreateFolder(modsFolder);
    if (created)
        LOG_WARNING("Created mods folder \"%s\"", modsFolder.c_str());

    created |= CreateFolder(m_modFolder);
    if (created)
        LOG_WARNING("Created mod folder \"%s\"", m_modFolder.c_str());

    string mapFolder = fmt::sprintf("%s\\maps", m_modFolder);
    created |= CreateFolder(mapFolder);
    if (created)
        LOG_WARNING("Created map folder \"%s\"", mapFolder.c_str());

    string configFolder = fmt::sprintf("%s\\config", m_modFolder);
    created |= CreateFolder(configFolder);
    if (created)
        LOG_WARNING("Created config folder \"%s\"", configFolder.c_str());

    string textFolder = fmt::sprintf("%s\\text", m_modFolder);
    created |= CreateFolder(textFolder);
    if (created)
        LOG_WARNING("Created text folder \"%s\"", textFolder.c_str());

    string textFolder_en_us = fmt::sprintf("%s\\text\\en_us", m_modFolder);
    created |= CreateFolder(textFolder_en_us);
    if (created)
        LOG_WARNING("Created text (en_us) folder \"%s\"", textFolder_en_us.c_str());

    string dataFolder = fmt::sprintf("%s\\data", m_modFolder.c_str());
    created |= CreateFolder(dataFolder);
    if (created)
        LOG_WARNING("Created data folder \"%s\"", dataFolder.c_str());

    string iconsFolder = fmt::sprintf("%s\\icons", m_modFolder);
    created |= CreateFolder(iconsFolder);
    if (created)
        LOG_WARNING("Created icon folder \"%s\"", iconsFolder.c_str());

    return created;
}

//--------------------------------------------------------------------------------------
bool Map::create(const string & _cwd, const string & _name, int _width, int _height)
{
    LOG_WARNING("Create map \"%s\" (%ux%u)", _name.c_str(), _width, _height);

    m_mapPath = fmt::sprintf("%s\\mods\\%s\\maps\\%s-map.js", _cwd,_name, _name);
    m_mapDataPath = Map::GetMapDataPathFromMapPath(m_mapPath);
    m_modFolder = fmt::sprintf("%s\\mods\\%s", _cwd, _name);

    // TODO: let user specify pretty name? Default to base file name for now.
    m_prettyName = getBaseName();

    createFolders();

    m_width = _width;
    m_height = _height;
    m_mapSize = getMapSize(_width, _height);

    m_isLoaded = true;

    m_civ7TerrainType.SetSize(m_width, m_height);

    // Clear the map to ocean
    for (uint j = 0; j < m_height; ++j)
    {
        for (uint i = 0; i < m_width; ++i)
        {
            Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            tile.terrain = TerrainType::Ocean;
            tile.biome = BiomeType::Marine;
            tile.continent = ContinentType::None;
        }       
    }

    // Default west & east islands

    // west
    {
        uint startX = 1, endX = m_width / 2 - 1;
        uint startY = 1, endY = m_height - 1;

        m_westStart = startX;
        m_westEnd = endX;

        for (uint j = startY; j < endY; ++j)
        {
            for (uint i = startX; i < endX; ++i)
            {
                Civ7Tile & tile = m_civ7TerrainType.get(i, j);

                if (i == startX || i == endX - 1 || j == startY || j == endY - 1)
                {
                    tile.terrain = TerrainType::Coast;
                    tile.biome = BiomeType::Marine;
                    tile.continent = ContinentType::None;
                }
                else
                {
                    tile.terrain = TerrainType::Flat;
                    tile.biome = BiomeType::Grassland;
                    tile.continent = ContinentType::None;
                }
            }
        }
    }

    // east
    {
        uint startX = m_width/2+1, endX = m_width-1;
        uint startY = 1, endY = m_height - 1;

        m_eastStart = startX;
        m_eastEnd = endX;

        for (uint j = startY; j < endY; ++j)
        {
            for (uint i = startX; i < endX; ++i)
            {
                Civ7Tile & tile = m_civ7TerrainType.get(i, j);

                if (i == startX || i == endX - 1 || j == startY || j == endY - 1)
                {
                    tile.terrain = TerrainType::Coast;
                    tile.biome = BiomeType::Marine;
                    tile.continent = ContinentType::None;
                }
                else
                {
                    tile.terrain = TerrainType::Flat;
                    tile.biome = BiomeType::Grassland;
                    tile.continent = ContinentType::None;
                }
            }
        }
    }

    refresh();
    resetCamera();

    // Create all files
    exportFiles(_cwd, true);

    return true;
}