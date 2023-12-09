// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

#include "BirdFlock.h"
#include "../BirdsLogic/PreyBehavior.h"
#include "CollisionPredictor.h"
#include "BirdExterminator/BirdsLogic/PredatorBehavior.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABird::ABird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Script/Engine.Material'/Game/Materials/M_Prey.M_Prey'"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterialPrey = FoundMaterial.Object;
	}

	FoundMaterial = (TEXT("/Script/Engine.Material'/Game/Materials/M_Predator.M_Predator'"));
	if (FoundMaterial.Succeeded())
	{
		StoredMaterialPredator = FoundMaterial.Object;
	}

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
}

void ABird::InitializeBase(UMaterial* MaterialToSet)
{
	if(!MeshComponent)
	{
		MeshComponent = FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->OnComponentHit.AddDynamic(this, &ABird::OnHit);
	}
	SetAppearance(MaterialToSet);
	
	IsInitialized = true;
}

void ABird::InitializeAsPrey(ABirdFlock* BirdFlock, const int& PlaceInFlockRef, FVector3f& PlayableAreaRef)
{
	if(BirdBehaviorDefinition)
	{
		delete BirdBehaviorDefinition;
	}
	BirdBehaviorDefinition = new PreyBehavior(BirdFlock, PlaceInFlockRef, PlayableAreaRef);
	
	InitializeBase(StoredMaterialPrey);

	MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel2);

	TArray<UCollisionPredictor*> other;
	GetComponents(UCollisionPredictor::StaticClass(),other);
	for(int i = 0; i < other.Num(); ++i)
	{
		other[i]->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	}
}

void ABird::InitializeAsPredator()
{
	if(BirdBehaviorDefinition)
	{
		delete BirdBehaviorDefinition;
	}
	BirdBehaviorDefinition = new PredatorBehavior(this);

	InitializeBase(StoredMaterialPredator);

	MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel3);

	TArray<UCollisionPredictor*> other;
	GetComponents(UCollisionPredictor::StaticClass(),other);
	for(int i = 0; i < other.Num(); ++i)
	{
		other[i]->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	}
}

void ABird::SetAppearance(UMaterial* MaterialToSet)
{
	DynamicMaterialInst = UMaterialInstanceDynamic::Create(MaterialToSet, MeshComponent);
	MeshComponent->SetMaterial(0, DynamicMaterialInst);
}


void ABird::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                  FVector NormalImpulse, const FHitResult& Hit)
{
	if(!IsOnHitCooldown)
	{
		MeshComponent->AddImpulse(GetActorForwardVector() * BirdBehaviorDefinition->InitialVelocity * 5);
		MeshComponent->SetEnableGravity(true);
	}
	
	IsOnHitCooldown = true;
}


void ABird::RecoverFromHit()
{
	IsOnHitCooldown = false;
	CurrentCooldown = 0;

	FVector RecoveryPosition = GetActorLocation() + FVector(0,0,1);
	SetActorRotation((RecoveryPosition - GetActorLocation()).Rotation());
	SetActorLocation(RecoveryPosition);
			
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
		AddActorLocalRotation(TurnSpeedRotator * (BirdBehaviorDefinition->TurnSpeed * DeltaTime));

		SetActorLocation(GetActorLocation() + GetActorForwardVector() * (BirdBehaviorDefinition->InitialVelocity * DeltaTime), true);

		//if no collision avoidance happens
		if(TurnSpeedRotator.Pitch == 0 && TurnSpeedRotator.Yaw == 0)
			SetActorRotation(BirdBehaviorDefinition->GetDirectionConditional(DeltaTime, GetActorLocation(), GetActorRotation()));
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


