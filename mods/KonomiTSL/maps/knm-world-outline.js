
console.log("Generating using script knm-world-outline.js");
import { assignStartPositions, chooseStartSectors } from '/base-standard/maps/assign-starting-plots.js';
import { addMountains, addHills, expandCoasts, buildRainfallMap, generateLakes } from '/base-standard/maps/elevation-terrain-generator.js';
import { addFeatures, designateBiomes } from '/base-standard/maps/feature-biome-generator.js';
import * as globals from '/base-standard/maps/map-globals.js';
import * as utilities from '/base-standard/maps/map-utilities.js';
import { addNaturalWonders } from '/base-standard/maps/natural-wonder-generator.js';
import { generateResources } from '/base-standard/maps/resource-generator.js';
import { addTundraVolcanoes, addVolcanoes } from '/base-standard/maps/volcano-generator.js';
import { assignAdvancedStartRegions } from '/base-standard/maps/assign-advanced-start-region.js';
import { generateDiscoveries } from '/base-standard/maps/discovery-generator.js';
import { generateSnow, dumpPermanentSnow } from '/base-standard/maps/snow-generator.js';
import { dumpStartSectors, dumpContinents, dumpTerrain, dumpElevation, dumpRainfall, dumpBiomes, dumpFeatures, dumpResources, dumpNoisePredicate } from '/base-standard/maps/map-debug-helpers.js';

let HEIGHT = 0, WIDTH = 0;

const terrains = [
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~~   ~~~~ . ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~~ ^. ~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~      ~~ ~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~  ~~~~~',
    '~~~~~~~~~~~~~~~~~~~~~ ^^      ~~~~~~~~~~~~~~~~~~~~~~~~ . ~~~~~~~~~~~~~~~~~~~~ ^ ~~~~',
    '~~~~~~~~~~~~~~~~~~~~ ^^      ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~  ~~~ ^ ~~~~',
    '~~~~~~~~~~~~~~~~~~~~ ^^    . ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ^ ~~~ ^ ~~~',
    '~~~~~~~~~~~~~~~~~~~~ ^^     ~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~   ~~~',
    '~~~~~~~~~~~~~~~~~~~~ .^^   ~~~~~~~~~~~~~~~~    ~~~~~~~~~~~~~~~~~~~      .^ ~~~~ ^ ~~',
    '~~~~~~~~~~~~~~~~~~~ .^^   ~~~~~~~~~~~~~~~~ ... ~~~~~~~~~~~~~~~~~~ ..   ..^ ~~~ . ~~~',
    '~~~~~~~~~~~~~~~~~~~  .^..  ~~~~~~~~~~~~~~~ ..^^ ~~~~~~~~~~~~~~~~~~ .....^.^ ~~~  ~~~',
    '~~~~~~~~~~~~~~~~~~~ .M..   ~~~~~~~~~~~~~~ ^.^M. ~~~~~~~~~~~~~~~~~ ........^ ~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~ .M...  ~~~~~~~~~~~~~  ^.^.  ~~~~~~~~~~~~~~~~ .........^ ~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~ .MM.... ~~~~~~~~~~~~  ^..^.    ~~~~~~~~~~~~~ .^..^^..^^ ~~~~~~~~',
    '~~~~~~~~~~~~~~~~~~~~ ^MM..^^  ~~~~~~~~~~~  ^^^..    ~~~~~~~~~~~~~ .^..^^.^. ~~  ~~~~',
    '~~~  ~~~~~~~~~~~~~~  ^M^..^.. ~~~~~~~~~~  .^....  . ~~~~~~~~~~~~~ ......^. ~~ .   ~~',
    '~~~ . ~~~~~~~~~~~~~~ ^M^...^^. ~~~~~~~~~  ...^..  .^ ~~~~~~~~~~~~~  .....^ ~~~   . ~',
    '~~~  ~~~~~~~~~~~~~~ ^ ^....^^. ~~~~~~~~~  ..^^..  .^  ~~~~~~~~~~~~  ^.. . ~~~     ~~',
    '~~~~~~~~~~~~~~~~~~~ ^ ^....^^. ~~~~~~~~~~ ...^ ..  ..  ~~~~~~~~~~~    . . ~~~~~  ~~~',
    '~~~~~~~~~~~~~~~~~~ .^^.....^.. ~~~~~~~~~~ ...^...     ~~~~~~~~~~~          ~  ~~~~~~',
    '~~~~~~~~~~~~~~~~~~ .M^....^.... ~~~~~~~~~ ...^^..    ~~~~~~~~~~~          ^  ^ ~~~~~',
    '~~~~~~~~~~~~~~~~~ .M^..^.....^. ~~~~~~   ....^^.  ~~~~~~~~~~~~~ .. .   ..^     ~~~~~',
    '~~~~~~~~~~~~~~~~~ .MM.......... ~~~~~    ..... ..  ~~~~~~~~~~~          ^M ^ ~~~~~~~',
    '~~~~~~~~~~~~~~~   ^M........^. ~~~~  ........^^.. ~~~~~~~~~~~ ^.   .  ^^.   ~~~~~~~~',
    '~~~~~~~~~~~~~~~   .M........   ~~~~ .............  ~~~~~~~~~  ^. .. .     ~~~~~~~~~~',
    '~~~~~~~~~~~~~~~   ^M.......  ~~~~  .^.........^..  ~~~~~~    ^  .^.  ~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~   ^M..^^..  ~~~~  .^..........^^.  ~~~~~      . .^  ~~~~~~~~~~~~~~~',
    '~~~~~~~~~~~~~~~~   ^M^....  ~~~~ . ..........^M...  ~~~~~ ^   ^   ^  ~~~~~~  ~~~~~~~',
    '~~~~~~~~~~~~~~~~~  .M^.^.  ~~~~~~  ......^....^.     ~~~      .     ^ ~~~~~ . ~~~~~~',
    '~~~~~~~~~~~~~~~  ...^^..  ~~~~~~  .......^..... ^.. ~~~ .    . .  ~  ~~~~~~  ~~~~~~~',
    '~~~~~~~~~~~~~~   .. ..^   ~~~~~~   .^.......... ^... ~~ ..    ..^ ~ . ~~~~~~ ~~~~~~~',
    '~~~~~~~~~~~~~   ..     . ~~~~~~    ^MM........ ..... ~ .^.    ..^    ~~~~~~~~~~~~~~~',
    '~~   ~~~~~~~~  .^^.       ~~~~~ ^   .^MM^..... .....^  ....   ...  . ~~~~~~~~~~~~~~~',
    '~  ^ ~~~~~~~ .^^...   .. ~~~~~~      ^^^.  ....... .   ...^  ...   ^ ~~~~~~~~~~~~~~~',
    '~ ^  ~~~~~ ~ .^^. .  .   ~~~~~~~   ..  ..      ...     ..... .^^.     ~~~~~~~~~~~~~~',
    '~   ~~~~~   .^M.   ..   ~~~~~~~   ..^    .     ....M.^.......^^^.. ^  ~~~  ~~~~~~~~~',
    '~~~~~~~~~ . .MM.      . ~~~~~~~   ....  ^ . .   ^..M..^.....^.M^^..       . ~~~~~~~~',
    '~~~~~~~~ . .M^^.   .   ~~~~~~~~   .^^.   ^ ^.   ...^..^.^^^M^^M^..^   .     ~~~~~~~~',
    '~~~~~~~~ . .^^...  .  ~~~~~~~~~~   .^M^.^..^^  ^.^.....^^MMMMMM^....    ..  ~~~~~~~~',
    '~~~~~~~ ...^^......   ~~~~~~~~~~     M..MM^^  ...^^...^MMMMMMM^^^...     .. ~~~~~~~~',
    '~~~~~~  .^^^^^..^....  ~~~~~~~~~     ..^^MM.....^^^^^^.^^MM^^^^^^^..  .   ^  ~~~~~~~',
    '~~~~~  .M^^MM^....^M.  ~~ ~~~~~~    ....^^^^..   ^^ ....^^^^^^^^^^.  ..   . ~~~~~~~~',
    '~~~~~~  .M^ M^......^.  ~  ~~~~~~   .....^...^   ^M . ...^^^^^^^^^^...^ ~    ~~~~~~~',
    '~~~~~~ .M^MM..... ^ ^.     ~~~~~      ............. .....^^^^^^^^^^^^^  ~ ^ ~~~~~~~~',
    '    ~~~ .^^^^..... ..... .  ~~~~   ...  ..............M......^^^MM^^^^.^     ~~~~~~ ',
    ' ^^ ~~  .MM^..........     ~~~~ .^  ..   . ........^..M........MM^ ^^^..  .     ~~ ^',
    '   ^ ~  ^MM^..........^..  ~~~~  .. ^.   .  ....^...^..M.......^^.^ ^^^^^^     M ~~ ',
    ' ...  ^^M^^. ....    ...  ~~     .  ..    .  ..........M........^.^^^..^^      ^  ~ ',
    '  .^^^^^^^M^.....    ..  ~~  ^ ^^   ^^   ^..  . ........MM.......^.^...^^^^^^. ^^^ ~',
    ' ^^MM^M^^M^ .....       ~~ ^^.      ^    ^^^ ... ........^........^^..^^.......^^^ ~',
    '~ ^^^^^^^^......   ..   ~  .....^    . .   ^^...         ..............             ',
    '~ ....~~~~~~~~~~~~    ~~ ^^.....^          ^^...  ~~~~~                 ..  ~       ',
]

const terrainMap = {
    '.': globals.g_FlatTerrain,
    '^': globals.g_HillTerrain,
    'M': globals.g_MountainTerrain,
    ' ': globals.g_CoastTerrain,
    '~': globals.g_OceanTerrain,
    'R': globals.g_NavigableRiverTerrain,
}

const mainLandPlots = [
    4529, 4530, 4531, 4532, 4533, 4534, 4535, 
    4447, 4448, 4449, 4450, 4451,
    4363, 4364, 4365, 
    4281, 4283, 
]

const mainLandWaterPlots = [
    4528, 
    4445, 4446,
    4362, 4366, 4367, 
    4279, 4280, 4282,
    4280, 4282,
    4196, 4197, 4198, 4199,
    4115, 
    4031, 
    3359, 
    3275,
    3191,
    3023, 
    2855, 
]

function getIndexFromXY(x, y, width) {
    return y * width + x;
}

function getXYFromIndex(index, width) {
    const y = Math.floor(index / width);
    const x = index % width;
    return [x, y];
}

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
function generateMap() {
    console.log("Generating a map!");
    console.log(`Age - ${GameInfo.Ages.lookup(Game.age).AgeType}`);
    let iWidth = GameplayMap.getGridWidth();
    WIDTH = iWidth;
    let iHeight = GameplayMap.getGridHeight();
    HEIGHT = iHeight;
    let uiMapSize = GameplayMap.getMapSize();
    let startPositions = [];
    let mapInfo = GameInfo.Maps.lookup(uiMapSize);
    if (mapInfo == null)
        return;
    let iNumNaturalWonders = mapInfo.NumNaturalWonders;
    let iTilesPerLake = mapInfo.LakeGenerationFrequency;
    let iNumPlayers1 = mapInfo.PlayersLandmass1;
    let iNumPlayers2 = mapInfo.PlayersLandmass2;
    // Establish continent boundaries
    let iOceanWaterColumns = globals.g_OceanWaterColumns;
    let westContinent = {
        west: 0,
        east: 49,
        south: globals.g_PolarWaterRows,
        north: iHeight - globals.g_PolarWaterRows,
        continent: 0
    };
    let eastContinent = {
        west: 51,
        east: 82,
        south: globals.g_PolarWaterRows,
        north: iHeight - globals.g_PolarWaterRows,
        continent: 0
    };
    let startSectors = [];
    let iStartSectorRows = 0;
    let iStartSectorCols = 0;
    let startPosition = Configuration.getMapValue("StartPosition");
    if (startPosition == null) {
        startPosition = Database.makeHash('START_POSITION_STANDARD');
    }
    startPosition = Number(BigInt.asIntN(32, BigInt(startPosition))); // Convert to signed int32.

    utilities.addPlotTags(iHeight, iWidth, 50);
    mainLandPlots.forEach(plot => {
        let [iX, iY] = getXYFromIndex(plot, iWidth)
        TerrainBuilder.removePlotTag(iX, iY, PlotTags.PLOT_TAG_EAST_LANDMASS);
        TerrainBuilder.addPlotTag(iX, iY, PlotTags.PLOT_TAG_WEST_LANDMASS);
    });
    mainLandWaterPlots.forEach(plot => {
        let [iX, iY] = getXYFromIndex(plot, iWidth)
        TerrainBuilder.removePlotTag(iX, iY, PlotTags.PLOT_TAG_EAST_WATER);
        TerrainBuilder.addPlotTag(iX, iY, PlotTags.PLOT_TAG_WEST_WATER);
    });

    setTerrain();
    TerrainBuilder.validateAndFixTerrain();
    AreaBuilder.recalculateAreas();
    TerrainBuilder.stampContinents();
    addVolcanoes(iWidth, iHeight);
    AreaBuilder.recalculateAreas();
    TerrainBuilder.buildElevation();

    buildRainfallMap(iWidth, iHeight);
    TerrainBuilder.modelRivers(5, 15, globals.g_NavigableRiverTerrain);
    TerrainBuilder.validateAndFixTerrain();
    TerrainBuilder.defineNamedRivers();
    designateBiomes(iWidth, iHeight);
    addTundraVolcanoes(iWidth, iHeight);
    addNaturalWonders(iWidth, iHeight, iNumNaturalWonders);
    TerrainBuilder.addFloodplains(4, 10);
    addFeatures(iWidth, iHeight);
    TerrainBuilder.validateAndFixTerrain();
    AreaBuilder.recalculateAreas();
    TerrainBuilder.storeWaterData();
    generateSnow(iWidth, iHeight);
    generateResources(iWidth, iHeight);
    startPositions = assignStartPositions(iNumPlayers1, iNumPlayers2, westContinent, eastContinent, iStartSectorRows, iStartSectorCols, startSectors);
    generateDiscoveries(iWidth, iHeight, startPositions);
    FertilityBuilder.recalculate(); // Must be after features are added.
    // let seed = GameplayMap.getRandomSeed(); // can use any seed you want for different noises
    // let avgDistanceBetweenPoints = 3;
    // let normalizedRangeSmoothing = 2;
    // let poisson = TerrainBuilder.generatePoissonMap(seed, avgDistanceBetweenPoints, normalizedRangeSmoothing);
    // let poissonPred = (val) => {
    //     return val >= 1 ? "*" : " ";
    // };
    // dumpNoisePredicate(iWidth, iHeight, poisson, poissonPred);
    assignAdvancedStartRegions();
}
// Register listeners.
engine.on('RequestMapInitData', requestMapData);
engine.on('GenerateMap', generateMap);
console.log("Loaded knm-world-outline.js");
// console.log("hey, continents is firing");
function setTerrain() {
    for (let iY = 0; iY < HEIGHT; iY++) {
        for (let iX = 0; iX < WIDTH; iX++) {
            const terrainChar = terrains[iY][iX];
            TerrainBuilder.setTerrainType((iX - 32) % WIDTH, iY, terrainMap[terrainChar])
        }
    }
}
