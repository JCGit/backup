#ifndef DebrisCfgMaster_h__
#define DebrisCfgMaster_h__

#include "DebrisCfg.pb.h"
#include "CfgMasterBase.h"
#include <map>

using namespace com::cfg::vo;
using namespace std;

class CDebrisCfgMaster : public CCfgMasterBase<DebrisCfgSet>
{
public:

    CDebrisCfgMaster(){}
    ~CDebrisCfgMaster(){}

    const DebrisCfg* GetDebrisCfgByID(const unsigned int uiCfgID);
    const string getIconPath(const unsigned int uiCfgID);

    const int getComposeThingStar(const unsigned int uiCfgID); //获取合成物的星级
    const vector<int>* getCfgIDListByType(const int iThingType);
    const map<int, vector<int> >& getTypeMap(){
        return m_stTypeIDListMap;
    }
protected:

    virtual int ValidateCfg()const;

    virtual int SetCfg();

    virtual void Clear() 
    {
        m_stID2IDX.clear(); 
        m_stTypeIDListMap.clear();
    }

    static bool compareDebris(const int iACfgID, const int iBCfgID); //排序m_stTypeIDListMap
private:
    map<unsigned int, int> m_stID2IDX;

    map<int, vector<int> > m_stTypeIDListMap; //<ThingType, [cfg id list] >
};

#define DEBRIS_CFG_MASTER (CVSingleton<CDebrisCfgMaster>::Instance())

#endif // DebrisCfgMaster_h__
