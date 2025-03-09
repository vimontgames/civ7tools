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
void Map::Paint(int _x, int _y)
{
    if (auto * paintWindow = PaintWindow::get())
    {
        Civ7Tile & tileRef = m_civ7TerrainType.get(_x, _y);
        Civ7Tile tileCopy = tileRef;

        if (paintWindow->m_paintContinentType)
            tileCopy.continent = paintWindow->m_continentType;

        if (paintWindow->m_paintTerrainType)
            tileCopy.terrain = paintWindow->m_terrainType;

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