// Fill out your copyright notice in the Description page of Project Settings.

#include "Plains.h"

Plains::Plains()
{
	ground = FastNoise();
	ground.SetSeed(1337);
	ground.SetFrequency(0.05);
}

Plains::~Plains()
{
}

Voxel_Voxel* Plains::Block(FIntVector pos) {
	float h = ground.GetPerlin(pos.X, pos.Y);
	h *= 5;
	if (pos.Z < h) {
		return new Voxel_Stone();
	}
	return nullptr;
}

Voxel_Voxel* Plains::SurfaceBlock(FIntVector pos) {
	return new Voxel_Grass();
}
