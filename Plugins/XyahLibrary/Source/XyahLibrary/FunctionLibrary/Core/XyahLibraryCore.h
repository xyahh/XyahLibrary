// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "XyahLibraryCore.generated.h"

#define XYAH(Lib) UXyah##Lib##Library::

/* XYAH LOGS */
DECLARE_LOG_CATEGORY_EXTERN(XyahLibLog, Log, All);
#define XYAH_LIB_LOG(Verbosity, Format, ...) UE_LOG(XyahLibLog, Verbosity, Format, __VA_ARGS__)

/* XYAH LIBRARY DEFINITIONS */
#define XYAH_UTILITY_LIBRARY	1
#define XYAH_MATH_LIBRARY		1
#define XYAH_ARRAY_LIBRARY		1
#define XYAH_ACTOR_LIBRARY		1
#define XYAH_MAP_LIBRARY		1


/* XYAH TYPES */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EXyahConsoleColor
{
	XCC_Dark	= 0			UMETA(Hidden),
	XCC_Red		= 1 << 0	UMETA(DisplayName = "Red"),
	XCC_Green	= 1	<< 1	UMETA(DisplayName = "Green"),
	XCC_Blue	= 1	<< 2	UMETA(DisplayName = "Blue"),
	XCC_Light	= 1	<< 3	UMETA(DisplayName = "Light"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EXyahPropertyFlags
{
	XPF_All						= 0		 UMETA(Hidden),
	XPF_DisableEditOnInstance	= 1 << 0 UMETA(DisplayName = "Instance Edit Disabled", ToolTip = "Property cannot be edited in an instance of this class."),
	XPF_BlueprintVisible		= 1 << 1 UMETA(DisplayName = "Blueprint Visible", ToolTip = "Property can be read by blueprint code."),
	XPF_BlueprintReadOnly		= 1 << 2 UMETA(DisplayName = "Blueprint ReadOnly", ToolTip = "Property cannot be modified by blueprint code."),
	XPF_Deprecated				= 1 << 3 UMETA(DisplayName = "Deprecated", ToolTip = "Property is deprecated.  Read it from an archive, but don't save it."),
	XPF_Net						= 1 << 4 UMETA(DisplayName = "Network Relevant", ToolTip = "Property is relevant to network replication."),
	XPF_SaveGame				= 1 << 5 UMETA(DisplayName = "SaveGame", ToolTip = "Property should be serialized for save games, this is only checked for game-specific archives with ArIsSaveGame."),
	XPF_Config					= 1 << 6 UMETA(DisplayName = "Config", ToolTip = "Property should be loaded/saved as permanent profile."),
	XPF_Transient				= 1 << 7 UMETA(DisplayName = "Transient", ToolTip = "Property is transient: shouldn't be saved or loaded, except for Blueprint CDOs.")
};

USTRUCT(BlueprintType)
struct FXyahPrintSettings
{
	GENERATED_BODY()

public:

	FXyahPrintSettings() 
		: LogID(-1), bPrintToScreen(true), ScreenTextColor(0.0, 0.66, 1.0)
		, bPrintToLog(true), ConsoleTextColor(15)
		, Duration(2.f), bNewerOnTop(true), TextScale(1.f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LogID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPrintToScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ScreenTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPrintToLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EXyahConsoleColor"))
	int32 ConsoleTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNewerOnTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TextScale;
};

/* XYAH GLOBAL FUNCS */

//Converts EXyahPropertyFlags to EPropertyFlags 
uint64 ConvertXyahPropertyFlags(int32 XyahPropertyFlags);


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