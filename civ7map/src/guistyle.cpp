#include "guistyle.h"
#include "imgui.h" 

using namespace ImGui;
using namespace std;

//--------------------------------------------------------------------------------------
string getFixedSizeString(const string & _string, u32 _size)
{
    const auto spaces = max(0,(int)_size - (int)_string.length());
    string result = _string;
    result.append(spaces, ' ');
    return result;
}

float g_globalScale = 1.0f;

//--------------------------------------------------------------------------------------
void SetupImGuiStyle()
{
    ImGui::StyleColorsLight();

    ImGuiStyle & style = ImGui::GetStyle();
    ImVec4 * colors = style.Colors;

    float rounding = 3.0f;

    style.PopupRounding = rounding;
    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(4, 4);
    style.ItemSpacing = ImVec2(4, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.ScrollbarSize = 16;
    style.IndentSpacing = 16;
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 0;
    style.PopupBorderSize = 0;
    style.FrameBorderSize = 0;
    style.WindowRounding = rounding;
    style.ChildRounding = rounding;
    style.FrameRounding = rounding;
    style.ScrollbarRounding = rounding;
    style.GrabRounding = rounding;
    style.Alpha = 0.85f;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = 0;
    style.TabRounding = rounding;
#endif

    float scale = g_globalScale;

    //style.WindowBorderSize *= scale;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    //style.WindowMinSize.x *= scale;  
    //style.WindowMinSize.y *= scale;            // Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
    //style.ChildBorderSize *= scale;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    //style.PopupBorderSize *= scale;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    //style.FrameBorderSize *= scale;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    //style.ScrollbarSize *= scale;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    //style.GrabMinSize *= scale;                // Minimum width/height of a grab box for slider/scrollbar.
    //style.TabBorderSize *= scale;              // Thickness of border around tabs.

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = scale;
    
    //colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.75f);
    //colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    //colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    //colors[ImGuiCol_ChildBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    //colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);
    //colors[ImGuiCol_Border] = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);
    //colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    //colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    //colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    //colors[ImGuiCol_FrameBgActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //colors[ImGuiCol_TitleBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    //colors[ImGuiCol_TitleBgActive] = ImVec4(0.23f, 0.34f, 0.54f, 1.00f);
    //colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.13f, 0.13f, 0.15f, 0.50f);
    //colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);
    //colors[ImGuiCol_ScrollbarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    //colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    //colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    //colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    //colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    //colors[ImGuiCol_SliderGrabActive] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_Button] = ImVec4(0.23f, 0.34f, 0.54f, 0.77f);
    //colors[ImGuiCol_ButtonHovered] = ImVec4(0.23f, 0.34f, 0.54f, 1.00f);
    //colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_Header] = ImVec4(0.23f, 0.34f, 0.54f, 0.77f);
    //colors[ImGuiCol_HeaderHovered] = ImVec4(0.23f, 0.34f, 0.54f, 1.00f);
    //colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_Separator] = ImVec4(0.23f, 0.34f, 0.54f, 0.77f);
    //colors[ImGuiCol_SeparatorHovered] = ImVec4(0.23f, 0.34f, 0.54f, 1.00f);
    //colors[ImGuiCol_SeparatorActive] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_ResizeGrip] = ImVec4(0.23f, 0.34f, 0.54f, 0.50f);
    //colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.23f, 0.34f, 0.54f, 0.77f);
    //colors[ImGuiCol_ResizeGripActive] = ImVec4(0.33f, 0.42f, 0.60f, 0.77f);
    //colors[ImGuiCol_Tab] = ImVec4(0.33f, 0.42f, 0.60f, 0.50f);
    //colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.42f, 0.60f, 1.00f);
    //colors[ImGuiCol_TabUnfocused] = ImVec4(0.54f, 0.54f, 0.54f, 0.19f);
    //colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.54f, 0.54f, 0.54f, 0.38f);
    //colors[ImGuiCol_DockingPreview] = ImVec4(0.20f, 0.29f, 0.43f, 0.77f);
    //colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.23f, 0.36f, 0.50f);
    //colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    //colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    //colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    //colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    //colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    //colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    //colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    //colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    //colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    //colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    //colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    //colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    //colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    //colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    //colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}