// Copyright Stephen Swires. All Rights Reserved.

using UnrealBuildTool;

public class EasyHud : ModuleRules
{
	public EasyHud(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"Slate",
			"SlateCore",
			"GameplayTags",
			"UMG",
		});
        
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
		});
	}
}
