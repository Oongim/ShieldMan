// Fill out your copyright notice in the Description page of Project Settings.


#include "SMUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASMUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (UIWidgetClass == nullptr)
	//	ULog::Invalid("UIWidgetClass", "", LO_Viewport);
	//UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);

	////if (UIWidgetInstance == nullptr)
	//	//ULog::Invalid("UIWidgetInstance", "", LO_Viewport);
	//UIWidgetInstance->AddToViewport();

	//FInputModeUIOnly Mode;
	//Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	//SetInputMode(Mode);
	//bShowMouseCursor = true;

}
