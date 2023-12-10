// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdsController.h"

#include "BirdExterminator/World/Bird.h"
#include "BirdExterminator/World/BirdFlock.h"

// Sets default values
ABirdsController::ABirdsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABirdsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABirdsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABirdsController::Initialize(const FVector3f &PlayableAreaRef, const int &BirdFlockCount)
{
	BirdFlocksArray.Reserve(BirdFlockCount);
	PlayableArea = PlayableAreaRef;
	
	SpawnBirdFlocks(BirdFlockCount);

	IsInitialized = true;
}

AActor* ABirdsController::GetNearestBird(const FVector& LocationFrom, float &Distance)
{
	AActor* NearestActor = nullptr;
	float MinDistance = Distance = TNumericLimits<float>::Max();
	for (int i=0; i < BirdFlocksArray.Num(); ++i)
	{
		for (int j=0; j < BirdFlocksArray[i]->BirdArray.Num(); ++j)
		{
			const float Dist = (LocationFrom - BirdFlocksArray[i]->BirdArray[j]->GetActorLocation()).SizeSquared();
			if(MinDistance > Dist)
			{
				MinDistance = Dist;
				NearestActor = BirdFlocksArray[i]->BirdArray[j];
			}
		}
	}
	for (int i=0; i < FreeBirdsArray.Num(); ++i)
	{
		const float Dist = (LocationFrom - FreeBirdsArray[i]->GetActorLocation()).SizeSquared();
		if(MinDistance > Dist)
		{
			MinDistance = Dist;
			NearestActor = FreeBirdsArray[i];
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
	for (int i=0; i < PredatorsArray.Num(); ++i)
	{
		const float Dist = (LocationFrom - PredatorsArray[i]->GetActorLocation()).SizeSquared();
		if(MinDistance > Dist)
		{
			MinDistance = Dist;
			NearestActor = PredatorsArray[i];
		}
	}

	if(NearestActor)
		Distance = FVector::Dist(LocationFrom, NearestActor->GetActorLocation());
	
	return NearestActor;
}

void ABirdsController::SpawnBirdFlocks(const int& BirdFlocksCount)
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

		BirdFlocksArray.Add(BirdFlockSpawned);

		PreyBirdsAlive += BirdsToSpawn;
		
		BirdFlockSpawned->OnBirdInFlockDestroyedChanged.AddUObject(this, &ABirdsController::BirdInFlockDestroyed);
	}
}

void ABirdsController::RegisterAsFreeBird(ABird* Bird)
{
	FreeBirdsArray.Add(Bird);
	OnBirdCountChangedEvent.Broadcast(++PreyBirdsAlive);
	Bird->OnBirdDestroyedEvent.AddUObject(this, &ABirdsController::HandleBirdDestroyed);
}

void ABirdsController::RegisterAsPredatorBird(ABird* Bird)
{
	OnPredatorCountChangedEvent.Broadcast(--PredatorBirdsAvailable);
	OnPredatorAliveCountChangedEvent.Broadcast(++PredatorBirdsAlive);

	PredatorsArray.Add(Bird);
}

void ABirdsController::UnregisterPredator(ABird* Bird)
{
	OnPredatorAliveCountChangedEvent.Broadcast(--PredatorBirdsAlive);
	
	PredatorsArray.Remove(Bird);
}

void ABirdsController::HandleBirdDestroyed(ABird* Bird)
{
	Bird->OnBirdDestroyedEvent.RemoveAll(this);
	OnBirdCountChangedEvent.Broadcast(--PreyBirdsAlive);
	FreeBirdsArray.Remove(Bird);
}

void ABirdsController::SpawnPredatorBird(const FVector &SpawnLocation, const FRotator &SpawnRotator, const FActorSpawnParameters &SpawnParameters)
{
	if(PredatorBirdsAvailable <= 0)
		return;
	
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		auto ActorSpawned = World->SpawnActor(PredatorBirdClass, &SpawnLocation, &SpawnRotator, SpawnParameters);
		auto BirdSpawned = Cast<ABird>(ActorSpawned);
		BirdSpawned->InitializeCommonObjects(this);
		BirdSpawned->InitializeAsPredator(PlayableArea);
		RegisterAsPredatorBird(BirdSpawned);
	}
}

void ABirdsController::BirdInFlockDestroyed()
{
	OnBirdCountChangedEvent.Broadcast(--PreyBirdsAlive);
}

