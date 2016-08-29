#include "CityProxy.h"

#include "ClientSvrMsg.pb.h"
#include "CSSenceOSD.pb.h"
#include "QNetMgr.h"

#include "CityControl.h"
#include "CityScene.h"
#include "game/UserObj.h"
#include "dailyact/GuardActScene.h"
#include "basic/TimeEx.h"

CityProxy::CityProxy()
{
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_OSD_GET_PLAYERS_IN_SCENE, this);
    P_NET_MGR->RegCmdListener(vmsg::CS_CMD_OSD_MOVE_IN_SCENE, this);
	// push
	P_NET_MGR->RegCmdListener(vmsg::CS_CMD_OSD_SYNC_PLAYER_IN_SCENE, this);
    m_pOSDSceneInf = NULL;
}

CityProxy::~CityProxy()
{
    m_pOSDSceneInf = NULL;
}

void CityProxy::setOSDSceneInf(OSDSceneInf *pOSDSceneInf) {
    if(NULL == pOSDSceneInf) {
        CCLOG("[warn]CityProxy::setOSDSceneInf, pOSDSceneInf is null.");
        return;
    }
    CCLOG("[trace]CityProxy::setOSDSceneInf,set OSDSceneInf to %s", typeid(*pOSDSceneInf).name() );
    //CCLOG("%s", typeid(CityProxy).name());
    m_pOSDSceneInf = pOSDSceneInf;
}

void CityProxy::stopOSD(){
    sendPlayerLeaveRqst();
    m_pOSDSceneInf = NULL;
    CCLOG("[trace] CityProxy::stopOSD.");
}

void CityProxy::DoRsp(vmsg::VLogicSvr2CliMsg &stMsg) {

    if(stMsg.msg_head().msg_id() == 603) {
        return; //没有msg body
    }

    const unsigned long uPlayerUin = P_USER_OBJ->getUserInfo()->uin;
	if(stMsg.has_sceneosdrespond()) {
        if(NULL == m_pOSDSceneInf) {
            CCLOG("[warn]osdsceneinf is null,ignore this osd msg %d", stMsg.msg_head().msg_id());
            return;
        }
		const vmsg::CSSceneOSDRespond& sceneOSDRsp = stMsg.sceneosdrespond();
        
		if(sceneOSDRsp.has_queryplayersceneosdinfo_rsp()) {
			const vmsg::CSQueryPlayerSceneOSDInfoRsp &sceneOSDInfo = (sceneOSDRsp.queryplayersceneosdinfo_rsp());

			google::protobuf::RepeatedPtrField<vmsg::CSPlayerPosInScene> playerPosInfos = sceneOSDInfo.playerspos();
			google::protobuf::RepeatedPtrField<vmsg::CSPlayerPosInScene>::iterator playerPosIter;

			for(playerPosIter = playerPosInfos.begin(); playerPosIter != playerPosInfos.end(); playerPosIter++) {
				if(playerPosIter->comm().uin() != uPlayerUin) {
					m_pOSDSceneInf->addRoleQueue(*playerPosIter);
					//m_pOSDSceneInf->addRole(playerPosIter->comm(), 
     //                   ccp(playerPosIter->scenepoint().xpos(), playerPosIter->scenepoint().ypos()), 
     //                   ccp(playerPosIter->direct().xpos(), playerPosIter->direct().ypos()) / 10000);
				}
			}
            
            //更新怪物
            for(int i = 0; i < sceneOSDInfo.monsterosdinfos_size(); ++i) {
                const CSOSDMonstersInfo& oneMonster = sceneOSDInfo.monsterosdinfos(i);
                m_pOSDSceneInf->addMonster(oneMonster);
            }

		} else if(sceneOSDRsp.has_syncplayerosdinfo_push()) {
			//CCLOG("osd move recv %s", TimeEx::getTimeStr().c_str());
			doSyncPlayerOSDInfo(stMsg);
		} //end else
	} //end if
}

void CityProxy::fault(const vmsg::CSCommonErrMsg &err){
	if(err.rqstcmdid() == CS_CMD_OSD_CHANGE_SCENE){  //切换场景出错

	}
}


void CityProxy::sendGetPlayersRqst() {
	unsigned int uCmd = vmsg::CS_CMD_OSD_GET_PLAYERS_IN_SCENE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CityProxy::sendPlayerLeaveRqst() {
	unsigned int uCmd = vmsg::CS_CMD_OSD_LEAVE_SCENE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	P_NET_MGR->SendPkg(stMsg);
}

void CityProxy::sendPlayerMoveRqst(const CCPoint &direct, const CCPoint &pos) {
	unsigned int uCmd = vmsg::CS_CMD_OSD_MOVE_IN_SCENE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);

	vmsg::CSSceneOSDRequest &sceneOSDRqst = *(stMsg.mutable_sceneosdrequest());

	vmsg::CSPlayerMoveInScene &playerMoveRqst = *(sceneOSDRqst.mutable_playermove_rqst());
	playerMoveRqst.set_uin(P_USER_OBJ->getUserInfo()->uin);
	vmsg::CSScenePoint &direction = *(playerMoveRqst.mutable_direct());
	direction.set_xpos(direct.x);
	direction.set_ypos(direct.y);
	vmsg::CSScenePoint &scenePoint = *(playerMoveRqst.mutable_scenepoint());
	scenePoint.set_xpos(pos.x);
	scenePoint.set_ypos(pos.y);

	P_NET_MGR->SendPkg(stMsg);
}

void CityProxy::sendPlayerSetPosRqst(const CCPoint &direct, const CCPoint &pos) {
	unsigned int uCmd = vmsg::CS_CMD_OSD_SETPOS_SCENE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);
	
	vmsg::CSSceneOSDRequest &sceneOSDRqst = *(stMsg.mutable_sceneosdrequest());

	vmsg::CSPlayerSetPosInScene &playerSetPosRqst = *(sceneOSDRqst.mutable_playersetpos_rqst());
	playerSetPosRqst.set_uin(P_USER_OBJ->getUserInfo()->uin);
	vmsg::CSScenePoint &direction = *(playerSetPosRqst.mutable_direct());
	direction.set_xpos(direct.x);
	direction.set_ypos(direct.y);
	vmsg::CSScenePoint &scenePoint = *(playerSetPosRqst.mutable_scenepoint());
	scenePoint.set_xpos(pos.x);
	scenePoint.set_ypos(pos.y);
	
	P_NET_MGR->SendPkg(stMsg);
}

void CityProxy::sendPlayerChangeSceneRqst(unsigned int uSceneID) {
	unsigned int uCmd = vmsg::CS_CMD_OSD_CHANGE_SCENE;
	vmsg::VCli2LogicSvrMsg stMsg;

	vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
	P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd, P_USER_OBJ->getUserInfo()->uin);
	
	vmsg::CSSceneOSDRequest &sceneOSDRqst = *(stMsg.mutable_sceneosdrequest());

	vmsg::CSPlayerChangeSceneRqst &playerChangeSceneRqst = *(sceneOSDRqst.mutable_playerchangescene_rqst());
	playerChangeSceneRqst.set_newsceneid(uSceneID);
	
	P_NET_MGR->SendPkg(stMsg);
}


void CityProxy::sendPlayerAtkRqst(bool bIsShooting, const CCPoint& direct, const CCPoint& pos) {
    unsigned int uCmd = bIsShooting ? vmsg::CS_CMD_OSD_START_ATK : CS_CMD_OSD_STOP_ATK;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSSceneOSDRequest &sceneOSDRqst = *(stMsg.mutable_sceneosdrequest());

    if(bIsShooting) {
        vmsg::CSPlayerStartAtk &startAtk = *(sceneOSDRqst.mutable_playerstartatk_rqst());
        startAtk.set_uin(P_USER_OBJ->getUserInfo()->uin);
        startAtk.mutable_direct()->set_xpos(direct.x);
        startAtk.mutable_direct()->set_ypos(direct.y);

        startAtk.mutable_scenepoint()->set_xpos(pos.x);
        startAtk.mutable_scenepoint()->set_ypos(pos.y);
    } else {
        vmsg::CSPlayerStopAtk &stopAtk = *(sceneOSDRqst.mutable_playerstopatk_rqst());
        stopAtk.set_uin(P_USER_OBJ->getUserInfo()->uin);
        stopAtk.mutable_direct()->set_xpos(direct.x);
        stopAtk.mutable_direct()->set_ypos(direct.y);

        stopAtk.mutable_scenepoint()->set_xpos(pos.x);
        stopAtk.mutable_scenepoint()->set_ypos(pos.y);
    }

    P_NET_MGR->SendPkg(stMsg);
}


void CityProxy::sendPlayerSkillInSceneRqst(const int iSkillID, const CCPoint& direct, const CCPoint& pos) {
    unsigned int uCmd = vmsg::CS_CMD_OSD_SKILL;
    vmsg::VCli2LogicSvrMsg stMsg;

    vmsg::MsgHead &stMsgHead = *(stMsg.mutable_msg_head());
    P_NET_MGR->GenRqstMsgHead(stMsgHead, uCmd);

    vmsg::CSSceneOSDRequest &sceneOSDRqst = *(stMsg.mutable_sceneosdrequest());

    vmsg::CSPlayerSkillInScene &playerSkillRqst = *(sceneOSDRqst.mutable_playerskill_rqst());
    playerSkillRqst.set_uin(P_USER_OBJ->getUserInfo()->uin);
    playerSkillRqst.mutable_direct()->set_xpos(direct.x);
    playerSkillRqst.mutable_direct()->set_ypos(direct.y);
    playerSkillRqst.mutable_scenepoint()->set_xpos(pos.x);
    playerSkillRqst.mutable_scenepoint()->set_ypos(pos.y);
    playerSkillRqst.set_skillid(iSkillID);

    P_NET_MGR->SendPkg(stMsg);
}


void CityProxy::doSyncPlayerOSDInfo(vmsg::VLogicSvr2CliMsg &stMsg) {
    const vmsg::CSSceneOSDRespond& sceneOSDRsp = stMsg.sceneosdrespond();
    const vmsg::CSSyncPlayerOSDInfo *pPlayerOSDInfo = &(sceneOSDRsp.syncplayerosdinfo_push());
    const unsigned long uPlayerUin = P_USER_OBJ->getUserInfo()->uin;
    /*
    if(typeid(*m_pOSDSceneInf) == typeid(GuardActScene)) {

    }*/

    unsigned int uUin = 0;
    CCPoint pos = CCPointZero;
    CCPoint direct = CCPointZero;

    google::protobuf::RepeatedPtrField<vmsg::CSPlayerOSDInfo> playerOSDInfos =  pPlayerOSDInfo->playerosdinfos();
    google::protobuf::RepeatedPtrField<vmsg::CSPlayerOSDInfo>::iterator iter;
    for(iter = playerOSDInfos.begin(); iter != playerOSDInfos.end(); iter++) {
        const int iActType = iter->type();
        if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_ENTER) {
            uUin = iter->playerenterscene().comm().uin();

            if(uUin != uPlayerUin) {
                pos = ccp(iter->playerenterscene().scenepoint().xpos(), iter->playerenterscene().scenepoint().ypos());
                direct = ccp(iter->playerenterscene().direct().xpos(), iter->playerenterscene().direct().ypos());
                //m_pOSDSceneInf->addRole(iter->playerenterscene().comm(), pos, direct / 10000); 
				m_pOSDSceneInf->addRoleQueue(iter->playerenterscene());		//先加入队列
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_LEAVE) {
            uUin = iter->playerleavesceneuin();

            if(uUin != uPlayerUin) {
                m_pOSDSceneInf->removeRole(uUin);
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_MOVE) {
            uUin = iter->playermoveinscene().uin();

            if(uUin != uPlayerUin) {
                direct = ccp(iter->playermoveinscene().direct().xpos(), iter->playermoveinscene().direct().ypos()) / 10000;
                pos = ccp(iter->playermoveinscene().scenepoint().xpos(), iter->playermoveinscene().scenepoint().ypos());
                m_pOSDSceneInf->moveRoleByDirection(uUin, pos, direct);
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_SET) {
            uUin = iter->playersetposinscene().uin();

            if(uUin != uPlayerUin) {
                pos = ccp(iter->playersetposinscene().scenepoint().xpos(), iter->playersetposinscene().scenepoint().ypos());
                direct = ccp(iter->playersetposinscene().direct().xpos(), iter->playersetposinscene().direct().ypos()) / 10000;
                m_pOSDSceneInf->setRolePos(uUin, pos, direct);
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_STARTATK) {
            
            uUin = iter->playerstartatk().uin();

            if(uUin != uPlayerUin) {
                m_pOSDSceneInf->setPlayerStartAtkStat(uUin, iter->playerstartatk());
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_STOPATK) {
            uUin = iter->playerstopatk().uin();

            if(uUin != uPlayerUin) {
                m_pOSDSceneInf->setPlayerStopAtkStat(uUin, iter->playerstopatk() );
            }
        } else if(iActType == vmsg::PLAYER_ACTION_IN_SCENE_SKILL) {
            uUin = iter->playerskill().uin();

            if(uUin != uPlayerUin) {
                m_pOSDSceneInf->setPlayerSkillInScene(uUin, iter->playerskill() );
            }
        }
    } //end for
}
