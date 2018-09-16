// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Math/FastNoise.h"
#include "CoreMinimal.h"

/**
 * 
 */
class VOXEL_VXGI_API Biome
{
public:
	Biome(int seed);
	~Biome();
	int getBiome(FIntVector);
	FastNoise temperature;
	FastNoise rainfall;
};
