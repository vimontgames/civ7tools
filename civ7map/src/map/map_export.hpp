//--------------------------------------------------------------------------------------
// TODO: Double hashtable TerrainType <=> Civ7TerrainString?
//--------------------------------------------------------------------------------------
string getTerrainTypeAsString(TerrainType _type)
{
    switch (_type)
    {
        case TerrainType::Moutain:
            return "\"TERRAIN_MOUNTAIN\"";
        case TerrainType::Hill:
            return "\"TERRAIN_HILL\"";
        default:
            LOG_WARNING("Unknow TerrainType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case TerrainType::Flat:
            return "\"TERRAIN_FLAT\"";
        case TerrainType::Coast:
            return "\"TERRAIN_COAST\"";
        case TerrainType::Ocean:
            return "\"TERRAIN_OCEAN\"";
        case TerrainType::NavigableRiver:
            return "\"TERRAIN_NAVIGABLE_RIVER\"";
    }
}

//--------------------------------------------------------------------------------------
string getBiomeTypeAsString(BiomeType _type)
{
    switch (_type)
    {
        case BiomeType::Tundra:
            return "\"BIOME_TUNDRA\"";
        case BiomeType::Grassland:
            return "\"BIOME_GRASSLAND\"";
        case BiomeType::Plains:
            return "\"BIOME_PLAINS\"";
        case BiomeType::Tropical:
            return "\"BIOME_TROPICAL\"";
        case BiomeType::Desert:
            return "\"BIOME_DESERT\"";
        default:
            LOG_WARNING("Unknow BiomeType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case BiomeType::Marine:
            return "\"BIOME_MARINE\"";
    }
}

//--------------------------------------------------------------------------------------
string getFeatureTypeAsString(FeatureType _type)
{
    switch (_type)
    {
        default:
            LOG_WARNING("Unknow FeatureType \"%s\" (%i)", asString(_type).c_str(), (int)_type);
        case FeatureType::None:
            return "-1";

        case FeatureType::SagebrushSteppe:
            return "\"FEATURE_SAGEBRUSH_STEPPE\"";
        case FeatureType::Oasis:
            return "\"FEATURE_OASIS\"";
        case FeatureType::DesertFloodplainMinor:
            return "\"FEATURE_DESERT_FLOODPLAIN_MINOR\"";
        case FeatureType::DesertFloodplainNavigable:
            return "\"FEATURE_DESERT_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Forest:
            return "\"FEATURE_FOREST\"";
        case FeatureType::Marsh:
            return "\"FEATURE_MARSH\"";
        case FeatureType::GrasslandFloodplainMinor:
            return "\"FEATURE_GRASSLAND_FLOODPLAIN_MINOR\"";
        case FeatureType::GrasslandFloodplainNavigable:
            return "\"FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Reef:
            return "\"FEATURE_REEF\"";
        case FeatureType::ColdReef:
            return "\"FEATURE_COLD_REEF\"";
        case FeatureType::Ice:
            return "\"FEATURE_ICE\"";
        case FeatureType::SavanaWoodland:
            return "\"FEATURE_SAVANAWOODLAND\"";
        case FeatureType::WateringHole:
            return "\"FEATURE_WATERINGHOLE\"";
        case FeatureType::PlainsFloodplainMinor:
            return "\"FEATURE_PLAINS_FLOODPLAIN_MINOR\"";
        case FeatureType::PlainsFloodplainNavigable:
            return "\"FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::RainForest:
            return "\"FEATURE_RAIN_FOREST\"";
        case FeatureType::Mangrove:
            return "\"FEATURE_MANGROVE\"";
        case FeatureType::TropicalFloodplainMinor:
            return "\"FEATURE_TROPICAL_FLOODPLAIN_MINOR\"";
        case FeatureType::TropicalFloodplainNavigable:
            return "\"FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Taiga:
            return "\"FEATURE_TAIGA\"";
        case FeatureType::TundraBog:
            return "\"FEATURE_TUNDRA_BOG\"";
        case FeatureType::TundraFloodplainMinor:
            return "\"FEATURE_TUNDRA_FLOODPLAIN_MINOR\"";
        case FeatureType::TundraFloodplainNavigable:
            return "\"FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE\"";
        case FeatureType::Volcano:
            return "\"FEATURE_VOLCANO\"";
    }
}


//--------------------------------------------------------------------------------------
void Map::exportCiv7Map(string & _map, const string & _cwd)
{
    string data = fmt::sprintf
    (
        "export function GetMapV7() {\n"
        "\n"
        "    let MapToConvertV7 = [];\n"
        "    for (let i = 0; i < %u; i++) {\n"
        "        MapToConvertV7[i] = [];\n"
        "    }\n"
        "\n"
        "    // Map Data (Civ7)\n"
        "    // MapToConvertV7 = { civ7TerrainType, civ7BiomeType, civ7FeatureType }\n"
        "\n"
        "\n",
        width
    );

    for (uint j = 0; j < height; ++j)
    {
        for (uint i = 0; i < width; ++i)
        {
            const Civ7Tile & tile = civ7TerrainType.get(i, j);

            data += fmt::sprintf("MapToConvertV7[%u][%u]=[%s, %s, %s]\n", i, j, getTerrainTypeAsString(tile.terrain), getBiomeTypeAsString(tile.biome), getFeatureTypeAsString(tile.feature));
        }
    }

    data +=
        "\n"
        "    return MapToConvertV7;\n"
        "}"
        "\n"
        "//# sourceMappingURL=file:///base-standard/maps/continents-plus.js.map\n";

    FILE * fp = fopen(_map.c_str(), "wb");
    
    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }

    //string::size_type idx = _map.rfind('.');
    //string extension;
    //if (idx != string::npos)
    //    extension = _map.substr(idx + 1);
    //for (auto c : extension)
    //    c = tolower(c);
    //if (extension != "js")
    //    _map += ".js";
    //
    //{
    //    XMLElement * xmlDoc = xmlDocDescriptor.FirstChildElement("Document");
    //    XMLElement * xmlTerrainSaveDescriptor = xmlDoc->FirstChildElement("TerrainSaveDescriptor");
    //    xmlTerrainSaveDescriptor->FirstChildElement("EmpiresCount")->FirstChild()->SetValue(to_string(empireCount).c_str());
    //}
    //
    //XMLElement * xmlDoc = xmlDocSave.FirstChildElement("Document");
    //XMLElement * xmlTerrainSave = xmlDoc->FirstChildElement("TerrainSave");
    //
    //xmlTerrainSave->FirstChildElement("UseMapCycling")->FirstChild()->SetValue(useMapCycling ? "true" : "false");
    //xmlTerrainSave->FirstChildElement("UseProceduralMountainChains")->FirstChild()->SetValue(useProceduralMountainChains ? "true" : "false");
    //
    //XMLElement * xmlEntitiesProvider = xmlTerrainSave->FirstChildElement("EntitiesProvider");
    //XMLElement * xmlSpawnPoints = xmlEntitiesProvider->FirstChildElement("SpawnPoints");
    //
    //xmlSpawnPoints->DeleteChildren();
    //
    //xmlSpawnPoints->SetAttribute("Length", allSpawnsPoints.size());
    //
    //for (u32 i = 0; i < allSpawnsPoints.size(); ++i)
    //{
    //    const auto & spawn = allSpawnsPoints[i];
    //
    //    XMLElement * xmlItem = xmlSpawnPoints->InsertNewChildElement("Item");
    //        
    //    XMLElement * xmlSpawnPoints = xmlItem->InsertNewChildElement("SpawnPoints");
    //    xmlSpawnPoints->InsertNewChildElement("Column")->InsertFirstChild(xmlDocSave.NewText(to_string(spawn.pos.x).c_str()));
    //    xmlSpawnPoints->InsertNewChildElement("Row")->InsertFirstChild(xmlDocSave.NewText(to_string(height - spawn.pos.y - 1).c_str()));
    //
    //    XMLElement * xmlFlags = xmlItem->InsertNewChildElement("Flags");
    //    xmlFlags->InsertFirstChild(xmlDocSave.NewText(to_string(spawn.flags).c_str()));
    //}        
    //
    //XMLNode * xmlLandmarkDatabase = xmlTerrainSave->FirstChildElement("LandmarkDatabase");
    //
    //if (xmlLandmarkDatabase)
    //{
    //    if (landmarkInfo.empty())
    //    {
    //        XMLElement * xmlLandarks = xmlLandmarkDatabase->FirstChildElement("Landmarks");
    //        xmlLandarks->DeleteAttribute("Length");
    //        xmlLandarks->DeleteChildren();
    //        xmlLandarks->SetAttribute("Null", "true");
    //    }
    //    else
    //    {
    //        assert(!"todo");
    //    }
    //}
    //
    //saveBitmap(elevationTexture,     xmlTerrainSave, "ElevationTexture");
    //saveBitmap(zonesTexture,         xmlTerrainSave, "ZonesTexture");
    //saveBitmap(poiTexture,           xmlTerrainSave, "POITexture");
    //saveBitmap(landmarksTexture,     xmlTerrainSave, "LandmarksTexture");
    //saveBitmap(naturalWonderTexture, xmlTerrainSave, "NaturalWonderTexture");
    //saveBitmap(riverTexture,         xmlTerrainSave, "RiverTexture");
    //saveBitmap(visibilityTexture,    xmlTerrainSave, "VisibilityTexture");
    //saveBitmap(roadTexture,          xmlTerrainSave, "RoadTexture");
    //saveBitmap(matchingSeedTexture,  xmlTerrainSave, "MatchingSeedTexture");
    //
    //string tempFolder = _cwd + "\\tmp\\";
    //bool created = CreateDirectory(tempFolder.c_str(), nullptr);
    //tempFolder = _cwd + "\\tmp\\zip\\";
    //created = CreateDirectory(tempFolder.c_str(), nullptr);
    //tempFolder = _cwd + "\\tmp\\zip\\write\\";
    //created = CreateDirectory(tempFolder.c_str(), nullptr);
    //
    //SetCurrentDirectory(tempFolder.c_str());
    //
    //string xmlDescriptorFilename = "Descriptor.hmd";
    //string xmlSaveFilename = "Save.hms";
    //
    //xmlDocDescriptor.SaveFile((xmlDescriptorFilename).c_str());
    //xmlDocSave.SaveFile((xmlSaveFilename).c_str());
    //
    //miniz_cpp::zip_file archive;
    //archive.write(xmlDescriptorFilename.c_str());
    //archive.write(xmlSaveFilename.c_str());
    //archive.save(_map.c_str());
}

//--------------------------------------------------------------------------------------
void Map::saveBitmap(const Array2D<u32> _bitmap, XMLElement * _xmlTerrainSave, const string & _field)
{
    XMLElement * xmlBytes = _xmlTerrainSave->FirstChildElement((_field + ".Bytes").c_str());

    if (xmlBytes)
    {
        int len = 0;
        ubyte * png = stbi_write_png_to_mem((const ubyte*)_bitmap.data(), width * sizeof(u32), width, height, 4, &len);
        string pngBase64 = base64::encode((char*)png, (size_t)len);

        xmlBytes->SetAttribute("Length", len);
        xmlBytes->FirstChild()->SetValue(pngBase64.c_str());
    }
}