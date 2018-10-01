// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Voxel/Voxel_Include.h"
#include "World_Chunk.generated.h"

UCLASS()
class VOXEL_VXGI_API AWorld_Chunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorld_Chunk();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	Voxel_Voxel* GetBlock(FIntVector blockPos);
	void SetVisibility(bool);
	void SetBlock(FIntVector blockPos, Voxel_Voxel*);
	void FindGenerationThread();
	void GenBlocks();
	void UpdateRenderData();
	bool IsOccluded(FIntVector);
	UFUNCTION()
	void OnRep_RenderData();
	void UpdateWorld();

	TArray<UInstancedStaticMeshComponent*> voxelMesh;

	bool bCurrentVisibility = true;
	bool bNeedsGeneration;
	bool bNeedsBase;
	bool bHasGenerationThread;
	int generationThreadID;
	UPROPERTY( VisibleAnywhere )
	FIntVector chunkPos;
	Voxel_Voxel* chunkData[1000];
	UPROPERTY(ReplicatedUsing = OnRep_RenderData)
	uint16 renderData[1000];
};
