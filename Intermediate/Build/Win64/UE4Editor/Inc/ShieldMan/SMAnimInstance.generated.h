// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SHIELDMAN_SMAnimInstance_generated_h
#error "SMAnimInstance.generated.h already included, missing '#pragma once' in SMAnimInstance.h"
#endif
#define SHIELDMAN_SMAnimInstance_generated_h

#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_RPC_WRAPPERS
#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_RPC_WRAPPERS_NO_PURE_DECLS
#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSMAnimInstance(); \
	friend struct Z_Construct_UClass_USMAnimInstance_Statics; \
public: \
	DECLARE_CLASS(USMAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/ShieldMan"), NO_API) \
	DECLARE_SERIALIZER(USMAnimInstance)


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_INCLASS \
private: \
	static void StaticRegisterNativesUSMAnimInstance(); \
	friend struct Z_Construct_UClass_USMAnimInstance_Statics; \
public: \
	DECLARE_CLASS(USMAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/ShieldMan"), NO_API) \
	DECLARE_SERIALIZER(USMAnimInstance)


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USMAnimInstance(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USMAnimInstance) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USMAnimInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USMAnimInstance); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USMAnimInstance(USMAnimInstance&&); \
	NO_API USMAnimInstance(const USMAnimInstance&); \
public:


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USMAnimInstance(USMAnimInstance&&); \
	NO_API USMAnimInstance(const USMAnimInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USMAnimInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USMAnimInstance); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(USMAnimInstance)


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__Hand_Right() { return STRUCT_OFFSET(USMAnimInstance, Hand_Right); } \
	FORCEINLINE static uint32 __PPO__Hand_Left() { return STRUCT_OFFSET(USMAnimInstance, Hand_Left); }


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_12_PROLOG
#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_RPC_WRAPPERS \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_INCLASS \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_INCLASS_NO_PURE_DECLS \
	ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SHIELDMAN_API UClass* StaticClass<class USMAnimInstance>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ShieldMan_Source_ShieldMan_Public_SMAnimInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
