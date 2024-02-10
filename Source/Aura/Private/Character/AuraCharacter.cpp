// Copyright Always NAT20


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // At this rotation rate
	GetCharacterMovement()->bConstrainToPlane = true; // Constrain movement to the XY plane (for top down games)
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // Snap to the XY plane at the start (for top down games)

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
