//--------------------------------------------------------------------------------------
void Map::exportCiv7Map(string & _map, const string & _cwd)
{
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