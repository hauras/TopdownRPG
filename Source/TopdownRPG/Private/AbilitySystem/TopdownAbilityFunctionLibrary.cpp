

#include "AbilitySystem/TopdownAbilityFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameMode/TopdownGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UTopdownAbilityFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                                 EMonsterType MonsterType, float Level, UAbilitySystemComponent* ASC)
{
	const ATopdownGameModeBase* TopdownGameMode = Cast<ATopdownGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopdownGameMode == nullptr) return;

	UMonsterClassInfo* MonsterTypeInfo = TopdownGameMode->MonsterClassInfo;
	FMonsterClassDefaultInfo ClassDefaultInfo = MonsterTypeInfo->GetClassDefaultInfo(MonsterType);

	AActor* AvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle PrimaryAttributesContextHandle  = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryMonsterAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());


	FGameplayEffectContextHandle SecondaryAttributesContextHandle  = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(MonsterTypeInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle  = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(MonsterTypeInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

}
