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

	if(const auto &NearestBird = BirdOwner->BirdsController->GetNearestBird(CurrentLocation, NearestDistance); NearestBird)
	{
		ConsumeEnergyAndGiveBonusIfPossible(DeltaTime, NearestDistance);
		return FMath::RInterpConstantTo(CurrentRotation,(NearestBird->GetActorLocation() - CurrentLocation).Rotation(), DeltaTime, TurnSpeed);
	}

	if(IsOnBoost)
		IsOnBoost = false;
	
	//if no prey left -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return CurrentRotation;
}

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
	
	//if collided with something that is not a Prey, act normal
	if(IsJustLaunched)
		IsJustLaunched = false;
	
	return false;
}

inline float PredatorBehavior::GetTurnSpeed() const
{
	return IsOnBoost ? TurnSpeed * BoostMultiplier : TurnSpeed;
}

//apply free 2x boost when predator bird is shot in the same direction
inline float PredatorBehavior::GetMoveSpeed() const
{
	return IsJustLaunched ? MoveSpeed * BoostMultiplier * 2 : (IsOnBoost ? MoveSpeed * BoostMultiplier : MoveSpeed);
}

inline bool PredatorBehavior::GetIsOnCooldownAndSetBoost(const float& DeltaTime)
{
	if(IsJustLaunched)
	{
		if((LaunchCooldown -= DeltaTime) > 0)
		{
			return true;
		}
		IsJustLaunched = false;
	}
	return false;
}

inline bool PredatorBehavior::GetCanTurn(const float& DeltaTime)
{
	return !GetIsOnCooldownAndSetBoost(DeltaTime);
}

inline void PredatorBehavior::ConsumeEnergyAndGiveBonusIfPossible(const float& DeltaTime, const float& NearestDistance)
{
	if(NearestDistance < BoostDistance && Energy > 0)
	{
		Energy -= EnergyLossPerSec * DeltaTime;
		if(!IsOnBoost)
			IsOnBoost = true;

		if(Energy <= 0)
		{
			BirdOwner->BirdsController->UnregisterPredator(BirdOwner);
			BirdOwner->BirdsController->RegisterAsFreeBird(BirdOwner);
		
			BirdOwner->TransformToPrey();
		}
	}
	else if(IsOnBoost)
	{
		IsOnBoost = false;
	}

}
