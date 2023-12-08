// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdBehavior.h"
#include "../World/Bird.h"
#include "../World/BirdFlock.h"

BirdBehavior::BirdBehavior()
{
}

BirdBehavior::~BirdBehavior()
{
}

PreyBehavior::PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef) : BirdBehavior()
{
	PlayableArea = PlayableAreaRef;
	BirdFlockToFollow = BirdFlock;
	PlaceInFlock = PlaceInFlockRef;
}

PreyBehavior::~PreyBehavior()
{
}

PredatorBehavior::PredatorBehavior() : BirdBehavior()
{
}

PredatorBehavior::~PredatorBehavior()
{
}

FRotator PreyBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return GetRotationToBirdFlock(DeltaTime, CurrentLocation, CurrentRotation);
}

FRotator PreyBehavior::GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	auto NewDir = FVector(0, 0, PlayableArea.Z / 2) - CurrentLocation;
	auto TargetRotation = NewDir.Rotation();
	auto NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);
	return NewRotation;
}

FRotator PreyBehavior::GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	if(BirdFlockToFollow)
	{
		return FMath::RInterpConstantTo(CurrentRotation,
			(BirdFlockToFollow->GetPlaceInFlock(PlaceInFlock) - CurrentLocation).Rotation(),
			DeltaTime, TurnSpeed);
	}
	return CurrentRotation;
}

bool PreyBehavior::GetIsOutOfBounds(const FVector& CurrentLocation) const
{
	return CurrentLocation.X < -PlayableArea.X / 2 || CurrentLocation.X > PlayableArea.X / 2
		|| CurrentLocation.Y < -PlayableArea.Y / 2 || CurrentLocation.Y > PlayableArea.Y / 2
		|| CurrentLocation.Z > PlayableArea.Z;
}

FRotator PredatorBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	return CurrentRotation;
}
