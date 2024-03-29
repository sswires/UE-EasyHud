﻿// Copyright Stephen Swires. All Rights Reserved.

#include "HUD/EasyHudBase.h"
#include "EasyHudLogging.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void AEasyHudBase::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay is the right place to load and spawn the widgets. We should be able to guarantee the game state is ready.
	LoadWidgets();
}

void AEasyHudBase::BeginDestroy()
{
	CleanupWidgets();
	Super::BeginDestroy();
}

void AEasyHudBase::ShowHUD()
{
	Super::ShowHUD();

	// update widgets to match the state of running the showhud cheat
	for (FEasyHudWidgetDefinition& WidgetDefinition : Widgets)
	{
		WidgetDefinition.SetVisibility(bShowHUD);
	}
}

void AEasyHudBase::SetElementsVisible(const FGameplayTagContainer& InTags, bool bVisible)
{
	for (FEasyHudWidgetDefinition& WidgetDefinition : Widgets)
	{
		if (!WidgetDefinition.WidgetTag.IsValid())
		{
			// invalid tag, ignore
			continue;
		}

		if (!WidgetDefinition.WidgetTag.MatchesAny(InTags))
		{
			// no matching tags
			continue;
		}
		
		WidgetDefinition.SetVisibility(bShowHUD);
	}
}

void AEasyHudBase::LoadWidgets()
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// get list of widgets to load
	TArray<FSoftObjectPath> AssetsToLoad;
	CollectWidgetsToLoad(AssetsToLoad);

	// request their load, after the load we'll spawn all the required widget instances
	StreamableManager.RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateUObject(this, &AEasyHudBase::OnWidgetsAsyncLoaded));
}

void AEasyHudBase::CollectWidgetsToLoad(TArray<FSoftObjectPath>& OutAssetsToLoad)
{
	for (const FEasyHudWidgetDefinition& WidgetDefinition : Widgets)
	{
		if (WidgetDefinition.WidgetClass.IsNull())
		{
			// ignoring empty entry
			continue;
		}
		
		OutAssetsToLoad.AddUnique(WidgetDefinition.WidgetClass.ToSoftObjectPath());
	}
}

void AEasyHudBase::OnWidgetsAsyncLoaded()
{
	SpawnLoadedWidgets();
}

void AEasyHudBase::SpawnLoadedWidgets()
{
	const bool bIsPrimaryPlayer = GetOwningPlayerController()->IsPrimaryPlayer();
	
	for (FEasyHudWidgetDefinition& WidgetDefinition : Widgets)
	{
		if (!WidgetDefinition.bPerPlayerWidget && !bIsPrimaryPlayer)
		{
			// we only spawn full screen widgets for the primary player
			continue;
		}
		
		// Get() should resolve here if it was successfully loaded
		const TSoftClassPtr<UUserWidget>& WidgetClass = WidgetDefinition.WidgetClass;
		TSubclassOf<UUserWidget> LoadedWidgetClass = WidgetClass.Get();

		if (!LoadedWidgetClass)
		{
			UE_LOGFMT(LogEasyHud, Warning, "'{WidgetClass}' failed to load. Skipping...", WidgetClass.ToString());
			continue;
		}

		UUserWidget* CreatedWidget = CreateWidget(GetOwningPlayerController(), LoadedWidgetClass);

		if (WidgetDefinition.bPerPlayerWidget)
		{
			CreatedWidget->AddToPlayerScreen();
			UE_LOGFMT(LogEasyHud, Verbose, "Spawned '{WidgetClass}' on player screen.", WidgetClass.ToString());
		}
		else
		{
			CreatedWidget->AddToViewport();
			UE_LOGFMT(LogEasyHud, Verbose, "Spawned '{WidgetClass}' on full screen viewport.", WidgetClass.ToString());		
		}
		
		// keep track of the spawned widget
		WidgetDefinition.WidgetInstance = CreatedWidget;

		// set to default visibility
		WidgetDefinition.SetVisibility(bShowHUD && WidgetDefinition.bVisibleByDefault);
	}
}

void AEasyHudBase::CleanupWidgets()
{
	for (FEasyHudWidgetDefinition& WidgetDefinition : Widgets)
	{
		UUserWidget*& WidgetInstance = WidgetDefinition.WidgetInstance;
		
		if (IsValid(WidgetInstance))
		{
			WidgetInstance->RemoveFromParent();
			WidgetInstance = nullptr;
		}
	}
}
