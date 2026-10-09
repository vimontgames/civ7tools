#include "imguiutils.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "tile/civ7tile.h"
#include "shader/colors.h"
#include "map/map.h"

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
void DrawSmallColoredSquare(const float4 & _color)
{
    const float size = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y;

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size, pos.y + size), ImGui::ColorConvertFloat4ToU32(ImVec4(_color.r, _color.g, _color.b, _color.a)));
    ImGui::Dummy(ImVec2(size, size));
    ImGui::SameLine();
    ImGui::PopItemFlag();
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

//--------------------------------------------------------------------------------------
template<typename E> struct EditEnumTraits;

//--------------------------------------------------------------------------------------
template<typename E> bool EditEnum(const Map * map, E & value, bool * pBool)
{
    const string label = EditEnumTraits<E>::GetLabel();

    auto drawItem = [&](E itemToSelect)
    {
        bool dirty = false;
        bool isSelected = (value == itemToSelect);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float height = ImGui::GetTextLineHeight();

        if (ImGui::Selectable(fmt::sprintf("##Edit%s%i", EditEnumTraits<E>::GetLabel(), (int)itemToSelect).c_str(), isSelected, 0, ImVec2(ImGui::GetContentRegionAvail().x, height)))
            dirty = true;

        ImGui::SameLine(0, 0);
        ImGui::SetCursorScreenPos(ImVec2(pos.x + 4, pos.y));
        DrawSmallColoredSquare(EditEnumTraits<E>::GetColor(itemToSelect));

        ImGui::SameLine();
        ImGui::Text("%s (%i)", EditEnumTraits<E>::GetName(map, itemToSelect).c_str(), (int)itemToSelect);
        return dirty;
    };

    E previous = value;

    const float comboX = ImGui::GetCursorPosX() + 128;

    if (pBool)
    {
        ImGui::Checkbox(fmt::sprintf("%s###Paint%s", label, label).c_str(), pBool);
        ImGui::SameLine(comboX);
    }

    PushDisabled(pBool && !(*pBool));
    {
        DrawColoredSquare(EditEnumTraits<E>::GetColor(value));

        string name = EditEnumTraits<E>::GetName(map, value);

        if (pBool)
        {
            const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
            ImGui::SetNextItemWidth(comboWidth);
        }

        if (ImGui::BeginCombo((!pBool ? label : "###" + label).c_str(), fmt::sprintf("%s (%i)", name, (int)value).c_str(), ImGuiComboFlags_HeightLargest))
        {
            if constexpr (EditEnumTraits<E>::HasNoneValue)
            {
                if (drawItem(E::None))
                    value = E::None;
            }

            if (map)
            {
                for (uint i = 0; i < EditEnumTraits<E>::GetCount(map); ++i)
                {
                    if (drawItem((E)i))
                        value = (E)i;
                }
            }

            ImGui::EndCombo();
        }
    }
    PopDisabled();

    return value != previous;
}

//--------------------------------------------------------------------------------------
string GetContinentShortName(const Map * map, ContinentType continent)
{
    return map ? map->getContinentShortName(continent).c_str() : "None";
}

//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<ContinentType>
{
    static constexpr bool HasNoneValue = true;

    static auto GetColor(ContinentType value) { return getContinentColor(value); }
    static string GetName(const Map * map, ContinentType value) { return GetContinentShortName(map, value); }
    static uint GetCount(const Map * map) { return map->getContinentCount(); }
    static const string GetLabel() { return "Continent"; }
};

//--------------------------------------------------------------------------------------
bool EditContinent(const Map * map, ContinentType & continent, bool * pBool)
{
    return EditEnum<ContinentType>(map, continent, pBool);
}

//--------------------------------------------------------------------------------------
string GetLandmassShortName(const Map * map, LandmassType landmass)
{
    return map && landmass < map->getLandmassCount() ? map->getLandmassShortName(landmass) : "Landmass 0";
}

template<> struct EditEnumTraits<LandmassType>
{
    static constexpr bool HasNoneValue = false;

    static auto GetColor(LandmassType value) { return getLandmassColor(value); }
    static string GetName(const Map * map, LandmassType value) { return GetLandmassShortName(map, value); }
    static uint GetCount(const Map * map) { return map->getLandmassCount(); }
    static const string GetLabel() { return "Landmass"; }
};

//--------------------------------------------------------------------------------------
bool EditLandmass(const Map * map, LandmassType & landmass, bool * pBool)
{
    return EditEnum<LandmassType>(map, landmass, pBool);
}
