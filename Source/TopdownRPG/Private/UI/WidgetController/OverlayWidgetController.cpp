

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/TopdownAbilitySystemComponent.h"
#include "AbilitySystem/TopdownAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UTopdownAttributeSet* TopdownAttributeSet = CastChecked<UTopdownAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopdownAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopdownAttributeSet->GetMaxHealth());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
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
	
	
	Cast<UTopdownAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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

