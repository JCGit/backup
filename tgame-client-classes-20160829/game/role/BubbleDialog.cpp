#include "BubbleDialog.h"
#include "ToolUtils.h"
#include "CCColorUtil.h"

#define MAX_WORD_NUM 12

CBubbleDialog::CBubbleDialog(){
	m_uIdx = 0;
	m_stTxt = "";
}

CBubbleDialog::~CBubbleDialog(){

}

bool CBubbleDialog::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/bubble_dialog_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pUpImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("up_img"));
		m_pMidImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("mid_img"));
		m_pDownImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("down_img"));
		m_pTxt = dynamic_cast<UILabel*>(m_pMidImg->getChildByName("txt"));
		m_pTxt->setText("");

		CCColorUtil::addStroke(m_pTxt, 1.0f, ccWHITE);

		setAnchorPoint(ccp(0.5, 0));

		bRet = true;
	} while(0);

	return bRet;
}

void CBubbleDialog::setString(string str){
	m_stTxt = str;
	m_uIdx = 0;
	m_pTxt->setText("");
	m_pMidImg->setSize(CCSizeMake(328, 30));
	m_pUpImg->setPosition(ccp(m_pUpImg->getPositionX(), m_pMidImg->getPositionY() + m_pMidImg->getSize().height - 7));

	//½âÎö×Ö·û´®
	CToolUtils::parseUTFStr(m_stTxt, m_stWords);

	float duration = 0.5;

	m_pRootLayout->setVisible(false);
	CCFiniteTimeAction* stepAction = CCCallFunc::create(this, callfunc_selector(CBubbleDialog::showTxtByStep));
	CCSequence* stAction = CCSequence::create(CCShow::create(), stepAction, NULL);
	m_pRootLayout->getVirtualRenderer()->runAction(stAction);
}

void CBubbleDialog::showTxtByStep(){
	this->schedule(schedule_selector(CBubbleDialog::stepUpdate), 0.15f);
}

void CBubbleDialog::stepUpdate(float dt){
	string words = "";

	if(m_uIdx < m_stWords.size()) {
		for(unsigned int i = 0; i <= m_uIdx; i++) {
			words.append(m_stWords[i]);
		}

		m_uIdx++;
	} 

	m_pTxt->setText(CToolUtils::splitUTFStr(words, MAX_WORD_NUM));

	sizeChange();

	if(m_uIdx >= m_stWords.size()) {
		this->unschedule(schedule_selector(CBubbleDialog::stepUpdate));

		CCFiniteTimeAction* hideAction = CCCallFunc::create(this, callfunc_selector(CBubbleDialog::hideDialog));
		CCActionInterval *pAction = CCEaseBackIn::create(CCMoveBy::create(0.8f, ccp(0, 5)));
		CCSequence* stAction = CCSequence::create(CCDelayTime::create(0.5f), pAction, CCHide::create(), hideAction, NULL);
		m_pRootLayout->getVirtualRenderer()->runAction(stAction);
	}
}

void CBubbleDialog::hideDialog(){
	dispatchEvent(BUBBLE_OVER);
}

void CBubbleDialog::sizeChange(){
	m_pMidImg->setSize(CCSizeMake(327, m_pTxt->getSize().height));
	m_pUpImg->setPosition(ccp(m_pUpImg->getPositionX(), m_pMidImg->getPositionY() + m_pMidImg->getSize().height - 7));
}

void CBubbleDialog::addBubbleEventListener(CCObject *pListener, SEL_BUBBLE pSelector) {
	m_pListener = pListener;
	m_pfnSelector = pSelector;
}

void CBubbleDialog::dispatchEvent(BubbleEventType type) {
	if(m_pListener != NULL && m_pfnSelector != NULL) {
		(m_pListener->*m_pfnSelector)(this, type);
	}
}
