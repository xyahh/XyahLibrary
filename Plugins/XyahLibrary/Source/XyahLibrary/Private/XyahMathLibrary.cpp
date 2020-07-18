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
