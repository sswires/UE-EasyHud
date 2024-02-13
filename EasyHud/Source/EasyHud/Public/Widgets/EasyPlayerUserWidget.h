// Copyright Stephen Swires. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EasyPlayerUserWidget.generated.h"

class APawn;
class APlayerState;

/**
 * Base widget that fires events when the pawn changes and when the player state is available
 */
UCLASS(Abstract)
class EASYHUD_API UEasyPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// Event called when the owning player's pawn changes.
	// The old pawn may be null if the player had no pawn, which will be the case the first time this event is called.
	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void OnPawnChanged(APawn* InOldPawn, APawn* InNewPawn);
	virtual void OnPawnChanged_Implementation(APawn* InOldPawn, APawn* InNewPawn) {}

	// Event called when player state is available
	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void OnPlayerStateChanged(APlayerState* InPlayerState);
	virtual void OnPlayerStateChanged_Implementation(APlayerState* InPlayerState) {}

protected:

	// Auto register delegate to the owning player's pawn changed event
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	bool bAutoRegisterPawnChanged = false;

	// Auto register delegate to the owning player's player state changed event
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	bool bAutoRegisterPlayerStateChanged = false;

private:
	
	void RegisterPlayerDelegates();

	// Dynamic delegate callback when the owner player's pawn changes
	UFUNCTION()
	void OnPawnChangedPrivate(APawn* InOldPawn, APawn* InNewPawn);

	void CheckForPlayerState();
	
	UPROPERTY()
	FTimerHandle TimerHandle_CheckPlayerState;
	
};
