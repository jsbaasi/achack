#ifndef GAMEMEMORY_H
#define GAMEMEMORY_H

#include <common.h>

bool initDynamicInfo(const wchar_t* pName, dynamicInfo& di);
void initGameInfo(dynamicInfo& di, gameInfo& gi);

#endif