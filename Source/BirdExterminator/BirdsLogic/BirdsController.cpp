// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdsController.h"

#include "BirdExterminator/World/Bird.h"
#include "BirdExterminator/World/BirdFlock.h"

// Sets default values
ABirdsController::ABirdsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Script/Engine.Material'/Game/Materials/M_Prey.M_Prey'"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterialPrey = FoundMaterial.Object;
	}

	FoundMaterial = (TEXT("/Script/Engine.Material'/Game/Materials/M_Predator.M_Predator'"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterialPredator = FoundMaterial.Object;
	}
}

void ABirdsController::Destroyed()
{
	Super::Destroyed();

	OnBirdCountChangedEvent.Clear();
	OnPredatorCountChangedEvent.Clear();
	OnPredatorAliveCountChangedEvent.Clear();
}

void ABirdsController::Initialize(const FVector3f &PlayableAreaRef, const uint16 &BirdFlockCount, const uint16 &PredatorBirdsAmmo)
{
	PredatorBirdsAvailable = PredatorBirdsAmmo;
	
	BirdFlocksVector.reserve(BirdFlockCount);
	PredatorsVector.reserve(PredatorBirdsAvailable);
	FreeBirdsVector.reserve(PredatorBirdsAvailable);
	
	PlayableArea = PlayableAreaRef;
	
	SpawnBirdFlocks(BirdFlockCount);

	IsInitialized = true;
}

AActor* ABirdsController::GetNearestBird(const FVector& LocationFrom, float &Distance)
{
	AActor* NearestActor = nullptr;
	float MinDistance = Distance = TNumericLimits<float>::Max();
	for (int i=0; i < BirdFlocksVector.size(); ++i)
	{
		for (int j=0; j < BirdFlocksVector[i]->BirdVector.size(); ++j)
		{
			if(const float Dist = (LocationFrom - BirdFlocksVector[i]->BirdVector[j]->GetActorLocation()).SizeSquared(); MinDistance > Dist)
			{
				MinDistance = Dist;
				NearestActor = BirdFlocksVector[i]->BirdVector[j];
			}
		}
	}
	for (int i=0; i < FreeBirdsVector.size(); ++i)
	{
		if(const float Dist = (LocationFrom - FreeBirdsVector[i]->GetActorLocation()).SizeSquared(); MinDistance > Dist)
		{
			MinDistance = Dist;
			NearestActor = FreeBirdsVector[i];
		}
	}

	if(NearestActor)
		Distance = FVector::Dist(LocationFrom, NearestActor->GetActorLocation());
	
	return NearestActor;
}

AActor* ABirdsController::GetNearestPredator(const FVector& LocationFrom, float& Distance)
{
	AActor* NearestActor = nullptr;
	float MinDistance = Distance = TNumericLimits<float>::Max();

	for (int i=0; i < PredatorsVector.size(); ++i)
	{
		if(const float Dist = (LocationFrom - PredatorsVector[i]->GetActorLocation()).SizeSquared(); MinDistance > Dist)
		{
			MinDistance = Dist;
			NearestActor = PredatorsVector[i];
		}
	}

	if(NearestActor)
		Distance = FVector::Dist(LocationFrom, NearestActor->GetActorLocation());
	
	return NearestActor;
}

inline void ABirdsController::SpawnBirdFlocks(const int& BirdFlocksCount)
{
	FVector RandomSpawnPoint;
	for(int i =0; i < BirdFlocksCount; ++i)
	{
		RandomSpawnPoint.X = FMath::RandRange(-PlayableArea.X / 2, PlayableArea.X / 2);
		RandomSpawnPoint.Y = FMath::RandRange(-PlayableArea.Y / 2, PlayableArea.Y / 2);
		RandomSpawnPoint.Z = FMath::RandRange(PlayableArea.Z/2, PlayableArea.Z);
		
		auto BirdFlockSpawned = Cast<ABirdFlock>(GetWorld()->SpawnActor(BirdFlockBlueprint, &RandomSpawnPoint));
		int BirdsToSpawn = FMath::RandRange(5,8);
		BirdFlockSpawned->Initialize(PlayableArea, BirdsToSpawn, this);

		BirdFlocksVector.push_back(BirdFlockSpawned);

		PreyBirdsAlive += BirdsToSpawn;

		BirdFlockSpawned->OnDestroyed.AddDynamic(this, &ABirdsController::HandleBirdFlockDestroyed);
		BirdFlockSpawned->OnBirdInFlockDestroyedChanged.AddUObject(this, &ABirdsController::BirdInFlockDestroyed);
	}
}

void ABirdsController::RegisterAsFreeBird(ABird* Bird)
{
	FreeBirdsVector.push_back(Bird);
	OnBirdCountChangedEvent.Broadcast(++PreyBirdsAlive);
	Bird->OnBirdDestroyedEvent.AddUObject(this, &ABirdsController::HandleBirdDestroyed);
}

inline void ABirdsController::RegisterAsPredatorBird(ABird* Bird)
{
	OnPredatorAliveCountChangedEvent.Broadcast(++PredatorBirdsAlive);
	PredatorsVector.push_back(Bird);
	
	OnPredatorCountChangedEvent.Broadcast(--PredatorBirdsAvailable);
}

void ABirdsController::UnregisterPredator(ABird* Bird)
{
	OnPredatorAliveCountChangedEvent.Broadcast(--PredatorBirdsAlive);
	std::erase(PredatorsVector, Bird);
}

void ABirdsController::HandleBirdDestroyed(ABird* Bird)
{
	Bird->OnBirdDestroyedEvent.RemoveAll(this);
	OnBirdCountChangedEvent.Broadcast(--PreyBirdsAlive);
	std::erase(FreeBirdsVector, Bird);
}

void ABirdsController::HandleBirdFlockDestroyed(AActor* Actor)
{
	const auto BirdFlock = Cast<ABirdFlock>(Actor);
	BirdFlock->OnDestroyed.RemoveAll(this);
	BirdFlock->OnBirdInFlockDestroyedChanged.RemoveAll(this);

	std::erase(BirdFlocksVector, BirdFlock);
}

void ABirdsController::SpawnPredatorBird(const FVector &SpawnLocation, const FRotator &SpawnRotator, const FActorSpawnParameters &SpawnParameters)
{
	if(PredatorBirdsAvailable <= 0)
		return;
	
	const auto BirdSpawned = Cast<ABird>(GetWorld()->SpawnActor(PredatorBirdClass, &SpawnLocation, &SpawnRotator, SpawnParameters));
	BirdSpawned->InitializeCommonObjects(this);
	BirdSpawned->InitializeAsPredator(PlayableArea);
	RegisterAsPredatorBird(BirdSpawned);
}

void ABirdsController::BirdInFlockDestroyed()
{
	OnBirdCountChangedEvent.Broadcast(--PreyBirdsAlive);
}

