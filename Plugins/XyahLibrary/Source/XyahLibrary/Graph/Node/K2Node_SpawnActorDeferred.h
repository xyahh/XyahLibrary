// Copyright (C), Juan Marcelo Portillo. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_SpawnActorDeferred.generated.h"

UCLASS()
class UK2Node_SpawnActorDeferred : public UK2Node
{
	GENERATED_BODY()	
//UEdGraphNode
public:

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
//UK2Node
public:

	virtual FText GetMenuCategory() const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

};
