// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CounterWithLabel.h"
#include "BirdsAliveCounter.generated.h"

class ABirdsController;
UCLASS()
class BIRDEXTERMINATOR_API UBirdsAliveCounter : public UCounterWithLabel
{
	GENERATED_BODY()

public:
	void RegisterToControllerEvents(ABirdsController* BirdsControllerToSet);
	
private:
	UPROPERTY()
	ABirdsController* BirdsController;
};
