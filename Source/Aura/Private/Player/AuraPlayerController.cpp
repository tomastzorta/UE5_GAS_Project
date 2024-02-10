// Copyright Always NAT20


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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
