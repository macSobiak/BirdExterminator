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
	
	ResetSpeedBonus();
	
	//if no prey left -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return CurrentRotation;
}

inline bool PredatorBehavior::HandleBirdHit(AActor* ActorHit)
{
	ABird* BirdActor = Cast<ABird>(ActorHit);
	if(BirdActor && BirdActor->IsDestructable)
	{
		ActorHit->Destroy();
		Energy += EnergyGainAfterKill;
		return true;
	}
	
	return false;
}

inline void PredatorBehavior::ConsumeEnergyAndGiveBonusIfPossible(const float& DeltaTime, const float& NearestDistance)
{
	if(NearestDistance < BoostDistance && Energy > 0)
	{
		Energy -= EnergyLossPerSec * DeltaTime;
		//UE_LOG(LogTemp, Error, TEXT("Energy CONSUUUME %f"), Energy);
		InitialVelocity = 700;
		TurnSpeed = 250;

		if(Energy <= 0)
		{
			BirdOwner->BirdsController->UnregisterPredator(BirdOwner);
			BirdOwner->BirdsController->RegisterAsFreeBird(BirdOwner);
		
			BirdOwner->TransformToPrey(PlayableArea);
		}
	}
	else
	{
		ResetSpeedBonus();
	}

}

inline void PredatorBehavior::ResetSpeedBonus()
{
	InitialVelocity = 500;
	TurnSpeed = 230;
}