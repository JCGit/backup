#ifndef __DAILYACT_CONTROL_h__
#define __DAILYACT_CONTROL_h__

#include "DailyActProxy.h"
#include "DailyActPanel.h"
#include "CSActMsg.pb.h"

using namespace vmsg;

class GuardActScene;

class DailyActControl
{
public:
    DailyActControl();
    ~DailyActControl();

    DailyActProxy* getProxy();

    void setGuardActScene(GuardActScene *scene){
        m_pGuardActScene = scene;
    }
    GuardActScene* getGuardActScene(){
        return m_pGuardActScene;
    }

    DailyActPanel* getDailyActPanel();

	bool isCanGetManual();

	void notifyGetManual();

    void doDailyActQueryRsp(const CSActQueryRsp& stMsg);
	const CSActQueryRsp& getDailyActInfo(){
		return m_stActInfo;
	}

	void doDailyActNotify(const CSActStatChange& stInfo);

	void doGetManualRsp();

	void notifyDailyAct();

	bool hasDailyActStart();

private:
    DailyActProxy m_stDailyActProxy;
    GuardActScene *m_pGuardActScene;
    DailyActPanel m_stDailyActPanel;

	CSActQueryRsp m_stActInfo;
};

#define P_DAILYACT_CTRL CVLib::CVSingleton<DailyActControl>::Instance()

#endif // __DAILYACT_CONTROL_h__
