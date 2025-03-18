#include "BaseWindow.h"

//--------------------------------------------------------------------------------------
class HelpWindow : public BaseWindow
{
public:
    HelpWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
HelpWindow::HelpWindow() :
    BaseWindow(ICON_FA_CIRCLE_QUESTION" Help")
{

}

//--------------------------------------------------------------------------------------
bool HelpWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_CIRCLE_QUESTION" Help###Help", &m_visible))
    {
        if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Columns(2, "MouseColumns", false);
            {
                SetColumnWidth(0, 96.0f);

                ImGui::Text("Select");
                ImGui::Text("Paint");
                ImGui::Text("Pan");
                ImGui::Text("Zoom");
            }
            ImGui::NextColumn();
            {
                ImGui::Text("Space");
                ImGui::Text("Left mouse button");
                ImGui::Text("Right mouse button");
                ImGui::Text("Mouse wheel");
            }
        }

        ImGui::Columns(1);

        if (ImGui::CollapsingHeader("Keyboard", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Columns(2, "KeyboardColumns", false);
            {
                SetColumnWidth(0, 96.0f);

                ImGui::Text("Shaders");
                ImGui::Text("Refresh");
            }
            ImGui::NextColumn();
            {
                ImGui::Text("F6");
                ImGui::Text("F7");
            }
        }
    }
    ImGui::End();

    return needRefresh;
}