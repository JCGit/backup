#include "TimeEx.h"
#include "defines.h"
#include "NounHelper.h"
#include "cocos2d.h"
#include "Global.h"

using namespace std;

TimeEx::TimeEx()
{
}

TimeEx::~TimeEx()
{
}

time_t TimeEx::getTimeVal() {
	time_t tmVal;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&tmVal);
#else
	struct cc_timeval tv;
	CCTime::gettimeofdayCocos2d(&tv, NULL);
	tmVal = tv.tv_sec;
#endif

	return tmVal;
}

struct tm* TimeEx::getTime() {
	time_t tmVal = getTimeVal();
	struct tm *t = localtime(&tmVal);
	return t;
}

string TimeEx::getTimeStr() {
	struct tm *tm = getTime();

	string tmstr = "";
	tmstr = tmstr \
		+ to_string(1900 + tm->tm_year) + "/" \
		+ to_string(tm->tm_mon + 1) + "/" \
		+ to_string(tm->tm_mday) + " " \
		+ to_string(tm->tm_hour) + ":" \
		+ to_string(tm->tm_min) + ":" \
		+ to_string(tm->tm_sec);

	return tmstr;
}

int TimeEx::getTodayZeroTime()
{

    time_t tTime = getTimeVal();
    struct tm* pstTemp = localtime(&tTime);
    pstTemp->tm_hour = 0;
    pstTemp->tm_min = 0;
    pstTemp->tm_sec = 0;
    return mktime(pstTemp);
}

/*
* 将一个时间数转换成"00:00:00"格式
* format:显示格式  时h 分m 秒s 
*/

string TimeEx::getStringTimeForInt(unsigned int timeInt, string format /* = "hms" */){
	string timeStr = "";
	time_t timep = timeInt;  
	tm* timeStuct = gmtime(&timep);  

	if(format.find("h") != string::npos){
		timeStr += intToString(100 + (timeStuct->tm_hour)).substr(1, 2);
	}

	if(format.find("m") != string::npos){
		if(format.find("h") != string::npos){
			timeStr += ":";
		}
		timeStr += intToString(100 + (timeStuct->tm_min)).substr(1, 2);
	}

	if(format.find("s") != string::npos){
		timeStr += ":";
		timeStr += intToString(100 + (timeStuct->tm_sec)).substr(1, 2);
	}

	return timeStr;
}

/*
* 将一个时间转换成"XX年XX月XX日XX时XX分XX秒"
* format:显示格式  年Y 月M 日D 时h 分m 秒s 
*/

string TimeEx::parseSecondToDate(unsigned int timeInt, std::string format /* = "YMDhms" */){
	string timeStr = "";
	time_t timep = timeInt;  
	tm* timeStuct = localtime(&timep);  

	if(format.find("Y") != string::npos){
		timeStr += intToString(1900 + timeStuct->tm_year);
	}

	if(format.find("M") != string::npos){
		timeStr += "/";
		timeStr += intToString((timeStuct->tm_mon + 1));
	}

	if(format.find("D") != string::npos){
		timeStr += "/";
		timeStr += intToString((timeStuct->tm_mday));
	}

	if(format.find("h") != string::npos){
		timeStr += "/";
		timeStr += intToString((timeStuct->tm_hour));
	}

	if(format.find("m") != string::npos){
		timeStr += "/";
		timeStr += intToString((timeStuct->tm_min));
	}

	if(format.find("s") != string::npos){
		timeStr += "/";
		timeStr += intToString((timeStuct->tm_sec));
	}

	return timeStr;
}



std::string TimeEx::getTimeStr(const int iTime, const std::string& strFormat) {
    std::string str("");
    int day = int(iTime/86400);
    int iRemainTime = iTime%86400;
    int hour = int(iRemainTime/3600);

    iRemainTime = iTime%3600; //min
    int minute = int(iRemainTime/60);

    iRemainTime = iTime%60; //min
    int second = iRemainTime;
    int iCnt = 0;
    for(unsigned int i = 0; i < strFormat.size(); ++i) {
        switch(strFormat[i]) {
        case 'd':
            if(day > 0) {
                str += intToString(day) + NounStr(DAY);
                iCnt++;
            }
            break;
        case 'h':
            if(hour > 0) {
                str += intToString(hour) + NounStr(HOUR);
                iCnt++;
            }
            break;
        case 'm':
            if(minute > 0) {
                str += intToString(minute) + NounStr(MINUTE);
                iCnt++;
            }
            break;
        case 's':
            if(second > 0 || str.size() == 0) {
                str += intToString(second) + NounStr(SECOND);
            }
            break;
        }
        if(iCnt >= 1) { //最多只显示1个时间段吧
            break;
        }
    }
    return str;
}

bool TimeEx::isToddy(time_t tmVal) {
	unsigned int days1 = P_GLOBAL->getSvrTime() / (24 * 3600);
	unsigned int days2 = tmVal / (24 * 3600);
	return days1 == days2;
}
