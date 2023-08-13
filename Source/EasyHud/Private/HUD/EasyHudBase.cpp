// Copyright Stephen Swires. All Rights Reserved.

#include "HUD/EasyHudBase.h"
#include "EasyHudLogging.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void AEasyHudBase::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay is the right place to load and spawn the widgets. We should be able to guarantee the game state is ready.
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// get list of widgets to load
	TArray<FSoftObjectPath> AssetsToLoad;
	CollectWidgetsToLoad(AssetsToLoad);

	// request their load, after the load we'll spawn all the required widget instances
	StreamableManager.RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &AEasyHudBase::OnWidgetsAsyncLoaded));
}

void AEasyHudBase::BeginDestroy()
{
	CleanupWidgets();
	Super::BeginDestroy();
}

void AEasyHudBase::CollectWidgetsToLoad(TArray<FSoftObjectPath>& OutAssetsToLoad)
{
	for (const TSoftClassPtr<UUserWidget>& WidgetClass : PlayerViewportWidgetClasses)
	{
		OutAssetsToLoad.AddUnique(WidgetClass.ToSoftObjectPath());
	}

	for (const TSoftClassPtr<UUserWidget>& WidgetClass : FullScreenWidgetClasses)
	{
		OutAssetsToLoad.AddUnique(WidgetClass.ToSoftObjectPath());
	}
}

void AEasyHudBase::OnWidgetsAsyncLoaded()
{
	SpawnWidgetsFromClasses(PlayerViewportWidgetClasses, PlayerViewportWidgets, false);

	if (GetOwningPlayerController()->IsPrimaryPlayer())
	{
		SpawnWidgetsFromClasses(FullScreenWidgetClasses, FullScreenWidgets, true);
	}
}

void AEasyHudBase::SpawnWidgetsFromClasses(const TArray<TSoftClassPtr<UUserWidget>>& WidgetClasses,
	TArray<TObjectPtr<UUserWidget>>& OutSpawnedWidgets, bool bInFullScreen)
{
	for (const TSoftClassPtr<UUserWidget>& WidgetClass : WidgetClasses)
	{
		TSubclassOf<UUserWidget> LoadedWidgetClass = WidgetClass.Get(); // Get() should resolve here if it was successfully loaded

		if (!LoadedWidgetClass)
		{
			UE_LOGFMT(LogEasyHud, Warning, "'{WidgetClass}' failed to load. Skipping...", WidgetClass.ToString());
			continue;
		}

		UUserWidget* CreatedWidget = CreateWidget(GetOwningPlayerController(), LoadedWidgetClass);

		if (bInFullScreen)
		{
			CreatedWidget->AddToViewport();
			UE_LOGFMT(LogEasyHud, Verbose, "Spawned '{WidgetClass}' on full screen viewport.", WidgetClass.ToString());
		}
		else
		{
			CreatedWidget->AddToPlayerScreen();
			UE_LOGFMT(LogEasyHud, Verbose, "Spawned '{WidgetClass}' on player screen.", WidgetClass.ToString());
		}

		OutSpawnedWidgets.Add(CreatedWidget);
	}
}

void AEasyHudBase::CleanupWidgets()
{
	for (UUserWidget* Widget : PlayerViewportWidgets)
	{
		if (IsValid(Widget))
		{
			Widget->RemoveFromParent();
		}
	}
	
	for (UUserWidget* Widget : FullScreenWidgets)
	{
		if (IsValid(Widget))
		{
			Widget->RemoveFromParent();
		}
	}
}
