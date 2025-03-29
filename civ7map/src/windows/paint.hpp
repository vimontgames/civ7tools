#include "paint.h"

//--------------------------------------------------------------------------------------
PaintWindow::PaintWindow() :
    BaseWindow(ICON_FA_PAINT_ROLLER" Paint")
{
    s_instance = this;
}

//--------------------------------------------------------------------------------------
PaintWindow * PaintWindow::get()
{
    return s_instance;
}

//--------------------------------------------------------------------------------------
bool PaintWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_PAINT_ROLLER " Paint###Paint", &m_visible))
    {
        Map * map = g_map;

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::InputInt2("Plot", (int *)&g_hoveredCell, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::PopItemFlag();

        ImGui::SliderInt("Radius", &m_brushRadius, 1, 8);

        // Continent
        //if (ImGui::CollapsingHeader("Continent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        //{
        //    ImGui::Checkbox("Enable###PaintContinent", &m_paintContinentType);
        //
        //    PushDisabled(!m_paintContinentType);
        //    {
        //        DrawColoredSquare(getContinentColor(m_continentType));
        //
        //        string continentName = map? map->getContinentShortName(m_continentType) : Map::s_noContinentName;
        //    
        //        if (ImGui::BeginCombo("Continent###SelectPaintContinentCombo", fmt::sprintf("%s (%i)", continentName, (int)m_continentType).c_str()))
        //        {
        //            // None
        //            {
        //                bool isSelected = ((int)m_continentType == -1);
        //                if (ImGui::Selectable(fmt::sprintf("%s (-1)", map ? map->getContinentShortName((ContinentType)-1) : Map::s_noContinentName).c_str(), isSelected))
        //                {
        //                    m_continentType = (ContinentType)-1;
        //                }
        //            }
        //
        //            if (map)
        //            {
        //                for (uint i = 0; i < map->getContinentCount(); ++i)
        //                {
        //                    bool isSelected = ((int)m_continentType == i);
        //                    if (ImGui::Selectable(fmt::sprintf("%s (%i)", map->getContinentShortName((ContinentType)i), i).c_str(), isSelected))
        //                    {
        //                        m_continentType = (ContinentType)i;
        //                    }
        //                }
        //            }
        //            ImGui::EndCombo();
        //        }
        //    }
        //    PopDisabled();
        //}
        //ImGui::Spacing();

        // TerrainType
        if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Enable###PaintTerrain", &m_paintTerrainType);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Enable painting terrain type");

            PushDisabled(!m_paintTerrainType);
            {
                ImGui::Checkbox("Automatic Coasts###AutoCoast", &m_autoCoast);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Add coasts around continents when painting ocean or continents");

                DrawColoredSquare(getTerrainColor(m_terrainType));

                if (ImGui::BeginCombo("Terrain###SelectPaintTerrainCombo", fmt::sprintf("%s (%i)", asString(m_terrainType), (int)m_terrainType).c_str()))
                {
                    for (auto val : enumValues<TerrainType>())
                    {
                        bool isSelected = (val.first == m_terrainType);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), (int)val.first).c_str(), isSelected))
                            m_terrainType = val.first;
                    }
                    ImGui::EndCombo();
                }
            }

            PopDisabled();
        }
        ImGui::Spacing();

        // BiomeType
        if (ImGui::CollapsingHeader("Biome", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Enable###PaintBiome", &m_paintBiomeType);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Enable painting biome type");

            PushDisabled(!m_paintBiomeType);
            {
                DrawColoredSquare(getBiomeColor(m_biomeType));

                if (ImGui::BeginCombo("Biome###SelectPaintBiomeCombo", fmt::sprintf("%s (%i)", asString(m_biomeType), (int)m_biomeType).c_str()))
                {
                    for (auto val : enumValues<BiomeType>())
                    {
                        bool isSelected = (val.first == m_biomeType);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), (int)val.first).c_str(), isSelected))
                            m_biomeType = val.first;
                    }
                    ImGui::EndCombo();
                }
            }
            PopDisabled();
        }

        // FeatureType
        if (ImGui::CollapsingHeader("Feature", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Enable###PaintFeature", &m_paintFeature);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Enable painting feature type");

            PushDisabled(!map);
            {
                float buttonWidth = ImGui::CalcTextSize("Clear").x + ImGui::GetStyle().FramePadding.x * 2; // Including padding
                ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - buttonWidth);
                if (ImGui::Button("Clear###ClearFeatures"))
                    map->clearFeatures();
            }
            PopDisabled();

            PushDisabled(!m_paintFeature);
            {
                //ImGui::Checkbox("Force compatible Terrain###FeatureAutoTerrain", &m_featureAutoTerrain);
                //if (ImGui::IsItemHovered())
                //    ImGui::SetTooltip("Force compatible terrain when placing feature");
                //
                //ImGui::Checkbox("Force compatible Biome###FeatureAutoBiome", &m_featureAutoBiome);
                //if (ImGui::IsItemHovered())
                //    ImGui::SetTooltip("Force compatible biome when placing feature");

                DrawColoredSquare(getFeatureColor(m_featureType));

                if (ImGui::BeginCombo("Feature###SelectPaintFeatureCombo", fmt::sprintf("%s (%i)", asString(m_featureType), (int)m_featureType).c_str()))
                {
                    for (auto val : enumValues<FeatureType>())
                    {
                        bool isSelected = (val.first == m_featureType);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), (int)val.first).c_str(), isSelected))
                            m_featureType = val.first;
                    }
                    ImGui::EndCombo();
                }
            }

            PopDisabled();
        }

        // ResourceType
        if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Enable###PaintResource", &m_paintResource);

            PushDisabled(!map);
            {
                float buttonWidth = ImGui::CalcTextSize("Clear").x + ImGui::GetStyle().FramePadding.x * 2; // Including padding
                ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - buttonWidth);
                if (ImGui::Button("Clear###ClearResources"))
                    map->clearResources();
            }
            PopDisabled();

            PushDisabled(!m_paintResource);
            {
                DrawColoredSquare(getResourceColor(m_resourceType));

                if (ImGui::BeginCombo("Resource###SelectPaintResourceCombo", fmt::sprintf("%s (%i)", asString(m_resourceType), (int)m_resourceType).c_str()))
                {
                    for (auto val : enumValues<ResourceType>())
                    {
                        bool isSelected = (val.first == m_resourceType);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), (int)val.first).c_str(), isSelected))
                            m_resourceType = val.first;
                    }
                    ImGui::EndCombo();
                }
            }
            PopDisabled();
        }
    }

    ImGui::End();

    return needRefresh;
}