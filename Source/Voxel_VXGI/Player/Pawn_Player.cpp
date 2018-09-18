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
	playerCamera->SetRelativeLocation(FVector(0,0,75));

	playerVelocity = FVector(0, 0, 0);
	playerInput = FVector(0, 0, 0);

	gravityZ = -2000;
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

		FVector targetLocation = GetActorLocation();
		targetLocation.X += playerVelocity.X * DeltaTime;
		if (CheckCollision(targetLocation))
			SetActorLocation(targetLocation);

		targetLocation = GetActorLocation();
		targetLocation.Y += playerVelocity.Y * DeltaTime;
		if (CheckCollision(targetLocation))
			SetActorLocation(targetLocation);

		targetLocation = GetActorLocation();
		playerVelocity.Z += gravityZ * DeltaTime;
		targetLocation.Z += playerVelocity.Z * DeltaTime;
		if (CheckCollision(targetLocation))
			SetActorLocation(targetLocation);
		else
			playerVelocity.Z = 0;
		
		//update location for clients
		playerLocation = GetActorLocation();
	}
	else
	{
		//update player rotation on server
		SetRotation(playerRotation);
		playerCamera->SetWorldRotation(playerRotation);
		//smooth movement
		FVector targetLocation = FMath::VInterpTo(GetActorLocation(), playerLocation, DeltaTime, 15);
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APawn_Player::JumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APawn_Player::JumpReleased);
}

bool APawn_Player::CheckCollision(FVector location)
{
	AVoxel_VXGIGameModeBase* gm = (AVoxel_VXGIGameModeBase*)GetWorld()->GetAuthGameMode();
	AWorld_World* world = gm->gameWorld;

	FIntVector blockPos;
	blockPos.X = FMath::RoundFromZero((float)(location.X + 25) / 100);
	blockPos.Y = FMath::RoundFromZero((float)location.Y / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z - 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)(location.X - 25) / 100);
	blockPos.Y = FMath::RoundFromZero((float)location.Y / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z - 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)location.X / 100);
	blockPos.Y = FMath::RoundFromZero((float)(location.Y + 25) / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z - 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)location.X / 100);
	blockPos.Y = FMath::RoundFromZero((float)(location.Y - 25) / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z - 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	

	blockPos.X = FMath::RoundFromZero((float)(location.X + 25) / 100);
	blockPos.Y = FMath::RoundFromZero((float)location.Y / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z + 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)(location.X - 25) / 100);
	blockPos.Y = FMath::RoundFromZero((float)location.Y / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z + 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)location.X / 100);
	blockPos.Y = FMath::RoundFromZero((float)(location.Y + 25) / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z + 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	blockPos.X = FMath::RoundFromZero((float)location.X / 100);
	blockPos.Y = FMath::RoundFromZero((float)(location.Y - 25) / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z + 75) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			return false;
	}

	return true;
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

void APawn_Player::JumpPressed()
{
	JumpPressedServer();
}

void APawn_Player::JumpReleased()
{
	JumpReleasedServer();
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

bool APawn_Player::JumpPressedServer_Validate()
{
	return true;
}

void APawn_Player::JumpPressedServer_Implementation()
{
	AVoxel_VXGIGameModeBase* gm = (AVoxel_VXGIGameModeBase*)GetWorld()->GetAuthGameMode();
	AWorld_World* world = gm->gameWorld;

	FVector location = GetActorLocation();
	FIntVector blockPos;
	blockPos.X = FMath::RoundFromZero((float)location.X / 100);
	blockPos.Y = FMath::RoundFromZero((float)location.Y / 100);
	blockPos.Z = FMath::RoundFromZero((float)(location.Z - 100) / 100);
	if (world->GetBlock(blockPos) != nullptr)
	{
		if (world->GetBlock(blockPos)->bHasCollision)
			playerVelocity.Z = 1000;
	}
}

bool APawn_Player::JumpReleasedServer_Validate()
{
	return true;
}

void APawn_Player::JumpReleasedServer_Implementation()
{

}