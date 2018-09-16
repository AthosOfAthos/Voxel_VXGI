// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_Voxel.h"

Voxel_Voxel::Voxel_Voxel()
{
	//use stone mesh as default
	name = FName(TEXT("genericVoxel"));
	renderID = 1000;
	hardness = 50;
	bHasCollision = true;
	bDoesOcclude = true;
}

Voxel_Voxel::~Voxel_Voxel()
{
}
