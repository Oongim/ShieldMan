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

	Player1_Name = FText::FromString(FString("KDK"));
	Player2_Name = FText::FromString(FString("JMS"));
	Player3_Name = FText::FromString(FString("LDW"));

	Player1->SetText(Player1_Name);
	Player2->SetText(Player2_Name);
	Player3->SetText(Player3_Name);

	bReadyRight = false;
	bReadyBody = false;
	bReadyLeft = false;

	leftpick = -1;
	bodypick = -1;
	rightpick = -1;

	return true;
}

void UUI_TitleLobbyScene::ChoiceButton_Right_Clicked()
{
	Ready_Right->SetVisibility(ESlateVisibility::Visible);

	if (rightpick == 0)
		ChoiceText_Right->SetText(Player1_Name);
	else if (rightpick == 1)
		ChoiceText_Right->SetText(Player2_Name);
	else if (rightpick == 2)
		ChoiceText_Right->SetText(Player3_Name);
	else
		ChoiceText_Right->SetText(FText::FromString(FString("")));

	Ready_Body->SetVisibility(ESlateVisibility::Hidden);

	bodypick = -1;
	ChoiceText_Body->SetText(FText::FromString(FString("")));

	if (bReadyBody)
		Ready_Body_Clicked();

	Ready_Left->SetVisibility(ESlateVisibility::Hidden);
	leftpick = -1;
	ChoiceText_Left->SetText(FText::FromString(FString("")));
	if (bReadyLeft)
		Ready_Left_Clicked();

}

void UUI_TitleLobbyScene::ChoiceButton_Body_Clicked()
{
	Ready_Body->SetVisibility(ESlateVisibility::Visible);

	if (bodypick == 0)
		ChoiceText_Body->SetText(Player1_Name);
	else if (bodypick == 1)
		ChoiceText_Body->SetText(Player2_Name);
	else if (bodypick == 2)
		ChoiceText_Body->SetText(Player3_Name);
	else
		ChoiceText_Body->SetText(FText::FromString(FString("")));
	UE_LOG(LogTemp, Warning, TEXT("bodypick = %d"), bodypick);

	Ready_Right->SetVisibility(ESlateVisibility::Hidden);

	rightpick = -1;
	ChoiceText_Right->SetText(FText::FromString(FString("")));

	if (bReadyRight)
		Ready_Right_Clicked();

	Ready_Left->SetVisibility(ESlateVisibility::Hidden);

	leftpick = -1;
	ChoiceText_Left->SetText(FText::FromString(FString("")));

	if (bReadyLeft)
		Ready_Left_Clicked();

}

void UUI_TitleLobbyScene::ChoiceButton_Left_Clicked()
{
	Ready_Left->SetVisibility(ESlateVisibility::Visible);

	if (leftpick == 0)
		ChoiceText_Left->SetText(Player1_Name);
	else if (leftpick == 1)
		ChoiceText_Left->SetText(Player2_Name);
	else if (leftpick == 2)
		ChoiceText_Left->SetText(Player3_Name);
	else
		ChoiceText_Left->SetText(FText::FromString(FString("")));


	Ready_Right->SetVisibility(ESlateVisibility::Hidden);

	rightpick = -1;
	ChoiceText_Right->SetText(FText::FromString(FString("")));

	if (bReadyRight)
		Ready_Right_Clicked();

	Ready_Body->SetVisibility(ESlateVisibility::Hidden);

	bodypick = -1;
	ChoiceText_Body->SetText(FText::FromString(FString("")));

	if (bReadyBody)
		Ready_Body_Clicked();


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

void UUI_TitleLobbyScene::Set_LeftPick(int pick)
{
	leftpick = pick;
}

void UUI_TitleLobbyScene::Set_RightPick(int pick)
{
	rightpick = pick;
}
void UUI_TitleLobbyScene::Set_BodyPick(int pick)
{
	bodypick = pick;
	UE_LOG(LogTemp, Warning, TEXT("bodypick(%d), pick(%d)"), bodypick, pick);
}