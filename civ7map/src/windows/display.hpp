#include "BaseWindow.h"
#include "shader/colors.h"
#include "shader/common.h"

static_assert((MapFilter)PASS_TYPE_ALL       == MapFilter::All);
static_assert((MapFilter)PASS_TYPE_TERRAIN   == MapFilter::Terrain);
static_assert((MapFilter)PASS_TYPE_BIOME     == MapFilter::Biome);
static_assert((MapFilter)PASS_TYPE_FEATURE   == MapFilter::Feature);
static_assert((MapFilter)PASS_TYPE_RESOURCE  == MapFilter::Resource);
static_assert((MapFilter)PASS_TYPE_CONTINENT == MapFilter::Continent);
static_assert((MapFilter)PASS_TYPE_LANDMASS  == MapFilter::Landmass);
static_assert((MapFilter)PASS_TYPE_ELEVATION == MapFilter::Elevation);

//--------------------------------------------------------------------------------------
class DisplayWindow : public BaseWindow
{
public:
    DisplayWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
DisplayWindow::DisplayWindow() :
    BaseWindow(ICON_FA_DISPLAY" Display")
{

}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, ContinentType _continent, bool _count = false)
{
    float4 color = getContinentColor(_continent);
    string continentName = _map->getContinentShortName(_continent);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s (%u)", continentName.c_str(), _map->getContinentInfo(_continent).count).c_str());
    else
        ImGui::Text(continentName.c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, LandmassType _landmass, bool _count = false)
{
    float4 color = getLandmassColor(_landmass);
    string landmassName = _map->getLandmassShortName(_landmass);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    ImGui::Text(landmassName.c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, Elevation _elevation, bool _count = false)
{
    float4 color = getElevationColor(_elevation);
    float f3Color[] = { color.r, color.g,  color.b };
    string landmassName = fmt::sprintf("%u", _elevation);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    ImGui::Text("%i", _elevation);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, TerrainType _terrain, bool _count = false)
{
    float4 color = getTerrainColor(_terrain);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s (%u)", asString(_terrain), _map->getTerrainInfo(_terrain).count).c_str());
    else
        ImGui::Text(asString(_terrain).c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, BiomeType _biome, bool _count = false)
{
    float4 color = getBiomeColor(_biome);
    float f3Color[] = { color.r, color.g,  color.b };
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s (%u)", asString(_biome), _map->getBiomeInfo(_biome).count).c_str());
    else
        ImGui::Text(asString(_biome).c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, FeatureType _feature, bool _count = false)
{
    float4 color = getFeatureColor(_feature);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s (%u)", asString(_feature), _map->getFeatureInfo(_feature).count).c_str());
    else
        ImGui::Text(asString(_feature).c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, ResourceType _resource, bool _count = false)
{
    float4 color = getResourceColor(_resource);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s (%u)", asString(_resource), _map->getResourceInfo(_resource).count).c_str());
    else
        ImGui::Text(asString(_resource).c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, BiomeType _biome, TerrainType _terrain, bool _count = false)
{
    float4 color = getBiomeTerrainColor(_biome, _terrain);
    
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    if (_count)
        ImGui::Text(fmt::sprintf("%s %s (%u)", asString(_biome), asString(_terrain), _map->getBiomeTerrainInfo(_biome, _terrain).count).c_str());
    else
        ImGui::Text(fmt::sprintf("%s %s", asString(_biome), asString(_terrain)).c_str());
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, NaturalWonderType _naturalWonder, bool _count = false)
{
    float4 color = getNaturalWonderColor(_naturalWonder);
    DrawSmallColoredSquare(color);
    ImGui::SameLine();
    ImGui::Text(asString(_naturalWonder).c_str());
}

bool g_selectOverlayImage = false;

//--------------------------------------------------------------------------------------
bool DisplayWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_DISPLAY" Display###Display", &m_visible) && g_map)
    {
        if (ImGui::CollapsingHeader("Show", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            needRefresh |= Checkbox("Grid", &g_map->m_showBorders);

            if (g_map->m_mapVersion == MapVersion::YnAMP)
                needRefresh |= Checkbox("Hemispheres", &g_map->m_showHemispheres);

            needRefresh |= Checkbox("Features", &g_map->m_showFeatures);
            needRefresh |= Checkbox("Resources", &g_map->m_showResources);
            needRefresh |= Checkbox("TSL", &g_map->m_showTSL);   
            needRefresh |= Checkbox("Overlay", &g_map->m_showOverlayImage);
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_IMAGE))
            {
                g_selectOverlayImage = true;
            }

            ImGui::SameLine();

            ImGui::SliderFloat("Opacity", &g_map->m_overlayOpacity, 0.0f, 1.0f);
        }

        if (g_selectOverlayImage)
        {
            ImGui::OpenPopup("Select Overlay");
            g_selectOverlayImage = false;
            SetCurrentDirectory(g_myDocumentsPath.c_str());
            ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
        }

        if (g_fileDialog.showFileDialog("Select Overlay", ImGuiFileBrowser::DialogMode::OPEN, ImVec2(float(g_screenWidth) / 2.0f, float(g_screenHeight) / 2.0f), ".png,.tga,.jpg"))
        {
            const string newFilePath = g_fileDialog.selected_path;
            
            if (g_map->m_overlayTex.loadFromFile(newFilePath))
            {
                LOG_INFO("Overlay texture \"%s\" loaded.", newFilePath.c_str());
                g_map->m_overlayTex.generateMipmap();
            }
            else
            {
                LOG_ERROR("Could not load overlay texture \"%s\".", newFilePath.c_str());
            }        

            SetCurrentDirectory(g_currentWorkingDirectory.c_str());
            ImGui::GetIO().IniFilename = g_saveImGuiIniPath;
        }

        //needRefresh |= Combo("GridType", (int *)&g_map->m_gridType, "Regular\0Offset\0Hexagon\0\0");

        if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            // e.g."All\0TerrainType\0Biome\0Feature\0Resource\0Continent\0Landmass\0Elevation\0\0"
            string mapFilters = "";
            for (uint i = 0; i < enumCount<MapFilter>(); ++i)
            {
                mapFilters += asString((MapFilter)i);
                mapFilters.push_back('\0');
            }
            mapFilters.push_back('\0');

            needRefresh |= Combo("###Layer", (int *)&g_map->m_mapFilter, mapFilters.c_str(), ImGuiComboFlags_HeightLargest);

            ImGui::Spacing();

            switch (g_map->m_mapFilter)
            {
                default:
                    LOG_ERROR("Missing case \"%s\" (%i)", asString(g_map->m_mapFilter).c_str(), (int)g_map->m_mapFilter);
                    break;

                case MapFilter::All:
                {
                    for (uint i = 0; i < enumCount<BiomeType>(); ++i)
                    {
                        for (uint j = 0; j < enumCount<TerrainType>(); ++j)
                        {
                            BiomeType biome = (BiomeType)i;
                            TerrainType terrain = (TerrainType)j;
                            if (BiomeType::Marine == biome)
                            {
                                if (terrain != TerrainType::Ocean && terrain != TerrainType::Coast)
                                    continue;
                            }
                            else
                            {
                                if (terrain == TerrainType::Ocean || terrain == TerrainType::Coast)
                                    continue;
                            }

                            if (terrain == TerrainType::NavigableRiver)
                                continue;

                            DrawColor(g_map, biome, terrain, true);
                        }
                        ImGui::Separator();
                    }
                }
                break;

                case MapFilter::Terrain:
                {
                    for (uint i = 0; i < enumCount<TerrainType>(); ++i)
                        DrawColor(g_map, (TerrainType)i, true);
                }
                break;

                case MapFilter::Biome:
                {
                    for (uint i = 0; i < enumCount<BiomeType>(); ++i)
                        DrawColor(g_map, (BiomeType)i, true);
                }
                break;

                case MapFilter::Feature:
                {
                    for (auto val : enumValues<FeatureType>())
                        DrawColor(g_map, val.first, true);
                }
                break;

                case MapFilter::Resource:
                {
                    for (auto val : enumValues<ResourceType>())
                        DrawColor(g_map, val.first, true);
                }
                break;

                case MapFilter::Continent:
                {
                    for (uint i = 0; i < g_map->getContinentCount(); ++i)
                        DrawColor(g_map, (ContinentType)i);
                }
                break;

                case MapFilter::Landmass:
                {
                    for (uint i = 0; i < g_map->getLandmassCount(); ++i)
                        DrawColor(g_map, (LandmassType)i);
                }
                break;

                case MapFilter::Elevation:
                {
                    DrawColor(g_map, (Elevation)0);
                    DrawColor(g_map, (Elevation)128);
                    DrawColor(g_map, (Elevation)256);
                    DrawColor(g_map, (Elevation)512);
                    DrawColor(g_map, (Elevation)768);
                    DrawColor(g_map, (Elevation)1024);
                }
                break;
            }
        }
            
        g_map->m_bitmaps[(int)MapBitmap::TerrainData].visible = true; // g_map->territoryBackground != TerritoryBackground::None;   
    }
    ImGui::End();

    return needRefresh;
}