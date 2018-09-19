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
		{
			if (worldPos.Z == -1)
			{
				if (world->GetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 2)) == nullptr)
					return new Voxel_Grass();
			}
			else
			{
				if (world->GetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 1)) == nullptr)
					return new Voxel_Grass();
			}

			

			return new Voxel_Dirt();
		}
			
	}
		

	return nullptr;
}