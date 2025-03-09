#include "imguiutils.h"
#include "imgui.h"
#include "imgui_internal.h"

//--------------------------------------------------------------------------------------
void DrawColoredSquare(const float4 & _color)
{
    float f3Color[] = { _color.r, _color.g,  _color.b };

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::ColorEdit3("", f3Color, ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine();
    ImGui::PopItemFlag();
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() - ImGui::GetFrameHeight() - ImGui::GetStyle().ItemSpacing.x);
}

//--------------------------------------------------------------------------------------
bool DrawColoredCheckbox(const float4 & _color, bool * _checked)
{
    bool changed = false;

    ImVec4 imColor = ImVec4(_color.r, _color.g,  _color.b, 1.0f);
    ImVec4 imColorHovered = ImVec4(_color.r * 1.1f, _color.g *1.1f, _color.b * 1.1f, 1.0f);
    ImVec4 imColorActive = ImVec4(_color.r * 1.3f, _color.g *1.3f, _color.b * 1.3f, 1.0f);

    const bool checked = *_checked;

    if (checked)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, imColor); 
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, imColorHovered); 
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, imColorActive); 
    }

    if (ImGui::Checkbox(fmt::sprintf("###0x%08p", (void*)_checked).c_str(),_checked))
        changed = true;

    if (checked)
        ImGui::PopStyleColor(3); // Restore original colors

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() - ImGui::GetFrameHeight() - ImGui::GetStyle().ItemSpacing.x);

    return changed;
}

static vector<bool> g_disabledStack;
static float g_backupAlpha;

//--------------------------------------------------------------------------------------
void ApplyDisabledStyle(bool _disabled)
{
    if (_disabled)
        GImGui->Style.Alpha = g_backupAlpha * 0.5f;
    else
        GImGui->Style.Alpha = g_backupAlpha;
}

//--------------------------------------------------------------------------------------
void PushDisabled(bool _disabled)
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, _disabled);

    if (g_disabledStack.size() == 0)
        g_backupAlpha = GImGui->Style.Alpha;

    ApplyDisabledStyle(_disabled);
    g_disabledStack.push_back(_disabled);
}

//--------------------------------------------------------------------------------------
void PopDisabled()
{
    if (g_disabledStack.size() > 0)
    {
        g_disabledStack.pop_back();
        bool disabled = g_disabledStack.size() > 0 && g_disabledStack.back();
        ApplyDisabledStyle(disabled);
    }
    ImGui::PopItemFlag();
}