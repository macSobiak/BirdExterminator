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
	void Initialize(const FVector3f &PlayableAreaRef, const int &BirdFlocksCount);

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
	
	TArray<ABirdFlock*> BirdFlocksArray;
	TArray<ABird*> PredatorsArray;
	TArray<ABird*> FreeBirdsArray;
	
	FVector3f PlayableArea;

public:
	UPROPERTY()
	UMaterial* StoredMaterialPrey = nullptr;
	UPROPERTY()
	UMaterial* StoredMaterialPredator = nullptr;
	
	int PreyBirdsAlive = 0;
	int PredatorBirdsAvailable = 10;
	int PredatorBirdsAlive = 0;

private:
	bool IsInitialized = false;
};
