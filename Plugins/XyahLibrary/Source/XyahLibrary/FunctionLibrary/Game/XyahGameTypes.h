// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XyahGameTypes.generated.h"

UENUM(BlueprintType)
enum EXyahNetRole
{
	XNR_None UMETA(DisplayName = "None", ToolTip = "No role at all."),
	XNR_SimulatedProxy UMETA(DisplayName = "Simulated Proxy", ToolTip = "Locally simulated proxy of this actor."),
	XNR_AutonomousProxy UMETA(DisplayName = "Autonomous Proxy", ToolTip = "Locally autonomous proxy of this actor."),
	XNR_Authority UMETA(DisplayName = "Autonomous Proxy", ToolTip = "Authoritative control over the actor."),
	XNR_MAX,
};

UENUM(BlueprintType)
enum EXyahNetMode
{
	XNM_Standalone UMETA(DisplayName = "Standalone", ToolTip = "A game without networking, with one or more local players. Still considered a server because it has all server functionality."),
	XNM_DedicatedServer UMETA(DisplayName = "Dedicated Server", ToolTip = "Server with no local players."),
	XNM_ListenServer UMETA(DisplayName = "Listen Server", ToolTip = "Server that also has a local player who is hosting the game, available to other players on the network."),
	XNM_Client UMETA(DisplayName = "Client", ToolTip = "Client connected to a remote server"),
	XNM_MAX,
};

FORCEINLINE EXyahNetRole ConvertXyahNetRole(ENetRole Role)
{
	return static_cast<EXyahNetRole>(Role);
}

FORCEINLINE EXyahNetMode ConvertXyahNetMode(ENetMode Mode)
{
	return static_cast<EXyahNetMode>(Mode);
}
