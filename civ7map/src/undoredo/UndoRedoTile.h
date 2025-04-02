#pragma once

#include "undoredo\UndoRedo.h"

class UndoRedoTile : public UndoRedoEvent
{
public:
    UndoRedoTile(const struct Map * _map);

    void add(int _x, int _y, const Civ7Tile & _before, const Civ7Tile & _after);

    void Reset() final override;
    void Undo() final override;
    void Redo() final override;

private:
    struct Entry
    {
        int m_x;
        int m_y;
        Civ7Tile m_before;
        Civ7Tile m_after;
    };
    const Map * m_map;
    vector<Entry> m_entries;
};
