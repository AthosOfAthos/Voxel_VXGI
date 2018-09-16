// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Math/FastNoise.h"
#include "Voxel/Voxel_Include.h"
#include "CoreMinimal.h"

/**
 *
 */
class VOXEL_VXGI_API Swamps
{
public:
	Swamps();
	~Swamps();
	FastNoise ground;
	Voxel_Voxel* Block(FIntVector);
};