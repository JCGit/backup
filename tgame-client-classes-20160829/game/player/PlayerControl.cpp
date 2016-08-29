#include "PlayerControl.h"

#include "basic/PopTipLayer.h"

#include "game/UserObj.h"
#include "game/task/TaskControl.h"
#include "game/bag/BagControl.h"
#include "game/city/CityControl.h"

PlayerControl::PlayerControl()
{
}

PlayerControl::~PlayerControl()
{
}

void PlayerControl::onRecvMsg(vmsg::VLogicSvr2CliMsg &stMsg) {
	unsigned int uCmd = stMsg.msg_head().msg_id();

	switch(uCmd) {
		case vmsg::CS_CMD_ERR:
			if(stMsg.has_errmsg()) {
				const vmsg::CSCommonErrMsg &stErrMsg = stMsg.errmsg();
				unsigned int uRqstCmd = stErrMsg.rqstcmdid();

			}
			break;
		case vmsg::CS_CMD_PLAYER_GETMONTHCARDAWARD:
		case vmsg::CS_CMD_PLAYER_PUSHMONTHCARD:
			if(stMsg.has_playerrespond()) {
				const vmsg::CSPlayerRespond &stPlayerRsp = stMsg.playerrespond();

				if(stPlayerRsp.has_getmonthcardaward_rsp()) {
					P_USER_OBJ->updateMonthCardInfo(stPlayerRsp.getmonthcardaward_rsp());
					POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_GOLD, 0, stPlayerRsp.getmonthcardaward_rsp().gold());
					P_CITY_CTRL->getGetAwardPanel()->claimed();
				} else if(stPlayerRsp.has_pushmonthcard_notify()) {
					P_USER_OBJ->updateMonthCardInfo(stPlayerRsp.pushmonthcard_notify());
					P_CITY_CTRL->getGetAwardPanel()->claimed();
				}

			}
			break;
		case vmsg::CS_CMD_PLAYER_VIP_DAILY_AWARD:
			if(stMsg.has_dailygiftrespond()) {
				const vmsg::CSDailyGiftRespond &stDailyGiftRsp = stMsg.dailygiftrespond();

				if(stDailyGiftRsp.has_claimvipdailyaward_rsp()) {
					const vmsg::CSClaimVipDailyAwardRsp &stVIPDailyAwardRsp = stDailyGiftRsp.claimvipdailyaward_rsp();
					P_BAG_CTRL->setBagInfo(stVIPDailyAwardRsp.baginfo());
					P_USER_OBJ->getMutableUserInfo()->isVIPDailyAwardClaimed = false;
					P_CITY_CTRL->getGetAwardPanel()->claimed();

					for(int i = 0; i < stVIPDailyAwardRsp.awardlst_size(); i++) {
						const vmsg::CSAwardElement &stAwardElmt = stVIPDailyAwardRsp.awardlst(i);

					}
				}
			}
			break;
		default:;
	}
}

void PlayerControl::sendGetMonthCardAwardRqst() {
	m_stPlayerProxy.sendGetMonthCardAwardRqst();
}

void PlayerControl::sendGetVIPDailyAwardRqst() {
	m_stPlayerProxy.sendGetVIPDailyAwardRqst();
}
