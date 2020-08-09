// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/Core/PushModel/PushModel.h"
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

	/*
	Gets the Class Default Object, as Mutable (use with Care!)
	@param ObjectClass - The class to get the Default Object from
	@param OutObject - Reference to the Default Object
	*/
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DeterminesOutputType = "ObjectClass", DynamicOutputParam = "OutObject"))
	static void GetClassDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& OutObject);

	//Same as GetClassDefaultObject, but for Settings Only
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Utility", meta = (DeterminesOutputType = "SettingsClass", DynamicOutputParam = "OutSettings"))
	static void GetSettings(TSubclassOf<UXyahSettings> SettingsClass, UXyahSettings*& OutSettings);

//Blueprints & C++ 
public:

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
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (Keywords = "log print", AdvancedDisplay="1", AutoCreateRefTerm="Settings", DevelopmentOnly))
	static void PrintMessage(const FString& Message, const FXyahPrintSettings& Settings);


	/*
	Gets all the Properties and their values of a Given UObject.
	@param OwnerObject - the Object whose properties to get
	@param ExcludeParents - The parents of the Object whose properties to ignore
	@param PropertyFlags - At least 1 flag must be satisfied by a property for it to be considered. If no flags are selected, no check will take place and all properties will be considered. 
	@param OutProperties - Result Map of the Property Names (Keys) and Property Values (Values)
	*/
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Utility", meta = (AutoCreateRefTerm = "ExcludeParents", AdvancedDisplay = "bIncludeReplicated,bIncludeNonReplicated"))
	static void GetProperties(UObject* OwnerObject, const TSet<TSubclassOf<UObject>>& ExcludeParents
	, TMap<FString, FString>& OutProperties
	, UPARAM(meta = (Bitmask, BitmaskEnum = "EXyahPropertyFlags")) int32 PropertyFlags = 2);

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

//Blueprint/C++ Internal Funcs
protected:

	static bool GetPropertyValue_Internal(UObject* OwnerObject, FProperty* Property, FString& OutPropertyValue);

//C++ Only
public:

	//Checks whether two properties are of the same class, type, etc. Additional checks such as Offsets, Names are also available
	static bool CheckProperties(const FProperty* A, const FProperty* B
	, bool bCheckPropertyOffsets = false, bool bCheckPropertyNames = false);

	//A Convenience Func for the PrintMessage Func
	template<class... Args>
	static void Print(const FXyahPrintSettings& Settings, const FString& Format, Args&&... Arguments);
	
	//Finds a Function from a Given Object
	static UFunction* FindFunction(const UObject* Object, FName FunctionName
		, const FString& ErrorString, FProperty* InnerProperty, int32 ExpectedInputParams);


//Generic Funcs
public:

	static bool Generic_ToString(FProperty* Property, void* Data, FString& OutString);
	static bool Generic_FromString(const FString& InString, FProperty* Property, void* Data);

//Exec Funcs
public:

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
