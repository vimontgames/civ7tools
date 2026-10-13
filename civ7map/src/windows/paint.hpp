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

            // "TerrainType"
            TerrainType terrain = m_terrainType;
            if (EditTerrain(map, terrain, &m_paintTerrainType))
                m_terrainType = terrain;

            // "BiomeType"
            BiomeType biome = m_biomeType;
            if (EditBiome(map, biome, &m_paintBiomeType))
                m_biomeType = biome;

            // "FeatureType"
            FeatureType feature = m_featureType;
            if (EditFeature(map, feature, &m_paintFeatureType))
                m_featureType = feature;

            // "ResourceType"
            ResourceType resource = m_resourceType;
            if (EditResource(map, resource, &m_paintResourceType))
                m_resourceType = resource;

            // "NaturalWonderType"
            NaturalWonderType naturalWonder = m_naturalWonder;
            if (EditNaturalWonder(map, naturalWonder, &m_paintNaturalWonderType))
                m_naturalWonder = naturalWonder;

            // "Elevation"
            Elevation elevation = m_elevation;
            if (EditElevation(map, elevation, &m_paintElevation))
                m_elevation = elevation;           
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
                        after.terrain = TerrainType::Mountain;
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