#include "FightEndlessProxy.h"
#include "QNetMgr.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"
#include "SceneManager.h"
#include "FightEndlessControl.h"
#include "fight/LevelControl.h"
#include "game/warmap/WarMapControl.h"

using namespace vmsg;

CFightEndlessProxy::CFightEndlessProxy(){
	P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_START, this);
	P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_END, this);
	//P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_GENINFO, this);
	//P_NET_MGR->RegCmdListener(CS_CMD_INFINIT_BUYGEN, this);
}

CFightEndlessProxy::~CFightEndlessProxy(){

}

void CFightEndlessProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_infiniterespond()){
		CSInfiniteRespond* pstRsp = stMsg.mutable_infiniterespond();

		if(pstRsp->has_infinitegeninfo_rsp()){  //基因强化查询
			P_ENDLESS_CTRL->doGenInfoRsp(pstRsp->mutable_infinitegeninfo_rsp());
		}else if(pstRsp->has_buygen_rsp()){   //基因强化购买
			P_ENDLESS_CTRL->doBuyGenRsp(pstRsp->mutable_buygen_rsp());
		}else if(pstRsp->has_infiniteend_rsp()){  //战斗结束
			P_ENDLESS_CTRL->doInfiniteEnd(pstRsp->mutable_infiniteend_rsp());
		}else{
			
		}
	}

	if(stMsg.msg_head().msg_id() == CS_CMD_INFINIT_START){  //战斗开始回包
		if(P_ENDLESS_CTRL->getStrengthenPanel()->isOpen()){
			P_ENDLESS_CTRL->getStrengthenPanel()->close();
		}

		P_WAR_MAP_CTRL->getWarMapPanel()->clone();

		P_CITY_CTRL->getProxy()->sendPlayerLeaveRqst();
		P_ENDLESS_CTRL->updateUserInfoByGenLst();
		SceneManager::sharedSceneManager()->enterScene(20001);
	}
}

void CFightEndlessProxy::sendInfiniteGenInfoRqst(){
	unsigned int uCmd = CS_CMD_INFINIT_GENINFO;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CFightEndlessProxy::sendInfiniteBuyGenRqst(unsigned int idx){
	unsigned int uCmd = CS_CMD_INFINIT_BUYGEN;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	stMsg.mutable_infiniterequest()->mutable_buygen_rqst()->set_idx(idx);
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CFightEndlessProxy::sendEndlessStartRqst(){
	unsigned int uCmd = CS_CMD_INFINIT_START;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());

	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);
	P_NET_MGR->SendPkg(stMsg);
}

void CFightEndlessProxy::sendEndlessEndRqst(const UNIT64_t score, const unsigned int packageCnt, const unsigned int waveCnt, 
											unsigned int coin, CVArray<int, 4> stMonsterCnt, unsigned int evolutionCnt){
	unsigned int uCmd = CS_CMD_INFINIT_END;
	VCli2LogicSvrMsg stMsg;
	MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	CSInfiniteRequest* pstEndRqst = stMsg.mutable_infiniterequest();
	pstEndRqst->mutable_infiniteend_rqst()->set_score(score);
	pstEndRqst->mutable_infiniteend_rqst()->set_packagecnt(packageCnt);
	pstEndRqst->mutable_infiniteend_rqst()->set_wavecnt(waveCnt);
	pstEndRqst->mutable_infiniteend_rqst()->set_coin(coin);
	pstEndRqst->mutable_infiniteend_rqst()->set_evolutioncnt(evolutionCnt);

	for(int i = 0; i < stMonsterCnt.Size(); i++){
		pstEndRqst->mutable_infiniteend_rqst()->add_monstercnt(stMonsterCnt[i]);
	}

	P_NET_MGR->SendPkg(stMsg);
}