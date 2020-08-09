// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

template<class... Args>
void UXyahUtilityLibrary::Print(const FXyahPrintSettings& Settings, const FString& Format, Args&&... Arguments)
{
	PrintMessage(FString::Printf(Format, Arguments), Settings);
}
