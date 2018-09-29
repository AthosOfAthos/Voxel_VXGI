// Fill out your copyright notice in the Description page of Project Settings.

#include "Mountains.h"

Mountains::Mountains()
{
	ground = FastNoise();
	ground.SetSeed(1337);
	ground.SetFrequency(0.03);
}

Mountains::~Mountains()
{
}

Voxel_Voxel* Mountains::Block(FIntVector pos) {
	float h = ground.GetPerlin(pos.X, pos.Y);
	h *= 30;
	if (pos.Z < h) {
		return new Voxel_Stone();
	}
	return nullptr;
}
Voxel_Voxel* Mountains::SurfaceBlock(FIntVector pos) {
	return new Voxel_Stone();
}
