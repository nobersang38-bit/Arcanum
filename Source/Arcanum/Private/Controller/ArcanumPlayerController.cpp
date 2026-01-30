// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controller/ArcanumPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/TouchInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AArcanumPlayerController::AArcanumPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	static ConstructorHelpers::FObjectFinder<UTouchInterface>
		touchInterface(TEXT("/Game/TopDown/Input/TI_VirtualJoystick.TI_VirtualJoystick"));

	if (touchInterface.Succeeded())
	{
		DefaultTouchInterface = touchInterface.Object;
	}

	bShowMouseCursor = true;
}

void AArcanumPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultTouchInterface)
	{
		ActivateTouchInterface(DefaultTouchInterface);
	}

	/*
	if (ULocalPlayer* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* enhancedinputsubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultIMC)
			{
				enhancedinputsubsystem->AddMappingContext(DefaultIMC, 0);
			}
		}
	}
	*/
}

void AArcanumPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subSystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		enhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AArcanumPlayerController::OnInputStarted);
		enhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AArcanumPlayerController::OnSetDestinationTriggered);
		enhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AArcanumPlayerController::OnSetDestinationReleased);
		enhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AArcanumPlayerController::OnSetDestinationReleased);

		enhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AArcanumPlayerController::OnInputStarted);
		enhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AArcanumPlayerController::OnTouchTriggered);
		enhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AArcanumPlayerController::OnTouchReleased);
		enhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AArcanumPlayerController::OnTouchReleased);

		enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AArcanumPlayerController::OnMove);
		enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &AArcanumPlayerController::OnMove);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AArcanumPlayerController::OnMove(const FInputActionValue& InValue)
{
	const FVector2D move2D = InValue.Get<FVector2D>();

	APawn* pawn = GetPawn();
	if (pawn)
	{
		const FVector forward = FVector::ForwardVector;
		const FVector right = FVector::RightVector;

		pawn->AddMovementInput(forward, move2D.Y);
		pawn->AddMovementInput(right, move2D.X);
	}
}

void AArcanumPlayerController::OnInputStarted()
{
	StopMovement();
}

void AArcanumPlayerController::OnSetDestinationTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	APawn* controlledPawn = GetPawn();
	if (controlledPawn != nullptr)
	{
		FVector worldDirection = (CachedDestination - controlledPawn->GetActorLocation()).GetSafeNormal();
		controlledPawn->AddMovementInput(worldDirection, 1.0, false);
	}
}

void AArcanumPlayerController::OnSetDestinationReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void AArcanumPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AArcanumPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
