// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * 
 */

class BIRDEXTERMINATOR_API PredatorBehavior : public BirdBehavior
{
public:
	PredatorBehavior(ABird *ActorOwner, const FVector3f& PlayableAreaRef);
	virtual ~PredatorBehavior() override;

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
	virtual bool HandleBirdHit(AActor *ActorHit) override;

	void ConsumeEnergyAndGiveBonusIfPossible(const float& DeltaTime, const float& NearestDistance);
	void ResetSpeedBonus();
	
	ABird* BirdOwner;

	float Energy = 100;
	float EnergyLossPerSec = 25;
	float EnergyGainAfterKill = 25;

	float BoostDistance = 500;

};