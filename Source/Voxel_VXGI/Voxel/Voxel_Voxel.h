// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class VOXEL_VXGI_API Voxel_Voxel
{
public:
	Voxel_Voxel();
	~Voxel_Voxel();

	FName name;
	//the instanced static mesh to use see World_Chunk
	uint16 renderID;
	//Will voxel occlude those behind it
	bool bDoesOcclude;
	//does voxel block objects
	bool bHasCollision;
	//how hard the voxel is with 0 being softed and 65,535 being max hardness
	uint16 hardness;
};
