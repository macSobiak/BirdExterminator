// Fill out your copyright notice in the Description page of Project Settings.


#include "PredatorBehavior.h"

#include "BirdsController.h"
#include "../World/Bird.h"
#include "BirdExterminator/GameBase/BirdExterminatorGameMode.h"
#include "Kismet/GameplayStatics.h"

PredatorBehavior::PredatorBehavior(AActor *ActorOwner, const FVector3f& PlayableAreaRef) : BirdBehavior(PlayableAreaRef)
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
	//if no prey left -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return CurrentRotation;
}

bool PredatorBehavior::HandleBirdHit(AActor* ActorHit)
{
	ABird* BirdActor = Cast<ABird>(ActorHit);
	if(BirdActor && BirdActor->IsDestructable)
	{
		ActorHit->Destroy();
		return true;
	}
	
	return false;
}
