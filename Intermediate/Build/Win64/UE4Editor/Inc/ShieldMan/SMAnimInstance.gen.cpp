// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShieldMan/Public/SMAnimInstance.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSMAnimInstance() {}
// Cross Module References
	SHIELDMAN_API UClass* Z_Construct_UClass_USMAnimInstance_NoRegister();
	SHIELDMAN_API UClass* Z_Construct_UClass_USMAnimInstance();
	ENGINE_API UClass* Z_Construct_UClass_UAnimInstance();
	UPackage* Z_Construct_UPackage__Script_ShieldMan();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
// End Cross Module References
	void USMAnimInstance::StaticRegisterNativesUSMAnimInstance()
	{
	}
	UClass* Z_Construct_UClass_USMAnimInstance_NoRegister()
	{
		return USMAnimInstance::StaticClass();
	}
	struct Z_Construct_UClass_USMAnimInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Hand_Left_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Hand_Left;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Hand_Right_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Hand_Right;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USMAnimInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAnimInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_ShieldMan,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USMAnimInstance_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "AnimInstance" },
		{ "IncludePath", "SMAnimInstance.h" },
		{ "ModuleRelativePath", "Public/SMAnimInstance.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Left_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "pawn" },
		{ "ModuleRelativePath", "Public/SMAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Left = { "Hand_Left", nullptr, (EPropertyFlags)0x0040000000000015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USMAnimInstance, Hand_Left), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Left_MetaData, ARRAY_COUNT(Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Left_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Right_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "pawn" },
		{ "ModuleRelativePath", "Public/SMAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Right = { "Hand_Right", nullptr, (EPropertyFlags)0x0040000000000015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USMAnimInstance, Hand_Right), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Right_MetaData, ARRAY_COUNT(Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Right_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USMAnimInstance_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Left,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USMAnimInstance_Statics::NewProp_Hand_Right,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_USMAnimInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USMAnimInstance>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_USMAnimInstance_Statics::ClassParams = {
		&USMAnimInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_USMAnimInstance_Statics::PropPointers,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		0,
		ARRAY_COUNT(Z_Construct_UClass_USMAnimInstance_Statics::PropPointers),
		0,
		0x009000A8u,
		METADATA_PARAMS(Z_Construct_UClass_USMAnimInstance_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_USMAnimInstance_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USMAnimInstance()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_USMAnimInstance_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(USMAnimInstance, 856006929);
	template<> SHIELDMAN_API UClass* StaticClass<USMAnimInstance>()
	{
		return USMAnimInstance::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_USMAnimInstance(Z_Construct_UClass_USMAnimInstance, &USMAnimInstance::StaticClass, TEXT("/Script/ShieldMan"), TEXT("USMAnimInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USMAnimInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
