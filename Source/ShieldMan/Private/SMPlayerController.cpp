// Fill out your copyright notice in the Description page of Project Settings.


#include "SMPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void ASMPlayerController::BeginPlay()
{
	player1 = nullptr;
	player2 = nullptr;

	player3 = nullptr;

	//Super::BeginPlay();

	//if (HUDOverlayAsset)
	//{
	//	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	//}
	//HUDOverlay->AddToViewport();
	//HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
