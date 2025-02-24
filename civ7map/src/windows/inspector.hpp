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
        
        if (g_map && g_selectedCell.x < g_map->m_width && g_selectedCell.y < g_map->m_height)
            tile = &g_map->civ7TerrainType.get(g_selectedCell.x, g_selectedCell.y);

        ImGui::BeginTable("TileTable", 2, ImGuiTableFlags_Resizable);
        {
            ImGui::TableNextColumn();

            if (!tile)
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

            ImGui::Text("Selected");

            const int offset = 7;

            ImGui::SetCursorPosY(GetCursorPosY() + offset);
            ImGui::Text("Continent");

            ImGui::SetCursorPosY(GetCursorPosY() + offset);
            ImGui::Text("Terrain");

            ImGui::SetCursorPosY(GetCursorPosY() + offset);
            ImGui::Text("Biome");

            ImGui::SetCursorPosY(GetCursorPosY() + offset);
            ImGui::Text("Feature");

            ImGui::TableNextColumn();

            if (tile)
            {
                //ImGui::Text("%i,%i", g_selectedCell.x, g_selectedCell.y);
                ImGui::InputInt2("###SelectedCell", (int*)&g_selectedCell, ImGuiInputTextFlags_EnterReturnsTrue);

                // Continent
                {
                    float4 color = getContinentColor(tile->continent);
                    float f3Color[] = { color.r, color.g,  color.b };
                    string continentName = g_map->getContinentShortName(tile->continent);

                    int selectedIndex = -1;
                    if (ImGui::BeginCombo("###Continent", continentName.c_str()))
                    {
                        for (uint i = 0; i < g_map->getContinentCount(); ++i)
                        {
                            bool isSelected = (selectedIndex == i);
                            if (ImGui::Selectable(g_map->getContinentShortName((ContinentType)i).c_str(), isSelected))
                            {
                                selectedIndex = i; 
                                tile->continent = (ContinentType)i;
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
                    if (ImGui::BeginCombo("###Terrain", asString(tile->terrain).c_str()))
                    {
                        for (uint i = 0; i < enumCount<TerrainType>(); ++i)
                        {
                            bool isSelected = (selectedIndex == i);
                            if (ImGui::Selectable(asString((TerrainType)i).c_str(), isSelected))
                            {
                                selectedIndex = i; 
                                tile->terrain = (TerrainType)i;
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
                    if (ImGui::BeginCombo("###Biome", asString(tile->biome).c_str()))
                    {
                        for (uint i = 0; i < enumCount<BiomeType>(); ++i)
                        {
                            bool isSelected = (selectedIndex == i);
                            if (ImGui::Selectable(asString((BiomeType)i).c_str(), isSelected))
                            {
                                selectedIndex = i;
                                tile->biome = (BiomeType)i;
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
                    if (ImGui::BeginCombo("###Biome", asString(tile->biome).c_str()))
                    {
                        for (uint i = 0; i < enumCount<FeatureType>(); ++i)
                        {
                            bool isSelected = (selectedIndex == i);
                            if (ImGui::Selectable(asString((FeatureType)i).c_str(), isSelected))
                            {
                                selectedIndex = i;
                                tile->feature = (FeatureType)i;
                            }
                        }

                        ImGui::EndCombo();
                    }
                }
            }

            if (!tile)
                ImGui::PopItemFlag();

        }
        ImGui::EndTable();

        
        //ImGui::Separator();
    }

    ImGui::End();

    return needRefresh;
}