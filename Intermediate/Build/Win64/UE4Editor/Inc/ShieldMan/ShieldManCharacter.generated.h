// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SHIELDMAN_ShieldManCharacter_generated_h
#error "ShieldManCharacter.generated.h already included, missing '#pragma once' in ShieldManCharacter.h"
#endif
#define SHIELDMAN_ShieldManCharacter_generated_h

#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_RPC_WRAPPERS
#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_RPC_WRAPPERS_NO_PURE_DECLS
#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAShieldManCharacter(); \
	friend struct Z_Construct_UClass_AShieldManCharacter_Statics; \
public: \
	DECLARE_CLASS(AShieldManCharacter, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ShieldMan"), NO_API) \
	DECLARE_SERIALIZER(AShieldManCharacter)


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_INCLASS \
private: \
	static void StaticRegisterNativesAShieldManCharacter(); \
	friend struct Z_Construct_UClass_AShieldManCharacter_Statics; \
public: \
	DECLARE_CLASS(AShieldManCharacter, ACharacter, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ShieldMan"), NO_API) \
	DECLARE_SERIALIZER(AShieldManCharacter)


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AShieldManCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AShieldManCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AShieldManCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AShieldManCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AShieldManCharacter(AShieldManCharacter&&); \
	NO_API AShieldManCharacter(const AShieldManCharacter&); \
public:


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AShieldManCharacter(AShieldManCharacter&&); \
	NO_API AShieldManCharacter(const AShieldManCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AShieldManCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AShieldManCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AShieldManCharacter)


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__SpringArm() { return STRUCT_OFFSET(AShieldManCharacter, SpringArm); } \
	FORCEINLINE static uint32 __PPO__Camera() { return STRUCT_OFFSET(AShieldManCharacter, Camera); } \
	FORCEINLINE static uint32 __PPO__AnimInstance() { return STRUCT_OFFSET(AShieldManCharacter, AnimInstance); } \
	FORCEINLINE static uint32 __PPO__Hand_Right() { return STRUCT_OFFSET(AShieldManCharacter, Hand_Right); } \
	FORCEINLINE static uint32 __PPO__Hand_Left() { return STRUCT_OFFSET(AShieldManCharacter, Hand_Left); }


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_13_PROLOG
#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_PRIVATE_PROPERTY_OFFSET \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_RPC_WRAPPERS \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_INCLASS \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_PRIVATE_PROPERTY_OFFSET \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_INCLASS_NO_PURE_DECLS \
	ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SHIELDMAN_API UClass* StaticClass<class AShieldManCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ShieldMan_Source_ShieldMan_Public_ShieldManCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
