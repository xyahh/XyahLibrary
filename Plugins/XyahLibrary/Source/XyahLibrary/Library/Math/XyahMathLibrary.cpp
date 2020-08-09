// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahMathLibrary.h"

FString UXyahMathLibrary::ToBinaryString(int32 Number, bool bIncludeLeftZeros)
{
	FString OutString;
	if(bIncludeLeftZeros)
		OutString.AppendChars(TEXT("0"), 32); //int32 would have 32 entries
	else
		OutString.AppendChars(TEXT("0"), FMath::FloorLog2(Number) + 1);

	int32 CurrentNumber = 1;
	for (int32 i = 0; i < 32; ++i)
	{
		if (CurrentNumber & Number)
			OutString[31 - i] = TEXT('1');
		CurrentNumber *= 2;
		if(CurrentNumber > Number && bIncludeLeftZeros == false)
			break;
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

void UXyahMathLibrary::GetRange_Vector(const TArray<FVector>& InArray, FVector& Min, FVector& Max)
{
	if (InArray.Num() > 0)
	{
		Min = Max = InArray[0];
		for (auto& i : InArray)
		{
			Max.X = FMath::Max(i.X, Max.X);
			Max.Y = FMath::Max(i.Y, Max.Y);
			Max.Z = FMath::Max(i.Z, Max.Z);

			Min.X = FMath::Min(i.X, Min.X);
			Min.Y = FMath::Min(i.Y, Min.Y);
			Min.Z = FMath::Min(i.Z, Min.Z);
		}
	}
}