#include "RefitControl.h"

#include "defines.h"

#include "CfgMaster/DotaLvCfgMaster.h"
#include "CfgMaster/DotaItemCfgMaster.h"

#include "basic/PopTipLayer.h"
#include "basic/NounHelper.h"

#include "game/UserObj.h"
#include "game/notification/NotificationControl.h"

RefitControl::RefitControl()
:m_bInitialized(false)
,m_pRefitPanel(NULL)
,m_pRefitMaterialPanel(NULL)
,m_pRefitDonePanel(NULL)
,m_uMosaicPart(0)
,m_uMosaicDotaItemID(0)
{
}

RefitControl::~RefitControl()
{
	finalize();
}

void RefitControl::init() {
	if(!m_bInitialized) {
		m_pRefitPanel = RefitPanel::create();
		CC_SAFE_RETAIN(m_pRefitPanel);

		m_pRefitMaterialPanel = RefitMaterialPanel::create();
		CC_SAFE_RETAIN(m_pRefitMaterialPanel);

		m_pRefitDonePanel = RefitDonePanel::create();
		CC_SAFE_RETAIN(m_pRefitDonePanel);

		m_bInitialized = true;
	}
}

void RefitControl::finalize() {
	if(m_bInitialized) {
		CC_SAFE_RELEASE_NULL(m_pRefitPanel);

		CC_SAFE_RELEASE_NULL(m_pRefitMaterialPanel);

		CC_SAFE_RELEASE_NULL(m_pRefitDonePanel);

		m_bInitialized = false;
	}
}

RefitPanel* RefitControl::getRefitPanel() {
	return m_pRefitPanel;
}

RefitMaterialPanel* RefitControl::getRefitMaterialPanel() {
	return m_pRefitMaterialPanel;
}

RefitDonePanel* RefitControl::getRefitDonePanel() {
	return m_pRefitDonePanel;
}

const vmsg::CSDotaItemBag& RefitControl::getBagInfo() {
	return m_stBagInfo;
}

const vmsg::CSDotaPartInfo& RefitControl::getPartInfo() {
	return m_stPartInfo;
}

unsigned int RefitControl::getItemCnt(unsigned int uItemID) {
	unsigned int uItemCnt = 0;

	for(int i = 0; i < m_stBagInfo.items_size(); i++) {
		const vmsg::CSDotaItem& stDotaItem = m_stBagInfo.items(i);

		if(uItemID == stDotaItem.dotaitemid()) {
			uItemCnt = stDotaItem.dotaitemcnt();
			break;
		}
	}

	return uItemCnt;
}

bool RefitControl::isItemCanCompose(unsigned int uItemID) {
	bool bRet = true;
	const DotaItemCfg *pNeedItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uItemID);
	
	if(pNeedItemCfg != NULL) {
		bRet = pNeedItemCfg->dotaitemneeds_size() > 0;

		for(int i = 0; i < pNeedItemCfg->dotaitemneeds_size(); i++) {
			const DotaItemComposeNeed &stNeedDotaItem = pNeedItemCfg->dotaitemneeds(i);
			unsigned int uNeedChildItemID = stNeedDotaItem.dotaitemid();

			if(P_REFIT_CTRL->getItemCnt(uNeedChildItemID) < stNeedDotaItem.dotaitemcnt()) {
				bRet = false;
				break;
			}
		}
	} else {
		bRet = false;
	}

	return bRet;
}

unsigned int RefitControl::getItemCntAddCompose(unsigned int uItemID) {
	int nComposeCnt = -1;
	const DotaItemCfg *pNeedItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uItemID);

	if(pNeedItemCfg != NULL && pNeedItemCfg->dotaitemneeds_size() > 0) { // 可通过合成得到
		for(int i = 0; i < pNeedItemCfg->dotaitemneeds_size(); i++) {
			const DotaItemComposeNeed &stNeedDotaItem = pNeedItemCfg->dotaitemneeds(i);
			unsigned int uNeedChildItemID = stNeedDotaItem.dotaitemid();

			unsigned int uChildItemCnt = getItemCntAddCompose(uNeedChildItemID);
			int nTmpCnt = uChildItemCnt / stNeedDotaItem.dotaitemcnt();
			
			if(nComposeCnt == -1) {
				nComposeCnt = nTmpCnt;
			} else {
				nComposeCnt = nComposeCnt < nTmpCnt ? nComposeCnt : nTmpCnt;
			}
		}
	}

	nComposeCnt = nComposeCnt == -1 ? 0 : nComposeCnt;

	return P_REFIT_CTRL->getItemCnt(uItemID) + nComposeCnt;
}

bool RefitControl::isItemCanComposeCountCompose(unsigned int uItemID) {
	bool bRet = true;
	const DotaItemCfg *pNeedItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uItemID);

	if(pNeedItemCfg != NULL) {
		bRet = pNeedItemCfg->dotaitemneeds_size() > 0;

		for(int i = 0; i < pNeedItemCfg->dotaitemneeds_size(); i++) {
			const DotaItemComposeNeed &stNeedDotaItem = pNeedItemCfg->dotaitemneeds(i);
			unsigned int uNeedChildItemID = stNeedDotaItem.dotaitemid();

			if(getItemCntAddCompose(uNeedChildItemID) < stNeedDotaItem.dotaitemcnt()) { // 加上可合成的数量
				bRet = false;
				break;
			}
		}
	} else {
		bRet = false;
	}

	return bRet;
}

bool RefitControl::isPartCanAdd(unsigned int uPart) {
	bool bRet = false;
	const vmsg::CSDotaPart &stPart = m_stPartInfo.parts(uPart);
	const DotaLvCfg *pDotaLvCfg = stPart.ByteSize() > 0 ? DOTA_LV_CFG_MASTER->getDotaLvCfg(stPart.lv(), uPart) : NULL;

	if(pDotaLvCfg != NULL) {
		bool hasItemCanMosaic = false;
		bool hasItemCanCompose = false;

		for(int i = 0; i < pDotaLvCfg->dotalvupneeds_size(); i++) {
			const DotaLvUpNeed &stDotaLvUpNeed = pDotaLvCfg->dotalvupneeds(i);
			unsigned int uNeedItemID = stDotaLvUpNeed.dotaitemid();
			unsigned int uNeedItemCnt = stDotaLvUpNeed.dotaitemcnt();

			bool bAdded = false;

			for(int j = 0; j < stPart.items_size(); j++) {
				if(stPart.items(j) == uNeedItemID) {
					bAdded = true;
					break;
				}
			}

			if(!bAdded) {
				if(P_REFIT_CTRL->getItemCnt(uNeedItemID) >= uNeedItemCnt) {
					hasItemCanMosaic = true;
					break;
				} else if(isItemCanCompose(uNeedItemID)) {
					hasItemCanCompose = true;
					break;
				}
			}
		}

		bRet = hasItemCanMosaic || hasItemCanCompose;
	}

	return bRet;
}

bool RefitControl::isPartCanRefit(unsigned int uPart) {
	const vmsg::CSDotaPart &stPart = m_stPartInfo.parts(uPart);
	const DotaLvCfg *pDotaLvCfg = stPart.ByteSize() > 0 ? DOTA_LV_CFG_MASTER->getDotaLvCfg(stPart.lv(), uPart) : NULL;
	UINT64_t uTotalCoin = P_USER_OBJ->getUserInfo()->coin;
	int nMosaicCnt = 0;

	if(pDotaLvCfg != NULL) {
		for(int i = 0; i < pDotaLvCfg->dotalvupneeds_size(); i++) {
			const DotaLvUpNeed &stDotaLvUpNeed = pDotaLvCfg->dotalvupneeds(i);
			unsigned int uNeedItemID = stDotaLvUpNeed.dotaitemid();

			bool bAdded = false;

			for(int j = 0; j < stPart.items_size(); j++) {
				if(stPart.items(j) == uNeedItemID) {
					bAdded = true;
					break;
				}
			}

			if(bAdded) {
				nMosaicCnt++;
			}
		}
	}

	return pDotaLvCfg != NULL && nMosaicCnt == pDotaLvCfg->dotalvupneeds_size() && uTotalCoin >= pDotaLvCfg->costcoin();
}

void RefitControl::notifyRefit() {
	bool hasPartCanRefitOrAdd = false;

	for(int i = 0; i < m_stPartInfo.parts_size(); i++) {
		if(isPartCanRefit(i) || isPartCanAdd(i)) {
			hasPartCanRefitOrAdd = true;
			break;
		}
	}

	if(hasPartCanRefitOrAdd) {
		PUSH_NOTIFICATION(ENTRY_REFIT, NOTIFY_TAG_REFIT_PART, 1);
	} else {
		PUSH_NOTIFICATION(ENTRY_REFIT, NOTIFY_TAG_REFIT_PART, 0);
	}
}

void RefitControl::onRecvMsg(vmsg::VLogicSvr2CliMsg& stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			if(stMsg.has_errmsg()) {
				const vmsg::CSCommonErrMsg &stErrMsg = stMsg.errmsg();
				unsigned int uRqstCmd = stErrMsg.rqstcmdid();

			}
			break;
		case vmsg::CS_CMD_DOTAITEM_QUERY:
		case vmsg::CS_CMD_DOTAITEM_COMPOSE:
		case vmsg::CS_CMD_DOTAITEM_MOSAIC:
		case vmsg::CS_CMD_DOTAITEM_LVUP:
			if(stMsg.has_dotaitemrespond()) {
				const vmsg::CSDotaItemRespond &stDotaItemRsp = stMsg.dotaitemrespond();

				if(stDotaItemRsp.has_dotaitembagquery_rsp()) { // 查询
					const vmsg::CSDotaInfo &stDotaInfo = stDotaItemRsp.dotaitembagquery_rsp();
					m_stBagInfo = stDotaInfo.baginfo();
					m_stPartInfo = stDotaInfo.partinfo();

					if(m_pRefitPanel != NULL && m_pRefitPanel->isOpen()) {
						m_pRefitPanel->updateUI();
					}
				} else if(stDotaItemRsp.has_dotaitemcompose_rsp()) { // 合成
					m_stBagInfo = stDotaItemRsp.dotaitemcompose_rsp();

					if(m_pRefitPanel != NULL && m_pRefitPanel->isOpen()) {
						m_pRefitPanel->updateUI();
					}

					if(m_pRefitMaterialPanel != NULL && m_pRefitMaterialPanel->isOpen()) {
						m_pRefitMaterialPanel->updateUI();
					}
				} else if(stDotaItemRsp.has_dotaitemmosaic_rsp()) { // 镶嵌
					const vmsg::CSDotaInfo &stDotaInfo = stDotaItemRsp.dotaitemmosaic_rsp();
					m_stBagInfo = stDotaInfo.baginfo();
					m_stPartInfo = stDotaInfo.partinfo();

					const DotaItemCfg *pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(m_uMosaicDotaItemID);

					if(pDotaItemCfg != NULL) {
						string attr1Name = "";
						string attr2Name = "";
						unsigned int attr1Val = 0;
						unsigned int attr2Val = 0;

						if(pDotaItemCfg->atk() > 0) {
							if(attr1Val == 0) {
								attr1Name = NounStr(NOUN_ATTR_DPS);
								attr1Val = pDotaItemCfg->atk();
							} else if(attr2Val == 0) {
								attr2Name = NounStr(NOUN_ATTR_DPS);
								attr2Val = pDotaItemCfg->atk();
							}
						}

						if(pDotaItemCfg->def() > 0) {
							if(attr1Val == 0) {
								attr1Name = NounStr(NOUN_DEFENSE);
								attr1Val = pDotaItemCfg->def();
							} else if(attr2Val == 0) {
								attr2Name = NounStr(NOUN_DEFENSE);
								attr2Val = pDotaItemCfg->def();
							}
						}

						if(pDotaItemCfg->hp() > 0) {
							if(attr1Val == 0) {
								attr1Name = NounStr(NOUN_HP);
								attr1Val = pDotaItemCfg->hp();
							} else if(attr2Val == 0) {
								attr2Name = NounStr(NOUN_HP);
								attr2Val = pDotaItemCfg->hp();
							}
						} 

						if(pDotaItemCfg->hit() > 0) {
							if(attr1Val == 0) {
								attr1Name = NounStr(NOUN_ATTR_HIT);
								attr1Val = floor(pDotaItemCfg->hit() * 2.3f + 0.5f);
							} else if(attr2Val == 0) {
								attr2Name = NounStr(NOUN_ATTR_HIT);
								attr2Val = floor(pDotaItemCfg->hit() * 2.3f + 0.5f);
							}
						}

						string tipStr = attr1Name + " +" + intToString(attr1Val);
						POP_TIP_LAYER_MASTER->ShowTipStr(tipStr.c_str());
						tipStr = attr2Name + " +" + intToString(attr2Val);
						POP_TIP_LAYER_MASTER->ShowTipStr(tipStr.c_str());
					}

					if(m_pRefitPanel != NULL && m_pRefitPanel->isOpen()) {
						m_pRefitPanel->updateUI();
					}
				} else if(stDotaItemRsp.has_dotaitemlvup_rsp()) { // 升级
					m_stPartInfo = stDotaItemRsp.dotaitemlvup_rsp();

					if(m_pRefitPanel != NULL && m_pRefitPanel->isOpen()) {
						m_pRefitPanel->refitDone();
					}
				}

				notifyRefit();
			}
			break;
		default:;
	}
}

void RefitControl::sendRefitQueryRqst() {
	m_stRefitProxy.sendRefitQueryRqst();
}

void RefitControl::sendRefitComposeRqst(unsigned int uDotaItemID) {
	m_stRefitProxy.sendRefitComposeRqst(uDotaItemID);
}

void RefitControl::sendRefitMosaicRqst(unsigned int uPart, unsigned int uDotaItemID) {
	m_uMosaicPart = uPart;
	m_uMosaicDotaItemID = uDotaItemID;
	m_stRefitProxy.sendRefitMosaicRqst(uPart, uDotaItemID);
}

void RefitControl::sendRefitLvUpRqst(unsigned int uPart) {
	m_stRefitProxy.sendRefitLvUpRqst(uPart);
}
