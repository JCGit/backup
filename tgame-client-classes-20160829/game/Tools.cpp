#include "Tools.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv\iconv.h"

//字符转换，使cocos2d-x在win32平台支持中文显示
int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode)
{
	iconv_t iconvH;
	iconvH = iconv_open(formCode,toCode);
	if(iconvH == 0)
	{
		return -1;
	}

	const char* strChar = gbkStr.c_str();
	const char** pin = &strChar;

	size_t strLength = gbkStr.length();
	char* outbuf = (char*)malloc(strLength*4);
	char* pBuff = outbuf;
	memset(outbuf,0,strLength*4);
	size_t outLength = strLength*4;
	if(-1 == iconv(iconvH,pin,&strLength,&outbuf,&outLength))
	{
		iconv_close(iconvH);
		return -1;
	}

	gbkStr = pBuff;
	iconv_close(iconvH);
	return 0;
}
/**
**在封装一层，直接传入一个string，转换后还回对应的编码给你
*/
const char* GBKToUTF(std::string &gbkStr)
{
		 GBKToUTF8(gbkStr,"gbk","utf-8"); //后面两个参数就默认了,免得后面再传参麻烦

		 return gbkStr.c_str();
}

#endif
