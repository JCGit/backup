#include "EquipLibControl.h"

#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/EquipStarStepCfgMaster.h"
#include "CfgMaster/EquipExpItemCfgMaster.h"

#include "game/UserObj.h"
#include "game/bag/BagControl.h"
#include "game/notification/NotificationControl.h"

EquipLibControl::EquipLibControl()
{
    m_pEquipLibPanel = NULL;
    m_pEquipSelPanel = NULL;
    m_pEquipSelMatPanel = NULL;
    m_pEquipLibListPanel = NULL;
}

EquipLibControl::~EquipLibControl()
{
    finalizePanel();
}

EquipLibProxy *EquipLibControl::getProxy() {
    return &m_stEquipLibProxy;
}



EquipLibPanel* EquipLibControl::getEquipLibPanel() {
    if(m_pEquipLibPanel == NULL) {
        m_pEquipLibPanel = EquipLibPanel::create();
        m_pEquipLibPanel->retain();
    }

    return m_pEquipLibPanel;
}


EquipSelPanel* EquipLibControl::getEquipSelPanel() {
    if(m_pEquipSelPanel == NULL) {
        m_pEquipSelPanel = EquipSelPanel::create();
        m_pEquipSelPanel->retain();
    }

    return m_pEquipSelPanel;
}

EquipSelMatPanel* EquipLibControl::getEquipSelMatPanel() {
    if(m_pEquipSelMatPanel == NULL) {
        m_pEquipSelMatPanel = EquipSelMatPanel::create();
        m_pEquipSelMatPanel->retain();
    }

    return m_pEquipSelMatPanel;
}


EquipLibListPanel* EquipLibControl::getEquipLibListPanel() {
    if(m_pEquipLibListPanel == NULL) {
        m_pEquipLibListPanel = EquipLibListPanel::create();
        m_pEquipLibListPanel->retain();
    }

    return m_pEquipLibListPanel;
}

void EquipLibControl::notifyEquipLvUp() {
	const NotificationCfg *pNotificationCfg = NOTIFICATION_CFG_MASTER->getNotificationCfg(ENTRY_EQUIP_CULTURE, NOTIFY_TAG_EQUIP_LVUP);

	if(pNotificationCfg != NULL) {
		unsigned int uTotalCnt = 0; // ²ÄÁÏ×ÜÊý
		unsigned int uThresholdCnt = pNotificationCfg->wordsparams(0);
		const EquipExpItemCfgSet &stCfgSet = EQUIPEXPITEM_CFG_MASTER->GetCfg();

		for(int i = 0; i < stCfgSet.equipexpitemcfg_size(); i++) {
			const EquipExpItemCfg &stEquipExpItemCfg = stCfgSet.equipexpitemcfg(i);
			unsigned int uItemCfgID = stEquipExpItemCfg.itemcfgid();

			uTotalCnt += P_BAG_CTRL->getItemCnt(uItemCfgID);

			if(uTotalCnt >= uThresholdCnt) {
				break;
			}
		}

		if(uTotalCnt >= uThresholdCnt) {
			PUSH_NOTIFICATION(ENTRY_EQUIP_CULTURE, NOTIFY_TAG_EQUIP_LVUP, 1);
		} else {
			PUSH_NOTIFICATION(ENTRY_EQUIP_CULTURE, NOTIFY_TAG_EQUIP_LVUP, 0);
		}
	}
}

void EquipLibControl::notifyEquipAdvance() {
	bool bRet = false;

	vector<vmsg::CSEquipInfo> stEquipInfos;

	// role equip
	const vector<vmsg::CSEquipInfo>& stRoleEquipLst = P_USER_OBJ->getUserInfo()->stEquipLst;
	stEquipInfos = stRoleEquipLst;

	// spare equip
	const vmsg::CSEquipInfo& stSpareEquip = P_USER_OBJ->getUserInfo()->spareEquip;

	if(stSpareEquip.equipguid() != 0) {
		stEquipInfos.push_back(stSpareEquip);
	}

	// bag equip
	const vmsg::CSBagInfo& stBagInfo = P_BAG_CTRL->getBagInfo();

	for (int i = 0; i < stBagInfo.celllst_size(); i++) {
		const vmsg::CSBagCellInfo &stBagCellInfo = stBagInfo.celllst(i);

		if(stBagCellInfo.has_equipinfo()) {
			stEquipInfos.push_back(stBagCellInfo.equipinfo());
		}
	}

	for(unsigned int i = 0; i < stEquipInfos.size(); i++) {
		const vmsg::CSEquipInfo &stEquipInfo = stEquipInfos[i];
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		const EquipStarStepCfg *pEquipAdvanceCfg = EQUIPSTARSTEP_CFG_MASTER->GetCfg(pEquipCfg->initstar(), pEquipCfg->initstep());
		unsigned int uMaxLv = P_EQUIP_LIB_CTRL->getObj()->getEquipMaxLvUpLv(stEquipInfo.equipid());

		if(stEquipInfo.level() < uMaxLv
			|| pEquipAdvanceCfg == NULL 
			|| (pEquipAdvanceCfg->star() == pEquipAdvanceCfg->nextstar() && pEquipAdvanceCfg->step() == pEquipAdvanceCfg->nextstep())) {
				continue;
		}

		bool isMaterialEnough = true;

		for(int i = 0; i < pEquipAdvanceCfg->needs_size(); i++) {
			const EquipNeeds &stEquipNeeds = pEquipAdvanceCfg->needs(i);

			if(P_BAG_CTRL->getItemCnt(stEquipNeeds.cfgid()) < stEquipNeeds.cfgcnt()) {
				isMaterialEnough = false;
				break;
			}
		}

		if(isMaterialEnough && pEquipAdvanceCfg->needequipcnt() > 0) {
			if(P_BAG_CTRL->getEquipCnt(stEquipInfo.equipid(), stEquipInfo.equipguid()) < (int)pEquipAdvanceCfg->needequipcnt()) {
				isMaterialEnough = false;
			}
		}

		if(isMaterialEnough && P_USER_OBJ->checkCoin(pEquipAdvanceCfg->costcoin()) == 0) {
			bRet = true;
			break;
		}
	}

	if(bRet) {
		PUSH_NOTIFICATION(ENTRY_EQUIP_CULTURE, NOTIFY_TAG_EQUIP_ADVANCE, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_EQUIP_CULTURE, NOTIFY_TAG_EQUIP_ADVANCE, 0);
	}
}

void EquipLibControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pEquipLibPanel);
    CC_SAFE_RELEASE_NULL(m_pEquipSelPanel);
    CC_SAFE_RELEASE_NULL(m_pEquipSelMatPanel);
    CC_SAFE_RELEASE_NULL(m_pEquipLibListPanel);
}


