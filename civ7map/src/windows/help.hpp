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
    BaseWindow("Help")
{

}

//--------------------------------------------------------------------------------------
bool HelpWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Help", &m_visible))
    {
        ImGui::Columns(2, "mycolumns2", false);
        {
            SetColumnWidth(0, 96.0f);

            ImGui::Text("Pan");
            ImGui::Text("Zoom");
        }
        ImGui::NextColumn();
        {
            ImGui::Text("Left Mouse Button");
            ImGui::Text("Mouse Wheel");
        }
    }
    ImGui::End();

    return needRefresh;
}