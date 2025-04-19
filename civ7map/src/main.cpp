#include "imgui.h" 
#include "imgui_internal.h"
#include "imgui-SFML.h" 
#include "ImGuiFileBrowser.h"
#include "Shlobj.h"
#include "stringconvert.h"
#include "guistyle.h"
#include "maths.h"
#include "map/map.h"
#include "shader/common.h"
#include "undoredo/UndoRedo.h"

using namespace std;
using namespace sf;
using namespace ImGui;
using namespace ImGui::SFML;
using namespace imgui_addons;

bool g_hasFocus = true;

const u32 g_initScreenWidth = 1920;
const u32 g_initScreenHeight = 1080;

u32 g_screenWidth = g_initScreenWidth;
u32 g_screenHeight = g_initScreenHeight;

static ImGuiFileBrowser g_fileDialog;
static string g_myDocumentsPath;
string g_currentWorkingDirectory;
static float g_comboxItemWidth = 120;
static const u32 g_fixedTextLengthShort = 10;
static const u32 g_fixedTextLengthLarge = 20;

bool g_saveFileDialog = false;
//bool g_saveMapDataOnly = false;

const char * g_saveImGuiIniPath = nullptr;
bool g_importFile = false;
bool g_createMap = false;

string g_newMapName = "";
int g_newMapSize[2] = { 84, 54 };
MapSize g_newMapSizeType = MapSize::Standard;

vector<Map*> g_maps;
static Map * g_map = nullptr;

Vector2i g_hoveredCell = Vector2i(-1, -1);
Vector2i g_selectedCell = Vector2i(-1, -1);

Vector2i g_selectedRectBeginEdit = Vector2i(-1, -1);
Vector2i g_selectedRectEndEdit = Vector2i(-1, -1);

Vector2i g_selectedRectMin = Vector2i(-1, -1);
Vector2i g_selectedRectMax = Vector2i(-1, -1);
bool g_selectingRect = false;

ImFont * font = nullptr;

#include "windows/help.hpp"
#include "windows/debug.hpp"
#include "windows/info.hpp"
#include "windows/display.hpp"
#include "windows/console.hpp"
#include "windows/inspector.hpp"
#include "windows/paint.hpp"

static vector<BaseWindow *> g_windows;

static const char * newMap = ICON_FA_FOLDER_PLUS" Create Map";
static const char * importMap = ICON_FA_FOLDER_OPEN" Import Map";
static const char * exportMap = ICON_FA_FLOPPY_DISK" Export Map";

//--------------------------------------------------------------------------------------
class dbg_stream_for_cout : public stringbuf
{
public:
    ~dbg_stream_for_cout() { sync(); }
    int sync()
    {
        LOG_WARNING(str().c_str());
        str(string()); // Clear the string buffer
        return 0;
    }
};
dbg_stream_for_cout g_DebugStreamFor_cout;

#include "imgui_internal.h"

const int g_version_major = 0;
const int g_version_minor = 44;
const char * g_appName = "Civ7Map";

//--------------------------------------------------------------------------------------
int main() 
{
    // init random seed
    srand((u32)time(NULL));

    // Redirect SFML errors to debug output
    cout.rdbuf(&g_DebugStreamFor_cout);
    streambuf* previous = sf::err().rdbuf(&g_DebugStreamFor_cout);

    // save dir
    TCHAR cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);
    g_currentWorkingDirectory = string(cwd);

    // default path
    //WCHAR userFolder[MAX_PATH];
    if (IsDebuggerPresent())
    {
        g_myDocumentsPath = g_currentWorkingDirectory + "\\mods";
    }
    //else
    //{
    //    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, CSIDL_LOCAL_APPDATA, userFolder)))
    //        g_myDocumentsPath = ws2s(wstring(userFolder)) + "\\Firaxis Games\\Sid Meier's Civilization VII\\Mods";
    //}

    sf::ContextSettings contextSettings;
                        contextSettings.sRgbCapable = false;

    RenderWindow mainWindow(VideoMode(g_screenWidth, g_screenHeight), "", Style::Titlebar | Style::Resize | Style::Close, contextSettings);
    mainWindow.setFramerateLimit(60);
    Init(mainWindow, false);

    const string windowTitle = fmt::sprintf("%s %i.%i", g_appName, g_version_major, g_version_minor);
    LOG_INFO(windowTitle.c_str());

    ShaderManager::init();

    // Setup dear ImGui
    ImGui::CreateContext();

    // Load custom font with icons
    const float baseFontSize = 16;
    const float iconFontSize = baseFontSize;

    ImGuiIO & io = ImGui::GetIO();
    ImFont * font1 = io.Fonts->AddFontFromFileTTF("data/fonts/ubuntu/UbuntuMono-R.ttf", 16);

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    font = io.Fonts->AddFontFromFileTTF("data/fonts/Font-Awesome-6.x/" FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges);

    ImGui::SFML::UpdateFontTexture();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingTransparentPayload = true;

    SetupImGuiStyle();

    g_windows.push_back(new ConsoleWindow());
    g_windows.push_back(new DebugWindow());
    g_windows.push_back(new DisplayWindow());
    g_windows.push_back(new HelpWindow());
    g_windows.push_back(new InfoWindow());
    g_windows.push_back(new InspectorWindow());
    g_windows.push_back(new PaintWindow());

    // static init
    Map::loadIcons(true);
    Map::loadFlags(true);

    Clock deltaClock;
    while (mainWindow.isOpen()) 
    {
        string title = windowTitle;
        if (g_map)
            title += string(" - ") + g_map->getBaseName();

        mainWindow.setTitle(title.c_str());

        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            ProcessEvent(event);

            switch (event.type)
            {
                case Event::Closed:
                    mainWindow.close();
                    break;

                case Event::GainedFocus:
                    g_hasFocus = true;
                    break;

                case Event::LostFocus:
                    g_hasFocus = false;
                    break;

                case Event::MouseWheelMoved:
                    if (g_map)
                        g_map->m_mouseWheelDelta = (float)event.mouseWheel.delta;
                    break;

                case Event::Resized:
                {
                    for (auto & map : g_maps)
                    {
                        Vector2f offset = Vector2f(float(g_screenWidth / 2) - float(event.size.width / 2), float(g_screenHeight / 2) - float(event.size.height / 2));

                        map->m_cameraOffset += offset;
                        map->m_cameraPreviousOffset += offset;
                        map->m_cameraZoom = map->m_cameraZoom / (float(event.size.height) / float(g_screenHeight));

                        g_screenWidth = event.size.width;
                        g_screenHeight = event.size.height;

                        mainWindow.setSize(Vector2u(g_screenWidth, g_screenHeight));
                        sf::FloatRect visibleArea(0, 0, (float)g_screenWidth, (float)g_screenHeight);
                        mainWindow.setView(sf::View(visibleArea));

                        map->refresh();
                    }
                }
                break;
            }
        }

        Update(mainWindow, deltaClock.restart());

        bool showUI = true;

        ImGuiViewport * viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_None);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &showUI, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);   

        bool needRefresh = false;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(newMap))
                    g_createMap = true;
 
                if (ImGui::MenuItem(importMap))
                    g_importFile = true;

                if (!g_map)
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

                if (ImGui::MenuItem(exportMap))
                    g_saveFileDialog = true;

                if (!g_map)
                    ImGui::PopItemFlag();

                ImGui::Separator();

                if (ImGui::MenuItem("Exit"))
                    mainWindow.close();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Windows"))
            {
                for (auto * window : g_windows)
                {
                    bool visible = window->IsVisible();
                    if (ImGui::MenuItem(window->Name().c_str(), nullptr, visible))
                        window->SetVisible(!visible);
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Maps"))
            {
                if (g_maps.size())
                {
                    for (u32 i = 0; i < g_maps.size(); ++i)
                    {
                        Map * map = g_maps[i];

                        if (ImGui::MenuItem(map->getShortName().c_str(), nullptr, map == g_map))
                            g_map = map;
                    }
                }
                else
                {
                    ImGui::TextDisabled("Empty");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Options"))
            {
                extern float g_globalScale;
                if (ImGui::DragFloat("UI Scale", &g_globalScale, 0.25f, 1.0f, 2.0f, "%.1f", 1.0f))
                    SetupImGuiStyle();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        for (auto * window : g_windows)
        {
            if (window->IsVisible())
                needRefresh |= window->Draw(mainWindow);
        }

        if (!g_maps.empty())
        {
            for (u32 m = 0; m < g_maps.size(); ++m)
            {
                Map * map = g_maps[m];

                if (!map->m_isDocked)
                {
                    auto dock_id_center = ImGui::DockBuilderGetCentralNode(dockspace_id);
                    DockBuilderDockWindow(g_map->getShortName().c_str(), dock_id_center->ID);
                    map->m_isDocked = true;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

                if (Begin(map->getShortName().c_str(), &map->m_isOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse /*| ImGuiWindowFlags_NoBackground*/))
                {
                    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
                        g_map = map;

                    ImGuiContext & g = *GImGui;
                    ImGuiIO & io = g.IO;

                    if (0 != (io.KeyMods & ImGuiKeyModFlags_Ctrl))
                    {
                        if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Z]))
                        {
                            UndoRedoStack::Undo();
                        }
                        else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Y]))
                        {
                            UndoRedoStack::Redo();
                        }
                    }

                    ImGui::ImageButton(map->m_renderTexture, (sf::Vector2f)map->m_renderTexture.getSize(), 0, sf::Color::White, sf::Color::White);

                    if (IsItemHovered())
                        map->m_isHovered = true;
                    else
                        map->m_isHovered = false;

                    if (g_map == map)
                    {
                        ImVec2 topLeft = ImGui::GetWindowPos();
                        ImVec2 size = ImGui::GetWindowSize();
                        const auto mousePos = Mouse::getPosition(mainWindow);
                        ImVec2 relativeMousePos = ImVec2(mousePos.x - topLeft.x, mousePos.y - topLeft.y - ImGui::GetFrameHeight());

                        if (relativeMousePos.x > 0 && relativeMousePos.y > 0 && relativeMousePos.x  < size.x && relativeMousePos.y < (size.y- ImGui::GetFrameHeight()))
                        {
                            //if (ImGui::IsMouseDown(0))
                            //{
                            //    int i = 42;
                            //}

                            float2 renderTargetSize = float2((float)map->m_renderTexture.getSize().x, (float)map->m_renderTexture.getSize().y);
                            Vector2f size = Vector2f(renderTargetSize.x, renderTargetSize.y);

                            sf::View view;
                            view.setCenter(sf::Vector2f(float(g_screenWidth) * 0.5f, (float)(g_screenHeight) * 0.5f));
                            view.setSize(sf::Vector2f(float(g_screenWidth), (float)g_screenHeight));
                            view.zoom(g_map->m_cameraZoom); // zeng
                            view.move(g_map->m_cameraOffset);

                            float ar = (float(g_screenWidth) / float(map->m_width)) / (float(g_screenHeight) / float(map->m_height));
                        
                            // Convert to world coordinates using the current SFML view (handles zoom & offset)
                            sf::Vector2i mousePixel = sf::Vector2i((int)relativeMousePos.x, (int)relativeMousePos.y);
                            sf::Vector2f uv = mainWindow.mapPixelToCoords(mousePixel, view);

                            uv.y /= ar;

                            uv.x *= 0.5f;
                            uv.y *= 0.5f;

                            uv.x /= (float)size.x+1;
                            uv.y /= (float)size.y+1;

                            float2 temp;
                            float2 texSize = float2((float)map->m_bitmaps[(int)MapBitmap::TerrainData].image.getSize().x, (float)map->m_bitmaps[(int)MapBitmap::TerrainData].image.getSize().y);

                            float w = (float)g_map->m_width;
                            float h = (float)g_map->m_height;

                            switch (map->m_gridType)
                            {
                                case GridType::Regular:
                                    temp = getTileUV(float2(uv.x - 0.0f / w, uv.y * 0.5f), texSize, 0);
                                    break;

                                case GridType::Offset:
                                    temp = getTileUV(float2(uv.x + 0.5f / w, uv.y * 0.5f), texSize, PASS_FLAG_OFFSET);
                                    break;

                                case GridType::Hexagon:
                                    temp = getTileUV(float2(uv.x - 0.0f / w, uv.y * 0.5f), texSize, PASS_FLAG_HEXAGON);
                                    break;
                            }

                            uv.x = temp.x * 1.0f;
                            uv.y = temp.y * 2.0f;

                            float hS = 0.0f, hE = 0.0f;
                            if (map->m_gridType == GridType::Offset)
                            {
                                const int Y = (int)min(floor((uv.y) * h), (h - 1));
                                if (0 == (Y & 1))
                                    hS = hE = +1.0f / w;
                            }
                            else if (map->m_gridType == GridType::Hexagon)
                            {
                                const int Y = (int)min(floor((uv.y) * h), (h - 1));
                                if (0 == (Y & 1))
                                {
                                    hE = +0.5f / w;
                                    hS = +0.5f / w;
                                }
                                else
                                {
                                    hS = -0.5f / w;
                                    hE = -0.5f / w;
                                }
                            }

                            const bool tileJustSelected = ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_Space]);

                            if (uv.x > 0.0f + hS && uv.y > 0.0f && uv.x < 1.0f + hE && uv.y < 1.0f)
                            {
                                Vector2i cell;

                                switch (g_map->m_gridType)
                                {
                                    default:
                                        cell.x = (int)min(floor((uv.x) * w), (w - 1));
                                        cell.y = (int)min(floor((uv.y) * h), (h - 1));
                                        break;

                                    case GridType::Offset:
                                        cell.x = (int)min(floor((uv.x) * w), (w));
                                        cell.y = (int)min(floor((uv.y) * h), (h - 1));

                                        if (0 == (cell.y & 1))
                                            cell.x = (int)min((int)cell.x - 1, (int)(w - 1));
                                        else
                                            cell.x = (int)min((int)cell.x + 0, (int)(w - 1));
                                        break;

                                    case GridType::Hexagon:
                                        cell.x = (int)min(round((uv.x) * w), (w));
                                        cell.y = (int)min(round((uv.y) * h), (h));

                                        if (0 == (cell.y & 1))
                                            cell.x = (int)min((int)cell.x-1, (int)(w - 1));
                                        else
                                            cell.x = (int)min((int)cell.x +0, (int)(w - 1));
                                        break;
                                }

                                cell.y = (int)h - cell.y - 1;

                                cell.x = clamp(cell.x, 0, (int)(map->m_width-1));
                                cell.y = clamp(cell.y, 0, (int)(map->m_height-1));

                                g_hoveredCell = cell;
                                if (tileJustSelected)
                                    g_selectedCell = cell;

                                // User can select area if shift pressed
                                if (0 != (io.KeyMods & ImGuiKeyModFlags_Shift))
                                {

                                    if (Mouse::isButtonPressed(Mouse::Left))
                                    {
                                        if (!g_selectingRect)
                                        {
                                            g_selectingRect = true;
                                            g_selectedRectBeginEdit = g_hoveredCell;
                                            g_selectedRectEndEdit = g_hoveredCell;
                                        }
                                        else
                                        {
                                            g_selectedRectEndEdit = g_hoveredCell;
                                        }
                                    }
                                    else
                                    {
                                        if (g_selectingRect)
                                        {
                                            g_selectingRect = false;
                                            g_selectedRectEndEdit = g_hoveredCell;
                                        }
                                    }

                                    g_selectedRectMin.x = min(g_selectedRectBeginEdit.x, g_selectedRectEndEdit.x);
                                    g_selectedRectMin.y = min(g_selectedRectBeginEdit.y, g_selectedRectEndEdit.y);

                                    g_selectedRectMax.x = max(g_selectedRectBeginEdit.x, g_selectedRectEndEdit.x);
                                    g_selectedRectMax.y = max(g_selectedRectBeginEdit.y, g_selectedRectEndEdit.y);
                                }

                                if (0 != (io.KeyMods & ImGuiKeyModFlags_Ctrl))
                                {
                                    if (g_selectedRectMax.x > g_selectedRectMin.x && g_selectedRectMax.y > g_selectedRectMin.y)
                                    {
                                        if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_C]))
                                        {
                                            map->copyRect(g_selectedRectMin, g_selectedRectMax);
                                            g_selectedRectBeginEdit = g_selectedRectEndEdit = g_selectedRectMin = g_selectedRectMax = Vector2i(-1, -1);
                                        }
                                        else if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_X]))
                                        {
                                            map->cutRect(g_selectedRectMin, g_selectedRectMax);
                                            g_selectedRectBeginEdit = g_selectedRectEndEdit = g_selectedRectMin = g_selectedRectMax = Vector2i(-1, -1);
                                        }
                                    }

                                    if (ImGui::IsKeyPressed(io.KeyMap[ImGuiKey_V]))
                                    {
                                        map->pasteRect(g_hoveredCell);
                                    }
                                }

                                const Civ7Tile tile = g_map->m_civ7TerrainType.get(cell.x, cell.y);

                                if (!ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup))
                                {
                                    ImGui::BeginTooltip();
                                    {
                                        ImGui::Text("%i,%i", cell.x, cell.y);
                                        ImGui::Separator();

                                        //DrawColor(g_map, tile.continent);
                                        //ImGui::SameLine();
                                        //ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                        //ImGui::Text("(%i)", (int)tile.continent);

                                        DrawColor(g_map, tile.terrain);
                                        ImGui::SameLine();
                                        ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                        ImGui::Text("(%i)", (int)tile.terrain);
                                
                                        DrawColor(g_map, tile.biome);
                                        ImGui::SameLine();
                                        ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                        ImGui::Text("(%i)", (int)tile.biome);

                                        DrawColor(g_map, tile.feature);
                                        ImGui::SameLine();
                                        ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                        ImGui::Text("(%i)",(int)tile.feature);

                                        DrawColor(g_map, tile.resource);
                                        ImGui::SameLine();
                                        ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                        ImGui::Text("(%i)", (int)tile.resource);

                                        ImGui::Separator();

                                        bool anyTSL = false;
                                        for (int c = 0; c < g_map->m_civilizations.size(); ++c)
                                        {
                                            const auto & civ = g_map->m_civilizations[c];
                                            for (int t = 0; t < civ.tsl.size(); ++t)
                                            {
                                                const auto & tsl = civ.tsl[t];
                                                if (tsl.pos.x == cell.x && tsl.pos.y == cell.y)
                                                {
                                                    float f3Color[] = { pow(civ.color.r, 1.0f/2.2f), pow(civ.color.g, 1.0f / 2.2f) , pow(civ.color.b, 1.0f / 2.2f) };
                                                    ImGui::ColorEdit3(fmt::sprintf("###%s", civ.name).c_str(), f3Color, ImGuiColorEditFlags_NoInputs);
                                                    ImGui::SameLine();
                                                    ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                                    ImGui::Text("%s (#%u)", civ.name.c_str(), t);
                                                    anyTSL = true;
                                                }
                                            }
                                        }

                                        #if _DEBUG0
                                        {
                                            //if (anyTSL)
                                            //    ImGui::Separator();

                                            //ImGui::Text("mouse %.0f,%.0f", relativeMousePos.x, relativeMousePos.y);
                                            //ImGui::Text("temp %.3f,%.3f", temp.x, temp.y);
                                            //ImGui::Text("uv %.3f,%.3f", uv.x, uv.y);
                                            float d = cellDist(int2(g_selectedCell.x, g_selectedCell.y), int2(cell.x, cell.y));
                                            ImGui::Text("Distance %.1f",d);

                                        }
                                        #endif
                                    }
                                    ImGui::EndTooltip();
                                }
                            }
                            else
                            {
                                g_hoveredCell = Vector2i(-1, -1);
                                if (tileJustSelected)
                                    g_selectedCell = Vector2i(-1, -1);
                            }
                        }
                    }
                }
                ImGui::End();

                ImGui::PopStyleVar(2);
            }

            for (u32 m = 0; m < g_maps.size(); ++m)
            {
                Map * map = g_maps[m];

                if (!map->m_isOpen)
                {
                    if (g_map == map)
                        g_map = nullptr;

                    g_maps.erase(g_maps.begin() + m);
                    break;
                }
            }
        }

        static bool demo = false;
        if (demo)
            ImGui::ShowDemoWindow(&demo);

        if (g_createMap)
        {
            ImGui::OpenPopup(newMap);
            SetCurrentDirectory(g_myDocumentsPath.c_str());
            ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
        }
        else if (g_importFile)
        {
            ImGui::OpenPopup(importMap);
            g_importFile = false;
            SetCurrentDirectory(g_myDocumentsPath.c_str());
            ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
        }
        else if (g_saveFileDialog)
        {
            ImGui::OpenPopup(exportMap);
            g_saveFileDialog = false;
            SetCurrentDirectory(g_myDocumentsPath.c_str());
            ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
        }

        if (g_createMap)
        {
            if (ImGui::BeginPopupModal(newMap, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                char temp[1024];
                sprintf(temp, "%s", g_newMapName.c_str());

                if (ImGui::InputText("Name", temp, sizeof(g_newMapName)))
                    g_newMapName = fmt::sprintf("%s", temp);

                if (ImGui::BeginCombo("###Size", asString(g_newMapSizeType).c_str()))
                {
                    for (auto val : enumValues<MapSize>())
                    {
                        bool isSelected = (g_newMapSizeType == val.first);
                        if (ImGui::Selectable(fmt::sprintf("%s (%i)", asString(val.first), (int)val.first).c_str(), isSelected))
                        {
                            g_newMapSizeType = val.first;

                            if ((int)val.first > 0)
                            {
                                g_newMapSize[0] = g_mapSizes[(int)val.first][0];
                                g_newMapSize[1] = g_mapSizes[(int)val.first][1];
                            }
                        }
                    }
                    ImGui::EndCombo();
                }                

                if (g_newMapSizeType != MapSize::Custom)
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

                ImGui::InputInt2("Size", g_newMapSize);

                if (g_newMapSizeType != MapSize::Custom)
                    ImGui::PopItemFlag();

                bool isSizeSupported = false;
                switch (g_newMapSizeType)
                {
                    case MapSize::Custom:
                        isSizeSupported = false;
                        break;

                    case MapSize::Tiny:
                    case MapSize::Small:
                    case MapSize::Standard:
                    case MapSize::Large:
                    case MapSize::Huge:
                    case MapSize::Greatest_Earth:
                    case MapSize::Massive:
                        isSizeSupported = true;
                        break;

                    case MapSize::Giant:
                    case MapSize::Ludicrous:
                        isSizeSupported = false;
                        break;
                }

                const bool isNameValid = !g_newMapName.empty();

                bool invalidSettings = !isSizeSupported || isNameValid;

                if (!invalidSettings)
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);    

                ImGui::Separator();

                if (ImGui::Button("OK"))
                {
                    // Create new map
                    Map * newMap = new Map();

                    SetCurrentDirectory(g_currentWorkingDirectory.c_str());
                    ImGui::GetIO().IniFilename = g_saveImGuiIniPath;

                    if (newMap->create(g_currentWorkingDirectory, g_newMapName, g_newMapSize[0], g_newMapSize[1]))
                    {
                        g_maps.push_back(newMap);
                        g_map = newMap;
                    }
 
                    ImGui::CloseCurrentPopup();
                    g_createMap = false;
                }

                if (!invalidSettings)
                    ImGui::PopItemFlag();

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup(); // Discard changes
                    g_newMapName = "";
                    g_createMap = false;
                }

                ImGui::EndPopup();
            }
           
        }
        else if (g_fileDialog.showFileDialog(importMap, ImGuiFileBrowser::DialogMode::OPEN, ImVec2(float(g_screenWidth)/2.0f, float(g_screenHeight)/2.0f), ".js"))
        {
            const string newFilePath = g_fileDialog.selected_path;

            if (EndsWith(newFilePath, "-map.js"))
            {
                // Create new map
                Map * newMap = new Map();
                newMap->m_mapPath = newFilePath;

                // Import it
                if (newMap->importFiles(g_currentWorkingDirectory))
                {
                    SetCurrentDirectory(g_currentWorkingDirectory.c_str());
                    ImGui::GetIO().IniFilename = g_saveImGuiIniPath;
                    newMap->refresh();
                    newMap->resetCamera();

                    // Is the map already loaded?
                    u32 mapIndex = u32(-1);
                    for (u32 i = 0; i < g_maps.size(); ++i)
                    {
                        if (g_maps[i]->m_mapPath == newFilePath)
                        {
                            mapIndex = i;
                            break;
                        }
                    }

                    if (u32(-1) == mapIndex)
                    {
                        g_maps.push_back(newMap);
                    }
                    else
                    {
                        SAFE_DELETE(g_maps[mapIndex]);
                        g_maps[mapIndex] = newMap;
                    }

                    // This is the new current edited map
                    g_map = newMap;
                }
            }
            else
            {
                LOG_ERROR("\"%s\" is not a valid map filename to import. Map filenames should end with \"-map.js\"", GetFilename(newFilePath).c_str());
            }
        }
        else if (g_fileDialog.showFileDialog(exportMap, ImGuiFileBrowser::DialogMode::SAVE, ImVec2(float(g_screenWidth) / 2.0f, float(g_screenHeight) / 2.0f), ".js"))
        {
            if (g_map)
            {
                auto * map = g_map;

                const string prevFilename = map->m_mapPath;

                string newFilePath = g_fileDialog.selected_path;

                bool canExport = false;

                if (EndsWith(newFilePath, "-map.js"))
                {
                    map->m_mapPath = newFilePath;
                    map->m_mapDataPath = Map::GetMapDataPathFromMapPath(map->m_mapPath);

                    // Dirty check to detect if we're using mod template (TODO: export options panel?)
                    bool useModTemplate = false;
                    const string mapTextPath = fmt::sprintf("%s\\text\\en_us\\MapText.xml", map->m_modFolder);
                    string data;
                    if (FileExists(mapTextPath) && ReadFile(mapTextPath, data))
                    {
                        if (-1 != data.find("(Civ7Map)"))
                            useModTemplate = true;
                    }

                    if (useModTemplate)
                        LOG_WARNING("Map \"%s\" is using Civ7Map mod template. All mod files will be updated.", GetFilename(newFilePath).c_str());
                    else
                        LOG_WARNING("Map \"%s\" is not using Civ7Map mod template. Only map and TSL files will be updated.", GetFilename(newFilePath).c_str());

                    map->exportFiles(g_currentWorkingDirectory, useModTemplate);

                    if (prevFilename != map->m_mapPath)
                        map->m_isDocked = false;
                }
                else
                {
                    LOG_ERROR("\"%s\" is not a valid map file to export. Map filenames should end with \"-map.js\"", GetFilename(newFilePath).c_str());
                }
            }

            SetCurrentDirectory(g_currentWorkingDirectory.c_str());
            ImGui::GetIO().IniFilename = g_saveImGuiIniPath;
        }

        if (needRefresh)
            g_map->refresh(true);

        // update "camera"
        const float panSpeed = 1.0f;
        const float zoomSpeed = 1.1f;

        if (g_map && g_map->m_isHovered)
        {
            static bool painting = false;

            if (g_hoveredCell.x >= 0 && g_hoveredCell.x < (int)g_map->m_width && g_hoveredCell.y >= 0 && g_hoveredCell.y < (int)g_map->m_height)
            {
                if (Mouse::isButtonPressed(Mouse::Left) && 0 == (io.KeyMods & ImGuiKeyModFlags_Shift) )
                {
                    if (!painting)
                    {
                        g_map->BeginPaint();
                        painting = true;
                    }

                    g_map->Paint(g_hoveredCell.x, g_hoveredCell.y);
                }
                else if (painting)
                {
                    g_map->EndPaint();
                    painting = false;
                }
            }

            if (Mouse::isButtonPressed(Mouse::Right))
            {
                if (!g_map->m_cameraPan)
                {
                    // begin pan
                    g_map->m_cameraPanOrigin = (Vector2f)Mouse::getPosition(mainWindow);
                    g_map->m_cameraPan = true;
                }
                else
                {
                    //continue pan
                    //g_map->cameraOffset = (g_map->cameraPanOrigin - (Vector2f)Mouse::getPosition(mainWindow)) * panSpeed * (g_map->cameraZoom*g_map->cameraZoom) + g_map->cameraPreviousOffset;
                    g_map->m_cameraOffset = (g_map->m_cameraPanOrigin - (Vector2f)Mouse::getPosition(mainWindow)) * panSpeed * g_map->m_cameraZoom + g_map->m_cameraPreviousOffset ;
                }
            }
            else if (g_map->m_cameraPan)
            {
                // end pan
                g_map->m_cameraPan = false;
                g_map->m_cameraPreviousOffset = g_map->m_cameraOffset;
            }

            if (0 != g_map->m_mouseWheelDelta)
            {
                if (g_map->m_mouseWheelDelta < 0)
                    g_map->m_cameraZoom *= zoomSpeed;
                else if (g_map->m_mouseWheelDelta > 0)
                    g_map->m_cameraZoom /= zoomSpeed;

                g_map->m_mouseWheelDelta = 0;
            }
        }

        if (sf::Keyboard::isKeyPressed(Keyboard::F6))
        {
            SetCurrentDirectory(g_currentWorkingDirectory.c_str());
            ShaderManager::update();
        }

        // Clear backbuffer
        mainWindow.clear(Color(0, 0, 0, 0));

        // Render map
        if (g_map)
            g_map->render(mainWindow);

        // Render UI
        Render(mainWindow);

        // Present backbuffer
        mainWindow.display();
    }

    for (auto * window : g_windows)
        delete(window);
    g_windows.clear();

    ShaderManager::deinit();
    Shutdown();

    return 0;
}
