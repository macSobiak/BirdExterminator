// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdBehavior.h"
#include "Kismet/GameplayStatics.h"

BirdBehavior::BirdBehavior(const FVector3f& PlayableAreaRef, ABird* Owner) : PlayableArea(PlayableAreaRef), BirdOwner(Owner)
{
}

BirdBehavior::~BirdBehavior()
{
}

FRotator BirdBehavior::GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const
{
	return FMath::RInterpConstantTo(CurrentRotation,
		FVector(-CurrentLocation.X, -CurrentLocation.Y, (PlayableArea.Z / 2) - CurrentLocation.Z).Rotation(),
		DeltaTime, GetTurnSpeed());
}

bool BirdBehavior::GetIsOutOfBounds(const FVector& CurrentLocation) const
{
	return CurrentLocation.X < -PlayableArea.X / 2 || CurrentLocation.X > PlayableArea.X / 2
		|| CurrentLocation.Y < -PlayableArea.Y / 2 || CurrentLocation.Y > PlayableArea.Y / 2
		|| CurrentLocation.Z > PlayableArea.Z;
}
