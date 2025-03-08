//--------------------------------------------------------------------------------------
bool Map::create(const string & _cwd, const string & _name, int _width, int _height)
{
    LOG_WARNING("Create map \"%s\" (%ux%u)", _name.c_str(), _width, _height);

    m_mapPath = fmt::sprintf("%s\\mods\\%s\\maps\\%s-map.js", _cwd,_name, _name);
    m_mapDataPath = Map::GetMapDataPathFromMapPath(m_mapPath);
    m_modFolder = fmt::sprintf("%s\\mods\\%s", _cwd, _name);

    bool created;

    created = CreateFolder(m_modFolder);
    if (created)
        LOG_WARNING("Created mod folder \"%s\"", m_modFolder.c_str());

    string mapFolder = fmt::sprintf("%s\\maps", m_modFolder);
    created = CreateFolder(mapFolder);
    if (created)
        LOG_WARNING("Created map folder \"%s\"", mapFolder.c_str());

    string configFolder = fmt::sprintf("%s\\config", m_modFolder);
    created = CreateFolder(configFolder);
    if (created)
        LOG_WARNING("Created config folder \"%s\"", configFolder.c_str());

    string textFolder = fmt::sprintf("%s\\text", m_modFolder);
    created = CreateFolder(textFolder);
    if (created)
        LOG_WARNING("Created text folder \"%s\"", textFolder.c_str());

    string textFolder_en_us = fmt::sprintf("%s\\text\\en_us", m_modFolder);
    created = CreateFolder(textFolder_en_us);
    if (created)
        LOG_WARNING("Created text (en_us) folder \"%s\"", textFolder_en_us.c_str());

    string iconsFolder = fmt::sprintf("%s\\icons", m_modFolder);
    created = CreateFolder(iconsFolder);
    if (created)
        LOG_WARNING("Created icon folder \"%s\"", iconsFolder.c_str());

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

    // Make 4 islands boy

    // Tundra island
    for (uint j = 0+2; j < m_height/2-2; ++j)
    {
        for (uint i = 0 + 2; i < m_width/2-2; ++i)
        {
            Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            tile.terrain = TerrainType::Flat;
            tile.biome = BiomeType::Tundra;
            tile.continent = ContinentType::None;
        }
    }

    // Grassland hills island
    for (uint j = 0 + 2; j < m_height / 2 - 2; ++j)
    {
        for (uint i = m_width / 2 + 2; i < m_width - 2; ++i)
        {
            Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            tile.terrain = TerrainType::Hill;
            tile.biome = BiomeType::Grassland;
            tile.continent = ContinentType::None;
        }
    }

    // Desert island
    for (uint j = m_height / 2 + 2; j < m_height - 2; ++j)
    {
        for (uint i = 0 + 2; i < m_width / 2 - 2; ++i)
        {
            Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            tile.terrain = TerrainType::Flat;
            tile.biome = BiomeType::Desert;
            tile.continent = ContinentType::None;
        }
    }

    // Grassland flag island
    for (uint j = m_height / 2 + 2; j < m_height - 2; ++j)
    {
        for (uint i = m_width / 2 + 2; i < m_width - 2; ++i)
        {
            Civ7Tile & tile = m_civ7TerrainType.get(i, j);

            tile.terrain = TerrainType::Flat;
            tile.biome = BiomeType::Grassland;
            tile.continent = ContinentType::None;
        }
    }

    refresh();
    resetCamera();

    // Create all files
    exportFiles(_cwd);

    return true;
}