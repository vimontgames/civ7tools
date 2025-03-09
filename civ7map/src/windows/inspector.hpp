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
    BaseWindow("Inspector")
{

}

//--------------------------------------------------------------------------------------
bool InspectorWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin("Inspector", &m_visible))
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
        }
        else
        {
            ImGui::InputInt2("Plot", (int*)&g_selectedCell, ImGuiInputTextFlags_EnterReturnsTrue);

            //int engineLoc[2] =
            //{
            //    x,
            //    y + 2
            //};
            //ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            //ImGui::InputInt2("Plot (engine)", engineLoc, ImGuiInputTextFlags_EnterReturnsTrue);
            //ImGui::PopItemFlag();

            // Continent
            {
                DrawColoredSquare(getContinentColor(tile->continent));

                string continentName = map->getContinentShortName(tile->continent);

                if (ImGui::BeginCombo("Continent", fmt::sprintf("%s (%i)", continentName, (int)tile->continent).c_str()))
                {
                    // None
                    {
                        bool isSelected = ((int)tile->continent == -1);
                        if (ImGui::Selectable(fmt::sprintf("%s (-1)", map->getContinentShortName((ContinentType)-1)).c_str(), isSelected))
                        {
                            if (map->setContinent(x, y, (ContinentType)-1))
                                dirty = true;
                        }
                    }
                    for (uint i = 0; i < map->getContinentCount(); ++i)
                    {
                        bool isSelected = ((int)tile->continent == i);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", map->getContinentShortName((ContinentType)i), i).c_str(), isSelected))
                        {
                            if (map->setContinent(x, y, (ContinentType)i))
                                dirty = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }

            // TerrainType
            {
                DrawColoredSquare(getTerrainColor(tile->terrain));

                if (ImGui::BeginCombo("Terrain", fmt::sprintf("%s (%i)", asString(tile->terrain), (int)tile->terrain).c_str()))
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

                if (ImGui::BeginCombo("Biome", fmt::sprintf("%s (%i)", asString(tile->biome), (int)tile->biome).c_str()))
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

                if (ImGui::BeginCombo("Feature", fmt::sprintf("%s (%i)", asString(tile->feature), (int)tile->feature).c_str()))
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

                if (ImGui::BeginCombo("Resource", fmt::sprintf("%s (%i)", asString(tile->resource), (int)tile->resource).c_str()))
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
        }

        if (dirty)
            map->refresh();

        if (!tile)
            ImGui::PopItemFlag();
    }

    ImGui::End();

    return needRefresh;
}