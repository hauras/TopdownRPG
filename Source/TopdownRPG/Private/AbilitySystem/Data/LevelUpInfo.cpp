

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	bool bSearching = true;
	int32 Level = 1;
	while (bSearching)
	{
		//LevelUPInformation[1] 은 레벨 1의 레벨업 정보와 같다.
		if (LevelUpInformation.Num() -1 <=Level) return Level;

		if (XP >= LevelUpInformation[Level].LevelUPRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
