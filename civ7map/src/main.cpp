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
static string g_currentWorkingDirectory;
static float g_comboxItemWidth = 120;
static const u32 g_fixedTextLengthShort = 10;
static const u32 g_fixedTextLengthLarge = 20;

bool g_saveFileDialog = false;
bool g_saveMapDataOnly = false;

const char * g_saveImGuiIniPath = nullptr;
bool g_importFile = false;
bool g_createMap = false;

enum class MapSize : int
{
    Custom = -1,

    Tiny,
    Small,
    Standart,
    Large,
    Huge
};

static const int g_mapSizes[enumCount<MapSize>()][2] =
{
    {60,38},
    {74,46},
    {84,54},
    {96,60},
    {106,66}
};

string g_newMapName = "";
int g_newMapSize[2] = { 84, 54 };
MapSize g_newMapSizeType = MapSize::Standart;

static vector<Map*> g_maps;
static Map * g_map = nullptr;

Vector2i g_hoveredCell = Vector2i(-1, -1);
Vector2i g_selectedCell = Vector2i(-1, -1);

#include "windows/help.hpp"
#include "windows/debug.hpp"
#include "windows/info.hpp"
#include "windows/display.hpp"
#include "windows/console.hpp"
#include "windows/inspector.hpp"

static vector<BaseWindow *> g_windows;

static const char * newMap = "Create new map mod";
static const char * importMap = "Import map";
static const char * exportMap = "Export all map mod files";
static const char * exportMapDataOnly = "Export map data only";

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
const int g_version_minor = 1;
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
    WCHAR userFolder[MAX_PATH];
    if (IsDebuggerPresent())
    {
        g_myDocumentsPath = g_currentWorkingDirectory + "\\mods";
    }
    else
    {
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, CSIDL_LOCAL_APPDATA, userFolder)))
            g_myDocumentsPath = ws2s(wstring(userFolder)) + "\\Firaxis Games\\Sid Meier's Civilization VII\\Mods";
    }

    sf::ContextSettings contextSettings;
                        contextSettings.sRgbCapable = false;

    RenderWindow mainWindow(VideoMode(g_screenWidth, g_screenHeight), "", Style::Titlebar | Style::Resize | Style::Close, contextSettings);
    mainWindow.setFramerateLimit(60);
    Init(mainWindow);

    const string windowTitle = fmt::sprintf("%s %i.%i", g_appName, g_version_major, g_version_minor);
    LOG_INFO(windowTitle.c_str());

    ShaderManager::init();

    // Setup dear ImGui
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingTransparentPayload = true;

    SetupImGuiStyle();

    g_windows.push_back(new ConsoleWindow());
    g_windows.push_back(new DebugWindow());
    g_windows.push_back(new DisplayWindow());
    g_windows.push_back(new HelpWindow());
    g_windows.push_back(new InfoWindow());
    g_windows.push_back(new InspectorWindow());

    // static init
    Map::loadIcons();

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
                {
                    g_createMap = true;
                }

                if (ImGui::MenuItem(importMap))
                {
                    g_importFile = true;
                }

                if (!g_map)
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

                if (ImGui::MenuItem(exportMap))
                {
                    g_saveFileDialog = true;
                    g_saveMapDataOnly = false;
                }

                if (ImGui::MenuItem(exportMapDataOnly))
                {
                    g_saveFileDialog = true;
                    g_saveMapDataOnly = true;
                }

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
                            if (ImGui::IsMouseDown(0))
                            {
                                int i = 42;
                            }

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
                                if (ImGui::IsMouseDown(0))
                                    g_selectedCell = cell;

                                const Civ7Tile tile = g_map->m_civ7TerrainType.get(cell.x, cell.y);

                                ImGui::BeginTooltip();
                                {
                                    ImGui::Text("%i,%i", cell.x, cell.y);
                                    ImGui::Separator();

                                    DrawColor(g_map, tile.continent);
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosY(GetCursorPosY() + 4);
                                    ImGui::Text("(%i)", (int)tile.continent);

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

                                    #if _DEBUG
                                    {
                                        ImGui::Text("mouse %.0f,%.0f", relativeMousePos.x, relativeMousePos.y);
                                        ImGui::Text("temp %.3f,%.3f", temp.x, temp.y);
                                        ImGui::Text("uv %.3f,%.3f", uv.x, uv.y);
                                    }
                                    #endif
                                }
                                ImGui::EndTooltip();
                            }
                            else
                            {
                                g_hoveredCell = Vector2i(-1, -1);
                                if (ImGui::IsMouseDown(0))
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
            if (g_saveMapDataOnly)
            {
                ImGui::OpenPopup(exportMapDataOnly);
                g_saveFileDialog = false;
                SetCurrentDirectory(g_myDocumentsPath.c_str());
                ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
            }
            else
            {
                ImGui::OpenPopup(exportMap);
                g_saveFileDialog = false;
                SetCurrentDirectory(g_myDocumentsPath.c_str());
                ImGui::GetIO().IniFilename = nullptr; // Prevents imgui.ini file being save during dialogs
            }
        }

        if (g_createMap)
        {
            if (ImGui::BeginPopupModal(newMap, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                char temp[1024];
                sprintf(temp, "%s", g_newMapName.c_str());

                if (ImGui::InputText("Name", temp, sizeof(g_newMapName)))
                    g_newMapName = fmt::sprintf("%s", temp);

                int selectedIndex = -2;
                if (ImGui::BeginCombo("###Size", asString(g_newMapSizeType).c_str()))
                {
                    for (auto val : enumValues<MapSize>())
                    {
                        bool isSelected = (selectedIndex == (int)val.first);
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

                ImGui::InputInt2("Size", g_newMapSize);

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
        else if (g_fileDialog.showFileDialog(exportMap, ImGuiFileBrowser::DialogMode::SAVE, ImVec2(float(g_screenWidth) / 2.0f, float(g_screenHeight) / 2.0f), ".js")
              || g_fileDialog.showFileDialog(exportMapDataOnly, ImGuiFileBrowser::DialogMode::SAVE, ImVec2(float(g_screenWidth) / 2.0f, float(g_screenHeight) / 2.0f), ".js"))
        {
            if (g_map)
            {
                auto * map = g_map;

                const string prevFilename = map->m_mapPath;

                string newFilePath = g_fileDialog.selected_path;

                bool canExport = false;

                if (g_saveMapDataOnly)
                {
                    if (!EndsWith(newFilePath, "-data.js"))
                        LOG_ERROR("\"%s\" is not a valid map filename to export. Map filenames should end with \"-map.js\"", GetFilename(newFilePath).c_str());
                    else
                        canExport = true;
                }
                else
                {
                    if (!EndsWith(newFilePath, "-map.js"))
                        LOG_ERROR("\"%s\" is not a valid mapdata filename to export. Mapdata filenames should end with \"-data.js\"", GetFilename(newFilePath).c_str());
                    else
                        canExport = true;
                }

                if (canExport)
                {
                    map->m_mapPath = newFilePath;
                    map->m_mapDataPath = Map::GetMapDataPathFromMapPath(map->m_mapPath);
                    map->exportFiles(g_currentWorkingDirectory, g_saveMapDataOnly);

                    if (prevFilename != map->m_mapPath)
                        map->m_isDocked = false;
                }
            }

            SetCurrentDirectory(g_currentWorkingDirectory.c_str());
            ImGui::GetIO().IniFilename = g_saveImGuiIniPath;
        }

        if (needRefresh)
            g_map->refresh();

        // update "camera"
        const float panSpeed = 1.0f;
        const float zoomSpeed = 1.1f;

         if (g_map && g_map->m_isHovered)
        {
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
