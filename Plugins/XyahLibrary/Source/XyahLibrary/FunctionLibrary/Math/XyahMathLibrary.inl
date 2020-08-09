// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

template<typename ElementType, typename SumType>
void UXyahMathLibrary::GetSum(const TArray<ElementType>& InArray, SumType& ResultSum)
{
	for (auto& i : InArray)
		ResultSum += static_cast<SumType>(i);
}

template<typename ElementType, typename AverageType>
void UXyahMathLibrary::GetAverage(const TArray<ElementType>& InArray, AverageType& ResultAverage)
{
	if (InArray.Num() > 0)
	{
		GetSum(InArray, ResultAverage);
		ResultAverage /= static_cast<float>(InArray.Num());
	}
}

template<typename ElementType>
float UXyahMathLibrary::GetMedian(TArray<ElementType> InArray, bool bRequiresSort)
{
	if (InArray.Num() > 0)
	{
		if (bRequiresSort)
			InArray.Sort();

		if (InArray.Num() % 2)
			//If Array is Odd, we deal with only 1 number
			return static_cast<float>(InArray[InArray.Num() / 2]);
		else
		//If Array is Even, we deal with 2
			return 0.5f * static_cast<float>(InArray[(InArray.Num() / 2) - 1] + InArray[InArray.Num() / 2]);
	}
	return 0.f;
}

template<typename ElementType>
void UXyahMathLibrary::GetRange(TArray<ElementType> InArray, bool bIsSorted, ElementType& OutMin, ElementType& OutMax)
{
	if (InArray.Num() > 0)
	{
		if (bIsSorted)
		{
			if (InArray.Last() > InArray[0])
			{
				OutMax = InArray.Last();
				OutMin = InArray[0];
			}
			else
			{
				OutMax = InArray[0];
				OutMin = InArray.Last();
			}
		}
		else
		{
			OutMax = OutMin = InArray[0];
			for (auto& i : InArray)
			{
				if (i > OutMax)
					OutMax = i;
				else if (i < OutMin)
					OutMin = i;
			}
		}
	}
}

