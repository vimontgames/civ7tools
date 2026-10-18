#include "imguiutils.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "tile/civ7tile.h"
#include "shader/colors.h"
#include "map/map.h"
#include "EditorOptions.h"

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
template<typename E> struct EditEnumTraitsBase
{
    static constexpr bool AddNoneValue = false;
    static constexpr bool UseIntSlider = false;
    static constexpr auto MaxValue = (uint)-1;

    static auto GetColor(E value) { return ImVec4(1, 1, 1, 1); }
    static string GetName(const Map * map, E value) { return SeparateCapitalizedWords(asString(value)); }
    static uint GetCount(const Map * map) { return enumCount<E>(); }
    static const string GetToolTip(E value) { return ""; }
    static const auto GetEditStep() { return 1; }
};

template<typename E> struct EditEnumTraits : EditEnumTraitsBase<E> {};

//--------------------------------------------------------------------------------------
template<typename E> bool EditEnum(const string & label, const Map * map, E & value, bool * pBool)
{
    E previous = value;

    const float comboX = ImGui::GetCursorPosX() + 128 * EditorOptions::Get().m_editorGlobalScale;

    if (pBool)
    {
        bool disabled = !*pBool;
        if (disabled)
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
        ImGui::Checkbox(fmt::sprintf("%s###Paint%s", label, label).c_str(), pBool);

        if (disabled)
            ImGui::PopStyleColor();

        ImGui::SameLine(comboX);
    }

    PushDisabled(pBool && !(*pBool));
    {
        DrawColoredSquare(EditEnumTraits<E>::GetColor(value));

        if (pBool)
        {
            const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
            ImGui::SetNextItemWidth(comboWidth);
        }

        if constexpr (EditEnumTraits<E>::UseIntSlider)
        {
            const int stepValue = EditEnumTraits<E>::GetEditStep();
            const int maxValue = EditEnumTraits<E>::MaxValue;

            int step = ((int)value + stepValue / 2) / stepValue;
            char format[32];
            sprintf_s(format, "%d", step * stepValue);

            if (ImGui::SliderInt((!pBool ? label : "###" + label).c_str(), &step, 0, maxValue / stepValue, format))
            {
                value = (E)(step * stepValue);
            }
        }
        else
        {
            auto drawItem = [&](E itemToSelect)
            {
                bool dirty = false;
                bool isSelected = (value == itemToSelect);

                ImVec2 pos = ImGui::GetCursorScreenPos();
                float height = ImGui::GetTextLineHeight();

                if (ImGui::Selectable(fmt::sprintf("##Edit%s%i", label, (int)itemToSelect).c_str(), isSelected, 0, ImVec2(ImGui::GetContentRegionAvail().x, height)))
                    dirty = true;

                ImGui::SameLine(0, 0);
                ImGui::SetCursorScreenPos(ImVec2(pos.x + 4, pos.y));
                DrawSmallColoredSquare(EditEnumTraits<E>::GetColor(itemToSelect));

                ImGui::SameLine();
                ImGui::Text("%s", EditEnumTraits<E>::GetName(map, itemToSelect).c_str());

                if (ImGui::IsItemHovered())
                {
                    auto tooltipMsg = EditEnumTraits<E>::GetToolTip(itemToSelect);
                    if (!tooltipMsg.empty())
                        ImGui::SetTooltip(tooltipMsg.c_str());
                }

                return dirty;
            };

            string preview = EditEnumTraits<E>::GetName(map, value);

            if (ImGui::BeginCombo((!pBool ? label : "###" + label).c_str(), fmt::sprintf("%s", preview).c_str(), ImGuiComboFlags_HeightLargest))
            {
                if constexpr (EditEnumTraits<E>::AddNoneValue)
                {
                    if (drawItem(E::None))
                        value = E::None;
                }

                for (uint i = 0; i < EditEnumTraits<E>::GetCount(map); ++i)
                {
                    if (drawItem((E)i))
                        value = (E)i;
                }

                ImGui::EndCombo();
            }
        }
    }
    PopDisabled();

    return value != previous;
}

//--------------------------------------------------------------------------------------
// ContinentType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<ContinentType> : EditEnumTraitsBase<ContinentType>
{
    static constexpr bool AddNoneValue = true;

    static auto GetColor(ContinentType value) { return getContinentColor(value); }
    static string GetName(const Map * map, ContinentType value) { return map ? map->getContinentShortName(value).c_str() : "None"; }
    static uint GetCount(const Map * map) { return map? map->getContinentCount() : 0; }
};

//--------------------------------------------------------------------------------------
bool EditContinent(const Map * map, ContinentType & continent, bool * pBool)
{
    return EditEnum<ContinentType>("Continent", map, continent, pBool);
}

//--------------------------------------------------------------------------------------
// LandmassType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<LandmassType> : EditEnumTraitsBase<LandmassType>
{
    static auto GetColor(LandmassType value) { return getLandmassColor(value); }
    static string GetName(const Map * map, LandmassType value) { return map && (int)value < map->getLandmassCount() ? map->getLandmassShortName(value) : "Landmass 0"; }
    static uint GetCount(const Map * map) { return map? map->getLandmassCount() : 1; }
};

//--------------------------------------------------------------------------------------
bool EditLandmass(const Map * map, LandmassType & landmass, bool * pBool)
{
    return EditEnum<LandmassType>("Landmass", map, landmass, pBool);
}

//--------------------------------------------------------------------------------------
// TerrainType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<TerrainType> : EditEnumTraitsBase<TerrainType>
{
    static auto GetColor(TerrainType value) { return getTerrainColor(value); }
};

//--------------------------------------------------------------------------------------
bool EditTerrain(const Map * map, TerrainType & terrain, bool * pBool)
{
    return EditEnum<TerrainType>("Terrain", map, terrain, pBool);
}

//--------------------------------------------------------------------------------------
// BiomeType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<BiomeType> : EditEnumTraitsBase<BiomeType>
{
    static auto GetColor(BiomeType value) { return getBiomeColor(value); }
};

//--------------------------------------------------------------------------------------
bool EditBiome(const Map * map, BiomeType & biome, bool * pBool)
{
    return EditEnum<BiomeType>("Biome", map, biome, pBool);
}

//--------------------------------------------------------------------------------------
// FeatureType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<FeatureType> : EditEnumTraitsBase<FeatureType>
{
    static auto GetColor(FeatureType value) { return getFeatureColor(value); }
};

//--------------------------------------------------------------------------------------
bool EditFeature(const Map * map, FeatureType & feature, bool * pBool)
{
    return EditEnum<FeatureType>("Feature", map, feature, pBool);
}

//--------------------------------------------------------------------------------------
// ResourceType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<ResourceType> : EditEnumTraitsBase<ResourceType>
{
    static auto GetColor(ResourceType value) { return getResourceColor(value); }
};

//--------------------------------------------------------------------------------------
bool EditResource(const Map * map, ResourceType & resource, bool * pBool)
{
    return EditEnum<ResourceType>("Resource", map, resource, pBool);
}

//--------------------------------------------------------------------------------------
// NaturalWonderType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<NaturalWonderType> : EditEnumTraitsBase<NaturalWonderType>
{
    static auto GetColor(NaturalWonderType value) { return getNaturalWonderColor(value); }
    static const string GetToolTip(NaturalWonderType value)
    { 
        return g_NaturalWonderPlacement[(int)value].description;
    }
};

//--------------------------------------------------------------------------------------
bool EditNaturalWonder(const Map * map, NaturalWonderType & naturalWonder, bool * pBool)
{
    return EditEnum<NaturalWonderType>("Wonder", map, naturalWonder, pBool);
}

//--------------------------------------------------------------------------------------
// Elevation traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<Elevation> : EditEnumTraitsBase<Elevation>
{
    static constexpr bool UseIntSlider = true;
    static constexpr auto MaxValue = maxElevation;

    static auto GetColor(Elevation value) { return getElevationColor(value); }
    static const auto GetEditStep() { return EditorOptions::Get().m_elevationStep; }
};

//--------------------------------------------------------------------------------------
bool EditElevation(const Map * map, Elevation & elevation, bool * pBool)
{
    return EditEnum<Elevation>("Elevation", map, elevation, pBool);
}

//--------------------------------------------------------------------------------------
// RiverName traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<RiverName> : EditEnumTraitsBase<RiverName>
{
    static constexpr bool AddNoneValue = true;
    static auto GetColor(RiverName value) { return getRiverNameColor(value); }
    static uint GetCount(const Map * map) 
    { 
        if (map)
            return (uint)map->getRivers().size();
        else
            return 0;
    }
    static string GetName(const Map * map, RiverName value)
    { 
        if (value == RiverName::None)
            return asString(value);

        if (map)
        {
            const vector<RiverInfo> & rivers = map->getRivers();
            if ((int)value < rivers.size())
            {
                const RiverInfo & river = rivers[(int)value];
                return river.prettyName;
            }
        }
        return "Error";
    }
};

//--------------------------------------------------------------------------------------
bool EditRiverName(const Map * map, RiverName & riverIndex, bool * pBool)
{
    return EditEnum<RiverName>("River", map, riverIndex, pBool);
}

//--------------------------------------------------------------------------------------
// RiverType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<RiverType> : EditEnumTraitsBase<RiverType>
{
    static auto GetColor(RiverType value) { return getRiverTypeColor(value); }
};

//--------------------------------------------------------------------------------------
bool EditRiverType(const Map * map, RiverType & river, bool * pBool)
{
    return EditEnum<RiverType>("Type", map, river, pBool);
}

//--------------------------------------------------------------------------------------
// RiverType traits
//--------------------------------------------------------------------------------------
template<> struct EditEnumTraits<DirectionType> : EditEnumTraitsBase<DirectionType>
{
    static auto GetColor(DirectionType value) { return getDirectionTypeColor(value); }
    static uint GetCount(const Map * map) { return enumCount<DirectionType>(); }
};

//--------------------------------------------------------------------------------------
bool EditDirectionType(const Map * map, DirectionType & direction, bool * pBool)
{
    return EditEnum<DirectionType>("Direction", map, direction, pBool);
}

