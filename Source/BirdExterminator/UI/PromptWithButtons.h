// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PromptWithButtons.generated.h"

/**
 * Prompt class adapted to display message and handle button click to exit the game
 */
UCLASS()
class BIRDEXTERMINATOR_API UPromptWithButtons : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeDestruct() override;
	UFUNCTION()
	void FirstButtonClicked();

public:
	void InitializePrompt(const FString &PromptMessage);

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PromptMessageBlock;

	UPROPERTY(meta=(BindWidget))
	class UButton* FirstButton;
	
};
