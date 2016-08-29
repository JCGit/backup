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
    const vector<int>* getTypeIndexList(const int iThingType); //�������ͣ���ȡ��ӵ�е���Ƭ��Ϣ
    int getNumByCfgID(const int iCfgID);

    const vector<int>* getTypeCfgList(const int iThingType); //�������ͣ���ȡ��û�е���Ƭ

	void sendDebrisCompose(const int iCfgID);//�ϳ�
private:
    static bool compareDebris(const int iAIndex, const int iBIndex);
private:
	FragProxy m_stFragProxy;

	FragPanel *m_pFragPanel;

    CSDebrisBagInfo m_stBagInfo;
    map<int, vector<int> > m_stTypeMap; //��ӵ�е���Ƭ��Ϣ��<ThingType,[bag index list] > ���ݲ�ͬ����������
    map<int , int > m_stNumMap; //<CfgID, num> ��ӵ�е���Ƭ������Ϣ

    map<int, vector<int> > m_stCfgTypeMap; //��û�е���Ƭ��Ϣ <ThingType, [cfg id]>
	
	unsigned int m_composeDebrisID;//���ϳɵ���Ƭ
};

#define P_FRAG_CTRL CVLib::CVSingleton<FragControl>::Instance()

#endif // FragControl_h__