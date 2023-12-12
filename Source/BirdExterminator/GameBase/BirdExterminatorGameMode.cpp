// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdExterminatorGameMode.h"

#include "BirdExterminator/World/WorldBuilder.h"
#include "BirdExterminator/BirdsLogic/BirdsController.h"
#include "BirdExterminator/UI/UIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ABirdExterminatorGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector3f PlayableArea;
	FString ErrorMessage;
	WorldBuilder = Cast<AWorldBuilder>(UGameplayStatics::GetActorOfClass(this, AWorldBuilder::StaticClass()));
	BirdsController = Cast<ABirdsController>(UGameplayStatics::GetActorOfClass(this, ABirdsController::StaticClass()));
	UIController = Cast<AUIController>(UGameplayStatics::GetActorOfClass(this, AUIController::StaticClass()));
	
	if(!WorldBuilder->GenerateWorld(PlayableArea, ErrorMessage))
	{
		UIController->ShowPopup(ErrorMessage);
		return;
	}
	
	BirdsController->Initialize(PlayableArea, BirdFlocksToSpawn, PredatorBirdsAvailable);

	BirdsController->OnBirdCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLoseConditions);
	BirdsController->OnPredatorCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLoseConditions);
	BirdsController->OnPredatorAliveCountChangedEvent.AddUObject(this, &ABirdExterminatorGameMode::CheckWinLoseConditions);

	UIController->InitializeInterfaceElements(BirdsController);

	CheckWinLoseConditions(0);
}

void ABirdExterminatorGameMode::CheckWinLoseConditions(const int& _) const
{
	bool GameEndConditionFailed = false;
	if(BirdsController->PreyBirdsAlive == 0)
	{
		UIController->ShowPopup(TEXT("ALL BIRDS EXTERMINATED!\nCONGRATULATIONS!"));
	}
	else if (BirdsController->PredatorBirdsAlive == 0 && BirdsController->PredatorBirdsAvailable == 0)
	{
		UIController->ShowPopup(FString::Printf(TEXT("THERE ARE %d BIRDS LEFT TO EXTERMINATE!\nYOU LOST!"), BirdsController->PreyBirdsAlive));
	}
	else
	{
		GameEndConditionFailed = true;
	}

	if(GameEndConditionFailed)
		return;
	
	if(const auto Character = UGameplayStatics::GetPlayerCharacter(this, 0); Character)
	{
		Character->GetCharacterMovement()->DisableMovement();
	}
}
