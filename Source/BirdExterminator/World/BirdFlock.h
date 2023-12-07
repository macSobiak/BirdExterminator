// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdFlock.generated.h"

class ABird;

UCLASS()
class BIRDEXTERMINATOR_API ABirdFlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdFlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<ABird*> BirdArray;

	UPROPERTY(EditAnywhere)
	int InitialBirdCount = 7;

	FVector FlockFlightDirection = FVector(1,0,0);
};
