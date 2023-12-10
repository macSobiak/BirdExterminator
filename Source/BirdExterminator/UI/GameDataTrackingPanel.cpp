// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataTrackingPanel.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"
#include "CounterWithLabel.h"

void UGameDataTrackingPanel::InitializePanel(ABirdsController* BirdsControllerData)
{
	BirdsController = BirdsControllerData;

	BirdsAliveCounter->InitializeElement(BirdsController->PreyBirdsAlive, BirdsController->PreyBirdsAlive, FString("Birds To Exterminate: "));
	PredatorAmmoCounter->InitializeElement(BirdsController->PredatorBirdsAvailable, BirdsController->PredatorBirdsAvailable, FString("Predator Birds Ammo: "));
	PredatorsAliveCounter->InitializeElement(BirdsController->PredatorBirdsAlive, 0, FString("Predator Birds Alive: "));

	BirdsController->OnBirdCountChangedEvent.AddUObject(BirdsAliveCounter, &UCounterWithLabel::RefreshCurrentCount);
	BirdsController->OnPredatorCountChangedEvent.AddUObject(PredatorAmmoCounter, &UCounterWithLabel::RefreshCurrentCount);
	BirdsController->OnPredatorAliveCountChangedEvent.AddUObject(PredatorsAliveCounter, &UCounterWithLabel::RefreshCurrentCount);
}
