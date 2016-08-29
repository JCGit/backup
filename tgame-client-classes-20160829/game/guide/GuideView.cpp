#include "GuideView.h"

#include "GuideControl.h"
#include "game/chat/AnimSprite.h"

#define HOLDING_DELAY 10 // seconds

GuideView::GuideView()
:m_pClipNode(NULL)
,m_pDarkLayer(NULL)
,m_pLightNode(NULL)
,m_pBorder(NULL)
,m_pHitArmture(NULL)
,m_pBorderAction(NULL)
,m_bForced(true)
,m_bBorderVisible(true)
,m_bHandVisible(true)
,m_bRectShape(true)
,m_pWordsTxt(NULL)
{
}

GuideView::~GuideView()
{
	CC_SAFE_RELEASE_NULL(m_pBorderAction);
}

GuideView* GuideView::create() {
	GuideView *pRet = new GuideView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuideView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayout::init());

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
		m_pLightNode = CCDrawNode::create();
		m_pClipNode->setStencil(m_pLightNode);

		m_pBorder = CCScale9Sprite::create(CCRectMake(20, 20, 80, 80), "ui_bg/rect.png");
		
		m_pHitArmture = CCArmature::create("ut00037");
		
		addNode(m_pClipNode);
		addNode(m_pBorder);
		addNode(m_pHitArmture);
		
		CCActionInterval *pBorderSeq = CCSequence::create(CCFadeTo::create(0.8f, 0xff), CCFadeTo::create(0.8f, 0x60), NULL);
		m_pBorderAction = CCRepeatForever::create(pBorderSeq);
		m_pBorderAction->retain();

		m_pWordsTxt = CCLabelTTF::create();
		m_pWordsTxt->setFontName("fonts/tgamefont.ttf");
		m_pWordsTxt->setFontSize(24);
		m_pWordsTxt->setColor(ccc3(0xff, 0xff, 0xff));
		m_pWordsTxt->setAnchorPoint(ccp(0, 1));
		m_pWordsTxt->setPosition(ccp(16, screenSize.height - 16));
		addNode(m_pWordsTxt);
		m_pWordsTxt->setDimensions(CCSizeMake(920, 96));
		
		bRet = true;
	} while(0);

	return bRet;
}

void GuideView::onEnter() {
	UILayout::onEnter();
	setTouchEnabled(true);
}

void GuideView::onExit() {
	UILayout::onExit();
	setTouchEnabled(false);
}

bool GuideView::hitTest(const CCPoint &pt) {
	bool bRet = true; // 默认其上的任何点都不可穿透
	CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSize size = m_pLightNode->getContentSize();
	float originX = m_pLightNode->getPositionX() - size.width / 2;
	float originY = m_pLightNode->getPositionY() - size.height / 2;
	CCRect rect = CCRectMake(originX, originY, size.width, size.height);

	if(m_bForced) {
		if(size.equals(CCSizeZero)) { // 大小为0
			bRet = true;
		} else {
			if(m_bRectShape && rect.containsPoint(pt - screenOrigin)) { // 矩形
				bRet = false;
			} else if(!m_bRectShape && m_pLightNode->getPosition().getDistance(pt - screenOrigin) <= size.width / 2) { // 圆形
				bRet = false;
			}
		}
	} else {
		bRet = false; // 相当于isTouchEnabled()为false
	}
	
	return bRet;
}

bool GuideView::isForced() {
	return m_bForced;
}

void GuideView::forced(bool bForced) {
	m_bForced = bForced;
}

bool GuideView::isHandVisible() {
	return m_bHandVisible;
}

void GuideView::setHandVisible(bool bHandVisible) {
	m_bHandVisible = bHandVisible;
}

bool GuideView::isBorderVisible() {
	return m_bBorderVisible;
}

void GuideView::setBorderVisible(bool bBorderVisible) {
	m_bBorderVisible = bBorderVisible;
}

void GuideView::show(const CCRect &rect, const std::string &shape) {
	bool bIsSizeNotZero = !rect.size.equals(CCSizeZero);

	ccColor4F fillColor = ccc4f(1, 1, 0, 1);
	ccColor4F borderColor = ccc4f(0, 1, 0, 1);

	m_pLightNode->clear();

	if(shape == "circle") {
		// 圆形
		float radius = rect.size.width / 2; // 半径
		const unsigned int vertexNum = 240; // 顶点数
		float angle = 2.0f * (float)M_PI / vertexNum; // 两个顶点与中心的夹角
		CCPoint circle[vertexNum];

		for(unsigned int i = 0; i < vertexNum; i++) {
			float radian = i * angle;
			circle[i].x = radius * cosf(radian);
			circle[i].y = radius * sinf(radian);
		}

		m_pLightNode->drawPolygon(circle, vertexNum, fillColor, 1, borderColor);
		m_bRectShape = false;
	} else {
		// 矩形
		float w = rect.size.width / 2;
		float h = rect.size.height / 2;
		CCPoint rectPt[4]={ccp(-w, h),ccp(w, h),ccp(w, -h),ccp(-w, -h)};

		m_pLightNode->drawPolygon(rectPt, 4, fillColor, 1, borderColor);
		m_bRectShape = true;
	}

	m_pLightNode->setPosition(ccp(rect.getMidX(), rect.getMidY()));
	m_pLightNode->setContentSize(rect.size);

	m_pBorder->setPosition(ccp(rect.getMidX(), rect.getMidY()));
	m_pBorder->setContentSize(CCSizeMake(rect.size.width + 22, rect.size.height + 22));

	if(m_pBorder->numberOfRunningActions() > 0) {
		m_pBorder->stopAllActions();
	}

	m_pBorder->runAction(m_pBorderAction);
	m_pHitArmture->setPosition(ccp(rect.getMidX(), rect.getMidY()));
	m_pHitArmture->getAnimation()->play("fly", -1, -1, 1);

	/*if(direction == "left") {
		m_pHitArmture->setPosition(ccp(rect.getMaxX(), rect.getMinY()));
	} else if(direction == "right") {
		m_pHitArmture->setPosition(ccp(rect.getMaxX(), rect.getMinY()));
	} else if(direction == "up") {
		m_pHitArmture->setPosition(ccp(rect.getMaxX(), rect.getMinY()));
	} else if(direction == "down") {
		m_pHitArmture->setPosition(ccp(rect.getMaxX(), rect.getMinY()));
	}*/
	
	//m_pWordsTxt->setString(m_stWords.c_str());

	m_pClipNode->setVisible(m_bForced);
	m_pBorder->setVisible(bIsSizeNotZero && m_bBorderVisible);
	m_pHitArmture->setVisible(bIsSizeNotZero && m_bHandVisible);
}

void GuideView::hide() {
	m_pLightNode->clear();
	m_pLightNode->setContentSize(CCSizeZero);
	m_pClipNode->setVisible(false);
	m_pBorder->setVisible(false);
	m_pHitArmture->setVisible(false);
}
