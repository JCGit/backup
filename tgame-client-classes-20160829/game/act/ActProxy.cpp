#include "ActProxy.h"
#include "QNetMgr.h"
#include "ActControl.h"

using namespace vmsg;

CActProxy::CActProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_COUPON, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CLAIM_FIRST_CHARGE, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QUERY_ALL_PKG_STATUS, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_CLAIM_STORED_MANUAL, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_CLAIM_SPRING_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_SUM_CHARGE_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILY_LOGIN_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GRADE_GEEK_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_FIGHT_GEEK_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILY_CHARGE_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_VIP_GIFT_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_DAILY_CONSUME_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_SUM_CONSUME_PKG, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_ONLINE_PKG,this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_NEXTDAY_PKG,this);
}

CActProxy::~CActProxy(){

}

void CActProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stMsg) {
	if(stMsg.msg_head().msg_id() == vmsg::CS_CMD_PLAYER_CLAIM_STORED_MANUAL) {
		sendQueryAllPkgStatusRqst();
	}
	
	if(stMsg.has_errmsg()){
		vmsg::CSCommonErrMsg stErrMsg = stMsg.errmsg();

		
	}else if(stMsg.has_couponrespond()) {
		CSCouponRespond* pstRsp = stMsg.mutable_couponrespond();

		if(pstRsp->has_claim_response()){   //cdk兑换
			P_ACT_CTRL->doClaimCouponRsp(pstRsp->claim_response());
		} else if(pstRsp->has_claim_first_charge_response()){  //首冲礼包领取
			P_ACT_CTRL->doClaimFirstChargePkgRsp(pstRsp->claim_first_charge_response());
		} else if(pstRsp->has_all_pkg_status()){   //查询所有礼包返回
			P_ACT_CTRL->doQueryAllPkgStatusRsp(pstRsp->all_pkg_status());
		} else if(pstRsp->has_claimspringpkg_rsp()) {
			P_ACT_CTRL->doClaimSpringPkgRsp(pstRsp->claimspringpkg_rsp());
		} else if (pstRsp->has_sumcharge_pkg_rsp()){		// 累计充值
			P_ACT_CTRL->doSumChargePkgRsp(pstRsp->sumcharge_pkg_rsp());
		} else if (pstRsp->has_dailycharge_pkg_rsp()){		// 每日充值
			P_ACT_CTRL->doDailyChargePkgRsp(pstRsp->dailycharge_pkg_rsp());
		} else if (pstRsp->has_dailylogin_pkg_rsp()){		// 累计登陆
			P_ACT_CTRL->doSumLoginPkgRsp(pstRsp->dailylogin_pkg_rsp());
		} else if (pstRsp->has_gradegeek_pkg_rsp()){		// 等级达人
			P_ACT_CTRL->doGradeGeekPkgRsp(pstRsp->gradegeek_pkg_rsp());
		} else if (pstRsp->has_fightgeek_pkg_rsp()){		// 战力达人
			P_ACT_CTRL->doFightGeekPkgRsp(pstRsp->fightgeek_pkg_rsp());
		} else if(pstRsp->has_vipgiftpkgrsp()){
			P_ACT_CTRL->doVipGiftPkgRsp(pstRsp->vipgiftpkgrsp());
		} else if(pstRsp->has_dailyconsumepkgrsp()){
			P_ACT_CTRL->doDailyConsumePkgRsp(pstRsp->dailyconsumepkgrsp());
		} else if(pstRsp->has_sumconsumepkgrsp()){
			P_ACT_CTRL->doSumConsumePkgRsp(pstRsp->sumconsumepkgrsp());
		} else if (pstRsp->has_onlinepkgrsp())
		{
			P_ACT_CTRL->doOnlinePkgRsp(pstRsp->onlinepkgrsp());
		}else if (pstRsp->has_nextdaypkgrsp())
		{
			P_ACT_CTRL->doNextDayPkgRsp(pstRsp->nextdaypkgrsp());
		}
	}
}

void CActProxy::sendClaimCouponRqst(const string couponID){
	unsigned int uCmd = vmsg::CS_CMD_COUPON;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	stMsg.mutable_couponrequest()->mutable_claim_request()->set_couponid(couponID);
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendClaimFirstChargePkgRqst(){
	unsigned int uCmd = vmsg::CS_CMD_CLAIM_FIRST_CHARGE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendQueryAllPkgStatusRqst(){
	unsigned int uCmd = vmsg::CS_CMD_QUERY_ALL_PKG_STATUS;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendClaimStoredManualRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_CLAIM_STORED_MANUAL;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendClaimSpringPkgRqst(unsigned int uDay) {
	unsigned int uCmd = vmsg::CS_CMD_CLAIM_SPRING_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::CSClaimSpringPkgRqst *pClaimSpringPkgRqst = pCouponRqst->mutable_claim_spring_pkg_rqst();
	pClaimSpringPkgRqst->set_day(uDay);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendSumChargeRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_SUM_CHARGE_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::SumChargePkgRqst *pSumChargePkgRqst = pCouponRqst->mutable_sumcharge_pkg_rqst();
	pSumChargePkgRqst->set_level(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendDailyChargeRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_DAILY_CHARGE_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::DailyChargePkgRqst *pDailyChargePkgRqst = pCouponRqst->mutable_dailycharge_pkg_rqst();
	pDailyChargePkgRqst->set_level(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendGradeGeekRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_GRADE_GEEK_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::GradeGeekPkgRqst *pGradeGeekPkgRqst = pCouponRqst->mutable_gradegeek_pkg_rqst();
	pGradeGeekPkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendFightGeekRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_FIGHT_GEEK_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::FightGeekPkgRqst *pFightGeekPkgRqst = pCouponRqst->mutable_fightgeek_pkg_rqst();
	pFightGeekPkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendDailyLoginRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_DAILY_LOGIN_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::DailyLoginPkgRqst *pDailyLoginPkgRqst = pCouponRqst->mutable_dailylogin_pkg_rqst();
	pDailyLoginPkgRqst->set_day(_lvl);
	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendVipGiftRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_VIP_GIFT_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::VipGiftPkgRqst *pVipGiftPkgRqst= pCouponRqst->mutable_vipgiftpkgrqst();
	pVipGiftPkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendDailyConsumeRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_DAILY_CONSUME_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::DailyConsumePkgRqst *pDailyConsumePkgRqst= pCouponRqst->mutable_dailyconsumepkgrqst();
	pDailyConsumePkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendSumConsumeRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_SUM_CONSUME_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::SumConsumePkgRqst *pSumConsumePkgRqst= pCouponRqst->mutable_sumconsumepkgrqst();
	pSumConsumePkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendOnlinePkgRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_ONLINE_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::OnlinePkgRqst *pOnlinePkgRqst= pCouponRqst->mutable_onlinepkgrqst();
	pOnlinePkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}

void CActProxy::sendNextDayPkgRqst(int _lvl)
{
	unsigned int uCmd = vmsg::CS_CMD_NEXTDAY_PKG;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSCouponRequest *pCouponRqst = stMsg.mutable_couponrequest();
	vmsg::NextDayPkgRqst *pNextDayPkgRqst= pCouponRqst->mutable_nextdaypkgrqst();
	pNextDayPkgRqst->set_awardlevel(_lvl);

	P_NET_MGR->SendPkg(stMsg);
}