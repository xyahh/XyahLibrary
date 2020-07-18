// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

/* XYAH LOGS */
#define XYAH(Lib) UXyah##Lib##Library::
DECLARE_LOG_CATEGORY_EXTERN(XyahLog, Log, All);
#define XYAH_LOG(Verbosity, Format, ...) UE_LOG(XyahLog, Verbosity, Format, __VA_ARGS__)

/* XYAH LIBRARY DEFINITIONS */
#define XYAH_UTILITY_LIBRARY	1
#define XYAH_MATH_LIBRARY		1
#define XYAH_ARRAY_LIBRARY		1

/* XYAH TYPES */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EXyahConsoleColor
{
	XCC_Dark = 0 UMETA(DisplayName = "Dark"),
	XCC_Red = 1 UMETA(DisplayName = "Red"),
	XCC_Green = 2 UMETA(DisplayName = "Green"),
	XCC_Blue = 4 UMETA(DisplayName = "Blue"),
	XCC_Light = 8 UMETA(DisplayName = "Light"),
};


/* XYAH HELPER MACROS */
#define XYAH_GET_PROPERTY_AND_ADDRESS(PropertyType, AddrName, PropertyName) Stack.MostRecentProperty = nullptr; \
	Stack.StepCompiledIn<PropertyType>(nullptr); \
	void* AddrName = Stack.MostRecentPropertyAddress; \
	PropertyType* PropertyName = CastField<PropertyType>(Stack.MostRecentProperty);

#define XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty) if (!ArrayProperty) \
	{\
		Stack.bArrayContextFailed = true;\
		return;\
	}

#define XYAH_MARK_PROPERTY_DIRTY(Property) MARK_PROPERTY_DIRTY(Stack.Object, Property)
#define XYAH_RETURN(ResultType, Func) *(ResultType*)RESULT_PARAM = Func
#define XYAH_GET_OBJECT	Stack.Object
#define XYAH_SHOULD_NEVER_HIT_THIS(...) { check(0); return __VA_ARGS__; } 