

#include "AbilitySystem/Data/MonsterClassInfo.h"

FMonsterClassDefaultInfo UMonsterClassInfo::GetClassDefaultInfo(EMonsterType MonsterClass)
{
	return MonsterClassInfo.FindChecked(MonsterClass);
}
