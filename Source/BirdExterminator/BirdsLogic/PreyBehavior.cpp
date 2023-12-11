// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBehavior.h"
#include "BirdsController.h"
#include "../World/Bird.h"
#include "../World/BirdFlock.h"

PreyBehavior::PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef, ABird* Owner) : BirdBehavior(PlayableAreaRef, Owner)
{
	BirdFlockToFollow = BirdFlock;
	PlaceInFlock = PlaceInFlockRef;
}

PreyBehavior::~PreyBehavior()
{
	BirdFlockToFollow = nullptr;
}

inline FRotator PreyBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	float NearestDistance;

	//prioritize avioding predators
	if(const auto &NearestPredator = BirdOwner->BirdsController->GetNearestPredator(CurrentLocation, NearestDistance); NearestPredator)
	{
		//if flying away from predator and out of bounds -> turn back
		if(GetIsOutOfBounds(CurrentLocation))
		{
			return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
		}
		//fly away from predator
		if(NearestDistance < DangerDistance)
		{
			return FMath::RInterpConstantTo(CurrentRotation,((NearestPredator->GetActorLocation() - CurrentLocation).Rotation()) * -1, DeltaTime, GetTurnSpeed());
		}
	}
	
	//If bird has a bird flock to follow, check its position and head to this direction
	if(BirdFlockToFollow)
	{
		return GetRotationToBirdFlock(DeltaTime, CurrentLocation, CurrentRotation);
	}

	//if no bird flock (for example its a lone bird transformed from a predator) -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}

	return CurrentRotation;
}

inline bool PreyBehavior::HandleBirdHit(AActor* ActorHit)
{
	return false;
}

inline FRotator PreyBehavior::GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const
{
	return FMath::RInterpConstantTo(CurrentRotation,
		(BirdFlockToFollow->GetPlaceInFlock(PlaceInFlock) - CurrentLocation).Rotation(),
		DeltaTime, TurnSpeed);
}
