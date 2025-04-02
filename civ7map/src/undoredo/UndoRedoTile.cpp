#include "UndoRedoTile.h"
#include "map/map.h"

//--------------------------------------------------------------------------------------
UndoRedoTile::UndoRedoTile(const Map * _map)
{
    m_map = _map;
}

//--------------------------------------------------------------------------------------
void UndoRedoTile::Reset()
{
    m_map = nullptr;
    m_entries.clear();
}

//--------------------------------------------------------------------------------------
void UndoRedoTile::add(int _x, int _y, const Civ7Tile & _before, const Civ7Tile & _after)
{
    if (_before != _after)
    {
        Entry entry;
        entry.m_x = _x;
        entry.m_y = _y;
        entry.m_before = _before;
        entry.m_after = _after;

        m_entries.push_back(entry);

        //LOG_INFO("Backup tile (%i,%i)", _x, _y);
    }
}

//--------------------------------------------------------------------------------------
void UndoRedoTile::Undo()
{
    extern vector<Map *> g_maps;

    for (auto * map : g_maps)
    {
        if (map == m_map)
        {
            for (int i = (int)m_entries.size() - 1; i >= 0; --i)
            {
                auto & entry = m_entries[i];
                auto & tile = map->m_civ7TerrainType.get(entry.m_x, entry.m_y);
                //LOG_INFO("Restore tile (%i,%i)", entry.m_x, entry.m_y);
                tile = entry.m_before;
            }

            map->refresh();
        }
    }

}

//--------------------------------------------------------------------------------------
void UndoRedoTile::Redo()
{
    extern vector<Map *> g_maps;

    for (auto * map : g_maps)
    {
        if (map == m_map)
        {
            for (auto & entry : m_entries)
                map->m_civ7TerrainType.get(entry.m_x, entry.m_y) = entry.m_after;

            map->refresh();
        }
    }
}