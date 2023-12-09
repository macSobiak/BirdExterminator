// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * 
 */
class BIRDEXTERMINATOR_API PreyBehavior : public BirdBehavior
{
public:
	PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef);
	virtual ~PreyBehavior() override;

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;

private:
	bool GetIsOutOfBounds(const FVector& CurrentLocation) const;
	FRotator GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const;
	FRotator GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const;
	
	ABirdFlock *BirdFlockToFollow;
	int PlaceInFlock = 0;

};
