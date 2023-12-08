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
	float TurnSpeed = 210;

protected:
	FVector3f PlayableArea;
	EBirdState BirdState = DefaultBehavior;

private:
};

class BIRDEXTERMINATOR_API PreyBehavior : public BirdBehavior
{
public:
	PreyBehavior(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, const FVector3f &PlayableAreaRef);
	virtual ~PreyBehavior() override;

	FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
	bool GetIsOutOfBounds(const FVector& CurrentLocation) const;
	FRotator GetRotationToMapCenter(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation);
	FRotator GetRotationToBirdFlock(float DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation);
	ABirdFlock *BirdFlockToFollow;
	int PlaceInFlock = 0;

};

class BIRDEXTERMINATOR_API PredatorBehavior : public BirdBehavior
{
public:
	PredatorBehavior();
	virtual ~PredatorBehavior() override;

	FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
};
