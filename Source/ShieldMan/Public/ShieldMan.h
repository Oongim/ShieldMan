// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Log.h"


enum EControlMode {
	BodyControlMode,
	RHandControlMode,
	LHandControlMode
};


/*
로그 출력

UE_LOG(LogTemp, Warning, TEXT("This is test"));

UE_LOG(LogTemp, Warning, TEXT("%f"), Player->GetActorLocation().X);

/////////////////////////////////////////////////////////////

Debug Logging Library 플러그인 

ULog::Hello();

ULog::Hello(LO_Viewport);

ULog::Invalid("Can't equip Shield", "", LO_Viewport);

ULog::Number(5, "The Number is: ");

ULog::Vector(GetActorLocation(), "MyActor location: ", "", LO_Viewport);

//현재 불린 곳의 클래스
ULog::Info(CUR_CLASS);

//현재 불린 클래스와 함수
ULog::Info(CUR_CLASS_FUNC);

//현재 불린 클래스, 함수, 몇번 째 라인인지
ULog::Info(CUR_CLASS_FUNC_WITH_LINE);

//현재 불린 곳의 함수 시그너쳐?
ULog::Info(CUR_FUNC_SIG);

....etc 조합 마음대로


 도큐먼트
http://alielsaleh.com/tools/UE4-Plugins/DebugLogLibrary/
		Build.cs에 추가해야 함
	PrivateDependencyModuleNames.AddRange(new string[] { "Debug"});
*/