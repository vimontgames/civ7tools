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
        ImGui::Columns(2, "mycolumns2", false);
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
    ImGui::End();

    return needRefresh;
}