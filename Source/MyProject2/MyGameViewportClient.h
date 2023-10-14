// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "MyGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UMyGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:

	void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice);

	bool OverrideInputAxisHandler(FInputKeyEventArgs& EventArgs, float& Delta, float& DeltaTime, int32& NumSamples);

	bool OverrideInputKey(FInputKeyEventArgs& EventArgs);

};
