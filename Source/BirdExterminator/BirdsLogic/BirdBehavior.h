// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ABird;
class ABirdFlock;

class BIRDEXTERMINATOR_API BirdBehavior
{
public:
	BirdBehavior(const FVector3f& PlayableAreaRef, ABird* Owner);
	virtual ~BirdBehavior();

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) = 0;
	virtual bool HandleBirdHit(AActor *ActorHit) = 0;
	virtual bool GetIsOutOfBounds(const FVector& CurrentLocation) const;
	virtual FRotator GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) const;
	virtual float GetTurnSpeed() const {return TurnSpeed;};
	virtual float GetMoveSpeed() const {return MoveSpeed;};
	virtual bool GetCanTurn(const float& DeltaTime) {return true;};

protected:
	FVector3f PlayableArea;
	ABird* BirdOwner;

	float TurnSpeed = 210;
	float MoveSpeed = 500;
};

