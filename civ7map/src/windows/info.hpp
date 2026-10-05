#include "BaseWindow.h"
#include "misc\imguiutils.h"

//--------------------------------------------------------------------------------------
class InfoWindow : public BaseWindow
{
public:
    InfoWindow();
    bool Draw(const RenderWindow & window) final override;
};

//--------------------------------------------------------------------------------------
InfoWindow::InfoWindow() :
    BaseWindow(ICON_FA_GLOBE" Map")
{

}


//--------------------------------------------------------------------------------------
bool InfoWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_GLOBE " Map###Map", &m_visible) && g_map)
    {
        const uint itemWidth = 128;

        char temp[4096];

        //ImGui::PushItemWidth(ImGui::GetFontSize() * 10.0f); 

        sprintf_s(temp, "%s", g_map->getBaseName().c_str());
        ImGui::InputText("Base Name", temp, sizeof(temp), ImGuiInputTextFlags_ReadOnly);

        sprintf_s(temp, "%s", g_map->getPrettyName().c_str());
        if (ImGui::InputText("Pretty Name", temp, sizeof(temp)))
            g_map->m_prettyName = temp;

        sprintf_s(temp, "%s", g_map->getAuthor().c_str());
        if (ImGui::InputText("Author", temp, sizeof(temp)))
            g_map->m_author = temp;

        sprintf_s(temp, "%s", g_map->getDescription().c_str());
        if (ImGui::InputText("Description", temp, sizeof(temp)))
            g_map->m_description = temp;

        if (ImGui::CollapsingHeader("Randomize", ImGuiTreeNodeFlags_Framed))
        {
            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Resources", &g_map->m_randomResources);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place resources randomly when generating map");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Features", &g_map->m_randomFeatures);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place features randomly when generating map");

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Lakes", &g_map->m_randomLakes);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place lakes randomly when generating map");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Elevation", &g_map->m_randomElevation);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Change elevation when generating map");

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Hills", &g_map->m_randomHills);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place hills randomly when generating map");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Rainfall", &g_map->m_randomRainfall);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place rainfalls randomly when generating map");

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("FloodPlains", &g_map->m_randomFloodPlains);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place flood plains randomly when generating map");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Rivers", &g_map->m_randomRivers);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place rivers randomly when generating map");

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Continents", &g_map->m_randomContinents);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Select continents randomly when generating map");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Biomes", &g_map->m_randomBiomes);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Select biomes randomly when generating map");

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Natural Wonders", &g_map->m_randomNaturalWonders);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Place snow randomly when generating map");
            //ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            //ImGui::Checkbox("Wonders", &g_map->m_randomNaturalWonders);
            //if (ImGui::IsItemHovered())
            //    ImGui::SetTooltip("Place natural wonders randomly when generating map"); 
        }

        if (ImGui::CollapsingHeader("Size", ImGuiTreeNodeFlags_Framed))
        {
            bool isValidMapSize = g_map->getMapSize(g_map->m_editMapSize[0], g_map->m_editMapSize[1]) != MapSize::Custom;

            ImGui::InputInt2("Size", g_map->m_editMapSize, ImGuiInputTextFlags_EnterReturnsTrue);

            ImGui::SameLine();

            const bool editing = g_map->m_width != g_map->m_editMapSize[0] || g_map->m_height != g_map->m_editMapSize[1];
            PushDisabled(!editing);
            {
                if (ImGui::Button(ICON_FA_CROP))
                {
                    g_map->crop(sf::Vector2i(g_map->m_editMapSize[0], g_map->m_editMapSize[1]));
                    g_map->refresh();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Crop");

                ImGui::SameLine();
                if (ImGui::Button(ICON_FA_MAXIMIZE))
                {
                    g_map->rescale(sf::Vector2i(g_map->m_editMapSize[0], g_map->m_editMapSize[1]));
                    g_map->refresh();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Rescale");
            }
            PopDisabled();

            ImGui::SameLine();
            if (!isValidMapSize)
                ImGui::TextColored(ImVec4(1, 0.5, 0, 1), ICON_FA_TRIANGLE_EXCLAMATION);
            else
                ImGui::TextColored(ImVec4(0, 1, 0, 1), ICON_FA_CHECK);

            if (ImGui::IsItemHovered())
            {
                //string invalidMapSizeMsg = isValidMapSize ? "Valid map sizes:\n" : "Please use a valid map size!\n";
                //for (auto val : enumValues<MapSize>())
                //{
                //    if (val.first == MapSize::Custom)
                //        continue;
                //    invalidMapSizeMsg += fmt::sprintf("- %s (%ix%i)\n", asString(val.first), g_mapSizes[(int)val.first][0], g_mapSizes[(int)val.first][1]);
                //}
                //ImGui::SetTooltip(invalidMapSizeMsg.c_str());

                if (!isValidMapSize)
                    ImGui::SetTooltip("Map is using a custom map size");
            }   

            if (ImGui::InputInt2("Offset", g_map->m_editMapOffset, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_mapOffset[0].x = g_map->m_editMapOffset[0];
                g_map->m_mapOffset[0].y = g_map->m_editMapOffset[1];

                Vector2i delta = g_map->m_mapOffset[0] - g_map->m_mapOffset[1];

                g_map->translate(delta);
                g_map->m_mapOffset[1] = g_map->m_mapOffset[0];
                g_map->refresh();
            }

            ImGui::SetNextItemWidth(itemWidth);
            ImGui::Checkbox("Wrap X", &g_map->m_wrapX);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Wrap map horizontally. Beware that disabling horizontal wrap may cause minimap issues, use impassable terrain + snow horizontal borders instead");
            ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - itemWidth);
            ImGui::Checkbox("Wrap Y", &g_map->m_wrapY);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Wrap map vertically");

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Force snow on each side of the map to mimic broken WrapX = false");           
        }

        if (ImGui::CollapsingHeader("Snow###SnowOptions", ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Generate Snow", &g_map->m_randomSnow);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Generate snow from poles");
 
            PushDisabled(!g_map->m_randomSnow || g_map->m_useAdvancedSnow);
            {
                if (ImGui::InputInt("Top Lattitude", &g_map->m_topLattitude))
                {
                    if (g_map->m_topLattitude > 90)
                        g_map->m_topLattitude = 90;
                    else if (g_map->m_topLattitude < -90)
                        g_map->m_topLattitude = -90;
                }

                if (ImGui::InputInt("Bottom Lattitude", &g_map->m_bottomLattitude))
                {
                    if (g_map->m_bottomLattitude < -90)
                        g_map->m_bottomLattitude = -90;
                    else if (g_map->m_bottomLattitude > 90)
                        g_map->m_bottomLattitude = 90;
                }
            }
            PopDisabled();

            PushDisabled(!g_map->m_randomSnow);
            {
                ImGui::Checkbox("Advanced Snow Parameters", &g_map->m_useAdvancedSnow);
            }
            PopDisabled();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Use advanced snow generation parameters");

            PushDisabled(!g_map->m_randomSnow || !g_map->m_useAdvancedSnow);
            {
                if (ImGui::InputInt("Top Rows", &g_map->m_topSnowRows))
                {
                    if (g_map->m_topSnowRows > g_map->m_height)
                        g_map->m_topSnowRows = g_map->m_height;
                    else if (g_map->m_topSnowRows < 0)
                        g_map->m_topSnowRows = 0;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Add N lines of snow at the top of the map");

                if (ImGui::InputInt("Bottom Rows", &g_map->m_bottomSnowRows))
                {
                    if (g_map->m_bottomSnowRows > g_map->m_height)
                        g_map->m_bottomSnowRows = g_map->m_height;
                    else if (g_map->m_bottomSnowRows < 0)
                        g_map->m_bottomSnowRows = 0;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Add N lines of snow at the bottom of the map");

                if (ImGui::InputInt("Max Weight", &g_map->m_maxSnowWeight))
                {
                    if (g_map->m_maxSnowWeight > 100)
                        g_map->m_maxSnowWeight = 100;
                    else if (g_map->m_maxSnowWeight < 0)
                        g_map->m_maxSnowWeight = 0;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Maximum amount of snow");

                if (ImGui::InputInt("Randomization", &g_map->m_snowRandomization))
                {
                    if (g_map->m_snowRandomization > 100)
                        g_map->m_snowRandomization = 1000;
                    else if (g_map->m_snowRandomization < 0)
                        g_map->m_snowRandomization = 0;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Randomize snow amount");
            }
            PopDisabled();

            if (ImGui::InputInt("Horizontal snow borders", &g_map->m_snowBorderX))
            {
                if (g_map->m_snowBorderX < 0)
                    g_map->m_snowBorderX = 0;
                else if (g_map->m_snowBorderX > 16)
                    g_map->m_snowBorderX = 16;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Add snow on X tiles on each map sides");
        }

        if (g_map->m_mapVersion == MapVersion::YnAMP && ImGui::CollapsingHeader("Hemispheres", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
        {
            int editWest[2] =
            {
                (int)g_map->m_westStart,
                (int)g_map->m_westEnd
            };
            if (ImGui::InputInt2("West", editWest, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_westStart = editWest[0];
                g_map->m_westEnd = editWest[1];
                g_map->fixHemispheres();
            }

            int editEast[2] =
            {
                (int)g_map->m_eastStart,
                (int)g_map->m_eastEnd
            };
            if (ImGui::InputInt2("East", editEast, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                g_map->m_eastStart = editEast[0];
                g_map->m_eastEnd = editEast[1];
                g_map->fixHemispheres();
            }
        }  

        if (ImGui::CollapsingHeader("Misc", ImGuiTreeNodeFlags_Framed))
        {
            ImGui::Checkbox("Export TSL", &g_map->m_useTSL);
        }

        //ImGui::PopItemWidth();
    }

    ImGui::End();

    return needRefresh;
}