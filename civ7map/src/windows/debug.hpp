#include "BaseWindow.h"

//--------------------------------------------------------------------------------------
class DebugWindow : public BaseWindow
{
public:
    DebugWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
DebugWindow::DebugWindow() :
    BaseWindow(ICON_FA_BUG_SLASH" Debug")
{

}

//--------------------------------------------------------------------------------------
bool DebugWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_BUG_SLASH" Debug###Debug", &m_visible))
    {
        if (ImGui::Button("Refresh") ||sf::Keyboard::isKeyPressed(Keyboard::F7))
        {
            ShaderManager::deinit();
            ShaderManager::init();
            needRefresh = true;
        }

        ImGui::Separator();

        char temp[4096] = "";

        if (g_map)
            sprintf_s(temp, "%s", GetFilename(g_map->m_mapPath).c_str());
        ImGui::InputText("Map file", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        if (g_map)
            sprintf_s(temp, "%s", GetFilename(g_map->m_mapDataPath).c_str());
        ImGui::InputText("Data file", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        if (g_map)
            sprintf_s(temp, "%s", GetFolder(g_map->m_mapPath).c_str());
        ImGui::InputText("Mod folder", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        ImGui::TreeNodeEx("Mouse", ImGuiTreeNodeFlags_DefaultOpen);
        {
            ImGui::Columns(2, "mycolumns2", false);
            {
                ImGui::Text("Position");
                ImGui::Text("Buttons");
                ImGui::Text("Wheel");
            }

            ImGui::NextColumn();
            {
                const auto & mousePos = sf::Mouse::getPosition(window);
                ImGui::Text((to_string(mousePos.x) + ", " + to_string(mousePos.y)).c_str());

                string mouseButtonsString = "";
                if (sf::Mouse::isButtonPressed(Mouse::Left))
                    mouseButtonsString += "Left ";

                if (sf::Mouse::isButtonPressed(Mouse::Middle))
                    mouseButtonsString += "Middle ";

                if (sf::Mouse::isButtonPressed(Mouse::Right))
                    mouseButtonsString += "Right ";

                ImGui::Text(mouseButtonsString.c_str());

                if (g_map)
                    ImGui::Text((to_string((int)g_map->m_mouseWheelDelta)).c_str());
            }

            ImGui::TreePop();
        }

        ImGui::Columns(1);

        if (TreeNodeEx("View", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Columns(2, "mycolumns2", false);
            {
                ImGui::Text("HasFocus");
                ImGui::Text("Offset");
                ImGui::Text("Zoom");
            }

            NextColumn();
            {
                ImGui::Text(g_hasFocus ? "true" : "false");

                if (g_map)
                {
                    char tmp[256];
                    sprintf_s(tmp, "%.1f, %.1f", g_map->m_cameraOffset.x, g_map->m_cameraOffset.y);
                    ImGui::Text(tmp);

                    sprintf_s(tmp, "%.1f", g_map->m_cameraZoom);
                    ImGui::Text(tmp);
                }
            }

            Columns(1);

            TreePop();
        }
    }

    ImGui::End();

    return needRefresh;
}