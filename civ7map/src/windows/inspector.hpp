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

            ImGui::SetCursorPosY(GetCursorPosY() + 4);
            ImGui::Text("Continent");

            ImGui::SetCursorPosY(GetCursorPosY() + 4);
            ImGui::Text("Terrain");

            ImGui::SetCursorPosY(GetCursorPosY() + 4);
            ImGui::Text("Biome");

            ImGui::SetCursorPosY(GetCursorPosY() + 4);
            ImGui::Text("Feature");

            ImGui::TableNextColumn();

            if (tile)
            {
                ImGui::Text("%i,%i", g_selectedCell.x, g_selectedCell.y);

                float4 contiColor = getContinentColor(tile->continent);
                float f3ContiColor[] = { contiColor.r, contiColor.g,  contiColor.b };
                string continentName = g_map->getContinentShortName(tile->continent);

                int selectedIndex = -1;
                if (ImGui::BeginCombo("###SelectContinent", continentName.c_str()))
                {
                    for (uint i = 0; i < g_map->getContinentCount(); ++i)
                    {
                        bool isSelected = (selectedIndex == i);
                        if (ImGui::Selectable(g_map->getContinentShortName((ContinentType)i).c_str(), isSelected))
                        {
                            selectedIndex = i; // Update selected index
                            tile->continent = (ContinentType)i;
                        }
                    }

                    ImGui::EndCombo();
                }

                //int selectedIndex = -1;
                //if (ImGui::BeginPopup("custom_combo"))
                //{
                //    for (int i = 0; i < g_map->getContinentCount(); i++)
                //    {
                //        ImGui::PushID(i);
                //
                //        if (ImGui::Selectable(g_map->getContinentShortName((ContinentType)i).c_str(), selectedIndex == i))
                //        {
                //            selectedIndex = i;
                //            //previewTexture = textures[i]; // Update preview texture
                //            ImGui::CloseCurrentPopup();
                //        }
                //
                //        // Show corresponding image next to the option
                //        ImGui::SameLine();
                //        //ImGui::Image(textures[i], ImVec2(32, 32));
                //
                //        ImGui::PopID();
                //    }
                //    ImGui::EndPopup();
                //}

                //ImGui::ColorEdit3(-1 != (int)tile->continent ? continentName.c_str() : "None", f3ContiColor, ImGuiColorEditFlags_NoInputs);
            
                float4 biomeColor = getBiomeColor(tile->biome);
                float f3BiomeColor[] = { biomeColor.r, biomeColor.g,  biomeColor.b };
                ImGui::ColorEdit3(asString(tile->biome).c_str(), f3BiomeColor, ImGuiColorEditFlags_NoInputs);

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