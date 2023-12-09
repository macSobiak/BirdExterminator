// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotPredatorsCounter.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"

void UShotPredatorsCounter::RegisterToControllerEvents(ABirdsController* BirdsControllerToSet)
{
	BirdsController = BirdsControllerToSet;
	BirdsController->OnPredatorCountChangedEvent.AddUObject(this, &UShotPredatorsCounter::RefreshCurrentCount);
}