#include "BaseWindow.h"

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
bool DisplayWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Display", &m_visible) && g_map)
    {
        if (TreeNodeEx("Map", ImGuiTreeNodeFlags_DefaultOpen))
        {
            needRefresh |= Checkbox(getFixedSizeString("Line offset", g_fixedTextLengthShort).c_str(), &g_map->useHexUVs);
            ImGui::Separator();

            PushItemWidth(g_comboxItemWidth);
            needRefresh |= Combo("Filter", (int *)&g_map->territoryBackground, "TerrainType\0Biome\0Continents\0\0");
            PopItemWidth();

            switch (g_map->territoryBackground)
            {
                case MapFilter::TerrainType:
                {                   
                    float flatColor[] = { 0.3f, 0.3f, 0.3f };
                    ImGui::ColorEdit3("Flat", flatColor, ImGuiColorEditFlags_NoInputs);

                    float hillColor[] = { 0.6f, 0.6f, 0.6f };
                    ImGui::ColorEdit3("Hill", hillColor, ImGuiColorEditFlags_NoInputs);

                    float moutainColor[] = { 0.9f, 0.9f, 0.9f };
                    ImGui::ColorEdit3("Moutain", moutainColor, ImGuiColorEditFlags_NoInputs);

                    float oceanColor[] = { 0, 0, 1 };
                    ImGui::ColorEdit3("Ocean", oceanColor, ImGuiColorEditFlags_NoInputs);

                    float coastColor[] = { 0, 0.5f, 1 };
                    ImGui::ColorEdit3("Coast", coastColor, ImGuiColorEditFlags_NoInputs);

                    float navigableRiverColor[] = { 0, 1, 1 };
                    ImGui::ColorEdit3("Navigable River", navigableRiverColor, ImGuiColorEditFlags_NoInputs);
                }
                break;

                case MapFilter::Biome:
                {
                    float tundraColor[] = { 0.8f, 1.0f, 0.8f };
                    ImGui::ColorEdit3("Tundra", tundraColor, ImGuiColorEditFlags_NoInputs);

                    float grasslandColor[] = { 0.0f, 1.0f, 0.0f };
                    ImGui::ColorEdit3("Grassland", grasslandColor, ImGuiColorEditFlags_NoInputs);

                    float plainsColor[] = { 0.5f, 0.8f, 0.0f };
                    ImGui::ColorEdit3("Plains", plainsColor, ImGuiColorEditFlags_NoInputs);

                    float tropicalColor[] = { 0.0f, 0.5f, 0.0f };
                    ImGui::ColorEdit3("Tropical", tropicalColor, ImGuiColorEditFlags_NoInputs);

                    float desertColor[] = { 1.0f, 1.0f, 0.0f };
                    ImGui::ColorEdit3("Desert", desertColor, ImGuiColorEditFlags_NoInputs);

                    float marineColor[] = { 0.0f, 0.0f, 1.0f };
                    ImGui::ColorEdit3("Marine", marineColor, ImGuiColorEditFlags_NoInputs);
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