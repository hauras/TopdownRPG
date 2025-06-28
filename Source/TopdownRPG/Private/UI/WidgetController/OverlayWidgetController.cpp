

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

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopdownAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanced);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopdownAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanced);

	
	Cast<UTopdownAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
			}
		});
}

void UOverlayWidgetController::HealthChanced(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanced(const FOnAttributeChangeData& Data) const 
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
