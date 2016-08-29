#include "FireArmsProxy.h"
#include "ClientSvrMsg.pb.h"
#include "QNetMgr.h"
#include "UserObj.h"
#include "weaponlib/WeaponLibControl.h"

CFireArmsProxy::CFireArmsProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WEAPON_BAG_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WEAPON_CHANGE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WEAPON_LVUP, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WEAPON_STARSTEP, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_WEAPON_REFIT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QUERY_INBATTLE_WEAPONS, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CHANGE_INBATTLE_WEAPONS, this);
}

CFireArmsProxy::~CFireArmsProxy(){

}


void CFireArmsProxy::fault(const vmsg::CSCommonErrMsg &err) {
    if(P_WEAPON_CTRL->getWeaponInfoPanel()->isOpen()) {
        P_WEAPON_CTRL->getWeaponInfoPanel()->doRspError();
    }
}

void CFireArmsProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
	if(stMsg.has_errmsg()){
        if(P_WEAPON_CTRL->getWeaponInfoPanel()->isOpen()) {
            P_WEAPON_CTRL->getWeaponInfoPanel()->doRspError();
        }
	}else if(stMsg.has_weaponrespond()) {
		vmsg::CSWeaponRespond& stWeaponRsp = *(stMsg.mutable_weaponrespond());

		if(stWeaponRsp.has_weaponquerybag_rsp()){
            P_WEAPON_CTRL->doQueryWeaponBagRsp(&stWeaponRsp.weaponquerybag_rsp());
		}else if(stWeaponRsp.has_weaponlvup_rsp()){
            P_WEAPON_CTRL->doWeaponLvUpRsp(&stWeaponRsp.weaponlvup_rsp());
		}else if(stWeaponRsp.has_weaponstarstep_rsp()){
            P_WEAPON_CTRL->doWeaponStarStepRsp(&stWeaponRsp.weaponstarstep_rsp());
		}else if(stWeaponRsp.has_weaponchange_rsp()){
            P_WEAPON_CTRL->doWeaponChangeRsp(&stWeaponRsp.weaponchange_rsp());
		}else if(stWeaponRsp.has_weaponrefit_rsp()) {
            P_WEAPON_CTRL->doWeaponRefitRsp(&stWeaponRsp.weaponrefit_rsp());
		}else if(stWeaponRsp.has_queryweaponinbattle_rsp()) {
			P_WEAPON_CTRL->doQueryWeaponInBattleRsp(stWeaponRsp.queryweaponinbattle_rsp());
		}else if(stWeaponRsp.has_changeweaponinbattle_rsp()) {
			P_WEAPON_CTRL->doChangeWeaponInBattleRsp(stWeaponRsp.changeweaponinbattle_rsp());
		}
	}
}

void CFireArmsProxy::sendWeaponBagRqst(){
	unsigned int uCmd = vmsg::CS_CMD_WEAPON_BAG_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendWeaponChangeRqst(unsigned int bagIdx, UNIT64_t weaponGUID){
	unsigned int uCmd = vmsg::CS_CMD_WEAPON_CHANGE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWeaponChangeRqst* stWeaponChangeRqst = stMsg.mutable_weaponrequest()->mutable_weaponchange_rqst();
	stWeaponChangeRqst->set_bagidx(bagIdx);
	stWeaponChangeRqst->set_weaponguid(weaponGUID);
    CCLOG("[trace]CFireArmsProxy::sendWeaponChangeRqst,weapon guid %llu", weaponGUID);
	P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendWeaponLvUpRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<vmsg::CSWeaponLvUpConsume> stConsume){
	unsigned int uCmd = vmsg::CS_CMD_WEAPON_LVUP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWeaponLvUpRqst* stWeaponLvUpRqst = stMsg.mutable_weaponrequest()->mutable_weaponlvup_rqst();
	stWeaponLvUpRqst->set_bagidx(bagIdx);
	stWeaponLvUpRqst->set_weaponguid(weaponGUID);

	for(unsigned int i = 0; i < stConsume.size(); i++){
		vmsg::CSWeaponLvUpConsume* pstConsume = stWeaponLvUpRqst->add_consumes();
		pstConsume->set_id(stConsume.at(i).id());
		pstConsume->set_type(stConsume.at(i).type());
		pstConsume->set_cnt(stConsume.at(i).cnt());
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendWeaponStarStepRqst(unsigned int bagIdx, UNIT64_t weaponGUID, vector<UNIT64_t> stNeedWeaponGUIDs){
	unsigned int uCmd = vmsg::CS_CMD_WEAPON_STARSTEP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWeaponStarStepRqst* stStarStepRqst = stMsg.mutable_weaponrequest()->mutable_weaponstarstep_rqst();
	stStarStepRqst->set_bagidx(bagIdx);
	stStarStepRqst->set_weaponguid(weaponGUID);

	for(unsigned int i = 0; i < stNeedWeaponGUIDs.size(); i++){
		stStarStepRqst->add_needweaponguids(stNeedWeaponGUIDs.at(i));
	}

	P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendWeaponRefitRqst(unsigned int bagIdx, UNIT64_t weaponGUID){
    unsigned int uCmd = vmsg::CS_CMD_WEAPON_REFIT;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSWeaponRefitRqst* pstRefitRqst = stMsg.mutable_weaponrequest()->mutable_weaponrefit_rqst();
    pstRefitRqst->set_bagidx(bagIdx);
    pstRefitRqst->set_weaponguid(weaponGUID);

    P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendQueryWeaponInBattleRqst(){
	unsigned int uCmd = vmsg::CS_CMD_QUERY_INBATTLE_WEAPONS;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CFireArmsProxy::sendChangeWeaponInBattleRqst(const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon, const vmsg::CSInbattleWeaponElmtBrief& stNewWeapon){
	unsigned int uCmd = vmsg::CS_CMD_CHANGE_INBATTLE_WEAPONS;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead& stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSWeaponInBattleRqst* stWeaponChangeRqst = stMsg.mutable_weaponrequest()->mutable_changeweaponinbattle_rqst();
	*stWeaponChangeRqst->mutable_oldweapon() = stOldWeapon;
	if(stNewWeapon.weaponguid() != 0){   //gui存在代表更换枪械，为0时表示卸下枪械
		*stWeaponChangeRqst->mutable_newweapon() = stNewWeapon;
	}

	P_NET_MGR->SendPkg(stMsg);
}

