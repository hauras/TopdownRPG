

#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "Controller/TopdownPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "State/TopdownPlayerState.h"
#include "UI/HUD/PlayerHUD.h"


APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState);
	TopdownPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TopdownPlayerState, this);
	Cast<UTopdownAbilitySystemComponent>(TopdownPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = TopdownPlayerState->GetAbilitySystemComponent();
	AttributeSet = TopdownPlayerState->GetAttributeSet();

	if (ATopdownPlayerController* TopdownPlayerController = Cast<ATopdownPlayerController>(GetController()))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(TopdownPlayerController->GetHUD()))
		{
			PlayerHUD->InitOverlay(TopdownPlayerController, TopdownPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
