// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/EasyHudBaseTypes.h"
#include "GameFramework/HUD.h"
#include "EasyHudBase.generated.h"

/**
 *  Provides a simple way of spawning UMG widgets on the screen for use in a HUD.
 *  Asynchronous loading of the widget is handled for you.
 */
UCLASS(Abstract)
class EASYHUD_API AEasyHudBase : public AHUD
{
	GENERATED_BODY()

public:
	
	// AActor interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	// Override to hide widgets if the ShowHUD command is run to mimick the behaviour of the command
	virtual void ShowHUD() override;

	// Function that allows HUD elements to be shown/hidden based on matching a gameplay tags
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetElementsVisible(const FGameplayTagContainer& InTags, bool bVisible);

protected:
	
	// List of widgets we want as part of this HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TArray<FEasyHudWidgetDefinition> Widgets;

public:

	// Non-mutable accessor
	const TArray<FEasyHudWidgetDefinition>& GetWidgetDefinitions() const
	{
		return Widgets;
	}
	
private:

	// Called from BeginPlay, this starts loading all of our widgets
	void LoadWidgets();

	// This iterates through the widget definitions and produces an array of assets to load that can be
	// passed onto the streamable manager
	void CollectWidgetsToLoad(TArray<FSoftObjectPath>& OutAssetsToLoad);

	// Callback for when the widgets have been loaded, we will spawn instances here.
	void OnWidgetsAsyncLoaded();

	// Creates widgets for the provided definitions. Assumes loaded.
	void SpawnLoadedWidgets();

	// Cleans up the widgets spawned by this HUD
	void CleanupWidgets();
	
};
