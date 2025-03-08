// Test4-map.js
/**
* Test4
* 
*/
console.log("loading script Test4-map.js");
import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';
import { GetMap } from '/authorname-Test4/maps/Test4-data.js';

function requestMapData(initParams) {
    initParams.width = 104;
    initParams.height = 64;
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
    const mapName = 'Test4';
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

console.log("Loaded Test4-map.js");
