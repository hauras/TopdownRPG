
#include "Character/EnemyCharacter.h"

#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/TopdownAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "TopdownRPG/TopdownRPG.h"
#include "UI/Widget/TopdownUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTopdownAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UTopdownAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UTopdownUserWidget* TopdownUserWidget = Cast<UTopdownUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		TopdownUserWidget->SetWidgetController(this);
	}
	if (const UTopdownAttributeSet* TopdownAS = Cast<UTopdownAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopdownAS->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopdownAS->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(TopdownAS->GetHealth());
		OnMaxHealthChanged.Broadcast(TopdownAS->GetMaxHealth());
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTopdownAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}
