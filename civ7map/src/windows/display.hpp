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
void DrawColor(const Map * _map, ContinentType _continent)
{
    float4 color = getContinentColor(_continent);
    float f3Color[] = { color.r, color.g,  color.b };
    string continentName = _map->getContinentShortName(_continent);
    ImGui::ColorEdit3(continentName.c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, TerrainType _terrain)
{
    float4 color = getTerrainColor(_terrain);
    float f3Color[] = { color.r, color.g,  color.b };
    ImGui::ColorEdit3(asString(_terrain).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, BiomeType _biome)
{
    float4 color = getBiomeColor(_biome);
    float f3Color[] = { color.r, color.g,  color.b };
    ImGui::ColorEdit3(asString(_biome).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, FeatureType _feature)
{
    float4 color = getFeatureColor(_feature);
    float f3Color[] = { color.r, color.g,  color.b };
    ImGui::ColorEdit3(asString(_feature).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, ResourceType _resource)
{
    float4 color = getResourceColor(_resource);
    float f3Color[] = { color.r, color.g,  color.b };
    ImGui::ColorEdit3(asString(_resource).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
bool DisplayWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_DISPLAY" Display###Display", &m_visible) && g_map)
    {
        if (TreeNodeEx("Map", ImGuiTreeNodeFlags_DefaultOpen))
        {
            needRefresh |= Checkbox("Show Resources", &g_map->m_showResources);
            needRefresh |= Checkbox("Show TSL", &g_map->m_showTSL);
            needRefresh |= Checkbox("Show Grid", &g_map->m_showBorders);

            //needRefresh |= Combo("GridType", (int *)&g_map->m_gridType, "Regular\0Offset\0Hexagon\0\0");

            ImGui::Separator();

            needRefresh |= Combo("Layer", (int *)&g_map->m_mapFilter, "TerrainType\0Biome\0Feature\0Continents\0Resource\0\0");

            switch (g_map->m_mapFilter)
            {
                default:
                    LOG_ERROR("Missing case \"%s\" (%i)", asString(g_map->m_mapFilter).c_str(), (int)g_map->m_mapFilter);
                    break;

                case MapFilter::TerrainType:
                {       
                    for (uint i = 0; i < enumCount<TerrainType>(); ++i)
                        DrawColor(g_map, (TerrainType)i);
                }
                break;

                case MapFilter::Biome:
                {
                    for (uint i = 0; i < enumCount<BiomeType>(); ++i)
                        DrawColor(g_map, (BiomeType)i);
                }
                break;

                case MapFilter::Feature:
                {
                    for (auto val : enumValues<FeatureType>())
                        DrawColor(g_map, val.first);
                }
                break;

                case MapFilter::Continent:
                {
                    for (uint i = 0; i < g_map->getContinentCount(); ++i)
                        DrawColor(g_map, (ContinentType)i);
                }
                break;

                case MapFilter::Resource:
                {
                    for (auto val : enumValues<ResourceType>())
                        DrawColor(g_map, val.first);
                }
                break;
            }
            
            g_map->m_bitmaps[(int)MapBitmap::TerrainData].visible = true; // g_map->territoryBackground != TerritoryBackground::None;

            TreePop();
        }        
    }
    ImGui::End();

    return needRefresh;
}