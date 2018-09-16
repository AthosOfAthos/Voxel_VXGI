// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel/Voxel_Include.h"
#include "World_World.h"

class VOXEL_VXGI_API GenerateBlocks
{
public:
	GenerateBlocks(AWorld_World*);
	~GenerateBlocks();

	Voxel_Voxel* Gen(FIntVector);

	AWorld_World* world;
};
