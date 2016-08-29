#ifndef WeaponLibControl_h__
#define WeaponLibControl_h__

#include "WeaponLibPanelNew.h"

#include "WeaponInfoPanel.h"
#include "WeaponSelMatPanel.h"
#include "WeaponAdvDonePanel.h"
#include "WeaponSelPanel.h"
#include "WeaponRefitTipPanel.h"
#include "firearms/FireArmsProxy.h"
#include "WeaponArrayPanel.h"
#include "WeaponArrRulePanel.h"
#include "WeaponArrOpenPanel.h"
#include "WeaponArrInfoPanel.h"

namespace vmsg {
    class CSWeaponBag;
    class CSBagInfo;
    class CSWeapon;
};

class CWeaponLibControl
{
public:
	CWeaponLibControl();
	~CWeaponLibControl();

    CWeaponLibPanelNew* getWeaponLibPanel();
    WeaponInfoPanel* getWeaponInfoPanel();
    WeaponSelMatPanel* getSelMatPanel(); //ѡ��������
    WeaponAdvDonePanel* getWeaponAdvDonePanel(); //ǹе������ɵ������
    WeaponSelPanel* getWeaponSelPanel(); //
	CWeaponArrayPanel* getArrayPanel();
	CWeaponArrRulePanel* getArrRulePanel();
	CWeaponArrOpenPanel* getArrOpenPanel();
	CWeaponArrInfoPanel* getArrInfoPanel();

	bool isWeaponCanAdvance(const vmsg::CSWeapon &stWeapon);

	void notifyWeaponLvUp();

	void notifyWeaponAdvance();

    //��ѯǹе�б�
    void sendWeaponBagRqst();
    //��ǹ
    void sendWeaponChangeRqst(unsigned int bagIdx, UNIT64_t weaponGUID);
    //����
    void sendWeaponLvUpRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume);
    //����
    void sendWeaponStarStepRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<UNIT64_t> stNeedWeaponGUIDs);
    //ǹе��װ
    void sendWeaponRefitRqst(unsigned int bagIdx, UNIT64_t weaponGUID);
	//��ѯ����ǹе
	void sendQueryWeaponInBattleRqst();
	//�޸�����ǹе
	void sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon);


    //��ѯǹе����
    void doQueryWeaponBagRsp(const vmsg::CSWeaponBag* info);		

    //ǹе����
    void doWeaponLvUpRsp(const vmsg::CSWeaponLvUpRsp* info);	
    //��ѯ������ԭ��
    void doQueryMaterialBagRsp(const vmsg::CSBagInfo* info);		
    //��ǹ
    void doWeaponChangeRsp(const vmsg::CSWeaponBag* info);	
    //ǹе����
    void doWeaponStarStepRsp(const vmsg::CSWeaponBag* info);
    //ǹе��װ
    void doWeaponRefitRsp(const vmsg::CSWeaponBag* info);
	//��ѯ����ǹе����
	void doQueryWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp);
	//�޸�����ǹе����
	void doChangeWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp);


protected:
private:
    CFireArmsProxy m_stProxy;

    CWeaponLibPanelNew m_stWeaponLibPanel;
    WeaponInfoPanel m_stWeaponInfoPanel;
    WeaponSelMatPanel m_stSelMatPanel; //ѡ��������
    WeaponAdvDonePanel m_stAdvDonePanel; //ǹе������ɵ������
    WeaponSelPanel m_stWeaponSelPanel; //ѡ��ǹе���
	CWeaponArrayPanel m_stArrayPanel;		//ǹеս�����
	CWeaponArrRulePanel m_stArrRulePanel;		//ս���������
	CWeaponArrOpenPanel m_stArrOpenPanel;		//ս���������
	CWeaponArrInfoPanel m_stArrInfoPanel;		//��λ��ǹе��Ϣ���

};

#define P_WEAPON_CTRL CVLib::CVSingleton<CWeaponLibControl>::Instance()

#endif // WeaponLibControl_h__
