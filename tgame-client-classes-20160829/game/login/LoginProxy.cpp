#include "LoginProxy.h"

#include "ClientSvrMsg.pb.h"
#include "PlayerLoginMsg.pb.h"
#include "CSPlayerMsg.pb.h"
#include "AppID.pb.h"
#include "QNetMgr.h"
#include "basic/SceneManager.h"
#include "UserObj.h"
#include "QEnvSet.h"
#include "JsonHelper.h"

#include "LoginControl.h"
#include "ChooseRolePanel.h"
#include "CreateRolePanel.h"
#include "LoginObj.h"
#include "Global.h"
#include "HeartBeat.h"
#include "HttpProxy.h"
#include "WarMapCfgMaster.h"
#include "fight/FightControl.h"
#include "UserObj.h"
#include "game/task/TaskControl.h"
#include "game/task/TaskPanel.h"
#include "game/task/TaskProxy.h"
#include "game/city/CityControl.h"
#include "fightguide/FightGuideControl.h"
#include "game/equiplib/EquipLibControl.h"

#include "game/guild/GuildControl.h"

#include "game/bag/BagControl.h"

#include "game/guide/GuideControl.h"
#include "game/vip/VIPControl.h"
#include "signin/SignInControl.h"
#include "GameInf.h"
#include "set/SetSysControl.h"
#include "evtlog/EvtLog.h"
#include "UserGoldControl.h"
#include "basic/PopTipLayer.h"
#include "CSPushMsg.pb.h"
#include "ChannelInf.h"
#include "basic/CommonDialog.h"
#include "game/city/AutoPopControl.h"

#ifdef TGAME_IN_APP_SDK
#include "FaceBookSDK.h"
#endif


LoginProxy::LoginProxy()
{
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_REG_PLAYERINFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_LOGIN, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QUERY_PLAYERINFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_QUERY_ROLE_LIST, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_ROLE_LOGIN, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_ROLE_LOGOUT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_HEART_BEAT, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_QUERY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_GOLD_NOTIFY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_COIN_NOTIFY, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_LV_UP, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERY_DETAILINFO, this);
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUERYOTHERPLAYER, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_FIGHTPOWERNOTIFY, this);			//ս���仯
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_QUICKLOGIN, this);	 // ���ٵ�½
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_DAILYREG, this); //ÿ�յ�¼
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_PLAYER_SETSETTING, this);	//�������

}

LoginProxy::~LoginProxy()
{
}

int LoginProxy::OnLogin(const vmsg::CSPlayerLoginRsp & stRsp) {
    const std::string & loginInfo = stRsp.logininfo();
    CCLog("[trace]LoginProxy::OnLogin, recv login info:%s,len %d",loginInfo.c_str(), loginInfo.size());

    rapidjson::Document reply;
    reply.Parse<0>(loginInfo.c_str());
    if (reply.HasParseError()) {
		//anysdk������£�������logininfo���أ�������������˳��ģ���������
        CCLog("[error]LoginProxy::OnLogin, parse json error: %s,json=%s",reply.GetParseError(), loginInfo.c_str());
        return 0;
    }

    //chameleon sdk��Ҫ����ɫ�����ݣ��������ﴦ��
    try {
        int code = GetJsonValue<int>(reply, "code");
        if (code != 0) {
            return code;
        }
        rapidjson::Value &jsonLoginInfo = 
            GetJsonValue<rapidjson::Value&>(reply, "loginInfo");
        if (jsonLoginInfo.IsNull()) {
            CCLOG("missing login info");
            return -1;
        }
        P_USER_OBJ->getMutableUserInfo()->openID = 
            GetJsonValue<const char*>(jsonLoginInfo, "uid");
        P_USER_OBJ->getMutableUserInfo()->token = 
            GetJsonValue<const char*>(jsonLoginInfo, "token");

		const DirSvrInfo &stDirSvrInfo = P_LOGIN_OBJ->getDirSvrInfo();

		// ������������������֤������
		if(stDirSvrInfo.isstop) {
			const string &whitelist = stDirSvrInfo.whitelist;
			string::size_type startPos = 0;
			string openID = P_USER_OBJ->getMutableUserInfo()->openID;
			startPos = whitelist.find(openID);

			if(startPos == string::npos
			|| !(startPos + openID.size() == whitelist.size() || whitelist.substr(startPos + openID.size(), 1) == ",")
			|| !(startPos == 0 || whitelist.substr(startPos - 1, 1) == ",")) {
				COMMON_DIALOG->show(stDirSvrInfo.notification);
				return 0;
			}
		}

        ChannelInf::onLoginRsp(loginInfo);
        ChannelInf::showToolbar(true);

        sendQueryRoleListRqst(); //���Բ�ѯ��ɫ�б���
        return 0;
    } catch (JsonException & e) {
        CCLOG("Fail to parse login rsp json %s", e.mErrMsg.c_str());
        return -1;
    }

    sendQueryRoleListRqst(); //���Բ�ѯ��ɫ�б���

    return 0;
#ifdef TGAME_USE_CYOU
    //CS_CMD_PLAYER_LOGIN �Ļذ�
    //cyouƽ̨��{"tag":654321,"opcode":10001,"state":200,"data":{"status":"1","oid":"658547114","userid":"684001658547114","type":"c"},"channelId":4001,"error":""}
    
    //cyou
    int iRetStatCode = 0;
    if(reply.HasMember("state") && reply["state"].IsInt() ) {
        iRetStatCode = reply["state"].GetInt();
    }
    CCLOG("[trace]LoginProxy::OnLogin, iRetStatCode %d", iRetStatCode );
    if(200 != iRetStatCode) {
        return iRetStatCode;
    }

    if(reply.HasMember("data") ) {
        const rapidjson::Value& stDataJson = reply["data"];
        //userid ��data��
        //oid
        string str;
        if(stDataJson.HasMember("oid") && stDataJson["oid"].IsString() ) {
            str = stDataJson["oid"].GetString();
        }
        CCLOG("[trace]LoginProxy::OnLogin, oid [%s]", str.c_str() );
        if(str.length() <= 0) {
            return -2;
        }
        P_USER_OBJ->getMutableUserInfo()->cyouOID = str;

        //userid
        str.clear();
        if(stDataJson.HasMember("userid") && stDataJson["userid"].IsString() ) {
            str = stDataJson["userid"].GetString();
        }
        CCLOG("[trace]LoginProxy::OnLogin, openid(userid) [%s]", str.c_str() );
        if(str.length() <= 0) {
            return -3;
        }
        P_USER_OBJ->getMutableUserInfo()->openID = str;
    } else {
        CCLOG("[error]LoginProxy::OnLogin, invalid json data");
        return -4;
    }
    sendQueryRoleListRqst(); //���Բ�ѯ��ɫ�б���
    return 0; //OK
#endif
   
}

void LoginProxy::DoRsp(vmsg::VLogicSvr2CliMsg& stMsg) {
    
	if(stMsg.msg_head().has_loginrsp()) {
        //������֤����
        do {
#ifdef TGAME_IN_APP_SDK
        int iRet = OnLogin(stMsg.msg_head().loginrsp());
        if (iRet != 0)  {
            CCLOG("[trace]LoginProxy::DoRsp, login fail, OnLogin ret %d", iRet);
            return;
        }
        break;
#endif
        P_EVT_LOG->logTDAppLogin(P_USER_OBJ->getMutableUserInfo()->openID);
        sendQueryRoleListRqst(); //����֤��ֱ�ӵ�½
        } while(0);
	}

	if(stMsg.has_errmsg()) {
		vmsg::CSCommonErrMsg stErrMsg = stMsg.errmsg();

        if(stErrMsg.rqstcmdid() == vmsg::CS_CMD_PLAYER_LOGIN && stErrMsg.errorcode() == 100005) { // login failure
            P_LOGIN_CTRL->getLoginScreenView()->closeLoading();
            P_LOGIN_OBJ->getRoleInfoList().clear(); //û��ע�ᣬ��ո��б�
			P_LOGIN_CTRL->getCreateRolePanel()->open();
		}
	} else if(stMsg.has_heartbeat_rsp()) { // ����
		P_GLOBAL->setSvrTime(stMsg.heartbeat_rsp().svr_time());
		CCLOG("heart beat svr_time %d", P_GLOBAL->getSvrTime());
	} else if(stMsg.has_playerrespond()) {
        bool bIsBeforePlayInfoInited = (P_USER_OBJ->getUserInfo()->newestWarMapID != 0 );

		vmsg::CSPlayerRespond playerRsp = stMsg.playerrespond();
		UIN_t uin = stMsg.msg_head().uid();
        
		if(playerRsp.has_rolelst_rsp()) { // ��ҵĽ�ɫ�б�
			P_LOGIN_OBJ->getRoleInfoList().clear();

			for(int i = 0; i < playerRsp.rolelst_rsp().rolelst_size(); i++) {
				P_LOGIN_OBJ->getRoleInfoList().push_back(playerRsp.rolelst_rsp().rolelst(i));
			}

			if(P_LOGIN_OBJ->getRoleInfoList().size() > 0) {
				P_LOGIN_CTRL->getChooseRolePanel()->open();
				P_LOGIN_CTRL->getChooseRolePanel()->updateUI();
			} else {
				P_LOGIN_CTRL->getCreateRolePanel()->open();
                //���ڻ�û�н�ɫ
                P_EVT_LOG->logTDAppReg(P_USER_OBJ->getMutableUserInfo()->openID);
			}
            P_LOGIN_CTRL->getLoginScreenView()->closeLoading();

		} else if(playerRsp.has_regplayerinfo_rsp()) { // ע��һ����ɫ
            // ע��ɹ��Զ���¼
            ADD_EVT_LOG(EVT_LOG_CREATE_ROLE_DONE); //��Ϸ�¼���־
			P_USER_OBJ->updateUserInfo(playerRsp.regplayerinfo_rsp().playerinfo());
			CCLOG("login success uin=>%lu", uin);

			P_USER_OBJ->setUin(uin);
			P_LOGIN_CTRL->getProxy()->sendHeartBeatRqst(); // �ȷ�һ�Σ�ͬ��������ʱ��
			P_LOGIN_CTRL->onLoginGetData(); //��ȡ��Ҫ������
			P_USER_OBJ->setLoginPrepare(true);
			P_GUIDE_CTRL->setNewcomer(true); // ��ע�����

            P_EVT_LOG->logTDGameAddRole(playerRsp.regplayerinfo_rsp().playerinfo().nick());
            P_EVT_LOG->doCyouMBILogLogin(); //cyou mbi log
		} else if(playerRsp.has_rolelogin_rsp()) { // ��ɫ����
			CCLOG("[trace]role login uin=>%lu", uin);
			P_USER_OBJ->setUin(uin);
			P_LOGIN_CTRL->getProxy()->sendHeartBeatRqst(); // �ȷ�һ�Σ�ͬ��������ʱ��
            P_LOGIN_CTRL->onLoginGetData(); //��ȡ��Ҫ������

			CCLOG("[trace]login success uin=>%lu", uin);

			P_USER_OBJ->setLoginPrepare(true);

            P_EVT_LOG->doCyouMBILogLogin(); //cyou mbi log
		} else if(playerRsp.has_rolelogout_rsp()) { // ��ɫ�ǳ�
			CCLOG("[trace]role logout uin=>%lu", uin);
		} else if(playerRsp.has_queryplayerinfo_rsp()) { // ��ѯ�����Ϣ
			P_USER_OBJ->updateUserInfo(playerRsp.queryplayerinfo_rsp().playerinfo());

			if(P_USER_OBJ->getLoginPrepare()){
				CCLOG("the weapon cfg id == %d", P_USER_OBJ->getUserInfo()->weaponCfgID);

				P_USER_OBJ->setLoginPrepare(false);
				if(P_GUIDE_CTRL->isNewcomer() && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) ){
					P_FIGHT_GUIDE_CTRL->enterFightGuideScene();
				}else{
					SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
				}
                P_USER_GOLD_CTRL->doLoginSubmitPlayerinfo(); //chameleon sdk �ϱ�
			}
            P_SIGN_IN_CTRL->updateSignInRsp(false);

		} else if(playerRsp.has_playercoin_notify()) { // ͭǮ�仯
			UINT64_t uCoinDiffVal = playerRsp.playercoin_notify() - P_USER_OBJ->getUserInfo()->coin;
            P_USER_OBJ->updateCoin(playerRsp.playercoin_notify());
            P_CITY_CTRL->updateMoneyTopBar();
			P_CITY_CTRL->updateCoin();
			POP_TIP_LAYER_MASTER->ShowAwardTip(vmsg::THING_TYPE_COIN, 0, uCoinDiffVal);
		} else if(playerRsp.has_querydetailinfo_rsp()) {
			P_USER_OBJ->updateUserDetailInfo(playerRsp.querydetailinfo_rsp());
			P_EQUIP_LIB_CTRL->notifyEquipLvUp();
            /*
            //2014-8-16,del,bug
			if(P_BAG_CTRL->getBagPanel()->isOpen()) {
				P_BAG_CTRL->getBagPanel()->updateUI();
			}*/
            P_BAG_CTRL->updateBag();
		} else if(playerRsp.has_playerlvup_rsp()) { // ����仯
            const unsigned int iPreLv = P_USER_OBJ->getUserInfo()->level;
            P_USER_OBJ->updateLvExp(playerRsp.playerlvup_rsp().lv(), playerRsp.playerlvup_rsp().exp());
            if(playerRsp.playerlvup_rsp().lv() > iPreLv) {
                P_CITY_CTRL->showLvUpEffect();

				P_CITY_CTRL->getLevelUpPanel()->levelup(iPreLv, playerRsp.playerlvup_rsp().lv());
				BaseScene *pRunningScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());

				if(pRunningScene != NULL && pRunningScene->getSceneType() == SCENE_OSD) { // ����ȵ����볡�����
					P_CITY_CTRL->getLevelUpPanel()->open();
				} else {
					P_AUTO_POP_CTRL->addPopPanel(P_CITY_CTRL->getLevelUpPanel());
				}

                P_EVT_LOG->logByLv(EVT_LOG_LV_UP, intToString(playerRsp.playerlvup_rsp().lv() ) );

                if(playerRsp.playerlvup_rsp().lv() == 2) {
                    P_EVT_LOG->logByLv(EVT_LOG_UP_TO_LV_TWO);
                }
            }

            P_CITY_CTRL->updateExp();
		} else if(playerRsp.has_queryotherplayer_rsp()) { // ��ѯ���������Ϣ
			P_LOGIN_OBJ->setOtherPlayerInfo(playerRsp.queryotherplayer_rsp().playerinfo());
			P_LOGIN_OBJ->setOtherPlayerDetailInfo(playerRsp.queryotherplayer_rsp().detailinfo());

            /*
            2014-11-5,ɾ���ϵ�Json����ʱ�ر�
			if(P_GUILD_CTRL->getGuildMemberPanel()->isOpen()) {
				P_GUILD_CTRL->getGuildMemberPanel()->updateUI();
			}
            */

			P_BAG_CTRL->updateOtherRoleInfo();

			/*if(P_BAG_CTRL->getRolePropertyPanel()->isOpen()) {
				P_BAG_CTRL->getRolePropertyPanel()->updateUI(playerRsp.queryotherplayer_rsp().playerinfo(), playerRsp.queryotherplayer_rsp().detailinfo());
			}*/
		}else if(playerRsp.has_fightpower_notify()){		//ս���仯
            const int iOldFp = P_USER_OBJ->getUserInfo()->fightPower;
            const int iNewFp = playerRsp.fightpower_notify().fightpower();
			P_USER_OBJ->updateFightPower(iNewFp);

            if(iNewFp > iOldFp) {
                P_GLOBAL->showFpUpEff(iOldFp, iNewFp);
            }

			P_CITY_CTRL->updateFightPower();
		} else if(playerRsp.has_quicklogin_rsp()) {
            P_USER_OBJ->setQuickLoginKey(playerRsp.quicklogin_rsp().quickkey());
            P_GAME_INF->doQuickLoginSuccess();
        } else if(playerRsp.has_dailyreg_rsp()) {
            P_USER_OBJ->updateDailyRegInfo(playerRsp.dailyreg_rsp());
            P_SIGN_IN_CTRL->updateSignInRsp(true);
        }

        bool bIsAfterPlayInfoInited = (P_USER_OBJ->getUserInfo()->newestWarMapID != 0 );

        if( !bIsBeforePlayInfoInited && bIsAfterPlayInfoInited ) {
            //��½ʱ��ȡ�����Ϣ����Ҫ���µ���Ϣ��������
            P_GLOBAL->onLoginPlayerInfoInited();
        }
		//SceneManager::sharedSceneManager()->enterScene(SCENE_ID_CITY);
	} else if(stMsg.has_goldrespond()) { // ����
		const vmsg::CSGoldRespond &stGoldRsp = stMsg.goldrespond();

		if(stGoldRsp.has_querygold_rsp()) {
			//P_USER_OBJ->setGoldInfo(stGoldRsp.querygold_rsp());
            //�����ֶ���Ҫ��ʱ����뵽UserInfo���
            P_USER_OBJ->updateGold(stGoldRsp.querygold_rsp().gold(), stGoldRsp.querygold_rsp().totalgoldbymoney());

		} else if(stGoldRsp.has_gold_notify()) {
			//P_USER_OBJ->setGoldInfo(stGoldRsp.gold_notify());
            P_USER_OBJ->updateGold(stGoldRsp.gold_notify().gold(), stGoldRsp.gold_notify().totalgoldbymoney());
			
		}

		P_CITY_CTRL->updateGold();
		P_VIP_CTRL->updateGold();
	} else if(stMsg.msg_head().msg_id() == vmsg::CS_CMD_PLAYER_SETSETTING){ //������÷��ػر�
		P_SET_CTRL->doSetRsp();
	}
}

void LoginProxy::fault(const vmsg::CSCommonErrMsg &err) {
	if(err.rqstcmdid() == vmsg::CS_CMD_PLAYER_LOGIN && err.errorcode() == 100005) { // login failure
        P_LOGIN_CTRL->getLoginScreenView()->closeLoading();
        P_LOGIN_OBJ->getRoleInfoList().clear(); //û��ע�ᣬ��ո��б�
        P_LOGIN_CTRL->getCreateRolePanel()->open();
	} else if(err.rqstcmdid() == vmsg::CS_CMD_REG_PLAYERINFO){  //ע������ʧ��
		P_LOGIN_CTRL->getCreateRolePanel()->showTip();
    } else if(err.rqstcmdid() == vmsg::CS_CMD_PLAYER_QUICKLOGIN){  //���ٵ�½ʧ��
        P_GAME_INF->doQuickLoginFail();
    }
}

void LoginProxy::sendLoginRqst(const std::string & loginInfo) {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_LOGIN;
    vmsg::VCli2LogicSvrMsg stMsg;
    
    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    stMsgHead.set_uid(0);
    stMsgHead.set_act(1);
    stMsgHead.set_msg_id(uCmd);
    stMsgHead.set_seq(1);
    stMsgHead.set_msg_type(vmsg::MsgHead::RQST_MSG);
    stMsgHead.set_routeid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);
    stMsgHead.set_app_id(APPID_VCONND);

    vmsg::CSPlayerLoginRqst &stPlayerLoginRqst = *(stMsgHead.mutable_loginrqst());
    stPlayerLoginRqst.set_zoneid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);

	//��У��
	unsigned int uTime = time(NULL);
	stPlayerLoginRqst.set_sign((uTime * uTime) % 0xFFFFFFEE); //���ﲻ�����ģ����˾��޷���֤��ȥ
	stPlayerLoginRqst.set_time(uTime);
	stPlayerLoginRqst.set_logininfo(loginInfo.c_str());

	
	//����򵥵ļ�һ��sign����
	vmsg::AuthInfo& stAuthInfo = *(stPlayerLoginRqst.mutable_authinfo());
	stAuthInfo.set_openid("");
	stAuthInfo.set_openkey("");
#ifdef TGAME_IN_APP_SDK
	stAuthInfo.set_platform(FaceBookSDK::getPlatformName());
#else
	// ���
	stAuthInfo.set_platform("null");
#endif

    P_NET_MGR->SendPkg(stMsg);

    CCLOG("LoginProxy::sendLoginRqst,zoneid=[%d],loginInfo=[%s]", 
        P_LOGIN_OBJ->getCurSvrInfo().zoneid , loginInfo.c_str() );
}

void LoginProxy::sendAppleGuestLoginRqst(const std::string & loginInfo) {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_LOGIN;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    stMsgHead.set_uid(0);
    stMsgHead.set_act(1);
    stMsgHead.set_msg_id(uCmd);
    stMsgHead.set_seq(1);
    stMsgHead.set_msg_type(vmsg::MsgHead::RQST_MSG);
    stMsgHead.set_routeid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);
    stMsgHead.set_app_id(APPID_VCONND);

    vmsg::CSPlayerLoginRqst &stPlayerLoginRqst = *(stMsgHead.mutable_loginrqst());
    stPlayerLoginRqst.set_zoneid(P_LOGIN_OBJ->getCurSvrInfo().zoneid);
    stPlayerLoginRqst.set_logininfo(loginInfo.c_str());
    stPlayerLoginRqst.set_guestlogin(1);
    stPlayerLoginRqst.set_appplatform("appstore");
    P_NET_MGR->SendPkg(stMsg);

    CCLOG("LoginProxy::sendAppleGuestLoginRqst,zoneid=[%d],loginInfo=[%s]", 
        P_LOGIN_OBJ->getCurSvrInfo().zoneid , loginInfo.c_str() );
}

void LoginProxy::sendQueryRoleListRqst() {
	unsigned int uCmd = vmsg::CS_CMD_QUERY_ROLE_LIST;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, 0);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendRegisterRqst(unsigned int uPlayerID, const char *pszNickName) {
	unsigned int uCmd = vmsg::CS_CMD_REG_PLAYERINFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, 0, true);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->mutable_regplayerinfo_rqst()->set_playerid(uPlayerID);
	pstPlayerRqst->mutable_regplayerinfo_rqst()->set_nick(pszNickName);

    P_NET_MGR->SendPkg(stMsg);
    ADD_EVT_LOG(EVT_LOG_CREATE_ROLE); //��Ϸ�¼���־
}

void LoginProxy::sendRoleLoginRqst(UIN_t uin) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_ROLE_LOGIN;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, uin, true);

	vmsg::CSPlayerRequest &stPlayerRqst = *(stMsg.mutable_playerrequest());
	vmsg::CSRoleLoginRqst &stRoleLoginRqst = *(stPlayerRqst.mutable_rolelogin_rqst());
	stRoleLoginRqst.set_uin(uin);

    P_NET_MGR->SendPkg(stMsg);
    CCLOG("LoginProxy::sendRoleLoginRqst,uin=%lu", uin );
}

void LoginProxy::sendRoleLogoutRqst(UIN_t uin) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_ROLE_LOGOUT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSPlayerRequest &stPlayerRqst = *(stMsg.mutable_playerrequest());
	vmsg::CSRoleLogoutRqst &stRoleLogoutRqst = *(stPlayerRqst.mutable_rolelogout_rqst());
	stRoleLogoutRqst.set_uin(uin);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendQueryPlayerInfoRqst(UIN_t uin) {
	unsigned int uCmd = vmsg::CS_CMD_QUERY_PLAYERINFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, uin);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendHeartBeatRqst() {
	unsigned int uCmd = vmsg::CS_CMD_HEART_BEAT;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin, false, false);		//��������seq

	vmsg::CSHeartBeatRqst *pstHeartBeatRqst = stMsg.mutable_heartbeat_rqst();
	pstHeartBeatRqst->set_reserved(0);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendGetSvrListHttpRqst(const std::string &strURL) {
    ADD_EVT_LOG(EVT_LOG_GET_SVR_LIST); //��Ϸ�¼���־
	P_HTTP_PROXY->sendGetDirInfoRqst(strURL);
}
 
void LoginProxy::sendQueryGoldInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_GOLD_QUERY;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendQueryPlayerDetailInfoRqst() {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERY_DETAILINFO;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendQueryOtherPlayerInfoRqst(UIN_t uin) {
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUERYOTHERPLAYER;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->set_queryotherplayer_rqst(uin);

	P_NET_MGR->SendPkg(stMsg);
}


void LoginProxy::sendQuickLoginRqst(const bool bNeedOsdInfo) {
    unsigned int uCmd = vmsg::CS_CMD_PLAYER_QUICKLOGIN;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    stMsgHead.set_quickloginkey(P_USER_OBJ->getQuickLoginKey());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin, true, false);

    vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
    pstPlayerRqst->mutable_quicklogin_rqst()->set_needosdcast(bNeedOsdInfo);

    P_NET_MGR->SendPkg(stMsg);
    CCLOG("[trace]LoginProxy::sendQuickLoginRqst,key %llu,need osd ? %d", P_USER_OBJ->getQuickLoginKey(), bNeedOsdInfo );
}

void LoginProxy::sendDailySignInRqst() {

    unsigned int uCmd = vmsg::CS_CMD_PLAYER_DAILYREG;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    P_NET_MGR->SendPkg(stMsg);
}

void LoginProxy::sendSetSettingParamRqst(const std::string &strSettingParam){
	unsigned int uCmd = vmsg::CS_CMD_PLAYER_SETSETTING;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

	vmsg::CSPlayerRequest *pstPlayerRqst = stMsg.mutable_playerrequest();
	pstPlayerRqst->mutable_setsettingparam_rqst()->set_settingparam(strSettingParam);

	P_NET_MGR->SendPkg(stMsg);
}


void LoginProxy::sendUmengDeviceTokenRqst() {
    const std::string strToken = P_GAME_INF->getUmengDeviceToken();
    if(strToken.size() <= 0) {
        return;
    }
    unsigned int uCmd = vmsg::CS_CMD_REPORT_UMEM_DEVICE_TOKEN;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSUMemDeviceTokenReport* pstRqst = stMsg.mutable_devicetokenreport();
    pstRqst->set_token(strToken);
    
    P_NET_MGR->SendPkg(stMsg);
}
