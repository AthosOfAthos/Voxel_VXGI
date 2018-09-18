// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_VXGIGameModeBase.h"

AVoxel_VXGIGameModeBase::AVoxel_VXGIGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = false;

	DefaultPawnClass = APawn_Player::StaticClass();
	PlayerControllerClass = AController_Player::StaticClass();
}

void AVoxel_VXGIGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Spawn gameWorld
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	gameWorld = GetWorld()->SpawnActor<AWorld_World>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo);
}

void AVoxel_VXGIGameModeBase::PostLogin(APlayerController* newPlayer)
{
	playerControllers.Add((AController_Player*)newPlayer);
}

void AVoxel_VXGIGameModeBase::Tick(float DeltaTime)
{
	for (int i = 0; i < playerControllers.Num(); i++)
	{
		if (playerControllers[i] != nullptr)
		{
			if (playerControllers[i]->bNeedsPawn)
			{
				if (SpawnPlayer(FVector(-400, -400, 500), playerControllers[i]))
					playerControllers[i]->bNeedsPawn = false;
			}
		}
	}
}

bool AVoxel_VXGIGameModeBase::SpawnPlayer(FVector spawnPos, APlayerController* controller)
{
	FIntVector chunkPos;
	chunkPos.X = FMath::RoundFromZero((float)spawnPos.X / 1000);
	chunkPos.Y = FMath::RoundFromZero((float)spawnPos.Y / 1000);
	chunkPos.Z = FMath::RoundFromZero((float)spawnPos.Z / 1000);
	if (gameWorld->chunkMap.Contains(chunkPos))
	{
		if (!gameWorld->chunkMap[chunkPos]->bNeedsGeneration)
		{
			FActorSpawnParameters spawnInfo;
			spawnInfo.Owner = controller;
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APawn* pawn = GetWorld()->SpawnActor<APawn_Player>(spawnPos, FRotator(0, 0, 0), spawnInfo);

			controller->Possess(pawn);
			return true;
		}
	}
		
	return false;
}