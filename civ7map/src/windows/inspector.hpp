#include "BaseWindow.h"

//--------------------------------------------------------------------------------------
class InspectorWindow : public BaseWindow
{
public:
    InspectorWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
InspectorWindow::InspectorWindow() :
    BaseWindow("Tile")
{

}

//--------------------------------------------------------------------------------------
bool InspectorWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Inspector", &m_visible))
    {
        Civ7Tile * tile = nullptr;
        Map * map = g_map;
        
        if (map && (uint)g_selectedCell.x < map->m_width && (uint)g_selectedCell.y < map->m_height)
            tile = &map->m_civ7TerrainType.get(g_selectedCell.x, g_selectedCell.y);

        if (!tile)
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        bool dirty = false;

        if (tile)
        {
            ImGui::InputInt2("Plot", (int*)&g_selectedCell, ImGuiInputTextFlags_EnterReturnsTrue);

            int engineLoc[2] =
            {
                g_selectedCell.x,
                g_selectedCell.y + 2
            };
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::InputInt2("Engine Plot", engineLoc, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemFlag();

            // Continent
            {
                float4 color = getContinentColor(tile->continent);
                float f3Color[] = { color.r, color.g,  color.b };
                string continentName = map->getContinentShortName(tile->continent);

                int selectedIndex = -2;
                if (ImGui::BeginCombo("Continent", fmt::sprintf("%s (%i)", continentName, (int)tile->continent).c_str()))
                {
                    // None
                    {
                        bool isSelected = (selectedIndex == -1);
                        if (ImGui::Selectable(fmt::sprintf("%s (-1)", map->getContinentShortName((ContinentType)-1)).c_str(), isSelected))
                        {
                            tile->continent = (ContinentType)-1;
                            dirty = true;
                        }
                    }
                    for (uint i = 0; i < map->getContinentCount(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", map->getContinentShortName((ContinentType)i), i).c_str(), isSelected))
                        {
                            tile->continent = (ContinentType)i;
                            dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // TerrainType
            {
                float4 color = getTerrainColor(tile->terrain);
                float f3Color[] = { color.r, color.g,  color.b };

                int selectedIndex = -2;
                if (ImGui::BeginCombo("Terrain", fmt::sprintf("%s (%i)", asString(tile->terrain), (int)tile->terrain).c_str()))
                {
                    for (auto val : enumValues<TerrainType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = (selectedIndex == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            selectedIndex = index;
                            tile->terrain = val.first;
                            dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Biome
            {
                float4 color = getBiomeColor(tile->biome);
                float f3Color[] = { color.r, color.g,  color.b };

                int selectedIndex = -2;
                if (ImGui::BeginCombo("Biome", fmt::sprintf("%s (%i)", asString(tile->biome), (int)tile->biome).c_str()))
                {
                    for (auto val : enumValues<BiomeType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = (selectedIndex == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            selectedIndex = index;
                            tile->biome = val.first;
                            dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Feature
            {
                float4 color = getFeatureColor(tile->feature);
                float f3Color[] = { color.r, color.g,  color.b };

                int selectedIndex = -2;
                if (ImGui::BeginCombo("Feature", fmt::sprintf("%s (%i)", asString(tile->feature), (int)tile->feature).c_str()))
                {
                    for (auto val : enumValues<FeatureType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = (selectedIndex == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            selectedIndex = index;
                            tile->feature = val.first;
                            dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Resource
            {
                float4 color = getResourceColor(tile->resource);
                float f3Color[] = { color.r, color.g,  color.b };

                int selectedIndex = -2;
                if (ImGui::BeginCombo("Resource", fmt::sprintf("%s (%i)", asString(tile->resource), (int)tile->resource).c_str()))
                {
                    for (auto val : enumValues<ResourceType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = (selectedIndex == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            selectedIndex = index;
                            tile->resource = val.first;
                            dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
        }

        if (dirty)
            map->refresh();

        if (!tile)
            ImGui::PopItemFlag();
    }

    ImGui::End();

    return needRefresh;
}