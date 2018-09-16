// Fill out your copyright notice in the Description page of Project Settings.

#include "Thread_GenerateBase.h"
#include "Generation/GenerateBase.h"

Thread_GenerateBase::Thread_GenerateBase(int worldSeed)
{
	seed = worldSeed;

	bIsActive = false;
	bHasCompleted = false;
}

Thread_GenerateBase::~Thread_GenerateBase()
{
}

bool Thread_GenerateBase::Init()
{
	return true;
}

uint32 Thread_GenerateBase::Run()
{
	GenerateBase generator = GenerateBase(seed);
	while (true)
	{
		if (bIsActive && !bHasCompleted)
		{
			//Generate terrain
			for (int voxelX = 0; voxelX < 10; voxelX++)
			{
				for (int voxelY = 0; voxelY < 10; voxelY++)
				{
					for (int voxelZ = 0; voxelZ < 10; voxelZ++)
					{
						FIntVector blockPos = FIntVector(voxelX, voxelY, voxelZ);
						blockPos.X += chunkPos.X * 10;
						if (blockPos.X > 0)
							blockPos.X -= 9;
						blockPos.Y += chunkPos.Y * 10;
						if (blockPos.Y > 0)
							blockPos.Y -= 9;
						blockPos.Z += chunkPos.Z * 10;
						if (blockPos.Z > 0)
							blockPos.Z -= 9;
						chunkData[voxelX + voxelY * 10 + voxelZ * 100] = generator.Gen(blockPos);
					}
				}
			}

			bHasCompleted = true;
		}
		else
		{
			//Await task
			FPlatformProcess::Sleep(0.05);
		}
	}
	return 0;
}

bool Thread_GenerateBase::Start(Voxel_Voxel** refChunkData, FIntVector refChunkPos)
{
	if (!bIsActive)
	{
		chunkData = refChunkData;
		chunkPos = refChunkPos;

		bIsActive = true;
		bHasCompleted = false;
		return true;
	}
	return false;
}

void Thread_GenerateBase::Confirm()
{
	bIsActive = false;
}

void Thread_GenerateBase::Stop()
{

}
