// Fill out your copyright notice in the Description page of Project Settings.


#include "PredatorBehavior.h"

#include "BirdsController.h"
#include "BirdExterminator/GameBase/BirdExterminatorGameMode.h"
#include "Kismet/GameplayStatics.h"

PredatorBehavior::PredatorBehavior(AActor *ActorOwner) : BirdBehavior()
{
	GameMode = Cast<ABirdExterminatorGameMode>(UGameplayStatics::GetGameMode(ActorOwner));
	InitialVelocity = 700;
	TurnSpeed = 250;
}

PredatorBehavior::~PredatorBehavior()
{
}

FRotator PredatorBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	const auto NearestBird = GameMode->BirdsController->GetNearestBird(CurrentLocation);

	if(NearestBird)
	{
		return FMath::RInterpConstantTo(CurrentRotation,(NearestBird->GetActorLocation() - CurrentLocation).Rotation(), DeltaTime, TurnSpeed);
	}
	
	return CurrentRotation;
}