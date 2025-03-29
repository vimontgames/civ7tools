#include "BaseWindow.h"
#include "shader/colors.h"

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
    float f3Color[] = { color.r, color.g,  color.b };
    string continentName = _map->getContinentShortName(_continent);
    ImGui::ColorEdit3(continentName.c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, TerrainType _terrain, bool _count = false)
{
    float4 color = getTerrainColor(_terrain);
    float f3Color[] = { color.r, color.g,  color.b };

    if (_count)
        ImGui::ColorEdit3(fmt::sprintf("%s (%u)", asString(_terrain), _map->getTerrainInfo(_terrain).count).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
    else
        ImGui::ColorEdit3(asString(_terrain).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, BiomeType _biome, bool _count = false)
{
    float4 color = getBiomeColor(_biome);
    float f3Color[] = { color.r, color.g,  color.b };

    if (_count)
        ImGui::ColorEdit3(fmt::sprintf("%s (%u)", asString(_biome), _map->getBiomeInfo(_biome).count).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
    else
        ImGui::ColorEdit3(asString(_biome).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, FeatureType _feature, bool _count = false)
{
    float4 color = getFeatureColor(_feature);
    float f3Color[] = { color.r, color.g,  color.b };
    if (_count)
        ImGui::ColorEdit3(fmt::sprintf("%s (%u)", asString(_feature), _map->getFeatureInfo(_feature).count).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
    else
        ImGui::ColorEdit3(asString(_feature).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, ResourceType _resource, bool _count = false)
{
    float4 color = getResourceColor(_resource);
    float f3Color[] = { color.r, color.g,  color.b };
    if (_count)
        ImGui::ColorEdit3(fmt::sprintf("%s (%u)", asString(_resource), _map->getResourceInfo(_resource).count).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
    else
        ImGui::ColorEdit3(asString(_resource).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, BiomeType _biome, TerrainType _terrain, bool _count = false)
{
    float4 color = getBiomeTerrainColor(_biome, _terrain);
    float f3Color[] = { color.r, color.g,  color.b };
    if (_count)
        ImGui::ColorEdit3( fmt::sprintf("%s %s (%u)", asString(_biome), asString(_terrain), _map->getBiomeTerrainInfo(_biome, _terrain).count).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
    else
        ImGui::ColorEdit3(fmt::sprintf("%s %s", asString(_biome), asString(_terrain)).c_str(), f3Color, ImGuiColorEditFlags_NoInputs); // TODO
}

//--------------------------------------------------------------------------------------
bool DisplayWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_DISPLAY" Display###Display", &m_visible) && g_map)
    {
        if (ImGui::CollapsingHeader("Show", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            needRefresh |= Checkbox("Grid", &g_map->m_showBorders);
            needRefresh |= Checkbox("Hemispheres", &g_map->m_showHemispheres);
            needRefresh |= Checkbox("Features", &g_map->m_showFeatures);
            needRefresh |= Checkbox("Resources", &g_map->m_showResources);
            needRefresh |= Checkbox("TSL", &g_map->m_showTSL);            
        }

        //needRefresh |= Combo("GridType", (int *)&g_map->m_gridType, "Regular\0Offset\0Hexagon\0\0");

        if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            needRefresh |= Combo("###Layer", (int *)&g_map->m_mapFilter, "All\0Terrain\0Biome\0Feature\0Continents\0Resource\0\0");

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

                case MapFilter::TerrainType:
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

                case MapFilter::Continent:
                {
                    for (uint i = 0; i < g_map->getContinentCount(); ++i)
                        DrawColor(g_map, (ContinentType)i, true);
                }
                break;

                case MapFilter::Resource:
                {
                    for (auto val : enumValues<ResourceType>())
                        DrawColor(g_map, val.first, true);
                }
                break;
            }
        }
            
        g_map->m_bitmaps[(int)MapBitmap::TerrainData].visible = true; // g_map->territoryBackground != TerritoryBackground::None;   
    }
    ImGui::End();

    return needRefresh;
}