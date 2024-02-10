// Copyright Always NAT20


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //don't lock mouse to viewport
	InputModeData.SetHideCursorDuringCapture(false); //don't hide cursor
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Checking if the input component is of type UEnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	//Binding Input Actions to functions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); //Getting the input axis vector
	const FRotator Rotation = GetControlRotation(); //Getting the control rotation
	const FRotator YawRotation(0, Rotation.Yaw, 0); //Creating a rotation with only the yaw value

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Getting the forward vector
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Getting the right vector

	if (APawn* ControlledPawn = GetPawn<APawn>()) //Checking if the controlled pawn is valid
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y); //Adding movement input in the forward vector
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X); //Adding movement input in the right vector
	}

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); //Getting the hit result under the cursor

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor; //Setting the last actor to the current actor
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor()); //Casting the hit actor to the IEnemyInterface to check if it is an enemy

	/*
	 * Line Trace from cursor, there are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *	- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *	- Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *	- Unhighlight LastActor
	 * D. LastActor is valid && ThisActor is valid but LastActor != ThisActor
	 *	- Unhighlight LastActor and Highlight ThisActor
	 * E. LastActor is valid && ThisActor is valid and LastActor == ThisActor
	 *	- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A
			// Do nothing
		}
	}
	else // LastActor != nullptr
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both LastActor and ThisActor are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E
				// Do nothing
			}
		}
	}
}
