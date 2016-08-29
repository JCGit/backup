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
    WeaponSelMatPanel* getSelMatPanel(); //选择材料面板
    WeaponAdvDonePanel* getWeaponAdvDonePanel(); //枪械进阶完成弹出面板
    WeaponSelPanel* getWeaponSelPanel(); //
	CWeaponArrayPanel* getArrayPanel();
	CWeaponArrRulePanel* getArrRulePanel();
	CWeaponArrOpenPanel* getArrOpenPanel();
	CWeaponArrInfoPanel* getArrInfoPanel();

	bool isWeaponCanAdvance(const vmsg::CSWeapon &stWeapon);

	void notifyWeaponLvUp();

	void notifyWeaponAdvance();

    //查询枪械列表
    void sendWeaponBagRqst();
    //换枪
    void sendWeaponChangeRqst(unsigned int bagIdx, UNIT64_t weaponGUID);
    //升级
    void sendWeaponLvUpRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume);
    //升星
    void sendWeaponStarStepRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<UNIT64_t> stNeedWeaponGUIDs);
    //枪械改装
    void sendWeaponRefitRqst(unsigned int bagIdx, UNIT64_t weaponGUID);
	//查询上阵枪械
	void sendQueryWeaponInBattleRqst();
	//修改上阵枪械
	void sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon);


    //查询枪械返回
    void doQueryWeaponBagRsp(const vmsg::CSWeaponBag* info);		

    //枪械升级
    void doWeaponLvUpRsp(const vmsg::CSWeaponLvUpRsp* info);	
    //查询背包中原料
    void doQueryMaterialBagRsp(const vmsg::CSBagInfo* info);		
    //换枪
    void doWeaponChangeRsp(const vmsg::CSWeaponBag* info);	
    //枪械升星
    void doWeaponStarStepRsp(const vmsg::CSWeaponBag* info);
    //枪械改装
    void doWeaponRefitRsp(const vmsg::CSWeaponBag* info);
	//查询上阵枪械返回
	void doQueryWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp);
	//修改上阵枪械返回
	void doChangeWeaponInBattleRsp(const vmsg::CSWeaponInBattleRsp& stRsp);


protected:
private:
    CFireArmsProxy m_stProxy;

    CWeaponLibPanelNew m_stWeaponLibPanel;
    WeaponInfoPanel m_stWeaponInfoPanel;
    WeaponSelMatPanel m_stSelMatPanel; //选择材料面板
    WeaponAdvDonePanel m_stAdvDonePanel; //枪械进阶完成弹出面板
    WeaponSelPanel m_stWeaponSelPanel; //选择枪械面板
	CWeaponArrayPanel m_stArrayPanel;		//枪械战备面板
	CWeaponArrRulePanel m_stArrRulePanel;		//战备规则面板
	CWeaponArrOpenPanel m_stArrOpenPanel;		//战备解锁面板
	CWeaponArrInfoPanel m_stArrInfoPanel;		//阵位上枪械信息面板

};

#define P_WEAPON_CTRL CVLib::CVSingleton<CWeaponLibControl>::Instance()

#endif // WeaponLibControl_h__
