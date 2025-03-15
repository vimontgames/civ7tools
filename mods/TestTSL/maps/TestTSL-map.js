// TestTSL-map.js
/**
* TestTSL
* 
*/
console.log("loading script TestTSL-map.js");
import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';
import { GetMap } from '/Civ7MapUser-TestTSL/maps/TestTSL-data.js';

function requestMapData(initParams) {
    initParams.width = 84;
    initParams.height = 54;
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
    const mapName = 'TestTSL';
    let genParameters = {
        westStart: 1,
        westEnd: 31,
        eastStart: 32,
        eastEnd: 102
    };
    let importedMap = GetMap();
    generateYnAMP(mapName, importedMap, genParameters);
}

// Register listeners.
engine.on('RequestMapInitData', requestMapData);
engine.on('GenerateMap', generateMap);

console.log("Loaded TestTSL-map.js");
