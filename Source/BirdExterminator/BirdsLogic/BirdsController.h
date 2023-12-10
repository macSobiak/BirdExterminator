// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdsController.generated.h"

class ABirdFlock;
class ABird;
class ABirdExterminatorGameMode;

DECLARE_EVENT_OneParam(ABirdsController, FOnBirdCountChanged, const int&);

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

	AActor* GetNearestBird(const FVector &LocationFrom, float &Distance);
	AActor* GetNearestPredator(const FVector &LocationFrom, float &Distance);
	void RegisterAsFreeBird(ABird* Bird);
	void RegisterAsPredatorBird(ABird* Bird);
	void UnregisterPredator(ABird* Bird);
	void HandleBirdDestroyed(ABird* Bird);
	void SpawnPredatorBird(const FVector &SpawnLocation, const FRotator &SpawnRotator, const FActorSpawnParameters &SpawnParameters);
	void BirdInFlockDestroyed();

	FOnBirdCountChanged OnBirdCountChangedEvent;
	FOnBirdCountChanged OnPredatorCountChangedEvent;
	FOnBirdCountChanged OnPredatorAliveCountChangedEvent;

private:
	void SpawnBirdFlocks(const int& BirdFlocksCount);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PredatorBirdClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABirdFlock> BirdFlockBlueprint;
	
	TArray<ABirdFlock*> BirdFlocksArray;
	TArray<ABird*> PredatorsArray;
	TArray<ABird*> FreeBirdsArray;

	FVector3f PlayableArea;
public:	
	bool IsInitialized = false;
	int PreyBirdsAlive = 0;
	int PredatorBirdsAvailable = 10;
	int PredatorBirdsAlive = 0;
};
