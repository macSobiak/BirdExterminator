// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"

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

	CollisionPredictorBoxAhead = Cast<UBoxComponent>(CollisionPredictorBoxAheadReference.GetComponent(this));
	CollisionPredictorBoxUnder = Cast<UBoxComponent>(CollisionPredictorBoxUnderReference.GetComponent(this));

	if(CollisionPredictorBoxAhead)
	{
		CollisionPredictorBoxAhead->OnComponentBeginOverlap.AddDynamic(this, &ABird::OnOverlapCollisionPredictorBeginFront);
		CollisionPredictorBoxAhead->OnComponentEndOverlap.AddDynamic(this, &ABird::OnOverlapCollisionPredictorEndFront);
	}

	if(CollisionPredictorBoxUnder)
	{
		CollisionPredictorBoxUnder->OnComponentBeginOverlap.AddDynamic(this, &ABird::OnOverlapCollisionPredictorBeginBottom);
		CollisionPredictorBoxUnder->OnComponentEndOverlap.AddDynamic(this, &ABird::OnOverlapCollisionPredictorEndBottom);
	}

	MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	MeshComponent->OnComponentHit.AddDynamic(this, &ABird::OnHit);

}

void ABird::OnOverlapCollisionPredictorBeginFront(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentTurnSpeedFront = 1;
}

void ABird::OnOverlapCollisionPredictorEndFront(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CurrentTurnSpeedFront = 0;
}

void ABird::OnOverlapCollisionPredictorBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CurrentTurnSpeedBottom = 1;
}

void ABird::OnOverlapCollisionPredictorEndBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CurrentTurnSpeedBottom = 0;
}


void ABird::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(!IsOnHitCooldown)
	{
		MeshComponent->AddImpulse(GetActorForwardVector() * InitialVelocity * 10);
		MeshComponent->SetEnableGravity(true);
	}
	
	IsOnHitCooldown = true;
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsOnHitCooldown)
	{
		if((CurrentCooldown += DeltaTime) >= HitCooldown)
		{
			IsOnHitCooldown = false;
			CurrentCooldown = 0;
			MeshComponent->SetEnableGravity(false);

			MeshComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
			MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
		}
	}
	else
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * (InitialVelocity * DeltaTime), true);
		auto rotatore = (FRotator(CurrentTurnSpeedBottom,CurrentTurnSpeedFront, 0.0f) * (TurnSpeed * DeltaTime));
		UE_LOG(LogTemp, Error, TEXT("rot: %s"), *rotatore.ToString())
		AddActorLocalRotation(rotatore);
	}



}

