// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateBase.h"
#include "Voxel/Voxel_Stone.h"

GenerateBase::GenerateBase(int worldSeed)
{
	seed = worldSeed;
	simpleNoise = FastNoise();
	simpleNoise.SetSeed(seed);
}

GenerateBase::~GenerateBase()
{
}

Voxel_Voxel* GenerateBase::Gen(FIntVector worldPos)
{
	float height = simpleNoise.GetSimplex(worldPos.X, worldPos.Y);
	height *= 5;

	if (worldPos.Z <= height)
		return new Voxel_Stone();

	return nullptr;
}