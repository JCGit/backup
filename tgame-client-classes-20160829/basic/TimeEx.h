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

	static std::string getStringTimeForInt(unsigned int timeInt, std::string format = "hms");  //将一个时间数转换成"00:00:00"格式

	static std::string parseSecondToDate(unsigned int timeInt, std::string format = "YMDhms");	//将一个时间转换成"XX年XX月XX日XX时XX分XX秒"

    /*
    将时间iTime转为字符串
    iTime 秒
    strFormat 显示格式
    */
    static std::string getTimeStr(const int iTime, const std::string& strFormat = "dhms");
    static int getTodayZeroTime(); //获取今天的0点时间

	static bool isToddy(time_t tmVal); // compared by seconds

private:

};

#endif // TimeEx_h__
