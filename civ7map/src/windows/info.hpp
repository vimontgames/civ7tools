#include "BaseWindow.h"
#include "misc\imguiutils.h"

//--------------------------------------------------------------------------------------
class InfoWindow : public BaseWindow
{
public:
    InfoWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
InfoWindow::InfoWindow() :
    BaseWindow(ICON_FA_GLOBE" Map")
{

}


//--------------------------------------------------------------------------------------
bool InfoWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_GLOBE " Map###Map", &m_visible) && g_map)
    {
        char temp[4096];

        ImGui::PushItemWidth(ImGui::GetFontSize() * 10.0f); 

        sprintf_s(temp, "%s", g_map->getPrettyName().c_str());
        ImGui::InputText("Name", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);
        
        sprintf_s(temp, "%s-XXX", g_map->getBaseName().c_str());
        ImGui::InputText("Files", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        if (ImGui::CollapsingHeader("Map size", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            if (ImGui::InputInt2("Offset", g_map->m_editMapOffset, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_mapOffset[0].x = g_map->m_editMapOffset[0];
                g_map->m_mapOffset[0].y = g_map->m_editMapOffset[1];

                Vector2i delta = g_map->m_mapOffset[0] - g_map->m_mapOffset[1];

                g_map->translate(delta);
                g_map->m_mapOffset[1] = g_map->m_mapOffset[0];
                g_map->refresh();
            }

            bool isValidMapSize = g_map->getMapSize(g_map->m_editMapSize[0], g_map->m_editMapSize[1]) != MapSize::Custom;

            ImGui::InputInt2("Size", g_map->m_editMapSize, ImGuiInputTextFlags_EnterReturnsTrue);

            ImGui::SameLine();

            const bool editing = g_map->m_width != g_map->m_editMapSize[0] || g_map->m_height != g_map->m_editMapSize[1];
            PushDisabled(!editing);
            {
                if (ImGui::Button(ICON_FA_CROP))
                {
                    g_map->crop(sf::Vector2i(g_map->m_editMapSize[0], g_map->m_editMapSize[1]));
                    g_map->refresh();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Crop");

                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_MAXIMIZE))
                {
                    g_map->rescale(sf::Vector2i(g_map->m_editMapSize[0], g_map->m_editMapSize[1]));
                    g_map->refresh();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Rescale");
            }
            PopDisabled();

            ImGui::SameLine();
            if (!isValidMapSize)
                ImGui::TextColored(ImVec4(1, 0.5, 0, 1), ICON_FA_TRIANGLE_EXCLAMATION);
            else
                ImGui::TextColored(ImVec4(0, 1, 0, 1), ICON_FA_CHECK);

            if (ImGui::IsItemHovered())
            {
                string invalidMapSizeMsg = isValidMapSize ? "Valid map sizes:\n" : "Please use a valid map size!\n";
                for (auto val : enumValues<MapSize>())
                {
                    if (val.first == MapSize::Custom)
                        continue;
                    invalidMapSizeMsg += fmt::sprintf("- %s (%ix%i)\n", asString(val.first), g_mapSizes[(int)val.first][0], g_mapSizes[(int)val.first][1]);
                }
                ImGui::SetTooltip(invalidMapSizeMsg.c_str());
            }                
        }

        if (ImGui::CollapsingHeader("Hemispheres", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            int editWest[2] =
            {
                (int)g_map->m_westStart,
                (int)g_map->m_westEnd
            };
            if (ImGui::InputInt2("West", editWest, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_westStart = editWest[0];
                g_map->m_westEnd = editWest[1];
                g_map->fixHemispheres();
            }

            int editEast[2] =
            {
                (int)g_map->m_eastStart,
                (int)g_map->m_eastEnd
            };
            if (ImGui::InputInt2("East", editEast, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_eastStart = editEast[0];
                g_map->m_eastEnd = editEast[1];
                g_map->fixHemispheres();
            }
        }  

        ImGui::PopItemWidth();
    }

    ImGui::End();

    return needRefresh;
}