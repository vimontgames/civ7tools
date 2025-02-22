
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
                    value = static_cast<uint32_t>(stoi(numberStr));
       return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool isDigits(const string & str) 
{
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

//--------------------------------------------------------------------------------------
bool GetNextValue(string & input, size_t & pos, string & value) 
{
    value.clear();

    // Skip whitespace and closing brackets
    while (pos < input.size() && (isspace(input[pos]) || input[pos] == '[' || input[pos] == ']') || input[pos] == '=')
    {
        ++pos;
    }

    if (pos >= input.size()) 
    {
        return false; // End of string
    }

    // Extract an identifier (letters, digits, underscores)
    if (isalpha(input[pos]) || input[pos] == '_') {
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
bool Map::GetCiv7TerrainFromCiv6(const string & data)
{
    const string label = "MapToConvert";

    for (u32 j = 0; j < civ7TerrainType.Height(); ++j)
    {
        for (u32 i = 0; i < civ7TerrainType.Width(); ++i)
        {
            Civ7Tile tile = {};
            civ7TerrainType.set(i, j, tile);
        }
    }

    size_t offset = 0;
    const string token = label + (string)"[";

    size_t posLine = data.find(token);

    if (-1 == posLine)
    {
        // This could be using data from an external file, e.g. new version of "greatest-earth-map.js" imports map data from "greatest-earth-data.js"
        const string importGetMapToken = "import { GetMap } from '";
        size_t importGetMap = data.find(importGetMapToken);
        if (-1 != importGetMap)
        {
            size_t endImportGetMap = data.find("'", importGetMap + importGetMapToken.length() + 1);
            m_GetMapPath = data.substr(importGetMap + importGetMapToken.length(), endImportGetMap - importGetMap - importGetMapToken.length());

            // Expect the data file to be in the same folder as the original file ...
            m_GetMapPath = GetFolder(m_path) + "\\" + GetFilename(m_GetMapPath);

            LOG_WARNING("File \"%s\" imports GetMap from \"%s\"", GetFilename(m_path).c_str(), m_GetMapPath.c_str());

            string getmapData;
            if (ReadFile(m_GetMapPath, getmapData))
            {
                return GetCiv7TerrainFromCiv6(getmapData);
            }
        }

        LOG_ERROR("Could not find \"%s\"", label.c_str());
        return false;
    }
    
    // Some maps have data in reverse order so we need to parse [i][j]
    while (posLine != -1)
    {
        string line = data.substr(posLine);
        size_t eol = line.find_first_of("\n");
        line = line.substr(0, eol);
        line = line.substr(token.length());

        // first digit is 'i'
        size_t curPos = 0;

        string iS, jS;
        int i, j;

        if (GetNextValue(line, curPos, iS))
        {
            if (isDigits(iS))
            {
                i = stoi(iS);

                string jS;
                if (GetNextValue(line, curPos, jS))
                {
                    j = stoi(jS);

                    // Now we have correct i and j let's assume the file format is OK
                    Civ6Tile civ6Tile;

                    string terrainTypeS;
                    if (GetNextValue(line, curPos, terrainTypeS))
                    {
                        if (isDigits(terrainTypeS))
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
                                civ6Tile.terrain = Civ6Terrain::Default;
                            }
                        }
                    }

                    string featureTypeS;
                    if (GetNextValue(line, curPos, featureTypeS))
                    {
                        if (isDigits(featureTypeS))
                        {
                            civ6Tile.feature = (Civ6Feature)stoi(featureTypeS);
                        }
                        else
                        {
                            LOG_WARNING("Unknown civ6Tile.feature \"%s\" at (%u,%u)", terrainTypeS.c_str(), i, j);
                        }
                    }

                    Civ7Tile civ7Tile = ConvertCiv6TileToCiv7(civ6Tile, i, j);

                    civ7TerrainType.set(i, j, civ7Tile);
                }
            }
        }

        offset = posLine + line.length();
        posLine = data.find(token, offset);
    }

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
            LOG_WARNING("Unknown civ6TerrainType index %i at (%u,%u)", (int)_civ6.terrain, i, j);

        case Civ6Terrain::Tundra:
        case Civ6Terrain::Snow:
        case Civ6Terrain::Desert:
        case Civ6Terrain::Plains:
        case Civ6Terrain::Grass:
            civ7.terrainType = TerrainType::Flat;
            break;

        case Civ6Terrain::Tundra_Hills:
        case Civ6Terrain::Snow_Hills:
        case Civ6Terrain::Desert_Hills:
        case Civ6Terrain::Plains_Hills:
        case Civ6Terrain::Grass_Hills:
            civ7.terrainType = TerrainType::Hill;
            break;

        case Civ6Terrain::Tundra_Moutain:
        case Civ6Terrain::Snow_Moutain:
        case Civ6Terrain::Plains_Moutain:
        case Civ6Terrain::Desert_Moutain:
        case Civ6Terrain::Grass_Moutain:
            civ7.terrainType = TerrainType::Moutain;
            break;

        case Civ6Terrain::Coast:
            civ7.terrainType = TerrainType::Coast;
            break;

        case Civ6Terrain::Ocean:
            civ7.terrainType = TerrainType::Ocean;
            break;
    }

    // Assign civ7 biome type (TODO: check for "Jungle" feature to use the "Tropical" biome)
    {
        switch (_civ6.terrain)
        {
            case Civ6Terrain::Tundra:
            case Civ6Terrain::Tundra_Hills:
            case Civ6Terrain::Tundra_Moutain:
                civ7.biomeType = BiomeType::Tundra;
                break;

            case Civ6Terrain::Snow:
            case Civ6Terrain::Snow_Hills:
            case Civ6Terrain::Snow_Moutain:
                civ7.biomeType = BiomeType::Tundra; // No "Snow" biome, use Tundra
                break;

            case Civ6Terrain::Desert:
            case Civ6Terrain::Desert_Hills:
            case Civ6Terrain::Desert_Moutain:
                civ7.biomeType = BiomeType::Desert;
                break;

            case Civ6Terrain::Plains:
            case Civ6Terrain::Plains_Hills:
            case Civ6Terrain::Plains_Moutain:
                civ7.biomeType = BiomeType::Plains;
                break;

            case Civ6Terrain::Grass:
            case Civ6Terrain::Grass_Hills:
            case Civ6Terrain::Grass_Moutain:
                civ7.biomeType = BiomeType::Grassland;
                break;

            case Civ6Terrain::Coast:
            case Civ6Terrain::Ocean:
                civ7.biomeType = BiomeType::Marine;
                break;
        }
    }

    return civ7;
}

//--------------------------------------------------------------------------------------
bool Map::importCiv7Map(const string & _map, const string & _cwd)
{
    string data;
    if (ReadFile(_map, data))
    {
        // We're looking for a map script with a "generate format" function
        auto generateMap = data.find("generateMap()");
        if (string::npos != generateMap)
        {
            loaded = true;

            u32 w, h;
            if (GetValue(data, "iWidth", w))
                this->width = w;

            if (GetValue(data, "iHeight", h))
                this->height = h;
            
            civ7TerrainType.SetSize(w, h);

            if (!GetCiv7TerrainFromCiv6(data))
                return false;

            return true;
        }
    }

    return false; 
}