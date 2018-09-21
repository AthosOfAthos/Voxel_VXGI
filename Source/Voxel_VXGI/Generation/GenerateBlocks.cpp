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

				int b = biome.getBiome(worldPos);
				switch (b) {
				case 1:
					return plain.SurfaceBlock(worldPos);
					break;
				case 2:
					return mountain.SurfaceBlock(worldPos);
					break;
				case 3:
					return swamp.SurfaceBlock(worldPos);
					break;
				case 4:
					return desert.SurfaceBlock(worldPos);
					break;
				default:
					return nullptr;
					break;

			}

			

			return new Voxel_Dirt();
		}
			
	}
		

	return nullptr;
}