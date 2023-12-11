// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * Behavior of a Predator Bird:
 * when initialized (shot) flies forward with free doubled boost to give more control over aiming and shooting for a LaunchCooldown amount of time
 * after LaunchCooldown - following nearest Prey Bird, consuming energy and applying boost to move and turn speed (2 levels of boost depending on distance to target)
 * when collided with destructable Prey Bird - destroys the Prey Bird and gains some Energy back
 * keeping in world bounds
 * when out of energy transforming to Prey behavior (deleting this behavior class and initializing as Prey on Bird owner class)
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
	
	float EnergyLossPerSec = 26;
	float EnergyGainAfterKill = 40;
	
	float BoostDistance = 500;
	float MiniBoostDistance = 1000;
	
	float BoostMultiplier = 1.5;
	float LaunchCooldown = 0.5;
	
	EBoostType BoostType = Launch;
};