// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionPredictor.h"

// Sets default values for this component's properties
UCollisionPredictor::UCollisionPredictor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UCollisionPredictor::OnOverlapCollisionPredictorBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner())
		return;

	if(CollidedObjectsCount == 0)
		BirdOwner->RegisterModifier(DirectionToSet, this);

	CollidedObjectsCount++;
}

void UCollisionPredictor::OnOverlapCollisionPredictorEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == GetOwner() || CollidedObjectsCount == 0)
		return;

	CollidedObjectsCount--;

	if(CollidedObjectsCount == 0)
		BirdOwner->UnregisterModifier(DirectionToSet, this);
}

// Called when the game starts
void UCollisionPredictor::BeginPlay()
{
	Super::BeginPlay();

	BirdOwner = Cast<ABird>(GetOwner());
	
	OnComponentBeginOverlap.AddUniqueDynamic(this, &UCollisionPredictor::OnOverlapCollisionPredictorBegin);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UCollisionPredictor::OnOverlapCollisionPredictorEnd);
}


