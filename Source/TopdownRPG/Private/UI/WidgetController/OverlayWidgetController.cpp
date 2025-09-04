

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/TopdownAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "State/TopdownPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UTopdownAttributeSet* TopdownAttributeSet = CastChecked<UTopdownAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopdownAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopdownAttributeSet->GetMaxHealth());

}
 
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	ATopdownPlayerState* TopdownPlayerState = CastChecked<ATopdownPlayerState>(PlayerState);
	TopdownPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	TopdownPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
		);
	const UTopdownAttributeSet* TopdownAttributeSet = CastChecked<UTopdownAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopdownAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopdownAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 추후 속성값이 추가될때 이 곳에 추가

	if (UTopdownAbilitySystemComponent* TopdownASC = Cast<UTopdownAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (TopdownASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(TopdownASC);
		}
		else
		{
			TopdownASC->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		TopdownASC->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
	}
	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UTopdownAbilitySystemComponent* TopdownAbilitySystemComponent)
{
	if (!TopdownAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, TopdownAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FTopdownAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TopdownAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = TopdownAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	TopdownAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const 
{
	const ATopdownPlayerState* TopdownPlayerState = CastChecked<ATopdownPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = TopdownPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT(" cant find levelupinfo"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUPRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUPRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

