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
            tile = &map->civ7TerrainType.get(g_selectedCell.x, g_selectedCell.y);

        if (!tile)
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        bool dirty = false;

        if (tile)
        {
            //ImGui::Text("%i,%i", g_selectedCell.x, g_selectedCell.y);
            ImGui::InputInt2("Selected", (int*)&g_selectedCell, ImGuiInputTextFlags_EnterReturnsTrue);

            // Continent
            {
                float4 color = getContinentColor(tile->continent);
                float f3Color[] = { color.r, color.g,  color.b };
                string continentName = map->getContinentShortName(tile->continent);

                int selectedIndex = -1;
                if (ImGui::BeginCombo("Continent", continentName.c_str()))
                {
                    // None
                    {
                        bool isSelected = (selectedIndex == -1);
                        if (ImGui::Selectable(map->getContinentShortName((ContinentType)-1).c_str(), isSelected))
                        {
                            selectedIndex = -1;
                            tile->continent = (ContinentType)-1;
                            dirty = true;
                        }
                    }
                    for (uint i = 0; i < map->getContinentCount(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(map->getContinentShortName((ContinentType)i).c_str(), isSelected))
                        {
                            selectedIndex = i; 
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

                int selectedIndex = -1;
                if (ImGui::BeginCombo("TerrainType", asString(tile->terrain).c_str()))
                {
                    for (uint i = 0; i < enumCount<TerrainType>(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(asString((TerrainType)i).c_str(), isSelected))
                        {
                            selectedIndex = i; 
                            tile->terrain = (TerrainType)i;
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

                int selectedIndex = -1;
                if (ImGui::BeginCombo("Biome", asString(tile->biome).c_str()))
                {
                    for (uint i = 0; i < enumCount<BiomeType>(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(asString((BiomeType)i).c_str(), isSelected))
                        {
                            selectedIndex = i;
                            tile->biome = (BiomeType)i;
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

                int selectedIndex = -1;
                if (ImGui::BeginCombo("Feature", asString(tile->biome).c_str()))
                {
                    for (uint i = 0; i < enumCount<FeatureType>(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(asString((FeatureType)i).c_str(), isSelected))
                        {
                            selectedIndex = i;
                            tile->feature = (FeatureType)i;
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