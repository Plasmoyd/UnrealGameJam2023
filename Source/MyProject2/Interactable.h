// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Interactable.generated.h"

class UStaticMeshComponent;

UENUM()
enum class TaskDifficulty : int32
{
	Easy UMETA(Display = "Easy"),
	Medium UMETA(Display = "Medium"),
	Hard UMETA(Display = "Hard")
};

UCLASS()
class MYPROJECT2_API AInteractable : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact();

	virtual void InteractWithObject_Implementation(FHitResult OutHitResult) override; // ignore error

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float InteractTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 RewardAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TaskDifficulty Difficulty;

public:

	FORCEINLINE float GetInteractTime() const { return InteractTime; }
	FORCEINLINE int32 GetRewardAmount() const { return RewardAmount; }
	FORCEINLINE TaskDifficulty GetTaskDifficulty() const { return Difficulty; }
};
