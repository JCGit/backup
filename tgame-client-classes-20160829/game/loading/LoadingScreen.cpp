#include "LoadingScreen.h"
#include "defines.h"
#include "Resources.h"
#include "basic/BaseScene.h"
#include "basic/HudLayer.h"
#include "ToolUtils.h"
#include "UIBoxLayer.h"
#include "WarmPromptCfgMaster.h"
#include "vlib/CVRand.h"

using namespace std;

static LoadingScreen* s_pInstance = NULL;

LoadingScreen::LoadingScreen()
:m_bIsAllReady(false)
,m_bIsOpen(false)
,m_fPercentage(0.0f)
,m_pBgImg(NULL)
,m_pLoadingBg(NULL)
,m_pLoadBar(NULL)
,m_pLoadTxt(NULL)
,m_pLoadingArmature(NULL)
,m_pTipTxt(NULL)
{
    m_pAniSp = NULL;
    m_pAni = NULL;
}

LoadingScreen::~LoadingScreen()
{
    CC_SAFE_RELEASE_NULL(m_pAni);

	CToolUtils::removeArmatureFile("effect/loding_tou_chuiqi1/loding_tou_chuiqi1.ExportJson");
}

LoadingScreen* LoadingScreen::getInstance() {
	if(s_pInstance == NULL) {
		s_pInstance = LoadingScreen::create();
		CC_SAFE_RETAIN(s_pInstance); // release at LoadingScreen::destroyInstance()
	}

	return s_pInstance;
}

void LoadingScreen::destroyInstance() {
	CC_SAFE_RELEASE_NULL(s_pInstance); // retain at LoadingScreen::getInstance()
    
}

LoadingScreen* LoadingScreen::create() {
	LoadingScreen *pRet = new LoadingScreen();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LoadingScreen::init(){
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCLayer::init());

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCSize screenSize = pDirector->getVisibleSize();
		CCPoint screenOrigin = pDirector->getVisibleOrigin();

		this->setContentSize(screenSize);

		m_pUILayout = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui_v2/loading_ui.ExportJson"));
		m_pUILayout->setPosition(screenOrigin);
		addChild(m_pUILayout, 10);

		//add bg
		CToolUtils::loadSpriteFrameRGBA4444("ui_bg/loading_ui_bg.plist");
		m_pBgImg = dynamic_cast<UIImageView*>(m_pUILayout->getChildByName("bg_img"));
		m_pBgImg->loadTexture("ui_bg/loading_ui_bg.png", UI_TEX_TYPE_PLIST);
		m_pBgImg->setPosition(m_pUILayout->convertToNodeSpace(screenOrigin + ccp(screenSize.width/2, screenSize.height/2)));
		float scaleX = screenSize.width / m_pBgImg->getSize().width;
		float scaleY = screenSize.height / m_pBgImg->getSize().height;
		float scale = scaleX > scaleY ? scaleX : scaleY;
		m_pBgImg->setScale(scale);

		CToolUtils::loadArmatureRGBA4444("effect/loding_tou_chuiqi1/loding_tou_chuiqi1.ExportJson");
		m_pLoadingArmature = CCArmature::create("loding_tou_chuiqi1");
		m_pLoadingArmature->setPosition(ccp(-360, -92));
		m_pBgImg->addNode(m_pLoadingArmature, 10);
		m_pLoadingArmature->getAnimation()->play("Animation1", -1, -1, 1);

		m_pLoadTxt = dynamic_cast<UILabel*>(m_pBgImg->getChildByName("progress_txt"));
		m_pLoadingBg = dynamic_cast<ImageView*>(m_pBgImg->getChildByName("bg_progress_img"));
		m_pLoadBar = dynamic_cast<LoadingBar*>(m_pBgImg->getChildByName("loading_progress"));
        m_pTipTxt = dynamic_cast<UILabel*>(m_pBgImg->getChildByName("tip_txt"));
        m_pTipTxt->setText("");

        UIImageView* pIconImg = dynamic_cast<UIImageView*>(m_pBgImg->getChildByName("icon_loading"));
        CCPoint pt = pIconImg->getPosition();
        pt.x += pIconImg->getSize().width / 2 + 5;
        pt.y -= pIconImg->getSize().height/2;
		m_pAniSp = CCSprite::create();
        m_pAniSp->initWithSpriteFrameName("loading_ui/loding_img_0.png");
		m_pAniSp->setPosition(pt);
        m_pAniSp->setAnchorPoint(ccp(0, 0));
		m_pUILayout->addNode(m_pAniSp);

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
		m_pAni = CCAnimate::create(animation);  
        
        m_pAni->retain();

		bRet = true;
	} while (0);

	return bRet;
}

void LoadingScreen::onEnter() {
	CCLayer::onEnter();
	
	//init();
    if(m_pAniSp && m_pAni) {
        m_pAniSp->runAction(m_pAni);
    }
	m_bIsAllReady = true;
	setPercentage(m_fPercentage);
	
	const WarmPromptCfgSet &stCfgSet = WARM_PROMPT_CFG_MASTER->GetCfg();
	int idx = CVLib::CRandom::GetRand(stCfgSet.warmpromptcfg_size());
	m_pTipTxt->setText(stCfgSet.warmpromptcfg(idx).prompt());

	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
	CCLayer *pLayer = pRunningScene->getLayer(LAYER_ID_HUD);

	if(pLayer != NULL) {
		dynamic_cast<HudLayer*>(pLayer)->Inactive();
	}

}

void LoadingScreen::onExit() {
	CCLayer::onExit();
	
	//removeAllChildren();
    if(m_pAni) {
        m_pAni->stop();
    }
	m_bIsAllReady = false;
	m_fPercentage = 0.0f;

	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
	CCLayer *pLayer = pRunningScene->getLayer(LAYER_ID_HUD);

	if(pLayer != NULL) {
		dynamic_cast<HudLayer*>(pLayer)->Active();
	}

}

void LoadingScreen::registerWithTouchDispatcher(){  
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);//使用int最小值做最高优先级,并且吞掉事件true  
}

bool LoadingScreen::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){     
	return true;  
}

void LoadingScreen::setPercentage(float fPercentage) {
    if(fPercentage > 100.0) {
        m_fPercentage = 100.0f;
    } else {
        m_fPercentage = fPercentage;
    }
	

	if(m_bIsAllReady) {
		m_pLoadBar->setPercent(m_fPercentage);

		string strPercent = to_string((int)m_fPercentage) + "%";
		m_pLoadTxt->setText(strPercent);
    }

}

void LoadingScreen::setNoticeWords(const char *pszNoticeWords) {
	
}

bool LoadingScreen::isOpen(){
	return m_bIsOpen;
}

void LoadingScreen::open(){
	if(m_bIsOpen){
		return;
	}

	BaseScene *pScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
	if(NULL == pScene) {
		CCLOG("[error]BasePanel::open fail,running scene is null.panel=%s", typeid(*this).name());
		return;
	}
	UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(pScene->getLayer(LAYER_ID_UI));
	pUILayer->addChild(this, 100);

	m_bIsOpen = true;
}

void LoadingScreen::close(){
	if(!m_bIsOpen){
		return;
	}

	if(this->getParent()){
		this->removeFromParentAndCleanup(true);
	}

	m_bIsOpen = false;
}

