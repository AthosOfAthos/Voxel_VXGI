// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn_Player.h"
#include "Voxel_VXGIGameModeBase.h"
#include "World_World.h"

APawn_Player::APawn_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
	bReplicates = true;
	bReplicateMovement = true;
	NetPriority = 5;

	playerCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("playerCollision"));
	RootComponent = playerCollision;

	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamera"));
	playerCamera->SetupAttachment(RootComponent);

	playerVelocity = FVector(0, 0, 0);
	playerInput = FVector(0, 0, 0);
}

void APawn_Player::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APawn_Player, playerInput);
	DOREPLIFETIME(APawn_Player, playerLocation);
}

void APawn_Player::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		playerLocation = GetActorLocation();
		playerRotation = GetActorRotation();
	}
}

void APawn_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		AVoxel_VXGIGameModeBase* gm = (AVoxel_VXGIGameModeBase*)GetWorld()->GetAuthGameMode();
		AWorld_World* world = gm->gameWorld;

		FVector rotatedInput = FRotator(0, playerRotation.Yaw, 0).RotateVector(playerInput);

		playerVelocity.X = rotatedInput.X * 500;
		playerVelocity.Y = rotatedInput.Y * 500;

		FIntVector blockPos;
		FVector targetLocation = GetActorLocation();

		targetLocation.X += playerVelocity.X * DeltaTime;
		blockPos.X = FMath::RoundFromZero((float)targetLocation.X / 100);
		blockPos.Y = FMath::RoundFromZero((float)targetLocation.Y / 100);
		blockPos.Z = FMath::RoundFromZero((float)targetLocation.Z / 100);
		if (world->GetBlock(blockPos) != nullptr)
		{
			if (!world->GetBlock(blockPos)->bHasCollision)
				SetActorLocation(targetLocation);
		}
		else
		{
			SetActorLocation(targetLocation);
		}

		targetLocation = GetActorLocation();
		targetLocation.Y += playerVelocity.Y * DeltaTime;
		blockPos.X = FMath::RoundFromZero((float)targetLocation.X / 100);
		blockPos.Y = FMath::RoundFromZero((float)targetLocation.Y / 100);
		blockPos.Z = FMath::RoundFromZero((float)targetLocation.Z / 100);
		if (world->GetBlock(blockPos) != nullptr)
		{
			if (!world->GetBlock(blockPos)->bHasCollision)
				SetActorLocation(targetLocation);
		}
		else
		{
			SetActorLocation(targetLocation);
		}
		
		//update location for clients
		playerLocation = GetActorLocation();
	}
	else
	{
		//update player rotation on server
		SetRotation(playerRotation);
		playerCamera->SetWorldRotation(playerRotation);
		//smooth movement
		FVector targetLocation = FMath::VInterpTo(GetActorLocation(), playerLocation, DeltaTime, 10);
		SetActorLocation(targetLocation);
	}
}

void APawn_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveY", this, &APawn_Player::MoveY);
	PlayerInputComponent->BindAxis("MoveX", this, &APawn_Player::MoveX);
	PlayerInputComponent->BindAxis("LookY", this, &APawn_Player::LookY);
	PlayerInputComponent->BindAxis("LookX", this, &APawn_Player::LookX);
}

void APawn_Player::MoveY(float input)
{
	MoveYServer(input);
}

void APawn_Player::MoveX(float input)
{
	MoveXServer(input);
}

void APawn_Player::LookY(float input)
{
	playerRotation.Pitch += input * FApp::GetDeltaTime() * 80;
	playerRotation.Pitch = FMath::Clamp(playerRotation.Pitch, (float)-90, (float)90);
}

void APawn_Player::LookX(float input)
{
	playerRotation.Yaw += input * FApp::GetDeltaTime() * 80;
}

bool APawn_Player::MoveYServer_Validate(float input)
{
	return true;
}

void APawn_Player::MoveYServer_Implementation(float input)
{
	playerInput.X = input;
}

bool APawn_Player::MoveXServer_Validate(float input)
{
	return true;
}

void APawn_Player::MoveXServer_Implementation(float input)
{
	playerInput.Y = input;
}

bool APawn_Player::SetRotation_Validate(FRotator newPlayerRotation)
{
	return true;
}

void APawn_Player::SetRotation_Implementation(FRotator newPlayerRotation)
{
	playerRotation = newPlayerRotation;
}