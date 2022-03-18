// Fill out your copyright notice in the Description page of Project Settings.


#include "ZixuanCraftMainGameWidget.h"
#include "Components/TextBlock.h"

void UZixuanCraftMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Blueprint subclass
	if (ItemTitle)
	{
		ItemTitle->SetText(FText::FromString(TEXT("Hello world!")));
	}
}