// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ABird;
class ABirdFlock;

enum EBirdState
{
	DefaultBehavior,
	RecoveryAfterHit,
	OutOfBounds
};

class BIRDEXTERMINATOR_API BirdBehavior
{
public:
	BirdBehavior();
	virtual ~BirdBehavior();

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) = 0;
	virtual bool HandleBirdHit(AActor *ActorHit) = 0;
	
	float TurnSpeed = 210;
	float InitialVelocity = 500;
	
protected:
	FVector3f PlayableArea;
	EBirdState BirdState = DefaultBehavior;
};

