#include "AttrStrengthenControl.h"

#include "basic/PopTipLayer.h"
#include "game/login/LoginControl.h"
#include "UserObj.h"
#include "Macros.pb.h"

AttrStrengthenControl::AttrStrengthenControl()
:m_pAttrStrengthenInfoPanel(NULL)
,m_pAttrStrengthenPanel(NULL)
,m_pCoinTipPanel(NULL)
{
}

AttrStrengthenControl::~AttrStrengthenControl()
{
	CC_SAFE_RELEASE_NULL(m_pAttrStrengthenInfoPanel);
	CC_SAFE_RELEASE_NULL(m_pAttrStrengthenPanel);
	CC_SAFE_RELEASE_NULL(m_pCoinTipPanel);
}

const vmsg::CSAttrStrengthenInfo& AttrStrengthenControl::getAttrStrengthenInfo() {
	return m_stAttrStrengthenObj.getAttrStrengthenInfo();
}

void AttrStrengthenControl::openAttrStrengthenInfoPanel() {
	getAttrStrengthenInfoPanel()->open();
	sendQueryAttrStrengthenInfoRqst();
}

AttrStrengthenInfoPanel* AttrStrengthenControl::getAttrStrengthenInfoPanel() {
	if(m_pAttrStrengthenInfoPanel == NULL) {
		m_pAttrStrengthenInfoPanel = AttrStrengthenInfoPanel::create();
		m_pAttrStrengthenInfoPanel->retain();
	}

	return m_pAttrStrengthenInfoPanel;
}

AttrStrengthenPanel* AttrStrengthenControl::getAttrStrengthenPanel() {
	if(m_pAttrStrengthenPanel == NULL) {
		m_pAttrStrengthenPanel = AttrStrengthenPanel::create();
		m_pAttrStrengthenPanel->retain();
	}

	return m_pAttrStrengthenPanel;
}

CoinTipPanel* AttrStrengthenControl::getCoinTipPanel() {
	if(m_pCoinTipPanel == NULL) {
		m_pCoinTipPanel = CoinTipPanel::create();
		m_pCoinTipPanel->retain();
	}

	return m_pCoinTipPanel;
}

void AttrStrengthenControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	if(stMsg.has_errmsg()) {
		CCLOG("error code %d", stMsg.errmsg().errorcode());
	} else if(stMsg.has_playerrespond()) {
		const vmsg::CSPlayerRespond &stPlayerRsp = stMsg.playerrespond();

		if(stPlayerRsp.has_queryattrstengthen_rsp()) { // 查询属性强化
			m_stAttrStrengthenObj.setAttrStrengthenInfo(stPlayerRsp.queryattrstengthen_rsp());

			if(getAttrStrengthenInfoPanel()->isOpen()) {
				m_pAttrStrengthenInfoPanel->updateUI();
			}
		} else if(stPlayerRsp.has_attrstengthen_rsp()) { // 强化属性
			const vmsg::CSAttrStrengthenInfo &stAttrStrengthenInfo = m_stAttrStrengthenObj.getAttrStrengthenInfo();

			bool bDirty = false;

			for(int i = 0; i < stAttrStrengthenInfo.atkmagics_size(); i++) {
				if(stAttrStrengthenInfo.atkmagics(i) != stPlayerRsp.attrstengthen_rsp().atkmagics(i)) {
					bDirty = true;
					break;
				}
			}

			if(!bDirty) {
				for(int i = 0; i < stAttrStrengthenInfo.defmagics_size(); i++) {
					if(stAttrStrengthenInfo.defmagics(i) != stPlayerRsp.attrstengthen_rsp().defmagics(i)) {
						bDirty = true;
						break;
					}
				}
			}

			if(bDirty) { // 有变化，强化成功
				m_stAttrStrengthenObj.setAttrStrengthenInfo(stPlayerRsp.attrstengthen_rsp());

				POP_TIP_LAYER_MASTER->ShowTipStr("strengthen success");
				P_LOGIN_CTRL->getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);
				P_LOGIN_CTRL->getProxy()->sendQueryPlayerDetailInfoRqst();

				if(getAttrStrengthenInfoPanel()->isOpen()) {
					m_pAttrStrengthenInfoPanel->updateUI();
				}

				if(getAttrStrengthenPanel()->isOpen()) {
					m_pAttrStrengthenPanel->updateUI();
				}
			} else {
				POP_TIP_LAYER_MASTER->ShowTipStr("strengthen failure");
			}

		}
	}
}

void AttrStrengthenControl::sendQueryAttrStrengthenInfoRqst() {
	m_stAttrStrengthenProxy.sendQueryAttrStrengthenInfoRqst();
}

void AttrStrengthenControl::sendAttrStrengthenRqst(unsigned int uType, unsigned int uMagicType) {
	m_stAttrStrengthenProxy.sendAttrStrengthenRqst(uType, uMagicType);
}
