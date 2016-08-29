#include "UserGoldProxy.h"

#include "UserGoldControl.h"
#include "ClientSvrMsg.pb.h"
#include "CSGoldMsg.pb.h"
#include "UserObj.h"
#include "QNetMgr.h"
#include "cocos-ext.h"
#include "JsonHelper.h"
#include "ChannelInf.h"
#include "CocoStudio/Json/rapidjson/writer.h"
#include "CocoStudio/Json/rapidjson/stringbuffer.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"

using namespace vmsg;
using namespace rapidjson;

UserGoldProxy::UserGoldProxy() {
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_BUY, this);	
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_RMBSTORE_BUY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_RMBSTORE_QUERY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_APP_AUTHED_BUY, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_APP_AUTHED_BUY_SUCCEED_REPORT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_QUERY_SINGLE_PRODUCT, this);
}

void UserGoldProxy::buyGold() {
    /*char temp[1024];
    const std::string & channelName = ChannelInf::GetChannelName();
    snprintf(temp, 128, "%lu", P_USER_OBJ->getUserInfo()->uin);
    std::string appUid(temp);
    unsigned int uiZoneID = P_USER_OBJ->GetMyZoneID();
    snprintf(temp, 128, "%u", uiZoneID);
    std::string zoneID (temp);

    rapidjson::MemoryPoolAllocator<> allocator (temp, sizeof(temp));
    Document root;
    root.SetObject();
    root.AddMember("channel", channelName.c_str(), allocator);
    root.AddMember("uid", P_USER_OBJ->getUserInfo()->openID.c_str(), allocator);
    root.AddMember("appUid", appUid.c_str(), allocator);
    root.AddMember("serverId", zoneID.c_str(), allocator);
    root.AddMember("currencyCount", 0, allocator);
    root.AddMember("realPayMoney", 0, allocator);
    StringBuffer strbuf;
    Writer<StringBuffer> writer(strbuf);
    root.Accept(writer);
    sendBuyMessage(strbuf.GetString());*/
    return;
}

void UserGoldProxy::sendBuyMessage(const char * msg) {
    vmsg::VCli2LogicSvrMsg stMsg;
    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, vmsg::CS_CMD_GOLD_BUY);
    CSBuyGoldRqst * rqst = stMsg.mutable_goldrequest()->mutable_buygold_rqst();
    rqst->set_str(msg);
    P_NET_MGR->SendPkg(stMsg);
}

void UserGoldProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
    unsigned int uiCmd = (unsigned int)stMsg.msg_head().msg_id();
    switch (uiCmd) {
    case vmsg::CS_CMD_GOLD_BUY:
        onGoldBuyRsp(stMsg);
        break;
    case vmsg::CS_CMD_GOLD_RMBSTORE_BUY:
        //
		if(stMsg.goldrespond().buyrmbstore_rsp().has_baginfo()){
			P_USER_GOLD_CTRL->onBagInfoRsp(stMsg.goldrespond().buyrmbstore_rsp().baginfo());
		}

        P_USER_GOLD_CTRL->onBuyRMBStoreRsp(stMsg.goldrespond().buyrmbstore_rsp().str());
        break;
    case vmsg::CS_CMD_GOLD_RMBSTORE_QUERY:
        //
        P_SHOP_CTRL->getRmbShopPanel()->updateUI(stMsg.goldrespond().queryrmbstore_rsp());
        break;
    case vmsg::CS_CMD_APP_AUTHED_BUY:
        //app store,get order id
        {
            AppAuthedBuyInfo stInfo;
            stInfo.strProductID = stMsg.goldrespond().appauthedbuy_rsp().productid();
            stInfo.strOrderID = stMsg.goldrespond().appauthedbuy_rsp().orderid();
            stInfo.iProductCount = stMsg.goldrespond().appauthedbuy_rsp().count();
            stInfo.iTotalMoneyAmount = stMsg.goldrespond().appauthedbuy_rsp().totalmoneyamount();

            P_USER_GOLD_CTRL->onAppAuthedBuyRsp(stInfo);
        }
        break;
    case vmsg::CS_CMD_APP_AUTHED_BUY_SUCCEED_REPORT:
        //
        break;
	case vmsg::CS_CMD_GOLD_QUERY_SINGLE_PRODUCT:
		P_USER_GOLD_CTRL->onQueryRMBStoreItemRsp(stMsg.goldrespond().querysingleitem_rsp());
		break;
    default:
        CCLog("wrong distribution %d", uiCmd);
    }
}

void UserGoldProxy::onGoldBuyRsp(vmsg::VLogicSvr2CliMsg &stMsg) {
   /* if (!stMsg.has_goldrespond() || !stMsg.goldrespond().has_buygold_rsp()) {
        // TODO: error handling here
        return;
    }
    const std::string & payRsp = stMsg.goldrespond().buygold_rsp().str();
    CCLog("recv %s", payRsp.c_str());
    rapidjson::Document reply;
    reply.Parse<0>(payRsp.c_str());
    std::string orderId;
    int rate = 0;

    try {
        int code = GetJsonValue<int>(reply, "code");
        if (code != 0) {
            // TODO: error handling here
            return;
        }
        orderId = 
            GetJsonValue<const char*>(reply, "orderId");
        rate = GetJsonValue<int>(reply, "rate");
    } catch (JsonException & e) {
        CCLOG("Fail to parse login rsp json %s", e.mErrMsg.c_str());
        return;
    }

    char temp[128];
    const std::string & channelName = ChannelInf::GetChannelName();
    snprintf(temp, 128, "%lu", P_USER_OBJ->getUserInfo()->uin);
    std::string appUid(temp);
    unsigned int uiZoneID = P_USER_OBJ->GetMyZoneID();
    snprintf(temp, 128, "%u", uiZoneID);
    std::string zoneID(temp);
    int ret = ChannelInf::Pay(orderId, P_USER_OBJ->getUserInfo()->openID, 
      P_USER_OBJ->getUserInfo()->token, appUid, 
      P_USER_OBJ->getUserInfo()->nick, zoneID, rate);
    if (ret != 0) {
        // error handling here
    }*/
}


void UserGoldProxy::sendBuyRMBStoreRqst(const std::string& strJson) {
    unsigned int uCmd = vmsg::CS_CMD_GOLD_RMBSTORE_BUY;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    stMsg.mutable_goldrequest()->mutable_buyrmbstore_rqst()->set_str(strJson);

    P_NET_MGR->SendPkg(stMsg);
}

void UserGoldProxy::sendQueryRMBStoreRqst() {
    vmsg::VCli2LogicSvrMsg stMsg;
    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, vmsg::CS_CMD_GOLD_RMBSTORE_QUERY);

    P_NET_MGR->SendPkg(stMsg);
}



void UserGoldProxy::sendAppAuthedBuyRqst(const std::string& strProductID, const int iNum) 
{
    unsigned int uCmd = vmsg::CS_CMD_APP_AUTHED_BUY;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    stMsg.mutable_goldrequest()->mutable_appauthedbuy_rqst()->set_productid(strProductID);
    stMsg.mutable_goldrequest()->mutable_appauthedbuy_rqst()->set_count(iNum);

    P_NET_MGR->SendPkg(stMsg);
}

void UserGoldProxy::sendAppAuthedBuySucceedRqst(const std::string& strProductID, const int iTotalMoney,
                                 const std::string& strOrderID, const int iNum) 
{
    unsigned int uCmd = vmsg::CS_CMD_APP_AUTHED_BUY_SUCCEED_REPORT;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    stMsg.mutable_goldrequest()->mutable_appauthedbuysucceed_rqst()->set_productid(strProductID);
    stMsg.mutable_goldrequest()->mutable_appauthedbuysucceed_rqst()->set_totalmoneyamount(iTotalMoney);
    stMsg.mutable_goldrequest()->mutable_appauthedbuysucceed_rqst()->set_orderid(strOrderID);
    stMsg.mutable_goldrequest()->mutable_appauthedbuysucceed_rqst()->set_count(iNum);

    P_NET_MGR->SendPkg(stMsg);
}

void UserGoldProxy::sendQueryRMBStoreItemRqst(const int productID) {
	unsigned int uCmd = vmsg::CS_CMD_GOLD_QUERY_SINGLE_PRODUCT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	stMsg.mutable_goldrequest()->mutable_querysingleitem_rqst()->set_productid(productID);

	P_NET_MGR->SendPkg(stMsg);
}


