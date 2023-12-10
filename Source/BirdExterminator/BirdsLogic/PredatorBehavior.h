// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * 
 */

class BIRDEXTERMINATOR_API PredatorBehavior : public BirdBehavior
{
	void ConsumeEnergyAndGiveBonusIfPossible(const float& DeltaTime, const float& NearestDistance);
	bool GetIsOnCooldownAndSetBoost(const float& DeltaTime);

public:
	PredatorBehavior(ABird *ActorOwner, const FVector3f& PlayableAreaRef);
	virtual ~PredatorBehavior() override;

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
	virtual bool HandleBirdHit(AActor *ActorHit) override;
	virtual float GetTurnSpeed() const override;
	virtual float GetMoveSpeed() const override;
	virtual bool GetCanTurn(const float& DeltaTime) override;

private:
	float MaxEnergy = 100;
	float Energy = 100;
	
	float EnergyLossPerSec = 25;
	float EnergyGainAfterKill = 25;
	
	float BoostDistance = 500;
	float BoostMultiplier = 1.4;
	
	float LaunchCooldown = 0.5;
	bool IsJustLaunched = true;
	bool IsOnBoost = false;
};