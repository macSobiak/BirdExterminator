// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

#include "BirdFlock.h"
#include "../BirdsLogic/PreyBehavior.h"
#include "CollisionPredictor.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"
#include "BirdExterminator/BirdsLogic/PredatorBehavior.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABird::ABird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ABird::~ABird()
{

}

void ABird::Destroyed()
{
	if(BirdBehaviorDefinition)
	{
		delete BirdBehaviorDefinition;
		BirdBehaviorDefinition = nullptr;
	}

	OnBirdDestroyedEvent.Broadcast(this);
	IsInitialized = false;

	Super::Destroyed();
}

#pragma region BirdBehaviorAndAppearance

void ABird::InitializeCommonObjects(ABirdsController* BirdsControllerInstance)
{
	BirdsController = BirdsControllerInstance;
	if(!MeshComponent)
	{
		MeshComponent = FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->OnComponentHit.AddDynamic(this, &ABird::OnHit);
	}
}

inline void ABird::AdjustAppearance(UMaterial* MaterialToSet, const ECollisionChannel &ChannelToSet, const ECollisionResponse &ResponseToBirdPrey)
{
	SetAppearance(MaterialToSet);
	SetCollision(ChannelToSet, ResponseToBirdPrey);
	IsInitialized = true;
}

void ABird::InitializeAsPrey(ABirdFlock* BirdFlock, const int& PlaceInFlockRef, const FVector3f& PlayableAreaRef)
{
	if(BirdBehaviorDefinition)
	{
		delete BirdBehaviorDefinition;
	}
	BirdBehaviorDefinition = new PreyBehavior(BirdFlock, PlaceInFlockRef, PlayableAreaRef, this);
	IsDestructable = true;
	AdjustAppearance(BirdsController->StoredMaterialPrey, ECC_GameTraceChannel2, ECR_Overlap);
}

void ABird::TransformToPrey()
{
	InitializeAsPrey(nullptr, 0, BirdsController->PlayableArea);
}

void ABird::InitializeAsPredator(const FVector3f& PlayableAreaRef)
{
	if(BirdBehaviorDefinition)
	{
		delete BirdBehaviorDefinition;
	}
	BirdBehaviorDefinition = new PredatorBehavior(this, PlayableAreaRef);
	IsDestructable = false;

	AdjustAppearance(BirdsController->StoredMaterialPredator, ECC_GameTraceChannel3, ECR_Ignore);
}

inline void ABird::SetAppearance(UMaterial* MaterialToSet)
{
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(MaterialToSet, MeshComponent);
	MeshComponent->SetMaterial(0, DynamicMaterialInst);
}

inline void ABird::SetCollision(const ECollisionChannel& ChannelToSet, const ECollisionResponse& ResponseToBirdPrey)
{
	MeshComponent->SetCollisionObjectType(ChannelToSet);

	TArray<UCollisionPredictor*> CollisionPredictors;
	GetComponents(UCollisionPredictor::StaticClass(),CollisionPredictors);
	for(int i = 0; i < CollisionPredictors.Num(); ++i)
	{
		CollisionPredictors[i]->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ResponseToBirdPrey);
	}
}

#pragma endregion

void ABird::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                  FVector NormalImpulse, const FHitResult& Hit)
{
	//if predator hits other bird, do not mark as hit and add impulse
	if(BirdBehaviorDefinition->HandleBirdHit(OtherActor))
		return;

	//when collision happened add impulse with gravity to simulate hit
	if(!IsOnHitCooldown)
	{
		MeshComponent->AddImpulse(GetActorForwardVector() * BirdBehaviorDefinition->GetMoveSpeed() * 5);
		MeshComponent->SetEnableGravity(true);
	}
	
	IsOnHitCooldown = true;
}


inline void ABird::RecoverFromHit()
{
	IsOnHitCooldown = false;
	CurrentCooldown = 0;

	const FVector &RecoveryPosition = FVector::UpVector;
	SetActorRotation(RecoveryPosition.Rotation());
	AddActorWorldOffset(RecoveryPosition);
	
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
	MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsInitialized)
		return;
	
	if(IsOnHitCooldown)
	{
		if((CurrentCooldown += DeltaTime) >= HitCooldown)
		{
			RecoverFromHit();
		}
	}
	else
	{
		//always prioritize avoiding obstacles (if is not a predator that just have been launched)
		if(BirdBehaviorDefinition->GetCanTurn(DeltaTime))
		{
			AddActorLocalRotation(TurnSpeedRotator * (BirdBehaviorDefinition->GetTurnSpeed() * DeltaTime));

			//if no collision avoidance happens, make standard decisions like keep in playable area or target prey bird
			if(TurnSpeedRotator.Pitch == 0 && TurnSpeedRotator.Yaw == 0)
				SetActorRotation(BirdBehaviorDefinition->GetDirectionConditional(DeltaTime, GetActorLocation(), GetActorRotation()));
		}

		//bird is always moving forward
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * (BirdBehaviorDefinition->GetMoveSpeed() * DeltaTime), true);
	}
}

void ABird::RegisterModifier(const FRotator& VectorOffset, UCollisionPredictor *CollisionPredictor)
{
	for(int i =0; i < CollidersToIgnore.Num(); ++i)
	{
		if(CollidersToIgnore[i] == CollisionPredictor->ColliderType)
			return;
	}
	for(int i =0; i < ActiveColliders.Num(); ++i)
	{
		if(ActiveColliders[i] == CollisionPredictor->ColliderType)
			return;
	}

	ActiveColliders.Add(CollisionPredictor->ColliderType);
	CollidersToIgnore.Add(CollisionPredictor->ColliderTypeToIgnore);

	TurnSpeedRotator += VectorOffset;
}

void ABird::UnregisterModifier(const FRotator& VectorOffset, UCollisionPredictor *CollisionPredictor)
{
	bool ColliderFound = false;
	for(int i =0; i < ActiveColliders.Num(); ++i)
	{
		if(ActiveColliders[i] == CollisionPredictor->ColliderType)
		{
			ColliderFound = true;
			break;;
		}
	}

	if(ColliderFound)
	{
		ActiveColliders.Remove(CollisionPredictor->ColliderType);
		CollidersToIgnore.Remove(CollisionPredictor->ColliderTypeToIgnore);

		TurnSpeedRotator -= VectorOffset;
	}
}


