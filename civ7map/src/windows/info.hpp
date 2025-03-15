#include "BaseWindow.h"

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

        sprintf_s(temp, "%s", g_map->getPrettyName().c_str());
        ImGui::InputText("Name", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);
        
        sprintf_s(temp, "%s-XXX", g_map->getBaseName().c_str());
        ImGui::InputText("Files", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        //sprintf_s(temp, "%s", GetFilename(g_map->m_mapDataPath).c_str());
        //ImGui::InputText("Data", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        //sprintf_s(temp, "%s", GetFolder(g_map->m_path).c_str());
        //ImGui::InputText("Folder", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        //sprintf_s(temp, g_map->m_author.c_str());
        //ImGui::InputText("Author", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        //if (TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int editMapSize[2] =
            {
                (int)g_map->m_width,
                (int)g_map->m_height
            };

            if (ImGui::InputInt2("Width", editMapSize, ImGuiInputTextFlags_EnterReturnsTrue))
            {

            }

            //TreePop();
        }

        //if (TreeNodeEx("Offset", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int editMapOffset[2] =
            {
                g_map->m_mapOffset[0].x, g_map->m_mapOffset[0].y
            };

            if (ImGui::InputInt2("Offset", editMapOffset, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_mapOffset[0].x = editMapOffset[0];
                g_map->m_mapOffset[0].y = editMapOffset[1];

                Vector2i delta = g_map->m_mapOffset[0] - g_map->m_mapOffset[1];

                g_map->translate(delta);

                g_map->m_mapOffset[1] = g_map->m_mapOffset[0];

                g_map->refresh();
            }

            //TreePop();
        }     
    }

    ImGui::End();

    return needRefresh;
}