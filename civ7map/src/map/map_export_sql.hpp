
//--------------------------------------------------------------------------------------
void Map::exportFilesCiv7Map(const string & _cwd, bool _useModTemplate)
{
    exportModInfoCiv7Map();

    //exportModInfo();
    //exportSQLTables();
    //exportConfig();
    //exportMap();
    //exportMapData();
    //exportMapText();
    //exportModuleText();
    //exportTSL();

    LOG_ERROR("Saving Civ7Map files is not yet fully implemented");
}

//--------------------------------------------------------------------------------------
void Map::exportModInfoCiv7Map()
{
    std::string data;

    data +=              "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    data += fmt::sprintf("<Mod id=\"%s\" version=\"1\" xmlns=\"ModInfo\">\n",                               getModID());
    data +=              "  <Properties>\n";
    data += fmt::sprintf("      <Name>%s</Name>\n",                                                         getPrettyName());
    data += fmt::sprintf("      <Description>%s</Description>\n",                                           getDescription());
    data += fmt::sprintf("      <Authors>%s</Authors>\n",                                                   getAuthor());
    data +=              "      <Package>Mod</Package>\n";
    data +=              "      <AffectsSavedGames>1</AffectsSavedGames>\n";
    data +=              "  </Properties>\n";
    data +=              "  <Dependencies>\n";
    data +=              "      <Mod id=\"base-standard\" title=\"LOC_MODULE_BASE_STANDARD_NAME\"/>\n";
    data +=              "  </Dependencies>\n";
    data +=              "  <ActionCriteria>\n";
    data +=              "      <Criteria id=\"always\">\n";
    data +=              "          <AlwaysMet></AlwaysMet>\n";
    data +=              "      </Criteria>\n";
    data +=              "  </ActionCriteria>\n";
    data +=              "  <ActionGroups>\n";
    data += fmt::sprintf("      <ActionGroup id=\"%s-shell\" scope=\"shell\" criteria=\"always\">\n",       getModID());
    data +=              "          <Actions>\n";
    data +=              "              <UpdateDatabase>\n";
    data +=              "                  <Item>config/config.xml</Item>\n";
    data +=              "              </UpdateDatabase>\n";
    data +=              "              <UpdateText>\n";
    data +=              "                  <Item>text/zh_Hans_CN_Text.xml</Item>\n";
    data +=              "              </UpdateText>\n";
    data +=              "          </Actions>\n";
    data +=              "      </ActionGroup>\n";
    data += fmt::sprintf("      <ActionGroup id=\"%s-game\" scope=\"game\" criteria=\"always\">\n",         getModID());
    data +=              "          <Actions>\n";
    data +=              "              <UpdateDatabase>\n";
    data +=              "                  <Item>data/maps.xml</Item>\n";
    data +=              "              </UpdateDatabase>\n";
    data +=              "              <UpdateText>\n";
    data +=              "                  <Item>text/zh_Hans_CN_Text.xml</Item>\n";
    data +=              "              </UpdateText>\n";
    data +=              "          </Actions>\n";
    data +=              "      </ActionGroup>\n";
    data +=              "  </ActionGroups>\n";
    data +=              "  <LocalizedText>\n";
    data +=              "      <File>text/zh_Hans_CN_Text.xml</File>\n";
    data +=              "  </LocalizedText>\n";
    data +=              "</Mod>\n";

    string modInfoPath = fmt::sprintf("%s\\%s.modinfo", m_modFolder, GetFilenameWithoutExtension(m_mapPath));
    FILE * fp = fopen(modInfoPath.c_str(), "wb");

    if (fp)
    {
        fwrite(data.c_str(), sizeof(char), data.size(), fp);
        fclose(fp);
    }
}