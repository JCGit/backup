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

    int getInstanceSize(); // 一共显示几个活动
    const DailyInstanceCfg* getOneCfg(const int iKey);
    void sortTodayInstance(const int iToday, vector<int>& stResult); //将今天开启的活动排前面
    bool isInstanceOpen(const int iToday, const int iCfgKey);
protected:
	virtual int ValidateCfg() const;

	virtual int SetCfg();

	virtual void Clear() {
        m_stDayMap.clear();
	}


private:
    // 0 标示星期天，1-6标示星期1到星期6
    map<int, set<int> > m_stDayMap; //<day, {keys set} >, 获取该day开启的活动数
};

#define DAILY_INST_CFG_MASTER CVLib::CVSingleton<DailyInstanceCfgMaster>::Instance()

#endif // DailyInstanceCfgMaster_h__
