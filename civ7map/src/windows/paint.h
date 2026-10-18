#include "BaseWindow.h"
#include "tile/civ7tile.h"

class PaintWindow : public BaseWindow
{
public:
    PaintWindow();
    bool Draw(const sf::RenderWindow & window) final override;

    static PaintWindow * get();

//private:
    static inline PaintWindow * s_instance = nullptr;

    int m_brushRadius = 1;

    bool m_paintContinentType = false;
    ContinentType m_continentType = ContinentType::None;

    bool m_paintLandmassType = false;
    LandmassType m_landmassType = (LandmassType)0;

    bool m_paintTerrainType = false;
    TerrainType m_terrainType = TerrainType::Flat;
    bool m_autoCoast = true;
    bool m_protectOcean = false;
    bool m_protectCoasts = false;
    bool m_autoRiverDirection = true;

    bool m_paintBiomeType = false;
    BiomeType m_biomeType = BiomeType::Grassland;

    bool m_paintFeatureType = false;
    FeatureType m_featureType = FeatureType::None;
    bool m_featureAutoBiome = true;
    bool m_featureAutoTerrain = true;

    bool m_paintResourceType = false;
    ResourceType m_resourceType = ResourceType::None;

    bool m_paintNaturalWonderType = false;
    NaturalWonderType m_naturalWonder = NaturalWonderType::None;

    bool m_paintElevation = false;
    Elevation m_elevation = (Elevation)0;

    bool m_paintRiverName = false;
    RiverName m_riverNameIndex = RiverName::None;

    bool m_paintRiverType = false;
    RiverType m_riverType = RiverType::Minor;
    
    bool m_paintRiverDirection = false;
    DirectionType m_riverDirection = DirectionType::West;
};