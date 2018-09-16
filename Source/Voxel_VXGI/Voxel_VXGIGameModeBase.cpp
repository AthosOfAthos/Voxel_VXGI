// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel_VXGIGameModeBase.h"

AVoxel_VXGIGameModeBase::AVoxel_VXGIGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bStartPlayersAsSpectators = true;

	DefaultPawnClass = APawn_Player::StaticClass();
	PlayerControllerClass = AController_Player::StaticClass();
}

void AVoxel_VXGIGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Spawn gameWorld
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	gameWorld = GetWorld()->SpawnActor<AWorld_World>(FVector(0, 0, 0), FRotator(0, 0, 0), SpawnInfo);
}

void AVoxel_VXGIGameModeBase::Tick(float DeltaTime)
{
	
}