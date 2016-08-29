#include "Tools.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv\iconv.h"

//�ַ�ת����ʹcocos2d-x��win32ƽ̨֧��������ʾ
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
**�ڷ�װһ�㣬ֱ�Ӵ���һ��string��ת���󻹻ض�Ӧ�ı������
*/
const char* GBKToUTF(std::string &gbkStr)
{
		 GBKToUTF8(gbkStr,"gbk","utf-8"); //��������������Ĭ����,��ú����ٴ����鷳

		 return gbkStr.c_str();
}

#endif
