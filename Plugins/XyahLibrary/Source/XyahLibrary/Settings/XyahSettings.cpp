// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.


#include "XyahSettings.h"
UXyahSettings::UXyahSettings()
{	
}

bool UXyahSettings::OnSettingsModified_Implementation()
{
	return true;
}

void UXyahSettings::SaveSettings()
{
	SaveConfig(CPF_Config);
}
