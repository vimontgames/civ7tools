#include "windows/paint.h"

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
        Civ7Tile & tileRef = m_civ7TerrainType.get(_x, _y);
        Civ7Tile tileCopy = tileRef;

        if (paintWindow->m_paintContinentType)
            tileCopy.continent = paintWindow->m_continentType;

        if (paintWindow->m_paintTerrainType)
        {
            tileCopy.terrain = paintWindow->m_terrainType;
            
            if (paintWindow->m_autoCoast)
            {
                // When painting Ocean, add coasts to land tiles around painted tile
                if (paintWindow->m_terrainType == TerrainType::Ocean)
                {
                    for (int i = 0; i < enumCount<HexTileSide>(); ++i)
                    {
                        int2 coords;
                        if (getHexSideTile(_x, _y, (HexTileSide)i, coords))
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
                    for (int i = 0; i < enumCount<HexTileSide>(); ++i)
                    {
                        int2 coords;
                        if (getHexSideTile(_x, _y, (HexTileSide)i, coords))
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

        if (paintWindow->m_paintBiomeType)
            tileCopy.biome = paintWindow->m_biomeType; 

        if (paintWindow->m_paintFeature)
            tileCopy.feature = paintWindow->m_featureType;

        if (paintWindow->m_paintResource)
            tileCopy.resource = paintWindow->m_resourceType;

        if (tileCopy != tileRef)
        {
            undoRedoPaintTile->add(_x, _y, tileRef, tileCopy);
            tileRef = tileCopy;
            refresh();
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