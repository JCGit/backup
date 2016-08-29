#pragma once
#include "Singleton.h"
#include <map>
#include <string>

#include "cocos2d.h"

#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#include <sys/time.h>
#else
#endif

#define TIME_CALC_PVP "save_energy"
#define TIME_CALC "save_power"
#define TIME_PVP_PING "pvp_ping"
#define  TIME_LASTSERVER "save_lastserver"
#define TIME_PVP_FullSrv_Invite "TIME_PVP_FullSrv_Invite_Time"

class TimeCalculator : public cocos2d::Ref
{
private:
	double mInitSeconds;
	double mCurrentSeconds;

	double mPauseSeconds;

	struct TIME_INFO
	{
		float timeleft;
		double registerTime;
		long lastGetTime;
	};
	std::map<std::string,TIME_INFO> mTimes;


	//�ӷ�������ȡ����У��ʱ��
	double m_serverTime;
	double m_clockBegin;
	double getClockTime();
	double getBeginClockTime();
public:

	TimeCalculator();

	void init();
	void update();

	void createTimeCalcultor(std::string key, float timeleft);
	void removeTimeCalcultor(std::string key);
	void resetTimeCalcultor(std::string key);

	void cleanData();

	// ����getTimeLeft(std::string),���㵼��lua�ӿ�
	float getCountDownByKey(std::string key);

	bool getTimeLeft(std::string, float& timeleft);
	float getTimeLeft(std::string);
	float getTimeLength(std::string);
	bool hasKey(std::string key);

	void addTimeByKey(std::string key, float time);

	void pause();
	void resume();

	double getCurTime();

	double getSystemTime();

	unsigned long long getCurrentTime();

	//modify by weihua
	void initServerTime(double serverTime);
	double getServerTime();

	void beganMainCountDown();	//����Ҫһֱ���еĵ���ʱ����������
	void updateCountDown(float dt);

};

#define sgTimeCalculator()	Singleton<TimeCalculator>::Get()
