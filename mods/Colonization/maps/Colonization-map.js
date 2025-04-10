// Colonization-map.js
/**
* Colonization
* 
*/
console.log("loading script Colonization-map.js");
import { generateYnAMP } from '/ged-ynamp/maps/ynamp-map-loading.js';
import { GetMap } from '/Civ7MapUser-Colonization/maps/Colonization-data.js';

function requestMapData(initParams) {
    initParams.width = 96;
    initParams.height = 60;
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
    const mapName = 'Colonization';
    let genParameters = {
        westStart: 3,
        westEnd: 39,
        eastStart: 45,
        eastEnd: 92
    };
    let importedMap = GetMap();
    generateYnAMP(mapName, importedMap, genParameters);
}

// Register listeners.
engine.on('RequestMapInitData', requestMapData);
engine.on('GenerateMap', generateMap);

console.log("Loaded Colonization-map.js");
