#ifndef DailyInstanceCfgMaster_h__
#define DailyInstanceCfgMaster_h__

#include "CfgMasterBase.h"
#include "DailyInstanceCfg.pb.h"
#include <map>
#include <set>
using namespace com::cfg::vo;
using namespace std;

class DailyInstanceCfgMaster: public CCfgMasterBase<DailyInstanceCfgSet>
{
public:
	DailyInstanceCfgMaster() {};
	~DailyInstanceCfgMaster() {};

    int getInstanceSize(); // һ����ʾ�����
    const DailyInstanceCfg* getOneCfg(const int iKey);
    void sortTodayInstance(const int iToday, vector<int>& stResult); //�����쿪���Ļ��ǰ��
    bool isInstanceOpen(const int iToday, const int iCfgKey);
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
        m_stDayMap.clear();
	}


private:
    // 0 ��ʾ�����죬1-6��ʾ����1������6
    map<int, set<int> > m_stDayMap; //<day, {keys set} >, ��ȡ��day�����Ļ��
};

#define DAILY_INST_CFG_MASTER CVLib::CVSingleton<DailyInstanceCfgMaster>::Instance()

#endif // DailyInstanceCfgMaster_h__
