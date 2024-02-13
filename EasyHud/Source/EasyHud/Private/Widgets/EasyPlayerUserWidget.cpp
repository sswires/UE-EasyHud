// Copyright Stephen Swires. All Rights Reserved.

#include "Widgets/EasyPlayerUserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

void UEasyPlayerUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RegisterPlayerDelegates();
}

void UEasyPlayerUserWidget::RegisterPlayerDelegates()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (IsValid(PlayerController))
	{
		if (bAutoRegisterPawnChanged)
		{
			PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UEasyPlayerUserWidget::OnPawnChangedPrivate);

			if (APawn* CurrentPawn = PlayerController->GetPawn())
			{
				OnPawnChangedPrivate(nullptr, CurrentPawn);
			}
		}

		if (bAutoRegisterPlayerStateChanged)
		{
			if (APlayerState* CurrentPlayerState = PlayerController->PlayerState)
			{
				OnPlayerStateChanged(CurrentPlayerState);
			}
			else
			{
				// Player state may not be available yet, so we'll listen for it
				// But the bad news, Unreal has no delegate for this, so we'll have to poll until we get it
				GetWorld()->GetTimerManager().SetTimer(
					TimerHandle_CheckPlayerState,
					this,
					&UEasyPlayerUserWidget::CheckForPlayerState,
					0.5f,
					true);
			}
		}
	}
}

void UEasyPlayerUserWidget::OnPawnChangedPrivate(APawn* InOldPawn, APawn* InNewPawn)
{
	OnPawnChanged(InOldPawn, InNewPawn);
}

void UEasyPlayerUserWidget::CheckForPlayerState()
{	
	APlayerController* PlayerController = GetOwningPlayer();

	if (IsValid(PlayerController) && IsValid(PlayerController->PlayerState))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CheckPlayerState);
		OnPlayerStateChanged(PlayerController->PlayerState);
	}
}
