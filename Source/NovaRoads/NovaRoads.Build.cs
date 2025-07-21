// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NovaRoads : ModuleRules
{
    public NovaRoads(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
				"NovaRoads/Public"
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
                "NovaRoads/Private"
            });


        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "ProceduralMeshComponent", // Needed for your RoadGenerator's mesh generation
	    "Landscape",                // Core Landscape module
     	    "Landmass",                 // <--- CRITICAL: Provides ALandscapeBlueprintBrush
	    "Foliage",
            "RenderCore",
            "RHI",
            "LandscapeEditor",           // Needed for editor-time landscape functions and brushes
            "LandscapeEditorUtilities"
        });


        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "Landscape",                // Core Landscape module
	    "Landmass",                 // <--- CRITICAL: Provides ALandscapeBlueprintBrush
	    "Foliage",
            "RenderCore",
            "RHI",
            "LandscapeEditor",           // Needed for editor-time landscape functions and brushes
            "LandscapeEditorUtilities"
        });


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}
