// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdsController.generated.h"

class ABirdFlock;
UCLASS()
class BIRDEXTERMINATOR_API ABirdsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialize(const FVector3f &PlayableAreaRef, const int &BirdFlocksCount);

	AActor* GetNearestBird(const FVector &LocationFrom);
	
private:
	void SpawnBirdFlocks(const int& BirdFlocksCount);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABirdFlock> BirdFlockBlueprint;
	
	TArray<ABirdFlock*> BirdFlocksArray;

	FVector3f PlayableArea;
	bool IsInitialized = false;
};
