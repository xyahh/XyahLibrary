// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#include "K2Node_SpawnActorDeferred.h"

#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"

#define LOCTEXT_NAMESPACE "K2Node_SpawnActorDeferred"


namespace XyahNode
{
	static const FName PreSpawnExec(TEXT("Pre Spawn"));
	static const FName PostSpawnExec(TEXT("Post Spawn"));
}


void UK2Node_SpawnActorDeferred::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	/*Create our pins*/

	// Execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, XyahNode::PreSpawnExec);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, XyahNode::PostSpawnExec);

}

FText UK2Node_SpawnActorDeferred::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("SpawnActorDeferred_Title", "SpawnActorDeferred");
}

FText UK2Node_SpawnActorDeferred::GetTooltipText() const
{
	return LOCTEXT("SpawnActorDeferred_Tooltip", "Spawns an Actor in a Deferred Way (i.e. calling a Given Function/Event) before Finishing Spawn and calling Begin Play");
}

FSlateIcon UK2Node_SpawnActorDeferred::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.SpawnActor_16x");
	return Icon;
}

FText UK2Node_SpawnActorDeferred::GetMenuCategory() const
{
	return LOCTEXT("SpawnActorDeferred_MenuCategory", "Xyah Library|Actor");
}

void UK2Node_SpawnActorDeferred::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	//After we are done we break all links to this node (not the internally created one)
	BreakAllNodeLinks();
}

//This method adds our node to the context menu
void UK2Node_SpawnActorDeferred::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	UClass* Action = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(Action)) {
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);

		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}
#undef LOCTEXT_NAMESPACE