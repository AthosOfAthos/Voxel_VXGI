// Fill out your copyright notice in the Description page of Project Settings.

#include "World_World.h"

AWorld_World::AWorld_World()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	//Load StaticMeshes into memory
	renderMesh.Init(nullptr, 2000);
	renderMesh[1000] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Stone.Voxel_Stone"));
	renderMesh[1001] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Grass.Voxel_Grass"));
	renderMesh[1002] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Dirt.Voxel_Dirt"));
	renderMesh[1003] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Sand.Voxel_Sand"));
	renderMesh[1004] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Rubble.Voxel_Rubble"));
	renderMesh[1005] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Wood.Voxel_Wood"));
	renderMesh[1006] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Leaf.Voxel_Leaf"));
	renderMesh[1100] = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Glow.Voxel_Glow"));
}

void AWorld_World::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		//Server logic
		seed = FMath::RandRange(0, 50000);

		//Create generation threads;
		generateBaseThreads.Add(new Thread_GenerateBase(seed));
		FRunnableThread::Create(generateBaseThreads[0], TEXT("Generation Thread 1"), 0, TPri_Normal);

		generateBaseThreads.Add(new Thread_GenerateBase(seed));
		FRunnableThread::Create(generateBaseThreads[1], TEXT("Generation Thread 2"), 0, TPri_Normal);

		generateBaseThreads.Add(new Thread_GenerateBase(seed));
		FRunnableThread::Create(generateBaseThreads[2], TEXT("Generation Thread 3"), 0, TPri_Normal);
		
		for (int chunkX = -5; chunkX < 5; chunkX++)
		{
			for (int chunkY = -5; chunkY < 5; chunkY++)
			{
				for (int chunkZ = -5; chunkZ < 5; chunkZ++)
				{
					LoadChunk(FIntVector(chunkX, chunkY, chunkZ));
				}
			}
		}

	}
	else
	{
		//client logic

	}
	
}

void AWorld_World::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		ManageChunk();
	}
}

Voxel_Voxel* AWorld_World::GetBlock(FIntVector blockPos)
{
	FIntVector chunkPos;
	chunkPos.X = FMath::RoundFromZero((float)blockPos.X / 10);
	chunkPos.Y = FMath::RoundFromZero((float)blockPos.Y / 10);
	chunkPos.Z = FMath::RoundFromZero((float)blockPos.Z / 10);

	if (chunkMap.Contains(chunkPos))
		return chunkMap[chunkPos]->GetBlock(blockPos);

	return nullptr;
}

void AWorld_World::SetBlock(FIntVector blockPos, Voxel_Voxel* voxel)
{
	FIntVector chunkPos;
	chunkPos.X = FMath::RoundFromZero((float)blockPos.X / 10);
	chunkPos.Y = FMath::RoundFromZero((float)blockPos.Y / 10);
	chunkPos.Z = FMath::RoundFromZero((float)blockPos.Z / 10);

	if (chunkMap.Contains(chunkPos))
		chunkMap[chunkPos]->SetBlock(blockPos, voxel);
}

bool AWorld_World::LoadChunk(FIntVector chunkPos)
{
	if (HasAuthority())
	{
		if (chunkPos.X == 0 || chunkPos.Y == 0 || chunkPos.Z == 0)
			return false;

		if (!chunkMap.Contains(chunkPos))
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FIntVector spawnPos = chunkPos;
			if (chunkPos.X > 0)
				spawnPos.X--;
			if (chunkPos.Y > 0)
				spawnPos.Y--;
			if (chunkPos.Z > 0)
				spawnPos.Z--;
			chunkMap.Add(chunkPos, GetWorld()->SpawnActor<AWorld_Chunk>(FVector(spawnPos.X * 1000, spawnPos.Y * 1000, spawnPos.Z * 1000), FRotator(0, 0, 0), SpawnInfo));
			return true;
		}
	}
	return false;
}

bool AWorld_World::UnloadChunk(FIntVector chunkPos)
{
	if (HasAuthority() && chunkMap.Contains(chunkPos))
	{
		if (chunkMap[chunkPos] != nullptr)
		{
			if (!chunkMap[chunkPos]->bNeedsGeneration)
			{
				//TODO: save chunk
				chunkMap[chunkPos]->Destroy();
				chunkMap.Remove(chunkPos);
				return true;
			}
		}
	}

	return false;
}

void AWorld_World::ManageChunk()
{
	int viewRadius = 4;
	int cullRadius = 5;
	int numPlayers = playerPawns.Num();
	for (int i = 0; i < numPlayers; i++)
	{
		if (playerPawns[i] != nullptr)
		{
			FIntVector chunkPos;
			chunkPos.X = FMath::RoundFromZero((float)playerPawns[i]->GetActorLocation().X / 1000);
			chunkPos.Y = FMath::RoundFromZero((float)playerPawns[i]->GetActorLocation().Y / 1000);
			chunkPos.Z = FMath::RoundFromZero((float)playerPawns[i]->GetActorLocation().Z / 1000);

			for (int chunkX = chunkPos.X - viewRadius; chunkX < chunkPos.X + viewRadius; chunkX++)
			{
				for (int chunkY = chunkPos.Y - viewRadius; chunkY < chunkPos.Y + viewRadius; chunkY++)
				{
					for (int chunkZ = chunkPos.Z - viewRadius; chunkZ < chunkPos.Z + viewRadius; chunkZ++)
					{
						LoadChunk(FIntVector(chunkX, chunkY, chunkZ));
					}
				}
			}
		}
	}

	TArray<FIntVector> chunkKeys;
	chunkMap.GetKeys(chunkKeys);
	for (int i = 0; i < chunkKeys.Num(); i++)
	{
		bool bNeedsCull = true;

		for (int x = 0; x < playerPawns.Num(); x++)
		{
			if (playerPawns[x] != nullptr)
			{
				FIntVector playerPos;
				playerPos.X = FMath::RoundFromZero((float)playerPawns[x]->GetActorLocation().X / 1000);
				playerPos.Y = FMath::RoundFromZero((float)playerPawns[x]->GetActorLocation().Y / 1000);
				playerPos.Z = FMath::RoundFromZero((float)playerPawns[x]->GetActorLocation().Z / 1000);
				playerPos = playerPos - chunkKeys[i];
				if (FMath::Abs(playerPos.X) < cullRadius && FMath::Abs(playerPos.Y) < cullRadius && FMath::Abs(playerPos.Z) < cullRadius)
				{
					bNeedsCull = false;
				}
			}
		}
		if (playerPawns.Num() == 0)
			bNeedsCull = false;

		if (bNeedsCull)
		{
			UnloadChunk(chunkKeys[i]);
		}
	}
}

bool AWorld_World::NeighborsReady(FIntVector chunkPos)
{
	if (chunkPos.X + 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X + 2, chunkPos.Y, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X + 2, chunkPos.Y, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X + 1, chunkPos.Y, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X + 1, chunkPos.Y, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}

	if (chunkPos.X - 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X - 2, chunkPos.Y, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X - 2, chunkPos.Y, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X - 1, chunkPos.Y, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X - 1, chunkPos.Y, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	
	if (chunkPos.Y + 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y + 2, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y + 2, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y + 1, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y + 1, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	
	if (chunkPos.Y - 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y - 2, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y - 2, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y - 1, chunkPos.Z)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y - 1, chunkPos.Z)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	
	if (chunkPos.Z + 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z + 2)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z + 2)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z + 1)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z + 1)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	
	if (chunkPos.Z - 1 == 0)
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z - 2)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z - 2)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}
	else
	{
		if (chunkMap.Contains(FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z - 1)))
		{
			if (chunkMap[FIntVector(chunkPos.X, chunkPos.Y, chunkPos.Z - 1)]->bNeedsBase)
				return false;
		}
		else
			return false;
	}

	return true;
}