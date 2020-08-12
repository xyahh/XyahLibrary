// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "XyahMathLibrary.h"

FString UXyahMathLibrary::ToBinaryString(int32 Number, bool bIncludeLeftZeros)
{
	FString OutString;
	int32 BinaryLength= 32;
	if(false == bIncludeLeftZeros)
		BinaryLength = FMath::FloorLog2(Number) + 1;

	OutString.Reserve(BinaryLength);
	int32 CurrentNumber = 1;
	for (int32 i = 0; i < BinaryLength; ++i)
	{
		if (CurrentNumber & Number)
			OutString = TEXT("1") + OutString;
		else
			OutString = TEXT("0") + OutString;

		CurrentNumber *= 2;
		if(CurrentNumber > Number && bIncludeLeftZeros == false)
			break;
	}
	return OutString;
}

int32 UXyahMathLibrary::AddAssign_Integer(int32& Ref, int32 Amount)
{
	Ref += Amount;
	return Ref;
}

float UXyahMathLibrary::AddAssign_Float(float& Ref, float Amount)
{
	Ref += Amount;
	return Ref;
}

FVector UXyahMathLibrary::AddAssign_Vector(FVector& Ref, const FVector& Amount, bool bNormalize)
{
	Ref += Amount;
	if(bNormalize)
		Ref.Normalize();
	return Ref;
}

FRotator UXyahMathLibrary::AddAssign_Rotator(FRotator& Ref, const FRotator& Amount, bool bNormalize)
{
	Ref += Amount;
	if (bNormalize)
		Ref.Normalize();
	return Ref;
}

int32 UXyahMathLibrary::SubtractAssign_Integer(int32& Ref, int32 Amount)
{
	return AddAssign_Integer(Ref, -1 * Amount);
}

float UXyahMathLibrary::SubtractAssign_Float(float& Ref, float Amount)
{
	return AddAssign_Float(Ref, -1.f * Amount);
}

FVector UXyahMathLibrary::SubtractAssign_Vector(FVector& Ref, const FVector& Amount, bool bNormalize)
{
	return AddAssign_Vector(Ref, -1.f * Amount, bNormalize);
}

FRotator UXyahMathLibrary::SubtractAssign_Rotator(FRotator& Ref, const FRotator& Amount, bool bNormalize)
{
	return AddAssign_Rotator(Ref, -1.f * Amount, bNormalize);
}

int32 UXyahMathLibrary::MultiplyAssign_Integer(int32& Ref, int32 Amount)
{
	Ref *= Amount;
	return Ref;
}

float UXyahMathLibrary::MultiplyAssign_Float(float& Ref, float Amount)
{
	Ref *= Amount;
	return Ref;
}

FVector UXyahMathLibrary::MultiplyAssign_Vector(FVector& Ref, const FVector& Amount, bool bNormalize)
{
	Ref *= Amount;
	if(bNormalize)
		Ref.Normalize();
	return Ref;
}

int32 UXyahMathLibrary::DivideAssign_Integer(int32& Ref, int32 Amount)
{
	if(Amount != 0)
		Ref /= Amount;
	else
		XYAH_LIB_LOG(Warning, TEXT("Attempting to divide %d by zero!"), Ref);
	return Ref;
}

float UXyahMathLibrary::DivideAssign_Float(float& Ref, float Amount)
{
	if (FMath::Abs(Amount) > SMALL_NUMBER)
		Ref /= Amount;
	else
	{
		XYAH_LIB_LOG(Warning, TEXT("Attempting to divide %f by zero or a really small number!"), Ref);
	}
	return Ref;
}

FVector UXyahMathLibrary::DivideAssign_Vector(FVector& Ref, const FVector& Amount, bool bNormalize)
{
	if (FMath::Abs(Amount.X) > SMALL_NUMBER && FMath::Abs(Amount.Y) > SMALL_NUMBER && FMath::Abs(Amount.Z) > SMALL_NUMBER)
		Ref /= Amount;
	else
	{
		XYAH_LIB_LOG(Warning, TEXT("Attempting to divide %s by zero or a really small number!"), *Ref.ToString());
	}
	return Ref;
}

int32 UXyahMathLibrary::GetSum_Integer(const TArray<int32>& InArray)
{
	int32 Sum = 0;
	GetSum(InArray, Sum);
	return Sum;
}

float UXyahMathLibrary::GetSum_Float(const TArray<float>& InArray)
{
	float Sum = 0;
	GetSum(InArray, Sum);
	return Sum;
}

int32 UXyahMathLibrary::GetSum_Bool(const TArray<bool>& InArray)
{
	int32 Sum = 0;
	GetSum(InArray, Sum);
	return Sum;
}

FVector UXyahMathLibrary::GetSum_Vector(const TArray<FVector>& InArray)
{
	FVector Sum = FVector::ZeroVector;
	GetSum(InArray, Sum);
	return Sum;
}

float UXyahMathLibrary::GetAverage_Integer(const TArray<int32>& InArray)
{
	float Average = 0.f;
	GetAverage(InArray, Average);
	return Average;
}

float UXyahMathLibrary::GetAverage_Float(const TArray<float>& InArray)
{
	float Average = 0.f;
	GetAverage(InArray, Average);
	return Average;
}

float UXyahMathLibrary::GetAverage_Bool(const TArray<bool>& InArray)
{
	float Average = 0.f;
	GetAverage(InArray, Average);
	return Average;
}

FVector UXyahMathLibrary::GetAverage_Vector(const TArray<FVector>& InArray)
{
	FVector Average = FVector::ZeroVector;
	GetAverage(InArray, Average);
	return Average;
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