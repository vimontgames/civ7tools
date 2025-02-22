#pragma once

#include "colorfloat4.h"

//--------------------------------------------------------------------------------------
struct ResourceInfo
{
    string name;
    ColorFloat4 color;
    bool visible = true;
    sf::Texture texture;
    u32 count = 0;
};

//--------------------------------------------------------------------------------------
// https://humankind.fandom.com/wiki/Strategic_Resource
//--------------------------------------------------------------------------------------
enum class StrategicResource
{
    Horse = 0x19,
    Copper,
    Iron,
    Coal,
    Salpetre,
    Oil,
    Aluminium,
    Uranium,

    First = Horse,
    Last = Uranium,
    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
// https://humankind.fandom.com/wiki/Luxury_Resource
//--------------------------------------------------------------------------------------
enum class LuxuryResource
{
    Salt = 0x21,
    Sage,
    Coffee,
    Tea,
    Saffron,

    Dye,
    Ebony,
    Marble,
    Obsidian,
    Silk,

    Incense,
    Porcelain,
    Pearls,
    Gold,
    Gemstone,

    Ambergris,
    Papyrus,
    Lead,
    Mercury,
    Silver,

    Weapon,

    First = Salt,
    Last = Weapon,

    Count = Last - First + 1
};

//--------------------------------------------------------------------------------------
// https://humankind.fandom.com/wiki/Natural_Wonder
//--------------------------------------------------------------------------------------
enum class NaturalWonderResource
{
    DanakilDesert = 0,
    GreatBarrierReef,
    GreatBlueHole,
    HalongBay,
    KawahIjen,
    LakeBaikal,
    LakeHillier,
    MountEverest,
    MountMulu,
    MountRoraima,
    MountVesuvius,
    PeritorMorenoGlacier,
    Vinicuna,
    Yellowstone,

    Invalid = 0xFF,

    First = DanakilDesert,
    Last = Yellowstone,

    Count = Last - First + 1
};

extern ResourceInfo luxuryResources[];
extern ResourceInfo strategicResources[];
extern ResourceInfo naturalWonderResources[];

enum class TileType
{
    Coastal = 1,
    Dry,
    Forest,
    Lake,
    Mountain,
    MoutainSnow,
    Ocean,
    Prairie,
    RockyField,
    RockyForest,
    Sterile,
    StoneField,
    Wasteland,
    Woodland,

    Invalid = 0,

    First = Coastal,
    Last = Woodland,

    Count = Last - First + 1
};