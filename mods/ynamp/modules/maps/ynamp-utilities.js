import * as globals from '/base-standard/maps/map-globals.js';
import * as utilities from '/base-standard/maps/map-utilities.js';

export function createLandmasses(iWidth, iHeight, continent1, continent2, iStartSectorRows, iStartSectorCols, startSectors, fMapScale, fWaterPercentFactor) {
    FractalBuilder.create(globals.g_LandmassFractal, iWidth, iHeight, 2, 0);
    let iWaterHeight = FractalBuilder.getHeightFromPercent(globals.g_LandmassFractal, globals.g_WaterPercent * fWaterPercentFactor);
    console.log("iWaterHeight = " + iWaterHeight);
    console.log("continent2.west = " + continent2.west);
    let iBuffer = Math.floor(iHeight / 18.0);
    let iBuffer2 = Math.floor(iWidth / 28.0);
    for (let iY = 0; iY < iHeight; iY++) {
        for (let iX = 0; iX < iWidth; iX++) {
            let terrain = globals.g_FlatTerrain;
            let iRandom = TerrainBuilder.getRandomNumber(iBuffer, "Random Top/Bottom Edges");
            let iRandom2 = TerrainBuilder.getRandomNumber(iBuffer2, "Random Left/Right Edges");
            // Initialize plot tag
            TerrainBuilder.setPlotTag(iX, iY, PlotTags.PLOT_TAG_NONE);
            //console.log("iPlotHeight at ("+iX+","+iY+")");
            let iPlotHeight = getHeightAdjustingForStartSector(iX, iY, iWaterHeight, globals.g_FractalWeight, globals.g_CenterWeight, globals.g_StartSectorWeight, continent1, continent2, iStartSectorRows, iStartSectorCols, startSectors, fMapScale);
            //console.log(" - Adjusted For Start Sector iPlotHeight = " + iPlotHeight);
			// if between the continents
            if (iX < continent1.west + iRandom2 || iX >= continent2.east - iRandom2 ||
                (iX >= continent1.east - iRandom2 && iX < continent2.west + iRandom2)) {
                iPlotHeight = Math.floor (iPlotHeight*0.5);
                //console.log("   - Adjusted between continents iPlotHeight = " + iPlotHeight);
            }

            //console.log(" - Final iPlotHeight = " + iPlotHeight + "/" + iWaterHeight + " * " +  globals.g_Cutoff);
            //  Must be water if at the poles
            if (iY < continent1.south + iRandom || iY >= continent1.north - iRandom) {
                terrain = globals.g_OceanTerrain;
            }
            else {
                // Finally see whether or not this stays as Land or has too low a score and drops back to water
                if (iPlotHeight < iWaterHeight * globals.g_Cutoff ) {
                    terrain = globals.g_OceanTerrain;
                }
            }
            // Add plot tag if applicable
            if (terrain != globals.g_OceanTerrain && terrain != globals.g_CoastTerrain) {
				//console.log("   - addLandmassPlotTags");
                utilities.addLandmassPlotTags(iX, iY, continent2.west);
            }
            else {
                utilities.addWaterPlotTags(iX, iY, continent2.west);
            }
            TerrainBuilder.setTerrainType(iX, iY, terrain);
			 if (GameplayMap.hasPlotTag(iX, iY, PlotTags.PLOT_TAG_EAST_LANDMASS)) {
				//console.log("   - PLOT_TAG_EAST_LANDMASS");
			 }
			 if (GameplayMap.hasPlotTag(iX, iY, PlotTags.PLOT_TAG_WEST_LANDMASS)) {
				//console.log("   - PLOT_TAG_WEST_LANDMASS");
			 }
        }
    }
}

export function createCloseIslands(iWidth, iHeight, continent1, continent2, iSize) {
    FractalBuilder.create(globals.g_LandmassFractal, iWidth, iHeight, iSize, 0);
    let iwater_percent = 50 /*Special Water Percent for Archipelago */ + iSize * 7;
    let iWaterHeight = FractalBuilder.getHeightFromPercent(globals.g_LandmassFractal, iwater_percent);
    let iBuffer = Math.floor(iWidth / 24.0);
    let terrain = globals.g_FlatTerrain;
    for (let iY = 0; iY < iHeight; iY++) {
        for (let iX = 0; iX < iWidth; iX++) {
            let iRandom = TerrainBuilder.getRandomNumber(iBuffer, "Random Top/Bottom Edges");
            if (iY >= continent1.south + iRandom &&
                iY <= continent1.north - iRandom &&
                (iX >= continent1.west && iX <= continent1.east ||
                    iX >= continent2.west && iX <= continent2.east)) {
                let iPlotHeight = FractalBuilder.getHeight(globals.g_LandmassFractal, iX, iY);
                if (iPlotHeight > iWaterHeight) {
                    TerrainBuilder.setTerrainType(iX, iY, terrain);
                    utilities.addLandmassPlotTags(iX, iY, continent2.west);
                }
            }
        }
    }
}

function getHeightAdjustingForStartSector(iX, iY, iWaterHeight, iFractalWeight, iCenterWeight, iStartSectorWeight, continent1, continent2, iStartSectorRows, iStartSectorCols, startSectors, fMapScale) {
    // Get the value from the fractal
    let iPlotHeight = FractalBuilder.getHeight(globals.g_LandmassFractal, iX, iY);
    iPlotHeight *= iFractalWeight;
    //console.log(" initial iPlotHeight = " + iPlotHeight);
	//*
    // Adjust based on distance from center of the continent
    let iDistanceFromCenter = utilities.getDistanceFromContinentCenter(iX, iY, continent1.south, continent1.north, continent1.west, continent1.east, continent2.west, continent2.east);
    let iMaxDistanceFromCenter = utilities.getMaxDistanceFromContinentCenter(iX, continent1.south, continent1.north, continent1.west, continent1.east, continent2.west, continent2.east);
    let iPercentFromCenter = Math.min(100 * iDistanceFromCenter / iMaxDistanceFromCenter * fMapScale, 100);
    iPlotHeight += iCenterWeight * Math.pow((iWaterHeight * (100 - iPercentFromCenter) / 100), globals.g_CenterExponent);
    //console.log(" Adjusted on distance from center of the continent : iPlotHeight = " + iPlotHeight + " / iPercentFromCenter =" + iPercentFromCenter + " / iDistanceFromCenter = " + iDistanceFromCenter);
	/*
    // Adjust based on whether or not the plot is near a start location (unless very far from center)
    if (iPercentFromCenter < globals.g_IgnoreStartSectorPctFromCtr) {
        let iSector = getSector(iX, iY, iStartSectorRows, iStartSectorCols, continent1.south, continent1.north, continent1.west, continent1.east, continent2.west);
        if (startSectors[iSector]) {
            // Start sector, increase chance we include it
            iPlotHeight += iStartSectorWeight * iWaterHeight;
            // Start sector and less than 2/3rds of full distance from center, add that amount again
            if (iPercentFromCenter < (globals.g_IgnoreStartSectorPctFromCtr * 2 / 3)) {
                iPlotHeight += iStartSectorWeight * iWaterHeight;
            }
        }
        // Interior sector that isn't a start sector? Give it the center bias
        if (iStartSectorCols > 2 && iStartSectorRows > 2) {
            let iTestSector = iSector;
            if (iTestSector >= iStartSectorRows * iStartSectorCols) {
                iTestSector = iSector - (iStartSectorRows * iStartSectorCols);
            }
            if ((iTestSector % iStartSectorCols) > 0 && (iTestSector % iStartSectorCols) < (iStartSectorCols - 1)) {
                if (iTestSector >= iStartSectorCols && iTestSector < (iStartSectorRows * iStartSectorCols - iStartSectorCols)) {
                    iPlotHeight += iCenterWeight * iWaterHeight;
                }
            }
        }
    }
	//*/
    return iPlotHeight;
}


/*	Civ6 maps --------------------------------------------------- | Civ7 maps --------------------------------------------------------------------------------
		
	0 FEATURE_FLOODPLAINS		0  TERRAIN_GRASS					0  FEATURE_SAGEBRUSH_STEPPE					0 BIOME_TUNDRA		0 TERRAIN_MOUNTAIN			
	1 FEATURE_ICE				1  TERRAIN_GRASS_HILLS				1  FEATURE_OASIS							1 BIOME_GRASSLAND	1 TERRAIN_HILL				
	2 FEATURE_JUNGLE			2  TERRAIN_GRASS_MOUNTAIN			2  FEATURE_DESERT_FLOODPLAIN_MINOR			2 BIOME_PLAINS		2 TERRAIN_FLAT				
	3 FEATURE_FOREST			3  TERRAIN_PLAINS					3  FEATURE_DESERT_FLOODPLAIN_NAVIGABLE		3 BIOME_TROPICAL	3 TERRAIN_COAST				
	4 FEATURE_OASIS				4  TERRAIN_PLAINS_HILLS				4  FEATURE_FOREST							4 BIOME_DESERT		4 TERRAIN_OCEAN				
	5 FEATURE_MARSH				5  TERRAIN_PLAINS_MOUNTAIN			5  FEATURE_MARSH							5 BIOME_MARINE		5 TERRAIN_NAVIGABLE_RIVER	
	6 FEATURE_BARRIER_REEF		6  TERRAIN_DESERT					6  FEATURE_GRASSLAND_FLOODPLAIN_MINOR		
								7  TERRAIN_DESERT_HILLS				7  FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE	
								8  TERRAIN_DESERT_MOUNTAIN			8  FEATURE_REEF								
								9  TERRAIN_TUNDRA					9  FEATURE_COLD_REEF						
								10 TERRAIN_TUNDRA_HILLS				10 FEATURE_ICE								
								11 TERRAIN_TUNDRA_MOUNTAIN			11 FEATURE_SAVANNA_WOODLAND					
								12 TERRAIN_SNOW						12 FEATURE_WATERING_HOLE					
								13 TERRAIN_SNOW_HILLS				13 FEATURE_PLAINS_FLOODPLAIN_MINOR			
								14 TERRAIN_SNOW_MOUNTAIN			14 FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE		
								15 TERRAIN_COAST					15 FEATURE_RAINFOREST						
								16 TERRAIN_OCEAN					16 FEATURE_MANGROVE							
																	17 FEATURE_TROPICAL_FLOODPLAIN_MINOR		
																	18 FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE	
																	19 FEATURE_TAIGA							
																	20 FEATURE_TUNDRA_BOG						
																	21 FEATURE_TUNDRA_FLOODPLAIN_MINOR			
																	22 FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE		
																	23 FEATURE_VOLCANO							
	
	Map Data (from Civ6 WB)
	MapToConvert[x][y] = {civ6TerrainType, civ6FeatureTypes, civ6ContinentType, {{IsNEOfRiver, flow}, {IsWOfRiver, flow}, {IsNWOfRiver, flow}}, {Civ6ResourceType, num}, {IsNEOfCliff, IsWOfCliff, IsNWOfCliff} }

//*/
const civ7terrain	= ["TERRAIN_MOUNTAIN", "TERRAIN_HILL", "TERRAIN_FLAT","TERRAIN_COAST", "TERRAIN_OCEAN","TERRAIN_NAVIGABLE_RIVER"];
const civ7biome		= ["BIOME_TUNDRA", "BIOME_GRASSLAND", "BIOME_PLAINS","BIOME_TROPICAL", "BIOME_DESERT", "BIOME_MARINE"];
const civ7Feature	= ["FEATURE_SAGEBRUSH_STEPPE", "FEATURE_OASIS", "FEATURE_DESERT_FLOODPLAIN_MINOR","FEATURE_DESERT_FLOODPLAIN_NAVIGABLE", "FEATURE_FOREST", "FEATURE_MARSH","FEATURE_GRASSLAND_FLOODPLAIN_MINOR", "FEATURE_GRASSLAND_FLOODPLAIN_NAVIGABLE", "FEATURE_REEF","FEATURE_COLD_REEF", "FEATURE_ICE", "FEATURE_SAVANNA_WOODLAND","FEATURE_WATERING_HOLE", "FEATURE_PLAINS_FLOODPLAIN_MINOR", "FEATURE_PLAINS_FLOODPLAIN_NAVIGABLE","FEATURE_RAINFOREST", "FEATURE_MANGROVE", "FEATURE_TROPICAL_FLOODPLAIN_MINOR","FEATURE_TROPICAL_FLOODPLAIN_NAVIGABLE", "FEATURE_TAIGA", "FEATURE_TUNDRA_BOG","FEATURE_TUNDRA_FLOODPLAIN_MINOR", "FEATURE_TUNDRA_FLOODPLAIN_NAVIGABLE", "FEATURE_VOLCANO"];
const civ6Terrain	= ["TERRAIN_GRASS", "TERRAIN_GRASS_HILLS", "TERRAIN_GRASS_MOUNTAIN", "TERRAIN_PLAINS", "TERRAIN_PLAINS_HILLS", "TERRAIN_PLAINS_MOUNTAIN", "TERRAIN_DESERT", "TERRAIN_DESERT_HILLS", "TERRAIN_DESERT_MOUNTAIN", "TERRAIN_TUNDRA", "TERRAIN_TUNDRA_HILLS", "TERRAIN_TUNDRA_MOUNTAIN", "TERRAIN_SNOW", "TERRAIN_SNOW_HILLS", "TERRAIN_SNOW_MOUNTAIN", "TERRAIN_COAST", "TERRAIN_OCEAN"];
const civ6Feature	= ["FEATURE_FLOODPLAINS","FEATURE_ICE", "FEATURE_JUNGLE", "FEATURE_FOREST","FEATURE_OASIS", "FEATURE_MARSH", "FEATURE_BARRIER_REEF"];

const mapIDX = {
	terrain: 0,
	feature: 1,
	continent: 2,
	river: 3,
	resource: 4,
	cliff: 5
}

const mapIDXV7 = {
	terrain: 0,
	biome: 1,
	feature: 2
}

function getCiv6Terrain(iTerrain) {
	return civ6Terrain[iTerrain];
}

function getTerrainType(sCiv6Terrain) {
	if (sCiv6Terrain.search("MOUNTAIN") != -1) {
		return globals.g_MountainTerrain;
	} else if (sCiv6Terrain.search("HILLS") != -1) {
		return globals.g_HillTerrain;
	} else if (sCiv6Terrain == "TERRAIN_COAST") {
		return globals.g_CoastTerrain;
	} else if (sCiv6Terrain == "TERRAIN_OCEAN") {
		return globals.g_OceanTerrain;
	} 
	// default
	return globals.g_FlatTerrain;
}

function getTerrainTypeV7(sCiv7Terrain) 
{
	console.log("getTerrainTypeV7 - sCiv7Terrain = " + sCiv7Terrain);
	
	if (sCiv7Terrain.search("MOUNTAIN") != -1) 
	{
		return globals.g_MountainTerrain;
	} else if (sCiv7Terrain.search("HILLS") != -1)
	{
		return globals.g_HillTerrain;
	} else if (sCiv7Terrain == "TERRAIN_HILL") 
	{
		return globals.g_HillTerrain;
	} else if (sCiv7Terrain == "TERRAIN_COAST") 
	{
		return globals.g_CoastTerrain;
	} else if (sCiv7Terrain == "TERRAIN_OCEAN") 
	{
		return globals.g_OceanTerrain;
	} 
	else if (sCiv7Terrain == "TERRAIN_NAVIGABLE_RIVER") 
	{
		return globals.g_NavigableRiverTerrain;
	} 
	// default
	return globals.g_FlatTerrain;
}

function getBiomeType(sCiv6Terrain) {
	if (sCiv6Terrain.search("GRASS") != -1) {
		return globals.g_GrasslandBiome;
	} else if (sCiv6Terrain.search("PLAINS") != -1) {
		return globals.g_PlainsBiome;
	} else if (sCiv6Terrain.search("DESERT") != -1) {
		return globals.g_DesertBiome;
	} else if (sCiv6Terrain.search("TUNDRA") != -1) {
		return globals.g_TundraBiome;
	} else if (sCiv6Terrain.search("SNOW") != -1) {
		return globals.g_TundraBiome;
	}
	// default
	return globals.g_MarineBiome;
}

function getBiomeTypeV7(sCiv7Biome) 
{
	if (sCiv7Biome.search("TUNDRA") != -1) 
	{
		return globals.g_TundraBiome;
	} 
	else if (sCiv7Biome.search("GRASSLAND") != -1) 
	{
		return globals.g_GrasslandBiome;
	}
	else if (sCiv7Biome.search("PLAINS") != -1) 
	{
		return globals.g_PlainsBiome;
	}
	else if (sCiv7Biome.search("TROPICAL") != -1) 
	{
		return globals.g_TropicalBiome;
	}
	else if (sCiv7Biome.search("DESERT") != -1) 
	{
		return globals.g_DesertBiome;
	}
	else if (sCiv7Biome.search("MARINE") != -1) 
	{
		return globals.g_MarineBiome;
	}
	
	// default
	return globals.g_MarineBiome;
}

export function getTerrainFromRow(row) {
	let terrain = row[mapIDX.terrain];
	console.log("getTerrainFromRow - terrain = " + terrain);
	if (typeof(terrain) == 'number') {
		let sCiv6Terrain = civ6Terrain[terrain];
		return getTerrainType(sCiv6Terrain);
	} else {
		return getTerrainType(terrain);
	}
}

export function getTerrainFromRowV7(row) 
{
	let terrain = row[mapIDXV7.terrain];
	console.log("getTerrainFromRowV7 - terrain = " + terrain);
	if (typeof(terrain) == 'number') 
	{
		terrain = civ7terrain[terrain];
	} 
	
	return getTerrainTypeV7(terrain);
}


export function getBiomeFromRow(row) {
	let terrain = row[mapIDX.terrain];
	let feature = row[mapIDX.feature];
	let isJungle;
	
	if (typeof(feature) == 'number') {
		isJungle = civ6Feature[feature] == "FEATURE_JUNGLE";
	} else {
		isJungle = feature == "FEATURE_JUNGLE";
	}
	
	if (isJungle) {
		return globals.g_TropicalBiome;
	}
	
	if (typeof(terrain) == 'number') {
		let sCiv6Terrain = civ6Terrain[terrain];
		return getBiomeType(sCiv6Terrain);
	} else {
		return getBiomeType(terrain);
	}
}

export function getBiomeFromRowV7(row) 
{
	let biome = row[mapIDXV7.biome];
	console.log("getBiomeFromRowV7 - biome = " + biome);
		
	if (typeof(biome) == 'number') 
	{
		biome = civ7biome[biome];
	} 
	
	return getBiomeTypeV7(biome);
}


export function isRowJungle(row) {
	let feature = row[mapIDX.feature];
	//console.log("isRowjungle - feature = " + feature);
	if (typeof(feature) == 'number') {
		let sCiv6Feature = civ6Feature[feature];
		return sCiv6Feature == "FEATURE_JUNGLE";
	} else {
		return feature == "FEATURE_JUNGLE";
	}
}

export function isRowSnow(row) {
	let terrain = row[mapIDX.terrain];
	console.log("getTerrainFromRow - terrain = " + terrain);
	if (typeof(terrain) == 'number') {
		let sCiv6Terrain = civ6Terrain[terrain];
		return sCiv6Terrain.search("SNOW") != -1;
	} else {
		return terrain.search("SNOW") != -1;
	}
}


/*
(for reference)
g_MountainTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_MOUNTAIN').$index;
g_HillTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_HILL').$index;
g_FlatTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_FLAT').$index;
g_CoastTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_COAST').$index;
g_OceanTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_OCEAN').$index;
g_NavigableRiverTerrain = GameInfo.Terrains.find(t => t.TerrainType == 'TERRAIN_NAVIGABLE_RIVER').$index;
g_TundraBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_TUNDRA').$index;
g_GrasslandBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_GRASSLAND').$index;
g_PlainsBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_PLAINS').$index;
g_TropicalBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_TROPICAL').$index;
g_DesertBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_DESERT').$index;
g_MarineBiome = GameInfo.Biomes.find(t => t.BiomeType == 'BIOME_MARINE').$index;
g_VolcanoFeature = GameInfo.Features.find(t => t.FeatureType == 'FEATURE_VOLCANO').$index;
//*/

console.log("Loaded YnAMP Utilities");

