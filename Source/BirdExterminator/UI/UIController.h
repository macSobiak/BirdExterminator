// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

class UGameDataTrackingPanel;
class UPromptWithButtons;
class ABirdsController;

/**
 * Main UI Controller object that creates needed widgets and also initializes and shows them when asked. 
 */
UCLASS()
class BIRDEXTERMINATOR_API AUIController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIController();
	void InitializeInterfaceElements(ABirdsController* BirdsControllerInstance);
	void ShowPopup(const FString &PopupMessage);

private:	

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameDataTrackingPanel> GameDataTrackingPanelWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CrosshairWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPromptWithButtons> PopupWidget;
	
	UPROPERTY()
	UGameDataTrackingPanel* GameDataTrackingPanel;
	UPROPERTY()
	UUserWidget* Crosshair;
	UPROPERTY()
	UPromptWithButtons* PromptWithButtons;
	
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	ABirdsController* BirdsController;
};
