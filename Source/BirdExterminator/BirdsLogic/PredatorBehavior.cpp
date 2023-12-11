// Fill out your copyright notice in the Description page of Project Settings.


#include "PredatorBehavior.h"
#include "BirdsController.h"
#include "../World/Bird.h"

PredatorBehavior::PredatorBehavior(ABird *ActorOwner, const FVector3f& PlayableAreaRef) : BirdBehavior(PlayableAreaRef, ActorOwner)
{
}

PredatorBehavior::~PredatorBehavior()
{
}

inline FRotator PredatorBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	float NearestDistance;

	//always prioritize chasing nearest prey bird
	if(const auto &NearestBird = BirdOwner->BirdsController->GetNearestBird(CurrentLocation, NearestDistance); NearestBird)
	{
		ConsumeEnergyAndGiveBonusIfPossible(DeltaTime, NearestDistance);
		return FMath::RInterpConstantTo(CurrentRotation,(NearestBird->GetActorLocation() - CurrentLocation).Rotation(), DeltaTime, TurnSpeed);
	}

	if(BoostType != None)
		BoostType = None;
	
	//if no prey left -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return CurrentRotation;
}

//when hit bird, check if should be destroyed and destroy if needed
inline bool PredatorBehavior::HandleBirdHit(AActor* ActorHit)
{
	const ABird* BirdActor = Cast<ABird>(ActorHit);
	if(BirdActor && BirdActor->IsDestructable)
	{
		ActorHit->Destroy();
		if((Energy += EnergyGainAfterKill) > MaxEnergy)
			Energy = MaxEnergy;

		return true;
	}
	
	//if collided with something that is not a Prey during launch time, act normal
	if(BoostType == Launch)
		BoostType = None;
	
	return false;
}

//predators should be able to turn more sharply when on boost
inline float PredatorBehavior::GetTurnSpeed() const
{
	return (BoostType == Standard || BoostType == Mini) ? TurnSpeed * BoostMultiplier * 2 : TurnSpeed;
}

inline float PredatorBehavior::GetMoveSpeed() const
{
	//after launch, predator is mindlessly flying forward with 2x boost
	if(BoostType == Launch)
		return MoveSpeed * BoostMultiplier * 2;
	
	if(BoostType == Mini)
		return MoveSpeed * (((BoostMultiplier - 1) / 2) + 1);
	
	if(BoostType == Standard)
		return MoveSpeed * BoostMultiplier;
	
	return MoveSpeed;
}

inline bool PredatorBehavior::GetIsOnCooldownAndSetBoost(const float& DeltaTime)
{
	if(BoostType == Launch)
	{
		if((LaunchCooldown -= DeltaTime) > 0)
		{
			return true;
		}
		BoostType = None;
	}
	return false;
}

//should turn only if launch cooldown has passed
inline bool PredatorBehavior::GetCanTurn(const float& DeltaTime)
{
	return !GetIsOnCooldownAndSetBoost(DeltaTime);
}

inline void PredatorBehavior::ConsumeEnergyAndGiveBonusIfPossible(const float& DeltaTime, const float& NearestDistance)
{
	float EnergyLoss;
	//if predators is very close, apply maximum boost multiplier
	if(NearestDistance < BoostDistance)
	{
		BoostType = Standard;
		EnergyLoss = EnergyLossPerSec;
	}
	//if predators is very close, but still quite far, apply mini boos to close up to prey
	else if (NearestDistance < MiniBoostDistance)
	{
		BoostType = Mini;
		EnergyLoss = EnergyLossPerSec / 2;
	}
	else
	{
		BoostType = None;
		return;
	}

	Energy -= EnergyLoss * DeltaTime;

	//transform to prey if no energy
	if(Energy <= 0)
	{

		BirdOwner->BirdsController->UnregisterPredator(BirdOwner);
		BirdOwner->BirdsController->RegisterAsFreeBird(BirdOwner);
	
		BirdOwner->TransformToPrey();
	}
}
