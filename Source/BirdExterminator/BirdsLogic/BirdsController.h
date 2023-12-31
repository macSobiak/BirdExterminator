// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdsController.generated.h"

class ABirdFlock;
class ABird;

DECLARE_EVENT_OneParam(ABirdsController, FOnBirdCountChanged, const int&);

/**
 * Main Bird Controller -> creates Bird Flocks, handles Bird destroyed events, keeps track of all counters
 * provides methods for getting nearest Prey / Predator birds
 * provides information for win / lose conditions
 */
UCLASS()
class BIRDEXTERMINATOR_API ABirdsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdsController();
	void Initialize(const FVector3f &PlayableAreaRef, const uint16 &BirdFlocksCount, const uint16 &PredatorBirdsAmmo);

	AActor* GetNearestBird(const FVector &LocationFrom, float &Distance) const;
	AActor* GetNearestPredator(const FVector &LocationFrom, float &Distance) const;
	void RegisterAsFreeBird(ABird* Bird);
	void RegisterAsPredatorBird(ABird* Bird);
	void UnregisterPredator(ABird* Bird);
	void SpawnPredatorBird(const FVector &SpawnLocation, const FRotator &SpawnRotator, const FActorSpawnParameters &SpawnParameters);

protected:
	virtual void Destroyed() override;

private:
	void HandleBirdDestroyed(ABird* Bird);
	UFUNCTION()
	void HandleBirdFlockDestroyed(AActor* Actor);
	void BirdInFlockDestroyed();
	void SpawnBirdFlocks(const int& BirdFlocksCount);
	static void FindNearestInVector(const FVector& LocationFrom, const std::vector<ABird*> &BirdsVector, AActor *&NearestActor, float& CurrentMinDistance);

public:
	FOnBirdCountChanged OnBirdCountChangedEvent;
	FOnBirdCountChanged OnPredatorCountChangedEvent;
	FOnBirdCountChanged OnPredatorAliveCountChangedEvent;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PredatorBirdClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABirdFlock> BirdFlockBlueprint;
	
	std::vector<ABirdFlock*> BirdFlocksVector;
	std::vector<ABird*> PredatorsVector;
	std::vector<ABird*> FreeBirdsVector;

public:
	FVector3f PlayableArea;

	UPROPERTY()
	UMaterial* StoredMaterialPrey = nullptr;
	UPROPERTY()
	UMaterial* StoredMaterialPredator = nullptr;
	
	int PreyBirdsAlive = 0;
	int PredatorBirdsAlive = 0;

	uint16 PredatorBirdsAvailable = 10;

private:
	bool IsInitialized = false;
};