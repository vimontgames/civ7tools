
//--------------------------------------------------------------------------------------
u32 * Map::loadTexture(XMLElement * _xmlTerrainSave, const string & _name)
{
    XMLElement * xmlBytes = _xmlTerrainSave->FirstChildElement((_name + ".Bytes").c_str());
    u32 elevationLength = 0;
    XMLError xmlErr = xmlBytes->QueryUnsignedAttribute("Length", &elevationLength);

    string base64encodedData = xmlBytes->FirstChild()->ToText()->Value();
    base64::decode_inplace(base64encodedData);
    vector<ubyte> temp;
    temp.resize(elevationLength);
    for (u32 i = 0; i < elevationLength; ++i)
        temp[i] = base64encodedData[i];

    int x, y, comp;
    u32 * data = (u32*)stbi_load_from_memory(temp.data(), (int)temp.size(), &x, &y, &comp, 0);
    return data;
}

//--------------------------------------------------------------------------------------
template <typename T> void Map::loadBitmap(Array2D<T> & _array, tinyxml2::XMLElement * _xmlTerrainSave, const string & _name, u32 _width, u32 _height)
{
    u32 * bitmap = loadTexture(_xmlTerrainSave, _name.c_str());
    _array = Array2D<T>(_width, _height, bitmap);
    SAFE_FREE(bitmap);
}

//--------------------------------------------------------------------------------------
bool GetValue(const string & data, const string label, u32 & value)
{
    const string token = label + " =";
    auto pos = data.find(token);
    if (-1 != pos)
    {
        string numberStr = data.substr(pos + token.length() + 1);
        numberStr = numberStr.substr(0, numberStr.find_first_of(";")); 

        if (isDigits(numberStr))
        {
            value = static_cast<uint32_t>(stoi(numberStr));
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool GetNextValue(string & input, size_t & pos, string & value) 
{
    value.clear();

    // Skip whitespace and closing brackets
    while (pos < input.size() && (isspace(input[pos]) || input[pos] == '[' || input[pos] == ']') || input[pos] == '=' || input[pos] == ',' || input[pos] == ',')
    {
        ++pos;
    }

if (pos >= input.size())
{
    return false; // End of string
}

// Extract an identifier (letters, digits, underscores)
if (isalpha(input[pos]) || input[pos] == '_')
{
    while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_'))
    {
        value += input[pos++];
    }
    return true;
}

// Extract a number (including negative numbers)
if (isdigit(input[pos]) || (input[pos] == '-' && pos + 1 < input.size() && isdigit(input[pos + 1])))
{
    if (input[pos] == '-')
        value += input[pos++]; // Include negative sign if present

    while (pos < input.size() && isdigit(input[pos]))
    {
        value += input[pos++];
    }
    return true;
}

// Extract individual symbols (e.g., `{`, `}`, `[`, `]`, `,`, `=`)
if (string("{}[]=,").find(input[pos]) != string::npos) {
    value = input[pos++];
    return true;
}

// If we reach here, move to the next character and retry
++pos;
return GetNextValue(input, pos, value);
}

//--------------------------------------------------------------------------------------
bool Map::importMapSize(const string & data, int & mapWidth, int & mapHeight) const
{
    const string label = "MapToConvert";
    const string token = label + (string)"[";
    const size_t firstToken = data.find(token);

    size_t posLine = firstToken;
    size_t offset = 0;

    mapWidth = mapHeight = -1;

    while (posLine != -1)
    {
        size_t eol = data.find_first_of("\n", posLine + 1);
        string line = data.substr(posLine + token.length(), eol - posLine - token.length());

        // first digit is 'i'
        size_t curPos = 0;

        string iS, jS;
        int i, j;

        if (GetNextValue(line, curPos, iS) && isDigits(iS))
        {
            if (GetNextValue(line, curPos, jS) && isDigits(jS))
            {
                i = stoi(iS);
                j = stoi(jS);

                mapWidth = max(mapWidth, i + 1);
                mapHeight = max(mapHeight, j + 1);
            }
        }

        offset = posLine + line.length();
        posLine = data.find(token, offset);
    }

    if (-1 != mapWidth && -1 != mapHeight)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------------------------------
bool Map::importPrettyName(const string & data)
{
    //default
    m_prettyName = getBaseName();

    string token = "const mapName";
    auto pos = data.find(token);
    if (-1 != pos)
    {
        auto begin = data.find("'", pos + token.length());
        if (-1 != begin)
        {
            auto end = data.find("'", begin + 1);
            {
                string mapName = data.substr(begin + 1, end - begin - 1);
                LOG_INFO("Map pretty name is \"%s\"", mapName.c_str());
                m_prettyName = mapName;
                return true;
            }
        }
    }

    LOG_ERROR("Could not find map name");
    return false;
}

//--------------------------------------------------------------------------------------
int importWestEastValue(const string & data, const string & token)
{
    auto westStart = data.find(token);
    if (-1 != westStart)
    {
        auto eol = data.find_first_of("\n", westStart + 1);
        string value = data.substr(westStart + token.length(), eol - westStart - token.length());
        return atoi(value.c_str());
    }
    return -1;
}

//--------------------------------------------------------------------------------------
bool Map::importHemispheres(const string & data)
{
    m_westStart = importWestEastValue(data, "westStart:");
    m_westEnd = importWestEastValue(data, "westEnd:");

    m_eastStart = importWestEastValue(data, "eastStart:");
    m_eastEnd = importWestEastValue(data, "eastEnd:");

    LOG_INFO("West: %i..%i", m_westStart, m_westEnd);
    LOG_INFO("East: %i..%i", m_eastStart, m_eastEnd);

    return m_westStart != -1 && m_westEnd != -1 && m_eastStart != -1 && m_eastEnd != -1;
}

//--------------------------------------------------------------------------------------
bool Map::fixHemispheres()
{
    m_westStart = clamp(m_westStart, 0, m_width - 4);
    m_westEnd = clamp(m_westEnd, 1, m_width - 3);
    m_eastStart = clamp(m_eastStart, 0, m_width - 2);
    m_eastEnd = clamp(m_eastEnd, 1, m_width - 1);

    m_westStart = clamp(m_westStart, 0, m_width - 1);
    m_westEnd = clamp(m_westEnd, m_westStart+1, m_width - 1);
    m_eastStart = clamp(m_eastStart, m_westEnd+1, m_width - 1);
    m_eastEnd = clamp(m_eastEnd, m_eastStart+1, m_width - 1);

    if (m_westStart != -1 && m_westEnd != -1 && m_eastStart != -1 && m_eastEnd != -1)
        return false;

    LOG_WARNING("Invalid hemispheres (West: %i..%i, East:%i..%i) have been fixed", m_westStart, m_westEnd, m_eastStart, m_eastEnd);

    int buffer = 1;
    m_westStart = buffer;
    m_westEnd = m_width / 2 - buffer;

    m_eastStart = m_width / 2 + buffer;
    m_eastEnd = m_width - buffer;

    return true;
}

//--------------------------------------------------------------------------------------
bool Map::importYnAMP(const string & data)
{
    const string label = "MapToConvert";
    const string token = label + (string)"[";
    const size_t firstToken = data.find(token);

    size_t posLine = firstToken;
    size_t offset = 0;

    if (-1 == posLine)
    {
        // This could be using data from an external file, e.g. new version of "greatest-earth-map.js" imports map data from "greatest-earth-data.js"
        const string importGetMapToken = "import { GetMap } from '";
        size_t importGetMap = data.find(importGetMapToken);
        if (-1 != importGetMap)
        {
            size_t endImportGetMap = data.find("'", importGetMap + importGetMapToken.length() + 1);
            m_mapDataPath = data.substr(importGetMap + importGetMapToken.length(), endImportGetMap - importGetMap - importGetMapToken.length());

            // Expect the data file to be in the same folder as the original file ...
            m_mapDataPath = GetFolder(m_mapPath) + "\\" + GetFilename(m_mapDataPath);

            LOG_INFO("File \"%s\" imports GetMap from \"%s\"", GetFilename(m_mapPath).c_str(), m_mapDataPath.c_str());

            string getmapData;
            if (ReadFile(m_mapDataPath, getmapData))
            {
                return importYnAMP(getmapData);
            }
        }

        LOG_ERROR("Could not find \"%s\" in file \"%s\"", label.c_str(), GetFilename(m_mapPath).c_str());
        return false;
    }

    // First pass to get map size
    int mapWidth, mapHeight;

    if (importMapSize(data, mapWidth, mapHeight))
    {
        // save map size
        m_width = mapWidth;
        m_height = mapHeight;
        m_mapSize = getMapSize(mapWidth, mapHeight);
        m_editMapSize[0] = m_width;
        m_editMapSize[1] = m_height;

        LOG_INFO("Map size is %ux%u", mapWidth, mapHeight);
        m_civ7TerrainType.SetSize(mapWidth, mapHeight);
    }
    else
    {
        LOG_ERROR("Could not get map size from file \"%s\"", GetFilename(m_mapPath).c_str());
        return false;
    }

    // Clear map (in case of holes in data)
    for (u32 j = 0; j < m_civ7TerrainType.Height(); ++j)
    {
        for (u32 i = 0; i < m_civ7TerrainType.Width(); ++i)
        {
            Civ7Tile tile = {};
            m_civ7TerrainType.set(i, j, tile);
        }
    }

    vector<string> continentNames;

    MapFormat mapFmt = MapFormat::Unknown;
    
    // Some maps have data in reverse order so we need to parse [i][j]
    while (posLine != -1)
    {
        size_t eol = data.find_first_of("\n", posLine+1);
        string line = data.substr(posLine + token.length(), eol- posLine - token.length());

        // first digit is 'i'
        size_t curPos = 0;

        string iS, jS;
        int i, j;

        if (GetNextValue(line, curPos, iS) && isDigits(iS))
        {
            if (GetNextValue(line, curPos, jS) && isDigits(jS))
            {
                i = stoi(iS);
                j = stoi(jS);

                // Now we have correct i and j let's assume the file format is OK

                // lets check table format if unknown
                if (mapFmt == MapFormat::Unknown)
                {
                    int paramCount = 0;
                    string dummyS;
                    auto dummyPos = curPos;
                    while (GetNextValue(line, dummyPos, dummyS))
                        paramCount++;

                    if (paramCount == 14 || paramCount == 15) // "greatest-earth-map" has regular 14 but "giant-earth-map" has extra "-1"
                    {
                        mapFmt = MapFormat::Civ6;
                        LOG_WARNING("Imported file uses the %s map map format (%u parameters)", asString(mapFmt).c_str(), paramCount);
                    }
                    else
                    {
                        mapFmt = MapFormat::Civ7;
                        LOG_INFO("Imported file uses the %s map map format (%u parameters)", asString(mapFmt).c_str(), paramCount);
                    }                    
                }

                if (mapFmt == MapFormat::Civ6)
                {
                    // MapToConvert[x][y] = {civ6TerrainType, civ6FeatureTypes, civ6ContinentType, {{IsNEOfRiver, flow}, {IsWOfRiver, flow}, {IsNWOfRiver, flow}}, {Civ6ResourceType, num}, {IsNEOfCliff, IsWOfCliff, IsNWOfCliff} }

                    Civ6Tile civ6Tile;

                    string terrainTypeS;
                    if (GetNextValue(line, curPos, terrainTypeS))
                    {
                        if (isNumber(terrainTypeS))
                        {
                            civ6Tile.terrain = (Civ6Terrain)stoi(terrainTypeS);
                        }
                        else
                        {
                            // https://civilization.fandom.com/wiki/Terrain_(Civ7)#Nomenclature

                            if (terrainTypeS == "TERRAIN_COAST")
                                civ6Tile.terrain = Civ6Terrain::Coast;
                            else if (terrainTypeS == "TERRAIN_OCEAN")
                                civ6Tile.terrain = Civ6Terrain::Ocean;
                            else if (terrainTypeS == "TERRAIN_GRASS")
                                civ6Tile.terrain = Civ6Terrain::Grass;
                            else if (terrainTypeS == "TERRAIN_GRASS_HILLS")
                                civ6Tile.terrain = Civ6Terrain::Grass_Hills;
                            else if (terrainTypeS == "TERRAIN_GRASS_MOUNTAIN")
                                civ6Tile.terrain = Civ6Terrain::Grass_Moutain;
                            else if (terrainTypeS == "TERRAIN_PLAINS")
                                civ6Tile.terrain = Civ6Terrain::Plains;
                            else if (terrainTypeS == "TERRAIN_PLAINS_HILLS")
                                civ6Tile.terrain = Civ6Terrain::Plains_Hills;
                            else if (terrainTypeS == "TERRAIN_PLAINS_MOUNTAIN")
                                civ6Tile.terrain = Civ6Terrain::Plains_Moutain;
                            else if (terrainTypeS == "TERRAIN_TUNDRA")
                                civ6Tile.terrain = Civ6Terrain::Tundra;
                            else if (terrainTypeS == "TERRAIN_TUNDRA_HILLS")
                                civ6Tile.terrain = Civ6Terrain::Tundra_Hills;
                            else if (terrainTypeS == "TERRAIN_TUNDRA_MOUNTAIN")
                                civ6Tile.terrain = Civ6Terrain::Tundra_Moutain;
                            else if (terrainTypeS == "TERRAIN_DESERT")
                                civ6Tile.terrain = Civ6Terrain::Desert;
                            else if (terrainTypeS == "TERRAIN_DESERT_HILLS")
                                civ6Tile.terrain = Civ6Terrain::Desert_Hills;
                            else if (terrainTypeS == "TERRAIN_DESERT_MOUNTAIN")
                                civ6Tile.terrain = Civ6Terrain::Desert_Moutain;
                            else if (terrainTypeS == "TERRAIN_SNOW")
                                civ6Tile.terrain = Civ6Terrain::Snow;
                            else if (terrainTypeS == "TERRAIN_SNOW_HILLS")
                                civ6Tile.terrain = Civ6Terrain::Snow_Hills;
                            else if (terrainTypeS == "TERRAIN_SNOW_MOUNTAIN")
                                civ6Tile.terrain = Civ6Terrain::Snow_Moutain;
                            else
                            {
                                LOG_WARNING("Unknown civ6Tile.terrain \"%s\" at (%u,%u)", terrainTypeS.c_str(), i, j);
                                civ6Tile.terrain = Civ6Terrain::Tundra;
                            }
                        }
                    }

                    string featureTypeS;
                    if (GetNextValue(line, curPos, featureTypeS))
                    {
                        if (isNumber(featureTypeS))
                        {
                            civ6Tile.feature = (Civ6Feature)stoi(featureTypeS);
                        }
                        else
                        {
                            if (featureTypeS == "FEATURE_FLOODPLAINS")
                                civ6Tile.feature = Civ6Feature::FloodPlains;
                            else if (featureTypeS == "FEATURE_ICE")
                                civ6Tile.feature = Civ6Feature::Ice;
                            else if (featureTypeS == "FEATURE_JUNGLE")
                                civ6Tile.feature = Civ6Feature::Jungle;
                            else if (featureTypeS == "FEATURE_FOREST")
                                civ6Tile.feature = Civ6Feature::Forest;
                            else if (featureTypeS == "FEATURE_OASIS")
                                civ6Tile.feature = Civ6Feature::Oasis;
                            else if (featureTypeS == "FEATURE_MARSH")
                                civ6Tile.feature = Civ6Feature::Marsh;
                            else if (featureTypeS == "FEATURE_BARRIER_REEF")
                                civ6Tile.feature = Civ6Feature::BarrierReef;
                            else
                            {
                                LOG_WARNING("Unknown civ6Tile.feature \"%s\" at (%u,%u)", featureTypeS.c_str(), i, j);
                            }
                        }
                    }

                    string continentS;
                    if (GetNextValue(line, curPos, continentS))
                    {
                        bool isUnknownContinent = false;
                        if (isNumber(continentS))
                        {
                            int index = stoi(continentS);

                            if (index == -1)
                            {
                                civ6Tile.continent = ContinentType::None;
                                isUnknownContinent = true;
                            }
                            else
                            {
                                continentS = fmt::sprintf("Continent %u", index);
                            }
                        }

                        if (!isUnknownContinent)
                        {
                            bool found = false;
                            for (uint c = 0; c < continentNames.size(); c++)
                            {
                                if (continentNames[c] == continentS)
                                {
                                    civ6Tile.continent = (ContinentType)c;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                continentNames.push_back(continentS);
                                civ6Tile.continent = (ContinentType)(continentNames.size() - 1);

                                LOG_INFO("Add continent \"%s\" at index %u", continentS.c_str(), (int)civ6Tile.continent);
                            }
                        }
                    }

                    // skip rivers
                    string IsNEOfRiverS;
                    if (GetNextValue(line, curPos, IsNEOfRiverS))
                    {
                    }

                    string IsNEOfRiverFlowS;
                    if (GetNextValue(line, curPos, IsNEOfRiverFlowS))
                    {
                    }

                    string IsWOfRiver;
                    if (GetNextValue(line, curPos, IsWOfRiver))
                    {
                    }

                    string IsWOfRiverFlowS;
                    if (GetNextValue(line, curPos, IsWOfRiverFlowS))
                    {
                    }

                    string IsNWOfRiver;
                    if (GetNextValue(line, curPos, IsNWOfRiver))
                    {
                    }

                    string IsNWOfRiverFlowS;
                    if (GetNextValue(line, curPos, IsNWOfRiverFlowS))
                    {
                    }

                    string resourceS;
                    if (GetNextValue(line, curPos, resourceS))
                    {
                        if (isNumber(resourceS))
                        {
                            auto res = (Civ6Resource)stoi(resourceS);
                            civ6Tile.resource = res;
                        }
                        else
                        {
                            if (resourceS == "RESOURCE_BANANAS")
                                civ6Tile.resource = Civ6Resource::Bananas;
                            else if (resourceS == "RESOURCE_CATTLE")
                                civ6Tile.resource = Civ6Resource::Cattle;
                            else if (resourceS == "RESOURCE_COPPER")
                                civ6Tile.resource = Civ6Resource::Copper;
                            else if (resourceS == "RESOURCE_CRABS")
                                civ6Tile.resource = Civ6Resource::Crabs;
                            else if (resourceS == "RESOURCE_DEER")
                                civ6Tile.resource = Civ6Resource::Deer;
                            else if (resourceS == "RESOURCE_FISH")
                                civ6Tile.resource = Civ6Resource::Fish;
                            else if (resourceS == "RESOURCE_RICE")
                                civ6Tile.resource = Civ6Resource::Rice;
                            else if (resourceS == "RESOURCE_SHEEP")
                                civ6Tile.resource = Civ6Resource::Sheep;
                            else if (resourceS == "RESOURCE_STONE")
                                civ6Tile.resource = Civ6Resource::Stone;
                            else if (resourceS == "RESOURCE_WHEAT")
                                civ6Tile.resource = Civ6Resource::Wheat;
                            else if (resourceS == "RESOURCE_CITRUS")
                                civ6Tile.resource = Civ6Resource::Citrus;
                            else if (resourceS == "RESOURCE_COCOA")
                                civ6Tile.resource = Civ6Resource::Cocoa;
                            else if (resourceS == "RESOURCE_COFFEE")
                                civ6Tile.resource = Civ6Resource::Coffee;
                            else if (resourceS == "RESOURCE_COTTON")
                                civ6Tile.resource = Civ6Resource::Cotton;
                            else if (resourceS == "RESOURCE_DIAMONDS")
                                civ6Tile.resource = Civ6Resource::Diamonds;
                            else if (resourceS == "RESOURCE_DYES")
                                civ6Tile.resource = Civ6Resource::Dyes;
                            else if (resourceS == "RESOURCE_FURS")
                                civ6Tile.resource = Civ6Resource::Furs;
                            else if (resourceS == "RESOURCE_GYPSUM")
                                civ6Tile.resource = Civ6Resource::Gypsum;
                            else if (resourceS == "RESOURCE_INCENSE")
                                civ6Tile.resource = Civ6Resource::Incense;
                            else if (resourceS == "RESOURCE_IVORY")
                                civ6Tile.resource = Civ6Resource::Ivory;
                            else if (resourceS == "RESOURCE_JADE")
                                civ6Tile.resource = Civ6Resource::Jade;
                            else if (resourceS == "RESOURCE_MARBLE")
                                civ6Tile.resource = Civ6Resource::Marble;
                            else if (resourceS == "RESOURCE_MERCURY")
                                civ6Tile.resource = Civ6Resource::Mercury;
                            else if (resourceS == "RESOURCE_PEARLS")
                                civ6Tile.resource = Civ6Resource::Pearls;
                            else if (resourceS == "RESOURCE_SALT")
                                civ6Tile.resource = Civ6Resource::Salt;
                            else if (resourceS == "RESOURCE_SILK")
                                civ6Tile.resource = Civ6Resource::Silk;
                            else if (resourceS == "RESOURCE_SILVER")
                                civ6Tile.resource = Civ6Resource::Silver;
                            else if (resourceS == "RESOURCE_SPICES")
                                civ6Tile.resource = Civ6Resource::Spices;
                            else if (resourceS == "RESOURCE_SUGAR")
                                civ6Tile.resource = Civ6Resource::Sugar;
                            else if (resourceS == "RESOURCE_TEA")
                                civ6Tile.resource = Civ6Resource::Tea;
                            else if (resourceS == "RESOURCE_TOBACCO")
                                civ6Tile.resource = Civ6Resource::Tobacco;
                            else if (resourceS == "RESOURCE_TRUFFLES")
                                civ6Tile.resource = Civ6Resource::Truffles;
                            else if (resourceS == "RESOURCE_WHALES")
                                civ6Tile.resource = Civ6Resource::Whales;
                            else if (resourceS == "RESOURCE_WINE")
                                civ6Tile.resource = Civ6Resource::Wine;
                            else if (resourceS == "RESOURCE_ALUMINUM")
                                civ6Tile.resource = Civ6Resource::Aluminum;
                            else if (resourceS == "RESOURCE_COAL")
                                civ6Tile.resource = Civ6Resource::Coal;
                            else if (resourceS == "RESOURCE_HORSES")
                                civ6Tile.resource = Civ6Resource::Horses;
                            else if (resourceS == "RESOURCE_IRON")
                                civ6Tile.resource = Civ6Resource::Iron;
                            else if (resourceS == "RESOURCE_NITER")
                                civ6Tile.resource = Civ6Resource::Niter;
                            else if (resourceS == "RESOURCE_OIL")
                                civ6Tile.resource = Civ6Resource::Oil;
                            else if (resourceS == "RESOURCE_URANIUM")
                                civ6Tile.resource = Civ6Resource::Uranium;
                            else
                                LOG_WARNING("Unknown civ6Tile.resource \"%s\" at (%u,%u)", resourceS.c_str(), i, j);
                        }
                    }

                    Civ7Tile civ7Tile = ConvertCiv6TileToCiv7(civ6Tile, i, j);

                    m_civ7TerrainType.set(i, j, civ7Tile);
                }
                else if (mapFmt == MapFormat::Civ7)
                {
                    // MapToConvert = { civ7TerrainType, civ7BiomeType, civ7FeatureType, civ7ResourceType }
                    Civ7Tile civ7Tile;

                    // TerrainType
                    {
                        string terrainTypeS;
                        if (GetNextValue(line, curPos, terrainTypeS))
                        {
                            if (isNumber(terrainTypeS))
                            {
                                civ7Tile.terrain = (TerrainType)stoi(terrainTypeS);
                            }
                            else
                            {
                                bool found = false;
                                for (auto val : enumValues<TerrainType>())
                                {
                                    if (terrainTypeS == getTerrainTypeAsString(val.first))
                                    {
                                        civ7Tile.terrain = val.first;
                                        found = true;
                                    }
                                }

                                if (!found)
                                {
                                    LOG_WARNING("Unknown TerrainType \"%s\" at (%u,%u)", terrainTypeS.c_str(), i, j);
                                    civ7Tile.terrain = TerrainType::Flat;
                                }
                            }
                        }
                    }

                    // BiomeType
                    {
                        string biomeTypeS;
                        if (GetNextValue(line, curPos, biomeTypeS))
                        {
                            if (isNumber(biomeTypeS))
                            {
                                civ7Tile.biome = (BiomeType)stoi(biomeTypeS);
                            }
                            else
                            {
                                bool found = false;
                                for (auto val : enumValues<BiomeType>())
                                {
                                    if (biomeTypeS == getBiomeTypeAsString(val.first))
                                    {
                                        civ7Tile.biome = val.first;
                                        found = true;
                                    }
                                }

                                if (!found)
                                {
                                    LOG_WARNING("Unknown BiomeType \"%s\" at (%u,%u)", biomeTypeS.c_str(), i, j);
                                    civ7Tile.biome = BiomeType::Tundra;
                                }
                            }
                        }
                    }

                    // FeatureType
                    {
                        string featureTypeS;
                        if (GetNextValue(line, curPos, featureTypeS))
                        {
                            if (isNumber(featureTypeS))
                            {
                                civ7Tile.feature = (FeatureType)stoi(featureTypeS);
                            }
                            else
                            {
                                bool found = false;
                                for (auto val : enumValues<FeatureType>())
                                {
                                    if (featureTypeS == getFeatureTypeAsString(val.first))
                                    {
                                        civ7Tile.feature = val.first;
                                        found = true;
                                    }
                                }

                                if (!found)
                                {
                                    LOG_WARNING("Unknown FeatureType \"%s\" at (%u,%u)", featureTypeS.c_str(), i, j);
                                    civ7Tile.feature = FeatureType::None;
                                }
                            }
                        }
                    }

                    // ResourceType
                    {
                        string resourceTypeS;
                        if (GetNextValue(line, curPos, resourceTypeS))
                        {
                            if (isNumber(resourceTypeS))
                            {
                                civ7Tile.resource = (ResourceType)stoi(resourceTypeS);
                            }
                            else
                            {
                                bool found = false;
                                for (auto val : enumValues<ResourceType>())
                                {
                                    if (resourceTypeS == getResourceTypeAsString(val.first))
                                    {
                                        civ7Tile.resource = val.first;
                                        found = true;
                                    }
                                }

                                if (!found)
                                {
                                    LOG_WARNING("Unknown ResourceType \"%s\" at (%u,%u)", resourceTypeS.c_str(), i, j);
                                    civ7Tile.resource = ResourceType::Random;
                                }
                            }
                        }
                    }

                    m_civ7TerrainType.set(i, j, civ7Tile);
                }
            }
        }

        offset = posLine + line.length();
        posLine = data.find(token, offset);
    }

    m_continents = continentNames;

    return true;
}

//--------------------------------------------------------------------------------------
// Convert from Civ6 to Civ7 the same way the script does
//--------------------------------------------------------------------------------------
Civ7Tile Map::ConvertCiv6TileToCiv7(const Civ6Tile & _civ6, u32 i, u32 j)
{
    Civ7Tile civ7;

    // Assign civ7 terrain type
    switch (_civ6.terrain)
    {
        default:
            LOG_WARNING("Unknown _civ6.terrain %i at (%u,%u)", (int)_civ6.terrain, i, j);

        case Civ6Terrain::Tundra:
        case Civ6Terrain::Snow:
        case Civ6Terrain::Desert:
        case Civ6Terrain::Plains:
        case Civ6Terrain::Grass:
            civ7.terrain = TerrainType::Flat;
            break;

        case Civ6Terrain::Tundra_Hills:
        case Civ6Terrain::Snow_Hills:
        case Civ6Terrain::Desert_Hills:
        case Civ6Terrain::Plains_Hills:
        case Civ6Terrain::Grass_Hills:
            civ7.terrain = TerrainType::Hill;
            break;

        case Civ6Terrain::Tundra_Moutain:
        case Civ6Terrain::Snow_Moutain:
        case Civ6Terrain::Plains_Moutain:
        case Civ6Terrain::Desert_Moutain:
        case Civ6Terrain::Grass_Moutain:
            civ7.terrain = TerrainType::Moutain;
            break;

        case Civ6Terrain::Coast:
            civ7.terrain = TerrainType::Coast;
            break;

        case Civ6Terrain::Ocean:
            civ7.terrain = TerrainType::Ocean;
            break;
    }

    // Assign civ7 biome type (TODO: check for "Jungle" feature to use the "Tropical" biome)
    {
        switch (_civ6.terrain)
        {
            default:
                LOG_WARNING("Unknown _civ6.terrain %i at (%u,%u)", (int)_civ6.terrain, i, j);

            case Civ6Terrain::Tundra:
            case Civ6Terrain::Tundra_Hills:
            case Civ6Terrain::Tundra_Moutain:
                civ7.biome = BiomeType::Tundra;
                break;

            case Civ6Terrain::Snow:
            case Civ6Terrain::Snow_Hills:
            case Civ6Terrain::Snow_Moutain:
                civ7.biome = BiomeType::Tundra; // No "Snow" biome, use Tundra
                break;

            case Civ6Terrain::Desert:
            case Civ6Terrain::Desert_Hills:
            case Civ6Terrain::Desert_Moutain:
                civ7.biome = BiomeType::Desert;
                break;

            case Civ6Terrain::Plains:
            case Civ6Terrain::Plains_Hills:
            case Civ6Terrain::Plains_Moutain:
                civ7.biome = BiomeType::Plains;
                break;

            case Civ6Terrain::Grass:
            case Civ6Terrain::Grass_Hills:
            case Civ6Terrain::Grass_Moutain:
                civ7.biome = BiomeType::Grassland;
                break;

            case Civ6Terrain::Coast:
            case Civ6Terrain::Ocean:
                civ7.biome = BiomeType::Marine;
                break;
        }
    }

    // Assign feature
    {
        switch (_civ6.feature)
        {
            default:
                LOG_WARNING("Unknown _civ6.feature %i at (%u,%u)", (int)_civ6.feature, i, j);
                civ7.feature = FeatureType::Random;
                break;

            case Civ6Feature::Random:
                civ7.feature = FeatureType::Random;
                break;

            case Civ6Feature::FloodPlains:
            {
                switch(civ7.biome)
                { 
                    case BiomeType::Desert:
                        civ7.feature = FeatureType::DesertFloodplainMinor;
                        break;

                    case BiomeType::Grassland:
                        civ7.feature = FeatureType::GrasslandFloodplainMinor;
                        break;

                    case BiomeType::Plains:
                        civ7.feature = FeatureType::PlainsFloodplainMinor;
                        break;

                    case BiomeType::Tropical:
                        civ7.feature = FeatureType::DesertFloodplainMinor;
                        break;

                    case BiomeType::Tundra:
                        civ7.feature = FeatureType::TundraFloodplainMinor;
                        break;                        
                }
            }
            break;
            
            case Civ6Feature::Ice:
                civ7.feature = FeatureType::Ice;
                break;
            case Civ6Feature::Jungle:
                civ7.biome = BiomeType::Tropical;
                civ7.feature = FeatureType::Forest;
                break;
            case Civ6Feature::Forest:
                break;
            case Civ6Feature::Oasis:
                civ7.feature = FeatureType::Oasis;
                break;
            case Civ6Feature::Marsh:
                civ7.feature = FeatureType::Marsh;
                break;
            case Civ6Feature::BarrierReef:
                civ7.feature = FeatureType::Reef;
                break;
        }
    }

    // continent index
    {
        civ7.continent = (ContinentType)_civ6.continent;
    }

    // Resource
    {
        switch (_civ6.resource)
        {
            default:
                LOG_WARNING("Cannot match _civ6.resource \"%s\" (%i) at (%u,%u)", asString(_civ6.resource).c_str(), (int)_civ6.resource, i, j);
                civ7.resource = ResourceType::Random; 
                break;

            case Civ6Resource::Random:
                civ7.resource = ResourceType::Random;
                break;

            // Replacement
            case Civ6Resource::Crabs:
                civ7.resource = ResourceType::Fish;
                break;

            case Civ6Resource::Sheep:
                civ7.resource = ResourceType::Wool;
                break;

            case Civ6Resource::Bananas:
                civ7.resource = ResourceType::Citrus;
                break;

            case Civ6Resource::Stone:
                civ7.resource = ResourceType::Marble;
                //civ7.resource = ResourceType::Gypsum;
                break;

            case Civ6Resource::Cattle:
                civ7.resource = ResourceType::Camels;
                break;

            case Civ6Resource::Rice:
                civ7.resource = ResourceType::Sugar;
                break;

            case Civ6Resource::Deer:
                civ7.resource = ResourceType::Furs;
                break;

            case Civ6Resource::Wheat:
                civ7.resource = ResourceType::Cotton;
                break;

            case Civ6Resource::Copper:
                civ7.resource = ResourceType::Iron;
                break;

            case Civ6Resource::Diamonds:
                civ7.resource = ResourceType::LapisLazuli;
                break;

            case Civ6Resource::Aluminum:
                civ7.resource = ResourceType::Nickel;
                break;

            case Civ6Resource::Uranium:
                civ7.resource = ResourceType::Coal;
                break;

            // Exact match
            case Civ6Resource::Fish:
                civ7.resource = ResourceType::Fish;
                break;

            case Civ6Resource::Citrus:
                civ7.resource = ResourceType::Citrus;
                break;

            case Civ6Resource::Cocoa:
                civ7.resource = ResourceType::Cocoa;
                break;

            case Civ6Resource::Coffee:
                civ7.resource = ResourceType::Coffee;
                break;

            case Civ6Resource::Cotton:
                civ7.resource = ResourceType::Cotton;
                break;

            case Civ6Resource::Dyes:
                civ7.resource = ResourceType::Dyes;
                break;

            case Civ6Resource::Furs:
                civ7.resource = ResourceType::Furs;
                break;

            case Civ6Resource::Gypsum:
                civ7.resource = ResourceType::Gypsum;
                break;

            case Civ6Resource::Incense:
                civ7.resource = ResourceType::Incense;
                break;

            case Civ6Resource::Ivory:
                civ7.resource = ResourceType::Ivory;
                break;

            case Civ6Resource::Jade:
                civ7.resource = ResourceType::Jade;
                break;

            case Civ6Resource::Marble:
                civ7.resource = ResourceType::Marble;
                break;

            case Civ6Resource::Pearls:
                civ7.resource = ResourceType::Pearls;
                break;

            case Civ6Resource::Salt:
                civ7.resource = ResourceType::Salt;
                break;

            case Civ6Resource::Silk:
                civ7.resource = ResourceType::Silk;
                break;

            case Civ6Resource::Silver:
                civ7.resource = ResourceType::Silver;
                break;

            case Civ6Resource::Spices:
                civ7.resource = ResourceType::Spices;
                break;

            case Civ6Resource::Sugar:
                civ7.resource = ResourceType::Sugar;
                break;

            case Civ6Resource::Tea:
                civ7.resource = ResourceType::Tea;
                break;

            case Civ6Resource::Tobacco:
                civ7.resource = ResourceType::Tobacco;
                break;

            case Civ6Resource::Truffles:
                civ7.resource = ResourceType::Truffles;
                break;

            case Civ6Resource::Whales:
                civ7.resource = ResourceType::Whales;
                break;

            case Civ6Resource::Wine:
                civ7.resource = ResourceType::Wine;
                break;

            case Civ6Resource::Coal:
                civ7.resource = ResourceType::Coal;
                break;

            case Civ6Resource::Horses:
                civ7.resource = ResourceType::Horses;
                break;

            case Civ6Resource::Iron:
                civ7.resource = ResourceType::Iron;
                break;

            case Civ6Resource::Niter:
                civ7.resource = ResourceType::Niter;
                break;

            case Civ6Resource::Oil:
                civ7.resource = ResourceType::Oil;
                break;
        }
    }

    return civ7;
}

//--------------------------------------------------------------------------------------
struct Attribute 
{
    string label;
    string value;
};

//--------------------------------------------------------------------------------------
// Function to trim spaces at the beginning and end of a string
//--------------------------------------------------------------------------------------
string trim(const string & str) 
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if (start == string::npos || end == string::npos) 
        return "";
    return str.substr(start, end - start + 1);
}

//--------------------------------------------------------------------------------------
// Function to parse attributes in a single line
//--------------------------------------------------------------------------------------
vector<Attribute> parseAttributes(const string & line)
{
    vector<Attribute> attributes;
    size_t pos = 0;

    // Loop to process all attributes in the line
    while ((pos = line.find('=', pos)) != string::npos) 
    {
        // Find the start of the key by scanning backward for the first non-space character
        size_t keyEnd = pos;
        size_t keyStart = keyEnd;
        while (keyStart > 0 && isspace(line[keyStart - 1])) 
            --keyStart;

        size_t keyStartReal = keyStart;
        while (keyStartReal > 0 && (isalpha(line[keyStartReal - 1]) || line[keyStartReal - 1] == '_')) 
            --keyStartReal;

        string key = trim(line.substr(keyStartReal, keyEnd - keyStartReal));

        // Find the start and end of the value (ensure it is between double quotes)
        size_t valueStart = line.find('"', pos) + 1;
        if (valueStart == string::npos) 
            break;
        size_t valueEnd = line.find('"', valueStart);
        if (valueEnd == string::npos) 
            break;

        string value = trim(line.substr(valueStart, valueEnd - valueStart));

        // Store 
        attributes.push_back({ key, value });

        pos = valueEnd + 1;
    }

    return attributes;
}

//--------------------------------------------------------------------------------------
string getExportCivName(const string & _civName)
{
    return fmt::sprintf("CIVILIZATION_%s", ToUpperLabel(_civName));
}

//--------------------------------------------------------------------------------------
bool Map::importTSL()
{
    const string tslPath = fmt::sprintf("%s\\%s.xml", GetFolder(m_mapPath), getBaseName());
    string tslData;
    if (ReadFile(tslPath, tslData))
    {
        const string startLabel = "<StartPosition>";
        const string endLabel = "</StartPosition>";

        auto tslStart = tslData.find(startLabel);
        if (-1 == tslStart)
        {
            LOG_ERROR("Could not find \"%s\" in TSL data", "<StartPosition>");
            return false;
        }

        auto tslEnd = tslData.find(endLabel);
        if (-1 != tslEnd)
        {
            // Extract the relevant portion of the input
            string relevantInput = tslData.substr(tslStart + startLabel.length(), tslEnd - tslStart - startLabel.length());

            // Split relevant input into lines
            stringstream ss(relevantInput);
            string line;

            while (getline(ss, line)) 
            {
                // Check if line contains a <Replace> tag
                if (line.find("<Replace") != string::npos)
                {
                    //LOG_INFO("%s", line.c_str());

                    // Parse the attributes in this line
                    vector<Attribute> attributes = parseAttributes(line);

                    int xStart = -1, yStart = -1;
                    for (const auto & attribute : attributes)
                    {
                        if (attribute.label == "X")
                        {
                            xStart = atoi(attribute.value.c_str());
                        }
                        else if (attribute.label == "Y")
                        {
                            yStart = atoi(attribute.value.c_str());
                        }
                    }

                    TSL tsl;
                    int civIndex = 0; // First Civ is "Invalid" civ
                    for (const auto & attribute : attributes)
                    {
                        if (attribute.label == "Civilization")
                        {
                            for (uint i = 0; i < m_civilizations.size(); ++i)
                            {
                                const string civName = m_civilizations[i].name;
                                if (getExportCivName(civName) == attribute.value)
                                {
                                    civIndex = i;
                                }
                            }

                            if (civIndex == 0)
                            {
                                LOG_ERROR("Invalid civilization \"%s\" for TSL at (%i,%i) ", attribute.value.c_str(), xStart, yStart);
                            }
                        }
                        else if (attribute.label == "X")
                        {
                            tsl.pos.x = atoi(attribute.value.c_str());
                        }
                        else if (attribute.label == "Y")
                        {
                            tsl.pos.y = atoi(attribute.value.c_str());
                        }
                    }

                    if (xStart >= 0 && xStart < (int)m_width && yStart >= 0 && yStart <= (int)m_height)
                    {
                        tsl.pos.x = xStart;
                        tsl.pos.y = yStart;

                        auto & civ = m_civilizations[civIndex];
                        LOG_INFO("Add TSL for civilization \"%s\" at (%i, %i)", civ.name.c_str(), tsl.pos.x, tsl.pos.y);
                        civ.tsl.push_back(tsl);
                    }
                    else
                    {
                        const auto & civ = m_civilizations[civIndex];
                        LOG_ERROR("Civilization \"%s\" has invalid TSL at (%i,%i) ", civ.name.c_str(), xStart, yStart);
                    }
                }
            }
        }

        return true;
    }

    LOG_WARNING("Could not read TSL data from \"%s\"", tslPath.c_str());
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::importFiles(const string & _cwd)
{
    string data;
    if (ReadFile(m_mapPath, data))
    {
        string name = getBaseName();
        m_modFolder = fmt::sprintf("%s\\mods\\%s", _cwd, name);

        // We're looking for a map script with a "generate format" function
        const string genMapToken = "generateMap()";
        auto generateMap = data.find(genMapToken);
        if (string::npos != generateMap)
        {
            m_isLoaded = true;

            // Get pretty name (for some it's different than the file name)
            importPrettyName(data);

            importHemispheres(data);

            if (!importYnAMP(data))
                return false;

            fixHemispheres();
            importTSL();

            return true;
        }
        else
        {
            LOG_ERROR("\"%s\" not found. File \"%s\" is not a valid YnAMP map.", genMapToken.c_str(), GetFilename(m_mapPath).c_str());
        }
    }

    return false; 
}