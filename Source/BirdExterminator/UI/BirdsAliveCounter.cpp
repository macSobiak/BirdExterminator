// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdsAliveCounter.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"

void UBirdsAliveCounter::RegisterToControllerEvents(ABirdsController* BirdsControllerToSet)
{
	BirdsController = BirdsControllerToSet;
	BirdsController->OnBirdCountChangedEvent.AddUObject(this, &UBirdsAliveCounter::RefreshCurrentCount);
}
