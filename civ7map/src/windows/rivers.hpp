#include "BaseWindow.h"
#include "misc\imguiutils.h"

const string defaultName = "RIVER_FOO";

//--------------------------------------------------------------------------------------
class RiversWindow : public BaseWindow
{
public:
    RiversWindow();
    bool Draw(const RenderWindow & window) final override;

private:
    int m_selectedRiver = -1;
    string m_editRiverName = defaultName;
};

//--------------------------------------------------------------------------------------
RiversWindow::RiversWindow() :
    BaseWindow(ICON_FA_WATER" Rivers")
{

}


//--------------------------------------------------------------------------------------
bool RiversWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    if (Begin(ICON_FA_WATER " Rivers###Rivers", &m_visible))
    {
        PushDisabled(!g_map || g_map->getRivers().size() == 0);
        {
            if (ImGui::Button("Check Rivers"))
            {
                g_map->UpdateRivers(true);
            }
        }
        PopDisabled();

        ImVec2 buttonSize = ImVec2(64 * EditorOptions::Get().m_editorGlobalScale, ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().CellPadding.y * 2.0f);

        int riverCount = g_map ? (int)g_map->getRivers().size() : 0;
        float rowHeight = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().CellPadding.y * 2.0f;
        float tableHeight = ImGui::GetContentRegionAvail().y - 64.0f;
        float desiredHeight = rowHeight + (rowHeight + ImGui::GetStyle().CellPadding.y * 1.0f) * max(riverCount, 1) + +ImGui::GetStyle().CellPadding.y;
        ImVec2 tableSize(0.0f, std::min(tableHeight, desiredHeight));

        if (ImGui::BeginTable("Names###NamedRivers", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY, tableSize))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            if (g_map)
            {
                auto & rivers = g_map->getRivers();

                // Keep the selection valid if rivers were removed.
                if (m_selectedRiver >= (int)rivers.size())
                    m_selectedRiver = -1;

                for (int i = 0; i < (int)rivers.size(); ++i)
                {
                    RiverInfo & river = rivers[i];

                    ImGui::TableNextRow();
                    ImGui::PushID(i);

                    ImGui::TableSetColumnIndex(0);

                    //ImGui::SetNextItemWidth(-FLT_MIN);

                    DrawSmallColoredSquare(getRiverNameColor(i));
                    ImGui::SameLine();

                    if (ImGui::Selectable(river.prettyName.c_str(), m_selectedRiver == i))
                    {
                        m_selectedRiver = i;
                    }

                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("\"%s\" (#%u)", river.name.c_str(), i);

                    ImGui::TableSetColumnIndex(1);
                    PushDisabled(g_map == nullptr || g_map->getRivers()[i].vanilla);
                    if (ImGui::Button(ICON_FA_TRASH))
                    {
                        vector<RiverInfo> & rivers = g_map->getRivers();
                        rivers.erase(rivers.begin() + i);
                        m_selectedRiver = i-1;
                    }
                    PopDisabled();
                    if (ImGui::IsItemHovered())
                    {
                        if (!g_map)
                            ImGui::SetTooltip("No map loaded");
                        else if (g_map && m_selectedRiver < g_map->getRivers().size() && g_map->getRivers()[m_selectedRiver].vanilla)
                            ImGui::SetTooltip("Vanilla rivers cannot be removed");
                        else
                            ImGui::SetTooltip("Delete %s", river.name.c_str());
                    }
                    ImGui::PopID();
                }
            }
            
            if (riverCount == 0)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("  ");
                ImGui::TableSetColumnIndex(1);
                PushDisabled(true);
                ImGui::Button(ICON_FA_TRASH);
                PopDisabled();
            }

            ImGui::EndTable();

            char temp[256];            

            //sprintf(temp, g_map && m_selectedRiver < g_map->getRivers().size() ? g_map->getRivers()[m_selectedRiver].prettyName.c_str() : "");
            //ImGui::InputText("###DeleteRiverName", temp, ImGuiInputTextFlags_ReadOnly);
            //ImGui::SameLine();
            //PushDisabled(g_map == nullptr || m_selectedRiver == -1 || g_map->getRivers()[m_selectedRiver].vanilla);
            //if (ImGui::Button("Remove", buttonSize))
            //{
            //    vector<RiverInfo> & rivers = g_map->getRivers();
            //    rivers.erase(rivers.begin() + m_selectedRiver);
            //    m_selectedRiver--;
            //}
            //PopDisabled();
            //if (ImGui::IsItemHovered())
            //{
            //    if (!g_map)
            //        ImGui::SetTooltip("No map loaded");
            //
            //    if (g_map && m_selectedRiver < g_map->getRivers().size() && g_map->getRivers()[m_selectedRiver].vanilla)
            //        ImGui::SetTooltip("Vanilla rivers cannot be removed");
            //}

            PushDisabled(g_map == nullptr);
            sprintf(temp, m_editRiverName.c_str());
            if (ImGui::InputText("###NewRiverName", temp, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                m_editRiverName = temp;
            }
            PopDisabled();

            ImGui::SameLine();

            PushDisabled(g_map == nullptr || m_editRiverName.length() == 0);
            if (ImGui::Button("Add", buttonSize))
            {
                auto & rivers = g_map->getRivers();
                rivers.push_back(RiverInfo(m_editRiverName, false));
                m_editRiverName = defaultName;
            }
            PopDisabled();
            if (ImGui::IsItemHovered())
            {
                if (!g_map)
                    ImGui::SetTooltip("No map loaded");
            }

            
        }
    }

    ImGui::End();

    return needRefresh;
}
