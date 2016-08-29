#ifndef DailyInstanceControl_h__
#define DailyInstanceControl_h__

#include "vlib/base/CVSingleton.h"
#include "DailyInstanceProxy.h"
#include "DailyInstancePanel.h"
#include "CSWarMapMsg.pb.h"

using namespace vmsg;

class DailyInstanceControl 
{
public:
	DailyInstanceControl();
	~DailyInstanceControl();

	DailyInstancePanel* getDailyInstancePanel();


    DailyInstanceProxy* getProxy() {
        return &m_stProxy;
    }

    void updateQueryRsp(const CSQueryDailyInstanceRsp& stRsp);
    void updatePassRsp(const CSDailyInstancePassRsp& stRsp);
    int getRemainChallengeCnt(const int iCfgKey);
    void finalizePanel();
    void setNowFightInfo(const int iSelCfgKey, const int iSelLvIdx);
    
    //ս�����ֵ���
    int getWarMapID();
    void doWarWin();
private:
    DailyInstancePanel* m_pDailyInstancePanel;
	DailyInstanceProxy m_stProxy;

    map<int, int> m_stInfo; //<CfgKey ����key, ChallengeCnt ����������ս�Ĵ���>

    int m_iSelCfgKey;
    int m_iSelLvIdx;
    int m_iWarMapID;

};

#define DAILY_INSTANCE_CTRL CVLib::CVSingleton<DailyInstanceControl>::Instance()

#endif // DailyInstanceControl_h__