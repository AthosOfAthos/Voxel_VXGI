// Fill out your copyright notice in the Description page of Project Settings.

#include "World_Chunk.h"
#include "World_World.h"
#include "Generation/GenerateBlocks.h"
#include "Generation/GenerateDetails.h"

AWorld_Chunk::AWorld_Chunk()
{
	PrimaryActorTick.bCanEverTick = true;

	bHasGenerationThread = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Load static meshs
	voxelMesh.Init(nullptr, 2000);
	UInstancedStaticMeshComponent* tmpMesh;

	//Stone
	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Stone"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Stone.Voxel_Stone")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1000] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Grass"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Grass.Voxel_Grass")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1001] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Dirt"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Dirt.Voxel_Dirt")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1002] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Sand"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Sand.Voxel_Sand")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1003] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Rubble"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Rubble.Voxel_Rubble")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1004] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Wood"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Wood.Voxel_Wood")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1005] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Leaf"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Leaf.Voxel_Leaf")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1006] = tmpMesh;

	tmpMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Voxel Glow"));
	tmpMesh->SetStaticMesh(FindObject<UStaticMesh>(NULL, TEXT("/Game/Mesh/Voxel/Voxel_Glow.Voxel_Glow")));
	tmpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tmpMesh->SetupAttachment(RootComponent);
	voxelMesh[1100] = tmpMesh;
}

void AWorld_Chunk::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AWorld_Chunk, renderData);
}

void AWorld_Chunk::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		bNeedsGeneration = true;
		bNeedsBase = true;

		chunkPos.X = GetActorLocation().X / 1000;
		if (chunkPos.X >= 0)
			chunkPos.X++;
		chunkPos.Y = GetActorLocation().Y / 1000;
		if (chunkPos.Y >= 0)
			chunkPos.Y++;
		chunkPos.Z = GetActorLocation().Z / 1000;
		if (chunkPos.Z >= 0)
			chunkPos.Z++;
			
	}

	chunkPos.X = GetActorLocation().X / 1000;
	if (chunkPos.X >= 0)
		chunkPos.X++;
	chunkPos.Y = GetActorLocation().Y / 1000;
	if (chunkPos.Y >= 0)
		chunkPos.Y++;
	chunkPos.Z = GetActorLocation().Z / 1000;
	if (chunkPos.Z >= 0)
		chunkPos.Z++;
}

void AWorld_Chunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		//Server logic
		if (bNeedsGeneration)
		{
			if (bNeedsBase)
			{
				//Generate base blocks
				if (!bHasGenerationThread)
				{
					FindGenerationThread();
				}
				else
				{
					AWorld_World* world = (AWorld_World*)GetOwner();
					if (world->generateBaseThreads[generationThreadID]->bHasCompleted)
					{
						world->generateBaseThreads[generationThreadID]->Confirm();
						bNeedsBase = false;
						UpdateRenderData();
					}
				}
			}
			else
			{
				//Fill in detail
				AWorld_World* world = (AWorld_World*)GetOwner();
				if (world->NeighborsReady(chunkPos))
				{
					GenBlocks();
					bNeedsGeneration = false;
					UpdateRenderData();
					SetReplicates(true);
				}
			}
		}

		

	}
	else
	{
		//client logic
		FVector cameraLocation = FVector(0, 0, 0);
		if (GEngine)
			cameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
		cameraLocation /= 1000;
		FVector cameraDistance = FVector(chunkPos.X, chunkPos.Y, chunkPos.Z);
		cameraDistance = cameraDistance - cameraLocation;
		if (FMath::Abs(cameraDistance.X) > 4 || FMath::Abs(cameraDistance.Y) > 4 || FMath::Abs(cameraDistance.Z) > 4)
		{
			SetVisibility(false);
		}
		else
		{
			SetVisibility(true);
		}
	}
	//common logic
	
}

void AWorld_Chunk::SetVisibility(bool bNewVisibility)
{
	if (bNewVisibility != bCurrentVisibility)
	{
		bCurrentVisibility = bNewVisibility;
		this->SetActorHiddenInGame(!bCurrentVisibility);
	}
}

Voxel_Voxel* AWorld_Chunk::GetBlock(FIntVector blockPos)
{
	if (blockPos.X > 0)
		blockPos.X--;
	blockPos.X %= 10;
	if (blockPos.X < 0)
		blockPos.X += 10;
	if (blockPos.Y > 0)
		blockPos.Y--;
	blockPos.Y %= 10;
	if (blockPos.Y < 0)
		blockPos.Y += 10;
	if (blockPos.Z > 0)
		blockPos.Z--;
	blockPos.Z %= 10;
	if (blockPos.Z < 0)
		blockPos.Z += 10;

	return chunkData[blockPos.X + blockPos.Y * 10 + blockPos.Z * 100];
}

void AWorld_Chunk::SetBlock(FIntVector blockPos, Voxel_Voxel* voxel)
{
	if (blockPos.X > 0)
		blockPos.X--;
	blockPos.X %= 10;
	if (blockPos.X < 0)
		blockPos.X += 10;
	if (blockPos.Y > 0)
		blockPos.Y--;
	blockPos.Y %= 10;
	if (blockPos.Y < 0)
		blockPos.Y += 10;
	if (blockPos.Z > 0)
		blockPos.Z--;
	blockPos.Z %= 10;
	if (blockPos.Z < 0)
		blockPos.Z += 10;

	chunkData[blockPos.X + blockPos.Y * 10 + blockPos.Z * 100] = voxel;
	UpdateRenderData();
}

void AWorld_Chunk::FindGenerationThread()
{
	AWorld_World* world = (AWorld_World*)GetOwner();
	int totalThreads = world->generateBaseThreads.Num();
	for (int i = 0; i < totalThreads; i++)
	{
		if (world->generateBaseThreads[i]->Start(chunkData, chunkPos))
		{
			bHasGenerationThread = true;
			generationThreadID = i;
			return;
		}
	}

}

void AWorld_Chunk::GenBlocks()
{
	AWorld_World* world = (AWorld_World*)GetOwner();
	GenerateBlocks genBlocks = GenerateBlocks(world);
	GenerateDetails genDetails = GenerateDetails(world);

	for (int voxelX = 0; voxelX < 10; voxelX++)
	{
		for (int voxelY = 0; voxelY < 10; voxelY++)
		{
			for (int voxelZ = 0; voxelZ < 10; voxelZ++)
			{
				FIntVector blockPos = FIntVector(voxelX, voxelY, voxelZ);
				blockPos.X += chunkPos.X * 10;
				if (blockPos.X >= 0)
					blockPos.X -= 9;
				blockPos.Y += chunkPos.Y * 10;
				if (blockPos.Y >= 0)
					blockPos.Y -= 9;
				blockPos.Z += chunkPos.Z * 10;
				if (blockPos.Z >= 0)
					blockPos.Z -= 9;

				//Fix leak
				chunkData[voxelX + voxelY * 10 + voxelZ * 100] = genBlocks.Gen(blockPos);
			}
		}
	}

	for (int voxelX = 0; voxelX < 10; voxelX++)
	{
		for (int voxelY = 0; voxelY < 10; voxelY++)
		{
			for (int voxelZ = 0; voxelZ < 10; voxelZ++)
			{
				FIntVector blockPos = FIntVector(voxelX, voxelY, voxelZ);
				blockPos.X += chunkPos.X * 10;
				if (blockPos.X >= 0)
					blockPos.X -= 9;
				blockPos.Y += chunkPos.Y * 10;
				if (blockPos.Y >= 0)
					blockPos.Y -= 9;
				blockPos.Z += chunkPos.Z * 10;
				if (blockPos.Z >= 0)
					blockPos.Z -= 9;

				genDetails.Gen(blockPos);
			}
		}
	}

}

void AWorld_Chunk::UpdateRenderData()
{
	for (int voxelX = 0; voxelX < 10; voxelX++)
	{
		for (int voxelY = 0; voxelY < 10; voxelY++)
		{
			for (int voxelZ = 0; voxelZ < 10; voxelZ++)
			{
				if (chunkData[voxelX + voxelY * 10 + voxelZ * 100] != nullptr)
				{
					if (!IsOccluded(FIntVector(voxelX, voxelY, voxelZ)))
						renderData[voxelX + voxelY * 10 + voxelZ * 100] = chunkData[voxelX + voxelY * 10 + voxelZ * 100]->renderID;
					else
						renderData[voxelX + voxelY * 10 + voxelZ * 100] = 0;
				}
				else
				{
					renderData[voxelX + voxelY * 10 + voxelZ * 100] = 0;
				}
			}
		}
	}

	UpdateWorld();
}

bool AWorld_Chunk::IsOccluded(FIntVector blockPos)
{
	if (blockPos.X == 0 || blockPos.X == 9 || blockPos.Y == 0 || blockPos.Y == 9 || blockPos.Z == 0 || blockPos.Z == 9)
		return false;

	if (chunkData[(blockPos.X + 1) + blockPos.Y * 10 + blockPos.Z * 100] != nullptr)
	{
		if (!chunkData[(blockPos.X + 1) + blockPos.Y * 10 + blockPos.Z * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;
		
	if (chunkData[(blockPos.X - 1) + blockPos.Y * 10 + blockPos.Z * 100] != nullptr)
	{
		if (!chunkData[(blockPos.X - 1) + blockPos.Y * 10 + blockPos.Z * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;

	if (chunkData[blockPos.X + (blockPos.Y + 1) * 10 + blockPos.Z * 100] != nullptr)
	{
		if (!chunkData[blockPos.X + (blockPos.Y + 1) * 10 + blockPos.Z * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;

	if (chunkData[blockPos.X + (blockPos.Y - 1) * 10 + blockPos.Z * 100] != nullptr)
	{
		if (!chunkData[blockPos.X + (blockPos.Y - 1) * 10 + blockPos.Z * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;

	if (chunkData[blockPos.X + blockPos.Y * 10 + (blockPos.Z + 1) * 100] != nullptr)
	{
		if (!chunkData[blockPos.X + blockPos.Y * 10 + (blockPos.Z + 1) * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;

	if (chunkData[blockPos.X + blockPos.Y * 10 + (blockPos.Z - 1) * 100] != nullptr)
	{
		if (!chunkData[blockPos.X + blockPos.Y * 10 + (blockPos.Z - 1) * 100]->bDoesOcclude)
			return false;
	}
	else
		return false;

	return true;
}

void AWorld_Chunk::OnRep_RenderData()
{
	UpdateWorld();
}

void AWorld_Chunk::UpdateWorld()
{
	for (int i = 0; i < 2000; i++)
	{
		if (voxelMesh[i] != nullptr)
			voxelMesh[i]->ClearInstances();
	}

	for (int voxelX = 0; voxelX < 10; voxelX++)
	{
		for (int voxelY = 0; voxelY < 10; voxelY++)
		{
			for (int voxelZ = 0; voxelZ < 10; voxelZ++)
			{
				int i = renderData[voxelX + voxelY * 10 + voxelZ * 100];
				if (voxelMesh[i] != nullptr)
					voxelMesh[i]->AddInstance(FTransform(FRotator(0, 0, 0), FVector(voxelX * 100, voxelY * 100, voxelZ * 100), FVector(1, 1, 1)));
			}
		}
	}
}