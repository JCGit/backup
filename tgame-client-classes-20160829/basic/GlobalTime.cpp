#include "GlobalTime.h"
#include "TimeEx.h"
#include "game/notification/NotificationControl.h"
#include "Global.h"

unsigned int GlobalTime::m_iNowTime = 0;
unsigned int GlobalTime::m_uLastTime = 0;
unsigned int GlobalTime::m_uTodayZeroTime = 0;

void GlobalTime::updateTime(){
    m_iNowTime = TimeEx::getTimeVal();

	if(m_iNowTime > m_uLastTime) {
		m_uLastTime = m_iNowTime;
		P_NOTIFICATION_CTRL->secTimer(m_uLastTime);
	}

    //check today zero time
    if(m_uTodayZeroTime == 0) {
        //init
        m_uTodayZeroTime = TimeEx::getTodayZeroTime();
    }

    if (m_iNowTime - m_uTodayZeroTime >= 24 * 3600) {
        m_uTodayZeroTime += 24 * 3600;
        //it's a new day
        //过了0点了，新的一天
        CCLOG("[trace]GlobalTime::updateTime, it's a new day!");
        P_GLOBAL->onNewDayRefresh();
    }
}

bool GlobalTime::isToday(unsigned int uiTime) {
    if(m_uTodayZeroTime == 0) {
        //init
        m_uTodayZeroTime = TimeEx::getTodayZeroTime();
    }

    return (int)uiTime >= m_uTodayZeroTime  && (int)uiTime < m_uTodayZeroTime + 24*3600;
}
