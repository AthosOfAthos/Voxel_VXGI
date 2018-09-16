// Fill out your copyright notice in the Description page of Project Settings.

#include "Swamps.h"

Swamps::Swamps()
{
	ground = FastNoise();
	ground.SetSeed(1337);
	ground.SetFrequency(0.1);
}

Swamps::~Swamps()
{
}

Voxel_Voxel* Swamps::Block(FIntVector pos) {
	float h = ground.GetPerlin(pos.X, pos.Y);
	h *= 2;
	h -= 1;
	if (pos.Z < h) {
		return new Voxel_Stone();
	}
	return nullptr;
}
