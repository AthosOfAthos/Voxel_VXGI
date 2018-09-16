// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateDetails.h"

GenerateDetails::GenerateDetails(AWorld_World* refWorld)
{
	world = refWorld;
}

GenerateDetails::~GenerateDetails()
{
}

void GenerateDetails::Gen(FIntVector worldPos)
{
	//GetBlock can and will return nullptr, IT WILL CRASH YOUR EDITOR if you dont have checks
	if (world->GetBlock(worldPos) != nullptr)
	{
		//FNames are apparently faster than FString so im using them for ids
		//or you could use casting but thats spooky
		if (world->GetBlock(worldPos)->name == FName(TEXT("dirt")))
		{
			if (world->GetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 1)) == nullptr && FMath::RandRange(0, 100) > 99)
			{
				//Setblock actualy works its guaranteed that you have chunks in a 1 block radius loaded
				//may increase that thou
				world->SetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 1), new Voxel_Grass);
				world->SetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 2), new Voxel_Grass);
				world->SetBlock(FIntVector(worldPos.X, worldPos.Y, worldPos.Z + 3), new Voxel_Grass);
			}
		}
	}
}