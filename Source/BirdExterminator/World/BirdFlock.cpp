// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdFlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bird.h"

void ABirdFlock::GenerateNewDestinationPoint()
{
	FlockFlightDestination = FVector(FMath::RandRange(-PlayableArea.X / 2, PlayableArea.X / 2),
	FMath::RandRange(-PlayableArea.Y / 2, PlayableArea.Y / 2),
	FMath::RandRange(10.0, PlayableArea.Z));
}

// Sets default values
ABirdFlock::ABirdFlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABirdFlock::SpawnBirdFlock()
{
	float Offset = 200;
	FVector SpawnPositionOffset = FVector::Zero();

	float Angle = 360 / (InitialBirdCount - 1);
	for (int i = 0; i < InitialBirdCount; ++i)
	{
		if(i != 0)
		{
			SpawnPositionOffset.Y = 200 * FMath::Cos(FMath::DegreesToRadians(Angle*(i-1))) +  FMath::RandRange(-10, 10);
			SpawnPositionOffset.X = 200 * FMath::Sin(FMath::DegreesToRadians(Angle*(i-1))) +  FMath::RandRange(-10, 10);
		}

		SpawnPositionOffset.Z = FMath::RandRange(-200, 200);

		auto SpawnPosition = GetActorLocation() + SpawnPositionOffset;
		const auto BirdSpawned = Cast<ABird>(GetWorld()->SpawnActor(BirdBlueprint, &SpawnPosition));
		PlaceInFlockMap.Add(i, SpawnPositionOffset + FVector(0,10,0));
		
		BirdSpawned->Initialize(this, i, PlayableArea);

	}
}

void ABirdFlock::Initialize(FVector &PlayableAreaRef)
{
	PlayableArea = PlayableAreaRef;
	SetActorRotation(FRotator(FMath::RandRange(0, 360) ,FMath::RandRange(0, 360), FMath::RandRange(0, 360)));
	SpawnBirdFlock();

	GenerateNewDestinationPoint();

	IsInitialized = true;
}

FVector ABirdFlock::GetPlaceInFlock(const int& PlaceInFlock)
{
	
	return GetActorLocation() + PlaceInFlockMap[PlaceInFlock];
}

// Called when the game starts or when spawned
void ABirdFlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABirdFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsInitialized)
		return;

	
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), FlockFlightDestination, DeltaTime, 500));

	if(GetActorLocation() == FlockFlightDestination)
	{
		GenerateNewDestinationPoint();
	}

	if(GetActorLocation().X < -PlayableArea.X/2 || GetActorLocation().X > PlayableArea.X/2
	|| GetActorLocation().Y < -PlayableArea.Y/2 || GetActorLocation().Y > PlayableArea.Y/2
	|| GetActorLocation().Z > PlayableArea.Z || GetActorLocation().Z < 10)
	{

		SetActorRotation(GetActorRotation() * -1);
	}

}
