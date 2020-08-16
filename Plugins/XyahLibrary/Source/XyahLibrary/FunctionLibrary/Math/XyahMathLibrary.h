// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibraryCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//C++ & Blueprint
public:

	/*
	Converts a given Integer Number to a String expressing it in Binary Form
	@param Number - the Decimal Number to convert to Binary
	@param bIncludeLeftZeros - whether to include the left zeros (e.g. 00001010) or not (e.g. 1010)
	*/
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math")
	static FString ToBinaryString(int32 Number, bool bIncludeLeftZeros);

/* OPERATOR ADD ASSIGN  */
	
	//Increases the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Add Assign Integer", CompactNodeTitle = "+=", Keywords = "sum increase"))
	static int32 AddAssign_Integer(UPARAM(Ref) int32& Ref, int32 Amount);
	
	//Increases the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Add Assign Float", CompactNodeTitle = "+=", Keywords = "sum increase"))
	static float AddAssign_Float(UPARAM(Ref) float& Ref, float Amount);

	//Increases the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Add Assign Vector", CompactNodeTitle = "+=", Keywords = "sum increase"))
	static FVector AddAssign_Vector(UPARAM(Ref) FVector& Ref, const FVector& Amount, bool bNormalize);

	//Increases the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Rotator", meta = (DisplayName = "Add Assign Rotator", CompactNodeTitle = "+=", Keywords = "sum increase"))
	static FRotator AddAssign_Rotator(UPARAM(Ref) FRotator& Ref, const FRotator& Amount, bool bNormalize);

/* OPERATOR SUBTRACT ASSIGN  */

	//Decreases the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Subtract Assign Integer", CompactNodeTitle = "-=", Keywords = "minus decrease"))
	static int32 SubtractAssign_Integer(UPARAM(Ref) int32& Ref, int32 Amount);
	
	//Decreases the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Subtract Assign Float", CompactNodeTitle = "-=", Keywords = "minus decrease"))
	static float SubtractAssign_Float(UPARAM(Ref) float& Ref, float Amount);

	//Decreases the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Subtract Assign Vector", CompactNodeTitle = "-=", Keywords = "minus decrease"))
	static FVector SubtractAssign_Vector(UPARAM(Ref) FVector& Ref, const FVector& Amount, bool bNormalize);

	//Decreases the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Rotator", meta = (DisplayName = "Subtract Assign Rotator", CompactNodeTitle = "-=", Keywords = "minus decrease"))
	static FRotator SubtractAssign_Rotator(UPARAM(Ref) FRotator& Ref, const FRotator& Amount, bool bNormalize);

/* OPERATOR MULTIPLY ASSIGN */
	
	//Multiples the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Multiply Assign Integer", CompactNodeTitle = "*=", Keywords = "times"))
	static int32 MultiplyAssign_Integer(UPARAM(Ref) int32& Ref, int32 Amount);
	
	//Multiples the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Multiply Assign Float", CompactNodeTitle = "*=", Keywords = "times"))
	static float MultiplyAssign_Float(UPARAM(Ref) float& Ref, float Amount);

	//Multiples the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Multiply Assign Vector", CompactNodeTitle = "*=", Keywords = "times"))
	static FVector MultiplyAssign_Vector(UPARAM(Ref) FVector& Ref, const FVector& Amount, bool bNormalize);

/* OPERATOR DIVIDE ASSIGN */

	//Multiples the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Divide Assign Integer", CompactNodeTitle = "/="))
	static int32 DivideAssign_Integer(UPARAM(Ref) int32& Ref, int32 Amount);
	
	//Multiples the given Number (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Divide Assign Float", CompactNodeTitle = "/="))
	static float DivideAssign_Float(UPARAM(Ref) float& Ref, float Amount);

	//Multiples the given Vector (passed by ref) by the given amount 
	UFUNCTION(BlueprintCallable, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Divide Assign Vector", CompactNodeTitle = "/="))
	static FVector DivideAssign_Vector(UPARAM(Ref) FVector& Ref, const FVector& Amount, bool bNormalize);


/* SUM */

	//Gets the Sum of all the Elements in the Array. ResultSum must be initialized to Zero
	template<typename ElementType, typename SumType=ElementType>
	static void GetSum(const TArray<ElementType>& InArray, SumType& ResultSum);

	//Gets the Sum of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Sum Integer Array", CompactNodeTitle = "SUM", Keywords = "add"))
	static int32 GetSum_Integer(const TArray<int32>& InArray);

	//Gets the Sum of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Sum Float Array", CompactNodeTitle = "SUM", Keywords = "add"))
	static float GetSum_Float(const TArray<float>& InArray);

	//Gets the Sum of all the Elements in the Array. Treats True as 1 and False as 0
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Bool", meta = (DisplayName = "Sum Bool Array", CompactNodeTitle = "SUM", Keywords = "add"))
	static int32 GetSum_Bool(const TArray<bool>& InArray);

	//Gets the Sum of all the Elements in the Array.
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Sum Vector Array", CompactNodeTitle = "SUM", Keywords = "add"))
	static FVector GetSum_Vector(const TArray<FVector>& InArray);
	
/* AVERAGE */

	//Gets the Average of all the Elements in the Array. ResultAverage must be initialized to Zero
	template<typename ElementType, typename AverageType = float>
	static void GetAverage(const TArray<ElementType>& InArray, AverageType& ResultAverage);

	//Gets the Average of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Get Average Integer", CompactNodeTitle = "AVG", Keywords = "mean avg"))
	static float GetAverage_Integer(const TArray<int32>& InArray);

	//Gets the Average of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Get Average Float", CompactNodeTitle = "AVG", Keywords = "mean avg"))
	static float GetAverage_Float(const TArray<float>& InArray);

	//Gets the Average of all the Elements in the Array. Treats True as 1 and False as 0
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Bool", meta = (DisplayName = "Get Average Bool", CompactNodeTitle = "AVG", Keywords = "mean avg"))
	static float GetAverage_Bool(const TArray<bool>& InArray);

	//Gets the Average of all the Elements in the Array. No normalization takes place in this process.
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Get Average Vector", CompactNodeTitle = "AVG", Keywords = "mean avg"))
	static FVector GetAverage_Vector(const TArray<FVector>& InArray);

/* MEDIAN */

	template<typename ElementType>
	static float GetMedian(TArray<ElementType> InArray, bool bRequiresSort);

	//Gets the Median of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Get Median Integer"))
	static float GetMedian_Integer(const TArray<int32>& InArray, bool bRequiresSort);

	//Gets the Median of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Get Median Float"))
	static float GetMedian_Float(const TArray<float>& InArray, bool bRequiresSort);

	//Gets the Median of all the Elements in the Array. Treats True as 1 and False as 0
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Bool", meta = (DisplayName = "Get Median Bool"))
	static float GetMedian_Bool(const TArray<bool>& InArray, bool bRequiresSort);


/* RANGE */

	//Gets the Range of the Array. If Already Sorted, the Time Complexity is O(1), else it's O(n)
	template<typename ElementType>
	static void GetRange(TArray<ElementType> InArray, bool bIsSorted, ElementType& OutMin, ElementType& OutMax);

	//Gets the Range of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Integer", meta = (DisplayName = "Get Range Integer"))
	static void GetRange_Integer(const TArray<int32>& InArray, bool bIsSorted, int32& Min, int32& Max);

	//Gets the Range of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Float", meta = (DisplayName = "Get Range Float"))
	static void GetRange_Float(const TArray<float>& InArray, bool bIsSorted, float& Min, float& Max);

	//Gets the Range of all the Elements in the Array
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math|Vector", meta = (DisplayName = "Get Range Vector"))
	static void GetRange_Vector(const TArray<FVector>& InArray, FVector& Min, FVector& Max);
};


#if XYAH_MATH_LIBRARY
#include "XyahMathLibrary.inl"
#endif
