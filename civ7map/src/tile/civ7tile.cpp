#include "civ7tile.h"
#include "map/map.h"

//--------------------------------------------------------------------------------------
RiverInfo::RiverInfo(const string & _name, bool _vanilla) :
    name(_name),
    vanilla(_vanilla)
{
    prettyName = Map::getRiverPrettyNameFromString(_name);
}