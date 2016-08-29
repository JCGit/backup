#include "BagControl.h"

#include "game/UserObj.h"
#include "game/evolution/EvolutionControl.h"
#include "game/evolution/EvolutionPanel.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/login/LoginControl.h"
#include "game/nihility/NihilityControl.h"
#include "game/notification/NotificationControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "fight/LevelControl.h"
#include "fight/FightControl.h"
#include "ConstHelper.h"
#include "game/warmap/WarMapControl.h"

#include "CfgMaster/EquipmentCfgMaster.h"
#include "PopTipLayer.h"

BagControl::BagControl()
:m_pRoleInfoPanel(NULL)
,m_pItemInfoPanel(NULL)
,m_pEquipInfoPanel(NULL)
,m_pOtherRoleInfoPanel(NULL)
,m_pPackageInfoPanel(NULL)
{
	
}

BagControl::~BagControl()
{
	CC_SAFE_RELEASE_NULL(m_pItemInfoPanel);
	CC_SAFE_RELEASE_NULL(m_pEquipInfoPanel);
	CC_SAFE_RELEASE_NULL(m_pOtherRoleInfoPanel);
	CC_SAFE_RELEASE_NULL(m_pPackageInfoPanel);
}

ItemInfoPanel* BagControl::getItemInfoPanel() {
	if(m_pItemInfoPanel == NULL) {
		m_pItemInfoPanel = ItemInfoPanel::create();
		m_pItemInfoPanel->retain();
	}

	return m_pItemInfoPanel;
}

EquipInfoPanel* BagControl::getEquipInfoPanel() {
	if(m_pEquipInfoPanel == NULL) {
		m_pEquipInfoPanel = EquipInfoPanel::create();
		m_pEquipInfoPanel->retain();
	}

	return m_pEquipInfoPanel;
}

RoleInfoPanel* BagControl::getRoleInfoPanel() {
	if(m_pRoleInfoPanel == NULL) {
		m_pRoleInfoPanel = RoleInfoPanel::create();
		m_pRoleInfoPanel->retain();
	}

	return m_pRoleInfoPanel;
}

OtherRoleInfoPanel* BagControl::getOtherRoleInfoPanel() {
	if(m_pOtherRoleInfoPanel == NULL) {
		m_pOtherRoleInfoPanel = OtherRoleInfoPanel::create();
		m_pOtherRoleInfoPanel->retain();
	}

	return m_pOtherRoleInfoPanel;
}

PackageInfoPanel* BagControl::getPackageInfoPanel() {
	if(m_pPackageInfoPanel == NULL) {
		m_pPackageInfoPanel = PackageInfoPanel::create();
		CC_SAFE_RETAIN(m_pPackageInfoPanel);
	}

	return m_pPackageInfoPanel;
}

CBuyItemPanel* BagControl::getBuyItemPanel(){
	if(!m_stBuyItemPanel.isInited()){
		m_stBuyItemPanel.init();
	}

	return &m_stBuyItemPanel;
}

void BagControl::requestOtherRoleInfo(UIN_t uin) {
	getOtherRoleInfoPanel()->open();
	P_LOGIN_CTRL->getProxy()->sendQueryOtherPlayerInfoRqst(uin);
}

void BagControl::updateOtherRoleInfo() {
	if(m_pOtherRoleInfoPanel != NULL && m_pOtherRoleInfoPanel->isOpen()) {
		m_pOtherRoleInfoPanel->updateUI();
	}
}

const vmsg::CSBagInfo& BagControl::getBagInfo() {
	return m_stBagObj.getBagInfo();
}

void BagControl::setBagInfo(const vmsg::CSBagInfo& stMsg){
	m_stBagObj.setBagInfo(stMsg);
	notifyAdvancedEquipInBag();
	P_WEAPON_CTRL->notifyWeaponLvUp();
	P_EQUIP_LIB_CTRL->notifyEquipLvUp();
	P_EVOLUTION_CTRL->notifyEvolution();

    m_stItemNumMap.clear();
    m_stEquipNumMap.clear();
    int cfgid = 0;
    int cnt = 0;
    for(int i = 0; i < stMsg.celllst_size(); i++) {
        if(stMsg.celllst(i).has_iteminfo()) {
            // 可能是可堆叠的物品
            cfgid = stMsg.celllst(i).iteminfo().itemcfgid();
            cnt = m_stItemNumMap[cfgid];
            m_stItemNumMap[cfgid] = cnt + stMsg.celllst(i).iteminfo().itemcnt();
        } else if(stMsg.celllst(i).has_equipinfo()){
            cfgid = stMsg.celllst(i).equipinfo().equipid();
            set<UINT64_t>& stOneEquipSet = m_stEquipNumMap[cfgid];
            stOneEquipSet.insert( stMsg.celllst(i).equipinfo().equipguid() );
        }
    }

}

vmsg::CSBagInfo& BagControl::mutableBagInfo() {
	return m_stBagObj.mutableBagInfo();
}

const vmsg::CSBagCellInfo& BagControl::getSelectedBagCellInfo() {
	return m_stBagObj.getSelectedBagCellInfo();
}

void BagControl::setSelectedBagCellInfo(const vmsg::CSBagCellInfo &stBagCellInfo) {
	m_stBagObj.setSelectedBagCellInfo(stBagCellInfo);
}

int BagControl::getIdxByEquipGUID(const UINT64_t uEquipGUID) {
	int iRet = -1;

	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();
	
	for (int i = 0; i < stBagInfo.celllst_size(); i++)
	{
		if(stBagInfo.celllst(i).has_equipinfo()
		&& uEquipGUID == stBagInfo.celllst(i).equipinfo().equipguid()) {
			iRet = stBagInfo.celllst(i).index();
			break;
		}
	}
	return iRet;
}

int BagControl::getIdxByItemCfgID(unsigned int uItemCfgID) {
	int iRet = -1;

	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();

	for (int i = 0; i < stBagInfo.celllst_size(); i++)
	{
		if(stBagInfo.celllst(i).has_iteminfo()
		&& uItemCfgID == stBagInfo.celllst(i).iteminfo().itemcfgid()) {
				iRet = stBagInfo.celllst(i).index();
				break;
		}
	}
	return iRet;
}

bool BagControl::checkItem(const unsigned int uItemCfgID, const unsigned int uItemCnt) {
	bool bRet = false;

	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();
	unsigned int uTotalCnt = 0;

	for(int i = 0; i < stBagInfo.celllst_size(); i++) {
		if(stBagInfo.celllst(i).has_iteminfo()
		&& uItemCfgID == stBagInfo.celllst(i).iteminfo().itemcfgid()) {
			uTotalCnt += stBagInfo.celllst(i).iteminfo().itemcnt(); // 可能是可堆叠的物品

			if(uTotalCnt >= uItemCnt) {
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

unsigned int BagControl::getItemCnt(unsigned int uItemCfgID) {
    if(m_stItemNumMap.count(uItemCfgID) == 0) {
        return 0;
    }
    return m_stItemNumMap[uItemCfgID];
}

void BagControl::updateItemCnt(unsigned int uItemCfgID, const unsigned int uItemCnt){
	m_stItemNumMap[uItemCfgID] = uItemCnt;
}

int BagControl::getEquipCnt(unsigned int uEquipCfgID, const UINT64_t uIgnoreGuid) {
    if(m_stEquipNumMap.count(uEquipCfgID) == 0) {
        return 0;
    }
    set<UINT64_t>& stOneSet = m_stEquipNumMap[uEquipCfgID];
    int iIgnoreEquip = stOneSet.count(uIgnoreGuid);
    
    return (stOneSet.size() - iIgnoreEquip);
}
const set<UINT64_t>* BagControl::getBagEquipGuidLst(const unsigned int uEquipCfgID) {
    if(m_stEquipNumMap.count(uEquipCfgID) == 0) {
        return NULL;
    }
    return &(m_stEquipNumMap[uEquipCfgID]);
}

bool BagControl::isFull() {
	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();
	unsigned int useNum = stBagInfo.celllst_size();
	return useNum >= stBagInfo.capacity();
}

unsigned int BagControl::getEmptyCellCnt() {
	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();
	return stBagInfo.capacity() - stBagInfo.celllst_size();
}

bool BagControl::hasAdvancedEquipInBag() {
	bool bRet = false;

	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	const vmsg::CSBagInfo &stBagInfo = m_stBagObj.getBagInfo();

	// 找出背包中不同部位的最强装备
	map<unsigned int, vmsg::CSEquipInfo> stEquipInfoMap;

	for(int i = 0; i < stBagInfo.celllst_size(); i++) {
		const vmsg::CSBagCellInfo &stBagCellInfo = stBagInfo.celllst(i);

		if(stBagCellInfo.has_equipinfo()) {
			const vmsg::CSEquipInfo &stEquipInfo = stBagCellInfo.equipinfo();
			const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
			unsigned int part = pEquipCfg->part();

			if(stEquipInfoMap.find(part) == stEquipInfoMap.end()) {
				stEquipInfoMap.insert(make_pair(part, stEquipInfo));
			} else {
				const vmsg::CSEquipInfo &stLastEquipInfo = stEquipInfoMap[part];
				const EquipmentCfg *pLastEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stLastEquipInfo.equipid());

				if(pEquipCfg->attr().attrid() == pLastEquipCfg->attr().attrid()
					&& pEquipCfg->attr().attrval() > pLastEquipCfg->attr().attrval()) {
						stEquipInfoMap[part] = stEquipInfo;
				}
			}
		}
	}

	// 对比身上的装备
	if(pUserInfo->stEquipLst.size() < stEquipInfoMap.size()) {
		bRet = true;
	} else {
		for(unsigned int i = 0; i < pUserInfo->stEquipLst.size(); i++) {
			const vmsg::CSEquipInfo &stCurEquipInfo = pUserInfo->stEquipLst[i];
			const EquipmentCfg *pCurEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stCurEquipInfo.equipid());
			unsigned int part = pCurEquipCfg->part();

			if(stEquipInfoMap.find(part) != stEquipInfoMap.end()) {
				const vmsg::CSEquipInfo &stEquipInfo = stEquipInfoMap[part];
				const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());

				if(pEquipCfg->attr().attrid() == pCurEquipCfg->attr().attrid()
					&& pEquipCfg->attr().attrval() > pCurEquipCfg->attr().attrval()) {
						bRet = true;
						break;
				}
			}
		}
	}

	return bRet;
}

void BagControl::notifyAdvancedEquipInBag() {
	if(hasAdvancedEquipInBag()) {
		PUSH_NOTIFICATION(ENTRY_ROLE, NOTIFY_TAG_ADVANCED_EQUIP_IN_BAG, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_ROLE, NOTIFY_TAG_ADVANCED_EQUIP_IN_BAG, 0);
	}
}

void BagControl::updateBag() {
	if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
		m_pRoleInfoPanel->updateUI();
	}
}

void BagControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	if(stMsg.has_errmsg()) {

	} else if(stMsg.has_bagrespond()) {
		unsigned int uCmd = stMsg.msg_head().msg_id();
		const vmsg::CSBagRespond &stBagRsp = stMsg.bagrespond();

		if(stBagRsp.has_querybaginfo_rsp()) { // 查询背包信息
            setBagInfo(stBagRsp.querybaginfo_rsp().baginfo());

			if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
				m_pRoleInfoPanel->updateUI();
			}
			
			if(P_EVOLUTION_CTRL->isEvolutionPanelOpen()) {
				P_EVOLUTION_CTRL->getEvolutionPanel()->updateUI();
				P_EVOLUTION_CTRL->notifyEvolution();
			}

			if(P_NIHILITY_CTRL->getItemPanel()->isOpen()){
				P_NIHILITY_CTRL->getItemPanel()->updateUI();
			}

			if(P_LEVEL_CTL->getRelivePanel()->isOpen()){
				P_LEVEL_CTL->getRelivePanel()->updateUI();
			}

			if(P_LEVEL_CTL->getFillBulletPanel()->isOpen()){
				P_LEVEL_CTL->getFillBulletPanel()->updateUI();
			}

			if(P_LEVEL_CTL->getEnterPanel()->isOpen()){
				P_LEVEL_CTL->getEnterPanel()->updateInfoCallBack();
			}

            P_WEAPON_CTRL->doQueryMaterialBagRsp(&stBagRsp.querybaginfo_rsp().baginfo());
			
		} else if(stBagRsp.has_enlarge_rsp()) { // 扩展背包
			stBagRsp.enlarge_rsp().enlargecnt();
		} else if(stBagRsp.has_wearequip_rsp()) { // 穿上装备
			setBagInfo(stBagRsp.wearequip_rsp().baginfo());
			P_USER_OBJ->updateUserDetailInfo(stBagRsp.wearequip_rsp().playerdetailinfo());

			if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
				m_pRoleInfoPanel->updateUI();
			}
            
		} else if(stBagRsp.has_takeoffequip_rsp()) { // 脱下装备
            setBagInfo(stBagRsp.takeoffequip_rsp().baginfo());
            P_USER_OBJ->updateUserDetailInfo(stBagRsp.takeoffequip_rsp().playerdetailinfo());

			if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
				m_pRoleInfoPanel->updateUI();
			}
			
		} else if(stBagRsp.has_bagsell_rsp()) { // 使用物品
			stBagRsp.bagsell_rsp().bagidx();
			sendQueryBagInfoRqst();
		} else if(stBagRsp.has_bagstat_notify()) { // 背包状态通知
			stBagRsp.bagstat_notify().isfull();
		} else if(stBagRsp.has_baginfo_notify()) {
			const vmsg::CSBagInfoNotify &stBagInfoNotify = stBagRsp.baginfo_notify();
			setBagInfo(stBagInfoNotify.baginfo());

			if(stBagInfoNotify.has_pkgid()) {
				unsigned uPackageID = stBagInfoNotify.pkgid();

				P_BAG_CTRL->getPackageInfoPanel()->showPackage(uPackageID);
			}
		}else if(stBagRsp.has_buyitem_rsp()){		//购买道具返回
			setBagInfo(stBagRsp.buyitem_rsp().baginfo());

			if(getBuyItemPanel()->isOpen()){
				getBuyItemPanel()->close();
			}

			if(P_LEVEL_CTL->getBuyMedicalPanel()->isOpen()){
				P_LEVEL_CTL->getBuyMedicalPanel()->doBuyMedicalDone();
			}

			if(P_WAR_MAP_CTRL->getWarMapPanel()->isOpen()) {
				P_WAR_MAP_CTRL->getWarMapPanel()->updateUI();
			}

		}else if(stBagRsp.has_consumeitem_rsp()){	//使用道具返回
			setBagInfo(stBagRsp.consumeitem_rsp().baginfo());

			//使用医药包
			unsigned int uMedicalCfgID = ConstHelper(FIGHT_MEDICAL_CFG_DI);
			if(stBagRsp.consumeitem_rsp().type() == vmsg::THING_TYPE_ITEM && stBagRsp.consumeitem_rsp().cfgid() == uMedicalCfgID){
				P_FIGHT_CTRL->getMedicalLayout()->updateUI();
			}
		}
	} else if(stMsg.has_playerrespond()) {
		const vmsg::CSPlayerRespond &stPlayerRsp = stMsg.playerrespond();

		P_USER_OBJ->updateUserDetailInfo(stPlayerRsp.changearmor_rsp().playerdetailinfo());

		if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
			m_pRoleInfoPanel->updateUI();
		}
		
	} else if(stMsg.has_itemrespond()) {
        //开礼包等协议
        const vmsg::CSItemRespond &stItemRsp = stMsg.itemrespond();
        if(stItemRsp.has_openpackage_rsp()) {
            setBagInfo(stItemRsp.openpackage_rsp().baginfo());

			if(m_pRoleInfoPanel != NULL && m_pRoleInfoPanel->isOpen()) {
				m_pRoleInfoPanel->updateUI();
			}
            for(int i = 0; i < stItemRsp.openpackage_rsp().awardlst_size(); ++i) {
                const vmsg::CSAwardElement& stElmt = stItemRsp.openpackage_rsp().awardlst(i);
                if(vmsg::THING_TYPE_COIN == stElmt.elementtype()) {
                    continue; //coin变化通知里会单独飘字
                }
                POP_TIP_LAYER_MASTER->ShowAwardTip(stElmt.elementtype() ,stElmt.elementid(), stElmt.elementcnt());
            }
        }
    }
}

void BagControl::sendQueryBagInfoRqst() {
	m_stBagProxy.sendQueryBagInfoRqst();
}

void BagControl::sendEnlargeBagRqst(int nEnlargeCnt) {
	m_stBagProxy.sendEnlargeBagRqst(nEnlargeCnt);
}

void BagControl::sendTakeOnEquipRqst(int nCellIdx) {
	m_stBagProxy.sendTakeOnEquipRqst(nCellIdx);
}

void BagControl::sendTakeOffEquipRqst(UINT64_t uEquipGUID) {
	m_stBagProxy.sendTakeOffEquipRqst(uEquipGUID);
}

void BagControl::sendBagSellRqst(int nBagIdx) {
	m_stBagProxy.sendBagSellRqst(nBagIdx);
}

void BagControl::sendChangeArmorRqst() {
	m_stBagProxy.sendChangeArmorRqst();
}

void BagControl::sendOpenPackageRqst(const int iPkgIdx, const int iPkgCfgID) {
    m_stBagProxy.sendOpenPackageRqst(iPkgIdx, iPkgCfgID);
}

void BagControl::sendBuyItemRqst(unsigned int uItemType, unsigned int uItemCfgID, unsigned int uCnt){
	m_stBagProxy.sendBuyItemRqst(uItemType, uItemCfgID, uCnt);
}

void BagControl::sendConsumeItemRqst(unsigned int uItemType, unsigned int uItemID, unsigned int uBagIdx, unsigned int uCnt /* = 1 */){
	m_stBagProxy.sendConsumeItemRqst(uItemType, uItemID, uBagIdx, uCnt);
}

void BagControl::resetOnConnClose() {
    //断线时重置数据
    CCLOG("[trace]BagControl,reset data on connection close.");
    BagObj stUserInfo;
    m_stBagObj = stUserInfo;
    m_stItemNumMap.clear();
    m_stEquipNumMap.clear();
}
