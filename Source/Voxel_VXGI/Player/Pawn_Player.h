// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnrealNetwork.h"
#include "App.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Player.generated.h"

UCLASS()
class VOXEL_VXGI_API APawn_Player : public APawn
{
	GENERATED_BODY()

public:
	APawn_Player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CheckCollision(FVector);

	void MoveY(float);
	void MoveX(float);
	void LookY(float);
	void LookX(float);
	void JumpPressed();
	void JumpReleased();
	UFUNCTION(Server, Reliable, WithValidation)
	void MoveYServer(float input);
	UFUNCTION(Server, Reliable, WithValidation)
	void MoveXServer(float input);
	UFUNCTION(Server, Reliable, WithValidation)
	void SetRotation(FRotator newPlayerRotation);
	UFUNCTION(Server, Reliable, WithValidation)
	void JumpPressedServer();
	UFUNCTION(Server, Reliable, WithValidation)
		void JumpReleasedServer();

	UCapsuleComponent* playerCollision;
	UCameraComponent* playerCamera;

	FVector playerVelocity;
	FRotator playerRotation;
	UPROPERTY(Replicated)
	FVector playerInput;
	UPROPERTY(Replicated)
	FVector playerLocation;

	float gravityZ;
};
