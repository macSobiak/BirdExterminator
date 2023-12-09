// Fill out your copyright notice in the Description page of Project Settings.


#include "CounterWithLabel.h"
#include "Components/TextBlock.h"

void UCounterWithLabel::InitializeElement(const int& MaxValue, const int& CurrentValue)
{
	RefreshCurrentCount(CurrentValue);
	CounterMax->SetText(FText::AsNumber(MaxValue));
}

void UCounterWithLabel::RefreshCurrentCount(const int& CurrentValue)
{
	CurrentCounter->SetText(FText::AsNumber(CurrentValue));
}
