//--------------------------------------------------------------------------------------
u32 * Map::loadTexture(XMLElement * _xmlTerrainSave, const string & _name)
{
    XMLElement * xmlBytes = _xmlTerrainSave->FirstChildElement((_name + ".Bytes").c_str());
    u32 elevationLength = 0;
    XMLError xmlErr = xmlBytes->QueryUnsignedAttribute("Length", &elevationLength);

    string base64encodedData = xmlBytes->FirstChild()->ToText()->Value();
    base64::decode_inplace(base64encodedData);
    std::vector<ubyte> temp;
    temp.resize(elevationLength);
    for (u32 i = 0; i < elevationLength; ++i)
        temp[i] = base64encodedData[i];

    int x, y, comp;
    u32 * data = (u32*)stbi_load_from_memory(temp.data(), (int)temp.size(), &x, &y, &comp, 0);
    return data;
}

//--------------------------------------------------------------------------------------
template <typename T> void Map::loadBitmap(Array2D<T> & _array, tinyxml2::XMLElement * _xmlTerrainSave, const std::string & _name, u32 _width, u32 _height)
{
    u32 * bitmap = loadTexture(_xmlTerrainSave, _name.c_str());
    _array = Array2D<T>(_width, _height, bitmap);
    SAFE_FREE(bitmap);
}

//--------------------------------------------------------------------------------------
bool GetValue(const std::string & data, const std::string label, u32 & value)
{
    const string token = label + " =";
    auto pos = data.find(token);
    if (-1 != pos)
    {
        std::string numberStr = data.substr(pos + token.length() + 1);
                    numberStr = numberStr.substr(0, numberStr.find_first_of(";")); 
                    value = static_cast<uint32_t>(std::stoi(numberStr));
       return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool isDigits(const std::string & str) 
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

//--------------------------------------------------------------------------------------
bool GetNextValue(string & input, size_t & pos, std::string & value) 
{
    value.clear();

    // Skip whitespace and closing brackets
    while (pos < input.size() && (std::isspace(input[pos]) || input[pos] == '[' || input[pos] == ']') || input[pos] == '=')
    {
        ++pos;
    }

    if (pos >= input.size()) 
    {
        return false; // End of string
    }

    // Extract an identifier (letters, digits, underscores)
    if (std::isalpha(input[pos]) || input[pos] == '_') {
        while (pos < input.size() && (std::isalnum(input[pos]) || input[pos] == '_')) 
        {
            value += input[pos++];
        }
        return true;
    }

    // Extract a number (including negative numbers)
    if (std::isdigit(input[pos]) || (input[pos] == '-' && pos + 1 < input.size() && std::isdigit(input[pos + 1]))) 
    {
        if (input[pos] == '-') 
            value += input[pos++]; // Include negative sign if present

        while (pos < input.size() && std::isdigit(input[pos]))
        {
            value += input[pos++];
        }
        return true;
    }

    // Extract individual symbols (e.g., `{`, `}`, `[`, `]`, `,`, `=`)
    if (std::string("{}[]=,").find(input[pos]) != std::string::npos) {
        value = input[pos++];
        return true;
    }

    // If we reach here, move to the next character and retry
    ++pos;
    return GetNextValue(input, pos, value);
}

//--------------------------------------------------------------------------------------
bool Map::GetCiv7TerrainFromCiv6(const string & data, const string label)
{
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
        Civ6TerrainType civ6TerrainType;
        if (GetNextValue(line, curPos, iS))
        {
            if (isDigits(iS))
            {
                i = stoi(iS);

                string jS;
                if (GetNextValue(line, curPos, jS))
                {
                    j = stoi(jS);
                    
                    string terrainTypeS;
                    if (GetNextValue(line, curPos, terrainTypeS))
                    {
                        if (isDigits(terrainTypeS))
                        {
                            civ6TerrainType = (Civ6TerrainType)std::stoi(terrainTypeS);
                        }
                        else
                        {
                            // https://civilization.fandom.com/wiki/Terrain_(Civ7)#Nomenclature

                            if (terrainTypeS == "TERRAIN_COAST")
                                civ6TerrainType = Civ6TerrainType::Coast;
                            else if (terrainTypeS == "TERRAIN_OCEAN")
                                civ6TerrainType = Civ6TerrainType::Ocean;
                            else if (terrainTypeS == "TERRAIN_GRASS")
                                civ6TerrainType = Civ6TerrainType::Grass;
                            else if (terrainTypeS == "TERRAIN_GRASS_HILLS")
                                civ6TerrainType = Civ6TerrainType::Grass_Hills;
                            else if (terrainTypeS == "TERRAIN_GRASS_MOUNTAIN")
                                civ6TerrainType = Civ6TerrainType::Grass_Moutain;
                            else if (terrainTypeS == "TERRAIN_PLAINS")
                                civ6TerrainType = Civ6TerrainType::Plains;
                            else if (terrainTypeS == "TERRAIN_PLAINS_HILLS")
                                civ6TerrainType = Civ6TerrainType::Plains_Hills;
                            else if (terrainTypeS == "TERRAIN_PLAINS_MOUNTAIN")
                                civ6TerrainType = Civ6TerrainType::Plains_Moutain;
                            else if (terrainTypeS == "TERRAIN_TUNDRA")
                                civ6TerrainType = Civ6TerrainType::Tundra;
                            else if (terrainTypeS == "TERRAIN_TUNDRA_HILLS")
                                civ6TerrainType = Civ6TerrainType::Tundra_Hills;
                            else if (terrainTypeS == "TERRAIN_TUNDRA_MOUNTAIN")
                                civ6TerrainType = Civ6TerrainType::Tundra_Moutain;
                            else if (terrainTypeS == "TERRAIN_DESERT")
                                civ6TerrainType = Civ6TerrainType::Desert;
                            else if (terrainTypeS == "TERRAIN_DESERT_HILLS")
                                civ6TerrainType = Civ6TerrainType::Desert_Hills;
                            else if (terrainTypeS == "TERRAIN_DESERT_MOUNTAIN")
                                civ6TerrainType = Civ6TerrainType::Desert_Moutain;
                            else if (terrainTypeS == "TERRAIN_SNOW")
                                civ6TerrainType = Civ6TerrainType::Snow;
                            else if (terrainTypeS == "TERRAIN_SNOW_HILLS")
                                civ6TerrainType = Civ6TerrainType::Snow_Hills;
                            else if (terrainTypeS == "TERRAIN_SNOW_MOUNTAIN")
                                civ6TerrainType = Civ6TerrainType::Snow_Moutain;
                            else
                            {
                                LOG_WARNING("[Import] Unknown terrain type \"%s\" at (%u,%u)", terrainTypeS.c_str(), i , j);
                                civ6TerrainType = (Civ6TerrainType)0;
                            }
                        }
                    }

                    // Convert from Civ6 to Civ7 the same way the script does
                    Civ7Tile tile;

                    // Assign civ7 terrain type
                    switch (civ6TerrainType)
                    {
                        default:
                            LOG_WARNING("Unknown civ6TerrainType = %i", (int)civ6TerrainType);

                        case Civ6TerrainType::Tundra:
                        case Civ6TerrainType::Snow:
                        case Civ6TerrainType::Desert:
                        case Civ6TerrainType::Plains:
                        case Civ6TerrainType::Grass:
                            tile.terrainType = TerrainType::Flat;
                            break;

                        case Civ6TerrainType::Tundra_Hills:
                        case Civ6TerrainType::Snow_Hills:
                        case Civ6TerrainType::Desert_Hills:
                        case Civ6TerrainType::Plains_Hills:
                        case Civ6TerrainType::Grass_Hills:
                            tile.terrainType = TerrainType::Hill;
                            break;

                        case Civ6TerrainType::Tundra_Moutain:
                        case Civ6TerrainType::Snow_Moutain:
                        case Civ6TerrainType::Plains_Moutain:
                        case Civ6TerrainType::Desert_Moutain:
                        case Civ6TerrainType::Grass_Moutain:
                            tile.terrainType = TerrainType::Moutain;
                            break;

                        //case 6: // desert
                        //    color = Color(192, 192, 96, 255);
                        //    break;

                        case Civ6TerrainType::Coast: // coast
                            tile.terrainType = TerrainType::Coast;
                            break;

                        case Civ6TerrainType::Ocean: // ocean
                            tile.terrainType = TerrainType::Ocean;
                            break;
                    }

                    // Assign civ7 biome type (TODO: check for "Jungle" feature to use the "Tropical" biome)
                    {
                        switch (civ6TerrainType)
                        {
                            case Civ6TerrainType::Tundra:
                            case Civ6TerrainType::Tundra_Hills:
                            case Civ6TerrainType::Tundra_Moutain:
                                tile.biomeType = BiomeType::Tundra;
                                break;
       
                            case Civ6TerrainType::Snow:
                            case Civ6TerrainType::Snow_Hills:
                            case Civ6TerrainType::Snow_Moutain:
                                tile.biomeType = BiomeType::Tundra; // No "Snow" biome, use Tundra
                                break;
                          
                            case Civ6TerrainType::Desert:
                            case Civ6TerrainType::Desert_Hills:
                            case Civ6TerrainType::Desert_Moutain:
                                tile.biomeType = BiomeType::Desert;
                                break;
                           
                            case Civ6TerrainType::Plains:
                            case Civ6TerrainType::Plains_Hills:
                            case Civ6TerrainType::Plains_Moutain:
                                tile.biomeType = BiomeType::Plains;
                                break;
                          
                            case Civ6TerrainType::Grass:
                            case Civ6TerrainType::Grass_Hills:
                            case Civ6TerrainType::Grass_Moutain:
                                tile.biomeType = BiomeType::Grassland;
                                break;
                            
                            case Civ6TerrainType::Coast:
                            case Civ6TerrainType::Ocean:
                                tile.biomeType = BiomeType::Marine;
                                break;
                        }
                    }

                    civ7TerrainType.set(i, j, tile);
                }
            }      
        }

        offset = posLine + line.length();
        posLine = data.find(token, offset);
    }

    return true;
}

//--------------------------------------------------------------------------------------
bool Map::importCiv7Map(const string & _map, const string & _cwd)
{
    FILE * fp = fopen(_map.c_str(), "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        size_t filesize = ftell(fp);
        rewind(fp); 
        char * temp = (char *)malloc(filesize + 1);
        fread(temp, 1, filesize, fp);
        temp[filesize] = '\0';
        fclose(fp);
        string data = (string)temp;
        free(temp);

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

            if (!GetCiv7TerrainFromCiv6(data, "MapToConvert"))
                return false;

            return true;
        }
    }

    return false; 
}