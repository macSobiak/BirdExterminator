// Fill out your copyright notice in the Description page of Project Settings.


#include "PromptWithButtons.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPromptWithButtons::InitializePrompt(const FString &PromptMessage)
{
	PromptMessageBlock->SetText(FText::FromString(PromptMessage));

	FirstButton->OnClicked.AddDynamic(this, &UPromptWithButtons::FirstButtonClicked);
}

void UPromptWithButtons::NativeDestruct()
{
	Super::NativeDestruct();
	FirstButton->OnClicked.RemoveAll(this);
}

void UPromptWithButtons::FirstButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}
