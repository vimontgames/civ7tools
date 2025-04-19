// EARTHSTANDARD-map.js
/**
* EARTHSTANDARD
* 
*/
console.log("loading script EARTHSTANDARD-map.js");
import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';
import { GetMap } from '/Civ7MapUser-EARTHSTANDARD/maps/EARTHSTANDARD-data.js';

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
    const mapName = 'EARTHSTANDARD';
    let genParameters = {
        westStart: 1,
        westEnd: 25,
        eastStart: 28,
        eastEnd: 83
    };
    let importedMap = GetMap();
    generateYnAMP(mapName, importedMap, genParameters);
}

// Register listeners.
engine.on('RequestMapInitData', requestMapData);
engine.on('GenerateMap', generateMap);

console.log("Loaded EARTHSTANDARD-map.js");
