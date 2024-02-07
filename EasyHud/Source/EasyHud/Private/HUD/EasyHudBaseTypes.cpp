// Copyright Stephen Swires. All Rights Reserved.

#include "HUD/EasyHudBaseTypes.h"
#include "Blueprint/UserWidget.h"

void FEasyHudWidgetDefinition::SetVisibility(bool bVisible)
{
	if (IsValid(WidgetInstance))
	{
		const ESlateVisibility DesiredVisibility = bVisible ?
			(bOverrideDisplayVisibility ? DisplayVisibility : ESlateVisibility::HitTestInvisible) :
			(bOverrideHiddenVisibility ? HiddenVisibility : ESlateVisibility::Collapsed);

		WidgetInstance->SetVisibility(DesiredVisibility);		
	}
}
