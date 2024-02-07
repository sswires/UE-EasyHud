// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SlateWrapperTypes.h"
#include "EasyHudBaseTypes.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct FEasyHudWidgetDefinition
{
	GENERATED_BODY()

	/* Widget class to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> WidgetClass;

	/* Should we override the visibility when this widget is displayed? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (InlineEditConditionToggle))
	bool bOverrideDisplayVisibility = false;
	
	/*
	 * Widget visibility to use when widget is shown on screen.
	 * HitTestInvisible will make it so it's visible on screen but receives no input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (EditCondition = "bOverrideDisplayVisibility"))
	ESlateVisibility DisplayVisibility = ESlateVisibility::HitTestInvisible;

	/* Should we override the hidden visibility? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (InlineEditConditionToggle))
	bool bOverrideHiddenVisibility = false;

	/* Visibility to use when widget is hidden. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (EditCondition = "bOverrideHiddenVisibility"))
	ESlateVisibility HiddenVisibility = ESlateVisibility::Collapsed;

	/* Is this widget shown by default? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	bool bVisibleByDefault = true;

	/*
	 * Is this widget created for each player's viewport?
	 * If disabled, this makes the widget take the entire screen (even in split screen) and is only created for the
	 * primary player.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	bool bPerPlayerWidget = true;

	/* Gameplay tag for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	FGameplayTag WidgetTag;

	/* Created widget instance */
	UPROPERTY(Transient)
	UUserWidget* WidgetInstance = nullptr;

	/* Set widget instance visibility */
	EASYHUD_API void SetVisibility(bool bVisible);
	
};
