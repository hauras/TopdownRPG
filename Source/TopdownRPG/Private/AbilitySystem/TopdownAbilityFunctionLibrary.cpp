

#include "AbilitySystem/TopdownAbilityFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameMode/TopdownGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// 추후 수정 필요

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

void UTopdownAbilityFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC)
{
	const ATopdownGameModeBase* TopdownGameMode = Cast<ATopdownGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	UMonsterClassInfo* MonsterClassInfo = TopdownGameMode->MonsterClassInfo;
	for (TSubclassOf<UGameplayAbility> AbilityClass : MonsterClassInfo->Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UMonsterClassInfo* UTopdownAbilityFunctionLibrary::GetMonsterClassInfo(const UObject* WorldContextObject)
{
	ATopdownGameModeBase* TopdownGameMode = Cast<ATopdownGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopdownGameMode == nullptr) return nullptr;
	return TopdownGameMode->MonsterClassInfo;
}

int32 UTopdownAbilityFunctionLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                                  EMonsterType MonsterType, int32 MonsterLevel)
{
	UMonsterClassInfo* MonsterClassInfo = GetMonsterClassInfo(WorldContextObject);
	if (MonsterClassInfo == nullptr) return 0;

	const FMonsterClassDefaultInfo Info = MonsterClassInfo->GetClassDefaultInfo(MonsterType);
	const float XPReward = Info.XPReward.GetValueAtLevel(MonsterLevel);

	return static_cast<int32>(XPReward);
}
