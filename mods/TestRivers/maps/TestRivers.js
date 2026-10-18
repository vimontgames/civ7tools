import { generateDiscoveries } from '/base-standard/maps/discovery-generator.js';
import { g_PolarWaterRows } from '/base-standard/maps/map-globals.js';
import { shuffle } from '/base-standard/maps/map-utilities.js';
import { GenerationContext, GenerationPhases, generateMapFeatures } from '/base-standard/scripts/common-generation.js';
import { assignStartPositionsFromHexMap } from '/base-standard/maps/assign-starting-plots.js';
import { HexMap } from '/base-standard/scripts/hex-map.js';
import { profileScope } from '/base-standard/scripts/profiling.js';

console.log("Generatingmap from Civ7Map script TestRivers.js.");
function requestMapData(initParams) {
  console.log("Begin requestMapData()");
  console.log(initParams.width);
  console.log(initParams.height);
  console.log(initParams.topLatitude);
  console.log(initParams.bottomLatitude);
  console.log(initParams.wrapX);
  console.log(initParams.wrapY);
  console.log(initParams.mapSize);
  engine.call("SetMapInitData", initParams);
  console.log("End requestMapData()");
}
async function generateMap() {
  console.log("Begin generateMap()");
  console.log(`Age - ${GameInfo.Ages.lookup(Game.age).AgeType}`);
  const civ7MapScope = new profileScope("TestRivers.js Generation");
  const iWidth = GameplayMap.getGridWidth();
  const iHeight = GameplayMap.getGridHeight();
  const uiMapSize = GameplayMap.getMapSize();
  const mapInfo = GameInfo.Maps.lookup(uiMapSize);
  if (mapInfo == null) return;
  const hexMap = new HexMap();
  hexMap.initFromTerrainBuilder();
  paintElevation();
  paintNaturalWonders(iWidth, iHeight);
  //paintEarthHugeResourcesAntiquity();
  const topSnowRows = 12;
  const bottomSnowRows = 0;
  const maxSnowWeight = 100;
  const snowRandomization = 20;
  paintSnow(iWidth, iHeight, topSnowRows, bottomSnowRows, maxSnowWeight, snowRandomization);
  const genCtx = new GenerationContext();
  genCtx.phases = GenerationPhases.WriteToTerrainBuilder| GenerationPhases.NaturalWonders;
  genCtx.bRunAestheticRiverValidation = false;
  generateMapFeatures(hexMap, genCtx);
  paintRivers();
  nameRivers();
  //nameVolcanoes();
  fakeWrapX();
  const startPositions = assignStartPositionsFromHexMap(hexMap);  generateDiscoveries(iWidth, iHeight, startPositions, g_PolarWaterRows);
  civ7MapScope.end();
  console.log("End generateMap()");
}
function paintElevation() {
    let elevationArray = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    TerrainBuilder.setElevation(elevationArray);
    TerrainBuilder.generateCliffsFromElevation();
}

function paintRivers()
{
  console.log("Begin paintRivers()");
  // RIVER_FOO
  TerrainBuilder.setRiverInfo(26, 26, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(25, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(24, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(23, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(22, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(21, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(20, 27, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(20, 28, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(19, 29, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 29, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 29, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 29, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 30, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(15, 31, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 32, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 33, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 34, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(15, 35, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(10, 29, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 29, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 29, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(13, 29, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 30, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 31, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 31, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 32, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 33, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 34, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(15, 35, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(24, 31, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(23, 31, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(23, 32, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(22, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(21, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(20, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(19, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 33, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 33, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 34, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(15, 35, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  // RIVER_SIX
  TerrainBuilder.setRiverInfo(12, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 23, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 22, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(9, 21, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 20, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(9, 19, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 18, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 17, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 17, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 17, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(13, 18, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 19, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 20, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  // RIVER_SEVEN
  TerrainBuilder.setRiverInfo(16, 17, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 18, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 19, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 20, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 21, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(19, 22, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(16, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(15, 23, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  console.log("End paintRivers()");
}

function nameRivers()
{
   console.log("Begin nameRivers()");
   TerrainBuilder.setCustomRiverName(26, 26, "Foo");
   TerrainBuilder.setCustomRiverName(12, 23, "Six");
   TerrainBuilder.setCustomRiverName(16, 17, "Seven");
   console.log("End nameRivers()");
}

function getDistanceToClosestOtherStart(iX, iY, startPositions, skipIndex) { 
  let minDistance = 32768;
  for (let iStart = 0; iStart < startPositions.length; iStart++) {
    const startPlotIndex = startPositions[iStart];
    if (startPlotIndex && iStart != skipIndex) {
      const iStartX = startPlotIndex % GameplayMap.getGridWidth();
      const iStartY = startPlotIndex / GameplayMap.getGridWidth();
      const distance = GameplayMap.getPlotDistance(iX, iY, iStartX, iStartY);
      if (distance < minDistance) {
        minDistance = distance;
      }
    }
  }
  return minDistance;
}
function getRandomCivStartLocation() {
  const randomCiv = TerrainBuilder.getRandomNumber(0, "CIV TSL RANDOMIZATION");
  console.log(randomCiv + " ");
  let plotIndex = -1;
  switch (randomCiv) {
  }
  return plotIndex;
}
function assignStartPositions() {
  const aliveMajorIds = Players.getAliveMajorIds();
  const startPositions = new Array(aliveMajorIds.length);
  let plotIndex = -1;
  for (const majorId of aliveMajorIds) {
    const player = Players.get(majorId);
    if (player != null) {
      switch (player.civilizationName) {
      }
    }
    if (plotIndex >= 0) {
      startPositions[majorId] = plotIndex;
      const location2 = GameplayMap.getLocationFromIndex(plotIndex);
      console.log("CHOICE FOR PLAYER: " + majorId + " (" + location2.x + ", " + location2.y + ")");
    } else {
      console.log("FAILED TO PICK LOCATION FOR: " + majorId);
    }
    plotIndex = -1;
  }
  const validatedStartPositions = startPositions;
  console.log(startPositions.length + "startlength");
  validatedStartPositions[0] = startPositions[0];
  const location = GameplayMap.getLocationFromIndex(validatedStartPositions[0]);
  console.log("CHOICE FOR PLAYER: 0 (" + location.x + ", " + location.y + ")");
  StartPositioner.setStartPosition(validatedStartPositions[0], 0);
  for (let index = startPositions.length - 1; index > 0; index--) {
    let invalidPosition = false;
    let position = startPositions[index];
    for (let indexCheck = 0; indexCheck < index; indexCheck++) {
      console.log(
        getDistanceToClosestOtherStart(
          GameplayMap.getLocationFromIndex(position).x,
          GameplayMap.getLocationFromIndex(position).y,
          startPositions,
          index
        )
      );
      if (getDistanceToClosestOtherStart(
        GameplayMap.getLocationFromIndex(position).x,
        GameplayMap.getLocationFromIndex(position).y,
        startPositions,
        index
      ) < 3) {
        invalidPosition = true;
        console.log("check1");
      } else {
        console.log(index + " fine " + indexCheck);
      }
    }
    while (invalidPosition) {
      position = getRandomCivStartLocation();
      invalidPosition = false;
      for (let indexCheck = 0; indexCheck < startPositions.length; indexCheck++) {
        if (index != indexCheck) {
          if (getDistanceToClosestOtherStart(
            GameplayMap.getLocationFromIndex(position).x,
            GameplayMap.getLocationFromIndex(position).y,
            startPositions,
            index
          ) < 4) {
            invalidPosition = true;
            console.log("check2");
          }
        }
      }
    }
    if (position >= 0) {
      validatedStartPositions[index] = position;
      const location2 = GameplayMap.getLocationFromIndex(position);
      console.log("CHOICE FOR PLAYER: " + index + " (" + location2.x + ", " + location2.y + ")");
      StartPositioner.setStartPosition(position, index);
    } else {
      console.log("FAILED TO PICK LOCATION FOR: " + index);
    }
  }
  return validatedStartPositions;
}
function fakeWrapX()
{ 
}

function paintSnow(width, height, topRows, bottomRows, maxWeight, randomization) {
    console.log("Generating permanent snow");
    const aLightSnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags(["SNOW", "LIGHT", "PERMANENT"]);
    const aMediumSnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags(["SNOW", "MEDIUM", "PERMANENT"]);
    const aHeavySnowEffects = MapPlotEffects.getPlotEffectTypesContainingTags(["SNOW", "HEAVY", "PERMANENT"]);
    const aWeightEffect = [-1, -1, -1];
    aWeightEffect[0] = aLightSnowEffects ? aLightSnowEffects[0] : -1;
    aWeightEffect[1] = aMediumSnowEffects ? aMediumSnowEffects[0] : -1;
    aWeightEffect[2] = aHeavySnowEffects ? aHeavySnowEffects[0] : -1;
    const aWeightChance = [10, 30, 60];
    const weightAdjustment = maxWeight / 100;
    const placeSnow = (x, y, percentChance) => {
        percentChance *= weightAdjustment;
        if (!GameplayMap.isWater(x, y)) {
            const snowRandomization = TerrainBuilder.getRandomNumber(randomization * 2, "Snow weight randomization");
            const snowWeight = percentChance + snowRandomization - randomization;
            if (snowWeight > 0) {
                for (let weight = aWeightChance.length - 1; weight >= 0; --weight) {
                    if (snowWeight > aWeightChance[weight]) {
                        MapPlotEffects.addPlotEffect(GameplayMap.getIndexFromXY(x, y), aWeightEffect[weight]);
                        break;
                    }
                }
            }
        }
    };
    if (topRows > 0) {
        const endTopRow = height - topRows;
        for (let row = height; row > endTopRow; --row) {
            const snowPercent = (1 - (height - row) / topRows) * 100;
            for (let col = 0; col < width; ++col) {
                placeSnow(col, row, snowPercent);
            }
        }
    }
    if (bottomRows > 0) {
        for (let row = 0; row < bottomRows; ++row) {
            const snowPercent = (1 - row / bottomRows) * 100;
            for (let col = 0; col < width; ++col) {
                placeSnow(col, row, snowPercent);
            }
        }
    }
}

async function paintNaturalWonders(width, height) {
  console.log("Begin paintNaturalWonders");
  let aPossibleWonders = [];
  let iElevation
  let featureParam;
  let placementX ;
  let placementY;
  for (const nwDef of GameInfo.Feature_NaturalWonders) 
  {
      aPossibleWonders.push(nwDef.$hash);
  }
  for (let iI = 0; iI < aPossibleWonders.length; iI++)
  {
      let eFeature = aPossibleWonders[iI];
      let nwDef = GameInfo.Feature_NaturalWonders.lookup(eFeature);
      if (nwDef != null)
      {
          console.log("Placing natural wonder " + nwDef.FeatureType + " ...");
          let possibleLocations = ""
          let possibleLocationCount = 0;
          for (let iY = height - 1; iY >= 0; iY--)
          {
              for (let iX = 0; iX < width; iX++)
              {
                  iElevation = GameplayMap.getElevation(iX, iY);
                  featureParam = 
                  {
                      Feature: nwDef.$hash,
                      Direction : nwDef.Direction,
                      Elevation : iElevation
                  };
                  if (TerrainBuilder.canHaveFeatureParam(iX, iY, featureParam))
                  {
                      if (possibleLocations.length > 0)
                      {
                          possibleLocations += ", ";
                      }
                      possibleLocations += "(" + String(iX).padStart(2, '0') + ", " + String(iY).padStart(2, '0') + ")";
                      possibleLocationCount++;
                  }
              }
              if (possibleLocations.length > 0)
              {
                  console.log(possibleLocations);
                  possibleLocations = "";
              }
          }
          if (possibleLocations.length > 0)
          {
              console.log(possibleLocations);
          }
          console.log(possibleLocationCount + " possible location(s) found.");
          switch (nwDef.FeatureType)
          {
                      default:
                          console.log(nwDef.FeatureType + " had no custom location defined."); 
                      break;
          }
      }
  }
  console.log("End paintNaturalWonders");
}
engine.on("RequestMapInitData", requestMapData);
engine.on("GenerateMap", generateMap);
//# sourceMappingURL=Earth_Huge.js.map
