#include "paint.h"
#include "undoredo\UndoRedoTile.h"

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

        if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::InputInt2("Plot", (int *)&g_hoveredCell, ImGuiInputTextFlags_EnterReturnsTrue);
            Vector2i selectionSize = g_selectedRectMax - g_selectedRectMin + Vector2i(1,1);
            ImGui::InputInt2("Selected", (int *)&selectionSize, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemFlag();

            ImGui::SliderInt("Radius", &m_brushRadius, 1, 8);

            ImGui::Checkbox("Automatic Coast###AutoCoast", &m_autoCoast);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Add coasts around continents when painting");   

            ImGui::Checkbox("Protect Coast###mProtectCoast", &m_protectCoasts);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disable painting on coast tiles (e.g. when painting continent)");

            ImGui::Checkbox("Protect Ocean###mProtectOcean", &m_protectOcean);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Disable painting on ocean tiles (e.g. when painting landmass)");

            //ImGui::Checkbox("Force compatible Terrain###FeatureAutoTerrain", &m_featureAutoTerrain);
            //if (ImGui::IsItemHovered())
            //    ImGui::SetTooltip("Force compatible terrain when placing feature");
            //
            //ImGui::Checkbox("Force compatible Biome###FeatureAutoBiome", &m_featureAutoBiome);
            //if (ImGui::IsItemHovered())
            //    ImGui::SetTooltip("Force compatible biome when placing feature");
        }

        if (ImGui::CollapsingHeader("Values", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            const float comboX = ImGui::GetCursorPosX() + 128;

            // "Continent"
            ContinentType continent = m_continentType;
            if (EditContinent(map, continent, &m_paintContinentType))
                m_continentType = continent;
   
            // "Landmass"
            LandmassType landmass = m_landmassType;
            if (EditLandmass(map, landmass, &m_paintLandmassType))
                m_landmassType = landmass;            
 
            // TerrainType
            //if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Checkbox("Terrain###PaintTerrain", &m_paintTerrainType);
                ImGui::SameLine(comboX);

                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Enable painting terrain type");

                PushDisabled(!m_paintTerrainType);
                {
                    DrawColoredSquare(getTerrainColor(m_terrainType));

                    const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
                    ImGui::SetNextItemWidth(comboWidth);

                    if (ImGui::BeginCombo("###SelectPaintTerrainCombo", fmt::sprintf("%s (%i)", asString(m_terrainType), (int)m_terrainType).c_str(), ImGuiComboFlags_HeightLargest))
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

            // BiomeType
            //if (ImGui::CollapsingHeader("Biome", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Checkbox("Biome###PaintBiome", &m_paintBiomeType);
                ImGui::SameLine(comboX);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Enable painting biome type");

                PushDisabled(!m_paintBiomeType);
                {
                    DrawColoredSquare(getBiomeColor(m_biomeType));

                    const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
                    ImGui::SetNextItemWidth(comboWidth);

                    if (ImGui::BeginCombo("###SelectPaintBiomeCombo", fmt::sprintf("%s (%i)", asString(m_biomeType), (int)m_biomeType).c_str(), ImGuiComboFlags_HeightLargest))
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
            //if (ImGui::CollapsingHeader("Feature", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Checkbox("Feature###PaintFeature", &m_paintFeature);
                ImGui::SameLine(comboX);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Enable painting feature type");

                PushDisabled(!m_paintFeature);
                {
                    DrawColoredSquare(getFeatureColor(m_featureType));

                    const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
                    ImGui::SetNextItemWidth(comboWidth);

                    if (ImGui::BeginCombo("###SelectPaintFeatureCombo", fmt::sprintf("%s (%i)", asString(m_featureType), (int)m_featureType).c_str(), ImGuiComboFlags_HeightLargest))
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
            //if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Checkbox("Resource###PaintResource", &m_paintResource);
                ImGui::SameLine(comboX);

                PushDisabled(!m_paintResource);
                {
                    DrawColoredSquare(getResourceColor(m_resourceType));

                    const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
                    ImGui::SetNextItemWidth(comboWidth);

                    if (ImGui::BeginCombo("###SelectPaintResourceCombo", fmt::sprintf("%s (%i)", asString(m_resourceType), (int)m_resourceType).c_str(), ImGuiComboFlags_HeightLargest))
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

            // Elevation
            //if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                ImGui::Checkbox("Elevation###PaintElevation", &m_paintElevation);
                ImGui::SameLine(comboX);

                PushDisabled(!m_paintElevation);
                {
                    DrawColoredSquare(getElevationColor(m_elevation));

                    const float comboWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetCursorPosX();
                    ImGui::SetNextItemWidth(comboWidth);

                    int temp = m_elevation;
                    if (ImGui::SliderInt("###Elevation", &temp, 0, 1023))
                    {
                        m_elevation = temp;
                    }
                }
                PopDisabled();
            }
        }

        
        if (ImGui::CollapsingHeader("Misc", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            if (ImGui::Button("Add Borders"))
            {
                UndoRedoTile * undoRedoPaintTile = new UndoRedoTile(map);

                const int borderSize = 3;

                for (uint y = 0; y < map->m_height; ++y)
                {
                    for (uint x = 0; x < map->m_width; ++x)
                    {
                        if (x >= borderSize && x < map->m_width - borderSize)
                            continue;

                        const Civ7Tile & before = map->m_civ7TerrainType.get(x, y);
                        Civ7Tile after = before;
                        after.biome = BiomeType::Marine;
                        after.terrain = TerrainType::Moutain;
                        after.feature = FeatureType::None;
                        after.resource = ResourceType::None;
                        after.continent = (ContinentType::None);

                        undoRedoPaintTile->add(x, y, before, after);
                        map->m_civ7TerrainType.get(x, y) = after;
                    }
                }

                map->refresh();

                UndoRedoStack::add(undoRedoPaintTile);
            }

            PushDisabled(!map);
            {
                //float buttonWidth = ImGui::CalcTextSize("Clear").x + ImGui::GetStyle().FramePadding.x * 2; // Including padding
                //ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - buttonWidth);
                ImGui::SameLine();
                if (ImGui::Button("Clear Features###ClearFeatures"))
                    map->clearFeatures();

                //float buttonWidth = ImGui::CalcTextSize("Clear").x + ImGui::GetStyle().FramePadding.x * 2; // Including padding
                //ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - buttonWidth);
                ImGui::SameLine();
                if (ImGui::Button("Clear Resources###ClearResources"))
                    map->clearResources();
            }
            PopDisabled();
        }
    }

    ImGui::End();

    return needRefresh;
}