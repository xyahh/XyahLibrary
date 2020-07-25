// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

template<class... Args>
void UXyahUtilityLibrary::Print(const FString& Format, Args&&... Arguments
	, int32 LogID /*= -1 */
	, bool bPrintToScreen /*= true*/, FLinearColor ScreenTextColor /*= FLinearColor(0.0, 0.66, 1.0) */
	, bool bPrintToLog /*= true*/, int32 ConsoleTextColor
	, float Duration /*= 2.f*/
	, bool bNewerOnTop /*= true*/, float TextScale /*= 1.f*/)
{
	PrintMessage(FString::Printf(Format, Arguments), LogID, bPrintToScreen, ScreenTextColor bPrintToLog
		, ConsoleTextColor, Duration, bNewerOnTop, TextScale);
}
