// Fill out your copyright notice in the Description page of Project Settings.

#include "Deserts.h"

Deserts::Deserts()
{
	ground = FastNoise();
	ground.SetSeed(1337);
	ground.SetFrequency(0.05);
	ground.SetCellularJitter(1);
}

Deserts::~Deserts()
{
}

Voxel_Voxel* Deserts::Block(FIntVector pos) {
	float h = ground.GetCellular(pos.X, pos.Y);
	h *= 3;
	if (pos.Z < h) {
		return new Voxel_Stone();
	}
	return nullptr;
}
Voxel_Voxel* Deserts::SurfaceBlock(FIntVector pos) {
	return new Voxel_Sand();
}