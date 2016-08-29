#include "Badge.h"

#include "defines.h"

Badge::Badge()
:m_pBadgeImg(NULL)
,m_pBadgeNumTxt(NULL)
,m_uStat(BADGE_STAT_NUM)
{
}

Badge::~Badge()
{
}

Badge* Badge::create() {
	Badge *pRet = new Badge();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool Badge::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!Widget::init());

		m_pBadgeImg = UIImageView::create();
		m_pBadgeImg->loadTexture("share/badge_x.png", UI_TEX_TYPE_PLIST);
		addChild(m_pBadgeImg);

		m_pBadgeNumTxt = UILabel::create();
		m_pBadgeNumTxt->setFontName("fonts/tgamefont.ttf");
		m_pBadgeNumTxt->setFontSize(20);
		addChild(m_pBadgeNumTxt);

		bRet = true;
	} while(0);

	return bRet;
}

void Badge::setStat(BadgeStat stat) {
	m_uStat = stat;

	if(m_uStat == BADGE_STAT_NUM) {
		m_pBadgeImg->loadTexture("share/badge_x.png", UI_TEX_TYPE_PLIST);
	} else if(m_uStat == BADGE_STAT_POINT) {
		m_pBadgeImg->loadTexture("share/badge.png", UI_TEX_TYPE_PLIST);
	}

	m_pBadgeNumTxt->setEnabled(m_uStat == BADGE_STAT_NUM);
}

void Badge::setNum(unsigned int num) {
	if(m_uStat == BADGE_STAT_NUM) {
		if(num == 0) {
			m_pBadgeNumTxt->setText("0");
		} else if(num > 0 && num < BADGE_CEILING_NUM) {
			m_pBadgeNumTxt->setText(intToString(num));
		} else {
			m_pBadgeNumTxt->setText("!");
		}
	}
}
