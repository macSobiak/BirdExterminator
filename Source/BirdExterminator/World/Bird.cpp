// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

#include "BirdFlock.h"
#include "CollisionPredictor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABird::ABird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
}


void ABird::Initialize(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, FVector &PlayableAreaRef)
{
	PlayableArea = PlayableAreaRef;

	MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	MeshComponent->OnComponentHit.AddDynamic(this, &ABird::OnHit);

	BirdFlockToFollow = BirdFlock;
	PlaceInFlock = PlaceInFlockRef;
}



void ABird::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                  FVector NormalImpulse, const FHitResult& Hit)
{
	if(BirdState != RecoveryAfterHit)
	{
		MeshComponent->AddImpulse(GetActorForwardVector() * InitialVelocity);
		MeshComponent->SetEnableGravity(true);
	}
	
	BirdState = RecoveryAfterHit;
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(BirdState == RecoveryAfterHit)
	{
		if((CurrentCooldown += DeltaTime) >= HitCooldown)
		{
			BirdState = FreeFlight;
			CurrentCooldown = 0;
			MeshComponent->SetEnableGravity(false);
			
			MeshComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
			MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
		}
	}
	else
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * (InitialVelocity * DeltaTime), true);
		AddActorLocalRotation(TurnSpeedRotator * (TurnSpeed * DeltaTime));

		if(GetActorLocation().X < -PlayableArea.X/2 || GetActorLocation().X > PlayableArea.X/2
			|| GetActorLocation().Y < -PlayableArea.Y/2 || GetActorLocation().Y > PlayableArea.Y/2
			|| GetActorLocation().Z > PlayableArea.Z)
		{
			BirdState = OutOfBounds;

			auto NewDir = FVector(0, 0, PlayableArea.Z / 2) - GetActorLocation();
			auto TargetRotation = NewDir.Rotation();
			auto NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, TurnSpeed);
			SetActorRotation(NewRotation);
		}
		else
		{
			BirdState = FreeFlight;
		}

		if(BirdState == FreeFlight && TurnSpeedRotator.Pitch == 0 && TurnSpeedRotator.Yaw == 0)
		{
			if(BirdFlockToFollow)
			{
				auto NewDir = BirdFlockToFollow->GetPlaceInFlock(PlaceInFlock) - GetActorLocation();
				auto TargetRotation = NewDir.Rotation();
				auto NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, TurnSpeed);
				SetActorRotation(NewRotation);
			}
		}


		

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
	UE_LOG(LogTemp, Error, TEXT("REJESTRUJE %d, %s"), (int)CollisionPredictor->ColliderType, *TurnSpeedRotator.ToString());

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
		UE_LOG(LogTemp, Error, TEXT("UNREJESTR %d, %s"), (int)CollisionPredictor->ColliderType, *TurnSpeedRotator.ToString());
	}
}


