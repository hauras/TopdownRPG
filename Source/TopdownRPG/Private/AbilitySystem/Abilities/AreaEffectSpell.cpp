#include "AbilitySystem/Abilities/AreaEffectSpell.h"
#include "Actor/TopdownEffectActor.h" // ATopdownEffectActor 때문에 필요

void UAreaEffectSpell::SpawnAreaEffect(const FVector& TargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(TargetLocation);

	// -------------------- [수정된 부분 시작] --------------------

	// 1. 스폰 파라미터를 담을 빈 구조체를 만듭니다.
	FActorSpawnParameters SpawnParams;
    
	// 2. 이 액터의 '소유자(Owner)'를 설정합니다.
	//    GetAvatarActorFromActorInfo()는 이 어빌리티를 사용하는 캐릭터를 반환합니다.
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
    
	// 3. 이 액션의 '원인 제공자(Instigator)'를 설정합니다.
	//    Instigator는 보통 APawn 타입이어야 하므로, 캐릭터로 캐스팅해줍니다.
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());

	// 4. 수정된 SpawnActor 함수를 호출합니다. 세 번째 인자로 SpawnParams를 전달하는 것이 핵심입니다.
	GetWorld()->SpawnActor<ATopdownEffectActor>(
		AreaEffectClass,
		SpawnTransform,
		SpawnParams); // <- 바로 이 부분!

	// -------------------- [수정된 부분 끝] --------------------
}