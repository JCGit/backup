#include "QianghunProxy.h"
#include "QNetMgr.h"
#include "QianghunControl.h"

using namespace vmsg;

CQianghunProxy::CQianghunProxy(){
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QIANGHUN_QUERY, this);	
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QIANGHUN_QUERY_LAYER, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QIANGHUN_FLOP, this);
}

CQianghunProxy::~CQianghunProxy(){

}

void CQianghunProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg){
	if(stMsg.has_errmsg()){

	}else if(stMsg.has_qianghunrespond()) {
		CSQiangHunRespond& stRsp = *(stMsg.mutable_qianghunrespond());

		if(stRsp.has_queryqianghun_rsp()){		//²éÑ¯
			P_QIANGHUN_CTRL->doQianghunInfoRsp(stRsp.queryqianghun_rsp());
		}else if(stRsp.has_querylayer_rsp()){	//²éÑ¯Ä³Ò»²ã
			P_QIANGHUN_CTRL->doQianghunInfoRsp(stRsp.querylayer_rsp());
		}else if(stRsp.has_flop_rsp()){		//·­ÅÆ
			P_QIANGHUN_CTRL->doFlopRsp(stRsp.flop_rsp());
		}
	}
}

void CQianghunProxy::sendQiangHunQueryRqst(){
	unsigned int uCmd = vmsg::CS_CMD_QIANGHUN_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void CQianghunProxy::sendQiangHunLayerQueryRqst(unsigned int layer){
	unsigned int uCmd = vmsg::CS_CMD_QIANGHUN_QUERY_LAYER;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSQiangHunLayerQueryRqst* pstRqst = stMsg.mutable_qianghunrequest()->mutable_querylayer_rqst();
	pstRqst->set_layer(layer);

	P_NET_MGR->SendPkg(stMsg);
}

void CQianghunProxy::sendQiangHunFlopRqst(unsigned int pos, unsigned int layer){
	unsigned int uCmd = vmsg::CS_CMD_QIANGHUN_FLOP;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSQiangHunFlopRqst* pstRqst = stMsg.mutable_qianghunrequest()->mutable_flop_rqst();
	pstRqst->set_pos(pos);
	pstRqst->set_layer(layer);

	P_NET_MGR->SendPkg(stMsg);
}