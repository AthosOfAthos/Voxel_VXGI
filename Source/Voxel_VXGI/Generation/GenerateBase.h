// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/FastNoise.h"
#include "Biome.h"
#include "Plains.h"
#include "Mountains.h"
#include "Swamps.h"
#include "Deserts.h"
#include "Voxel/Voxel_Voxel.h"

class VOXEL_VXGI_API GenerateBase
{
public:
	GenerateBase(int worldSeed);
	~GenerateBase();
	Voxel_Voxel* Gen(FIntVector);

	int seed;
	FastNoise simpleNoise;
	Biome biome = Biome(1);
	Plains plain = Plains();
	Mountains mountain = Mountains();
	Swamps swamp = Swamps();
	Deserts desert = Deserts();
};
