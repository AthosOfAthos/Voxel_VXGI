// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Runnable.h"
#include "Voxel/Voxel_Voxel.h"

class VOXEL_VXGI_API Thread_GenerateBase : public FRunnable
{
public:
	Thread_GenerateBase(int worldSeed);
	~Thread_GenerateBase();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	bool Start(Voxel_Voxel** refChunkData, FIntVector refChunkPos);
	void Confirm();

	int seed;
	Voxel_Voxel** chunkData;
	FIntVector chunkPos;
	bool bIsActive;
	bool bHasCompleted;
};
