#ifndef FragControl_h__
#define FragControl_h__

#include "vlib/base/CVSingleton.h"
#include "CSDebrisMsg.pb.h"
#include "FragProxy.h"
#include "FragPanel.h"

using namespace vmsg;

class FragControl 
{
public:
	FragControl();
	~FragControl();
    
	void finalize();

    FragPanel* getFragPanel();

    
    FragProxy* getProxy() {
        return &m_stFragProxy;
    }

    const CSDebrisInfo* getOneFragByIdx(const int iBagIdx);
    const int getBagIdxByCfgID(const int iCfgID);
    void setDebrisBagInfo(const CSDebrisBagInfo& stInfo, const bool bIsComposeRsp = false);
    const vector<int>* getTypeIndexList(const int iThingType); //根据类型，获取我拥有的碎片信息
    int getNumByCfgID(const int iCfgID);

    const vector<int>* getTypeCfgList(const int iThingType); //根据类型，获取我没有的碎片

	void sendDebrisCompose(const int iCfgID);//合成
private:
    static bool compareDebris(const int iAIndex, const int iBIndex);
private:
	FragProxy m_stFragProxy;

	FragPanel *m_pFragPanel;

    CSDebrisBagInfo m_stBagInfo;
    map<int, vector<int> > m_stTypeMap; //我拥有的碎片信息，<ThingType,[bag index list] > 根据不同的类型索引
    map<int , int > m_stNumMap; //<CfgID, num> 我拥有的碎片个数信息

    map<int, vector<int> > m_stCfgTypeMap; //我没有的碎片信息 <ThingType, [cfg id]>
	
	unsigned int m_composeDebrisID;//待合成的碎片
};

#define P_FRAG_CTRL CVLib::CVSingleton<FragControl>::Instance()

#endif // FragControl_h__