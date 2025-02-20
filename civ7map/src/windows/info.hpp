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
    BaseWindow("Info")
{

}


//--------------------------------------------------------------------------------------
bool InfoWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Infos", &m_visible) && g_map)
    {
        char temp[256];
        sprintf_s(temp, g_map->author.c_str());
        ImGui::InputText("Author", temp, 256, ImGuiInputTextFlags_ReadOnly);

        if (TreeNodeEx("Size", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int editMapSize[2] =
            {
                (int)g_map->width,
                (int)g_map->height
            };

            const bool editX = ImGui::InputInt("horizontal", &editMapSize[0], 1, 100, ImGuiInputTextFlags_ReadOnly);
            const bool editY = ImGui::InputInt("vertical", &editMapSize[1], 1, 100, ImGuiInputTextFlags_ReadOnly);

            if (editX || editY)
            {

            }

            TreePop();
        }

        if (TreeNodeEx("Offset", ImGuiTreeNodeFlags_DefaultOpen))
        {
            int editMapOffset[2] =
            {
                g_map->mapOffset[0].x, g_map->mapOffset[0].y
            };

            const bool editX = ImGui::InputInt("horizontal", &editMapOffset[0]);
            const bool editY = ImGui::InputInt("vertical", &editMapOffset[1]);

            if (editX || editY)
            {
                g_map->mapOffset[0].x = editMapOffset[0];
                g_map->mapOffset[0].y = editMapOffset[1];

                Vector2i delta = g_map->mapOffset[0] - g_map->mapOffset[1];

                g_map->translate(delta);

                g_map->mapOffset[1] = g_map->mapOffset[0];

                g_map->refresh();
            }

            TreePop();
        }

        if (TreeNodeEx("Options", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox(getFixedSizeString("Map cycling", g_fixedTextLengthShort).c_str(), &g_map->useMapCycling);
            ImGui::Checkbox(getFixedSizeString("Procedural mountain chains", g_fixedTextLengthShort).c_str(), &g_map->useProceduralMountainChains);
            TreePop();
        }
    }

    ImGui::End();

    return needRefresh;
}