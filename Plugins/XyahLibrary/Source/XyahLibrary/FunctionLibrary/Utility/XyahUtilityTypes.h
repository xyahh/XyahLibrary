// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahUtilityTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EXyahConsoleColor
{
	XCC_Dark = 0 UMETA(Hidden),
	XCC_Red = 1 << 0 UMETA(DisplayName = "Red"),
	XCC_Green = 1 << 1 UMETA(DisplayName = "Green"),
	XCC_Blue = 1 << 2 UMETA(DisplayName = "Blue"),
	XCC_Light = 1 << 3 UMETA(DisplayName = "Light"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EXyahPropertyFlags
{
	XPF_None = 0	UMETA(Hidden),
	XPF_DisableEditOnInstance = 1 << 0 UMETA(DisplayName = "Instance Edit Disabled", ToolTip = "Property cannot be edited in an instance of this class."),
	XPF_BlueprintVisible = 1 << 1 UMETA(DisplayName = "Blueprint Visible", ToolTip = "Property can be read by blueprint code."),
	XPF_BlueprintReadOnly = 1 << 2 UMETA(DisplayName = "Blueprint ReadOnly", ToolTip = "Property cannot be modified by blueprint code."),
	XPF_Deprecated = 1 << 3 UMETA(DisplayName = "Deprecated", ToolTip = "Property is deprecated.  Read it from an archive, but don't save it."),
	XPF_Net = 1 << 4 UMETA(DisplayName = "Network Relevant", ToolTip = "Property is relevant to network replication."),
	XPF_SaveGame = 1 << 5 UMETA(DisplayName = "SaveGame", ToolTip = "Property should be serialized for save games, this is only checked for game-specific archives with ArIsSaveGame."),
	XPF_Config = 1 << 6 UMETA(DisplayName = "Config", ToolTip = "Property should be loaded/saved as permanent profile."),
	XPF_Transient = 1 << 7 UMETA(DisplayName = "Transient", ToolTip = "Property is transient: shouldn't be saved or loaded, except for Blueprint CDOs.")
};

UENUM(BlueprintType)
enum EXyahDirectoryType
{
	XDT_Absolute UMETA(DisplayName = "Absolute", ToolTip = "The absolute location of the directory."),
	XDT_Game UMETA(DisplayName = "Game", ToolTip = "The path relative to the Game (Project) directory."),
	XDT_Saved UMETA(DisplayName = "Saved", ToolTip = "The path relative to the Saved directory."),
};

USTRUCT(BlueprintType)
struct FXyahPrint
{
	GENERATED_BODY()

public:

	FXyahPrint()
		: LogID(-1), bPrintToScreen(true), ScreenTextColor(0.0, 0.66, 1.0)
		, bPrintToLog(true), ConsoleTextColor(15)
		, Duration(2.f), bNewerOnTop(true), TextScale(1.f)
	{}

	FXyahPrint(int32 LogID, const FLinearColor& Color = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f)
		: LogID(LogID), bPrintToScreen(true), ScreenTextColor(Color)
		, bPrintToLog(true), ConsoleTextColor(15)
		, Duration(Duration), bNewerOnTop(true), TextScale(1.f) 
	{}

	FXyahPrint(int32 LogID, bool bPrintToScreen, const FLinearColor& ScreenTextColor
		, bool bPrintToLog, int32 ConsoleTextColor, float Duration, bool bNewerOnTop, float TextScale)
		: LogID(LogID), bPrintToScreen(bPrintToScreen), ScreenTextColor(ScreenTextColor)
		, bPrintToLog(bPrintToLog), ConsoleTextColor(ConsoleTextColor)
		, Duration(Duration), bNewerOnTop(bNewerOnTop), TextScale(TextScale)
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


//Converts EXyahPropertyFlags to EPropertyFlags 
FORCEINLINE uint64 ConvertXyahPropertyFlags(int32 XyahPropertyFlags)
{
#define CONVERT(PropertyFlag) \
	if(XyahPropertyFlags & static_cast<int32>(EXyahPropertyFlags::XPF_##PropertyFlag)) \
		OutPropertyFlags |= static_cast<uint64>(EPropertyFlags::CPF_##PropertyFlag);

	uint64 OutPropertyFlags = 0;

	CONVERT(DisableEditOnInstance);
	CONVERT(BlueprintVisible);
	CONVERT(BlueprintReadOnly);
	CONVERT(Deprecated);
	CONVERT(Net);
	CONVERT(SaveGame);
	CONVERT(Config);
	CONVERT(Transient);

	return OutPropertyFlags;
}