#include "FightGuideLayout.h"
#include "CCColorUtil.h"
#include "FightGuideControl.h"
#include "FightGuideHud.h"

#include "FightGuideCfgMaster.h"

CFightGuideLayout::CFightGuideLayout()
:m_pDarkLayer(NULL)
,m_pLightStencil(NULL)
,m_pClipNode(NULL)
,m_stLightNodes(NULL)
,m_stBorders(NULL)
,m_pTipImg(NULL)
{
	m_stLightNodes = CCArray::create();
	CC_SAFE_RETAIN(m_stLightNodes);

	m_stBorders = CCArray::create();
	CC_SAFE_RETAIN(m_stBorders);
}

CFightGuideLayout::~CFightGuideLayout(){
	CC_SAFE_RELEASE_NULL(m_stLightNodes);
	CC_SAFE_RELEASE_NULL(m_stBorders);
}

bool CFightGuideLayout::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CFightGuideLayout::initUI(){
	CCDirector *pDirector=CCDirector::sharedDirector();
	CCSize screenSize = pDirector->getVisibleSize();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();

	setSize(screenSize);
	setPosition(screenOrigin);

	m_pClipNode = CCClippingNode::create();
	m_pClipNode->setInverted(true);
	m_pClipNode->setAlphaThreshold(1.0f);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_pDarkLayer = CCLayerColor::create(ccc4(0x00, 0x00, 0x00, 0x00));
#else
	m_pDarkLayer = CCLayerColor::create(ccc4(0x00, 0x00, 0x00, 0x80));
#endif
	m_pClipNode->addChild(m_pDarkLayer);

	m_pLightStencil = CCNode::create();
	m_pClipNode->setStencil(m_pLightStencil);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");
	m_pTipImg = ImageView::create();
	m_pTipImg->setPosition(ccp(screenSize.width/2, screenSize.height*2/3));

	addChild(m_pTipImg, 100);
	addNode(m_pClipNode);
}

void CFightGuideLayout::onEnter() {
	UILayout::onEnter();
	setTouchEnabled(true);
}

void CFightGuideLayout::onExit() {
	UILayout::onExit();
	setTouchEnabled(false);
}

bool CFightGuideLayout::hitTest(const CCPoint &pt){
	bool bRet = true;
	CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCObject* pstObj;
	CCARRAY_FOREACH(m_stLightNodes, pstObj){
		CCSprite* pNode = dynamic_cast<CCSprite*>(pstObj);
		CCSize size = pNode->getContentSize();
		float originX = pNode->getPositionX() - size.width / 2;
		float originY = pNode->getPositionY() - size.height / 2;
		CCRect rect = CCRectMake(originX, originY, size.width, size.height);
		if(rect.containsPoint(pt - screenOrigin)){
			bRet = false;
			return bRet;
		}
	}
	return bRet;
}

void CFightGuideLayout::show(const CCRect &rect){
	CCSprite* lightNode = CCSprite::create("ui_v2/kong.png");
	lightNode->setScale(rect.size.width/lightNode->getContentSize().width);
	lightNode->setPosition(ccp(rect.getMidX(), rect.getMidY()));
	m_pLightStencil->addChild(lightNode);
	m_stLightNodes->addObject(lightNode);

	CCScale9Sprite* pBorder = CCScale9Sprite::create(CCRectMake(20, 20, 80, 80), "ui_bg/rect.png");
	pBorder->setPosition(ccp(rect.getMidX(), rect.getMidY()));
	pBorder->setContentSize(rect.size + CCSize(22, 22));
	m_stBorders->addObject(pBorder);
	addNode(pBorder);

	int step = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(step);
	if(stCfg->desc() == "null"){
		m_pTipImg->setVisible(false);
	}else{
		m_pTipImg->setVisible(true);
		string str = "fight/" + stCfg->desc() + ".png";
		m_pTipImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
	}
}

void CFightGuideLayout::showEff(CCPoint stPos){
	CCArmature* pArm = CCArmature::create("ut00043");
	pArm->setPosition(stPos);
	addNode(pArm, 0, 101);
	if(P_FIGHT_GUIDE_CTRL->getDirect() == UP_TYPE){
		pArm->getAnimation()->play("shang", -1, -1, 1);
	}else if(P_FIGHT_GUIDE_CTRL->getDirect() == DOWN_TYPE){
		pArm->getAnimation()->play("xia", -1, -1, 1);
	}
}

void CFightGuideLayout::showFireEff(CCPoint stPos){
	CCArmature* pArm = CCArmature::create("ut00046");
	pArm->setPosition(stPos);
	addNode(pArm, 0, 301);
	pArm->getAnimation()->play("fly", -1, -1, 1);
}

void CFightGuideLayout::showShootTip(CCPoint stPos){
	ImageView* pImg = ImageView::create();
	pImg->loadTexture("ui_v2/long_click.png");
	pImg->setPosition(stPos + ccp(0, 50));
	addChild(pImg, 0, 201);
}

void CFightGuideLayout::clearAllStencil(){
	m_pLightStencil->removeAllChildrenWithCleanup(true);
	m_stLightNodes->removeAllObjects();

	CCObject* pstObj;
	CCARRAY_FOREACH(m_stBorders, pstObj){
		CCNode* pNode = dynamic_cast<CCNode*>(pstObj);
		if(pNode && pNode->getParent()){
			pNode->removeFromParentAndCleanup(true);
		}
	}

	m_stBorders->removeAllObjects();

	if(getNodeByTag(101)){
		removeNodeByTag(101);
	}

	if(getNodeByTag(301)){
		removeNodeByTag(301);
	}

	if(getChildByTag(201)){
		removeChildByTag(201, true);
	}
}


