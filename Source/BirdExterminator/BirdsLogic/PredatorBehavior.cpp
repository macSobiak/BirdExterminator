// Fill out your copyright notice in the Description page of Project Settings.


#include "PredatorBehavior.h"

#include "BirdsController.h"
#include "../World/Bird.h"
#include "BirdExterminator/GameBase/BirdExterminatorGameMode.h"
#include "Kismet/GameplayStatics.h"

PredatorBehavior::PredatorBehavior(ABird *ActorOwner, const FVector3f& PlayableAreaRef) : BirdBehavior(PlayableAreaRef)
{
	BirdOwner = ActorOwner;
	GameMode = Cast<ABirdExterminatorGameMode>(UGameplayStatics::GetGameMode(ActorOwner));
	TurnSpeed = 250;
}

PredatorBehavior::~PredatorBehavior()
{
}

FRotator PredatorBehavior::GetDirectionConditional(const float& DeltaTime, const FVector& CurrentLocation, const FRotator& CurrentRotation)
{
	float NearestDistance;
	const auto NearestBird = GameMode->BirdsController->GetNearestBird(CurrentLocation, NearestDistance);

	if(NearestBird)
	{
		if(NearestDistance < BoostDistance && Energy > 0)
		{
			ConsumeEnergyAndGiveBonus(DeltaTime);
		}
		
		return FMath::RInterpConstantTo(CurrentRotation,(NearestBird->GetActorLocation() - CurrentLocation).Rotation(), DeltaTime, TurnSpeed);
	}
	//if no prey left -> fly around, avoid obstacles and don't leave the play area
	if(GetIsOutOfBounds(CurrentLocation))
	{
		return GetRotationToMapCenter(DeltaTime, CurrentLocation, CurrentRotation);
	}
	
	return CurrentRotation;
}

bool PredatorBehavior::HandleBirdHit(AActor* ActorHit)
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

void PredatorBehavior::ConsumeEnergyAndGiveBonus(const float& DeltaTime)
{
	Energy -= EnergyLossPerSec * DeltaTime;
	UE_LOG(LogTemp, Error, TEXT("Energy CONSUUUME %f"), Energy);
	InitialVelocity = 700;
	
	if(Energy <= 0)
	{
		BirdOwner->InitializeAsPrey(nullptr, 0, PlayableArea);
	}
}
