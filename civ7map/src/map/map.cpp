#include "map.h"
#include "maths.h"
#include "stringconvert.h"
#include "tinyxml2.h"
#include "zip_file.hpp"
#include "base64.hpp"
#include "undoredo/UndoRedoTile.h"
#include "shader/common.h"
#include "winsqlite/winsqlite3.h"

#define STBI_ONLY_PNG

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace sf;
using namespace tinyxml2;

#include "map_types.hpp"
#include "map_import.hpp"
#include "map_import_sql.hpp"
#include "map_export.hpp"
#include "map_refresh.hpp"
#include "map_create.hpp"
#include "map_render.hpp"

SharedIcon Map::s_resourceIcons[enumCount<ResourceType>()];
SharedIcon Map::s_featureIcons[enumCount<FeatureType>()];
SharedIcon Map::s_naturalWonderIcons[enumCount<NaturalWonderType>()];
SharedIcon Map::s_riverIcons[enumCount<RiverType>() * enumCount<DirectionType>()];
SharedIcon Map::s_miscIcons[enumCount<MiscIcons>()];

//--------------------------------------------------------------------------------------
// Test conversion funcs
//--------------------------------------------------------------------------------------
void Map::Test()
{
    // TODO: remove 'Random' enum values
     
    for (auto val : enumValues<NaturalWonderType>())
    {
        NaturalWonderType value = val.first;
    
        string s1 = getNaturalWonderTypeAsString(value);
        string s2 = getEnumAsExportString("FEATURE",  value);
        ASSERT(s1 == s2, "String \"%s\" and \"%s\" are different", s1.c_str(), s2.c_str());
    
        NaturalWonderType e1 = getNaturalWonderFromString(s1, -1, -1);
        NaturalWonderType e2 = getEnumValueFromString<NaturalWonderType>("FEATURE", s1);
        ASSERT(e1 == e2);
    
        ASSERT(e1 == value, "Count not convert enum \"%s\" to enum type", s1.c_str());
    }
}

//--------------------------------------------------------------------------------------
string Map::GetMapDataPathFromMapPath(const string & _mapPath) const
{
    string mapDataPath = GetFilenameWithoutExtension(_mapPath);

    switch (m_mapVersion)
    {
        case MapVersion::YnAMP:
            mapDataPath = mapDataPath.substr(0, mapDataPath.length() - 4); // remove "-map"
            mapDataPath = fmt::sprintf("%s\\%s-data.js", GetFolder(_mapPath), mapDataPath);
            break;

        case MapVersion::Civ7Map:
            mapDataPath = fmt::sprintf("%s\\%s.js", GetFolder(_mapPath), mapDataPath);
            break;

        default:
            ASSERT(false);
            break;
    }
    return mapDataPath;
}

//--------------------------------------------------------------------------------------
Map::Map()
{
    // Add vanilla and popular civs
    CreateDefaultCivilizations();

    // Add default river names
    CreateDefaultRivers();

    // Create a few landmasses
    CreateDefaultLandmasses();

    // Same for continents
    CreateDefaultContinents();
}

//--------------------------------------------------------------------------------------
void Map::CreateDefaultCivilizations()
{
    // Invalid civ
    m_civilizations.push_back(Civilization("", Era::Invalid, float4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f)));

    // Vanilla civs

    // Antiquity Age Civilizations
    m_civilizations.push_back(Civilization("CIVILIZATION_AKSUM", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_EGYPT", Era::Antiquity, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_GREECE", Era::Antiquity, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_HAN", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_KHMER", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MAURYA", Era::Antiquity, float4(44.0f / 255.0f, 48.0f / 255.0f, 44.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MAYA", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MISSISSIPPIAN", Era::Antiquity, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_PERSIA", Era::Antiquity, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_ROME", Era::Antiquity, float4(44.0f / 255.0f, 48.0f / 255.0f, 44.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_CARTHAGE", Era::Antiquity, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_ASSYRIA", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SILLA", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));

    m_civilizations.push_back(Civilization("CIVILIZATION_BABYLON", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_GAUL", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_GORYEO", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_HEIAN", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SENGOKU", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));

    // Exploration Age Civilizations
    m_civilizations.push_back(Civilization("CIVILIZATION_ABBASID", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_CHOLA", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_HAWAII", Era::Exploration, float4(255.0f / 255.0f, 244.0f / 255.0f, 129.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_INCA", Era::Exploration, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MAJAPAHIT", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MING", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MONGOLIA", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_NORMAN", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SONGHAI", Era::Exploration, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SPAIN", Era::Exploration, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SHAWNEE", Era::Exploration, float4(95.0f / 255.0f, 86.0f / 255.0f, 10.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_BULGARIA", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_DAI_VIET", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_ICELAND", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_OTTOMANS", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_JOSEON", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));

    // Modern Age Civilizations
    m_civilizations.push_back(Civilization("CIVILIZATION_AMERICA", Era::Modern, float4(54.0f / 255.0f, 50.0f / 255.0f, 19.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_FRENCH_EMPIRE", Era::Modern, float4(0.0f / 255.0f, 44.0f / 255.0f, 140.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MEXICO", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MUGHAL", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_PRUSSIA", Era::Modern, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_QING", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_RUSSIA", Era::Modern, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_SIAM", Era::Modern, float4(55.0f / 255.0f, 62.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_GREAT_BRITAIN", Era::Modern, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_MEIJI", Era::Modern, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_BUGANDA", Era::Modern, float4(157.0f / 255.0f, 144.0f / 255.0f, 14.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_NEPAL", Era::Modern, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));
    m_civilizations.push_back(Civilization("CIVILIZATION_QAJAR", Era::Modern, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f)));

    // Extra civilizations from mods (TODO: parse mod files for custom civs ?)

    // Antiquity
    m_civilizations.push_back(Civilization("CIVILIZATION_GERMANIA_JEC", Era::Antiquity, float4(126.0f / 255.0f, 126.0f / 255.0f, 126.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_SAXON", Era::Antiquity, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_SANSEB_TAGALOG", Era::Antiquity, float4(19.0f / 255.0f, 126.0f / 255.0f, 247.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_HUNS_ROG", Era::Antiquity, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_PB_YAMATAI", Era::Antiquity, float4(55.0f / 255.0f, 162.0f / 255.0f, 85.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_TONGA", Era::Antiquity, float4(55.0f / 255.0f, 162.0f / 255.0f, 85.0f / 255.0f, 1.0f), false));

    // Exploration
    m_civilizations.push_back(Civilization("CIVILIZATION_ENGLAND", Era::Exploration, float4(249.0f / 255.0f, 249.0f / 255.0f, 249.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_TCS_OUTREMER", Era::Exploration, float4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_VENICE", Era::Exploration, float4(184.0f / 255.0f, 184.0f / 255.0f, 184.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_DOURADOS_PORTUGAL", Era::Exploration, float4(55.0f / 255.0f, 162.0f / 255.0f, 85.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_EDO", Era::Exploration, float4(55.0f / 255.0f, 162.0f / 255.0f, 85.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_PIRATE_REPUBLIC", Era::Exploration, float4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f), false));

    // Modern
    m_civilizations.push_back(Civilization("CIVILIZATION_SANSEB_PHILIPPINES", Era::Modern, float4(19.0f / 255.0f, 126.0f / 255.0f, 247.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_NYGUITA_ARGENTINA", Era::Modern, float4(100.0f / 255.0f, 144.0f / 255.0f, 240.0f / 255.0f, 1.0f), false));
    m_civilizations.push_back(Civilization("CIVILIZATION_ARTHUR_SPANISH_EMPIRE", Era::Modern, float4(247.0f / 255.0f, 226.0f / 255.0f, 19.0f / 255.0f, 1.0f), false));
}

//--------------------------------------------------------------------------------------
void Map::CreateDefaultRivers()
{
#if 0
    m_rivers.push_back(RiverInfo("GUADIANA_RIVER"));
    m_rivers.push_back(RiverInfo("HADDAS_RIVER"));
    m_rivers.push_back(RiverInfo("HAI_RIVER"));
    m_rivers.push_back(RiverInfo("HALIACMON_RIVER"));
    m_rivers.push_back(RiverInfo("HAN_RIVER"));
    m_rivers.push_back(RiverInfo("HANALEI_RIVER"));
    m_rivers.push_back(RiverInfo("HONDO_RIVER"));
    m_rivers.push_back(RiverInfo("HVITA_RIVER"));
    m_rivers.push_back(RiverInfo("INDUS_RIVER"));
    m_rivers.push_back(RiverInfo("IRRAWADDY_RIVER"));
    m_rivers.push_back(RiverInfo("ISHIKARI_RIVER"));
    m_rivers.push_back(RiverInfo("JAJRUD_RIVER"));
    m_rivers.push_back(RiverInfo("JORDAN_RIVER"));
    m_rivers.push_back(RiverInfo("JUCAR_RIVER"));
    m_rivers.push_back(RiverInfo("KAPUAS_RIVER"));
    m_rivers.push_back(RiverInfo("KARKHEH_RIVER"));
    m_rivers.push_back(RiverInfo("KARUN_RIVER"));
    m_rivers.push_back(RiverInfo("KASAI_RIVER"));
    m_rivers.push_back(RiverInfo("KATONGA_RIVER"));
    m_rivers.push_back(RiverInfo("KAUKONAHUA_STREAM"));
    m_rivers.push_back(RiverInfo("KAVERI_RIVER"));
    m_rivers.push_back(RiverInfo("KERULEN_RIVER"));
    m_rivers.push_back(RiverInfo("KITAKAMI_RIVER"));
    m_rivers.push_back(RiverInfo("KIZILIRMAK_RIVER"));
    m_rivers.push_back(RiverInfo("KOLEKOLE_STREAM"));
    m_rivers.push_back(RiverInfo("KRISHNA_RIVER"));
    m_rivers.push_back(RiverInfo("KSHIPRA_RIVER"));
    m_rivers.push_back(RiverInfo("LENA_RIVER"));
    m_rivers.push_back(RiverInfo("LERMA_RIVER"));
    m_rivers.push_back(RiverInfo("LIMPOPO_RIVER"));
    m_rivers.push_back(RiverInfo("LOIRE_RIVER"));
    m_rivers.push_back(RiverInfo("LUGOGO_RIVER"));
    m_rivers.push_back(RiverInfo("MACKENZIE_RIVER"));
    m_rivers.push_back(RiverInfo("MAHAKAM_RIVER"));
    m_rivers.push_back(RiverInfo("MAIPO_RIVER"));
    m_rivers.push_back(RiverInfo("MAMBERAMO_RIVER"));
    m_rivers.push_back(RiverInfo("MARITSA_RIVER"));
    m_rivers.push_back(RiverInfo("MAULE_RIVER"));
    m_rivers.push_back(RiverInfo("MAYANJA_RIVER"));
    m_rivers.push_back(RiverInfo("MEKONG_RIVER"));
    m_rivers.push_back(RiverInfo("MEUSE_RIVER"));
    m_rivers.push_back(RiverInfo("MISSISSIPPI_RIVER"));
    m_rivers.push_back(RiverInfo("MISSOURI_RIVER"));
    m_rivers.push_back(RiverInfo("MOGAMI_RIVER"));
    m_rivers.push_back(RiverInfo("MOTAGUA_RIVER"));
    m_rivers.push_back(RiverInfo("MUN_RIVER"));
    m_rivers.push_back(RiverInfo("MURRAY_RIVER"));
    m_rivers.push_back(RiverInfo("MUSI_RIVER"));
    m_rivers.push_back(RiverInfo("NACHI_RIVER"));
    m_rivers.push_back(RiverInfo("NAKDONG_RIVER"));
    m_rivers.push_back(RiverInfo("NAN_RIVER"));
    m_rivers.push_back(RiverInfo("NARMADA_RIVER"));
    m_rivers.push_back(RiverInfo("NAZAS_RIVER"));
    m_rivers.push_back(RiverInfo("NESTOS_RIVER"));
    m_rivers.push_back(RiverInfo("NIGER_RIVER"));
    m_rivers.push_back(RiverInfo("NILE_RIVER"));
    m_rivers.push_back(RiverInfo("NORTHERN_DVINA"));
    m_rivers.push_back(RiverInfo("OB_RIVER"));
    m_rivers.push_back(RiverInfo("ODER_RIVER"));
    m_rivers.push_back(RiverInfo("OFANTO_RIVER"));
    m_rivers.push_back(RiverInfo("OHIO_RIVER"));
    m_rivers.push_back(RiverInfo("OKAVANGO_RIVER"));
    m_rivers.push_back(RiverInfo("ORANGE_RIVER"));
    m_rivers.push_back(RiverInfo("ORINOCO_RIVER"));
    m_rivers.push_back(RiverInfo("ORKHON_RIVER"));
    m_rivers.push_back(RiverInfo("PARAGUAY_RIVER"));
    m_rivers.push_back(RiverInfo("PARANA_RIVER"));
    m_rivers.push_back(RiverInfo("PEARL_RIVER"));
    m_rivers.push_back(RiverInfo("PENNAR_RIVER"));
    m_rivers.push_back(RiverInfo("PING_RIVER"));
    m_rivers.push_back(RiverInfo("PO_RIVER"));
    m_rivers.push_back(RiverInfo("POTOMAC_RIVER"));
    m_rivers.push_back(RiverInfo("PURUS_RIVER"));
    m_rivers.push_back(RiverInfo("PUTUMAYU_RIVER"));
    m_rivers.push_back(RiverInfo("RED_RIVER"));
    m_rivers.push_back(RiverInfo("RHINE_RIVER"));
    m_rivers.push_back(RiverInfo("RHONE_RIVER"));
    m_rivers.push_back(RiverInfo("RIO_GRANDE"));
    m_rivers.push_back(RiverInfo("RIO_MADEIRA"));
    m_rivers.push_back(RiverInfo("RIO_NEGRO"));
    m_rivers.push_back(RiverInfo("RIVER_AVON"));
    m_rivers.push_back(RiverInfo("RIVER_GREAT_OUSE"));
    m_rivers.push_back(RiverInfo("RIVER_KAFU"));
    m_rivers.push_back(RiverInfo("RIVER_SEVERN"));
    m_rivers.push_back(RiverInfo("RIVER_THAMES"));
    m_rivers.push_back(RiverInfo("RIVER_TRENT"));
    m_rivers.push_back(RiverInfo("RIVER_WELLAND"));
    m_rivers.push_back(RiverInfo("ROSETTA_RIVER"));
    m_rivers.push_back(RiverInfo("RUBICON_RIVER"));
    m_rivers.push_back(RiverInfo("SAINT_LAWRENCE_RIVER"));
    m_rivers.push_back(RiverInfo("SAKARYA_RIVER"));
    m_rivers.push_back(RiverInfo("SALADO_RIVER"));
    m_rivers.push_back(RiverInfo("SALWEEN_RIVER"));
    m_rivers.push_back(RiverInfo("SAMALA_RIVER"));
    m_rivers.push_back(RiverInfo("SAO_FRANCISCO_RIVER"));
    m_rivers.push_back(RiverInfo("SARASWATI_RIVER"));
    m_rivers.push_back(RiverInfo("SEFID_RIVER"));
    m_rivers.push_back(RiverInfo("SEINE_RIVER"));
    m_rivers.push_back(RiverInfo("SENEGAL_RIVER"));
    m_rivers.push_back(RiverInfo("SEPIK_RIVER"));
    m_rivers.push_back(RiverInfo("SEZIBWA_RIVER"));
    m_rivers.push_back(RiverInfo("SHINANO_RIVER"));
    m_rivers.push_back(RiverInfo("SONGHUA_RIVER"));
    m_rivers.push_back(RiverInfo("SREPOK_RIVER"));
    m_rivers.push_back(RiverInfo("STRUMA_RIVER"));
    m_rivers.push_back(RiverInfo("SUSQUEHANNA_RIVER"));
    m_rivers.push_back(RiverInfo("SYR_DARYA_RIVER"));
    m_rivers.push_back(RiverInfo("TAGUS_RIVER"));
    m_rivers.push_back(RiverInfo("TARIM_RIVER"));
    m_rivers.push_back(RiverInfo("TENRYU_RIVER"));
    m_rivers.push_back(RiverInfo("TESHIO_RIVER"));
    m_rivers.push_back(RiverInfo("THA_CHIN_RIVER"));
    m_rivers.push_back(RiverInfo("TIBER_RIVER"));
    m_rivers.push_back(RiverInfo("TIGRIS_RIVER"));
    m_rivers.push_back(RiverInfo("TOCANTINS_RIVER"));
    m_rivers.push_back(RiverInfo("TONE_RIVER"));
    m_rivers.push_back(RiverInfo("TONLE_SAN_RIVER"));
    m_rivers.push_back(RiverInfo("TONLE_SAP_RIVER"));
    m_rivers.push_back(RiverInfo("TUUL_RIVER"));
    m_rivers.push_back(RiverInfo("UCAYALI_RIVER"));
    m_rivers.push_back(RiverInfo("UMFOLOZI_RIVER"));
    m_rivers.push_back(RiverInfo("URAL_RIVER"));
    m_rivers.push_back(RiverInfo("USUMACINTA_RIVER"));
    m_rivers.push_back(RiverInfo("VARDAR_RIVER"));
    m_rivers.push_back(RiverInfo("VISTULA_RIVER"));
    m_rivers.push_back(RiverInfo("VOLGA_RIVER"));
    m_rivers.push_back(RiverInfo("WADI_AL_BATIN"));
    m_rivers.push_back(RiverInfo("WADI_AL_RUMMAH"));
    m_rivers.push_back(RiverInfo("WADI_FEIRAN"));
    m_rivers.push_back(RiverInfo("WADI_HANIFA"));
    m_rivers.push_back(RiverInfo("WAILUA_RIVER"));
    m_rivers.push_back(RiverInfo("WAILUKU_RIVER"));
    m_rivers.push_back(RiverInfo("WAIMEA_RIVER"));
    m_rivers.push_back(RiverInfo("WALLAQA_MAYU"));
    m_rivers.push_back(RiverInfo("WEI_RIVER"));
    m_rivers.push_back(RiverInfo("WESER_RIVER"));
    m_rivers.push_back(RiverInfo("WHITE_NILE"));
    m_rivers.push_back(RiverInfo("WHITE_VOLTA"));
    m_rivers.push_back(RiverInfo("WILLKA_MAYU"));
    m_rivers.push_back(RiverInfo("YAMUNA_RIVER"));
    m_rivers.push_back(RiverInfo("YANGTZE_RIVER"));
    m_rivers.push_back(RiverInfo("YELLOW_RIVER"));
    m_rivers.push_back(RiverInfo("YENISEI_RIVER"));
    m_rivers.push_back(RiverInfo("YUKON_RIVER"));
    m_rivers.push_back(RiverInfo("ZAMBEZI_RIVER"));
    m_rivers.push_back(RiverInfo("ZAVKHAN_RIVER"));
    m_rivers.push_back(RiverInfo("ZAYANDE_RIVER"));
#endif
}

//--------------------------------------------------------------------------------------
Civilization::Civilization(const string & _civilizationName, Era _era, const float4 _color, bool _vanilla) :
    vanilla(_vanilla),
    civilizationName(_civilizationName),
    era(_era),
    color(_color)
{
    if (_civilizationName == "")
    {
        userFriendlyName = "Undefined";
    }
    else
    {
        userFriendlyName = Map::getCivilizationNameFromString(_civilizationName);
    }
}

//--------------------------------------------------------------------------------------
void Map::createBitmaps()
{
    for (u32 i = 0; i < enumCount<MapBitmap>(); ++i)
    {
        auto & bitmap = m_bitmaps[i];

        if ((MapBitmap)i == MapBitmap::TerrainData)
        {
            bitmap.image.create(m_width, m_height * 2);
            bitmap.sprites.clear();
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadIcons(bool _reload)
{
    loadMiscIcons(_reload);
    loadResourceIcons(_reload);
    loadFeatureIcons(_reload);
    loadNaturalWonderIcons(_reload);    
    loadRiverIcons(_reload);    
}

//--------------------------------------------------------------------------------------
void Map::loadMiscIcons(bool _reload)
{
    for (uint i = 0; i < enumCount<MiscIcons>(); ++i)
    {
        const MiscIcons miscIcon = (MiscIcons)i;
        string path = "";
        switch (miscIcon)
        {
            case MiscIcons::TSL:
                path = "data/img/tsl/white.png";
                break;

            case MiscIcons::RiverError:
                path = "data/img/rivers/error.png";
                break;
        }

        if (path.length() > 0)
        {
            SharedIcon & sharedIcon = s_miscIcons[i];
            if (sharedIcon.dirty || _reload)
            {
                bool dirty = false;
                if (_reload && sharedIcon.texture.loadFromFile(path))
                {
                    LOG_INFO("Texture \"%s\" loaded", GetFilename(path).c_str());
                    sharedIcon.dirty = dirty;
                    sharedIcon.texture.generateMipmap();
                }
                else
                    LOG_ERROR("Texture \"%s\" could not be loaded", GetFilename(path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadResourceIcons(bool _reload)
{
    for (auto val : enumValues<ResourceType>())
    {
        if (ResourceType::None != val.first)
        {
            int index = (int)val.first;
            SharedIcon & resIcon = s_resourceIcons[index];
            if (resIcon.dirty || _reload)
            {
                bool dirty = false;

                auto path = fmt::sprintf("data/img/resources/%s.png", asString((ResourceType)index));
                if (!FileExists(path))
                {
                    if (ResourceType::None != val.first)
                        LOG_WARNING("Texture \"%s\" not found.", (path).c_str());
                    path = fmt::sprintf("data/img/resources/%s.png", "Default");
                    dirty = true;
                }

                if (_reload && FileExists(path) && resIcon.texture.loadFromFile(path))
                {
                    if (!EndsWith(path, "Default.png"))
                        LOG_INFO("Texture \"%s\" loaded.", GetFilename(path).c_str());
                    resIcon.dirty = dirty;
                    resIcon.texture.generateMipmap();
                }
                else if (!dirty)
                    LOG_ERROR("Texture \"%s\" could not be loaded.", (path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadFeatureIcons(bool _reload)
{
    for (auto val : enumValues<FeatureType>())
    {
        if (FeatureType::None != val.first)
        {
            int index = (int)val.first;
            SharedIcon & resIcon = s_featureIcons[index];
            if (resIcon.dirty || _reload)
            {
                bool dirty = false;

                auto path = fmt::sprintf("data/img/features/%s.png", asString((FeatureType)index));
                if (!FileExists(path))
                {
                    if (FeatureType::None != val.first)
                        LOG_WARNING("Texture \"%s\" not found.", (path).c_str());
                    path = fmt::sprintf("data/img/features/%s.png", "Default");
                    dirty = true;
                }

                if (_reload && FileExists(path) && resIcon.texture.loadFromFile(path))
                {
                    if (!EndsWith(path, "Default.png"))
                        LOG_INFO("Texture \"%s\" loaded.", GetFilename(path).c_str());
                    resIcon.dirty = dirty;
                    resIcon.texture.generateMipmap();
                }
                else if (!dirty)
                    LOG_ERROR("Texture \"%s\" could not be loaded.", (path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadNaturalWonderIcons(bool _reload)
{
    for (auto val : enumValues<NaturalWonderType>())
    {
        if (NaturalWonderType::None != val.first)
        {
            int index = (int)val.first;
            SharedIcon & resIcon = s_naturalWonderIcons[index];
            if (resIcon.dirty || _reload)
            {
                bool dirty = false;

                auto path = fmt::sprintf("data/img/naturalwonders/%s.png", asString((NaturalWonderType)index));
                if (!FileExists(path))
                {
                    if (NaturalWonderType::None != val.first)
                        LOG_WARNING("Texture \"%s\" not found.", (path).c_str());
                    path = fmt::sprintf("data/img/naturalwonders/%s.png", "Default");
                    dirty = true;
                }

                if (_reload && FileExists(path) && resIcon.texture.loadFromFile(path))
                {
                    if (!EndsWith(path, "Default.png"))
                        LOG_INFO("Texture \"%s\" loaded.", GetFilename(path).c_str());
                    resIcon.dirty = dirty;
                    resIcon.texture.generateMipmap();
                }
                else if (!dirty)
                    LOG_ERROR("Texture \"%s\" could not be loaded.", (path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void Map::loadRiverIcons(bool _reload)
{
    for (auto riverType : enumValues<RiverType>())
    {
        int riverTypeIndex = (int)riverType.first;

        for (auto riverDir : enumValues<DirectionType>())
        {
            int riverDirIndex = (int)riverDir.first;

            int iconIndex = riverTypeIndex * enumCount<DirectionType>() + riverDirIndex;

            SharedIcon & resIcon = s_riverIcons[iconIndex];
            if (resIcon.dirty || _reload)
            {
                bool dirty = false;

                auto path = fmt::sprintf("data/img/rivers/%s_%s.png", asString((RiverType)riverTypeIndex), asString((DirectionType)riverDirIndex));
                if (!FileExists(path))
                {
                    if (!EndsWith(path, "Default.png"))
                        LOG_WARNING("Texture \"%s\" not found", (path).c_str());
                    path = fmt::sprintf("data/img/rivers/%s.png", "Default");
                    dirty = true;
                }

                if (_reload && FileExists(path) && resIcon.texture.loadFromFile(path))
                {
                    LOG_INFO("Texture \"%s\" loaded", GetFilename(path).c_str());
                    resIcon.dirty = dirty;
                    resIcon.texture.generateMipmap();
                }
                else if (!dirty)
                    LOG_ERROR("Texture \"%s\" could not be loaded", (path).c_str());
            }
        }
    }
}

//--------------------------------------------------------------------------------------
string Map::getShortName() const
{
    return GetFilenameWithoutExtension(m_mapPath);
}

//--------------------------------------------------------------------------------------
void Map::resetCameraPan()
{
    m_cameraOffset = Vector2f(0, 0);
    m_cameraPreviousOffset = m_cameraOffset;
}

//--------------------------------------------------------------------------------------
void Map::resetCameraZoom()
{
    m_cameraZoom = 1.0f;
}

//--------------------------------------------------------------------------------------
void Map::resetCamera()
{
    resetCameraPan();
    resetCameraZoom();
}

//--------------------------------------------------------------------------------------
void Map::crop(const sf::Vector2i & _newSize)
{
    const auto oldMapData = m_civ7TerrainType;

    m_civ7TerrainType.SetSize(_newSize.x, _newSize.y);

    for (int y = 0; y < _newSize.y; ++y)
    {
        for (int x = 0; x < _newSize.x; ++x)
        {
            if (x < (int)oldMapData.Width() && y < (int)oldMapData.Height())
            {
                m_civ7TerrainType.get(x, y) = oldMapData.get(x, y);
            }
            else
            {
                m_civ7TerrainType.get(x, y) = Civ7Tile();
            }
        }
    }

    for (uint c = 0; c < m_civilizations.size(); ++c)
    {
        auto & civ = m_civilizations[c];
        for (int t = (int)(civ.tsl.size())-1; t >= 0; t--)
        {
            auto & tsl = civ.tsl[t];

            if (tsl.pos.x >= _newSize.x || tsl.pos.y >= _newSize.y)
            {
                LOG_WARNING("TSL for civ \"%s\" at (%u,%u) has been removed", civ.civilizationName.c_str(), tsl.pos.x, tsl.pos.y);
                civ.tsl.erase(civ.tsl.begin() + t);
                continue;
            }            
        }
    }

    m_width = _newSize.x;
    m_height = _newSize.y;    
}

//--------------------------------------------------------------------------------------
sf::Vector2i ResizeCoords(int x, int y, const sf::Vector2i& _oldSize, const sf::Vector2i& _newSize)
{
    int newX = (int)round(float(x) * float(_newSize.x) / float(_oldSize.x));
    int newY = (int)round(float(y) * float(_newSize.y) / float(_oldSize.y));
    return sf::Vector2i(newX, newY);
}

//--------------------------------------------------------------------------------------
void Map::rescale(const sf::Vector2i & _newSize)
{
    const sf::Vector2i oldSize = sf::Vector2i(m_width, m_height);
    const auto oldMapData = m_civ7TerrainType;
    for (int y = 0; y < _newSize.y; ++y)
    {
        for (int x = 0; x < _newSize.x; ++x)
        {
            auto oldPos = ResizeCoords(x, y, _newSize, oldSize);
            m_civ7TerrainType.get(x,y) = oldMapData.get(oldPos.x, oldPos.y);
        }
    }

    for (uint c = 0; c < m_civilizations.size(); ++c)
    {
        auto & civ = m_civilizations[c];
        for (uint t = 0; t < civ.tsl.size(); ++t)
        {
            auto & tsl = civ.tsl[t];

            auto newTSLPos = ResizeCoords(tsl.pos.x, tsl.pos.y, oldSize, _newSize);
            tsl.pos.x = newTSLPos.x;
            tsl.pos.y = newTSLPos.y;
        }
    }

    crop(_newSize);
}

//--------------------------------------------------------------------------------------
void Map::translate(const sf::Vector2i & _offset)
{
    int offsetX = _offset.x;
    int offsetY = _offset.y;

    while (offsetX < 0)
        offsetX += m_civ7TerrainType.Width();
    while (offsetY < 0)
        offsetY += m_civ7TerrainType.Height();

    LOG_INFO("Translate map by {%i,%i}", offsetX, offsetY);

    // move spawn points also
    for (uint c = 0; c < m_civilizations.size(); ++c)
    {
        auto & civ = m_civilizations[c];
        for (uint t = 0; t < civ.tsl.size(); ++t)
        {
            auto & tsl = civ.tsl[t];

            tsl.pos.x = (tsl.pos.x - _offset.x) % m_civ7TerrainType.Width();
            tsl.pos.y = (tsl.pos.y - _offset.y) % m_civ7TerrainType.Height();
        }
    }

    m_civ7TerrainType.translate(offsetX, offsetY);
}

//--------------------------------------------------------------------------------------
string Map::getContinentName(ContinentType continent) const
{
    if ((int)continent < m_continents.size())
        return m_continents[(int)continent].name;
    else
        return "";
}

//--------------------------------------------------------------------------------------
string Map::getContinentShortName(ContinentType continent) const
{
    if (ContinentType::None!= continent)
    {
        string name = getContinentName(continent);
        const string continentPrefix = "CONTINENT_";
        size_t trim = name.find(continentPrefix);
        if (-1 != trim)
            name = name.substr(trim + continentPrefix.length());

        name = CapitalizeWords(name);
        return name;
    }
    else
    {
        return "None";
    }
}

//--------------------------------------------------------------------------------------
uint Map::getContinentCount() const
{
    return (uint)m_continents.size();
}

//--------------------------------------------------------------------------------------
ContinentType Map::getOrCreateContinentType(const string & continentS)
{
    string name = continentS;

    if (!BeginsWith(continentS, "CONTINENT_"))
        name = fmt::sprintf("CONTINENT_%s", ToUpperLabel(continentS));

    if (continentS.length() > 0)
    {
        bool found = false;
        for (uint c = 0; c < m_continents.size(); ++c)
        {
            if (m_continents[c].name == name)
            {
                return (ContinentType)c;
            }
        }
        if (!found)
        {
            m_continents.push_back({ name, 0 });
            return (ContinentType)(m_continents.size() - 1);
        }
    }
    return ContinentType::None;
}

//--------------------------------------------------------------------------------------
string Map::getLandmassName(LandmassType landmass) const
{
    if ((int)landmass < m_landmasses.size())
        return fmt::sprintf("Landmass %u", m_landmasses[(int)landmass].index);
    else
        return "";
}

//--------------------------------------------------------------------------------------
string Map::getLandmassShortName(LandmassType landmass) const
{
    return getLandmassName(landmass);
}

//--------------------------------------------------------------------------------------
uint Map::getLandmassCount() const
{
    return (uint)m_landmasses.size();
}

//--------------------------------------------------------------------------------------
 LandmassType Map::getOrCreateLandmassType(const uint landmassIndex)
{
    if (landmassIndex >= m_landmasses.size())
    {
        m_landmasses.resize((int)landmassIndex + 1);
        for (int i = 0; i < m_landmasses.size(); ++i)
        {
            m_landmasses[i].index = i;
        }
    }
    return (LandmassType)m_landmasses[(int)landmassIndex].index;
}

//--------------------------------------------------------------------------------------
MapSize Map::getMapSize(uint _width, uint _height, bool _forceCustomMapSize)
{
    if (!_forceCustomMapSize)
    {
        for (auto val : enumValues<MapSize>())
        {
            const int index = (int)val.first;
            if (index >= 0)
            {
                if (g_mapSizes[index][0] == _width && g_mapSizes[index][1] == _height)
                    return val.first;
            }
        }
    }
    return MapSize::Custom;
}

//--------------------------------------------------------------------------------------
bool Map::setTerrain(int _x, int _y, TerrainType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.terrain != _value)
    {
        tile.terrain = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setBiome(int _x, int _y, BiomeType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.biome != _value)
    {
        tile.biome = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setFeature(int _x, int _y, FeatureType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.feature != _value)
    {
        tile.feature = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}



//--------------------------------------------------------------------------------------
bool Map::setResource(int _x, int _y, ResourceType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.resource != _value)
    {
        tile.resource = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setContinent(int _x, int _y, ContinentType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.continent != _value)
    {
        tile.continent = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setLandmass(int _x, int _y, LandmassType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.landmass != _value)
    {
        tile.landmass = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setNaturalWonder(int _x, int _y, NaturalWonderType _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.naturalWonder != _value)
    {
        tile.naturalWonder = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setElevation(int _x, int _y, Elevation _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.elevation != _value)
    {
        tile.elevation = _value;
        return setTileUndoRedo(_x,_y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setSnow(int _x, int _y, SnowAmount _value)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    if (tile.snow != _value)
    {
        tile.snow = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setRiverName(int _x, int _y, RiverName _value, int _index)
{
    if (_index < MAX_RIVER_PER_TILE)
    {
        Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

        RiverTileInfo & riverTile = tile.rivers[_index];
        if (riverTile.nameIndex != _value)
        {
            riverTile.nameIndex = _value;
            return setTileUndoRedo(_x, _y, tile);
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setRiverType(int _x, int _y, RiverType _value, int _index)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);

    RiverTileInfo & riverTile = tile.rivers[_index];
    if (riverTile.type != _value)
    {
        riverTile.type = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setRiverDirection(int _x, int _y, DirectionType _value, int _index)
{
    Civ7Tile tile = m_civ7TerrainType.get(_x, _y);
    RiverTileInfo & riverTile = tile.rivers[_index];
    if (riverTile.direction != _value)
    {
        riverTile.direction = _value;
        return setTileUndoRedo(_x, _y, tile);
    }
    return false;
}

//--------------------------------------------------------------------------------------
bool Map::setTileUndoRedo(int _x, int _y, const Civ7Tile & _value)
{
    auto event = new UndoRedoTile(this);
    event->add(_x, _y, m_civ7TerrainType.get(_x, _y), _value);
    UndoRedoStack::add(event);
    m_civ7TerrainType.get(_x, _y) = _value;
    return true;
}
