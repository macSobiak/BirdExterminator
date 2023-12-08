// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Bird.h"
#include "CollisionPredictor.generated.h"

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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	EColliderType ColliderTypeToIgnore;
	UPROPERTY(EditAnywhere)
	EColliderType ColliderType;

	UPROPERTY()
	TArray<AActor*> OverlappingActors;
};
