#include "UFOCell.h"

#include "ShowRole.h"

UFOCell::UFOCell()
:m_pTrailFlameArmture(NULL)
,m_pBackParticleEff(NULL)
,m_pUFOImg(NULL)
,m_pFrontParticleEff(NULL)
,m_pRole(NULL)
,m_bSelected(false)
{
}

UFOCell::~UFOCell()
{
	
}

UFOCell* UFOCell::create() {
	UFOCell *pRet = new UFOCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool UFOCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pRootLayout = UILayout::create();
		CC_SAFE_RETAIN(m_pRootLayout);
		addChild(m_pRootLayout);

		/*m_pTrailFlameArmture = CCArmature::create("ut00026");
		m_pRootLayout->addNode(m_pTrailFlameArmture);
		m_pTrailFlameArmture->getAnimation()->play("fly", -1, -1, 1);*/

		//m_pBackParticleEff = CCParticleSystemQuad::create("effect/ut00026/ut00026_lizi_houshu.plist");

		//m_pFrontParticleEff = CCParticleSystemQuad::create("effect/ut00026/ut00026_lizi_qian.plist");

		//m_pRootLayout->addNode(m_pBackParticleEff);

		m_pUFOImg = UIImageView::create();
		m_pRootLayout->addChild(m_pUFOImg);
		m_pUFOImg->loadTexture("start/ufo.png", UI_TEX_TYPE_PLIST);

		//m_pRootLayout->addNode(m_pFrontParticleEff);

		/*CCActionInterval *pMove = CCMoveTo::create(1.0f, ccp(0, 8));
		CCActionInterval *pMoveBack = CCMoveTo::create(1.0f, ccp(0, 0));
		CCAction *pAction = CCRepeatForever::create(CCSequence::create(pMove, pMoveBack, NULL));

		m_pRootLayout->runAction(pAction);*/

		bRet = true;
	} while(0);

	return bRet;
}

void UFOCell::onEnter() {
	BaseLayout::onEnter();

	CCActionInterval *pMove = CCMoveTo::create(1.0f, ccp(0, 8));
	CCActionInterval *pMoveBack = CCMoveTo::create(1.0f, ccp(0, 0));
	CCAction *pAction = CCRepeatForever::create(CCSequence::create(pMove, pMoveBack, NULL));

	if(m_pRootLayout->numberOfRunningActions() > 0) {
		m_pRootLayout->stopAllActions();
	}

	m_pRootLayout->runAction(pAction);
}	

void UFOCell::updateRole(unsigned int uPlayerID) {
	if(m_pRole != NULL && m_pRole->getPlayerID() != uPlayerID) {
		m_pRole->removeFromParent();
		m_pRole = NULL;
	}

	if(m_pRole == NULL) {
		m_pRole = ShowRole::create();
		m_pRole->setPlayerID(uPlayerID);
		m_pRole->setPosition(ccp(0, 0));
		m_pRole->stop();
		m_pRootLayout->addNode(m_pRole);
	}
}

unsigned int UFOCell::getPlayerID() {
	return m_pRole->getPlayerID();
}

bool UFOCell::isSelected() {
	return m_bSelected;
}

void UFOCell::selected(bool bSelected) {
	m_bSelected = bSelected;

	if(m_pRole != NULL) {
		if(m_bSelected) {
			m_pRole->play();
		} else {
			m_pRole->stop();
		}
	}
}
