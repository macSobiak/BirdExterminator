// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBehavior.h"
#include "../World/BirdFlock.h"

PreyBehavior::PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef) : BirdBehavior()
{
	PlayableArea = PlayableAreaRef;
	BirdFlockToFollow = BirdFlock;
	PlaceInFlock = PlaceInFlockRef;
}

PreyBehavior::~PreyBehavior()
{
}

FRotator PreyBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	//If bird has a bird flock to follow, check its position and head to this direction
	if(BirdFlockToFollow)
	{
		return GetRotationToBirdFlock(DeltaTime, CurrentLocation, CurrentRotation);
	}

	//if no bird flock (for example its a lone bird transformed from a predator) fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}

	return CurrentRotation;
}

FRotator PreyBehavior::GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const
{
	return FMath::RInterpConstantTo(CurrentRotation,
		FVector(-CurrentLocation.X, -CurrentLocation.Y, (PlayableArea.Z / 2) - CurrentLocation.Z).Rotation(),
		DeltaTime, TurnSpeed);
}

FRotator PreyBehavior::GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const
{
	return FMath::RInterpConstantTo(CurrentRotation,
		(BirdFlockToFollow->GetPlaceInFlock(PlaceInFlock) - CurrentLocation).Rotation(),
		DeltaTime, TurnSpeed);
}

bool PreyBehavior::GetIsOutOfBounds(const FVector& CurrentLocation) const
{
	return CurrentLocation.X < -PlayableArea.X / 2 || CurrentLocation.X > PlayableArea.X / 2
		|| CurrentLocation.Y < -PlayableArea.Y / 2 || CurrentLocation.Y > PlayableArea.Y / 2
		|| CurrentLocation.Z > PlayableArea.Z;
}