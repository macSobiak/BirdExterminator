// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * 
 */
enum EBoostType : uint8
{
	None,
	Mini,
	Standard,
	Launch
};

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
	
	float EnergyLossPerSec = 24;
	float EnergyGainAfterKill = 38;
	
	float BoostDistance = 500;
	float MiniBoostDistance = 1000;
	
	float BoostMultiplier = 1.5;
	float LaunchCooldown = 0.5;
	
	EBoostType BoostType = Launch;
};