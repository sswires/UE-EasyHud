// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "EasyHudBase.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct FEasyHudWidgetDefinition
{
	GENERATED_BODY()

	/* Widget class to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> WidgetClass;

	/*
	 * Widget visibility to default to.
	 * HitTestInvisible will make it so it's visible on screen but receives no input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	ESlateVisibility DefaultVisibility = ESlateVisibility::HitTestInvisible;

	/*
	 * Is this widget created as a full screen widget?
	 * If enabled, this makes the widget take the entire screen (even in split screen) and is only created for the
	 * primary player.
	 */
	bool bIsFullScreen = false;

	/* Created widget instance */
	UPROPERTY(Transient)
	UUserWidget* WidgetInstance;
	
};

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

protected:
	
	// List of widgets we want as part of this HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TArray<FEasyHudWidgetDefinition> Widgets;

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
