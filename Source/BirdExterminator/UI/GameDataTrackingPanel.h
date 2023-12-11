// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameDataTrackingPanel.generated.h"

class ABirdsController;

/**
 * Main widget of the game that holds 3 counters that are informing about current bird counts through the game.
 */
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