// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Player/Pawn_Player.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "World_Chunk.h"
#include "Generation/Thread_GenerateBase.h"
#include "World_World.generated.h"

UCLASS()
class VOXEL_VXGI_API AWorld_World : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorld_World();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	Voxel_Voxel* GetBlock(FIntVector blockPos);
	void SetBlock(FIntVector blockPos, Voxel_Voxel*);

	bool LoadChunk(FIntVector chunkPos);
	bool UnloadChunk(FIntVector chunkPos);
	void ManageChunk();
	bool NeighborsReady(FIntVector);

	int seed;
	TArray<UStaticMesh*> renderMesh;
	TMap<FIntVector, AWorld_Chunk*> chunkMap;
	TArray<Thread_GenerateBase*> generateBaseThreads;
	TArray<APawn_Player*> playerPawns;
};
