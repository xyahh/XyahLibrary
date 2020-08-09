// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XyahMathLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XYAHLIBRARY_API UXyahMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Converts a given Integer Number to a String expressing the Number in Binary Form
	/*
	Converts a given Integer Number to a String expressing it in Binary Form
	@param Number - the Decimal Number to convert to Binary
	@param bIncludeLeftZeros - whether to include the left zeros (e.g. 00001010) or not (e.g. 1010)
	*/
	UFUNCTION(BlueprintPure, Category = "XyahLibrary|Math")
	static FString ToBinaryString(int32 Number, bool bIncludeLeftZeros);

/* SUM */

	template<typename ElementType, typename SumType=ElementType>
	static SumType GetSum(const TArray<ElementType>& InArray);

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

	template<typename ElementType, typename AverageType = float>
	static AverageType GetAverage(const TArray<ElementType>& InArray);

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
