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
    static unsigned int m_iNowTime; //��ǰUnixʱ�������

	static unsigned int m_uLastTime; // ��һ�μ�¼��ʱ�䣬�����ʱ
    static unsigned int m_uTodayZeroTime;
};

#endif // GlobalTime_h__
