
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FTopdownGameplayTags
{
public:
	static const FTopdownGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_CriticalHitChance;

	FGameplayTag Attributes_Meta_IncomingXP;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;

	FGameplayTag Effects_HitReact;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag Abilities_Fire_FireStorm;
	FGameplayTag Cooldown_Fire_FireStorm;

	FGameplayTag Abilities_Ice_FrostAttack;
	FGameplayTag Cooldown_Ice_FrostAttack;

	FGameplayTag State; // 
	FGameplayTag State_Frozen; // 빙결 상태
protected:

private:

 static FTopdownGameplayTags GameplayTags;
};