
#include "State/TopdownPlayerState.h"

#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/TopdownAttributeSet.h"
#include "Net/UnrealNetwork.h"

ATopdownPlayerState::ATopdownPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTopdownAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UTopdownAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void ATopdownPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATopdownPlayerState, Level);
}

UAbilitySystemComponent* ATopdownPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopdownPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
