// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../XyahLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/Core/PushModel/PushModel.h"
#include "XyahUtilityLibrary.generated.h"

class UXyahBaseSettings;

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

	/*
	Returns the Input as a String in the Simple Object Text Format.

	@param InValue - The Input
	@param OutString - The Input Value in String (Simple Object Text) Format.
	@return [bool] Whether the operation was successful
	*/
	UFUNCTION(BlueprintPure, CustomThunk, Category = "XyahLibrary|Utility", meta = (CustomStructureParam = "InValue"))
	static bool ToString(int32 InValue, FString& OutString);

	/*
	Sets the Referenced Variable the Value given in the String.

	@param InString - The String Containing the Value
	@param RefValue - The Variable to Modify based on the String's value
	@return [bool] Whether the operation was successful
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "XyahLibrary|Utility", meta = (CustomStructureParam = "RefValue"))
	static bool FromString(const FString& InString, UPARAM(Ref) int32& RefValue);

//Blueprints & C++ 
public:

	/*
	Gets the Class Default Object, as Mutable (use with Care!)
	@param ObjectClass - The class to get the Default Object from
	@param OutObject - Reference to the Default Object
	*/
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
	static void GetClassDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& OutObject);

	//Same as GetClassDefaultObject, but for Settings Only
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DeterminesOutputType = "SettingsClass", DynamicOutputParam = "OutSettings"))
	static void GetSettings(TSubclassOf<UXyahBaseSettings> SettingsClass, UXyahBaseSettings*& OutSettings);

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
		, bool bPrintToLog = true, int32 ConsoleTextColor = 15
		, float Duration = 2.f, bool bNewerOnTop = true, float TextScale = 1.f);

	/*
	Sets a Given Property belonging to the Given Object a value. The formatting of the String must be in Simple Object Text Format
	@see ToString
	@see FromString
	*/
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility")
	static bool SetPropertyValue(UObject* OwnerObject, const FString& PropertyName, const FString& PropertyValue);

	//Gets a Given Property's Value belonging to the Given Object as String in the Simple Object Text Format	
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility")
	static bool GetPropertyValue(UObject* OwnerObject, const FString& PropertyName, FString& OutPropertyValue);

//C++ Only
public:

	//Checks whether two properties are of the same class, type, etc. Additional checks such as Offsets, Names are also available
	static bool CheckProperties(const FProperty* A, const FProperty* B, bool bCheckPropertyOffsets = false, bool bCheckPropertyNames = false);

	//A Convenience Func for the PrintMessage Func
	template<class... Args>
	static void Print(const FString& Format, Args&&... Arguments, int32 LogID = -1
		, bool bPrintToScreen = true, FLinearColor ScreenTextColor = FLinearColor(0.0, 0.66, 1.0)
		, bool bPrintToLog = true, int32 ConsoleTextColor = 15
		, float Duration = 2.f, bool bNewerOnTop = true, float TextScale = 1.f);
	
	//Finds a Function from a Given Object
	static UFunction* FindFunction(const UObject* Object, FName FunctionName
		, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams);


//ExecFuncs
public:

	static void Generic_GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FProperty* ArrayInnerProperty
		, TArray<AActor*>& OutActors
		, const TSet<TSubclassOf<AActor>>& ClassesToIgnore, FName FilterFunctionName, UObject* FuncOwner);

	static bool Generic_ToString(FProperty* Property, void* Data, FString& OutString);
	static bool Generic_FromString(const FString& InString, FProperty* Property, void* Data);

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

	DECLARE_FUNCTION(execToString)
	{
		Stack.Step(Stack.Object, NULL);
		FProperty* InProperty = Stack.MostRecentProperty;
		void* Data = Stack.MostRecentPropertyAddress;

		PARAM_PASSED_BY_REF(OutString, FStrProperty, FString);

		P_FINISH;
		P_NATIVE_BEGIN;
		XYAH_RETURN(bool, Generic_ToString(InProperty, Data, OutString));
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execFromString)
	{
		P_GET_PROPERTY(FStrProperty, InString);

		Stack.Step(Stack.Object, NULL);
		FProperty* RefProperty = Stack.MostRecentProperty;
		void* Data = Stack.MostRecentPropertyAddress;
		P_FINISH;

		P_NATIVE_BEGIN;
		XYAH_MARK_PROPERTY_DIRTY(RefProperty);
		XYAH_RETURN(bool, Generic_FromString(InString, RefProperty, Data));
		P_NATIVE_END;
	}
};

#if XYAH_UTILITY_LIBRARY
#include "XyahUtilityLibrary.inl"
#endif
