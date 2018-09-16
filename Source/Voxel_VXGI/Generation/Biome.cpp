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
	float t = temperature.GetPerlinFractal(pos.X, pos.Y);
	float r = rainfall.GetPerlinFractal(pos.X, pos.Y);
	int layer = 20;
	if (pos.Z > 0) {
		//surface biomes
		if (r > t) {
			return 2;
		}
		else {
			return 1;
		}
	}
	else if (pos.Z > -layer) {
		if (r > t) {
			return 2;
		}
		else {
			return 1;
		}

	}
	else if (pos.Z > -2* layer) {
		//layer 2
	}
	
	
	return 0;
}
