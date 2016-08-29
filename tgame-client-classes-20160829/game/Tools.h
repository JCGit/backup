#ifndef _TOOLS_H_
#define  _TOOLS_H_

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "iconv\iconv.h"

int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode);


#endif

#endif

