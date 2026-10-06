import { generateDiscoveries } from '/base-standard/maps/discovery-generator.js';
import { g_PolarWaterRows } from '/base-standard/maps/map-globals.js';
import { shuffle } from '/base-standard/maps/map-utilities.js';
import { GenerationContext, GenerationPhases, generateMapFeatures } from '/base-standard/scripts/common-generation.js';
import { assignStartPositionsFromHexMap } from '/base-standard/maps/assign-starting-plots.js';import { HexMap } from '/base-standard/scripts/hex-map.js';
import { profileScope } from '/base-standard/scripts/profiling.js';

console.log("Generatingmap from Civ7Map script Earth_Small_8P_Multiplayer.js.");
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
  const civ7MapScope = new profileScope("Earth_Small_8P_Multiplayer.js Generation");
  const iWidth = GameplayMap.getGridWidth();
  const iHeight = GameplayMap.getGridHeight();
  const uiMapSize = GameplayMap.getMapSize();
  const mapInfo = GameInfo.Maps.lookup(uiMapSize);
  if (mapInfo == null) return;
  const hexMap = new HexMap();
  hexMap.initFromTerrainBuilder();
  paintElevation();
  //paintEarthHugeRivers();
  //paintEarthHugeNaturalWonders();
  //paintEarthHugeResourcesAntiquity();
  const topSnowRows = 6;
  const bottomSnowRows = 5;
  const maxSnowWeight = 100;
  const snowRandomization = 20;
  paintSnow(iWidth, iHeight, topSnowRows, bottomSnowRows, maxSnowWeight, snowRandomization);
  const genCtx = new GenerationContext();
  genCtx.phases = GenerationPhases.WriteToTerrainBuilder| GenerationPhases.Lakes| GenerationPhases.Elevation| GenerationPhases.Hills| GenerationPhases.Rainfall| GenerationPhases.Rivers| GenerationPhases.NaturalWonders| GenerationPhases.m_randomFloodPlains| GenerationPhases.Features| GenerationPhases.Resources;
  genCtx.bRunAestheticRiverValidation = false;
  generateMapFeatures(hexMap, genCtx);
  //nameRivers();
  //nameVolcanoes();
  fakeWrapX();
  const startPositions = assignStartPositions();
  generateDiscoveries(iWidth, iHeight, startPositions, g_PolarWaterRows);
  civ7MapScope.end();
  console.log("End generateMap()");
}
function paintElevation() {
    let elevationArray = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    TerrainBuilder.setElevation(elevationArray);
    TerrainBuilder.generateCliffsFromElevation();
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
  const randomCiv = TerrainBuilder.getRandomNumber(62, "CIV TSL RANDOMIZATION");
  console.log(randomCiv + " ");
  let plotIndex = -1;
  switch (randomCiv) {
    case 0:
      plotIndex = GameplayMap.getIndexFromXY(46, 20);
      break;
    case 1:
      plotIndex = GameplayMap.getIndexFromXY(42, 25);
      break;
    case 2:
      plotIndex = GameplayMap.getIndexFromXY(42, 30);
      break;
    case 3:
      plotIndex = GameplayMap.getIndexFromXY(58, 35);
      break;
    case 4:
      plotIndex = GameplayMap.getIndexFromXY(59, 29);
      break;
    case 5:
      plotIndex = GameplayMap.getIndexFromXY(55, 27);
      break;
    case 6:
      plotIndex = GameplayMap.getIndexFromXY(11, 22);
      break;
    case 7:
      plotIndex = GameplayMap.getIndexFromXY(7, 34);
      break;
    case 8:
      plotIndex = GameplayMap.getIndexFromXY(51, 30);
      break;
    case 9:
      plotIndex = GameplayMap.getIndexFromXY(36, 31);
      break;
    case 10:
      plotIndex = GameplayMap.getIndexFromXY(34, 26);
      break;
    case 11:
      plotIndex = GameplayMap.getIndexFromXY(45, 28);
      break;
    case 12:
      plotIndex = GameplayMap.getIndexFromXY(64, 39);
      break;
    case 13:
      plotIndex = GameplayMap.getIndexFromXY(48, 28);
      break;
    case 14:
      plotIndex = GameplayMap.getIndexFromXY(31, 33);
      break;
    case 15:
      plotIndex = GameplayMap.getIndexFromXY(64, 40);
      break;
    case 16:
      plotIndex = GameplayMap.getIndexFromXY(68, 35);
      break;
    case 17:
      plotIndex = GameplayMap.getIndexFromXY(69, 35);
      break;
    case 18:
      plotIndex = GameplayMap.getIndexFromXY(45, 27);
      break;
    case 19:
      plotIndex = GameplayMap.getIndexFromXY(56, 24);
      break;
    case 20:
      plotIndex = GameplayMap.getIndexFromXY(2, 23);
      break;
    case 21:
      plotIndex = GameplayMap.getIndexFromXY(16, 11);
      break;
    case 22:
      plotIndex = GameplayMap.getIndexFromXY(61, 18);
      break;
    case 23:
      plotIndex = GameplayMap.getIndexFromXY(61, 34);
      break;
    case 24:
      plotIndex = GameplayMap.getIndexFromXY(57, 40);
      break;
    case 25:
      plotIndex = GameplayMap.getIndexFromXY(28, 34);
      break;
    case 26:
      plotIndex = GameplayMap.getIndexFromXY(32, 17);
      break;
    case 27:
      plotIndex = GameplayMap.getIndexFromXY(27, 27);
      break;
    case 28:
      plotIndex = GameplayMap.getIndexFromXY(11, 34);
      break;
    case 29:
      plotIndex = GameplayMap.getIndexFromXY(39, 33);
      break;
    case 30:
      plotIndex = GameplayMap.getIndexFromXY(60, 29);
      break;
    case 31:
      plotIndex = GameplayMap.getIndexFromXY(22, 44);
      break;
    case 32:
      plotIndex = GameplayMap.getIndexFromXY(43, 33);
      break;
    case 33:
      plotIndex = GameplayMap.getIndexFromXY(65, 40);
      break;
    case 34:
      plotIndex = GameplayMap.getIndexFromXY(13, 33);
      break;
    case 35:
      plotIndex = GameplayMap.getIndexFromXY(29, 35);
      break;
    case 36:
      plotIndex = GameplayMap.getIndexFromXY(9, 24);
      break;
    case 37:
      plotIndex = GameplayMap.getIndexFromXY(54, 28);
      break;
    case 38:
      plotIndex = GameplayMap.getIndexFromXY(40, 41);
      break;
    case 39:
      plotIndex = GameplayMap.getIndexFromXY(61, 40);
      break;
    case 40:
      plotIndex = GameplayMap.getIndexFromXY(42, 42);
      break;
    case 41:
      plotIndex = GameplayMap.getIndexFromXY(60, 28);
      break;
    case 42:
      plotIndex = GameplayMap.getIndexFromXY(28, 39);
      break;
    case 43:
      plotIndex = GameplayMap.getIndexFromXY(69, 34);
      break;
    case 44:
      plotIndex = GameplayMap.getIndexFromXY(41, 13);
      break;
    case 45:
      plotIndex = GameplayMap.getIndexFromXY(57, 34);
      break;
    case 46:
      plotIndex = GameplayMap.getIndexFromXY(47, 30);
      break;
    case 47:
      plotIndex = GameplayMap.getIndexFromXY(39, 40);
      break;
    case 48:
      plotIndex = GameplayMap.getIndexFromXY(29, 39);
      break;
    case 49:
      plotIndex = GameplayMap.getIndexFromXY(64, 25);
      break;
    case 50:
      plotIndex = GameplayMap.getIndexFromXY(46, 36);
      break;
    case 51:
      plotIndex = GameplayMap.getIndexFromXY(67, 35);
      break;
    case 52:
      plotIndex = GameplayMap.getIndexFromXY(2, 7);
      break;
    case 53:
      plotIndex = GameplayMap.getIndexFromXY(29, 38);
      break;
    case 54:
      plotIndex = GameplayMap.getIndexFromXY(47, 28);
      break;
    case 55:
      plotIndex = GameplayMap.getIndexFromXY(37, 34);
      break;
    case 56:
      plotIndex = GameplayMap.getIndexFromXY(24, 27);
      break;
    case 57:
      plotIndex = GameplayMap.getIndexFromXY(70, 34);
      break;
    case 58:
      plotIndex = GameplayMap.getIndexFromXY(17, 24);
      break;
    case 59:
      plotIndex = GameplayMap.getIndexFromXY(65, 24);
      break;
    case 60:
      plotIndex = GameplayMap.getIndexFromXY(21, 6);
      break;
    case 61:
      plotIndex = GameplayMap.getIndexFromXY(26, 27);
      break;
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
        case "LOC_CIVILIZATION_AKSUM_NAME":
          plotIndex = GameplayMap.getIndexFromXY(46, 20);
          break;
        case "LOC_CIVILIZATION_EGYPT_NAME":
          plotIndex = GameplayMap.getIndexFromXY(42, 25);
          break;
        case "LOC_CIVILIZATION_GREECE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(42, 30);
          break;
        case "LOC_CIVILIZATION_HAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(58, 35);
          break;
        case "LOC_CIVILIZATION_KHMER_NAME":
          plotIndex = GameplayMap.getIndexFromXY(59, 29);
          break;
        case "LOC_CIVILIZATION_MAURYA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(55, 27);
          break;
        case "LOC_CIVILIZATION_MAYA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(11, 22);
          break;
        case "LOC_CIVILIZATION_MISSISSIPPIAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(7, 34);
          break;
        case "LOC_CIVILIZATION_PERSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(51, 30);
          break;
        case "LOC_CIVILIZATION_ROME_NAME":
          plotIndex = GameplayMap.getIndexFromXY(36, 31);
          break;
        case "LOC_CIVILIZATION_CARTHAGE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(34, 26);
          break;
        case "LOC_CIVILIZATION_ASSYRIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(45, 28);
          break;
        case "LOC_CIVILIZATION_SILLA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(64, 39);
          break;
        case "LOC_CIVILIZATION_BABYLON_NAME":
          plotIndex = GameplayMap.getIndexFromXY(48, 28);
          break;
        case "LOC_CIVILIZATION_GAUL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(31, 33);
          break;
        case "LOC_CIVILIZATION_GORYEO_NAME":
          plotIndex = GameplayMap.getIndexFromXY(64, 40);
          break;
        case "LOC_CIVILIZATION_HEIAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(68, 35);
          break;
        case "LOC_CIVILIZATION_SENGOKU_NAME":
          plotIndex = GameplayMap.getIndexFromXY(69, 35);
          break;
        case "LOC_CIVILIZATION_ABBASID_NAME":
          plotIndex = GameplayMap.getIndexFromXY(45, 27);
          break;
        case "LOC_CIVILIZATION_CHOLA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(56, 24);
          break;
        case "LOC_CIVILIZATION_HAWAII_NAME":
          plotIndex = GameplayMap.getIndexFromXY(2, 23);
          break;
        case "LOC_CIVILIZATION_INCA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(16, 11);
          break;
        case "LOC_CIVILIZATION_MAJAPAHIT_NAME":
          plotIndex = GameplayMap.getIndexFromXY(61, 18);
          break;
        case "LOC_CIVILIZATION_MING_NAME":
          plotIndex = GameplayMap.getIndexFromXY(61, 34);
          break;
        case "LOC_CIVILIZATION_MONGOLIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(57, 40);
          break;
        case "LOC_CIVILIZATION_NORMAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(28, 34);
          break;
        case "LOC_CIVILIZATION_SONGHAI_NAME":
          plotIndex = GameplayMap.getIndexFromXY(32, 17);
          break;
        case "LOC_CIVILIZATION_SPAIN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(27, 27);
          break;
        case "LOC_CIVILIZATION_SHAWNEE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(11, 34);
          break;
        case "LOC_CIVILIZATION_BULGARIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(39, 33);
          break;
        case "LOC_CIVILIZATION_DAI_VIET_NAME":
          plotIndex = GameplayMap.getIndexFromXY(60, 29);
          break;
        case "LOC_CIVILIZATION_ICELAND_NAME":
          plotIndex = GameplayMap.getIndexFromXY(22, 44);
          break;
        case "LOC_CIVILIZATION_OTTOMANS_NAME":
          plotIndex = GameplayMap.getIndexFromXY(43, 33);
          break;
        case "LOC_CIVILIZATION_JOSEON_NAME":
          plotIndex = GameplayMap.getIndexFromXY(65, 40);
          break;
        case "LOC_CIVILIZATION_AMERICA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(13, 33);
          break;
        case "LOC_CIVILIZATION_FRENCH_EMPIRE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(29, 35);
          break;
        case "LOC_CIVILIZATION_MEXICO_NAME":
          plotIndex = GameplayMap.getIndexFromXY(9, 24);
          break;
        case "LOC_CIVILIZATION_MUGHAL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(54, 28);
          break;
        case "LOC_CIVILIZATION_PRUSSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(40, 41);
          break;
        case "LOC_CIVILIZATION_QING_NAME":
          plotIndex = GameplayMap.getIndexFromXY(61, 40);
          break;
        case "LOC_CIVILIZATION_RUSSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(42, 42);
          break;
        case "LOC_CIVILIZATION_SIAM_NAME":
          plotIndex = GameplayMap.getIndexFromXY(60, 28);
          break;
        case "LOC_CIVILIZATION_GREAT_BRITAIN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(28, 39);
          break;
        case "LOC_CIVILIZATION_MEIJI_NAME":
          plotIndex = GameplayMap.getIndexFromXY(69, 34);
          break;
        case "LOC_CIVILIZATION_BUGANDA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(41, 13);
          break;
        case "LOC_CIVILIZATION_NEPAL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(57, 34);
          break;
        case "LOC_CIVILIZATION_QAJAR_NAME":
          plotIndex = GameplayMap.getIndexFromXY(47, 30);
          break;
        case "LOC_CIVILIZATION_GERMANIA_JEC_NAME":
          plotIndex = GameplayMap.getIndexFromXY(39, 40);
          break;
        case "LOC_CIVILIZATION_SAXON_NAME":
          plotIndex = GameplayMap.getIndexFromXY(29, 39);
          break;
        case "LOC_CIVILIZATION_SANSEB_TAGALOG_NAME":
          plotIndex = GameplayMap.getIndexFromXY(64, 25);
          break;
        case "LOC_CIVILIZATION_HUNS_ROG_NAME":
          plotIndex = GameplayMap.getIndexFromXY(46, 36);
          break;
        case "LOC_CIVILIZATION_PB_YAMATAI_NAME":
          plotIndex = GameplayMap.getIndexFromXY(67, 35);
          break;
        case "LOC_CIVILIZATION_TONGA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(2, 7);
          break;
        case "LOC_CIVILIZATION_ENGLAND_NAME":
          plotIndex = GameplayMap.getIndexFromXY(29, 38);
          break;
        case "LOC_CIVILIZATION_TCS_OUTREMER_NAME":
          plotIndex = GameplayMap.getIndexFromXY(47, 28);
          break;
        case "LOC_CIVILIZATION_VENICE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(37, 34);
          break;
        case "LOC_CIVILIZATION_DOURADOS_PORTUGAL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(24, 27);
          break;
        case "LOC_CIVILIZATION_EDO_NAME":
          plotIndex = GameplayMap.getIndexFromXY(70, 34);
          break;
        case "LOC_CIVILIZATION_PIRATE_REPUBLIC_NAME":
          plotIndex = GameplayMap.getIndexFromXY(17, 24);
          break;
        case "LOC_CIVILIZATION_SANSEB_PHILIPPINES_NAME":
          plotIndex = GameplayMap.getIndexFromXY(65, 24);
          break;
        case "LOC_CIVILIZATION_NYGUITA_ARGENTINA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(21, 6);
          break;
        case "LOC_CIVILIZATION_ARTHUR_SPANISH_EMPIRE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(26, 27);
          break;
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
      ) < 4) {
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
engine.on("RequestMapInitData", requestMapData);
engine.on("GenerateMap", generateMap);
//# sourceMappingURL=Earth_Huge.js.map
