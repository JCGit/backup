#include "UpdateResScreen.h"
#include "defines.h"
#include "Resources.h"
#include "CommonDialog.h"
#include "SceneManager.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "CfgLoader.h"
#include "login/LoginScene.h"
#include "evtlog/EvtLog.h"
#include "QEnvSet.h"
#include "login/HttpProxy.h"
#include "GameInf.h"
#include "basic/ToolUtils.h"

using namespace std;

 
UpdateResScreen::UpdateResScreen()
:m_bIsAllReady(false)
,m_fPercentage(0.0f)
,m_pBgImg(NULL)
,m_pLoadingBg(NULL)
,m_pLoadBar(NULL)
,m_pLoadTxt(NULL)
,m_pLoadIcon(NULL)
,m_stInitPos(CCPointZero)
{
}

UpdateResScreen::~UpdateResScreen()
{
}

bool UpdateResScreen::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

        CCDirector *pDirector = CCDirector::sharedDirector();
        CCSize visibleSize = pDirector->getVisibleSize();
        CCPoint visiblePos = pDirector->getVisibleOrigin();

        this->setContentSize(visibleSize);

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/share.plist"); //dialog
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/loading_ui.plist");
        initLayoutFromJsonFile("ui_v2/loading_ui.ExportJson");
		//initLayoutFromJsonFile("ui/loading_ui.json");
        m_pRootLayout->setPosition(visiblePos);
		addChild(m_pRootLayout);

		CToolUtils::loadSpriteFrameRGBA4444("ui_bg/loading_ui_bg.plist");
		m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));
		m_pBgImg->loadTexture("ui_bg/loading_ui_bg.png", UI_TEX_TYPE_PLIST);
		m_pBgImg->setPosition(m_pRootLayout->convertToNodeSpace(visiblePos + ccp(visibleSize.width/2, visibleSize.height/2)));
		float scaleX = visibleSize.width / m_pBgImg->getSize().width;
		float scaleY = visibleSize.height / m_pBgImg->getSize().height;
		float scale = scaleX > scaleY ? scaleX : scaleY;
		m_pBgImg->setScale(scale);

		m_pLoadTxt = dynamic_cast<UILabel*>(m_pBgImg->getChildByName("progress_txt"));
		m_pLoadingBg = dynamic_cast<ImageView*>(m_pBgImg->getChildByName("bg_progress_img"));
		m_pLoadBar = dynamic_cast<LoadingBar*>(m_pBgImg->getChildByName("loading_progress"));
        m_pTipTxt = dynamic_cast<UILabel*>(m_pBgImg->getChildByName("tip_txt"));
        m_pTipTxt->setText("");

		m_stInitPos = m_pTipTxt->getPosition();

		CCSprite* sp = CCSprite::create();
        sp->initWithSpriteFrameName("loading_ui/loding_img_0.png");

        sp->setAnchorPoint(ccp(0.0f, 0.5f));
        m_pLoadIcon = dynamic_cast<ImageView*>(m_pBgImg->getChildByName("icon_loading"));
		sp->setPosition(visiblePos + m_pLoadIcon->getPosition() + ccp(m_pLoadIcon->getSize().width/2 + 10, -30));
		addNode(sp, 0, 100);

		CCAnimation* animation = CCAnimation::create();
		string str;
		for( int i = 0; i < 4; i++)  {  
			str = "loading_ui/loding_img_" + intToString(i) + PNG_TYPE_NAME;
            CCSpriteFrame* pSpFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str.c_str());
            animation->addSpriteFrame(pSpFrame); //neilwu mod
			//animation->addSpriteFrameWithFileName(str.c_str()); //加载动画的帧  
		}  
		animation->setDelayPerUnit(4.3f/14.0f);  
		animation->setLoops(-1); 
		CCAnimate *animate = CCAnimate::create(animation);  
		sp->runAction(animate);

		m_pLoadingBg->setVisible(false);
		m_pLoadBar->setVisible(false);
		m_pLoadTxt->setVisible(false);
		m_pLoadIcon->setVisible(false);
		m_pTipTxt->setPositionY(m_pLoadingBg->getPositionY() - 15);

		bRet = true;
	} while (0);

	return bRet;
}


bool UpdateResScreen::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	
	return true;
}

void UpdateResScreen::onEnter() {
	BaseLayout::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
	//init();
	m_bIsAllReady = true;
	setPercentage(m_fPercentage);

    m_pTipTxt->setText(NounStr(NOUN_CHECK_VER) + "...");
    queryDirConfig(); //查询dir更新配置
}


void UpdateResScreen::onExit() {
	BaseLayout::onExit();
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	removeAllChildrenWithCleanup(true);
	m_bIsAllReady = false;
	m_fPercentage = 0.0f;

	m_pLoadingBg->setVisible(false);
	m_pLoadBar->setVisible(false);
	m_pLoadTxt->setVisible(false);
	m_pLoadIcon->setVisible(false);
	m_pTipTxt->setPositionY(m_pLoadingBg->getPositionY() - 15);
}

void UpdateResScreen::setPercentage(float fPercentage) {
	m_fPercentage = fPercentage;

	if(m_bIsAllReady) {
		m_pLoadBar->setPercent(fPercentage);

		string strPercent = to_string((int)fPercentage) + "%";
		m_pLoadTxt->setText(strPercent);

		if(fPercentage != 0 && !m_pLoadingBg->isVisible()){
			m_pLoadingBg->setVisible(true);
			m_pLoadBar->setVisible(true);
			m_pLoadTxt->setVisible(true);
			m_pLoadIcon->setVisible(true);
			m_pTipTxt->setPositionY(m_stInitPos.y);
		}
	}
}

void UpdateResScreen::setNoticeWords(const char *pszNoticeWords) {
	if(pszNoticeWords) {
        m_pTipTxt->setText(pszNoticeWords);
    }
}


void UpdateResScreen::checkUpdate(float dt) {
    auto stat = m_stUpdateMgr.GetStatus();
    bool bNeedRestart = false;
    CCfgLoader stCfgLoader;
    switch (stat.stage)
    {
    case UpdateMgr::CHECK_VERSION:
        CCLOG("[trace]UpdateResScreen::checkUpdate, CHECK_VERSION.");
        m_pTipTxt->setText(NounStr(NOUN_CHECK_VER) + "...");
        break;
    case UpdateMgr::DOWNLOAD_RESOURCE:
        CCLOG("[trace]UpdateResScreen::checkUpdate, DOWNLOAD_RESOURCE. progress %d", stat.nowProgress);
        setPercentage(stat.nowProgress);
        m_pTipTxt->setText(NounStr(NOUN_DOWNLOAD) + "...");
        break;
    case UpdateMgr::EXTRACT_RESOURCE:
        CCLOG("[trace]UpdateResScreen::checkUpdate, EXTRACT_RESOURCE.");
        setPercentage(stat.nowProgress);
        m_pTipTxt->setText(NounStr(NOUN_EXTRACE_RES) + "...");
        break;
    case UpdateMgr::DONE:
        bNeedRestart = stat.needRestartGame;
        if(0 != stat.ret) {
            //出现错误了
            CCLOG("[warn]UpdateResScreen::checkUpdate, update done. error ret [%d], file [%s]", stat.ret, stat.filename.c_str() );
        }
        CCLOG("[trace]UpdateResScreen::checkUpdate, update done. need restart ? %d", bNeedRestart);
        m_pTipTxt->setText(NounStr(NOUN_DONE));
        setPercentage(100);
        if(0 == stat.totalStep) {
            //没有东西要更新
            CCLOG("[trace]UpdateResScreen::checkUpdate, nothing to update.");
        }
        setPercentage(100);

        this->unschedule(schedule_selector(UpdateResScreen::checkUpdate));
        m_stUpdateMgr.updateSearchPath();
        //clear full path cache
        CCFileUtils::sharedFileUtils()->resetFullPathCache();

        if(bNeedRestart) {
            //show restart
            COMMON_DIALOG->show(TextStr(TEXT_REBOOT_TIP), this, NounStr(NOUN_REBOOT_GAME), "", dialogbtneventselector(UpdateResScreen::onClickDialogRestartBtn));
        } else {
            //update res done,now enter login scene
            reloadAllCfg();
            LoginScene* pParentScene = dynamic_cast<LoginScene*>(this->getParent());
            if(pParentScene) {
                pParentScene->doUpdateResDone();
            } else {
                this->removeFromParent();
                onClickDialogEnterBtn();
            }
        }
        break;
    default:
        break;
    }

}
void UpdateResScreen::reloadAllCfg() {
    CCLOG("[trace]UpdateResScreen::checkUpdate,relaod all cfg.");

    ADD_EVT_LOG(EVT_LOG_LOAD_CFG_RES); //游戏事件日志
    CCfgLoader stCfgLoader;
    if (stCfgLoader.LoadAllCfg() != 0){
        CCLOG("[error]UpdateResScreen::checkUpdate,fail to init config!");
        CCAssert(0, "UpdateResScreen fail to init all config!");
        return;
    } else {
        CCLOG("[trace]UpdateResScreen::checkUpdate,load all cfg done.");
    }
    ADD_EVT_LOG(EVT_LOG_LOAD_CFG_RES_END); //游戏事件日志
}


void UpdateResScreen::onClickDialogRestartBtn() {
    CCLOG("[trace]now restart game");

    ADD_EVT_LOG(EVT_LOG_NEED_REBOOT); //游戏事件日志

    //CCDirector::sharedDirector()->end();
    P_GAME_INF->doRestartGame();
}


void UpdateResScreen::onClickDialogEnterBtn() {
    CCLOG("[trace]now enter game");

    SceneManager::sharedSceneManager()->enterScene(1);
}


void UpdateResScreen::queryDirConfig() {
    std::string strURL(P_GAME_INF->getDirURL());
    strURL += "/cdnurl";
    CCLOG("[trace]UpdateResScreen::queryDirConfig,query dir config, %s", strURL.c_str());
    
    std::string strRspData;

    int iRet = P_HTTP_PROXY->HttpGet(strURL, strRspData);
    onQueryDirConfigRsp(strRspData, iRet);
}

void UpdateResScreen::onQueryDirConfigRsp(const std::string& strRspData, const int iRetCode) {

    CCLOG("[trace]UpdateResScreen::onQueryDirConfigRsp,rsp code [%d], rsp [%s]", iRetCode , strRspData.c_str() );
    if(!P_HTTP_PROXY->bIsDoCurlOK(iRetCode) ) {
        //连接失败
        CCLOG("[error]UpdateResScreen::onQueryDirConfigRsp,rsp code %d", iRetCode);
        reloadAllCfg();
        LoginScene* pParentScene = dynamic_cast<LoginScene*>(this->getParent());
        if(pParentScene) {
            pParentScene->doUpdateResDone();
        } 

        return;
    }

    if(strRspData.size() <= 0) {
        CCLOG("[trace]UpdateResScreen::onQueryDirConfigRsp, dir config is empty,no need update");
        reloadAllCfg();
        LoginScene* pParentScene = dynamic_cast<LoginScene*>(this->getParent());
        if(pParentScene) {
            pParentScene->doUpdateResDone();
        } 
    } else {
        CCLOG("[trace]UpdateResScreen::onQueryDirConfigRsp, dir config is [%s]", strRspData.c_str());
        m_stUpdateMgr.setResUrl(strRspData.c_str());
        doDirConfigDone(); //可以开始更新
    }
    
}


void UpdateResScreen::doDirConfigDone() {
    CCLOG("UpdateResScreen,WritablePath=%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
    m_stUpdateMgr.updateSearchPath(); //提前更新一下搜索路径
    this->schedule(schedule_selector(UpdateResScreen::checkUpdate), 0.2f);
    m_stUpdateMgr.startUpdate();
}
