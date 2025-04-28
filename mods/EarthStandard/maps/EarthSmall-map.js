// EarthSmall-map.js
/**
* EarthSmall
* 
*/
console.log("loading script EarthSmall-map.js");
import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';
import { GetMap } from '/Civ7MapUser-EarthSmall/maps/EarthSmall-data.js';

function requestMapData(initParams) {
    initParams.width = 74;
    initParams.height = 46;
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

console.log("Loaded EarthSmall-map.js");
