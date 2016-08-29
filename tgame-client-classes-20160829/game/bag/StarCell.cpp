#include "StarCell.h"

#include "defines.h"

StarCell::StarCell()
:m_pDarkImg(NULL)
,m_pLightImg(NULL)
,m_pLevelTxt(NULL)
,m_uState(STATE_DARK)
,m_uLevel(0)
{
}

StarCell::~StarCell()
{
}

StarCell* StarCell::create() {
	StarCell *pRet = new StarCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

StarCell* StarCell::create(UILayout *pRefLayout) {
	StarCell *pRet = new StarCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool StarCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());
		
		bRet = true;
	} while(0);

	return bRet;
}

bool StarCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_pDarkImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("star_dark_img"));

		m_pLightImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("star_light_img"));

		m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		setState(STATE_DARK);
		setLevel(0);

		bRet = true;
	} while(0);

	return bRet;
}

unsigned int StarCell::getState() {
	return m_uState;
}

void StarCell::setState(unsigned int uState) {
	m_uState = uState;

	m_pDarkImg->setVisible(m_uState == STATE_DARK);
	m_pLightImg->setVisible(m_uState == STATE_LIGHT);
	m_pLevelTxt->setVisible(m_uLevel != 0 && m_uState != STATE_DARK);
	setTouchEnabled(m_uState == STATE_LIGHT);
}

void StarCell::setLevel(unsigned int uLevel) {
	m_uLevel = uLevel;

	m_pLevelTxt->setText(intToString(m_uLevel));
	m_pLevelTxt->setVisible(m_uLevel != 0 && m_uState != STATE_DARK);
}
