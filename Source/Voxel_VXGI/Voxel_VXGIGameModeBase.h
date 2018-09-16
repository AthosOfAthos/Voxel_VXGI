// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "World_World.h"
#include "Player/Pawn_Player.h"
#include "Player/Controller_Player.h"
#include "Voxel_VXGIGameModeBase.generated.h"

UCLASS()
class VOXEL_VXGI_API AVoxel_VXGIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVoxel_VXGIGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	AWorld_World* gameWorld;
};
