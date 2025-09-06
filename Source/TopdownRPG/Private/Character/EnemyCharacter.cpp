
#include "Character/EnemyCharacter.h"

#include "TopdownGameplayTags.h"
#include "AbilitySystem/TopdownAbilityFunctionLibrary.h"
#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/TopdownAttributeSet.h"
#include "AI/TopdownAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/TopdownAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TopdownRPG/TopdownRPG.h"
#include "UI/Widget/TopdownUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTopdownAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UTopdownAttributeSet>("AttributeSet");

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	StatusImage = CreateDefaultSubobject<UWidgetComponent>("StatusImage");
	StatusImage->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	TopdownAIController = Cast<ATopdownAIController>(NewController);
	
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

int32 AEnemyCharacter::GetPlayerLevel_Implementation()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemyCharacter::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::DebuffTagChanged(CallbackTag, NewCount);
	if (TopdownAIController && TopdownAIController->GetBlackboardComponent())
	{
		// 블랙보드에 "IsFrozen" 키가 있어야 합니다!
		TopdownAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsFrozen"), bIsFrozen);
	}

	if (StatusImage)
	{
		StatusImage->SetVisibility(bIsFrozen);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UTopdownAbilityFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	
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
	Super::InitAbilityActorInfo(); // << 이 줄이 없으면 절대 작동하지 않습니다!

	InitializeDefaultAttributes();
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UTopdownAbilityFunctionLibrary::InitializeDefaultAttributes(this, MonsterClass, Level, AbilitySystemComponent);
}

