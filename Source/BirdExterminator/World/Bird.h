// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Bird.generated.h"

UCLASS()
class BIRDEXTERMINATOR_API ABird : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapCollisionPredictorBeginFront(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapCollisionPredictorEndFront(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapCollisionPredictorBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapCollisionPredictorEndBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float DefaultVelocity = 100;

	UPROPERTY(EditAnywhere)
	FComponentReference CollisionPredictorBoxAheadReference;
	UPROPERTY(EditAnywhere)
	FComponentReference CollisionPredictorBoxUnderReference;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *CollisionPredictorBoxAhead;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *CollisionPredictorBoxUnder;

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 10;

	float CurrentTurnSpeedFront = 0;
	float CurrentTurnSpeedBottom = 0;

};
