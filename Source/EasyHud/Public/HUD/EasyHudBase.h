// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EasyHudBase.generated.h"

class UUserWidget;

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

protected:
	
	// List of widgets we want to spawn on the player's viewport
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TArray<TSoftClassPtr<UUserWidget>> PlayerViewportWidgetClasses;

	// List of widgets to spawn for the full screen for the primary player.
	// This is ignored for the 2nd player and beyond in split screen games.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TArray<TSoftClassPtr<UUserWidget>> FullScreenWidgetClasses;

	// List of widgets spawned that are attached to the owning player's viewport
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TArray<TObjectPtr<UUserWidget>> PlayerViewportWidgets;

	// List of widgets spawned to the screen. This will be only populated for the first local player.
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TArray<TObjectPtr<UUserWidget>> FullScreenWidgets;

private:

	void CollectWidgetsToLoad(TArray<FSoftObjectPath>& OutAssetsToLoad);

	// Callback for when the widgets have been loaded, we will spawn instances here.
	void OnWidgetsAsyncLoaded();

	// Shared logic for spawning widgets from a list of classes
	void SpawnWidgetsFromClasses(const TArray<TSoftClassPtr<UUserWidget>>& WidgetClasses,
		TArray<TObjectPtr<UUserWidget>>& OutSpawnedWidgets,
		bool bInFullScreen
		);

	// Cleans up the widgets spawned by this HUD
	void CleanupWidgets();
	
};
