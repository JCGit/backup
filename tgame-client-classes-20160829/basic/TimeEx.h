#ifndef TimeEx_h__
#define TimeEx_h__

#include "cocos2d.h"

USING_NS_CC;

class TimeEx
{
public:
	TimeEx();
	~TimeEx();

	static time_t getTimeVal();

	static struct tm* getTime();

	static std::string getTimeStr();

	static std::string getStringTimeForInt(unsigned int timeInt, std::string format = "hms");  //��һ��ʱ����ת����"00:00:00"��ʽ

	static std::string parseSecondToDate(unsigned int timeInt, std::string format = "YMDhms");	//��һ��ʱ��ת����"XX��XX��XX��XXʱXX��XX��"

    /*
    ��ʱ��iTimeתΪ�ַ���
    iTime ��
    strFormat ��ʾ��ʽ
    */
    static std::string getTimeStr(const int iTime, const std::string& strFormat = "dhms");
    static int getTodayZeroTime(); //��ȡ�����0��ʱ��

	static bool isToddy(time_t tmVal); // compared by seconds

private:

};

#endif // TimeEx_h__
