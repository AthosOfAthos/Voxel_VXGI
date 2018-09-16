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
	float height;
	height = simpleNoise.GetSimplex(worldPos.X, worldPos.Y);
	switch (b) {
		case 1:
			//biome = b1;
			//really this should say
			//return b1.generate(worldPos);
			//where b1.generate returns a voxel
			height = 5;
			break;
		case 2:
			//biome = b2;
			height = 10;
			break;
		default:
			height = 0;
			break;
	}
	if (worldPos.Z <= height)
		return new Voxel_Stone();

	return nullptr;
}