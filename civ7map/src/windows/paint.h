#include "BaseWindow.h"

class PaintWindow : public BaseWindow
{
public:
    PaintWindow();
    bool Draw(const RenderWindow & window) final override;

    static PaintWindow * get();

//private:
    static inline PaintWindow * s_instance = nullptr;

    int m_brushRadius = 1;

    bool m_paintContinentType = false;
    ContinentType m_continentType = (ContinentType)-1;

    bool m_paintTerrainType = false;
    TerrainType m_terrainType = TerrainType::Flat;
    bool m_autoCoast = true;

    bool m_paintBiomeType = false;
    BiomeType m_biomeType = BiomeType::Tundra;

    bool m_paintFeature = false;
    FeatureType m_featureType = FeatureType::Forest;
    bool m_featureAutoBiome = true;
    bool m_featureAutoTerrain = true;

    bool m_paintResource = false;
    ResourceType m_resourceType = ResourceType::Wine;
};