#include "Global.h"
#include <string>
#include "Resources.h"
#include "BaseScene.h"
#include "control/AudioManager.h"
#include "ItemUtils.h"
#include "ImageNumber.h"
#include "task/TaskControl.h"
#include "shop/ShopControl.h"
#include "chat/ChatControl.h"
#include "evolution/EvolutionControl.h"
#include "frag/FragControl.h"
#include "vip/VIPControl.h"
#include "mail/MailControl.h"
#include "signin/SignInControl.h"
#include "warmap/WarMapControl.h"
#include "UserObj.h"
#include "bag/BagControl.h"
#include "login/LoginObj.h"
#include "ToolUtils.h"
#include "login/LoginControl.h"
#include "GlobalTime.h"
#include "ChannelInfoCallback.h"
#include "dailyinstance/DailyInstanceControl.h"
#include "game/lottery/LotteryControl.h"

using namespace std;


Global::Global()
{
    m_iDeltaTime = 0;
}

Global::~Global()
{
}

unsigned int Global::getSvrTime() const {
	return m_uSvrTime;
}
unsigned int Global::getSvrTimeRelative() {

    int iLocalNow = GlobalTime::getTime();

    return iLocalNow + m_iDeltaTime;
}

void Global::setSvrTime(unsigned int uSvrTime) {
    ///m_iDeltaTime
    int iLocalNow = GlobalTime::getTime();
    m_iDeltaTime = uSvrTime - iLocalNow;

	m_uSvrTime = uSvrTime;

    //P_WEAPON_LOT_CTRL->onHeartBeat();
	P_LOTTERY_CTRL->onHeartBeat();
    CCLOG("Global::setSvrTime,%d,local %d, delta %d", uSvrTime, iLocalNow, m_iDeltaTime);
}


void Global::showPopEff(const char* strEffName) {
    if(!strEffName) {
        return;
    }
    string fullPath(strEffName);
    fullPath = EFFECT_RES_PATH + fullPath + "/" + fullPath + JSON_TYPE_NAME;
    string strSound(strEffName);
    strSound += ".ogg";

    GPopEffInfo stInfo;
    stInfo.strJsonPath = fullPath;
    stInfo.strArmatureName = strEffName;
    stInfo.strSoundName = strSound;
    showPopEff(stInfo);
    
}


void Global::showEffOnNode(CCNode* pParentNode, const char* strArmatureName, 
                           const char* strFullPath, 
                           const CCPoint& stArmaturePos,
                           const CCPoint& stArmatureAnchorPt,
                           const int iLoop) 
{
    if(!strFullPath || !pParentNode) {
        return;
    }


    CCArmature* pstArmature = NULL; 
    Widget* pw = dynamic_cast<Widget*>(pParentNode);
    if(pw) {
        pw->removeNodeByTag(POP_EFFECT_TAG);
    } else {
        pParentNode->removeChildByTag(POP_EFFECT_TAG);
    }
    

    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(strFullPath);
    pstArmature = CCArmature::create(strArmatureName);

    //特殊处理一下，ccp(255, 255)
    if(stArmatureAnchorPt.x != 255 || stArmatureAnchorPt.y != 255) {
        pstArmature->setAnchorPoint(stArmatureAnchorPt);
    }
    
    pstArmature->setPosition(stArmaturePos);

    pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Global::onPopEffDone));

    if(pw) {
        pw->addNode(pstArmature, POP_EFFECT_Z, POP_EFFECT_TAG);
    } else {
        pParentNode->addChild(pstArmature, POP_EFFECT_Z, POP_EFFECT_TAG);
    }
    

    pstArmature->getAnimation()->play("fly", -1, -1, iLoop); //not loop

}

void Global::onPopEffDone(CCArmature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type){
        //CCLOG("%s", armature->getName().c_str()); //例如：ut00031
        string file = "effect/" + armature->getName() + "/" + armature->getName() + ".ExportJson";
        CToolUtils::removeArmatureFile(file.c_str());
        armature->removeFromParentAndCleanup(true);
        
    } else if(LOOP_COMPLETE == type) {
        /*
        static int iPlayCnt = 1;
        if(iPlayCnt >= 2) { //播放2次吧
            iPlayCnt = 0;
            armature->removeFromParentAndCleanup(true);
        } else {
            iPlayCnt++;
        }*/
    }
    
}


void Global::rmEffOnNode(CCNode* pParentNode) {
    if(NULL == pParentNode) {
        return;
    }
    CCArmature* pstArmature = NULL; 
    Widget* pw = dynamic_cast<Widget*>(pParentNode);
    if(pw) {
        pstArmature = dynamic_cast<CCArmature*>(pw->getNodeByTag(POP_EFFECT_TAG));
        pw->removeNodeByTag(POP_EFFECT_TAG);
    } else {
        pstArmature = dynamic_cast<CCArmature*>(pParentNode->getChildByTag(POP_EFFECT_TAG));
        pParentNode->removeChildByTag(POP_EFFECT_TAG);
    }

}


void Global::showPopEff(const GPopEffInfo& stEffInfo) {
    if(stEffInfo.strJsonPath.size() <= 0 && stEffInfo.strAniName.size() <= 0) {
        CCLOG("[error]Global::showPopEff2 fail,GPopEffInfo's json or ani is empty!");
        return;
    }

    CCLayer *pUILayer = getScenePopLayer();
    if(NULL == pUILayer) {
        CCLOG("[error]Global::showPopEff2 fail,LAYER_ID_POP is null.eff=%s", stEffInfo.strJsonPath.c_str());
        return;
    }

    if(stEffInfo.strSoundName.size() > 0) {
        AudioManager::playEffect( stEffInfo.strSoundName.c_str() );
    }

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    const CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    pUILayer->removeChildByTag(POP_EFFECT_TAG);
    CCArmature* pstArmature = NULL;

    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(stEffInfo.strJsonPath.c_str());
    pstArmature = CCArmature::create(stEffInfo.strArmatureName.c_str());

    pstArmature->setAnchorPoint(ccp(0.5f, 0.5f));
    pstArmature->setPosition(ccp(globalSize.width/2, globalSize.height * stEffInfo.fYPosPercent ) + stOrigin); //屏幕中位置

    if(stEffInfo.target && stEffInfo.callFunc) {
        pstArmature->getAnimation()->setMovementEventCallFunc(stEffInfo.target, stEffInfo.callFunc);
    } else {
        if(false == stEffInfo.bIsLoop) {
            //不循环的动作，默认播放一次，然后清除
            pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Global::onPopEffDone));
        }
    }

    pUILayer->addChild(pstArmature, POP_EFFECT_Z, POP_EFFECT_TAG);

    pstArmature->getAnimation()->play(stEffInfo.strAniName.c_str(), -1, -1, stEffInfo.bIsLoop); 
}

void Global::clearPopEff() {
    CCLayer *pUILayer = getScenePopLayer();
    if(NULL == pUILayer) {
        CCLOG("[error]Global::clearPopEff fail,LAYER_ID_POP is null.");
        return;
    }

    pUILayer->removeChildByTag(POP_EFFECT_TAG);
}


void Global::showAdvItemEff(const int iType, const int iCfgID, CCObject *target, SEL_MovementEventCallFunc callFunc) 
{
    GPopEffInfo stEffInfo;
    stEffInfo.strJsonPath = "effect/ut00025/ut00025.ExportJson";
    stEffInfo.strArmatureName = "ut00025";
    stEffInfo.strSoundName = "ut00025.ogg";
    if(vmsg::THING_TYPE_WEAPON == iType) {
        stEffInfo.strAniName = "fly_d";
    } else {
        stEffInfo.strAniName = "fly_x"; //道具，装备
    }
    stEffInfo.target = target;
    stEffInfo.callFunc = callFunc;
    
    CCLayer *pUILayer = getScenePopLayer();
    if(NULL == pUILayer) {
        CCLOG("[error]Global::showAdvItemEff fail,LAYER_ID_POP is null.eff=%s", stEffInfo.strJsonPath.c_str());
        return;
    }

    if(stEffInfo.strSoundName.size() > 0) {
        AudioManager::playEffect( stEffInfo.strSoundName.c_str() );
    }

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    const CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCArmature* pstArmature = NULL;
    pUILayer->removeChildByTag(POP_EFFECT_TAG);

    CToolUtils::loadArmatureRGBA4444(stEffInfo.strJsonPath);
    //CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(stEffInfo.strJsonPath.c_str());
    pstArmature = CCArmature::create(stEffInfo.strArmatureName.c_str());
    pstArmature->setAnchorPoint(ccp(0.5f, 0.5f));
    pstArmature->setPosition(ccp(globalSize.width*0.5, globalSize.height*0.7) + stOrigin); //屏幕中位置
    pstArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(Global::onAdvItemEffFrameEvt));
    if(stEffInfo.target && stEffInfo.callFunc) {
        pstArmature->getAnimation()->setMovementEventCallFunc(stEffInfo.target, stEffInfo.callFunc);
    } else {
        if(false == stEffInfo.bIsLoop) {
            //不循环的动作，默认播放一次，然后清除
            pstArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Global::onPopEffDone));
        }
    }

    CCSprite* sp = CCSprite::create(ItemUtils::getItemIcon(iType, iCfgID).c_str());
    if(sp) {
        sp->setAnchorPoint(ccp(0.5f, 0.5f));
        sp->setVisible(false);
        pstArmature->addChild(sp, 20, POP_EFFECT_TAG); //把图标也放这里吧
    }

    pUILayer->addChild(pstArmature, POP_EFFECT_Z, POP_EFFECT_TAG);
    pstArmature->getAnimation()->play(stEffInfo.strAniName.c_str(), -1, -1, 0); 
}


void Global::onAdvItemEffFrameEvt(CCBone *bone, const char * stEvent, int originFrameIndex, int currentFrameIdx) {
    //CCLOG("frame event %s", stEvent);
    
    CCLayer *pUILayer = getScenePopLayer();
    if(NULL == pUILayer) {
        CCLOG("[error]Global::onAdvItemEffFrameEvt fail,LAYER_ID_POP is null.");
        return;
    }

    CCArmature* pstArmature = dynamic_cast<CCArmature*>(pUILayer->getChildByTag(POP_EFFECT_TAG));
    if(NULL == pstArmature) {
        return;
    }
    CCSprite* pIcon = dynamic_cast<CCSprite*>(pstArmature->getChildByTag(POP_EFFECT_TAG));
    if(pIcon) {
        pIcon->setVisible(true);
    }
}

void Global::clearAdvItemEff() {
    CCLayer *pUILayer = getScenePopLayer();
    if(NULL == pUILayer) {
        CCLOG("[error]Global::onAdvItemEffFrameEvt fail,LAYER_ID_POP is null.");
        return;
    }

    pUILayer->removeChildByTag(POP_EFFECT_TAG);
}


CCLayer* Global::getScenePopLayer() {
    BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(NULL == pScene) {
        CCLOG("[error]Global::getScenePopLayer fail,running scene is null.");
        return NULL;
    }

    CCLayer *pUILayer = dynamic_cast<CCLayer*>(pScene->getLayer(LAYER_ID_POP));
    if(NULL == pUILayer) {
        CCLOG("[error]Global::getScenePopLayer fail,LAYER_ID_POP is null.");
        return NULL;
    }
    return pUILayer;
}

void Global::showFpUpEff(const int iFromVal, const int iToVal) {
    if(iToVal <= iFromVal) {
        return;
    }
    CCLayer* pPopLayer = getScenePopLayer();
    if(!pPopLayer) {
        return;
    }

    UIImageView* pBg = dynamic_cast<UIImageView*>(pPopLayer->getChildByTag(POP_FP_UP_TAG)) ;
    pPopLayer->removeChildByTag(POP_FP_UP_TAG);

    pBg = UIImageView::create();
    pBg->loadTexture("ui_v2/fp_up_bg.png");

    ImageNumber* pImgNum = ImageNumber::create(26);
    pImgNum->setAnchorPoint(ccp(0.0f, 0.5f));
    pImgNum->setPosition( ccp(10, 25) );
    pBg->addNode(pImgNum);

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    const CCPoint stOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    pBg->setAnchorPoint(ccp(0.5f, 0.5f));
    pBg->setPosition(ccp(globalSize.width*0.5, globalSize.height*0.64) + stOrigin);
    pPopLayer->addChild(pBg, POP_FP_UP_Z, POP_FP_UP_TAG);

    pImgNum->setRollDoneCallback(this, rolldone_selector(Global::onRollFpDone) );
    int iDeltaVal = ((iToVal - iFromVal) <= 20) ? 3 : 20;
    pImgNum->rollNumFromTo(iFromVal, iToVal, 0.0f, iDeltaVal);
}

void Global::onRollFpDone() {
    //roll完了
    CCLayer* pPopLayer = getScenePopLayer();
    if(!pPopLayer) {
        return;
    }

    UIImageView* pBg = dynamic_cast<UIImageView*>(pPopLayer->getChildByTag(POP_FP_UP_TAG)) ;

    if(!pBg) {
        return;
    }
    // CCFadeOut::create(1.5f)  CCDelayTime::create(1.5)
    CCCallFunc *actbackfun = CCCallFunc::create(this, callfunc_selector(Global::onCleanFpEff));
    pBg->runAction(CCSequence::create(CCDelayTime::create(1.5), actbackfun, NULL));
}

void Global::onCleanFpEff() {
    //开始销毁
    CCLayer* pPopLayer = getScenePopLayer();
    if(!pPopLayer) {
        return;
    }
    pPopLayer->removeChildByTag(POP_FP_UP_TAG);

}

void Global::releaseUnusePanel() {
    CCLOG("[trace]Global::releaseUnusePanel");
    //切出city时，清除掉一些不常用的面板吧
    //探秘，任务，商城，vip
    P_TASK_CTRL->finalize();
    P_SHOP_CTRL->finalize();
    P_CHAT_CTRL->finalize();
    P_EVOLUTION_CTRL->finalizePanel();
    P_FRAG_CTRL->finalize();
    P_VIP_CTRL->finalizePanel();
    P_MAIL_CTRL->finalizePanel();
    P_LOTTERY_CTRL->finalize();
    P_SIGN_IN_CTRL->finalizePanel();
    DAILY_INSTANCE_CTRL->finalizePanel();


    CToolUtils::removeArmatureFile("effect/ut00066/ut00066.ExportJson"); ///枪械库升级特效
    CToolUtils::removeArmatureFile("effect/ut00064/ut00064.ExportJson"); ///枪械库背板特效

    CToolUtils::removeArmatureFile("effect/ut00070/ut00070.ExportJson"); //装备升级特效
    CToolUtils::removeArmatureFile("effect/ut00059/ut00059.ExportJson"); //背包面板角色脚下特效
    CToolUtils::removeArmatureFile("effect/ut00028/ut00028.ExportJson"); //接受任务
    CToolUtils::removeArmatureFile("effect/ut00029/ut00029.ExportJson"); //完成任务
}

void Global::resetDataOnReconnect() {
    //清除掉当前登录角色的数据
    //角色，背包，任务
    P_USER_OBJ->resetOnConnClose();
    P_BAG_CTRL->resetOnConnClose();
    P_TASK_CTRL->resetOnConnClose();
    P_LOGIN_OBJ->resetOnConnClose();
    ChannelInfoCallback::resetSDKLoginInfo();
}

void Global::onLoginPlayerInfoInited() {
    CCLOG("[trace]Global::onLoginPlayerInfoInited");
    
    P_WAR_MAP_CTRL->sendQueryWarMapInfoRqst();
}

void Global::onNewDayRefresh() {
    CCLOG("[trace]Global::onNewDayRefresh!");
    //刷新签到面板
    P_LOGIN_CTRL->getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);

}
