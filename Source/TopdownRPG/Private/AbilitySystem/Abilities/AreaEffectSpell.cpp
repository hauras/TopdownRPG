

#include "AbilitySystem/Abilities/AreaEffectSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TopdownGameplayTags.h"
#include "Actor/TopdownEffectActor.h"

void UAreaEffectSpell::SpawnAreaEffect(const FVector& TargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(TargetLocation);

	// 그냥 생성하고 끝냅니다. 데미지 정보 전달 로직이 필요 없습니다.
	GetWorld()->SpawnActor<ATopdownEffectActor>(
		AreaEffectClass,
		SpawnTransform);
}