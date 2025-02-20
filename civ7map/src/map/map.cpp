#include "map.h"
#include "resourceinfo.h"
#include "maths.h"
#include "tinyxml2.h"
#include "zip_file.hpp"
#include "base64.hpp"

#include "shader/common.h"

#define STBI_ONLY_PNG

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace sf;
using namespace tinyxml2;

#include "map_import.hpp"
#include "map_export.hpp"
#include "map_refresh.hpp"
#include "map_actions.hpp"
#include "map_render.hpp"

//--------------------------------------------------------------------------------------
Map::Map()
{
    
}

//--------------------------------------------------------------------------------------
void Map::loadIcons()
{
    // load resource textures if needed
    for (u32 i = 0; i < (u32)StrategicResource::Count; ++i)
    {
        ResourceInfo & shared = strategicResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/" + strategicResources[i].name + ".png");
        shared.count = 0;
    }

    for (u32 i = 0; i < (u32)LuxuryResource::Count; ++i)
    {
        ResourceInfo & shared = luxuryResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/" + luxuryResources[i].name + ".png");
        shared.count = 0;
    }

    for (u32 i = 0; i < (u32)NaturalWonderResource::Count; ++i)
    {
        ResourceInfo & shared = naturalWonderResources[i];
        Texture & tex = shared.texture;
        if (Vector2u(0, 0) == tex.getSize())
            tex.loadFromFile("data/img/wonder.png");
        shared.count = 0;
    }

    for (u32 i = 0; i < MapBitmap::Count; ++i)
    {
        auto & bitmap = bitmaps[i];
        bitmap.image.create(width, height);
        bitmap.sprites.clear();
    }

    for (u32 i = 0; i < _countof(spawnInfo); ++i)
    {
        auto & info = spawnInfo[i];
        if (Vector2u(0, 0) == info.texture.getSize())
            info.texture.loadFromFile("data/img/" + to_string(i + 1) + ".png");  
    }
}

//--------------------------------------------------------------------------------------
string Map::getShortName() const
{
    string shortName = path;

    auto path = shortName.find_last_of("/\\");
    if (path != string::npos)
        shortName.erase(0, path + 1);

    const size_t ext = shortName.rfind('.');
    if (ext != string::npos)
        shortName.erase(ext);

    return shortName;
}

//--------------------------------------------------------------------------------------
void Map::resetCameraPan()
{
    cameraOffset = Vector2f(0, 0);
    cameraPreviousOffset = cameraOffset;
}

//--------------------------------------------------------------------------------------
void Map::resetCameraZoom()
{
    cameraZoom = 1.0f;
}

//--------------------------------------------------------------------------------------
void Map::resetCamera()
{
    resetCameraPan();
    resetCameraZoom();
}

//--------------------------------------------------------------------------------------
void Map::translate(const sf::Vector2i & _offset)
{
    int offsetX = _offset.x;
    int offsetY = _offset.y;

    while (offsetX < 0)
        offsetX += width;
    while (offsetY < 0)
        offsetY += height;

    debugPrint("Translate map by {%i,%i}\n", offsetX, offsetY);

    for (u32 i = 0; i < allSpawnsPoints.size(); ++i)
    {
        SpawnPoint & spawn = allSpawnsPoints[i];
        spawn.pos.x = ((int)spawn.pos.x + (int)width - offsetX) % (int)width;
        spawn.pos.y = ((int)spawn.pos.y + (int)height - offsetY) % (int)height;
    }

    elevationTexture.    translate(offsetX, offsetY);
    zonesTexture.        translate(offsetX, offsetY);
    poiTexture.          translate(offsetX, offsetY);
    landmarksTexture.    translate(offsetX, offsetY);
    naturalWonderTexture.translate(offsetX, offsetY);
    riverTexture.        translate(offsetX, offsetY);
    visibilityTexture.   translate(offsetX, offsetY);
    roadTexture.         translate(offsetX, offsetY);
    matchingSeedTexture. translate(offsetX, offsetY);
}