// Fill out your copyright notice in the Description page of Project Settings.

#include "Biome.h"

Biome::Biome(int seed)
{
	//Shouldset seeds
	temperature = FastNoise();
	temperature.SetSeed(seed);
	temperature.SetFrequency(0.001);


	rainfall = FastNoise();
	rainfall.SetSeed(seed+1);
	rainfall.SetFrequency(0.001);
}

Biome::~Biome()
{
}

int Biome::getBiome(FIntVector pos) 
{
	float t = temperature.GetPerlin(pos.X, pos.Y);
	float r = rainfall.GetPerlin(pos.X, pos.Y);
	int layer = 20;
	if (pos.Z > -layer) {
		//surface biomes
		if (r >0) {
			if (t > 0) {
				return 3;//swamp 3
			}
			else {
				return 1;//plains
			}
		}
		else {
			if (t > 0) {
				return 4;//desert
			}
			else {
				return 2;//mountain
			}
		}
	}
	else if (pos.Z > -2* layer) {
		//layer 2
	}
	
	
	return 0;
}
