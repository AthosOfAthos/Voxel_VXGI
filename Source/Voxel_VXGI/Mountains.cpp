// Fill out your copyright notice in the Description page of Project Settings.

#include "Mountains.h"

Mountains::Mountains()
{
	ground = FastNoise();
	ground.SetSeed(1337);
	ground.SetFrequency(0.05);
}

Mountains::~Mountains()
{
}

Voxel_Voxel* Mountains::Block(FIntVector pos) {
	float h = ground.GetPerlin(pos.X, pos.Y);
	h *= 20;
	if (pos.Z < h) {
		return new Voxel_Stone();
	}
	return nullptr;
}
