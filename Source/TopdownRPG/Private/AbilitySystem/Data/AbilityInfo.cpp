

#include "AbilitySystem/Data/AbilityInfo.h"

#include "TopdownRPG/TopdownLogChannels.h"

FTopdownAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FTopdownAbilityInfo& Info : AbilityInformation)
	{
		if(Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTopdown, Error, TEXT("어빌리티 정보 [%s]에서 태그정보[%s]를 찾을 수 없음 "), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FTopdownAbilityInfo();
}

