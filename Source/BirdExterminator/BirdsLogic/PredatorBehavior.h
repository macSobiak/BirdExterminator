// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BirdBehavior.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ABirdExterminatorGameMode;

class BIRDEXTERMINATOR_API PredatorBehavior : public BirdBehavior
{
public:
	PredatorBehavior(AActor *ActorOwner);
	virtual ~PredatorBehavior() override;

	virtual FRotator GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation) override;
	virtual bool HandleBirdHit(AActor *ActorHit) override;

	ABirdExterminatorGameMode* GameMode;
};
