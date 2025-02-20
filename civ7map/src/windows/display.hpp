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
            PushItemWidth(g_comboxItemWidth);
            needRefresh |= Combo("Filter", (int *)&g_map->territoryBackground, "None\0Tile\0Territories\0Biomes\0Landmarks\0Wonders\0\0");
            PopItemWidth();

            needRefresh |= Checkbox(getFixedSizeString("Borders", g_fixedTextLengthShort).c_str(), &g_map->showTerritoriesBorders);

            ImGui::SameLine();
            needRefresh |= Checkbox(getFixedSizeString("Hexes", g_fixedTextLengthShort).c_str(), &g_map->useHexUVs);

            g_map->bitmaps[Territories].visible = g_map->territoryBackground != TerritoryBackground::None;

            TreePop();
        }

        if (ImGui::TreeNodeEx("Resources", ImGuiTreeNodeFlags_DefaultOpen))
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

            g_map->bitmaps[Resources].visible = g_map->showStrategicResources || g_map->showLuxuryResources || g_map->showWonders || g_map->showSpawnPoints;

            ImGui::TreePop();
        }
    }
    ImGui::End();

    return needRefresh;
}