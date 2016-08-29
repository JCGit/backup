#ifndef GlobalTime_h__
#define GlobalTime_h__

class GlobalTime 
{
public:
	GlobalTime(){}
    ~GlobalTime(){}
    static void updateTime();
public:
    static unsigned int getTime(){
        return m_iNowTime;
    }
    static bool isToday(unsigned int uiTime);
private:
    static unsigned int m_iNowTime; //当前Unix时间戳，秒

	static unsigned int m_uLastTime; // 上一次记录的时间，以秒计时
    static unsigned int m_uTodayZeroTime;
};

#endif // GlobalTime_h__
