// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateBlocks.h"

GenerateBlocks::GenerateBlocks(AWorld_World* refWorld)
{
	world = refWorld;
}

GenerateBlocks::~GenerateBlocks()
{
}

Voxel_Voxel* GenerateBlocks::Gen(FIntVector worldPos)
{
	Voxel_Voxel* voxel = world->GetBlock(worldPos);
	if (voxel != nullptr)
	{
		if (FMath::RandRange(0, 100) > 98)
			return new Voxel_Glow();

		if (voxel->name == FName(TEXT("stone")))
			return new Voxel_Dirt();
	}
		

	return nullptr;
}