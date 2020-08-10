// Copyright (C), Juan Marcelo Portillo. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node.h"
#include "K2Node_SpawnActorFromClass.h"
#include "K2Node_SpawnActorDeferred.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

UCLASS()
class XYAHLIBRARY_API UK2Node_SpawnActorDeferred : public UK2Node_SpawnActorFromClass
{
	GENERATED_UCLASS_BODY()

public:

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsSpawnVarPin(UEdGraphPin* Pin) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

protected:

	UEdGraphPin* GetPreSpawnPin() const;
	/** Get the spawn transform input pin */
	UEdGraphPin* GetSpawnTransformPin_Child() const;
	/** Get the collision handling method input pin */
	UEdGraphPin* GetCollisionHandlingOverridePin_Child() const;
	/** Get the actor owner pin */
	UEdGraphPin* GetOwnerPin_Child() const;

	void MaybeUpdateCollisionPin_Child(TArray<UEdGraphPin*>& OldPins);
};
