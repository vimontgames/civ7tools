#include "BaseWindow.h"
#include "misc/imguiutils.h"

//--------------------------------------------------------------------------------------
class InspectorWindow : public BaseWindow
{
public:
    InspectorWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
InspectorWindow::InspectorWindow() :
    BaseWindow(ICON_FA_MAGNIFYING_GLASS" Inspector")
{

}

//--------------------------------------------------------------------------------------
bool InspectorWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_MAGNIFYING_GLASS" Inspector###Inspector", &m_visible))
    {
        Civ7Tile * tile = nullptr;
        Map * map = g_map;

        const auto x = g_selectedCell.x;
        const auto y = g_selectedCell.y;
        
        if (map && (uint)x < map->m_width && (uint)y < map->m_height)
            tile = &map->m_civ7TerrainType.get(x, y);

        if (!tile)
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        bool dirty = false;

        if (!tile)
        {
            ImGui::Text("Press \"Space\" to select  tile");
            ImGui::Separator();
        }
        else
        {
            ImGui::InputInt2("Plot", (int*)&g_selectedCell, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::Separator();

            // "Continent"
            ContinentType continent = tile->continent;
            if (EditContinent(map, continent))
                dirty |= map->setContinent(x, y, continent);

            // "Landmass"
            LandmassType landmass = tile->landmass;
            if (EditLandmass(map, landmass))
                dirty |= map->setLandmass(x, y, landmass);

            // "TerrainType"
            TerrainType terrain = tile->terrain;
            if (EditTerrain(map, terrain))
                dirty |= map->setTerrain(x, y, terrain);

            // "BiomeType"
            BiomeType biome = tile->biome;
            if (EditBiome(map, biome))
                dirty |= map->setBiome(x, y, biome);

            // "FeatureType"
            FeatureType feature = tile->feature;
            if (EditFeature(map, feature))
                dirty |= map->setFeature(x, y, feature);

            // "ResourceType"
            ResourceType resource = tile->resource;
            if (EditResource(map, resource))
                dirty |= map->setResource(x, y, resource);

            // "NaturalWonderType"
            NaturalWonderType naturalWonder = tile->naturalWonder;
            if (EditNaturalWonder(map, naturalWonder))
                dirty |= map->setNaturalWonder(x, y, naturalWonder);

            // "Elevation"
            Elevation elevation = tile->elevation;
            if (EditElevation(map, elevation))
                dirty |= map->setElevation(x, y, elevation);

            if (ImGui::CollapsingHeader("Rivers###RiversInspector", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                bool dirtyRiver = false;

                // "RiverName"
                for (uint i = 0; i < MAX_RIVER_PER_TILE; ++i)
                {
                    ImGui::PushID(i);
                    ImGui::Text(fmt::sprintf("River %i", i).c_str() );

                    RiverTileInfo riverTileInfo = tile->rivers[i];

                    RiverName riverIndex = riverTileInfo.nameIndex;
                    if (EditRiverName(map, riverIndex))
                        dirtyRiver |= map->setRiverName(x, y, riverIndex, i);

                    if (riverIndex != RiverName::None)
                    {
                        // "RiverType"
                        RiverType riverType = riverTileInfo.type;
                        if (EditRiverType(map, riverType))
                            dirtyRiver |= map->setRiverType(x, y, riverType, i);
                        
                        // "RiverDirection"
                        DirectionType riverDirection = riverTileInfo.direction;
                        if (EditDirectionType(map, riverDirection))
                            dirtyRiver |= map->setRiverDirection(x, y, riverDirection, i);
                    }
                    ImGui::PopID();
                }

                if (dirtyRiver)
                    g_map->UpdateRivers(true);

                dirty |= dirtyRiver;                
            }

            // TSL
            {
                auto & civilizations = map->getCivilizations();
                bool anyTSL = false;
                for (int c = 0; c < civilizations.size(); ++c)
                {
                    const auto & civ = civilizations[c];
                    for (int t = 0; t < civ.tsl.size(); ++t)
                    {
                        const auto & tsl = civ.tsl[t];
                        if (tsl.pos.x == x && tsl.pos.y == y)
                        {
                            anyTSL = true;
                            break;
                        }
                    }
                }

                //if (anyTSL)
                {
                    if (ImGui::CollapsingHeader("TSL", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
                    {
                        if (ImGui::Checkbox("Include modded civilizations", &EditorOptions::Get().m_useModdedCivsForTSL))
                            EditorOptions::Get().Save();

                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip("Include modded civilizations from popular mods");

                        for (int c = 0; c < civilizations.size(); ++c)
                        {
                            auto & civ = civilizations[c];
                            for (int t = 0; t < civ.tsl.size(); ++t)
                            {
                                auto & tsl = civ.tsl[t];
                                if (tsl.pos.x == x && tsl.pos.y == y)
                                {
                                    const uint id = (c << 8) | t;

                                    DrawColoredSquare(float4(pow(civ.color.r, 1.0f / 2.2f), pow(civ.color.g, 1.0f / 2.2f), pow(civ.color.b, 1.0f / 2.2f), 1.0f));

                                    if (ImGui::BeginCombo(fmt::sprintf("TSL###TSL%u", id).c_str(), fmt::sprintf("%s (%u)", civ.userFriendlyName.c_str(), civ.tsl.size()).c_str(), ImGuiComboFlags_HeightLarge))
                                    {
                                        // Sort by era then alphabetical order
                                        vector<Civilization *> sortedCivs;
                                        for (int p = 0; p < civilizations.size(); ++p)
                                        {
                                            if (EditorOptions::Get().m_useModdedCivsForTSL || civilizations[p].vanilla)
                                                sortedCivs.push_back(&civilizations[p]);
                                        }
                                        sort(sortedCivs.begin(), sortedCivs.end(), [](const Civilization * a, const Civilization * b) {
                                            if (a->era == b->era)
                                                return a->userFriendlyName < b->userFriendlyName;  // Sort by name if categories are the same
                                            return (int)a->era < (int)b->era;  // Otherwise, sort by category
                                            });

                                        Era prevEra = (Era)-2;

                                        bool firstEra = true;

                                        for (uint i = 0; i < sortedCivs.size(); ++i)
                                        {
                                            bool isSelected = i == c;
                                            auto * dstCiv = sortedCivs[i];
                                            Era era = dstCiv->era;

                                            if (era != prevEra)
                                            {
                                                int eraCivsCount = 0;
                                                int eraCivsCountWithTSL = 0;

                                                for (int cc = 0; cc < sortedCivs.size(); ++cc)
                                                {
                                                    auto & cciv = civilizations[cc];
                                                    if (cciv.era == era)
                                                    {
                                                        eraCivsCount++;
                                                        if (cciv.tsl.size() > 0)
                                                            eraCivsCountWithTSL++;
                                                    }
                                                }

                                                if (!firstEra)
                                                {
                                                    ImGui::Text(" ");
                                                    ImGui::Separator();
                                                    ImGui::TextDisabled(fmt::sprintf("%s (%u/%u)", asString(era), eraCivsCountWithTSL, eraCivsCount).c_str());
                                                    ImGui::Separator();
                                                }
              
                                                firstEra = false;                             
                                            }

                                            if (ImGui::Selectable(fmt::sprintf("%s (%i)", dstCiv->userFriendlyName, dstCiv->tsl.size()).c_str(), isSelected))
                                            {
                                                civ.tsl.erase(civ.tsl.begin() + t);
                                                TSL newTSL;
                                                newTSL.pos.x = x;
                                                newTSL.pos.y = y;
                                                dstCiv->tsl.push_back(newTSL);
                                                dirty = true;
                                            }

                                            if (ImGui::IsItemHovered() && !dstCiv->civilizationName.empty())
                                            {
                                                ImGui::SetTooltip(dstCiv->civilizationName.c_str());
                                            }

                                            prevEra = era;
                                        }

                                        ImGui::EndCombo();
                                    }

                                    ImGui::SameLine();
                                     
                                    if (ImGui::Button(fmt::sprintf("Remove###Remove%u", id).c_str()))
                                    {
                                        civ.tsl.erase(civ.tsl.begin() + t);
                                        dirty = true;
                                    }
                                }
                            }
                        }

                        if (ImGui::Button("Add TSL"))
                        {
                            TSL newTSL;
                            newTSL.pos.x = x;
                            newTSL.pos.y = y;
                            civilizations[0].tsl.push_back(newTSL);
                            dirty = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Missing TSLs"))
                        {
                            g_map->LogMissingAndDuplicateTSLs();
                        }
                    }
                }
            }
        }

        if (dirty)
            map->refresh();

        if (!tile)
            ImGui::PopItemFlag();
    }

    ImGui::End();

    return needRefresh;
}