// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * Behavior of a Prey Bird:
 * following Bird Flock, if not available (in case the bird was created from Predator) it just moves ahead
 * keeping in world bounds
 * flying in opposite direction as vector to nearest Predator detected
 */
class BIRDEXTERMINATOR_API PreyBehavior : public BirdBehavior
{
public:
	PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef, ABird* Owner);
	virtual ~PreyBehavior() override;

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
	virtual bool HandleBirdHit(AActor *ActorHit) override;

private:
	FRotator GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const;
	
	ABirdFlock *BirdFlockToFollow;
	int PlaceInFlock = 0;
	float DangerDistance = 750;

};
