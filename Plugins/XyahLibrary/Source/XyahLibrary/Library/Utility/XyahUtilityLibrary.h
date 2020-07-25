// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../XyahLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//Blueprints Only
protected:

	/*
	Gets all the Actors of a Given Class but first Filters out all the actors whose class belongs in the ClassesToIgnore set.
	Additionally, if present and valid, the Filter Function will be called for every actor filtered after the ClassesToIgnore. 
	If the Function returns true, the actor will be kept in the Output list, else it will be filtered out of the list.

	@param ClassesToIgnore - The Subclasses of the ActorClass to NOT consider and be left out.
	@param FilterFunctionName - The Name of the Function to call, if not specified, this extra Filtering Stage will not take place and return immediately. If specified this Function must be local (if FunctionOwner is null) or from the Function Owner (if specified)
	@param FunctionOwner - The owner of the Predicate Function to call. If null, the Function will be searched locally.

	@param OutActors - The filtered list of actors
	@return [bool] Whether GetAllActorsOfClass actually took place or if the given Filter Function was not found.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Utility", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "ActorClass", DynamicOutputParam = "OutActors", AutoCreateRefTerm = "ClassesToIgnore", AdvancedDisplay = "ClassesToIgnore,FilterFunctionName,FunctionOwner"))
	static bool GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors
		, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName = NAME_None, UObject* FunctionOwner = nullptr);

//Blueprints & C++ 
public:

	/*
	Gets the Class Default Object, as Mutable (use with Care!)
	@param ObjectClass - The class to get the Default Object from
	@param OutObject - Reference to the Default Object
	*/
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
	static void GetClassDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& OutObject);



	/* 
	Prints a String to either the Screen, Log, or both. 
	@param InString - The String to Print
	@param LogID - The ID or Key of the Log. -1 if to not use OR else the log with a an ID is going to be overwritten by the next log with the same ID.
	@param bPrintToScreen - Whether to Print to the Screen
	@param ScreenTextColor - The Text Color of the Log in the Screen
	@param bPrintToLog - Whether to Print to the OutputLog/Console
	@param ConsoleTextColor - The Color of the Log in the Console (NOTE: This is the Console that is opened with "-log" command, not the Editor one!).
	@param Duration - The duration of the Log, as long as it's not overwritten by same ID logs.
	@param bNewerOnTop - Whether to put this Log at the Top, or stack them below
	@param TextScale - the Scale of the Text (default 1.f)
	*/
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly))
	static void PrintMessage(const FString& Message = FString(TEXT("Xyah Library")), int32 LogID = -1
		, bool bPrintToScreen = true, FLinearColor ScreenTextColor = FLinearColor(0.0, 0.66, 1.0)
		, bool bPrintToLog = true, UPARAM(meta = (Bitmask, BitmaskEnum = "EXyahConsoleColor")) int32 ConsoleTextColor = 0x1111
		, float Duration = 2.f, bool bNewerOnTop = true, float TextScale = 1.f);

	/*
	Sets a Given Property belonging to the Given Object a value. The formatting of the String must be valid for the Property in question.
	@see XyahUtility's Implementations of String Conversion types for a safe conversion of core structs like Vector, Rotator, etc.
	*/
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility")
	static bool SetPropertyValue(UObject* OwnerObject, const FString& PropertyName, const FString& PropertyValue);

	//Gets a Given Property's Value belonging to the Given Object as String 	
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility")
	static bool GetPropertyValue(UObject* OwnerObject, const FString& PropertyName, FString& OutPropertyValue);

	//Formats a Vector to the String Format expected for the SetPropertyValue
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "V3toSTR"))
	static FString VectorToString(const FVector& V);
	
	//Formats a Rotator to the String Format expected for the SetPropertyValue
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "RtoSTR"))
	static FString RotatorToString(const FRotator& R);

	//Formats a Transform to the String Format expected for the SetPropertyValue
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "TtoSTR"))
	static FString TransformToString(const FTransform& T);

	//Formats a Vector2D to the String Format expected for the SetPropertyValue
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "V2toSTR"))
	static FString Vector2DToString(const FVector2D& V);

	//Parses the String from the GetPropertyValue to Vector
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "STRtoV3"))
	static bool StringToVector(const FString& S, FVector& V);

	//Parses the String from the GetPropertyValue to Rotator
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "STRtoR"))
	static bool StringToRotator(const FString& S, FRotator& R);

	//Parses the String from the GetPropertyValue to Transform
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "STRtoT"))
	static bool StringToTransform(const FString & S, FTransform & T);

	//Parses the String from the GetPropertyValue to FVector2D
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (CompactNodeTitle = "STRtoV2"))
	static bool StringToVector2D(const FString& S, FVector2D& V);


//C++ Only
public:

	//Checks whether two properties are of the same class, type, etc. Additional checks such as Offsets, Names are also available
	static bool CheckProperties(const FProperty* A, const FProperty* B, bool bCheckPropertyOffsets = false, bool bCheckPropertyNames = false);

	//A Convenience Func for the PrintMessage Func
	template<class... Args>
	static void Print(const FString& Format, Args&&... Arguments, int32 LogID = -1
		, bool bPrintToScreen = true, FLinearColor ScreenTextColor = FLinearColor(0.0, 0.66, 1.0)
		, bool bPrintToLog = true, UPARAM(meta = (Bitmask, BitmaskEnum = "EXyahConsoleColor")) int32 ConsoleTextColor = 0x1111
		, float Duration = 2.f, bool bNewerOnTop = true, float TextScale = 1.f);
	
	//Finds a Function from a Given Object
	static UFunction* FindFunction(const UObject* Object, FName FunctionName
		, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams);


//ExecFuncs
public:

	static void Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
		, TArray<AActor*>& OutActors
		, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName, UObject* FuncOwner);

public:

	DECLARE_FUNCTION(execGetAllActorsOfClass)
	{
		P_GET_OBJECT(UObject, WorldContextObject);
		P_GET_OBJECT(UClass, ActorClass);

		P_GET_TARRAY_REF(AActor*, OutActors);
		FArrayProperty* ActorProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);

		P_GET_TSET_REF(TSubclassOf<AActor>, ClassesToIgnore);
		P_GET_PROPERTY(FNameProperty, FilterFuncName);
		P_GET_PROPERTY(FObjectProperty, FuncOwner);
		P_FINISH;
		P_NATIVE_BEGIN;

		//No need to Mark property as dirty since we are getting TArrayRef
		Generic_GetAllActorsOfClass(WorldContextObject, ActorClass, ActorProperty ? ActorProperty->Inner : nullptr, OutActors, ClassesToIgnore, FilterFuncName, IsValid(FuncOwner) ? FuncOwner : XYAH_GET_OBJECT);
		P_NATIVE_END;
	}

};

#if XYAH_UTILITY_LIBRARY
#include "XyahUtilityLibrary.inl"
#endif
