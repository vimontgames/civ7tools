#include "BaseWindow.h"

//--------------------------------------------------------------------------------------
class ConsoleWindow : public BaseWindow
{
public:
    ConsoleWindow();
    ~ConsoleWindow();

    bool Draw(const RenderWindow & window) final override;

private:
    void ClearConsoleWindow();
    int TextEditCallback(ImGuiInputTextCallbackData * data);
    void ExecuteConsoleCommand(const char * command_line);

private:
    char                    g_inputBuffer[256];
    ImVector<const char *>  g_commands;
    ImVector<char *>        g_history;
    int                     g_historyPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter         g_filter;
    bool                    g_autoscroll;
    bool                    g_scrollToBottom;

    friend int TextEditCallbackStub(ImGuiInputTextCallbackData * data);
};

//--------------------------------------------------------------------------------------
ConsoleWindow::ConsoleWindow() : 
    BaseWindow("Console")
{
    memset(g_inputBuffer, 0, sizeof(g_inputBuffer));
    g_historyPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
    g_commands.push_back("HELP");
    g_commands.push_back("HISTORY");
    g_commands.push_back("CLEAR");
    g_commands.push_back("CLASSIFY");
    g_autoscroll = true;
    g_scrollToBottom = false;
}

//--------------------------------------------------------------------------------------
ConsoleWindow::~ConsoleWindow()
{

}

//--------------------------------------------------------------------------------------
void ConsoleWindow::ClearConsoleWindow()
{
    //Kernel::getLogger()->Clear();
}

//--------------------------------------------------------------------------------------
int TextEditCallbackStub(ImGuiInputTextCallbackData * data)
{
    ConsoleWindow * console = (ConsoleWindow *)data->UserData;
    return console->TextEditCallback(data);
}

//--------------------------------------------------------------------------------------
int ConsoleWindow::TextEditCallback(ImGuiInputTextCallbackData * data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char * word_end = data->Buf + data->CursorPos;
            const char * word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char *> candidates;
            for (int i = 0; i < g_commands.Size; i++)
                if (Strnicmp(g_commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(g_commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                //VG_INFO("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                //VG_INFO("Possible matches:\n");
                //for (int i = 0; i < candidates.Size; i++)
                    //VG_INFO("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = g_historyPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (g_historyPos == -1)
                    g_historyPos = g_history.Size - 1;
                else if (g_historyPos > 0)
                    g_historyPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (g_historyPos != -1)
                    if (++g_historyPos >= g_history.Size)
                        g_historyPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != g_historyPos)
            {
                const char * history_str = (g_historyPos >= 0) ? g_history[g_historyPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------------
void ConsoleWindow::ExecuteConsoleCommand(const char * command_line)
{
    //VG_INFO("# %s", command_line);

    // Insert into history. First find match and delete it so it can be pushed to the back.
    // This isn't trying to be smart or optimal.
    g_historyPos = -1;
    for (int i = g_history.Size - 1; i >= 0; i--)
        if (Stricmp(g_history[i], command_line) == 0)
        {
            free(g_history[i]);
            g_history.erase(g_history.begin() + i);
            break;
        }
    g_history.push_back(Strdup(command_line));

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0)
    {
        ClearConsoleWindow();
    }
    else if (Stricmp(command_line, "HELP") == 0)
    {
        //VG_INFO("Commands:");
        //for (int i = 0; i < m_commands.Size; i++)
            //VG_INFO("- %s", m_commands[i]);
    }
    else if (Stricmp(command_line, "HISTORY") == 0)
    {
        int first = g_history.Size - 10;
        //for (int i = first > 0 ? first : 0; i < m_history.Size; i++)
            //VG_INFO("%3d: %s\n", i, m_history[i]);
    }
    else
    {
        //VG_ERROR("Unknown command: '%s'\n", command_line);
    }

    // On command input, we scroll to bottom even if AutoScroll==false
    g_scrollToBottom = true;
}

//--------------------------------------------------------------------------------------
bool ConsoleWindow::Draw(const RenderWindow & window)
{
    bool needRefresh = false;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if(Begin("Console", &m_visible))
    {
        const auto warningColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        const auto errorColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g., IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                m_visible = false;
            ImGui::EndPopup();
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &g_autoscroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");

        ImGui::SameLine();

        bool clearLog = false;
        bool copyToClipboard = false;// ImGui::Button("Copy");

        ImGui::SameLine();

        //auto * logger = Kernel::getLogger();
        //logger->Lock();
        //{
        //    m_filter.Draw("Filter", 256);
        //
        //    ImGui::SameLine();
        //
        //    uint infoCount = 0, warningCount = 0, errorCount = 0;
        //    const auto count = logger->GetLogCount();
        //    for (uint i = 0; i < count; i++)
        //    {
        //        const auto & item = logger->GetLog(i);
        //        switch (item.level)
        //        {
        //            case Level::Info:
        //                infoCount += item.count;
        //                break;
        //
        //            case Level::Warning:
        //                warningCount += item.count;
        //                break;
        //
        //            case Level::Error:
        //                errorCount += item.count;
        //                break;
        //        }
        //    }
        //
        //    auto buttonSize = style::button::SizeSmall;
        //    buttonSize.x *= 2;
        //
        //    ImVec2 size = GetWindowContentRegionSize();
        //    ImGuiStyle & style = ImGui::GetStyle();
        //
        //    char infoLabel[256];
        //    sprintf_s(infoLabel, "%s %u", style::icon::Info, infoCount);
        //    auto infoLabelWidth = ImGui::CalcTextSize(infoLabel).x + style.FramePadding.x * 2.0f;
        //    strcat(infoLabel, "###ShowInfo");
        //
        //    char warningLabel[256];
        //    sprintf_s(warningLabel, "%s %u", style::icon::Warning, warningCount);
        //    auto warningLabelWidth = ImGui::CalcTextSize(warningLabel).x + style.FramePadding.x * 2.0f;
        //    strcat(warningLabel, "###ShowWarning");
        //
        //    char errorLabel[256];
        //    sprintf_s(errorLabel, "%s %u", style::icon::Error, errorCount);
        //    auto errorLabelWidth = ImGui::CalcTextSize(errorLabel).x + style.FramePadding.x * 2.0f;
        //    strcat(errorLabel, "###ShowError");
        //
        //    float offset = size.x - infoLabelWidth - warningLabelWidth - errorLabelWidth - style.FramePadding.x;
        //
        //    auto & options = getConsoleOptions();
        //
        //    bool showInfos, showWarnings, showErrors;
        //
        //    PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
        //    {
        //        ImGui::SetCursorPosX(offset);
        //
        //        showInfos = asBool(LevelFlags::Info & options.m_levels);
        //        if (ImGui::TooltipButton(infoLabel, showInfos, true, "Show Infos"))
        //        {
        //            showInfos = !showInfos;
        //
        //            if (showInfos)
        //                options.m_levels |= LevelFlags::Info;
        //            else
        //                options.m_levels &= ~LevelFlags::Info;
        //        }
        //
        //        ImGui::SameLine();
        //
        //        if (warningCount > 0)
        //            ImGui::PushStyleColor(ImGuiCol_Text, warningColor);
        //        showWarnings = asBool(LevelFlags::Warning & options.m_levels);
        //        if (ImGui::TooltipButton(warningLabel, showWarnings, true, "Show Warnings"))
        //        {
        //            showWarnings = !showWarnings;
        //
        //            if (showWarnings)
        //                options.m_levels |= LevelFlags::Warning;
        //            else
        //                options.m_levels &= ~LevelFlags::Warning;
        //        }
        //        if (warningCount > 0)
        //            ImGui::PopStyleColor();
        //
        //        ImGui::SameLine();
        //
        //        if (errorCount > 0)
        //            ImGui::PushStyleColor(ImGuiCol_Text, errorColor);
        //        showErrors = asBool(LevelFlags::Error & options.m_levels);
        //        if (ImGui::TooltipButton(errorLabel, showErrors, true, "Show Errors"))
        //        {
        //            showErrors = !showErrors;
        //
        //            if (showErrors)
        //                options.m_levels |= LevelFlags::Error;
        //            else
        //                options.m_levels &= ~LevelFlags::Error;
        //        }
        //        if (errorCount > 0)
        //            ImGui::PopStyleColor();
        //    }
        //    PopStyleVar();
        //
        //    //ImGui::SameLine();
        //    //
        //    //if (ImGui::SmallButton("Add Info"))
        //    //    VG_INFO("[Console] something went wrong");
        //    //
        //    //ImGui::SameLine();
        //    //
        //    //if (ImGui::SmallButton("Add Warning"))
        //    //    VG_WARNING("[Console] something went wrong");
        //    //
        //    //ImGui::SameLine();
        //    //
        //    //if (ImGui::SmallButton("Add Error")) 
        //    //    VG_ERROR("[Console] something went wrong");
        //
        //    ImGui::Separator();
        //
        //    // Reserve enough left-over height for 1 separator + 1 input text
        //    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        //    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        //    if (ImGui::BeginPopupContextWindow())
        //    {
        //        if (ImGui::Selectable("Clear"))
        //            clearLog = true;
        //        if (ImGui::Selectable("Copy"))
        //            copyToClipboard = true;
        //        ImGui::EndPopup();
        //    }
        //
        //    // Display every line as a separate entry so we can change their color or add custom widgets.
        //    // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        //    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
        //    // to only process visible items. The clipper will automatically measure the height of your first item and then
        //    // "seek" to display only items in the visible area.
        //    // To use the clipper we can replace your standard loop:
        //    //      for (int i = 0; i < Items.Size; i++)
        //    //   With:
        //    //      ImGuiListClipper clipper;
        //    //      clipper.Begin(Items.Size);
        //    //      while (clipper.Step())
        //    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        //    // - That your items are evenly spaced (same height)
        //    // - That you have cheap random access to your elements (you can access them given their index,
        //    //   without processing all the ones before)
        //    // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
        //    // We would need random-access on the post-filtered list.
        //    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
        //    // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
        //    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
        //    // to improve this example code!
        //    // If your items are of variable height:
        //    // - Split them into same height items would be simpler and facilitate random-seeking into your list.
        //    // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
        //    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        //    if (copyToClipboard)
        //        ImGui::LogToClipboard();
        //
        //    //const auto count = logger->GetLogCount();
        //    for (uint i = 0; i < count; i++)
        //    {
        //        const auto & item = logger->GetLog(i);
        //
        //        switch (item.level)
        //        {
        //            case Level::Info:
        //                if (!showInfos)
        //                    continue;
        //                break;
        //
        //            case Level::Warning:
        //                if (!showWarnings)
        //                    continue;
        //                break;
        //
        //            case Level::Error:
        //                if (!showErrors)
        //                    continue;
        //                break;
        //        }
        //
        //        char fullmsg[8192];
        //
        //        if (item.count > 1)
        //            sprintf(fullmsg, "[%s] %s (x%u)", item.category.c_str(), item.message.c_str(), item.count);
        //        else
        //            sprintf(fullmsg, "[%s] %s", item.category.c_str(), item.message.c_str());
        //
        //        if (!m_filter.PassFilter(fullmsg))
        //            continue;
        //
        //        ImVec4 color;
        //        bool has_color = false;
        //
        //        switch (item.level)
        //        {
        //            case Level::Error:
        //                color = errorColor;
        //                has_color = true;
        //                break;
        //
        //            case Level::Warning:
        //                color = warningColor;
        //                has_color = true;
        //                break;
        //        }
        //
        //        if (has_color)
        //            ImGui::PushStyleColor(ImGuiCol_Text, color);
        //        ImGui::TextUnformatted(fullmsg);
        //        if (has_color)
        //            ImGui::PopStyleColor();
        //    }
        //}
        //logger->Unlock();

        if (copyToClipboard)
            ImGui::LogFinish();

        if (clearLog)
            ClearConsoleWindow();

        if (g_scrollToBottom || (g_autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        g_scrollToBottom = false;

        //ImGui::PopStyleVar();
        //ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        PushItemWidth(-1);
        if (ImGui::InputText("###Input", g_inputBuffer, IM_ARRAYSIZE(g_inputBuffer), input_text_flags, &TextEditCallbackStub, (void**)this))
        {
            char * s = g_inputBuffer;
            Strtrim(s);
            if (s[0])
                ExecuteConsoleCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }
        PopItemWidth();

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        End();
    }

    return needRefresh;
}