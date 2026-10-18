import { generateDiscoveries } from '/base-standard/maps/discovery-generator.js';
import { g_PolarWaterRows } from '/base-standard/maps/map-globals.js';
import { shuffle } from '/base-standard/maps/map-utilities.js';
import { GenerationContext, GenerationPhases, generateMapFeatures } from '/base-standard/scripts/common-generation.js';
import { HexMap } from '/base-standard/scripts/hex-map.js';
import { profileScope } from '/base-standard/scripts/profiling.js';

console.log("Generating Earth_Huge map from Civ7Map and script helper.");
function requestMapData(initParams) {
  console.log(initParams.width);
  console.log(initParams.height);
  console.log(initParams.topLatitude);
  console.log(initParams.bottomLatitude);
  console.log(initParams.wrapX);
  console.log(initParams.wrapY);
  console.log(initParams.mapSize);
  engine.call("SetMapInitData", initParams);
}
async function generateMap() {
  console.log("Generating a map!");
  console.log(`Age - ${GameInfo.Ages.lookup(Game.age).AgeType}`);
  const earthHugeScope = new profileScope("Earth_Huge Generation");
  const iWidth = GameplayMap.getGridWidth();
  const iHeight = GameplayMap.getGridHeight();
  const uiMapSize = GameplayMap.getMapSize();
  const mapInfo = GameInfo.Maps.lookup(uiMapSize);
  if (mapInfo == null) return;
  const hexMap = new HexMap();
  hexMap.initFromTerrainBuilder();
  paintEarthHugeElevation();
  paintEarthHugeRivers();
  paintEarthHugeNaturalWonders();
  paintEarthHugeResourcesAntiquity();
  const topSnowRows = 11;
  const bottomSnowRows = 0;
  const maxSnowWeight = 60;
  const snowRandomization = 20;
  paintEarthHugeSnow(iWidth, iHeight, topSnowRows, bottomSnowRows, maxSnowWeight, snowRandomization);
  const genCtx = new GenerationContext();
  genCtx.phases = GenerationPhases.Rainfall | GenerationPhases.FloodPlains;
  genCtx.bRunAestheticRiverValidation = false;
  generateMapFeatures(hexMap, genCtx);
  nameRivers();
  nameVolcanoes();
  const startPositions = assignStartPositionsEarthHuge();
  generateDiscoveries(iWidth, iHeight, startPositions, g_PolarWaterRows);
  earthHugeScope.end();
}
function paintEarthHugeElevation() {
  let elevationArray = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 700, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 350, 0, 0, 600, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 350, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 350, 500, 500, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1100, 1100, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 700, 500, 500, 500, 500, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 700, 200, 0, 0, 0, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 222, 223, 224, 225, 500, 700, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 200, 0, 201, 450, 700, 1400, 200, 0, 0, 0, 500, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 221, 500, 500, 500, 500, 700, 900, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 0, 0, 0, 0, 0, 200, 200, 200, 202, 203, 204, 650, 350, 0, 0, 0, 350, 0, 600, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 700, 201, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 700, 700, 500, 500, 1100, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 200, 200, 0, 0, 0, 200, 200, 200, 200, 200, 223, 450, 350, 200, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 202, 300, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 500, 700, 500, 500, 226, 225, 500, 350, 221, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 350, 350, 450, 200, 500, 600, 200, 200, 200, 160, 200, 200, 224, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 203, 222, 300, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 500, 500, 500, 500, 227, 500, 224, 223, 222, 200, 0, 200, 200, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 200, 350, 350, 450, 400, 450, 500, 450, 450, 450, 450, 160, 200, 200, 450, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 204, 205, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 500, 500, 700, 700, 500, 500, 500, 500, 500, 200, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 200, 350, 350, 450, 600, 450, 600, 450, 450, 600, 450, 450, 300, 300, 450, 450, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 500, 350, 206, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 500, 700, 800, 700, 700, 700, 500, 500, 200, 0, 0, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 450, 600, 450, 600, 450, 400, 600, 600, 450, 450, 600, 450, 450, 450, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 700, 350, 600, 450, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 700, 500, 700, 700, 900, 900, 700, 500, 500, 200, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 450, 450, 600, 450, 600, 400, 450, 700, 450, 600, 700, 450, 600, 450, 450, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 400, 1e3, 350, 350, 650, 600, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 500, 500, 500, 700, 700, 700, 500, 900, 200, 0, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 400, 600, 500, 600, 700, 450, 500, 600, 450, 600, 450, 600, 450, 450, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 1100, 1100, 500, 350, 550, 600, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 223, 500, 207, 500, 500, 600, 500, 160, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 400, 450, 600, 450, 600, 450, 500, 600, 450, 450, 600, 450, 450, 450, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 500, 450, 350, 350, 700, 450, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 201, 222, 200, 206, 200, 160, 350, 900, 160, 500, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 500, 450, 450, 600, 500, 450, 600, 450, 600, 450, 450, 200, 200, 0, 0, 0, 0, 0, 0, 0, 400, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 160, 450, 600, 450, 450, 450, 450, 450, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 202, 200, 205, 225, 200, 200, 160, 350, 500, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 400, 450, 450, 500, 450, 450, 450, 450, 450, 200, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 160, 450, 600, 229, 500, 226, 500, 203, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 203, 204, 200, 200, 200, 160, 350, 500, 500, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 400, 200, 500, 450, 200, 200, 200, 200, 200, 0, 200, 0, 0, 0, 0, 0, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 1100, 1100, 600, 228, 228, 227, 225, 550, 202, 201, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 223, 200, 200, 350, 350, 500, 500, 160, 160, 700, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 200, 450, 200, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 1100, 500, 228, 227, 227, 226, 225, 224, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 224, 225, 350, 500, 900, 500, 500, 160, 700, 700, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 200, 200, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 229, 227, 227, 226, 226, 225, 224, 204, 500, 500, 500, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 200, 350, 500, 900, 160, 500, 218, 200, 500, 350, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 228, 207, 206, 225, 224, 223, 222, 203, 500, 350, 350, 0, 0, 0, 0, 0, 231, 200, 200, 0, 0, 200, 200, 200, 500, 200, 350, 350, 350, 200, 217, 500, 500, 350, 700, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 209, 208, 226, 205, 204, 223, 222, 202, 350, 350, 0, 0, 0, 0, 0, 0, 230, 230, 200, 200, 200, 201, 200, 200, 500, 500, 500, 200, 200, 200, 200, 216, 200, 200, 350, 700, 400, 200, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 200, 200, 0, 350, 0, 0, 200, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 350, 1100, 228, 227, 225, 224, 203, 202, 222, 201, 0, 0, 0, 0, 0, 0, 0, 230, 210, 200, 350, 200, 200, 202, 200, 500, 500, 500, 200, 200, 200, 200, 200, 215, 200, 200, 350, 700, 400, 200, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 700, 500, 700, 700, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 227, 226, 225, 223, 222, 201, 350, 0, 0, 0, 0, 0, 0, 0, 200, 350, 209, 350, 350, 200, 203, 200, 500, 500, 500, 350, 350, 350, 350, 200, 200, 214, 200, 200, 350, 700, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 300, 200, 0, 0, 0, 200, 200, 200, 0, 0, 0, 200, 0, 200, 0, 350, 350, 0, 0, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 350, 226, 224, 222, 221, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 208, 200, 200, 204, 200, 200, 500, 500, 200, 350, 650, 350, 200, 350, 350, 213, 800, 231, 232, 700, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 200, 450, 450, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 160, 400, 550, 350, 300, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 350, 207, 206, 205, 200, 500, 500, 500, 500, 200, 200, 350, 350, 350, 200, 212, 229, 230, 350, 350, 350, 450, 350, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 500, 0, 0, 200, 200, 0, 0, 450, 350, 350, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 200, 350, 350, 160, 380, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 350, 350, 350, 200, 200, 200, 160, 200, 200, 200, 200, 200, 350, 350, 210, 211, 228, 700, 550, 650, 0, 0, 450, 350, 0, 0, 0, 0, 0, 0, 300, 350, 350, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 350, 350, 0, 0, 0, 200, 0, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 0, 0, 350, 320, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 450, 450, 450, 450, 450, 450, 200, 160, 200, 200, 200, 200, 200, 350, 350, 209, 200, 200, 227, 700, 650, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 300, 350, 350, 200, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 350, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 800, 0, 350, 500, 350, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 200, 200, 450, 450, 450, 450, 200, 200, 200, 500, 500, 200, 350, 350, 350, 200, 350, 208, 207, 226, 200, 350, 0, 950, 350, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 221, 0, 0, 0, 0, 0, 0, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 200, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 350, 350, 350, 350, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 450, 450, 450, 350, 350, 350, 200, 500, 200, 350, 700, 700, 350, 200, 350, 350, 206, 200, 350, 0, 350, 650, 350, 700, 0, 0, 0, 0, 0, 0, 350, 500, 222, 200, 0, 0, 0, 0, 0, 350, 350, 0, 221, 201, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 160, 800, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 450, 450, 450, 350, 350, 200, 350, 200, 200, 350, 700, 1e3, 700, 350, 200, 350, 205, 200, 200, 0, 350, 650, 650, 350, 350, 0, 0, 0, 0, 0, 0, 350, 223, 200, 200, 0, 0, 0, 350, 0, 350, 200, 222, 202, 500, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 160, 350, 0, 0, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 350, 700, 200, 200, 350, 200, 200, 350, 700, 700, 350, 200, 200, 204, 200, 200, 0, 350, 650, 650, 350, 200, 450, 0, 0, 0, 0, 0, 200, 500, 200, 200, 200, 0, 0, 0, 350, 650, 350, 223, 203, 222, 0, 200, 200, 0, 0, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 350, 350, 0, 0, 0, 350, 350, 0, 0, 200, 0, 0, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 200, 200, 200, 350, 700, 350, 200, 350, 200, 200, 350, 350, 350, 200, 200, 203, 200, 200, 0, 350, 650, 650, 350, 200, 200, 200, 0, 0, 0, 0, 200, 500, 500, 200, 200, 200, 0, 0, 500, 350, 700, 350, 204, 223, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 350, 700, 500, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 200, 200, 350, 350, 200, 200, 350, 200, 200, 200, 200, 200, 200, 222, 202, 200, 0, 350, 650, 650, 450, 200, 200, 200, 0, 0, 0, 0, 200, 200, 200, 500, 200, 200, 200, 0, 200, 350, 350, 350, 600, 205, 200, 0, 200, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 350, 700, 500, 350, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 350, 350, 200, 200, 200, 200, 0, 0, 0, 200, 200, 221, 221, 201, 200, 350, 650, 650, 450, 200, 0, 200, 0, 350, 350, 201, 200, 200, 200, 200, 200, 200, 200, 201, 221, 350, 350, 900, 900, 206, 200, 200, 200, 200, 350, 350, 200, 0, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 350, 500, 600, 500, 350, 0, 0, 0, 0, 0, 0, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 350, 350, 500, 350, 350, 450, 350, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 350, 650, 450, 200, 200, 0, 0, 350, 700, 700, 202, 200, 500, 200, 205, 204, 203, 202, 350, 700, 350, 900, 900, 226, 350, 350, 350, 350, 350, 350, 350, 200, 0, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 350, 500, 700, 500, 221, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 700, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 650, 350, 200, 0, 0, 0, 350, 700, 550, 203, 1100, 200, 500, 206, 200, 200, 450, 800, 800, 800, 1100, 1100, 227, 900, 350, 350, 350, 350, 350, 350, 350, 200, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 221, 350, 500, 500, 500, 222, 350, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 200, 0, 0, 700, 350, 200, 0, 0, 350, 700, 1400, 550, 204, 550, 550, 850, 207, 800, 800, 1100, 1100, 1100, 1100, 1100, 228, 1100, 900, 350, 350, 350, 350, 350, 350, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 400, 222, 500, 500, 224, 223, 350, 0, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 350, 350, 0, 0, 0, 0, 0, 0, 350, 350, 0, 350, 0, 0, 0, 0, 0, 0, 350, 500, 350, 201, 350, 1400, 1400, 700, 550, 205, 206, 550, 850, 1100, 1100, 1100, 1100, 1100, 950, 1100, 929, 1100, 213, 212, 350, 350, 350, 350, 160, 350, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 250, 400, 223, 350, 350, 225, 350, 300, 300, 300, 300, 201, 200, 200, 300, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 201, 202, 500, 350, 400, 0, 0, 0, 200, 250, 0, 350, 350, 350, 0, 350, 350, 0, 0, 0, 0, 350, 550, 202, 350, 1400, 700, 950, 700, 650, 550, 1400, 1400, 950, 1400, 1400, 1550, 1550, 950, 1400, 1200, 1100, 214, 450, 211, 700, 1400, 1400, 700, 200, 160, 200, 200, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 550, 350, 374, 375, 350, 226, 350, 400, 400, 400, 202, 222, 400, 400, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 500, 500, 350, 0, 0, 300, 0, 0, 0, 0, 350, 350, 0, 0, 350, 0, 0, 200, 0, 300, 500, 203, 350, 700, 700, 700, 950, 700, 700, 700, 1400, 1100, 1400, 1400, 1550, 1550, 1100, 1100, 1400, 1100, 215, 550, 450, 210, 550, 550, 206, 205, 200, 200, 200, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 500, 1e3, 350, 375, 350, 350, 350, 225, 224, 223, 203, 300, 400, 500, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 800, 500, 350, 0, 0, 0, 350, 0, 0, 221, 350, 0, 0, 350, 350, 0, 0, 0, 0, 350, 204, 500, 700, 900, 700, 700, 950, 950, 1400, 1400, 1100, 1400, 1550, 1550, 1550, 1400, 1400, 1100, 1100, 216, 550, 650, 450, 209, 208, 207, 350, 204, 250, 201, 200, 0, 0, 0, 0, 0, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 500, 1e3, 350, 600, 350, 227, 226, 350, 350, 350, 204, 300, 400, 500, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 500, 700, 650, 400, 0, 0, 0, 200, 0, 350, 350, 0, 0, 350, 350, 0, 350, 600, 350, 350, 500, 205, 500, 350, 350, 900, 900, 700, 700, 1400, 1100, 1100, 1400, 1400, 1400, 1400, 1100, 1100, 550, 217, 550, 650, 650, 650, 550, 450, 350, 350, 203, 202, 200, 0, 0, 0, 0, 0, 350, 350, 700, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 700, 700, 350, 350, 350, 228, 350, 400, 400, 350, 205, 300, 300, 400, 500, 600, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 700, 350, 700, 700, 400, 0, 200, 0, 0, 350, 350, 0, 0, 200, 350, 0, 350, 350, 700, 700, 700, 700, 500, 500, 160, 350, 350, 350, 350, 1400, 1400, 950, 950, 950, 950, 950, 950, 750, 550, 550, 550, 750, 650, 650, 450, 650, 450, 600, 450, 450, 200, 0, 0, 300, 0, 0, 0, 350, 350, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 700, 1e3, 700, 1100, 350, 350, 400, 400, 350, 226, 206, 226, 227, 228, 300, 500, 232, 400, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 700, 700, 350, 350, 450, 0, 350, 350, 0, 200, 300, 700, 350, 0, 350, 350, 350, 300, 350, 1100, 500, 160, 160, 200, 400, 400, 200, 1400, 1400, 950, 950, 950, 950, 950, 950, 750, 750, 750, 750, 750, 650, 500, 407, 406, 405, 650, 450, 200, 0, 0, 0, 0, 0, 0, 0, 0, 500, 600, 350, 0, 0, 0, 0, 0, 0, 0, 0, 300, 500, 1100, 700, 700, 1100, 350, 350, 400, 350, 228, 227, 350, 207, 300, 160, 229, 230, 231, 160, 500, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 500, 224, 1100, 700, 350, 350, 700, 200, 200, 450, 700, 900, 350, 350, 0, 0, 0, 0, 350, 1e3, 160, 400, 200, 200, 400, 200, 200, 350, 500, 650, 800, 650, 650, 800, 650, 650, 650, 800, 750, 550, 550, 508, 330, 600, 404, 650, 450, 200, 0, 0, 0, 200, 200, 0, 0, 0, 350, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 1100, 1100, 700, 700, 1100, 500, 500, 500, 229, 500, 500, 208, 350, 350, 160, 350, 160, 350, 224, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 221, 222, 223, 500, 700, 1100, 1100, 700, 1100, 350, 350, 450, 900, 900, 201, 0, 0, 0, 0, 350, 800, 200, 160, 400, 160, 200, 400, 400, 1400, 1400, 1100, 1100, 650, 650, 650, 650, 650, 650, 650, 650, 750, 500, 509, 350, 200, 200, 203, 202, 300, 250, 200, 0, 0, 200, 350, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 350, 600, 700, 700, 700, 1100, 600, 500, 700, 500, 230, 500, 209, 350, 350, 350, 350, 160, 160, 350, 204, 350, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 500, 600, 500, 500, 204, 1100, 1100, 226, 225, 224, 204, 203, 202, 350, 0, 200, 0, 350, 500, 200, 200, 160, 400, 160, 200, 400, 200, 200, 1400, 1400, 1100, 1100, 1100, 350, 350, 350, 550, 526, 525, 550, 550, 510, 200, 350, 350, 350, 202, 250, 0, 0, 0, 200, 350, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 600, 700, 705, 700, 236, 235, 500, 500, 231, 500, 400, 500, 350, 160, 160, 350, 350, 200, 200, 203, 202, 201, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 200, 350, 350, 500, 500, 202, 203, 500, 500, 450, 350, 350, 700, 350, 350, 350, 221, 200, 200, 350, 500, 200, 200, 201, 201, 200, 200, 400, 200, 200, 200, 200, 1400, 1400, 550, 550, 550, 550, 550, 550, 550, 524, 550, 511, 400, 350, 350, 350, 350, 201, 0, 0, 0, 200, 350, 0, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 1e3, 710, 500, 500, 234, 233, 232, 500, 500, 500, 350, 350, 160, 350, 200, 200, 200, 200, 350, 350, 350, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 201, 222, 350, 500, 500, 500, 500, 500, 500, 500, 500, 350, 222, 200, 350, 500, 200, 200, 202, 200, 202, 203, 200, 400, 400, 200, 200, 1400, 400, 600, 400, 400, 600, 400, 400, 600, 400, 513, 512, 400, 400, 350, 500, 200, 200, 0, 200, 0, 0, 350, 0, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 700, 1100, 800, 500, 500, 500, 500, 500, 500, 700, 500, 350, 350, 350, 350, 350, 200, 500, 350, 350, 350, 350, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 221, 200, 350, 350, 350, 350, 350, 500, 700, 500, 350, 223, 200, 350, 350, 200, 200, 203, 400, 400, 400, 204, 400, 600, 400, 400, 300, 300, 500, 300, 500, 300, 300, 500, 300, 500, 300, 350, 350, 350, 350, 200, 200, 200, 200, 0, 200, 200, 200, 330, 350, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 200, 0, 0, 700, 700, 1400, 800, 500, 700, 700, 700, 700, 500, 500, 700, 500, 160, 350, 500, 350, 200, 200, 200, 350, 500, 700, 500, 350, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 350, 500, 0, 0, 200, 200, 200, 200, 200, 200, 350, 500, 500, 500, 350, 350, 500, 600, 500, 350, 200, 204, 400, 600, 400, 400, 600, 600, 600, 400, 400, 200, 350, 200, 200, 350, 200, 200, 350, 200, 350, 700, 700, 350, 350, 350, 500, 350, 200, 200, 200, 200, 700, 350, 350, 0, 0, 0, 200, 450, 0, 0, 0, 0, 0, 0, 350, 700, 0, 350, 1100, 800, 500, 700, 900, 900, 700, 500, 160, 500, 500, 160, 350, 600, 350, 350, 200, 0, 200, 500, 700, 700, 500, 200, 0, 350, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 221, 222, 221, 0, 0, 200, 200, 0, 0, 0, 200, 350, 350, 350, 500, 500, 350, 350, 350, 350, 350, 200, 205, 225, 400, 600, 600, 800, 600, 600, 400, 1100, 1100, 1100, 350, 200, 200, 350, 200, 200, 350, 700, 1e3, 700, 700, 350, 350, 500, 350, 200, 200, 200, 200, 160, 201, 0, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 700, 0, 350, 800, 500, 700, 700, 700, 700, 500, 350, 350, 350, 350, 350, 400, 350, 350, 200, 0, 200, 350, 500, 700, 500, 350, 200, 0, 0, 0, 0, 0, 0, 0, 0, 400, 200, 0, 200, 222, 200, 300, 0, 0, 200, 0, 0, 0, 200, 200, 200, 200, 350, 350, 350, 200, 200, 200, 200, 200, 206, 200, 226, 600, 600, 600, 600, 600, 600, 400, 400, 1100, 200, 160, 160, 200, 200, 160, 160, 350, 700, 700, 700, 700, 350, 200, 200, 200, 205, 204, 200, 201, 200, 350, 0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 300, 350, 350, 800, 800, 208, 500, 700, 500, 500, 500, 350, 350, 350, 350, 160, 350, 350, 200, 0, 0, 0, 200, 350, 500, 350, 500, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 400, 250, 0, 200, 500, 200, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 200, 200, 200, 200, 200, 229, 209, 208, 207, 200, 200, 400, 400, 500, 400, 500, 400, 500, 400, 1100, 400, 400, 400, 160, 200, 350, 350, 160, 350, 700, 700, 350, 350, 211, 200, 200, 206, 350, 203, 202, 201, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 200, 201, 202, 500, 500, 207, 500, 700, 500, 205, 206, 160, 350, 160, 350, 350, 400, 350, 200, 0, 0, 0, 0, 200, 200, 350, 350, 600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 0, 350, 200, 0, 0, 0, 200, 0, 0, 200, 0, 200, 200, 0, 200, 160, 160, 200, 223, 200, 200, 200, 200, 350, 350, 900, 400, 400, 227, 400, 208, 400, 400, 400, 400, 400, 208, 550, 550, 700, 708, 700, 700, 700, 700, 700, 500, 210, 200, 207, 350, 350, 350, 350, 700, 700, 200, 200, 0, 350, 0, 200, 0, 0, 0, 0, 0, 0, 300, 203, 200, 206, 500, 700, 1e3, 204, 350, 350, 160, 350, 500, 500, 500, 350, 200, 0, 0, 0, 0, 0, 200, 0, 350, 800, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 0, 0, 0, 0, 0, 200, 0, 0, 200, 200, 200, 200, 200, 200, 200, 222, 223, 200, 350, 350, 350, 350, 350, 400, 200, 200, 226, 207, 200, 200, 200, 229, 400, 400, 207, 800, 700, 1e3, 707, 1e3, 1e3, 1e3, 1e3, 800, 500, 209, 208, 350, 350, 350, 350, 350, 1100, 1100, 350, 350, 700, 0, 0, 0, 0, 0, 0, 200, 300, 500, 204, 205, 500, 500, 1e3, 203, 350, 350, 350, 350, 500, 500, 500, 500, 350, 200, 0, 0, 200, 0, 0, 200, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 700, 500, 700, 0, 0, 0, 900, 0, 350, 350, 0, 200, 200, 350, 200, 0, 200, 200, 221, 200, 200, 350, 500, 500, 500, 500, 900, 500, 200, 200, 206, 200, 200, 500, 200, 228, 200, 400, 206, 550, 700, 1e3, 706, 1e3, 1100, 1100, 1100, 800, 500, 500, 500, 350, 700, 350, 350, 1100, 1400, 1100, 950, 700, 0, 0, 200, 0, 0, 0, 0, 0, 0, 350, 500, 500, 500, 500, 350, 202, 350, 350, 350, 350, 200, 200, 200, 200, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 201, 350, 0, 0, 700, 350, 0, 0, 0, 350, 350, 350, 350, 0, 0, 200, 350, 200, 0, 0, 0, 0, 0, 200, 350, 500, 600, 700, 800, 1e3, 500, 200, 200, 205, 200, 226, 200, 200, 227, 200, 200, 200, 205, 550, 700, 1e3, 705, 1e3, 1200, 1200, 1100, 1100, 800, 800, 700, 700, 1100, 1100, 1100, 1400, 1100, 950, 950, 1100, 0, 350, 200, 0, 0, 0, 200, 350, 1e3, 700, 1e3, 700, 350, 201, 200, 160, 160, 200, 200, 0, 0, 0, 200, 200, 350, 350, 350, 350, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, 700, 600, 350, 350, 0, 0, 350, 0, 0, 0, 0, 700, 1100, 700, 350, 0, 0, 200, 500, 200, 200, 0, 0, 0, 0, 200, 200, 350, 600, 800, 900, 500, 200, 200, 204, 224, 225, 200, 200, 226, 200, 450, 200, 204, 550, 700, 1e3, 704, 1100, 1100, 1100, 1100, 1250, 1100, 1100, 1e3, 1e3, 1100, 1400, 950, 1100, 950, 950, 950, 1100, 0, 350, 350, 0, 0, 0, 0, 0, 350, 700, 1e3, 1e3, 1e3, 350, 0, 200, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 350, 0, 350, 350, 600, 0, 0, 0, 0, 0, 0, 0, 500, 650, 650, 350, 0, 0, 0, 0, 0, 0, 0, 0, 700, 1e3, 700, 350, 350, 350, 500, 350, 200, 0, 0, 0, 200, 0, 0, 200, 350, 500, 900, 900, 500, 200, 203, 200, 200, 200, 224, 225, 200, 450, 200, 203, 700, 700, 950, 703, 950, 950, 950, 950, 950, 950, 950, 950, 950, 950, 1100, 700, 950, 950, 950, 700, 1100, 1100, 350, 700, 700, 700, 0, 0, 0, 700, 350, 200, 200, 0, 0, 0, 0, 200, 200, 0, 350, 350, 350, 0, 350, 350, 0, 350, 0, 350, 0, 0, 350, 800, 0, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 1100, 700, 700, 500, 1e3, 200, 0, 0, 0, 200, 200, 0, 0, 200, 350, 600, 350, 1e3, 500, 200, 202, 200, 200, 223, 200, 200, 450, 450, 200, 202, 700, 700, 700, 702, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 1100, 700, 700, 700, 700, 700, 700, 1100, 350, 350, 1100, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 600, 600, 350, 0, 350, 0, 350, 0, 0, 0, 0, 350, 350, 0, 0, 0, 700, 350, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 1100, 350, 350, 350, 0, 0, 0, 0, 350, 0, 0, 0, 0, 350, 350, 350, 350, 1100, 500, 350, 201, 350, 222, 350, 350, 350, 350, 350, 201, 350, 350, 350, 501, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 0, 1100, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 600, 800, 600, 350, 0, 0, 350, 0, 0, 0, 0, 350, 350, 0, 0, 0, 900, 700, 350, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, 0, 350, 350, 0, 350, 221, 350, 350, 0, 0, 0, 0, 350, 350, 350, 350, 350, 350, 350, 350, 350, 0, 0, 0, 0, 0, 0, 350, 350, 350, 350, 0, 0, 0, 0, 0, 0, 0, 0];
  TerrainBuilder.setElevation(elevationArray);
  TerrainBuilder.generateCliffsFromElevation();
}
function paintEarthHugeRivers() {
  TerrainBuilder.setRiverInfo(19, 41, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(19, 42, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 43, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(19, 44, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 45, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 46, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 47, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(18, 48, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(17, 49, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(17, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(17, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(16, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 44, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 44, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(13, 45, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(17, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(16, 47, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 47, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 48, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 49, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 51, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(13, 51, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 51, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 50, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 50, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(20, 42, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(19, 46, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(20, 46, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(21, 46, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(21, 47, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(22, 47, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(23, 47, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(24, 46, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(16, 38, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 39, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(15, 40, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 40, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(13, 41, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(14, 42, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 39, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 40, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(10, 41, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 42, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(11, 43, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(12, 42, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(5, 49, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(6, 49, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(7, 50, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(8, 50, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(9, 50, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(9, 51, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(3, 57, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(4, 57, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(5, 58, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(5, 59, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(6, 59, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(7, 58, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(7, 57, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(8, 56, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(8, 55, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(9, 55, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(9, 61, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(10, 60, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(10, 59, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(11, 58, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(11, 57, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(12, 57, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(13, 57, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 50, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(27, 50, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(26, 50, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(25, 49, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(25, 48, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 25, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(31, 24, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(30, 24, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(29, 23, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 23, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(27, 22, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(26, 23, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(25, 23, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(31, 26, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 26, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 26, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(28, 26, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 25, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 25, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(28, 25, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(27, 25, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(26, 25, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(32, 24, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 24, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(28, 24, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(27, 24, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(26, 24, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 23, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 23, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(27, 23, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 22, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 22, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(26, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(28, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(27, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(26, 21, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(25, 21, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(32, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(28, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 19, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 19, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 19, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(30, 18, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(33, 24, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(32, 23, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(33, 22, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(32, 21, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(33, 20, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(32, 19, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(32, 18, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(32, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(35, 19, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(34, 19, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(34, 18, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(33, 17, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(34, 16, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 8, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 9, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 10, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(28, 11, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(29, 11, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(30, 12, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(30, 13, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(31, 14, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(29, 10, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(46, 23, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(47, 24, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(46, 25, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(46, 26, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(45, 27, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(44, 27, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(43, 27, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(43, 26, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(42, 25, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(42, 24, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(41, 24, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(41, 23, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(42, 23, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(42, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(48, 16, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(48, 17, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(49, 18, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(50, 18, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(50, 17, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(51, 16, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(51, 15, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(49, 16, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(49, 15, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(49, 19, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(50, 20, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(51, 20, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(51, 17, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(48, 7, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(49, 6, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(50, 6, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(51, 6, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(52, 6, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(57, 9, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(57, 10, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(56, 10, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(55, 10, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(54, 9, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(53, 9, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(53, 10, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(57, 36, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 35, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 34, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 33, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 32, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 31, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 30, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 30, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(55, 29, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 28, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 28, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 27, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(58, 26, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 25, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(57, 24, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 23, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 22, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 21, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(55, 21, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(54, 21, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 30, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 29, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 28, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 27, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(59, 27, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(60, 26, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(61, 26, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(40, 41, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(41, 41, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(43, 48, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(44, 48, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(45, 48, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(45, 47, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(46, 51, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(47, 50, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(48, 50, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(48, 49, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(47, 52, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(47, 51, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(51, 44, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(50, 45, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(57, 48, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(56, 49, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(55, 49, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(54, 49, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(53, 49, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(52, 49, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(51, 49, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(51, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(50, 50, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 51, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(58, 52, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(65, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(64, 51, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(64, 52, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(64, 53, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(65, 54, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(64, 55, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(64, 56, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(63, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(62, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(61, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(66, 54, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(66, 55, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(66, 50, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(66, 51, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(67, 51, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(68, 52, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(68, 53, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(68, 54, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(43, 54, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(43, 55, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(45, 54, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(44, 54, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(38, 60, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(37, 61, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(37, 62, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(38, 62, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(64, 40, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(63, 41, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(63, 42, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(62, 43, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(63, 44, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(69, 36, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(69, 37, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(69, 38, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(69, 39, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(70, 40, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(71, 40, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(72, 40, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(77, 36, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(76, 37, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(75, 37, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(74, 37, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(73, 37, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(73, 38, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(73, 39, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(78, 36, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(74, 30, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(73, 31, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(73, 32, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(84, 31, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(84, 32, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 33, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 34, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 35, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(83, 36, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(82, 37, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(82, 38, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(81, 39, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(81, 40, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(84, 33, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(84, 34, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(83, 31, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(83, 32, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(82, 33, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(92, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(91, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(91, 44, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(90, 44, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(89, 43, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(89, 42, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(88, 42, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(87, 43, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(86, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 43, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 42, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(84, 41, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(84, 40, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 40, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(82, 41, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(82, 42, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(81, 43, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(81, 44, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(80, 45, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(91, 50, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(90, 49, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(90, 48, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(89, 48, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(88, 47, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(88, 46, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(87, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(86, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 47, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 48, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 49, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(85, 50, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(84, 51, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 51, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 50, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(82, 49, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(85, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(84, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(83, 46, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(98, 56, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(97, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(96, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(95, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(94, 56, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(93, 55, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(92, 55, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(92, 56, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(91, 57, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(91, 58, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(90, 58, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(89, 57, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(89, 56, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(88, 55, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(87, 55, DirectionTypes.DIRECTION_EAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(71, 64, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(70, 63, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(70, 62, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(70, 61, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(71, 60, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(71, 59, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(72, 58, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(72, 57, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(71, 58, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(70, 57, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(71, 61, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(72, 61, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(73, 60, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(73, 64, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(73, 63, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(74, 62, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(75, 62, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(75, 61, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(76, 60, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(76, 59, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(76, 58, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(83, 64, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 63, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 62, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 61, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(84, 60, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 59, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 58, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(82, 57, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(83, 56, DirectionTypes.DIRECTION_NORTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(99, 43, DirectionTypes.DIRECTION_SOUTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(100, 46, DirectionTypes.DIRECTION_NORTHEAST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(101, 46, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(95, 52, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(96, 52, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(97, 52, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(95, 49, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(96, 49, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(97, 6, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(97, 7, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(98, 7, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(99, 7, DirectionTypes.DIRECTION_WEST, RiverTypes.RIVER_NAVIGABLE);
  TerrainBuilder.setRiverInfo(99, 8, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
  TerrainBuilder.setRiverInfo(99, 9, DirectionTypes.DIRECTION_SOUTHWEST, RiverTypes.RIVER_MINOR);
}
function nameRivers() {
  TerrainBuilder.setCustomRiverName(19, 41, "LOC_RIVER_MISSISSIPPI_NAME");
  TerrainBuilder.setCustomRiverName(16, 38, "LOC_RIVER_RIO_GRANDE_NAME");
  TerrainBuilder.setCustomRiverName(10, 39, "LOC_RIVER_COLORADO_NAME");
  TerrainBuilder.setCustomRiverName(3, 57, "LOC_RIVER_YUKON_NAME");
  TerrainBuilder.setCustomRiverName(9, 61, "LOC_RIVER_MACKENZIE_NAME");
  TerrainBuilder.setCustomRiverName(28, 50, "LOC_RIVER_SAINT_LAWRENCE_NAME");
  TerrainBuilder.setCustomRiverName(31, 25, "LOC_RIVER_AMAZON_NAME");
  TerrainBuilder.setCustomRiverName(33, 24, "LOC_RIVER_TOCANTINS_NAME");
  TerrainBuilder.setCustomRiverName(35, 19, "LOC_RIVER_SAO_FRANCISCO_NAME");
  TerrainBuilder.setCustomRiverName(29, 8, "LOC_RIVER_PARANA_NAME");
  TerrainBuilder.setCustomRiverName(46, 23, "LOC_RIVER_NIGER_NAME");
  TerrainBuilder.setCustomRiverName(48, 16, "LOC_RIVER_CONGO_NAME");
  TerrainBuilder.setCustomRiverName(48, 7, "LOC_RIVER_ORANGE_NAME");
  TerrainBuilder.setCustomRiverName(57, 9, "LOC_RIVER_ZAMBEZI_NAME");
  TerrainBuilder.setCustomRiverName(57, 36, "LOC_RIVER_NILE_NAME");
  TerrainBuilder.setCustomRiverName(40, 41, "LOC_RIVER_TAGUS_NAME");
  TerrainBuilder.setCustomRiverName(43, 48, "LOC_RIVER_LOIRE_NAME");
  TerrainBuilder.setCustomRiverName(46, 51, "LOC_RIVER_RHINE_NAME");
  TerrainBuilder.setCustomRiverName(57, 48, "LOC_RIVER_DANUBE_NAME");
  TerrainBuilder.setCustomRiverName(58, 50, "LOC_RIVER_DNIEPER_NAME");
  TerrainBuilder.setCustomRiverName(65, 50, "LOC_RIVER_VOLGA_NAME");
  TerrainBuilder.setCustomRiverName(66, 50, "LOC_RIVER_URAL_NAME");
  TerrainBuilder.setCustomRiverName(43, 54, "LOC_RIVER_SEVERN_NAME");
  TerrainBuilder.setCustomRiverName(45, 54, "LOC_RIVER_THAMES_NAME");
  TerrainBuilder.setCustomRiverName(38, 60, "LOC_RIVER_HVITA_NAME");
  TerrainBuilder.setCustomRiverName(64, 40, "LOC_RIVER_TIGRIS_NAME");
  TerrainBuilder.setCustomRiverName(69, 36, "LOC_RIVER_INDUS_NAME");
  TerrainBuilder.setCustomRiverName(77, 36, "LOC_RIVER_GANGES_NAME");
  TerrainBuilder.setCustomRiverName(74, 30, "LOC_RIVER_GODAVARI_NAME");
  TerrainBuilder.setCustomRiverName(84, 31, "LOC_RIVER_MEKONG_NAME");
  TerrainBuilder.setCustomRiverName(92, 43, "LOC_RIVER_YANGTZE_NAME");
  TerrainBuilder.setCustomRiverName(91, 50, "LOC_RIVER_YELLOW_NAME");
  TerrainBuilder.setCustomRiverName(98, 56, "LOC_RIVER_AMUR_NAME");
  TerrainBuilder.setCustomRiverName(71, 64, "LOC_RIVER_OB_NAME");
  TerrainBuilder.setCustomRiverName(73, 64, "LOC_RIVER_YENISEI_NAME");
  TerrainBuilder.setCustomRiverName(83, 64, "LOC_RIVER_LENA_NAME");
  TerrainBuilder.setCustomRiverName(97, 6, "LOC_RIVER_MURRAY_NAME");
  TerrainBuilder.setCustomRiverName(5, 49, "LOC_RIVER_COLUMBIA_NAME");
  TerrainBuilder.setCustomRiverName(100, 46, "LOC_RIVER_SHINANO_NAME");
  TerrainBuilder.setCustomRiverName(51, 44, "LOC_RIVER_TIBER_NAME");
  TerrainBuilder.setCustomRiverName(95, 52, "LOC_RIVER_AMNOK_NAME");
  TerrainBuilder.setCustomRiverName(95, 49, "LOC_RIVER_HAN_NAME");
  TerrainBuilder.setCustomRiverName(99, 43, "LOC_RIVER_NACHI_NAME");
}
function nameVolcanoes() {
  TerrainBuilder.setCustomVolcanoName(1, 8, "LOC_VOLCANO_TAUPO_VOLCANO_NAME");
  TerrainBuilder.setCustomVolcanoName(27, 9, "LOC_VOLCANO_RUCAPILLAN_NAME");
  TerrainBuilder.setCustomVolcanoName(24, 23, "LOC_VOLCANO_CHIMBORAZO_NAME");
  TerrainBuilder.setCustomVolcanoName(81, 24, "LOC_VOLCANO_KRAKATOA_NAME");
  TerrainBuilder.setCustomVolcanoName(97, 24, "LOC_VOLCANO_MOUNT_GILUWE_NAME");
  TerrainBuilder.setCustomVolcanoName(79, 27, "LOC_VOLCANO_SINABUNG_NAME");
  TerrainBuilder.setCustomVolcanoName(59, 29, "LOC_VOLCANO_ALU_DALAFILLA_NAME");
  TerrainBuilder.setCustomVolcanoName(97, 29, "LOC_VOLCANO_MAYON_NAME");
  TerrainBuilder.setCustomVolcanoName(19, 30, "LOC_VOLCANO_VOLCAN_ATITLAN_NAME");
  TerrainBuilder.setCustomVolcanoName(2, 33, "LOC_VOLCANO_KILAUEA_NAME");
  TerrainBuilder.setCustomVolcanoName(94, 33, "LOC_VOLCANO_MOUNT_PINATUBO_NAME");
  TerrainBuilder.setCustomVolcanoName(15, 34, "LOC_VOLCANO_POPOCATEPETL_NAME");
  TerrainBuilder.setCustomVolcanoName(1, 36, "LOC_VOLCANO_HALEAKAL_NAME");
  TerrainBuilder.setCustomVolcanoName(49, 41, "LOC_VOLCANO_MOUNT_ETNA_NAME");
  TerrainBuilder.setCustomVolcanoName(52, 42, "LOC_VOLCANO_MOUNT_VESUVIUS_NAME");
  TerrainBuilder.setCustomVolcanoName(46, 47, "LOC_VOLCANO_PUY_DE_DOME_NAME");
  TerrainBuilder.setCustomVolcanoName(7, 51, "LOC_VOLCANO_MOUNT_RAINIER_NAME");
  TerrainBuilder.setCustomVolcanoName(103, 59, "LOC_VOLCANO_AVACHINSKY_NAME");
  TerrainBuilder.setCustomVolcanoName(104, 60, "LOC_VOLCANO_BEZYMIANNY_NAME");
  TerrainBuilder.setCustomVolcanoName(38, 61, "LOC_VOLCANO_ASKJA_NAME");
  TerrainBuilder.setCustomVolcanoName(103, 61, "LOC_VOLCANO_SHIVELUCH_NAME");
}
function assignStartPositionsEarthHuge() {
  const aliveMajorIds = Players.getAliveMajorIds();
  const startPositions = new Array(aliveMajorIds.length);
  let plotIndex = -1;
  for (const majorId of aliveMajorIds) {
    const player = Players.get(majorId);
    if (player != null) {
      switch (player.civilizationName) {
        //Base Game Civs
        //Antiquity
        case "LOC_CIVILIZATION_AKSUM_NAME":
          plotIndex = GameplayMap.getIndexFromXY(61, 28);
          break;
        case "LOC_CIVILIZATION_EGYPT_NAME":
          plotIndex = GameplayMap.getIndexFromXY(57, 33);
          break;
        case "LOC_CIVILIZATION_GREECE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(56, 43);
          break;
        case "LOC_CIVILIZATION_HAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(85, 46);
          break;
        case "LOC_CIVILIZATION_KHMER_NAME":
          plotIndex = GameplayMap.getIndexFromXY(83, 32);
          break;
        case "LOC_CIVILIZATION_MAURYA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(75, 36);
          break;
        case "LOC_CIVILIZATION_MAYA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(20, 32);
          break;
        case "LOC_CIVILIZATION_MISSISSIPPIAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(19, 45);
          break;
        case "LOC_CIVILIZATION_PERSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(67, 41);
          break;
        case "LOC_CIVILIZATION_ROME_NAME":
          plotIndex = GameplayMap.getIndexFromXY(51, 44);
          break;
        //Exploration
        case "LOC_CIVILIZATION_ABBASID_NAME":
          plotIndex = GameplayMap.getIndexFromXY(63, 43);
          break;
        case "LOC_CIVILIZATION_CHOLA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(74, 29);
          break;
        case "LOC_CIVILIZATION_HAWAII_NAME":
          plotIndex = GameplayMap.getIndexFromXY(0, 36);
          break;
        case "LOC_CIVILIZATION_INCA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(27, 20);
          break;
        case "LOC_CIVILIZATION_MAJAPAHIT_NAME":
          plotIndex = GameplayMap.getIndexFromXY(84, 23);
          break;
        case "LOC_CIVILIZATION_MING_NAME":
          plotIndex = GameplayMap.getIndexFromXY(90, 43);
          break;
        case "LOC_CIVILIZATION_MONGOLIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(81, 52);
          break;
        case "LOC_CIVILIZATION_NORMAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(44, 51);
          break;
        case "LOC_CIVILIZATION_SONGHAI_NAME":
          plotIndex = GameplayMap.getIndexFromXY(46, 27);
          break;
        case "LOC_CIVILIZATION_SPAIN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(43, 42);
          break;
        //Modern
        case "LOC_CIVILIZATION_AMERICA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(24, 44);
          break;
        case "LOC_CIVILIZATION_BUGANDA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(56, 21);
          break;
        case "LOC_CIVILIZATION_FRENCH_EMPIRE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(44, 49);
          break;
        case "LOC_CIVILIZATION_MEIJI_NAME":
          plotIndex = GameplayMap.getIndexFromXY(102, 46);
          break;
        case "LOC_CIVILIZATION_MEXICO_NAME":
          plotIndex = GameplayMap.getIndexFromXY(16, 34);
          break;
        case "LOC_CIVILIZATION_MUGHAL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(72, 37);
          break;
        case "LOC_CIVILIZATION_PRUSSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(50, 51);
          break;
        case "LOC_CIVILIZATION_QING_NAME":
          plotIndex = GameplayMap.getIndexFromXY(90, 52);
          break;
        case "LOC_CIVILIZATION_RUSSIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(56, 56);
          break;
        case "LOC_CIVILIZATION_SIAM_NAME":
          plotIndex = GameplayMap.getIndexFromXY(82, 32);
          break;
        //CC0
        case "LOC_CIVILIZATION_SHAWNEE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(20, 45);
          break;
        //CC1
        case "LOC_CIVILIZATION_GREAT_BRITAIN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(45, 54);
          break;
        case "LOC_CIVILIZATION_CARTHAGE_NAME":
          plotIndex = GameplayMap.getIndexFromXY(48, 38);
          break;
        case "LOC_CIVILIZATION_BULGARIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(56, 47);
          break;
        case "LOC_CIVILIZATION_NEPAL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(76, 38);
          break;
        //CC2
        case "LOC_CIVILIZATION_DAI_VIET_NAME":
          plotIndex = GameplayMap.getIndexFromXY(84, 35);
          break;
        case "LOC_CIVILIZATION_SILLA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(97, 48);
          break;
        case "LOC_CIVILIZATION_ASSYRIA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(63, 44);
          break;
        case "LOC_CIVILIZATION_QAJAR_NAME":
          plotIndex = GameplayMap.getIndexFromXY(66, 44);
          break;
        //CC3
        case "LOC_CIVILIZATION_PIRATE_REPUBLIC_NAME":
          plotIndex = GameplayMap.getIndexFromXY(25, 37);
          break;
        case "LOC_CIVILIZATION_TONGA_NAME":
          plotIndex = GameplayMap.getIndexFromXY(4, 17);
          break;
        case "LOC_CIVILIZATION_OTTOMANS_NAME":
          plotIndex = GameplayMap.getIndexFromXY(58, 46);
          break;
        case "LOC_CIVILIZATION_ICELAND_NAME":
          plotIndex = GameplayMap.getIndexFromXY(37, 60);
          break;
        //CC4
        case "LOC_CIVILIZATION_GORYEO_NAME":
          plotIndex = GameplayMap.getIndexFromXY(95, 49);
          break;
        case "LOC_CIVILIZATION_JOSEON_NAME":
          plotIndex = GameplayMap.getIndexFromXY(95, 49);
          break;
        case "LOC_CIVILIZATION_HEIAN_NAME":
          plotIndex = GameplayMap.getIndexFromXY(99, 45);
          break;
        case "LOC_CIVILIZATION_SENGOKU_NAME":
          plotIndex = GameplayMap.getIndexFromXY(99, 45);
          break;
        //CC5
        case "LOC_CIVILIZATION_BABYLON_NAME":
          plotIndex = GameplayMap.getIndexFromXY(64, 42);
          break;
        case "LOC_CIVILIZATION_GAUL_NAME":
          plotIndex = GameplayMap.getIndexFromXY(45, 48);
          break;
        case "LOC_CIVILIZATION_ENGLAND_NAME":
          plotIndex = GameplayMap.getIndexFromXY(45, 54);
          break;
        default:
          plotIndex = GameplayMap.getIndexFromXY(91, 11);
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
async function paintEarthHugeNaturalWonders() {
  const iNumNaturalWonders = 7;
  let aPossibleWonders = [];
  let iPlacedWonders = 0;
  for (const nwDef of GameInfo.Feature_NaturalWonders) {
    aPossibleWonders.push(nwDef.$hash);
  }
  if (aPossibleWonders.length > 0) {
    aPossibleWonders = shuffle(aPossibleWonders);
    for (const eFeature of aPossibleWonders) {
      if (iPlacedWonders < iNumNaturalWonders) {
        const nwDef = GameInfo.Feature_NaturalWonders.lookup(eFeature);
        let placementX = 0;
        let placementY = 0;
        if (nwDef != null) {
          let iElevation = GameplayMap.getElevation(0, 0);
          let featureParam = { Feature: eFeature, Direction: nwDef.Direction, Elevation: iElevation };
          switch (nwDef.FeatureType) {
            //Natural Wonders
            case "FEATURE_VALLEY_OF_FLOWERS":
              placementX = 73;
              placementY = 42;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_BARRIER_REEF":
              placementX = 100;
              placementY = 17;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_REDWOOD_FOREST":
              placementX = 6;
              placementY = 45;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_GRAND_CANYON":
              placementX = 10;
              placementY = 42;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              } else {
                console.log("Did not place: " + nwDef.FeatureType);
              }
              break;
            case "FEATURE_GULLFOSS":
              placementX = 37;
              placementY = 61;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              } else {
                console.log("Did not place: " + nwDef.FeatureType);
              }
              break;
            case "FEATURE_HOERIKWAGGO":
              placementX = 49;
              placementY = 4;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_IGUAZU_FALLS":
              placementX = 30;
              placementY = 13;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_KILIMANJARO":
              placementX = 58;
              placementY = 19;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_ZHANGJIAJIE":
              placementX = 86;
              placementY = 41;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_THERA":
              placementX = 57;
              placementY = 41;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_TORRES_DEL_PAINE":
              placementX = 25;
              placementY = 5;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_ULURU":
              placementX = 93;
              placementY = 14;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_MOUNT_EVEREST":
              placementX = 77;
              placementY = 39;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_BERMUDA_TRIANGLE":
              placementX = 26;
              placementY = 39;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_MACHAPUCHARE":
              placementX = 75;
              placementY = 40;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_MOUNT_FUJI":
              placementX = 101;
              placementY = 44;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              } else {
                console.log("Did not place: " + nwDef.FeatureType);
              }
              break;
            case "FEATURE_VIHREN":
              placementX = 55;
              placementY = 48;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_VINICUNCA":
              placementX = 25;
              placementY = 20;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_GREAT_BLUE_HOLE":
              placementX = 23;
              placementY = 33;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_MAPU_A_VAEA_BLOWHOLES":
              placementX = 3;
              placementY = 16;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            case "FEATURE_NACHI_FALLS":
              placementX = 98;
              placementY = 43;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              } else {
                console.log("Did not place: " + nwDef.FeatureType);
              }
              break;
            case "FEATURE_SEONGSAN_ILCHULBONG":
              placementX = 95;
              placementY = 45;
              iElevation = GameplayMap.getElevation(placementX, placementY);
              featureParam = {
                Feature: eFeature,
                Direction: DirectionTypes.NO_DIRECTION,
                Elevation: iElevation
              };
              if (TerrainBuilder.canHaveFeature(placementX, placementY, nwDef.FeatureType)) {
                TerrainBuilder.setFeatureType(placementX, placementY, featureParam);
                console.log("Placed: " + nwDef.FeatureType);
                iPlacedWonders++;
              }
              break;
            default:
              console.log("Did not place: " + nwDef.FeatureType);
              break;
          }
        }
      }
    }
  }
}
function paintEarthHugeResourcesAntiquity() {
  const aResourceList = [];
  for (const resDef of GameInfo.Resources) {
    aResourceList.push(resDef.ResourceType);
    console.log("Resource: " + resDef.ResourceType);
  }
  ResourceBuilder.setResourceType(89, 13, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(92, 16, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(94, 11, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(75, 46, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(79, 50, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(46, 28, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(63, 26, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(42, 33, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(49, 32, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(56, 32, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(71, 52, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(62, 34, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(64, 33, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(69, 47, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(70, 39, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(53, 35, "RESOURCE_CAMELS");
  ResourceBuilder.setResourceType(54, 60, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(27, 55, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(14, 54, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(67, 54, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(79, 42, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(48, 52, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(26, 52, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(19, 48, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(18, 52, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(4, 60, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(56, 24, "RESOURCE_CLAY");
  ResourceBuilder.setResourceType(20, 42, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(21, 44, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(90, 45, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(72, 39, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(69, 40, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(19, 46, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(21, 47, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(33, 20, "RESOURCE_COTTON");
  ResourceBuilder.setResourceType(64, 11, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(60, 12, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(59, 9, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(65, 29, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(61, 29, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(70, 22, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(70, 25, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(71, 30, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(69, 35, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(79, 25, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(86, 17, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(84, 25, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(91, 31, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(96, 32, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(98, 29, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(94, 25, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(92, 21, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(99, 23, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(102, 18, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(103, 22, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(0, 23, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(5, 17, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(9, 14, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(12, 11, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(2, 36, "RESOURCE_COWRIE");
  ResourceBuilder.setResourceType(25, 44, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(2, 52, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(5, 55, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(4, 49, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(7, 41, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(18, 40, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(24, 41, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(29, 49, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(104, 57, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(101, 56, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(97, 50, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(103, 55, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(1, 57, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(22, 35, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(18, 34, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(20, 29, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(19, 25, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(24, 18, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(25, 8, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(28, 61, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(31, 62, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(97, 44, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(93, 45, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(83, 22, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(88, 25, "RESOURCE_CRABS");
  ResourceBuilder.setResourceType(64, 28, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(60, 29, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(62, 32, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(67, 36, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(62, 41, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(57, 34, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(51, 30, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(43, 30, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(45, 36, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(48, 29, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(39, 31, "RESOURCE_DATES");
  ResourceBuilder.setResourceType(12, 9, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(60, 41, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(59, 38, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(55, 39, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(56, 45, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(53, 40, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(52, 36, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(49, 38, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(46, 42, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(50, 39, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(41, 37, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(46, 18, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(47, 12, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(54, 3, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(62, 21, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(66, 33, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(62, 9, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(78, 32, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(84, 29, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(88, 30, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(85, 33, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(93, 33, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(93, 27, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(98, 26, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(105, 38, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(8, 13, "RESOURCE_DYES");
  ResourceBuilder.setResourceType(25, 1, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(29, 2, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(31, 8, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(33, 12, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(36, 18, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(37, 20, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(33, 25, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(30, 29, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(27, 34, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(25, 33, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(17, 29, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(13, 34, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(3, 58, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(5, 64, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(9, 63, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(15, 62, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(21, 59, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(24, 58, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(27, 59, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(31, 56, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(32, 52, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(34, 54, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(27, 46, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(30, 36, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(28, 41, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(60, 18, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(64, 25, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(63, 39, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(69, 24, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(75, 27, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(76, 33, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(83, 24, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(90, 28, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(92, 35, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(92, 49, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(98, 53, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(100, 51, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(104, 48, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(100, 42, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(99, 47, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(96, 42, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(95, 20, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(88, 18, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(91, 8, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(97, 3, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(107, 6, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(1, 9, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(7, 13, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(12, 12, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(14, 12, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(37, 30, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(39, 33, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(38, 36, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(39, 44, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(41, 50, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(41, 60, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(46, 60, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(47, 57, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(51, 56, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(52, 60, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(58, 60, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(61, 62, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(63, 65, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(77, 65, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(91, 65, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(103, 64, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(102, 59, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(44, 39, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(49, 43, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(53, 43, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(39, 25, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(43, 21, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(48, 4, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(53, 2, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(56, 5, "RESOURCE_FISH");
  ResourceBuilder.setResourceType(40, 57, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(45, 50, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(41, 42, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(49, 53, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(53, 48, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(54, 55, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(56, 51, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(59, 53, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(62, 55, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(63, 49, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(75, 34, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(30, 8, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(16, 45, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(15, 51, "RESOURCE_FLAX");
  ResourceBuilder.setResourceType(96, 53, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(87, 49, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(78, 41, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(88, 10, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(57, 18, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(53, 6, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(47, 23, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(42, 26, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(25, 50, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(8, 45, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(6, 52, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(8, 53, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(26, 26, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(98, 23, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(13, 36, "RESOURCE_GOLD");
  ResourceBuilder.setResourceType(26, 2, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(13, 50, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(13, 43, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(44, 43, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(42, 42, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(74, 45, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(64, 41, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(62, 35, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(62, 24, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(58, 31, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(40, 29, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(42, 34, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(49, 8, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(100, 14, "RESOURCE_GYPSUM");
  ResourceBuilder.setResourceType(31, 22, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(28, 27, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(31, 10, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(23, 30, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(17, 34, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(24, 53, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(19, 58, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(12, 56, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(52, 17, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(49, 19, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(42, 23, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(50, 61, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(58, 58, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(64, 61, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(73, 58, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(77, 60, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(83, 60, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(91, 56, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(97, 57, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(99, 25, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(32, 54, "RESOURCE_HARDWOOD");
  ResourceBuilder.setResourceType(69, 53, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(55, 59, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(61, 60, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(64, 62, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(72, 62, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(81, 61, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(86, 60, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(94, 57, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(90, 54, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(81, 54, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(74, 53, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(54, 53, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(53, 27, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(49, 27, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(45, 25, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(49, 12, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(16, 49, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(20, 59, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(22, 62, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(12, 63, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(18, 61, "RESOURCE_HIDES");
  ResourceBuilder.setResourceType(98, 8, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(98, 13, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(96, 16, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(55, 9, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(73, 29, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(88, 52, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(83, 54, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(80, 53, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(77, 52, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(63, 50, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(59, 50, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(41, 40, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(17, 44, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(9, 42, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(17, 47, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(13, 52, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(28, 4, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(45, 55, "RESOURCE_HORSES");
  ResourceBuilder.setResourceType(55, 19, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(56, 27, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(65, 43, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(72, 33, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(73, 35, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(91, 52, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(82, 40, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(83, 47, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(14, 43, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(92, 51, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(61, 38, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(85, 38, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(60, 50, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(80, 34, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(101, 49, "RESOURCE_INCENSE");
  ResourceBuilder.setResourceType(26, 49, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(23, 46, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(14, 45, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(28, 17, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(46, 37, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(41, 44, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(43, 49, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(52, 47, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(47, 60, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(72, 28, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(86, 15, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(96, 27, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(73, 52, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(84, 47, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(85, 39, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(66, 57, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(81, 45, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(30, 56, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(9, 41, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(74, 61, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(78, 62, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(81, 62, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(85, 59, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(88, 62, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(93, 59, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(98, 18, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(51, 63, "RESOURCE_IRON");
  ResourceBuilder.setResourceType(52, 10, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(51, 7, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(55, 12, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(58, 16, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(48, 21, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(52, 20, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(48, 26, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(42, 28, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(53, 30, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(81, 26, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(88, 29, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(83, 27, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(89, 35, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(76, 35, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(73, 27, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(71, 36, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(68, 42, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(62, 39, "RESOURCE_IVORY");
  ResourceBuilder.setResourceType(98, 9, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(102, 47, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(90, 47, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(79, 33, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(76, 42, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(69, 63, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(21, 31, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(11, 54, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(4, 62, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(91, 59, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(96, 62, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(85, 57, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(81, 57, "RESOURCE_JADE");
  ResourceBuilder.setResourceType(87, 36, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(72, 55, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(49, 54, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(32, 16, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(30, 15, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(35, 22, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(39, 27, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(49, 25, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(52, 13, "RESOURCE_KAOLIN");
  ResourceBuilder.setResourceType(71, 54, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(76, 47, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(90, 62, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(62, 53, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(69, 57, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(64, 44, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(67, 38, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(59, 35, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(14, 39, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(23, 62, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(63, 25, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(47, 34, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(50, 33, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(22, 43, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(40, 33, "RESOURCE_LIMESTONE");
  ResourceBuilder.setResourceType(29, 13, "RESOURCE_LLAMAS");
  ResourceBuilder.setResourceType(28, 21, "RESOURCE_LLAMAS");
  ResourceBuilder.setResourceType(26, 25, "RESOURCE_LLAMAS");
  ResourceBuilder.setResourceType(23, 23, "RESOURCE_LLAMAS");
  ResourceBuilder.setResourceType(30, 19, "RESOURCE_LLAMAS");
  ResourceBuilder.setResourceType(75, 32, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(73, 36, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(71, 33, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(71, 38, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(59, 16, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(47, 20, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(44, 22, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(36, 20, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(29, 17, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(30, 25, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(25, 36, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(21, 33, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(92, 26, "RESOURCE_MANGOS");
  ResourceBuilder.setResourceType(52, 43, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(78, 37, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(40, 45, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(60, 46, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(56, 39, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(58, 44, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(28, 51, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(47, 48, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(53, 46, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(20, 49, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(40, 55, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(58, 55, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(65, 56, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(69, 55, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(71, 57, "RESOURCE_MARBLE");
  ResourceBuilder.setResourceType(73, 25, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(90, 34, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(46, 54, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(41, 53, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(65, 38, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(67, 35, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(71, 23, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(76, 30, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(80, 28, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(88, 32, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(93, 40, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(99, 42, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(55, 37, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(59, 33, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(10, 35, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(8, 39, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(14, 10, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(3, 20, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(84, 14, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(85, 8, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(94, 5, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(89, 6, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(1, 16, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(105, 21, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(4, 20, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(38, 38, "RESOURCE_PEARLS");
  ResourceBuilder.setResourceType(89, 39, "RESOURCE_RICE");
  ResourceBuilder.setResourceType(89, 36, "RESOURCE_RICE");
  ResourceBuilder.setResourceType(88, 44, "RESOURCE_RICE");
  ResourceBuilder.setResourceType(74, 33, "RESOURCE_RICE");
  ResourceBuilder.setResourceType(68, 48, "RESOURCE_RICE");
  ResourceBuilder.setResourceType(87, 15, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(82, 34, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(79, 35, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(70, 41, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(53, 17, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(56, 14, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(58, 10, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(55, 7, "RESOURCE_RUBIES");
  ResourceBuilder.setResourceType(23, 51, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(69, 44, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(57, 49, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(26, 13, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(57, 56, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(60, 25, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(59, 21, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(24, 20, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(26, 29, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(13, 54, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(41, 31, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(54, 31, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(64, 35, "RESOURCE_SALT");
  ResourceBuilder.setResourceType(67, 43, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(16, 38, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(100, 46, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(95, 52, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(86, 46, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(88, 48, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(82, 37, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(73, 31, "RESOURCE_SILK");
  ResourceBuilder.setResourceType(26, 18, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(100, 8, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(98, 5, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(61, 22, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(44, 36, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(45, 31, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(72, 50, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(69, 51, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(63, 31, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(52, 51, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(25, 47, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(21, 52, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(9, 51, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(4, 55, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(8, 61, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(12, 38, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(13, 46, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(25, 4, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(39, 62, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(17, 64, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(52, 64, "RESOURCE_SILVER");
  ResourceBuilder.setResourceType(69, 45, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(70, 48, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(61, 52, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(59, 25, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(54, 46, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(51, 52, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(46, 46, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(44, 41, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(68, 49, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(54, 23, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(50, 24, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(54, 10, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(50, 13, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(32, 14, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(33, 18, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(24, 25, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(101, 10, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(105, 4, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(80, 31, "RESOURCE_TIN");
  ResourceBuilder.setResourceType(31, 32, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(24, 38, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(20, 35, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(14, 31, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(1, 29, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(2, 31, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(105, 35, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(102, 37, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(6, 14, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(0, 18, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(103, 24, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(99, 21, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(96, 30, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(92, 28, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(91, 23, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(82, 29, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(80, 24, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(99, 40, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(101, 43, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(76, 24, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(62, 14, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(64, 13, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(58, 8, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(60, 42, "RESOURCE_TURTLES");
  ResourceBuilder.setResourceType(8, 57, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(89, 8, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(96, 8, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(101, 13, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(96, 25, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(87, 27, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(86, 25, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(88, 39, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(98, 44, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(91, 60, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(87, 59, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(84, 63, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(81, 59, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(75, 63, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(77, 58, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(69, 60, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(59, 63, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(53, 62, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(44, 56, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(29, 52, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(22, 53, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(16, 56, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(13, 60, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(25, 28, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(51, 23, "RESOURCE_WILD_GAME");
  ResourceBuilder.setResourceType(86, 38, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(91, 41, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(87, 53, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(91, 47, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(54, 29, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(58, 23, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(61, 43, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(53, 41, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(51, 45, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(44, 46, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(43, 50, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(48, 37, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(48, 41, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(56, 41, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(67, 46, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(66, 48, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(53, 37, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(9, 39, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(5, 47, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(55, 11, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(28, 6, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(2, 8, "RESOURCE_WINE");
  ResourceBuilder.setResourceType(39, 60, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(86, 10, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(62, 59, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(78, 59, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(16, 60, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(105, 7, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(20, 54, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(62, 11, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(92, 11, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(90, 17, "RESOURCE_WOOL");
  ResourceBuilder.setResourceType(99, 16, "RESOURCE_WOOL");
}
function getRandomCivStartLocation() {
  const randomCiv = TerrainBuilder.getRandomNumber(50, "CIV TSL RANDOMIZATION");
  console.log(randomCiv + " ");
  let plotIndex = -1;
  switch (randomCiv) {
    //Base Game Civs
    //Antiquity
    case 0:
      plotIndex = GameplayMap.getIndexFromXY(61, 28);
      break;
    case 1:
      plotIndex = GameplayMap.getIndexFromXY(57, 33);
      break;
    case 2:
      plotIndex = GameplayMap.getIndexFromXY(56, 43);
      break;
    case 3:
      plotIndex = GameplayMap.getIndexFromXY(85, 46);
      break;
    case 4:
      plotIndex = GameplayMap.getIndexFromXY(83, 32);
      break;
    case 5:
      plotIndex = GameplayMap.getIndexFromXY(75, 36);
      break;
    case 6:
      plotIndex = GameplayMap.getIndexFromXY(20, 32);
      break;
    case 7:
      plotIndex = GameplayMap.getIndexFromXY(19, 45);
      break;
    case 8:
      plotIndex = GameplayMap.getIndexFromXY(67, 41);
      break;
    case 9:
      plotIndex = GameplayMap.getIndexFromXY(51, 44);
      break;
    //Exploration
    case 10:
      plotIndex = GameplayMap.getIndexFromXY(63, 43);
      break;
    case 11:
      plotIndex = GameplayMap.getIndexFromXY(74, 29);
      break;
    case 12:
      plotIndex = GameplayMap.getIndexFromXY(0, 36);
      break;
    case 13:
      plotIndex = GameplayMap.getIndexFromXY(27, 20);
      break;
    case 14:
      plotIndex = GameplayMap.getIndexFromXY(84, 23);
      break;
    case 15:
      plotIndex = GameplayMap.getIndexFromXY(90, 43);
      break;
    case 16:
      plotIndex = GameplayMap.getIndexFromXY(81, 52);
      break;
    case 17:
      plotIndex = GameplayMap.getIndexFromXY(44, 51);
      break;
    case 18:
      plotIndex = GameplayMap.getIndexFromXY(46, 27);
      break;
    case 19:
      plotIndex = GameplayMap.getIndexFromXY(43, 42);
      break;
    //Modern
    case 20:
      plotIndex = GameplayMap.getIndexFromXY(24, 44);
      break;
    case 21:
      plotIndex = GameplayMap.getIndexFromXY(56, 21);
      break;
    case 22:
      plotIndex = GameplayMap.getIndexFromXY(44, 49);
      break;
    case 23:
      plotIndex = GameplayMap.getIndexFromXY(102, 46);
      break;
    case 24:
      plotIndex = GameplayMap.getIndexFromXY(16, 34);
      break;
    case 25:
      plotIndex = GameplayMap.getIndexFromXY(72, 37);
      break;
    case 26:
      plotIndex = GameplayMap.getIndexFromXY(50, 51);
      break;
    case 27:
      plotIndex = GameplayMap.getIndexFromXY(90, 52);
      break;
    case 28:
      plotIndex = GameplayMap.getIndexFromXY(56, 56);
      break;
    case 29:
      plotIndex = GameplayMap.getIndexFromXY(82, 32);
      break;
    //CC0
    case 30:
      plotIndex = GameplayMap.getIndexFromXY(20, 45);
      break;
    //CC1
    case 31:
      plotIndex = GameplayMap.getIndexFromXY(45, 54);
      break;
    case 32:
      plotIndex = GameplayMap.getIndexFromXY(48, 38);
      break;
    case 33:
      plotIndex = GameplayMap.getIndexFromXY(56, 47);
      break;
    case 34:
      plotIndex = GameplayMap.getIndexFromXY(76, 38);
      break;
    //CC2
    case 35:
      plotIndex = GameplayMap.getIndexFromXY(84, 35);
      break;
    case 36:
      plotIndex = GameplayMap.getIndexFromXY(97, 48);
      break;
    case 37:
      plotIndex = GameplayMap.getIndexFromXY(63, 44);
      break;
    case 38:
      plotIndex = GameplayMap.getIndexFromXY(66, 44);
      break;
    //CC3
    case 39:
      plotIndex = GameplayMap.getIndexFromXY(25, 37);
      break;
    case 40:
      plotIndex = GameplayMap.getIndexFromXY(4, 17);
      break;
    case 41:
      plotIndex = GameplayMap.getIndexFromXY(58, 46);
      break;
    case 42:
      plotIndex = GameplayMap.getIndexFromXY(37, 60);
      break;
    //CC4
    case 43:
      plotIndex = GameplayMap.getIndexFromXY(95, 49);
      break;
    case 44:
      plotIndex = GameplayMap.getIndexFromXY(95, 49);
      break;
    case 45:
      plotIndex = GameplayMap.getIndexFromXY(99, 45);
      break;
    case 46:
      plotIndex = GameplayMap.getIndexFromXY(99, 45);
      break;
    //CC5
    case 47:
      plotIndex = GameplayMap.getIndexFromXY(64, 42);
      break;
    case 48:
      plotIndex = GameplayMap.getIndexFromXY(45, 48);
      break;
    case 49:
      plotIndex = GameplayMap.getIndexFromXY(45, 54);
      break;
    default:
      plotIndex = GameplayMap.getIndexFromXY(97, 9);
      break;
  }
  return plotIndex;
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
function paintEarthHugeSnow(width, height, topRows, bottomRows, maxWeight, randomization) {
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
