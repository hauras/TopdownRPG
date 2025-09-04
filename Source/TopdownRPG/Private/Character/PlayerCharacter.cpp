

#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
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

void APlayerCharacter::AddToEXP_Implementation(int32 InXP)
{
	ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState);
	TopdownPlayerState->AddToXP(InXP);
}

void APlayerCharacter::LevelUp_Implementation()
{
	
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState);
	return TopdownPlayerState->GetXP();
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState);
	return TopdownPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState)
	return TopdownPlayerState->AddToLevel(InPlayerLevel);
}

int32 APlayerCharacter::GetPlayerLevel_Implementation()
{
	const ATopdownPlayerState* TopdownPlayerState =  GetPlayerState<ATopdownPlayerState>();
	check(TopdownPlayerState);
	return TopdownPlayerState->GetPlayerLevel();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	// ASC를 PlayerState가 소유
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
	InitializeDefaultAttributes();
	AddCharacterAbilities();
}
