

#include "AbilitySystem/Abilities/TeleportSpell.h"

#include "NavigationSystem.h"
#include "TopdownGameplayTags.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayCueFunctionLibrary.h"
UTeleportSpell::UTeleportSpell()
{
	
}

void UTeleportSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!Player)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// --- 로직 변경 시작 ---

	const FVector PlayerLocation = Player->GetActorLocation(); // 1. 현재 플레이어 위치
	const FVector ForwardDirection = Player->GetActorForwardVector(); // 2. 플레이어가 바라보는 정면 방향
	
	// 3. (현재 위치) + (바라보는 방향 * 최대 거리) = 최종 목표 지점
	const FVector TargetLocation = PlayerLocation + ForwardDirection * MaxTeleportDistance; 

	// --- 로직 변경 끝 ---


	// 아래의 네비게이션 검증 로직은 그대로 사용합니다.
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(NavMeshProjectionRadius)))
	{
		// 출발 지점 이펙트
		if (TeleportStartCueTag.IsValid())
		{
			UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(Player, TeleportStartCueTag, FGameplayCueParameters());
		}

		// 계산되고 검증된 위치로 순간이동
		Player->TeleportTo(ProjectedLocation.Location, Player->GetActorRotation());

		// 어빌리티 성공적으로 종료
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else
	{
		// 정면에 벽이 있거나, 낭떠러지라서 이동할 수 없는 경우 어빌리티 취소
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}
