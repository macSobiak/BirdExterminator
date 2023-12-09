// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CounterWithLabel.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

class UShotPredatorsCounter;
class UBirdsAliveCounter;
class ABirdsController;
UCLASS()
class BIRDEXTERMINATOR_API AUIController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIController();
	void InitializeInterfaceElements(ABirdsController* BirdsControllerInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ShowWidget(UUserWidget* Widget);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCounterWithLabel> PreyBirdCounterWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCounterWithLabel> PredatorBirdsCounterWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairWidget;

	UPROPERTY()
	UBirdsAliveCounter* PreyBirdCounter;
	UPROPERTY()
	UShotPredatorsCounter* PredatorBirdsCounter;
	UPROPERTY()
	UUserWidget* Crosshair;
	
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	ABirdsController* BirdsController;
};
