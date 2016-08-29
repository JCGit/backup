#include "VIPControl.h"

#include "game/UserObj.h"
#include "BuyCoinPanel.h"
#include "game/city/CityControl.h"
#include "game/manual/ManualControl.h"

VIPControl::VIPControl()
:m_pVIPPanel(NULL)
{
    m_pBuyCoinPanel = NULL;
}

VIPControl::~VIPControl()
{
	finalizePanel();
}

VIPPanel* VIPControl::getVIPPanel() {
	if(m_pVIPPanel == NULL) {
		m_pVIPPanel = VIPPanel::create();
		m_pVIPPanel->retain(); // release at VIPControl::~VIPControl()
	}

	return m_pVIPPanel;
}

BuyCoinPanel* VIPControl::getBuyCoinPanel() {
    if(m_pBuyCoinPanel == NULL) {
        m_pBuyCoinPanel = BuyCoinPanel::create();
        m_pBuyCoinPanel->retain(); // release at VIPControl::~VIPControl()
    }

    return m_pBuyCoinPanel;
}

void VIPControl::updateGold() {
	if(m_pVIPPanel != NULL && m_pVIPPanel->isOpen()) {
		m_pVIPPanel->updateUI();
	}
}

const vmsg::CSVIPInfos& VIPControl::getVIPInfos() {
	return m_stVIPInfos;
}

void VIPControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			if(stMsg.has_errmsg()) {

			}
			break;
		case vmsg::CS_CMD_PLAYER_QUERYVIPINFOS:
		case vmsg::CS_CMD_PLAYER_VIP_NOTIFY:
			if(stMsg.has_playerrespond()) {
				const vmsg::CSPlayerRespond &stPlayerRsp = stMsg.playerrespond();

				if(stPlayerRsp.has_queryvipinfo_rsp()) { // 查询VIP信息
					m_stVIPInfos = stPlayerRsp.queryvipinfo_rsp();

                    //todo
                    for(int i = 0; i < m_stVIPInfos.vipaward_size(); ++i) {
                        CCLOG("===idx=%d,stat=%d", i, m_stVIPInfos.vipaward(i));
                    }

					if(m_pVIPPanel != NULL && m_pVIPPanel->isOpen()) {
						m_pVIPPanel->updateUI();
					}
				} else if(stPlayerRsp.has_vip_notify()) { // VIP升级
					P_USER_OBJ->getMutableUserInfo()->vipLv = stPlayerRsp.vip_notify().viplv();
					P_USER_OBJ->getMutableUserInfo()->vipLv = stPlayerRsp.vip_notify().viplv();

					if(m_pVIPPanel != NULL && m_pVIPPanel->isOpen()) {
						m_pVIPPanel->updateUI();
					}

					P_CITY_CTRL->updateGold();
					P_MANUAL_CTRL->sendQueryBuyManualRqst(); //  购买体力次数可能增加
				}
			}
			break;
		case vmsg::CS_CMD_PLAYER_VIP_AWARD:
			sendQueryVIPInfoRqst();
            break;
        case vmsg::CS_CMD_PLAYER_QUERYBUYCOIN:
            *(m_stBuyCoinRsp.mutable_info()) = stMsg.playerrespond().querybuycoin_rsp();
            if(m_pBuyCoinPanel && m_pBuyCoinPanel->isOpen()) {
                m_pBuyCoinPanel->updateUI();
            }
            break;
        case vmsg::CS_CMD_PLAYER_BUYCOIN:
            m_stBuyCoinRsp = stMsg.playerrespond().buycoin_rsp();
            if(m_pBuyCoinPanel && m_pBuyCoinPanel->isOpen()) {
                m_pBuyCoinPanel->updateUI();
				m_pBuyCoinPanel->showMultiEff(m_stBuyCoinRsp.critmulti());
            }
            break;
		default:;
	}

}

void VIPControl::sendQueryVIPInfoRqst() {
	m_stVIPProxy.sendQueryVIPInfoRqst();
}

void VIPControl::sendGetVIPAwardRqst(unsigned int uVIPLv) {
	m_stVIPProxy.sendGetVIPAwardRqst(uVIPLv);
    CCLOG("[trace]get vip %d's award", uVIPLv);
}

void VIPControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pVIPPanel);
    CC_SAFE_RELEASE_NULL(m_pBuyCoinPanel);
}

