// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class XyahLibrary : ModuleRules
{
	public XyahLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "FunctionLibrary"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Core"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Utility"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Game"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Math"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Array"),
                Path.Combine(ModuleDirectory, "FunctionLibrary/Map"),
                Path.Combine(ModuleDirectory, "Graph"),
                Path.Combine(ModuleDirectory, "Components"),
                Path.Combine(ModuleDirectory, "Components/Core"),
                Path.Combine(ModuleDirectory, "Settings"),
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
                "KismetCompiler",
				"BlueprintGraph",
				"Kismet",
				"KismetWidgets",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
