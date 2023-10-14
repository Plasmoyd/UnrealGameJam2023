// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameViewportClient.h"
#include "GameFramework/PlayerInput.h"

void UMyGameViewportClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
	OnOverrideInputAxis().BindUObject(this, &ThisClass::OverrideInputAxisHandler);
	OnOverrideInputKey().BindUObject(this, &ThisClass::OverrideInputKey);
}

bool UMyGameViewportClient::OverrideInputAxisHandler(FInputKeyEventArgs& EventArgs, float& Delta, float& DeltaTime, int32& NumSamples)
{
	if (EventArgs.IsGamepad()) {
		if (const auto GI = GetGameInstance()) {
			const int32 NumLocalPlayers = GI->GetNumLocalPlayers();
			if (NumLocalPlayers > 1) {
				// TODO: implement custom logic in your GameInstance class to map gamepads.
				const auto TargetPlayer = GI->GetLocalPlayerByIndex(1);
				const auto ControllerId = TargetPlayer->GetControllerId();
				if (ControllerId < 0) { EventArgs.ControllerId++; }
				if (TargetPlayer && TargetPlayer->PlayerController)
				{
					EventArgs.ControllerId = ControllerId;
					TargetPlayer->PlayerController->InputKey(FInputKeyParams(EventArgs.Key, (double)Delta, DeltaTime, NumSamples, EventArgs.IsGamepad(), EventArgs.InputDevice));
					return true;
				}
			}
		}
	}
	return false;
}

bool UMyGameViewportClient::OverrideInputKey(FInputKeyEventArgs& EventArgs)
{
	if (EventArgs.IsGamepad()) {
		if (const auto GI = GetGameInstance()) {
			const int32 NumLocalPlayers = GI->GetNumLocalPlayers();
			if (NumLocalPlayers > 1) {
				const auto TargetPlayer = GI->GetLocalPlayerByIndex(1);
				const auto ControllerId = TargetPlayer->GetControllerId();
				if (ControllerId < 0) { EventArgs.ControllerId++; }
				if (TargetPlayer && TargetPlayer->PlayerController)
				{
					EventArgs.ControllerId = ControllerId;
					const auto bResult = TargetPlayer->PlayerController->InputKey(FInputKeyParams(EventArgs.Key, EventArgs.Event, static_cast<double>(EventArgs.AmountDepressed), EventArgs.IsGamepad(), EventArgs.InputDevice));
					// A gameviewport is always considered to have responded to a mouse buttons to avoid throttling
					if (!bResult && EventArgs.Key.IsMouseButton())
					{
						return true;
					}
					return bResult;
				}
			}
		}
	}
	return false;
}