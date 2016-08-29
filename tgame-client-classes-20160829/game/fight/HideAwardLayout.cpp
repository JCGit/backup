#include "HideAwardLayout.h"
#include "LevelControl.h"
#include "FightObj.h"
#include "ItemUtils.h"
#include "defines.h"

CHideAwardLayout::CHideAwardLayout(){
	m_pLayer = NULL;

	m_stAniArr = CCArray::create();
	m_stAniArr->retain();

	m_uAwardIdx= 0;
	m_uAwardSize = 0;
}

CHideAwardLayout::~CHideAwardLayout(){
	CC_SAFE_RELEASE_NULL(m_stAniArr);
}

bool CHideAwardLayout::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

		float largeOffset = 100.0f;
		CCSize largeSize = CCSizeMake(size.width + largeOffset * 2, size.height + largeOffset * 2);

		setSize(size);
		setPosition(point);

		m_pLayer = UILayout::create();
		m_pLayer->setTouchEnabled(true);
		m_pLayer->setBackGroundColor(ccc3(0x00, 0x00, 0x00));
		m_pLayer->setBackGroundColorType(LAYOUT_COLOR_SOLID);
		m_pLayer->setBackGroundColorOpacity(0x70);
		m_pLayer->setSize(largeSize);
		addChild(m_pLayer);

		bRet = true;
	} while(0);

	return bRet;
}

void CHideAwardLayout::openAwardBox(){
	while (m_stAniArr->count() > 0){
		CCArmature* pFirst = dynamic_cast<CCArmature*>(m_stAniArr->objectAtIndex(0));
		if(pFirst){
			pFirst->removeFromParentAndCleanup(true);
		}
		m_stAniArr->removeObjectAtIndex(0);
	}

	vmsg::CSWarMapEndRsp stAwardRsp = P_FIGHT_OBJ->getAwardInfo();
	m_uAwardSize = stAwardRsp.hidenawards_size();
	m_uAwardIdx = 0;

	if(m_uAwardSize <= 0){
		return;
	}

	addOneArm();
}

void CHideAwardLayout::addOneArm(){
	if(m_uAwardIdx >= m_uAwardSize){
		return;
	}

	CCArmature* pArm = CCArmature::create("ut00092");
	m_pLayer->addNode(pArm);
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CHideAwardLayout::handleArmComplete));
	pArm->getAnimation()->play("fly", -1, -1, 0);

	CCSize size = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint stPos = size/2;
	pArm->setPosition(stPos + ccp(0, -pArm->getContentSize().height*m_uAwardIdx/2));

	m_stAniArr->addObject(pArm);

	m_uAwardIdx++;
}

void CHideAwardLayout::addOneTxt(CCArmature* pArm){
	Label* pLabel = Label::create();
	pLabel->setAnchorPoint(ccp(0, 0.5));
	pLabel->setFontName("fonts/tgamefont.ttf");
	pLabel->setFontSize(28);
	pLabel->setPosition(ccp(60, -5));
	pArm->addChild(pLabel, 20);

	unsigned int uIdx = m_uAwardIdx - 1;
	if(uIdx >= m_uAwardSize){
		return;
	}

	vmsg::CSWarMapEndRsp stAwardRsp = P_FIGHT_OBJ->getAwardInfo();
	vmsg::CSAwardElement stAwardElm = stAwardRsp.hidenawards(uIdx);
	string nameStr = ItemUtils::getItemName(stAwardElm.elementtype(), stAwardElm.elementid());
	nameStr += "x" + intToString(stAwardElm.elementcnt());
	pLabel->setText(nameStr);
}

void CHideAwardLayout::handleArmComplete(CCArmature * armature, MovementEventType type, const char * name){
	string nameStr = name;
	if(type == COMPLETE){
		if(nameStr == "fly"){
			nextArm(armature);
		}
	}	
}

void CHideAwardLayout::nextArm(CCArmature* pArm){
	pArm->getAnimation()->play("fly_xh", -1, -1, 1);
	addOneTxt(pArm);

	if(m_uAwardIdx >= m_uAwardSize){
		scheduleOnce(schedule_selector(CHideAwardLayout::onShowEnd), 2.0f);
		return;
	}

	//ÏÂÒ»Ìõ½±Àø
	addOneArm();
}

void CHideAwardLayout::onShowEnd(float dt){
	while (m_stAniArr->count() > 0){
		CCArmature* pFirst = dynamic_cast<CCArmature*>(m_stAniArr->objectAtIndex(0));
		if(pFirst){
			pFirst->removeFromParentAndCleanup(true);
		}
		m_stAniArr->removeObjectAtIndex(0);
	}

	m_uAwardSize = 0;
	m_uAwardIdx = 0;

	this->removeFromParentAndCleanup(true);

	P_LEVEL_CTL->getResultPanel()->open();
	P_LEVEL_CTL->getResultPanel()->updateUI();
}



