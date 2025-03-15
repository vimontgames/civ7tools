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
    char                    m_inputBuffer[256];
    ImVector<const char *>  m_commands;
    ImVector<char *>        m_history;
    int                     m_historyPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter         m_filter;
    bool                    g_autoscroll;
    bool                    g_scrollToBottom;

    friend int TextEditCallbackStub(ImGuiInputTextCallbackData * data);
};

//--------------------------------------------------------------------------------------
ConsoleWindow::ConsoleWindow() : 
    BaseWindow(ICON_FA_TERMINAL" Console")
{
    memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
    m_historyPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
    m_commands.push_back("HELP");
    m_commands.push_back("HISTORY");
    m_commands.push_back("CLEAR");
    m_commands.push_back("CLASSIFY");
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
    g_logger.Clear();
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
            for (int i = 0; i < m_commands.Size; i++)
                if (Strnicmp(m_commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(m_commands[i]);

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
            const int prev_history_pos = m_historyPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (m_historyPos == -1)
                    m_historyPos = m_history.Size - 1;
                else if (m_historyPos > 0)
                    m_historyPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (m_historyPos != -1)
                    if (++m_historyPos >= m_history.Size)
                        m_historyPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != m_historyPos)
            {
                const char * history_str = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
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
    m_historyPos = -1;
    for (int i = m_history.Size - 1; i >= 0; i--)
        if (Stricmp(m_history[i], command_line) == 0)
        {
            free(m_history[i]);
            m_history.erase(m_history.begin() + i);
            break;
        }
    m_history.push_back(Strdup(command_line));

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
        int first = m_history.Size - 10;
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

    if(Begin(ICON_FA_TERMINAL" Console###Console", &m_visible))
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

        Logger * logger = &g_logger;
        {
            m_filter.Draw("Filter", 256);
        
            ImGui::SameLine();
        
            int infoCount = 0, warningCount = 0, errorCount = 0;
            const auto count = logger->GetLogCount();
            for (int i = 0; i < count; i++)
            {
                const auto & item = logger->GetLog(i);
                switch (item.level)
                {
                    case Level::Info:
                        infoCount ++;
                        break;
        
                    case Level::Warning:
                        warningCount ++;
                        break;
        
                    case Level::Error:
                        errorCount ++;
                        break;
                }
            }
        
            ImGuiStyle & style = ImGui::GetStyle();
        
            char infoLabel[256];
            sprintf_s(infoLabel, "Infos (%u)", infoCount);
            auto infoLabelWidth = ImGui::CalcTextSize(infoLabel).x + style.FramePadding.x * 2.0f;
            strcat(infoLabel, "###ShowInfo");
        
            char warningLabel[256];
            sprintf_s(warningLabel, "Warnings (%u)",warningCount);
            auto warningLabelWidth = ImGui::CalcTextSize(warningLabel).x + style.FramePadding.x * 2.0f;
            strcat(warningLabel, "###ShowWarning");
        
            char errorLabel[256];
            sprintf_s(errorLabel, "Errors (%u)", errorCount);
            auto errorLabelWidth = ImGui::CalcTextSize(errorLabel).x + style.FramePadding.x * 2.0f;
            strcat(errorLabel, "###ShowError");
        
            ImVec2 size = ImGui::GetContentRegionMax();
            float offset = size.x - infoLabelWidth - warningLabelWidth - errorLabelWidth - style.FramePadding.x;
                    
            static bool g_showInfos = false;
            static bool g_showWarnings = true;
            static bool g_showErrors = true;

            PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f));
            {
                ImGui::SetCursorPosX(offset);

                bool grayed = !g_showInfos;

                if (grayed)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

                if (ImGui::Button(infoLabel))
                    g_showInfos = !g_showInfos;

                if (grayed)
                    ImGui::PopStyleColor();
        
                ImGui::SameLine();
                
                grayed = !g_showWarnings;

                if (grayed)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
                else if (warningCount > 0)
                    ImGui::PushStyleColor(ImGuiCol_Text, warningColor);
              
                if (ImGui::Button(warningLabel))
                    g_showWarnings = !g_showWarnings;
                
                if (grayed || warningCount > 0)
                    ImGui::PopStyleColor();
        
                ImGui::SameLine();
        
                grayed = !g_showErrors;

                if (grayed)
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
                else if (errorCount > 0)
                    ImGui::PushStyleColor(ImGuiCol_Text, errorColor);
        
                if (ImGui::Button(errorLabel))
                    g_showErrors = !g_showErrors;

                if (grayed || errorCount > 0)
                    ImGui::PopStyleColor();
            }
            PopStyleVar();
                
            ImGui::Separator();
        
            // Reserve enough left-over height for 1 separator + 1 input text
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear"))
                    clearLog = true;
                if (ImGui::Selectable("Copy"))
                    copyToClipboard = true;
                ImGui::EndPopup();
            }
       
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); 
            if (copyToClipboard)
                ImGui::LogToClipboard();
        
            for (int i = 0; i < count; i++)
            {
                const auto & item = logger->GetLog(i);
        
                switch (item.level)
                {
                    case Level::Info:
                        if (!g_showInfos)
                            continue;
                        break;
        
                    case Level::Warning:
                        if (!g_showWarnings)
                            continue;
                        break;
        
                    case Level::Error:
                        if (!g_showErrors)
                            continue;
                        break;
                }
        
                char fullmsg[8192];
        
                sprintf(fullmsg, "%s", item.message.c_str());
        
                if (!m_filter.PassFilter(fullmsg))
                    continue;
        
                ImVec4 color;
                bool has_color = false;
        
                switch (item.level)
                {
                    case Level::Error:
                        color = errorColor;
                        has_color = true;
                        break;
        
                    case Level::Warning:
                        color = warningColor;
                        has_color = true;
                        break;
                }
        
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(fullmsg);
                if (has_color)
                    ImGui::PopStyleColor();
            }
        }

        if (copyToClipboard)
            ImGui::LogFinish();

        if (clearLog)
            ClearConsoleWindow();

        if (g_scrollToBottom || (g_autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        g_scrollToBottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        PushItemWidth(-1);
        if (ImGui::InputText("###Input", m_inputBuffer, IM_ARRAYSIZE(m_inputBuffer), input_text_flags, &TextEditCallbackStub, (void**)this))
        {
            char * s = m_inputBuffer;
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