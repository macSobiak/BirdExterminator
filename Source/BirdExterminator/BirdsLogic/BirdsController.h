// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BirdsController.generated.h"

class ABirdFlock;
class ABird;

DECLARE_EVENT_OneParam(ABirdsController, FOnBirdCountChanged, const int&);

UCLASS()
class BIRDEXTERMINATOR_API ABirdsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdsController();
	void Initialize(const FVector3f &PlayableAreaRef, const uint16 &BirdFlocksCount, const uint16 &PredatorBirdsAmmo);

	AActor* GetNearestBird(const FVector &LocationFrom, float &Distance);
	AActor* GetNearestPredator(const FVector &LocationFrom, float &Distance);
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
