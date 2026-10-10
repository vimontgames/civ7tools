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
                map->setContinent(x, y, continent);

            // "Landmass"
            LandmassType landmass = tile->landmass;
            if (EditLandmass(map, landmass))
                map->setLandmass(x, y, landmass);

            //// Landmass
            //{
            //    DrawColoredSquare(getLandmassColor(tile->landmass));
            //
            //    string landmassName = map->getLandmassShortName(tile->landmass);
            //
            //    if (ImGui::BeginCombo("Landmass", fmt::sprintf("%s (%i)", landmassName, (int)tile->landmass).c_str(), ImGuiComboFlags_HeightLargest))
            //    {
            //        for (uint i = 0; i < map->getLandmassCount(); ++i)
            //        {
            //            bool isSelected = ((int)tile->landmass == i);
            //            if (ImGui::Selectable(fmt::sprintf("%s (%i)", map->getLandmassShortName((LandmassType)i), i).c_str(), isSelected))
            //            {
            //                if (map->setLandmass(x, y, (LandmassType)i))
            //                    dirty = true;
            //            }
            //        }
            //        ImGui::EndCombo();
            //    }
            //}

            // TerrainType
            {
                DrawColoredSquare(getTerrainColor(tile->terrain));

                if (ImGui::BeginCombo("Terrain", fmt::sprintf("%s (%i)", asString(tile->terrain), (int)tile->terrain).c_str(), ImGuiComboFlags_HeightLargest))
                {
                    for (auto val : enumValues<TerrainType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = ((int)tile->terrain == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            if (map->setTerrain(x, y, val.first))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Biome
            {
                DrawColoredSquare(getBiomeColor(tile->biome));

                if (ImGui::BeginCombo("Biome", fmt::sprintf("%s (%i)", asString(tile->biome), (int)tile->biome).c_str(), ImGuiComboFlags_HeightLargest))
                {
                    for (auto val : enumValues<BiomeType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = ((int)tile->biome == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            if (map->setBiome(x, y, val.first))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Feature
            {
                DrawColoredSquare(getFeatureColor(tile->feature));

                if (ImGui::BeginCombo("Feature", fmt::sprintf("%s (%i)", asString(tile->feature), (int)tile->feature).c_str(), ImGuiComboFlags_HeightLargest))
                {
                    for (auto val : enumValues<FeatureType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = ((int)tile->feature == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                             if (map->setFeature(x, y, val.first))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Resource
            {
                DrawColoredSquare(getResourceColor(tile->resource));

                if (ImGui::BeginCombo("Resource", fmt::sprintf("%s (%i)", asString(tile->resource), (int)tile->resource).c_str(), ImGuiComboFlags_HeightLargest))
                {
                    for (auto val : enumValues<ResourceType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = ((int)tile->resource == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), index).c_str(), isSelected))
                        {
                            if (map->setResource(x, y, val.first))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Natural Wonder
            {
                DrawColoredSquare(getNaturalWonderColor(tile->naturalWonder));

                if (ImGui::BeginCombo("Natural", fmt::sprintf("%s (%i)", SeparateCapitalizedWords(asString(tile->naturalWonder)), (int)tile->naturalWonder).c_str(), ImGuiComboFlags_HeightLargest))
                {
                    for (auto val : enumValues<NaturalWonderType>())
                    {
                        const int index = (int)val.first;
                        bool isSelected = ((int)tile->naturalWonder == index);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", SeparateCapitalizedWords(asString(val.first)), index).c_str(), isSelected))
                        {
                            if (map->setNaturalWonder(x, y, val.first))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // Elevation
            {
                DrawColoredSquare(getElevationColor(tile->elevation));

                int step = (tile->elevation + elevationStep / 2) / elevationStep;
                char format[32];
                sprintf_s(format, "%d", step * elevationStep);

                if (ImGui::SliderInt("Elevation", &step, 0, maxElevation / elevationStep, format))
                {
                    tile->elevation = step * elevationStep;
                }
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
                                        vector<Civilization *> sortedCivs(civilizations.size());
                                        for (int p = 0; p < civilizations.size(); ++p)
                                            sortedCivs[p] = &civilizations[p];

                                        sort(sortedCivs.begin(), sortedCivs.end(), [](const Civilization * a, const Civilization * b) {
                                            if (a->era == b->era)
                                                return a->userFriendlyName < b->userFriendlyName;  // Sort by name if categories are the same
                                            return (int)a->era < (int)b->era;  // Otherwise, sort by category
                                            });

                                        Era prevEra = (Era)-2;

                                        for (uint i = 0; i < sortedCivs.size(); ++i)
                                        {
                                            bool isSelected = i == c;
                                            auto * dstCiv = sortedCivs[i];
                                            Era era = dstCiv->era;

                                            if (era != prevEra)
                                            {
                                                int eraCivsCount = 0;
                                                int eraCivsCountWithTSL = 0;

                                                for (int cc = 0; cc < civilizations.size(); ++cc)
                                                {
                                                    auto & cciv = civilizations[cc];
                                                    if (cciv.era == era)
                                                    {
                                                        eraCivsCount++;
                                                        if (cciv.tsl.size() > 0)
                                                            eraCivsCountWithTSL++;
                                                    }
                                                }

                                                ImGui::TextDisabled(fmt::sprintf("%s (%u/%u)", asString(era), eraCivsCountWithTSL, eraCivsCount).c_str());
                                            }

                                            if (ImGui::Selectable(fmt::sprintf("%s (%i)", dstCiv->userFriendlyName, dstCiv->tsl.size()).c_str(), isSelected))
                                            {
                                                civ.tsl.erase(civ.tsl.begin() + t);
                                                TSL newTSL;
                                                newTSL.pos.x = x;
                                                newTSL.pos.y = y;
                                                dstCiv->tsl.push_back(newTSL);
                                                map->refresh();
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
                                        map->refresh();
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
                            map->refresh();
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