
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
bool isNumber(const std::string & str) 
{
    if (str.empty()) 
        return false;

    size_t start = 0;
    if (str[0] == '-') 
    {
        if (str.size() == 1) return false; 
            start = 1; 
    }

    for (size_t i = start; i < str.size(); ++i) 
    {
        if (!std::isdigit(str[i])) 
            return false;
    }

    return true;
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
bool Map::GetCiv7TerrainFromCiv6(const string & data)
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
            m_GetMapPath = data.substr(importGetMap + importGetMapToken.length(), endImportGetMap - importGetMap - importGetMapToken.length());

            // Expect the data file to be in the same folder as the original file ...
            m_GetMapPath = GetFolder(m_path) + "\\" + GetFilename(m_GetMapPath);

            LOG_INFO("File \"%s\" imports GetMap from \"%s\"", GetFilename(m_path).c_str(), m_GetMapPath.c_str());

            string getmapData;
            if (ReadFile(m_GetMapPath, getmapData))
            {
                return GetCiv7TerrainFromCiv6(getmapData);
            }
        }

        LOG_ERROR("Could not find \"%s\"", label.c_str());
        return false;
    }

    // First pass to get map size
    int mapWidth = 0, mapHeight = 0;

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

    // save map size
    m_width = mapWidth;
    m_height = mapHeight;

    // restart for actual parsing 
    posLine = firstToken;
    offset = 0;

    LOG_INFO("Map size is %ux%u", mapWidth, mapHeight);
    civ7TerrainType.SetSize(mapWidth, mapHeight);

    // Clear map (in case of holes in data)
    for (u32 j = 0; j < civ7TerrainType.Height(); ++j)
    {
        for (u32 i = 0; i < civ7TerrainType.Width(); ++i)
        {
            Civ7Tile tile = {};
            civ7TerrainType.set(i, j, tile);
        }
    }

    vector<string> continentNames;
    
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

                            LOG_INFO("Add continent \"%s\" (%u)", continentS.c_str(), (int)civ6Tile.continent);
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

                civ7TerrainType.set(i, j, civ7Tile);
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
                civ7.feature = FeatureType::None;
                break;

            case Civ6Feature::None:
                civ7.feature = FeatureType::None;
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
                civ7.resource = ResourceType::None; 
                break;

            case Civ6Resource::None:
                civ7.resource = ResourceType::None;
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

            // New format does not have map size in main .js, so deduce it from parsing MapToConvert[i][j] instead
            //u32 w, h;
            //if (GetValue(data, "iWidth", w))
            //    this->width = w;
            //
            //if (GetValue(data, "iHeight", h))
            //    this->height = h;
            
            //civ7TerrainType.SetSize(w, h);

            if (!GetCiv7TerrainFromCiv6(data))
                return false;

            return true;
        }
    }

    return false; 
}