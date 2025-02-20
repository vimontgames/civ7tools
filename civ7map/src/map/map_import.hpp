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
bool Map::importCiv7Map(const string & _map, const string & _cwd)
{
    //// unzip file contents to temp folder
    //miniz_cpp::zip_file archive(_map);
    //string tempFolder = _cwd + "\\tmp\\";
    //bool created = CreateDirectory(tempFolder.c_str(), nullptr);
    //tempFolder = _cwd + "\\tmp\\zip\\";
    //created = CreateDirectory(tempFolder.c_str(), nullptr);
    //tempFolder = _cwd + "\\tmp\\zip\\read\\";
    //created = CreateDirectory(tempFolder.c_str(), nullptr);
    //archive.extractall(tempFolder);
    //
    //XMLError xmlErr;
    //
    //// load descriptor
    //{
    //    string xmlDescriptorFilename = tempFolder + "Descriptor.hmd";
    //    xmlErr = xmlDocDescriptor.LoadFile(xmlDescriptorFilename.c_str());
    //    assert(XML_SUCCESS == xmlErr);
    //
    //    XMLElement * xmlDoc = xmlDocDescriptor.FirstChildElement("Document");
    //    XMLElement * xmlTerrainSaveDescriptor = xmlDoc->FirstChildElement("TerrainSaveDescriptor");
    //    u32 count = 0;
    //    xmlErr = xmlTerrainSaveDescriptor->FirstChildElement("EmpiresCount")->ToElement()->QueryUnsignedText(&count);
    //    empireCount = count;
    //}
    //
    //// load xml
    //string xmlSaveFilename = tempFolder + "Save.hms";
    //xmlErr = xmlDocSave.LoadFile(xmlSaveFilename.c_str());
    //
    //XMLElement * xmlDoc = xmlDocSave.FirstChildElement("Document");
    //XMLElement * xmlTerrainSave = xmlDoc->FirstChildElement("TerrainSave");
    //XMLNode * xmlAuthor = xmlTerrainSave->FirstChildElement("Author")->FirstChild();
    //if (xmlAuthor)
    //    author = xmlAuthor->ToText()->Value();
    //
    //xmlErr = xmlTerrainSave->FirstChildElement("Width")->ToElement()->QueryUnsignedText(&width);
    //xmlErr = xmlTerrainSave->FirstChildElement("Height")->ToElement()->QueryUnsignedText(&height);
    //
    //xmlErr = xmlTerrainSave->FirstChildElement("UseMapCycling")->ToElement()->QueryBoolText(&useMapCycling);
    //xmlErr = xmlTerrainSave->FirstChildElement("UseProceduralMountainChains")->ToElement()->QueryBoolText(&useProceduralMountainChains);
    //
    //XMLElement * xmlElevation = xmlTerrainSave->FirstChildElement("ElevationTexture.Bytes");
    //u32 elevationLength = 0;
    //xmlErr = xmlElevation->QueryUnsignedAttribute("Length", &elevationLength);
    //
    //loadBitmap(elevationTexture,     xmlTerrainSave, "ElevationTexture",     width, height);
    //loadBitmap(zonesTexture,         xmlTerrainSave, "ZonesTexture",         width, height);
    //loadBitmap(poiTexture,           xmlTerrainSave, "POITexture",           width, height);
    //loadBitmap(landmarksTexture,     xmlTerrainSave, "LandmarksTexture",     width, height);
    //loadBitmap(naturalWonderTexture, xmlTerrainSave, "NaturalWonderTexture", width, height);
    //loadBitmap(riverTexture,         xmlTerrainSave, "RiverTexture",         width, height);
    //loadBitmap(visibilityTexture,    xmlTerrainSave, "VisibilityTexture",    width, height);
    //loadBitmap(roadTexture,          xmlTerrainSave, "RoadTexture",          width, height);
    //loadBitmap(matchingSeedTexture,  xmlTerrainSave, "MatchingSeedTexture",  width, height);
    //
    //// load spawn points
    //{
    //    XMLElement * xmlEntitiesProvider = xmlTerrainSave->FirstChildElement("EntitiesProvider");
    //    XMLElement * xmlSpawnPoints = xmlEntitiesProvider->FirstChildElement("SpawnPoints");
    //
    //    u32 spawnPointCount = 0;
    //    xmlErr = xmlSpawnPoints->QueryUnsignedAttribute("Length", &spawnPointCount);
    //
    //    allSpawnsPoints.clear();
    //    allSpawnsPoints.reserve(spawnPointCount);
    //
    //    XMLNode * xmlSpawnNode = xmlSpawnPoints->FirstChildElement("Item");
    //
    //    if (xmlSpawnNode)
    //    {
    //        XMLElement * xmlSpawn = xmlSpawnNode->ToElement();
    //        while (xmlSpawn)
    //        {
    //            SpawnPoint spawn;
    //
    //            xmlErr = xmlSpawn->FirstChildElement("SpawnPoints")->FirstChildElement("Column")->ToElement()->QueryUnsignedText(&spawn.pos.x);
    //            xmlErr = xmlSpawn->FirstChildElement("SpawnPoints")->FirstChildElement("Row")->ToElement()->QueryUnsignedText(&spawn.pos.y);
    //
    //            spawn.pos.y = height - spawn.pos.y - 1;
    //
    //            xmlErr = xmlSpawn->FirstChildElement("Flags")->ToElement()->QueryUnsignedText(&spawn.flags);
    //
    //            allSpawnsPoints.push_back(spawn);
    //
    //            xmlSpawn = xmlSpawn->NextSiblingElement("Item");
    //        }
    //    }
    //
    //    // default to max spawn count
    //    for (u32 i = 0; i < _countof(spawnInfo); ++i)
    //    {
    //        for (u32 j = 0; j < allSpawnsPoints.size(); ++j)
    //        {
    //            SpawnPoint & spawn = allSpawnsPoints[j];
    //            if ((1 << i) & spawn.flags)
    //                spawnPlayerCountDisplayed = i + 1;
    //        }
    //    }
    //
    //    computeSpawnOrder();
    //}
    //
    //// load natural wonders info
    //{
    //    //XMLElement * xmlNaturalWonderNames = xmlTerrainSave->FirstChildElement("NaturalWonderNames");
    //    //u32 naturalWonderNameCount = 0;
    //    //xmlErr = xmlNaturalWonderNames->QueryUnsignedAttribute("Length", &naturalWonderNameCount);
    //    //
    //    //naturalWondersInfo.clear();
    //    //naturalWondersInfo.reserve(naturalWonderNameCount);
    //    //
    //    //XMLNode * xmlWonderNameNode = xmlNaturalWonderNames->FirstChildElement("String");
    //    //
    //    //int i = 0;
    //    //
    //    //ColorFloat4 wonderColors[] = 
    //    //{
    //    //    {1.00f, 1.00f, 0.50f, 1.00f},
    //    //    {0.00f, 1.00f, 1.00f, 1.00f},
    //    //    {0.00f, 0.00f, 1.00f, 1.00f},
    //    //    {0.00f, 0.60f, 1.00f, 1.00f},
    //    //    {0.00f, 0.80f, 1.00f, 1.00f},
    //    //    {0.00f, 0.40f, 1.00f, 1.00f},
    //    //    {0.00f, 0.20f, 1.00f, 1.00f},
    //    //    {0.90f, 0.90f, 0.90f, 1.00f},
    //    //    {0.80f, 0.80f, 0.80f, 1.00f},
    //    //    {0.70f, 0.70f, 0.70f, 1.00f},
    //    //    {0.70f, 0.00f, 0.00f, 1.00f},
    //    //    {0.75f, 0.75f, 1.00f, 1.00f},
    //    //    {0.90f, 1.00f, 0.60f, 1.00f},
    //    //    {1.00f, 1.00f, 0.00f, 1.00f},
    //    //};
    //    //
    //    //while (xmlWonderNameNode)
    //    //{
    //    //    string name = xmlWonderNameNode->FirstChild()->ToText()->Value();
    //    //    
    //    //    NaturalWonder wonder;
    //    //
    //    //    wonder.name = name;
    //    //
    //    //    wonder.color = wonderColors[i % _countof(wonderColors)];
    //    //
    //    //    naturalWondersInfo.push_back(wonder);
    //    //
    //    //    xmlWonderNameNode = xmlWonderNameNode->NextSiblingElement("String");
    //    //    ++i;
    //    //}
    //}
    //
    //// load territories info
    //{
    //    XMLNode * xmlTerritoryDatabase = xmlTerrainSave->FirstChildElement("TerritoryDatabase");
    //    XMLElement * xmlTerritories = xmlTerritoryDatabase->FirstChildElement("Territories");
    //    u32 territoryCount = 0;
    //    xmlErr = xmlTerritories->QueryUnsignedAttribute("Length", &territoryCount);
    //
    //    territoriesInfo.clear();
    //    territoriesInfo.reserve(territoryCount);
    //
    //    XMLNode * xmlTerritoryNode = xmlTerritories->FirstChildElement("Item");
    //
    //    if (xmlTerritoryNode)
    //    {
    //        XMLElement * xmlTerritory = xmlTerritoryNode->ToElement();
    //        while (xmlTerritory)
    //        {
    //            Territory territory;
    //
    //            xmlErr = xmlTerritory->FirstChildElement("ContinentIndex")->ToElement()->QueryUnsignedText(&territory.continent);
    //            xmlErr = xmlTerritory->FirstChildElement("Biome")->ToElement()->QueryUnsignedText(&territory.biome);
    //            xmlErr = xmlTerritory->FirstChildElement("IsOcean")->ToElement()->QueryBoolText(&territory.ocean);
    //
    //            territoriesInfo.push_back(territory);
    //
    //            xmlTerritory = xmlTerritory->NextSiblingElement("Item");
    //        }
    //    }
    //}
    //
    //// load landmarks info
    //{
    //    XMLNode * xmlLandmarkDatabase = xmlTerrainSave->FirstChildElement("LandmarkDatabase");
    //    XMLElement * xmlLandmarks = xmlLandmarkDatabase->FirstChildElement("Landmarks");
    //    u32 landmarkCount = 0;
    //    xmlErr = xmlLandmarks->QueryUnsignedAttribute("Length", &landmarkCount);
    //
    //    landmarkInfo.clear();
    //    landmarkInfo.reserve(landmarkCount);
    //
    //    XMLNode * xmlLandmarkNode = xmlLandmarks->FirstChildElement("Item");
    //
    //    if (xmlLandmarkNode)
    //    {
    //        XMLElement * xmlLandmark = xmlLandmarkNode->ToElement();
    //
    //        while (xmlLandmark)
    //        {
    //            Landmark landmark;
    //
    //            landmark.name = xmlLandmark->FirstChildElement("Name")->FirstChild()->ToText()->Value();
    //            xmlErr = xmlLandmark->FirstChildElement("DefinitionIndex")->ToElement()->QueryUnsignedText(&landmark.definitonIndex);
    //
    //            landmarkInfo.push_back(landmark);
    //
    //            xmlLandmark = xmlLandmark->NextSiblingElement("Item");
    //        }
    //    }
    //}

    // Load the .js file to a temp buffer
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
            return true;
        }
    }

    return false; 
}