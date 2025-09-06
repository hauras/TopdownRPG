

#include "TopdownGameplayTags.h"
#include "GameplayTagsManager.h"

FTopdownGameplayTags FTopdownGameplayTags::GameplayTags;

void FTopdownGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("물리 데미지"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("마법 데미지 및 마나 상승"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"), FString("민첩(공격 속도 및 치명타 확률 상승)"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("활력(최대 체력 및 체력 회복량 상승)"));
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("방어력"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("치명타 확률"));

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("경험치"));

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("마우스 왼쪽 클릭"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("숫자 1번 "));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("숫자 2번 "));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("숫자 3번 "));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("숫자 4번 "));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Effects_HitReact"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("FireBolt Ability Tag"));
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("FireBolt Cooldown Tag"));

	GameplayTags.Abilities_Fire_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireStorm"), FString("FireStorm Ability Tag"));
	GameplayTags.Cooldown_Fire_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireStorm"), FString("FireStorm Cooldown Tag"));

	GameplayTags.Abilities_Ice_FrostAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Ice.FrostAttack"), FString("IceAttack Ability Tag"));
	GameplayTags.Cooldown_Ice_FrostAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ice.FrostAttack"), FString("IceAttack Ability Tag"));
	GameplayTags.State_Frozen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Frozen"), FString("State Ability Tag"));
}
