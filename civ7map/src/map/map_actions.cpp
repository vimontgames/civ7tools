#include "windows/paint.h"
#include "map/map.h"
#include "undoredo/UndoRedoTile.h"
#include "maths.h"
#include "shader/common.h"

static UndoRedoTile * undoRedoPaintTile = nullptr;

//--------------------------------------------------------------------------------------
void Map::BeginPaint()
{
    if (auto * paintWindow = PaintWindow::get())
    {
        // Create undo/redo event, but don't add it yet
        undoRedoPaintTile = new UndoRedoTile(this);
    }
}

//--------------------------------------------------------------------------------------
int Map::getPlotIndex(const int2& coords) const
{
    return coords.x + coords.y * m_width;
}

//--------------------------------------------------------------------------------------
bool Map::getPlotIndex(int x, int y, int * index) const
{
    *index = getPlotIndex(int2(x, y));
    return true;
}

//--------------------------------------------------------------------------------------
int2 Map::getPlotCoords(int index) const
{
    return int2(index % m_width, index / m_width);
}

//--------------------------------------------------------------------------------------
bool Map::getPlotCoords(int index, int * x, int * y) const
{
    int2 coords = getPlotCoords(index);
    *x = coords.x;
    *y = coords.y;
    return true;
}

//--------------------------------------------------------------------------------------
bool Map::getHexSideTile(int _x, int _y, HexTileSide _side, int2 & _result) const
{
    int offset = (_y & 1) ? 1 : 0;
    int2 coords = int2(-1, -1);
    switch (_side)
    {
        case HexTileSide::TopLeft:
            coords = int2(_x - 1 + offset, _y - 1);
            break;

        case HexTileSide::TopRight:
            coords = int2(_x + offset, _y - 1);
            break;

        case HexTileSide::Left:
            coords = int2(_x - 1, _y );
            break;

        case HexTileSide::Right:
            coords = int2(_x + 1, _y);
            break;

        case HexTileSide::BottomLeft:
            coords = int2(_x - 1 + offset, _y + 1);
            break;

        case HexTileSide::BottomRight:
            coords = int2(_x + offset, _y + 1);
            break;
    }

    _result = coords;
    if (_result.x >= 0 && _result.x < (int)m_width && _result.y >= 0 && _result.y < (int)m_height)
        return true;
    return false;
}

//--------------------------------------------------------------------------------------
void Map::Paint(int _x, int _y)
{
    if (auto * paintWindow = PaintWindow::get())
    {
        const int r = paintWindow->m_brushRadius;
        int count = 0;
        
        for (int y = _y -r + 1; y <= _y+r-1; ++y)
        {
            for (int x = _x -r + 1; x <= _x + r - 1; ++x)
            {
                if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height)
                    continue;

                if (cellDist(int2(_x,_y), int2(x,y)) >= r)
                    continue;   

                Civ7Tile & tileRef = m_civ7TerrainType.get(x, y);
                Civ7Tile tileCopy = tileRef;

                if (paintWindow->m_protectCoasts)
                {
                    if (tileCopy.biome == BiomeType::Marine && tileCopy.terrain == TerrainType::Coast)
                        continue;
                }

                if (paintWindow->m_protectOcean)
                {
                    if (tileCopy.biome == BiomeType::Marine && tileCopy.terrain == TerrainType::Ocean)
                        continue;
                }

                count++;

                //LOG_WARNING("Paint x = %i to %i (%u tiles)", _x -r + 1, _x + r - 1, count);

                if (paintWindow->m_paintContinentType)
                    tileCopy.continent = paintWindow->m_continentType;

                if (paintWindow->m_paintLandmassType)
                    tileCopy.landmass = paintWindow->m_landmassType;

                if (paintWindow->m_paintTerrainType)
                    tileCopy.terrain = paintWindow->m_terrainType;            

                if (paintWindow->m_paintBiomeType)
                    tileCopy.biome = paintWindow->m_biomeType; 

                if (paintWindow->m_paintFeatureType)
                    tileCopy.feature = paintWindow->m_featureType;

                if (paintWindow->m_paintResourceType)
                    tileCopy.resource = paintWindow->m_resourceType;

                if (paintWindow->m_paintNaturalWonderType)
                    tileCopy.naturalWonder = paintWindow->m_naturalWonder;

                if (paintWindow->m_paintElevation)
                    tileCopy.elevation = paintWindow->m_elevation;

                if (paintWindow->m_paintRiverName)
                {
                    if (paintWindow->m_riverNameIndex == RiverName::None)
                    {
                        // remove last river
                        for (int r = MAX_RIVER_PER_TILE - 1; r >= 0; --r)
                        {
                            RiverTileInfo & riverTile = tileCopy.rivers[r];
                            if (riverTile.nameIndex != RiverName::None)
                            {
                                riverTile.nameIndex = RiverName::None;
                                break;
                            }
                        }
                    }
                    else
                    {
                        // Use first available river slot (empty or same slot)     
                        int paintedRiverIndex = -1;
                        for (int r = 0; r < MAX_RIVER_PER_TILE; ++r)
                        {
                            RiverTileInfo & riverTile = tileCopy.rivers[r];
                            if (riverTile.nameIndex == RiverName::None || riverTile.nameIndex == paintWindow->m_riverNameIndex)
                            {
                                paintedRiverIndex = r;
                                riverTile.nameIndex = paintWindow->m_riverNameIndex;
                                break;
                            }
                        }

                        if (-1 != paintedRiverIndex)
                        {
                            RiverTileInfo & riverTile = tileCopy.rivers[paintedRiverIndex];

                            if (paintWindow->m_autoRiverDirection)
                            {
                                // Use the direction we're coming from if any
                                for (uint d = 0; d < enumCount<DirectionType>(); ++d)
                                {
                                    DirectionType dir = (DirectionType)d;
                                    int2 nCoordsXY = GetTileCoordsInDirection(x, y, dir);
                                    Civ7Tile & nTileRef = m_civ7TerrainType.get(nCoordsXY.x, nCoordsXY.y);

                                    for (int r = 0; r < MAX_RIVER_PER_TILE; ++r)
                                    {
                                        RiverTileInfo & nRiverTileRef = nTileRef.rivers[r];
                                        if (nRiverTileRef.nameIndex == riverTile.nameIndex)
                                        {
                                            if (AreOpposedDirections(nRiverTileRef.direction, dir))
                                            {
                                                riverTile.direction = GetOppositeDirection(dir);

                                                Civ7Tile nTileCopy = nTileRef;
                                                nTileCopy.rivers[r].direction = GetOppositeDirection(dir);
                                                undoRedoPaintTile->add(nCoordsXY.x, nCoordsXY.y, nTileRef, nTileCopy);
                                                nTileRef = nTileCopy;
                                            }
                                            else
                                            {
                                                riverTile.direction = dir;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                riverTile.direction = paintWindow->m_riverDirection;
                            }
                        }
                    }
                }

                if (paintWindow->m_paintRiverType)
                {
                    int paintedRiverIndex = -1;
                    if ((RiverName)paintWindow->m_riverNameIndex == RiverName::None)
                    {
                        for (int r = 0; r < MAX_RIVER_PER_TILE; ++r)
                        {
                            RiverTileInfo & riverTile = tileCopy.rivers[r];
                            if (riverTile.nameIndex != RiverName::None)
                            {
                                RiverTileInfo & riverTile = tileCopy.rivers[r];
                                paintedRiverIndex = r;
                                riverTile.type = paintWindow->m_riverType;
                            }
                        }
                    }
                    else
                    {
                        for (int r = 0; r < MAX_RIVER_PER_TILE; ++r)
                        {
                            RiverTileInfo & riverTile = tileCopy.rivers[r];
                            if (riverTile.nameIndex == paintWindow->m_riverNameIndex)
                            {
                                paintedRiverIndex = r;
                                riverTile.type = paintWindow->m_riverType;
                                break;
                            }
                        }
                    }
                }

                if (tileCopy != tileRef)
                {
                    for (uint r = 0; r < MAX_RIVER_PER_TILE; ++r)
                    {
                        if (tileCopy.rivers[r] != tileRef.rivers[r])
                            UpdateRivers(true);
                    }

                    undoRedoPaintTile->add(x, y, tileRef, tileCopy);
                    tileRef = tileCopy;

                    // Fix neightbours
                    if (paintWindow->m_paintTerrainType)
                    {
                        tileCopy.terrain = paintWindow->m_terrainType;

                        if (paintWindow->m_autoCoast)
                        {
                            // When painting Ocean, add coasts to land tiles around painted tile
                            if (paintWindow->m_terrainType == TerrainType::Ocean)
                            {
                                for (uint i = 0; i < enumCount<HexTileSide>(); ++i)
                                {
                                    int2 coords;
                                    if (getHexSideTile(x, y, (HexTileSide)i, coords))
                                    {
                                        Civ7Tile & sideTile = m_civ7TerrainType.get(coords.x, coords.y);
                                        if (sideTile.terrain != TerrainType::Ocean)
                                        {
                                            Civ7Tile sideTileCopy = sideTile;
                                            sideTileCopy.terrain = TerrainType::Coast;
                                            undoRedoPaintTile->add(coords.x, coords.y, sideTile, sideTileCopy);
                                            sideTile = sideTileCopy;
                                        }
                                    }
                                }
                            }
                            else if (paintWindow->m_terrainType != TerrainType::Coast)
                            {
                                // When painting land, add coast to ocean tiles around painted tile
                                for (uint i = 0; i < enumCount<HexTileSide>(); ++i)
                                {
                                    int2 coords;
                                    if (getHexSideTile(x, y, (HexTileSide)i, coords))
                                    {
                                        Civ7Tile & sideTile = m_civ7TerrainType.get(coords.x, coords.y);
                                        if (sideTile.terrain == TerrainType::Ocean)
                                        {
                                            Civ7Tile sideTileCopy = sideTile;
                                            sideTileCopy.terrain = TerrainType::Coast;
                                            undoRedoPaintTile->add(coords.x, coords.y, sideTile, sideTileCopy);
                                            sideTile = sideTileCopy;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    refresh();
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::EndPaint()
{
    if (auto * paintWindow = PaintWindow::get())
    {
        UndoRedoStack::add(undoRedoPaintTile);
        undoRedoPaintTile = nullptr;
    }
}

//--------------------------------------------------------------------------------------
void Map::clearFeatures()
{
    BeginPaint();
    {
        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                Civ7Tile tile = m_civ7TerrainType.get(x, y);
                if (tile.feature != FeatureType::None)
                {
                    tile.feature = FeatureType::None;
                    undoRedoPaintTile->add(x, y, m_civ7TerrainType.get(x, y), tile);
                    m_civ7TerrainType.get(x, y) = tile;
                }
            }
        }
    }
    EndPaint();
    refresh();
}

//--------------------------------------------------------------------------------------
void Map::LogMissingAndDuplicateTSLs()
{
    uint noTSLCount = 0;
    uint dupTSLCount = 0;

    const auto & civilizations = getCivilizations();

    for (uint i = 0; i < civilizations.size(); ++i)
    {
        const Civilization & civ = civilizations[i];
        if (civ.civilizationName.length() == 0)
            continue;

        if (civ.tsl.size() == 0)
            noTSLCount++;
        else if (civ.tsl.size() > 1)
            dupTSLCount++;
    }

    if (noTSLCount > 0)
    {
        uint index = 0;
        LOG_WARNING("Found %u Civilizations with no TSL:", noTSLCount);
        for (uint i = 0; i < civilizations.size(); ++i)
        {
            const Civilization & civ = civilizations[i];
            if (civ.civilizationName.length() == 0)
                continue;

            if (civ.tsl.size() == 0)
            {
                LOG_WARNING("#%2u - \"%s\" (%s)", index, civ.userFriendlyName.c_str(), civ.civilizationName.c_str());
                index++;
            }
        }
    }

    if (dupTSLCount > 0)
    {
        uint index = 0;
        LOG_ERROR("Found %u Civilizations has more than one TSL:", noTSLCount);
        for (uint i = 0; i < civilizations.size(); ++i)
        {
            const Civilization & civ = civilizations[i];
            if (civ.civilizationName.length() == 0)
                continue;

            if (civ.tsl.size() > 1)
            {
                LOG_ERROR("#%2u - \"%s\" (%s)", index, civ.userFriendlyName.c_str(), civ.civilizationName.c_str());
                index++;
            }
        }
    }
}

//--------------------------------------------------------------------------------------
int Map::getCivilizationIndexByName(const string & name) const
{
    for (uint i = 0; i < m_civilizations.size(); ++i)
    {
        if (m_civilizations[i].civilizationName == name)
            return i;
    }
    return -1;
}

//--------------------------------------------------------------------------------------
void Map::clearResources()
{
    BeginPaint();
    {
        for (uint y = 0; y < m_height; ++y)
        {
            for (uint x = 0; x < m_width; ++x)
            {
                Civ7Tile tile = m_civ7TerrainType.get(x, y);
                if (tile.resource != ResourceType::None)
                {
                    tile.resource = ResourceType::None;
                    undoRedoPaintTile->add(x, y, m_civ7TerrainType.get(x, y), tile);
                    m_civ7TerrainType.get(x, y) = tile;
                }
            }
        }
    }
    EndPaint();
    refresh();
}

Array2D<Civ7Tile> g_copyTiles;
vector<TSL> g_copyTsl;

//--------------------------------------------------------------------------------------
void Map::copyRect(sf::Vector2i _begin, sf::Vector2i _end)
{
    const auto w = _end.x - _begin.x + 1;
    const auto h = _end.y - _begin.y + 1;
    g_copyTiles.SetSize(w, h);

    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            g_copyTiles.get(i, j) = m_civ7TerrainType.get(_begin.x + i, _begin.y + j);
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::pasteRect(sf::Vector2i _origin)
{
    if (_origin.x >= 0 && _origin.x < (int)m_width && _origin.y >= 0 && _origin.y <= (int)m_height)
    {
        BeginPaint();

        for (int j = 0; j < (int)g_copyTiles.Height(); ++j)
        {
            for (int i = 0; i < (int)g_copyTiles.Width(); ++i)
            {
                if (_origin.x + i >= (int)m_width || _origin.y + j >= (int)m_height)
                    continue;

                undoRedoPaintTile->add(_origin.x + i, _origin.y + j, m_civ7TerrainType.get(_origin.x + i, _origin.y + j), g_copyTiles.get(i, j));
                m_civ7TerrainType.get(_origin.x + i, _origin.y + j) = g_copyTiles.get(i, j);
            }
        }

        EndPaint();
        refresh();
    }
}

//--------------------------------------------------------------------------------------
void Map::cutRect(sf::Vector2i _begin, sf::Vector2i _end)
{
    copyRect(_begin, _end);

    const auto w = _end.x - _begin.x + 1;
    const auto h = _end.y - _begin.y + 1;

    BeginPaint();

    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            auto tile = m_civ7TerrainType.get(_begin.x + i, _begin.y + j);
            tile = Civ7Tile();
            tile.terrain = TerrainType::Ocean;
            tile.biome = BiomeType::Marine;

            undoRedoPaintTile->add(_begin.x + i, _begin.y + j, m_civ7TerrainType.get(_begin.x + i, _begin.y + j), tile);
            m_civ7TerrainType.get(_begin.x + i, _begin.y + j) = tile;
        }
    }

    EndPaint();
    refresh();
}