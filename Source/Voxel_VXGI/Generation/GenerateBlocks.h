// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel/Voxel_Include.h"
#include "Biome.h"
#include "Plains.h"
#include "Mountains.h"
#include "Swamps.h"
#include "Deserts.h"
#include "World_World.h"

class VOXEL_VXGI_API GenerateBlocks
{
public:
	GenerateBlocks(AWorld_World*);
	~GenerateBlocks();

	Voxel_Voxel* Gen(FIntVector);

	Biome biome = Biome(1);
	Plains plain = Plains();
	Mountains mountain = Mountains();
	Swamps swamp = Swamps();
	Deserts desert = Deserts();

	AWorld_World* world;
};
