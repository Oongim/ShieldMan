// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TitleLobbyScene.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UUI_TitleLobbyScene::Initialize()
{
	bool Success = Super::Initialize();
	if(!Success) return false;

	Ready_Right->SetVisibility(ESlateVisibility::Hidden);
	Ready_Body->SetVisibility(ESlateVisibility::Hidden);
	Ready_Left->SetVisibility(ESlateVisibility::Hidden);

	ChoiceButton_Right->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::ChoiceButton_Right_Clicked);
	ChoiceButton_Body->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::ChoiceButton_Body_Clicked);
	ChoiceButton_Left->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::ChoiceButton_Left_Clicked);

	Ready_Right->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::Ready_Right_Clicked);
	Ready_Body->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::Ready_Body_Clicked);
	Ready_Left->OnClicked.AddDynamic(this, &UUI_TitleLobbyScene::Ready_Left_Clicked);

	Player1_Name = FText::FromString(FString("KangDongKyun"));
	Player1->SetText(Player1_Name);

	bReadyRight = false;
	bReadyBody = false;
	bReadyLeft = false;

	return true;
}

void UUI_TitleLobbyScene::ChoiceButton_Right_Clicked()
{
	Ready_Right->SetVisibility(ESlateVisibility::Visible);
	ChoiceText_Right->SetText(Player1_Name);
	if (ChoiceText_Body->GetText().EqualTo(Player1_Name))
	{
		Ready_Body->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Body->SetText(FText::FromString(FString("Choice")));
		if(bReadyBody)
			Ready_Body_Clicked();
	}
	if (ChoiceText_Left->GetText().EqualTo(Player1_Name))
	{
		Ready_Left->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Left->SetText(FText::FromString(FString("Choice")));
		if (bReadyLeft)
			Ready_Left_Clicked();
	}
}

void UUI_TitleLobbyScene::ChoiceButton_Body_Clicked()
{
	Ready_Body->SetVisibility(ESlateVisibility::Visible);
	ChoiceText_Body->SetText(Player1_Name);
	if (ChoiceText_Right->GetText().EqualTo(Player1_Name))
	{
		Ready_Right->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Right->SetText(FText::FromString(FString("Choice")));
		if (bReadyRight)
			Ready_Right_Clicked();
	}
	if (ChoiceText_Left->GetText().EqualTo(Player1_Name))
	{
		Ready_Left->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Left->SetText(FText::FromString(FString("Choice")));
		if (bReadyLeft)
			Ready_Left_Clicked();
	}
}

void UUI_TitleLobbyScene::ChoiceButton_Left_Clicked()
{
	Ready_Left->SetVisibility(ESlateVisibility::Visible);
	ChoiceText_Left->SetText(Player1_Name);
	if (ChoiceText_Right->GetText().EqualTo(Player1_Name))
	{
		Ready_Right->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Right->SetText(FText::FromString(FString("Choice")));
		if (bReadyRight)
			Ready_Right_Clicked();
	}
	if (ChoiceText_Body->GetText().EqualTo(Player1_Name))
	{
		Ready_Body->SetVisibility(ESlateVisibility::Hidden);
		ChoiceText_Body->SetText(FText::FromString(FString("Choice")));
		if (bReadyBody)
			Ready_Body_Clicked();

	}
}

void UUI_TitleLobbyScene::Ready_Right_Clicked()
{
	if (!bReadyRight) {
		bReadyRight = true;
		ReadyText_Right->SetText(FText::FromString(FString("ReadyComplete")));
	}
	else
	{
		bReadyRight = false;
		ReadyText_Right->SetText(FText::FromString(FString("Ready")));
	}
}

void UUI_TitleLobbyScene::Ready_Body_Clicked()
{
	if (!bReadyBody) {
		bReadyBody = true;
		ReadyText_Body->SetText(FText::FromString(FString("ReadyComplete")));
	}
	else
	{
		bReadyBody = false;
		ReadyText_Body->SetText(FText::FromString(FString("Ready")));
	}
}

void UUI_TitleLobbyScene::Ready_Left_Clicked()
{
	if (!bReadyLeft) {
		bReadyLeft = true;
		ReadyText_Left->SetText(FText::FromString(FString("ReadyComplete")));
	}
	else
	{
		bReadyLeft = false;
		ReadyText_Left->SetText(FText::FromString(FString("Ready")));
	}
}
