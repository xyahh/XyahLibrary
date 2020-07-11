// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define XYAH(Lib) UXyah##Lib##Library::

DECLARE_LOG_CATEGORY_EXTERN(XyahLog, Log, All);

#define XYAH_LOG(Verbosity, Format, ...) UE_LOG(XyahLog, Verbosity, Format, __VA_ARGS__)

class FXyahLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


/* Helper Macros */

#define XYAH_GET_PROPERTY_AND_ADDRESS(PropertyType, AddrName, PropertyName) Stack.MostRecentProperty = nullptr; \
	Stack.StepCompiledIn<PropertyType>(nullptr); \
	void* AddrName = Stack.MostRecentPropertyAddress; \
	PropertyType* PropertyName = CastField<PropertyType>(Stack.MostRecentProperty);

#define XYAH_CHECK_ARRAY_PROPERTY(ArrayProperty) if (!ArrayProperty) \
	{\
		Stack.bArrayContextFailed = true;\
		return;\
	}

#define XYAH_MARK_PROPERTY_DIRTY(Property) MARK_PROPERTY_DIRTY(Stack.Object, Property)
#define XYAH_RETURN(ResultType, Func) *(ResultType*)RESULT_PARAM = Func
#define XYAH_GET_OBJECT	Stack.Object
#define XYAH_SHOULD_NEVER_HIT_THIS(...) { check(0); return __VA_ARGS__; } 