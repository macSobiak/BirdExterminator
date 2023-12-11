// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CounterWithLabel.generated.h"

/**
 * Class for a widget that holds Counter values and a Label besides it
 */
UCLASS()
class BIRDEXTERMINATOR_API UCounterWithLabel : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentCounter;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CounterMax;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Label;

	virtual void InitializeElement(const int& MaxValue, const int& CurrentValue, const FString& LabelText);
	virtual void RefreshCurrentCount(const int& CurrentValue);
};
