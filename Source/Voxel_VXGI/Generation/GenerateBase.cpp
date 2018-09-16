// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateBase.h"
#include "Voxel/Voxel_Stone.h"

GenerateBase::GenerateBase(int worldSeed)
{
	seed = worldSeed;
	simpleNoise = FastNoise();
	simpleNoise.SetSeed(seed);
	simpleNoise.SetFrequency(0.005);
}

GenerateBase::~GenerateBase()
{
}

Voxel_Voxel* GenerateBase::Gen(FIntVector worldPos)
{
	/*Actually this should figure out which biome it is in using !extremly! clever math
	So it calls get biome based on posX, posY, posZ
	Switches through the biomes
	Then based on the biome it calls a new biome class with the information
	*/
	int b = biome.getBiome(worldPos);
	switch (b) {
		case 1:
			return plain.Block(worldPos);
			break;
		case 2:
			return mountain.Block(worldPos);
			break;
		case 3:
			return swamp.Block(worldPos);
			break;
		case 4:
			return desert.Block(worldPos);
			break;
		default:
			return nullptr;
			break;
	}

	return nullptr;
}