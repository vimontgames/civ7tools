#include "paint.h"

//--------------------------------------------------------------------------------------
PaintWindow::PaintWindow() :
    BaseWindow("Paint")
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

    if (Begin("Paint", &m_visible))
    {
        Map * map = g_map;

        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::InputInt2("Plot", (int *)&g_hoveredCell, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::PopItemFlag();

        // Continent
        {
            DrawColoredCheckbox(getContinentColor(m_continentType), &m_paintContinentType);

            string continentName = map? map->getContinentShortName(m_continentType) : Map::s_noContinentName;
            
            if (ImGui::BeginCombo("Continent", fmt::sprintf("%s (%i)", continentName, (int)m_continentType).c_str()))
            {
                // None
                {
                    bool isSelected = ((int)m_continentType == -1);
                    if (ImGui::Selectable(fmt::sprintf("%s (-1)", map ? map->getContinentShortName((ContinentType)-1) : Map::s_noContinentName).c_str(), isSelected))
                    {
                        m_continentType = (ContinentType)-1;
                    }
                }

                if (map)
                {
                    for (uint i = 0; i < map->getContinentCount(); ++i)
                    {
                        bool isSelected = ((int)m_continentType == i);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", map->getContinentShortName((ContinentType)i), i).c_str(), isSelected))
                        {
                            m_continentType = (ContinentType)i;
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }

        // TerrainType
        {
            DrawColoredCheckbox(getTerrainColor(m_terrainType), &m_paintTerrainType);

            if (ImGui::BeginCombo("Terrain", fmt::sprintf("%s (%i)", asString(m_terrainType), (int)m_terrainType).c_str()))
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

        // BiomeType
        {
            DrawColoredCheckbox(getBiomeColor(m_biomeType), &m_paintBiomeType);

            if (ImGui::BeginCombo("Biome", fmt::sprintf("%s (%i)", asString(m_biomeType), (int)m_biomeType).c_str()))
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

        // FeatureType
        {
            DrawColoredCheckbox(getFeatureColor(m_featureType), &m_paintFeature);

            if (ImGui::BeginCombo("Feature", fmt::sprintf("%s (%i)", asString(m_featureType), (int)m_featureType).c_str()))
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

        // ResourceType
        {
            DrawColoredCheckbox(getResourceColor(m_resourceType), &m_paintResource);

            if (ImGui::BeginCombo("Resource", fmt::sprintf("%s (%i)", asString(m_resourceType), (int)m_resourceType).c_str()))
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
    }

    ImGui::End();

    return needRefresh;
}