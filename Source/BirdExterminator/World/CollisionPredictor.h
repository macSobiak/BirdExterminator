// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Bird.h"
#include "CollisionPredictor.generated.h"

/**
 * Box Component that informs a Bird Actor if anything is overlapping with it to change the direction of the flight
 * in order to avoid detected obstacle
 * ignores other components of this type and the owner Actor itself
 */

UENUM()
enum class EColliderType : uint8
{
	HorizontalLeft = 0,
	HorizontalRight = 1,
	VerticalTop = 2,
	VerticalBottom = 3
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIRDEXTERMINATOR_API UCollisionPredictor : public UBoxComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnOverlapCollisionPredictorBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapCollisionPredictorEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	FRotator DirectionToSet;

	UPROPERTY()
	ABird *BirdOwner;
	
public:	
	// Sets default values for this component's properties
	UCollisionPredictor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//when registering collision, this type is also saved to ignore when it wants to also register (the opposite sides that could negate te turn vector)
	UPROPERTY(EditAnywhere)
	EColliderType ColliderTypeToIgnore;
	UPROPERTY(EditAnywhere)
	EColliderType ColliderType;

	int CollidedObjectsCount = 0;
};
