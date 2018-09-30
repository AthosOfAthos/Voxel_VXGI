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
				int height = FMath::RandRange(2, 10);
				//Setblock actualy works its guaranteed that you have chunks in a 1 block radius loaded
				//may increase that thou
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						for (int k = -1; k <= height; k++) {
							world->SetBlock(FIntVector(worldPos.X+i, worldPos.Y+j, worldPos.Z + k), new Voxel_Wood);
						}
					}
				}
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						for (int k = height-2; k <= height+1; k++) {
							if (world->GetBlock(FIntVector(worldPos.X + i, worldPos.Y + j, worldPos.Z + k)) == nullptr) {
								world->SetBlock(FIntVector(worldPos.X + i, worldPos.Y + j, worldPos.Z + k), new Voxel_Grass);
							}
						}
					}
				}
				
			}
		}
	}
}