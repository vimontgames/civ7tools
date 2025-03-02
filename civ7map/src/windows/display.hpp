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
    BaseWindow("Display")
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
    ImGui::ColorEdit3(FeatureType::None != _feature ? asString(_feature).c_str() : "No feature", f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
void DrawColor(const Map * _map, ResourceType _resource)
{
    float4 color = getResourceColor(_resource);
    float f3Color[] = { color.r, color.g,  color.b };
    ImGui::ColorEdit3(ResourceType::None != _resource ? asString(_resource).c_str() : "No resource", f3Color, ImGuiColorEditFlags_NoInputs);
}

//--------------------------------------------------------------------------------------
bool DisplayWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Display", &m_visible) && g_map)
    {
        if (TreeNodeEx("Map", ImGuiTreeNodeFlags_DefaultOpen))
        {
            needRefresh |= Combo("GridType", (int *)&g_map->m_gridType, "Regular\0Offset\0Hexagon\0\0");
            needRefresh |= Checkbox("Show Borders", &g_map->m_showBorders);

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
                    auto vals = magic_enum::enum_entries<FeatureType>();

                    for (auto val : vals)
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
                    auto vals = magic_enum::enum_entries<ResourceType>();
                    for (auto val : vals)
                        DrawColor(g_map, val.first);
                }
                break;
            }

            //needRefresh |= Checkbox(getFixedSizeString("Borders", g_fixedTextLengthShort).c_str(), &g_map->showTerritoriesBorders);

            //ImGui::SameLine();
            
            g_map->bitmaps[(int)MapBitmap::TerrainData].visible = true; // g_map->territoryBackground != TerritoryBackground::None;

            TreePop();
        }

        if (0 && ImGui::TreeNodeEx("Resources", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto ListResources = [=](ResourceInfo * _infos, u32 _first, u32 _last)
            {
                bool changed = false;

                ImGui::Indent();
                for (u32 i = _first; i <= _last; ++i)
                {
                    const u32 index = i - _first;
                    auto & info = _infos[index];

                    changed |= ImGui::Checkbox(getFixedSizeString(info.name.c_str(), g_fixedTextLengthLarge).c_str(), &info.visible);
                    ImGui::SameLine();
                    float * pFloat = (float *)&info.color.rgba[0];
                    changed |= ImGui::ColorEdit4(info.name.c_str(), pFloat, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoLabel);

                    ImGui::SameLine();
                    ImGui::Text(to_string(_infos[index].count).c_str());
                }
                ImGui::Unindent();

                return changed;
            };

            needRefresh |= ImGui::Checkbox(getFixedSizeString("Spawns", g_fixedTextLengthShort).c_str(), &g_map->showSpawnPoints);

            if (g_map->showSpawnPoints)
            {
                PushItemWidth(g_comboxItemWidth);

                ImGui::Indent();
                {
                    for (u32 i = 0; i < _countof(g_map->spawnInfo); ++i)
                    {
                        needRefresh |= ImGui::RadioButton(getFixedSizeString(to_string(i + 1) + " player" + string(i ? "s" : ""), g_fixedTextLengthLarge).c_str(), (int *)&g_map->spawnPlayerCountDisplayed, int(i + 1));
                        SameLine();

                        u32 count = 0;
                        for (u32 j = 0; j < g_map->allSpawnsPoints.size(); ++j)
                        {
                            const SpawnPoint & spawn = g_map->allSpawnsPoints[j];
                            if (spawn.flags & (1 << i))
                                count++;
                        }

                        ImGui::Text("%u/%u", count, i + 1);
                    }
                }
                ImGui::Unindent();

                PopItemWidth();
            }

            //g_map->bitmaps[Resources].visible = g_map->showStrategicResources || g_map->showLuxuryResources || g_map->showWonders || g_map->showSpawnPoints;

            ImGui::TreePop();
        }
    }
    ImGui::End();

    return needRefresh;
}