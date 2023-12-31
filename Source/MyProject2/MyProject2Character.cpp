// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <Kismet/GameplayStatics.h>
#include "Math/Vector2D.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "InteractInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProject2Character

AMyProject2Character::AMyProject2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 150.0f;

	////// Create a camera boom (pulls in towards the player if there is a collision)
	////CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	////CameraBoom->SetupAttachment(RootComponent);
	////CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	////CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	////CameraBoom->bInheritYaw = false;
	////CameraBoom->bInheritPitch = false;
	////CameraBoom->bInheritRoll = false;

	//// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(RootComponent); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProject2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyProject2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//HandleInteractables();
}

void AMyProject2Character::HandleInteractables()
{
	/*FVector MovementDirection = FVector(MovementVector.X, 0.f, MovementVector.Y);

	if (!MovementDirection.IsZero())
	{
		LastInteractableDirection = FVector(MovementVector.X, 0.f, MovementVector.Y);
	}*/

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // ignore character that owns this line trace
	FHitResult OutHitResult;
	FVector Start = GetActorLocation();
	Start.X += 50.f;
	FVector End = Start + GetActorForwardVector() * InteractTraceDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2, 0, 2);

	bool didHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_GameTraceChannel1, CollisionParams);

	if (didHit && OutHitResult.bBlockingHit && OutHitResult.GetActor()) {
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Actor Being Hit is: %s"), *OutHit.GetActor()->GetName()));
		}

		IInteractInterface* HitInteractInterface = Cast<IInteractInterface>(OutHitResult.GetActor());

		/*OutHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());*/

		if (HitInteractInterface)
		{
			//UE_LOG(LogTemplateCharacter, Warning, TEXT("JEBEM TI MATER"));

			HitInteractInterface->InteractWithObject_Implementation(OutHitResult);
			//SelectedInteractInterface = HitInteractInterface;
		}
		else
		{
			//SelectedInteractInterface = nullptr;
		}
	}
	else 
	{
		//SelectedInteractInterface = nullptr;

	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject2Character::Move);
		/*EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyProject2Character::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Ongoing, this, &AMyProject2Character::InteractTime);*/
		/*EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyProject2Character::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyProject2Character::Interact);*/

		// NOT NEEDED FOR THIS PROJECT
		// Jumping
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);*/
		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject2Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProject2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// Interact with interactable Objects
void AMyProject2Character::Interact(const FInputActionValue& Value) 
{
	// we must get time that elapsed from press and pass it in Handle Interaction

	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("INTERACTED")));
	}

	//HandleInteractables();

	/*if (SelectedInteractInterface != nullptr){}*/
}

void AMyProject2Character::InteractTime(const FInputActionInstance& Instance)
{
	// we must get time that elapsed from press and pass it in Handle Interaction
	float timeElapsed = Instance.GetElapsedTime();

	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%f"), timeElapsed));
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Cyan, "TIME " + FString::SanitizeFloat(Instance.GetElapsedTime()));
	}
}

// ---------------- FUNCTIONS WE DON'T CURRENTLY NEED --------------

// LOOK NOT NEEDED FOR THIS PROJECT
void AMyProject2Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}