// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameDataTrackingPanel.generated.h"

class ABirdsController;

UCLASS()
class BIRDEXTERMINATOR_API UGameDataTrackingPanel : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	ABirdsController* BirdsController;

public:
	void InitializePanel(ABirdsController* BirdsControllerData);
	
	UPROPERTY(meta=(BindWidget))
	class UCounterWithLabel* BirdsAliveCounter;
	UPROPERTY(meta=(BindWidget))
	class UCounterWithLabel* PredatorAmmoCounter;
	UPROPERTY(meta=(BindWidget))
	class UCounterWithLabel* PredatorsAliveCounter;
	
};