// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahMathLibrary.h"

FString UXyahMathLibrary::ToBinaryString(int32 Number)
{
	FString OutString(TEXT("00000000000000000000000000000000"));
	int32 CurrentNumber = 1;
	for (int32 i = 0; i < 32 && CurrentNumber <= Number; ++i)
	{
		if (CurrentNumber & Number)
			OutString[31 - i] = TEXT('1');
		CurrentNumber *= 2;
	}
	return OutString;
}

int32 UXyahMathLibrary::GetSum_Integer(const TArray<int32>& InArray)
{
	return GetSum(InArray);
}

float UXyahMathLibrary::GetSum_Float(const TArray<float>& InArray)
{
	return GetSum(InArray);
}

int32 UXyahMathLibrary::GetSum_Bool(const TArray<bool>& InArray)
{
	return GetSum<bool, int32>(InArray);
}

FVector UXyahMathLibrary::GetSum_Vector(const TArray<FVector>& InArray)
{
	return GetSum(InArray);
}

float UXyahMathLibrary::GetAverage_Integer(const TArray<int32>& InArray)
{
	return GetAverage(InArray);
}

float UXyahMathLibrary::GetAverage_Float(const TArray<float>& InArray)
{
	return GetAverage(InArray);
}

float UXyahMathLibrary::GetAverage_Bool(const TArray<bool>& InArray)
{
	return GetAverage(InArray);
}

FVector UXyahMathLibrary::GetAverage_Vector(const TArray<FVector>& InArray)
{
	return GetAverage<FVector, FVector>(InArray);
}

float UXyahMathLibrary::GetMedian_Integer(const TArray<int32>& InArray, bool bRequiresSort)
{
	return GetMedian(InArray, bRequiresSort);
}

float UXyahMathLibrary::GetMedian_Float(const TArray<float>& InArray, bool bRequiresSort)
{
	return GetMedian(InArray, bRequiresSort);
}

float UXyahMathLibrary::GetMedian_Bool(const TArray<bool>& InArray, bool bRequiresSort)
{
	return GetMedian(InArray, bRequiresSort);
}

void UXyahMathLibrary::GetRange_Integer(const TArray<int32>& InArray, bool bIsSorted, int32& Min, int32& Max)
{
	GetRange(InArray, bIsSorted, Min, Max);
}

void UXyahMathLibrary::GetRange_Float(const TArray<float>& InArray, bool bIsSorted, float& Min, float& Max)
{
	GetRange(InArray, bIsSorted, Min, Max);
}

void UXyahMathLibrary::SplitVectorArray(const TArray<FVector>& InArray, TArray<float>& X, TArray<float>& Y, TArray<float>& Z)
{
	const int32 Count = InArray.Num();

	if (Count > 0)
	{
		X.Init(0.f, Count);
		Y.Init(0.f, Count);
		Z.Init(0.f, Count);
		for (int32 i = 0; i < Count; ++i)
		{
			X[i] = InArray[i].X;
			Y[i] = InArray[i].Y;
			Z[i] = InArray[i].Z;
		}
	}
	
}

